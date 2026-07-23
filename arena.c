#include "arena.h"

void arena_init(arena *self, size_t cap, wa_t *wa) {
    self->offset = 0;
    self->cap = cap;
    self->arena = wa;
}

void *arena_alloc(arena *self, size_t s, size_t alignment) {
    ARENA_ASSERT(alignment != 0);
    ARENA_ASSERT((alignment & (alignment - 1)) == 0);
    if (!self->arena) {
        return NULL;
    }

    size_t aligned_offset = (self->offset + alignment - 1) & \
        ~(alignment - 1);

    if (aligned_offset + s > self->cap) {
        return NULL;
    }

    void *ptr = (void*)(self->arena + aligned_offset);
    self->offset = aligned_offset + s;
    return ptr;
}

void arena_free(arena *self) {
    self->offset = 0;
}
