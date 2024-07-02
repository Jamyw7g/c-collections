#ifndef _VEC_CONTAINER_H_
#define _VEC_CONTAINER_H_

#include <stddef.h>

typedef struct container {
    void **buf;
    size_t cap;
} container;

void container_init(container *con, size_t cap);
void* container_get(container *con, size_t idx);
void container_set(container *con, size_t idx, void *val);
void container_grow(container *con);
void container_destroy(container *con);

#endif
