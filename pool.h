#ifndef POOL_H
#define POOL_H

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#ifndef POOL_ASSERT
    #define POOL_ASSERT assert
#endif

typedef uint8_t pool_wa_t;

struct pool_chunk_s {
    struct pool_chunk_s *next;
    pool_wa_t data[];
};
typedef struct pool_chunk_s pool_chunk;

struct pool_s {
    pool_wa_t *wa;
    pool_chunk *top;
    size_t size;
    size_t cap;
    size_t n;
};
typedef struct pool_s pool;

void pool_init_size(pool *self, pool_wa_t *wa, size_t size, size_t chunk_size);
void pool_init_number(pool *self, pool_wa_t *wa, size_t size, size_t n);
void pool_init_entries(pool *self, pool_wa_t *wa, size_t chunk_size, size_t n);
void *pool_alloc(pool *self);
void pool_free(pool *self, void *ptr);
size_t pool_n(pool *self);

#endif
