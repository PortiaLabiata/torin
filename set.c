#include "set.h"
#include "pool.h"
#include <assert.h>
#include <string.h>

typedef struct hash_entry_header_s {
    hash_t hash;
    struct hash_entry_header_s *next;
} hash_entry_header;

void set_init(set *self, hash_func func, wa_t *wa, size_t entry_size, size_t cap) {
    pool_init_entries(&self->data, wa, entry_size+sizeof(hash_entry_header), cap+1);
    self->first = NULL;

    hash_entry_header *first_entry = pool_alloc(&self->data);
    if (!first_entry) {
        return;
    }

    first_entry->next = NULL;
    first_entry->hash = 0;

    self->first = first_entry;
    self->type_size = entry_size;
    self->func = func;
}

bool set_push(set *self, void *value) {
    if (!self->first) {
        return false;
    }

    hash_entry_header *hdr = self->first;
    while (hdr->next) {
        hdr = hdr->next;
    }

    hash_entry_header *new_hdr = pool_alloc(&self->data);
    if (!new_hdr) {
        return false;
    }

    new_hdr->hash = self->func(value, self->type_size);
    new_hdr->next = NULL;

    uint8_t *dst = (uint8_t*)&new_hdr->next + 1;
    memcpy(dst, value, self->type_size);

    hdr->next = new_hdr;
    return true;
}

bool set_contains(set *self, void *value) {
    hash_entry_header *hdr = ((hash_entry_header*)self->first)->next;
    hash_t hash = self->func(value, self->type_size);
    while (hdr) {
        if (hdr->hash == hash) {
            return true;
        }
        hdr = hdr->next;
    }
    return false;
}
