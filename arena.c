#include "arena.h"
#include <stddef.h>

void arena_init(arena *self, size_t cap, wa_t *wa) {
    self->offset_bytes = 0;
    self->cap_bytes = cap;
    self->wa = wa;
}

void *arena_alloc(arena *self, size_t s, size_t alignment) {
    ARENA_ASSERT(alignment != 0);
    ARENA_ASSERT((alignment & (alignment - 1)) == 0);
    if (!self->wa) {
        return NULL;
    }

    size_t aligned_offset = (self->offset_bytes + alignment - 1) & \
        ~(alignment - 1);

    if (aligned_offset + s > self->cap_bytes) {
        return NULL;
    }

    void *ptr = (void*)(self->wa + aligned_offset);
    self->offset_bytes = aligned_offset + s;
    return ptr;
}

void arena_dealloc(arena *self) {
    self->offset_bytes = 0;
}

size_t arena_free(arena *self) {
    return self->cap_bytes - self->offset_bytes; 
}
