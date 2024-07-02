#ifndef _VEC_DEQUE_H_
#define _VEC_DEQUE_H_

#include "vec_container.h"

typedef void(*drop_t)(void *);

typedef struct vec_deque {
    size_t len;
    size_t head;
    container container;
    drop_t drop;
} vec_deque;

void deque_init(vec_deque *que, size_t cap, drop_t drop);
void deque_destroy(vec_deque *que);
int deque_full(vec_deque *que);
int deque_empty(vec_deque *que);
void deque_push_front(vec_deque *que, void *val);
void deque_push_back(vec_deque *que, void *val);
void *deque_pop_front(vec_deque *que);
void *deque_pop_back(vec_deque *que);

#endif
