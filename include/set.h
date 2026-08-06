#ifndef SET_H
#define SET_H

#include <stdint.h>
#include <stddef.h>

#include "common.h"
#include "pool.h"

struct set_s {
    pool data;
    size_t type_size;
    hash_func func;
    void *first;
};
typedef struct set_s set;

void set_init(set *self, hash_func func, wa_t *wa, size_t entry_size, size_t cap);
bool set_push(set *self, void *value);
bool set_contains(set *self, void *value);

#endif
