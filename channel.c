#include "channel.h"
#include <stdlib.h>
#include <stdio.h>

void channel_init(channel *ch, drop_t drop)
{
    pthread_mutex_init(&ch->mutex, NULL);
    pthread_cond_init(&ch->cond, NULL);
    vec_deque *que = calloc(1, sizeof(vec_deque));
    deque_init(que, 0, drop);
    ch->deque = que;
}

void channel_send(channel *ch, void *val)
{
    pthread_mutex_lock(&ch->mutex);
    if (ch->deque) {
        deque_push_back(ch->deque, val);
        pthread_cond_broadcast(&ch->cond);
    }
    pthread_mutex_unlock(&ch->mutex);
}

void *channel_recv(channel *ch)
{
    pthread_mutex_lock(&ch->mutex);
    void *ret = NULL;
    for (;;) {
        // channel cancelled
        if (!ch->deque) {
            break;
        }
        if (!deque_empty(ch->deque)) {
            ret = deque_pop_front(ch->deque);
            break;
        }
        pthread_cond_wait(&ch->cond, &ch->mutex);
    }
    pthread_mutex_unlock(&ch->mutex);
    return ret;
}

void channel_cancel(channel *ch)
{
    pthread_mutex_lock(&ch->mutex);
    if (ch->deque) {
        deque_destroy(ch->deque);
        ch->deque = NULL;
        pthread_cond_broadcast(&ch->cond);
    }
    pthread_mutex_unlock(&ch->mutex);
}

void channel_destroy(channel *ch)
{
    pthread_mutex_lock(&ch->mutex);
    if (ch->deque) {
        deque_destroy(ch->deque);
        ch->deque = NULL;
    }
    pthread_mutex_unlock(&ch->mutex);
    pthread_mutex_destroy(&ch->mutex);
    pthread_cond_destroy(&ch->cond);
}