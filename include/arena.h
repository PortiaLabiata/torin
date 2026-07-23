#ifndef ARENA_H
#define ARENA_H

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#ifndef ARENA_ASSERT
    #define ARENA_ASSERT assert
#endif

struct arena_s {
    size_t offset;
    size_t cap;
    uint8_t *arena;
};

typedef struct arena_s arena;
typedef uint8_t wa_t;

void arena_init(arena *self, size_t cap, wa_t *wa);
void *arena_alloc(arena *self, size_t s, size_t alignment);
void arena_free(arena *self);

#endif
