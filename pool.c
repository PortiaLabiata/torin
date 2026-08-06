#include "pool.h"

static void _pool_init(pool *self, wa_t *wa, size_t wa_size, size_t chunk_size) {
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
    self->cap_chunks = wa_size / chunk_size;
    self->used_chunks = 0;
    self->chunk_size_bytes = chunk_size;
}

void pool_init_size(pool *self, wa_t *wa, size_t size, size_t chunk_size) {
    _pool_init(self, wa, size, chunk_size);
}

void pool_init_number(pool *self, wa_t *wa, size_t size, size_t n) {
    _pool_init(self, wa, size, size / n);
}

void pool_init_entries(pool *self, wa_t *wa, size_t chunk_size, size_t n) {
    _pool_init(self, wa, chunk_size * n, chunk_size);
}

void *pool_alloc(pool *self) {
    if (!self->top) {
        return NULL;
    }

    void *ptr = self->top;
    self->top = self->top->next;
    self->used_chunks++;
    return ptr;
}

void pool_dealloc(pool *self, void *ptr) {
    POOL_ASSERT((wa_t*)ptr >= self->wa && \
                (wa_t*)ptr <= self->wa + (self->cap_chunks * self->chunk_size_bytes));
    POOL_ASSERT(((wa_t*)ptr - self->wa) % self->chunk_size_bytes == 0);

    pool_chunk *chunk = ptr;
    chunk->next = self->top;
    self->top = chunk;
    self->used_chunks--;
}

size_t pool_free(pool *self) {
    return self->used_chunks;
}
