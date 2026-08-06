#include "set.h"
#include "common.h"
#include "fnv1a.h"
#include "utest.h"
#include <stdint.h>

#define MAKE_SET(size) \
    static const size_t WA_SIZE = size*sizeof(uint8_t); \
    wa_t wa[WA_SIZE]; \
    set s; \
    set_init(&s, fnv1a, wa, sizeof(uint8_t), size);

UTEST(set, creation) {
    MAKE_SET(16);
    ASSERT_NE(s.first, NULL);
    ASSERT_EQ(s.type_size, sizeof(uint8_t));
}

typedef struct hash_entry_test_s {
    hash_t hash;
    struct hash_entry_test_s *next;
    uint8_t value;
} hash_entry_test;

UTEST(set, push) {
    MAKE_SET(16);

    uint8_t value = 0xDE;
    ASSERT_TRUE(set_push(&s, &value));

    hash_entry_test *entry = ((hash_entry_test*)s.first)->next;
    ASSERT_EQ(entry->next, NULL);
    ASSERT_EQ(entry->hash, fnv1a(&value, 1));
    ASSERT_EQ(entry->value, 0xDE);
}

UTEST_MAIN()
