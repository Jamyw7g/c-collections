#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include <pthread.h>
#include "vec_deque.h"

typedef struct channel {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    vec_deque *deque;
} channel;


void channel_init(channel *ch, drop_t drop);
void channel_send(channel *ch, void *val);
void *channel_recv(channel *ch);
void channel_cancel(channel *ch);
void channel_destroy(channel *ch);

#endif