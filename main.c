#include "vec_deque.h"
#include "channel.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


void *sender(void *_ch)
{
    channel *ch = (channel *)_ch;
    for (int i = 0; i < 32; i++) {
        int *val = (int *)malloc(sizeof(int));
        *val = i * i;
        channel_send(ch, val);
    }
    return NULL;
}


int main() {
    // vec_deque que;
    // deque_init(&que, 0, free);
    // for (int i = 0; i < 1024; i++) {
    //     int *val = malloc(sizeof(int));
    //     *val = i * i;
    //     deque_push_front(&que, val);
    // }
    // for (int i = 0; i < 32; i++) {
    //     int *res = deque_pop_front(&que);
    //     if (res) {
    //         printf("val = %d \n", *res);
    //         free(res);
    //     }
    // }
    // deque_destroy(&que);
    channel ch;
    channel_init(&ch, free);
    pthread_t th1, th2;
    pthread_create(&th1, NULL, sender, &ch);
    pthread_create(&th2, NULL, sender, &ch);

    for (int i = 0; i < 32; i++) {
        int *val = (int *)channel_recv(&ch);
        if (val) {
            printf("val = %d\n", *val);
            free(val);
        }
    }

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    channel_destroy(&ch);
    
    return 0;
}
