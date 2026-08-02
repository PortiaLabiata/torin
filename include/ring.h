#ifndef RING_H
#define RING_H

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#ifndef RING_ASSERT
    #define RING_ASSERT assert
#endif

typedef uint8_t ring_wa_t;
struct ring_s {
    size_t left;
    size_t right;
    size_t size_entries;
    size_t cap_entries;
    size_t type_size_bytes;
    ring_wa_t *wa;
};
typedef struct ring_s ring;

void ring_init(ring *self, ring_wa_t *wa, size_t cap, size_t type_size);
bool ring_push(ring *self, void *value);
bool ring_pop(ring *self, void *value);
bool ring_peek(ring *self, void *value);

#endif
