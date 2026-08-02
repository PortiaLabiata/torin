#include "include/ring.h"
#include <stdint.h>
#include <string.h>

void ring_init(ring *self, ring_wa_t *wa, size_t cap, size_t type_size) {
    RING_ASSERT(self);
    RING_ASSERT(cap);
    RING_ASSERT(wa);
    RING_ASSERT(type_size);

    self->left = 0;
    self->right = 0;
    self->size_entries = 0;
    self->cap_entries = cap;
    self->wa = wa;
    self->type_size_bytes = type_size;
}

bool ring_push(ring *self, void *value) {
    RING_ASSERT(self);
    RING_ASSERT(value);

    if (self->size_entries >= self->cap_entries) {
        return false;
    } 

    void *ptr = &self->wa[self->right * self->type_size_bytes];
    memcpy(ptr, value, self->type_size_bytes);

    self->right = (self->right + 1) % self->cap_entries;
    self->size_entries++;
    return true;
}

bool ring_pop(ring *self, void *value) {
    RING_ASSERT(self);
    RING_ASSERT(value);

    if (self->size_entries == 0) {
        return false;
    }

    memcpy(value, &self->wa[self->left * self->type_size_bytes], self->type_size_bytes);
    self->left = (self->left + 1) % self->cap_entries;
    self->size_entries--;
    return true;
}

bool ring_peek(ring *self, void *value) {
    RING_ASSERT(self);
    RING_ASSERT(value);

    if (self->size_entries == 0) {
        return false;
    }

    *(uint8_t*)value = self->wa[self->left * self->type_size_bytes];
    return true;
}
