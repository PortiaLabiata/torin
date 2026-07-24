#ifndef POOL_H
#define POOL_H

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include "common.h"

#ifndef POOL_ASSERT
    #define POOL_ASSERT assert
#endif

struct pool_chunk_s {
    struct pool_chunk_s *next;
    wa_t data[];
};
typedef struct pool_chunk_s pool_chunk;

struct pool_s {
    wa_t *wa;
    pool_chunk *top;
    size_t size_bytes;
    size_t cap_chunks;
    size_t chunk_size_bytes;
    size_t used_chunks;
};
typedef struct pool_s pool;

void pool_init_size(pool *self, wa_t *wa, size_t size, size_t chunk_size);
void pool_init_number(pool *self, wa_t *wa, size_t size, size_t n);
void pool_init_entries(pool *self, wa_t *wa, size_t chunk_size, size_t n);
void *pool_alloc(pool *self);
void pool_dealloc(pool *self, void *ptr);
size_t pool_free(pool *self);

#endif
