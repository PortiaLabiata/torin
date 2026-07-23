#include "pool.h"

static void _pool_init(pool *self, pool_wa_t *wa, size_t wa_size, size_t chunk_size) {
    POOL_ASSERT(wa);
    POOL_ASSERT(wa_size > 0);
    POOL_ASSERT(chunk_size > 0);
    POOL_ASSERT(wa_size % chunk_size == 0);
    POOL_ASSERT(chunk_size >= sizeof(pool_chunk));

    pool_chunk *chunks = (pool_chunk*)wa;
    size_t i = 0;
    for (; i < (wa_size / chunk_size - 1); i++) {
        chunks[i].next = &chunks[i+1]; 
    }
    chunks[i].next = NULL;

    self->wa = wa;
    self->top = &chunks[0];
    self->size = wa_size;
    self->cap = wa_size / chunk_size;
    self->n = 0;
}

void pool_init_size(pool *self, pool_wa_t *wa, size_t size, size_t chunk_size) {
    _pool_init(self, wa, size, chunk_size);
}

void pool_init_number(pool *self, pool_wa_t *wa, size_t size, size_t n) {
    _pool_init(self, wa, size, size / n);
}

void pool_init_entries(pool *self, pool_wa_t *wa, size_t chunk_size, size_t n) {
    _pool_init(self, wa, chunk_size * n, n);
}

void *pool_alloc(pool *self) {
    if (!self->top) {
        return NULL;
    }

    void *ptr = self->top;
    self->top = self->top->next;
    self->n++;
    return ptr;
}

void pool_free(pool *self, void *ptr) {
    POOL_ASSERT((pool_wa_t*)ptr >= self->wa && \
                (pool_wa_t*)ptr <= self->wa + self->size);

    pool_chunk *chunk = ptr;
    chunk->next = self->top;
    self->top = chunk;
    self->n--;
}

size_t pool_n(pool *self) {
    return self->n;
}
