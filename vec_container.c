#include "vec_container.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void container_init(container *con, size_t cap)
{
    con->cap = cap;
    con->buf = (void **)calloc(cap, sizeof(void*));
}

void* container_get(container *con, size_t idx)
{
    return con->buf[idx];
}

void container_set(container *con, size_t idx, void *val)
{
    con->buf[idx] = val;
}

void container_grow(container *con)
{
    size_t cap = con->cap ? con->cap * 2 : 1;
    void **buf = (void **)calloc(cap, sizeof(void*));
    memcpy(buf, con->buf, con->cap * sizeof(void*));
    free(con->buf);
    con->buf = buf;
    con->cap = cap;
}

void container_destroy(container *con)
{
    free(con->buf);
    con->buf = NULL;
    con->cap = 0;
}
