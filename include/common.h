#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stddef.h>

typedef uint8_t wa_t;
typedef uint32_t hash_t;
typedef hash_t (*hash_func)(void *data, size_t size);

#endif
