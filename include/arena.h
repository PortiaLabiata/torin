#ifndef ARENA_H
#define ARENA_H

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include "common.h"

#ifndef ARENA_ASSERT
    #define ARENA_ASSERT assert
#endif

struct arena_s {
    size_t offset_bytes;
    size_t cap_bytes;
    wa_t *wa;
};
typedef struct arena_s arena;

void arena_init(arena *self, size_t cap, wa_t *wa);
void *arena_alloc(arena *self, size_t s, size_t alignment);
void arena_dealloc(arena *self);
size_t arena_free(arena *self);

#endif
