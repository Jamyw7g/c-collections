#include "vec_deque.h"
#include <stdio.h>
#include <string.h>

// private function
inline
size_t wrap_add(size_t head, size_t offset, size_t cap);
inline
size_t wrap_sub(size_t head, size_t offset, size_t cap);
inline
size_t wrap_index(size_t index, size_t cap);
void deque_grow(vec_deque *que);

void deque_init(vec_deque *que, size_t cap, drop_t drop)
{
    container_init(&que->container, cap);
    que->len = 0;
    que->head = 0;
    que->drop = drop;
}

void deque_destroy(vec_deque *que)
{
    if (que->drop) {
        void *val = NULL;
        while ((val = deque_pop_front(que))) {
            que->drop(val);
        }
    }
    container_destroy(&que->container);
    que->len = 0;
    que->head = 0;
}

int deque_full(vec_deque *que)
{
    return que->len == que->container.cap;
}

int deque_empty(vec_deque *que)
{
    return que->len == 0;
}

void deque_push_front(vec_deque *que, void *val)
{
    if (deque_full(que)) {
        deque_grow(que);
    }
    que->head = wrap_sub(que->head, 1, que->container.cap);
    container_set(&que->container, que->head, val);
    que->len += 1;
}

void deque_push_back(vec_deque *que, void *val)
{
    if (deque_full(que)) {
        deque_grow(que);
    }
    container_set(&que->container, wrap_add(que->head, que->len, que->container.cap), val);
    que->len += 1;
}

void *deque_pop_front(vec_deque *que)
{
    if (deque_empty(que)) {
        return NULL;
    }
    que->len -= 1;
    void *ret = container_get(&que->container, que->head);
    que->head = wrap_add(que->head, 1, que->container.cap);
    return ret;
}

void *deque_pop_back(vec_deque *que)
{
    if (deque_empty(que)) {
        return NULL;
    }
    que->len -= 1;
    void *ret = container_get(&que->container, wrap_add(que->head, que->len, que->container.cap));
    return ret;
}

void deque_grow(vec_deque *que)
{
    size_t old_cap = que->container.cap;
    container_grow(&que->container);
    size_t new_cap = que->container.cap;

    if (que->head <= old_cap - que->len) {
        // noop
    } else {
        size_t head_len = old_cap - que->head;
        size_t tail_len = que->len - head_len;
        if (head_len > tail_len && new_cap - old_cap >= tail_len) {
            memcpy(que->container.buf + old_cap, que->container.buf, tail_len * sizeof(void*));
        } else {
            size_t new_head = new_cap - head_len;
            memmove(que->container.buf + new_head, que->container.buf + que->head, head_len * sizeof(void*));
            que->head = new_head;
        }
    }
}

inline
size_t wrap_add(size_t head, size_t offset, size_t cap)
{
    return wrap_index(head + offset, cap);
}

inline
size_t wrap_sub(size_t head, size_t offset, size_t cap)
{
    return wrap_index((head - offset + cap), cap);
}

inline
size_t wrap_index(size_t index, size_t cap)
{
    return index >= cap ? (index - cap) : index;
}
