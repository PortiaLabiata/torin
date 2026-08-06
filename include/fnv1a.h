#ifndef FNV1A_H
#define FNV1A_H

#include "common.h"
#include <stddef.h>

hash_t fnv1a(void *data, size_t size) {
    static const hash_t prime = 0x01000193;
    static const hash_t start = 0x811c9dc5;

    hash_t result = start;
    for (size_t i = 0; i < size; i++) {
        result ^= ((uint8_t*)data)[i];
        result *= prime;
    }
    return result;
}

#endif
