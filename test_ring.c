#include "common.h"
#include "utest.h"
#include <stdint.h>
#include <ring.h>
#include <string.h>

#define RING_CREATE(size_entries, type_size) \
    wa_t wa[size_entries*type_size]; \
    ring ring; \
    ring_init(&ring, wa, size_entries, type_size); 

UTEST(ring, creation) {
    RING_CREATE(1024, 1);

    ASSERT_EQ(ring.cap_entries, (size_t)1024);
    ASSERT_EQ(ring.size_entries, (size_t)0);
    ASSERT_EQ(ring.type_size_bytes, (size_t)1);
    ASSERT_EQ(ring.left, (size_t)0);
    ASSERT_EQ(ring.right, (size_t)0);
    ASSERT_EQ(ring.wa, wa);
}

UTEST(ring, push_byte) {
    RING_CREATE(1024, 1);

    uint8_t byte = 0xDE;
    ASSERT_TRUE(ring_push(&ring, &byte));
    ASSERT_EQ(ring.size_entries, (size_t)1);
    ASSERT_EQ(ring.right, (size_t)1);
    ASSERT_EQ(ring.left, (size_t)0);
    ASSERT_EQ(ring.wa[0], 0xDE);
}

UTEST(ring, pop_byte) {
    RING_CREATE(1024, 1);

    uint8_t byte = 0xDE;
    ASSERT_TRUE(ring_push(&ring, &byte));
    ASSERT_EQ(ring.size_entries, (size_t)1);
    ASSERT_EQ(ring.right, (size_t)1);
    ASSERT_EQ(ring.left, (size_t)0);
    ASSERT_EQ(ring.wa[0], 0xDE);

    byte = 0x00;
    ASSERT_TRUE(ring_pop(&ring, &byte));
    ASSERT_EQ(byte, 0xDE);
    ASSERT_EQ(ring.right, (size_t)1);
    ASSERT_EQ(ring.left, (size_t)1);
    ASSERT_EQ(ring.size_entries, (size_t)0);
}

UTEST(ring, push_struct) {
    struct test {
        uint8_t byte;
        uint32_t word;
    } test = {
        .byte = 0xDE,
        .word = 0xADBEEF,
    };

    RING_CREATE(1024, sizeof(struct test));
    ASSERT_TRUE(ring_push(&ring, &test));
    ASSERT_EQ(ring.size_entries, (size_t)1);
    ASSERT_EQ(ring.right, (size_t)1);
    ASSERT_EQ(ring.left, (size_t)0);
    ASSERT_TRUE(memcmp(&test, &wa[0], sizeof(struct test)) == 0);
}

UTEST(ring, pop_struct) {
    struct test {
        uint8_t byte;
        uint32_t word;
    } test = {
        .byte = 0xDE,
        .word = 0xADBEEF,
    };

    RING_CREATE(1024, sizeof(struct test));
    ASSERT_TRUE(ring_push(&ring, &test));
    ASSERT_EQ(ring.size_entries, (size_t)1);
    ASSERT_EQ(ring.right, (size_t)1);
    ASSERT_EQ(ring.left, (size_t)0);
    ASSERT_MEMEQ(&test, wa, sizeof(struct test));

    struct test test2 = {
        .byte = 0xDE,
        .word = 0xADBEEF,
    };

    memset(&test, 0, sizeof(struct test));
    ASSERT_TRUE(ring_pop(&ring, &test));
    ASSERT_MEMEQ(&test, &test2, sizeof(struct test));
    ASSERT_EQ(ring.right, (size_t)1);
    ASSERT_EQ(ring.left, (size_t)1);
    ASSERT_EQ(ring.size_entries, (size_t)0);
}

UTEST(ring, push_fail) {
    RING_CREATE(256, 1);
    uint8_t value = 0xDE;
    for (size_t i = 0; i < 256; i++) {
        ASSERT_TRUE(ring_push(&ring, &value));
        ASSERT_EQ(ring.size_entries, i+1);
    }
    ASSERT_FALSE(ring_push(&ring, &value));
}

UTEST(ring, pop_fail) {
    RING_CREATE(256, 1);
    uint8_t value = 0xDE;
    for (size_t i = 0; i < 256; i++) {
        ASSERT_TRUE(ring_push(&ring, &value));
    }

    for (size_t i = 0; i < 256; i++) {
        ASSERT_TRUE(ring_pop(&ring, &value));
    }
    ASSERT_FALSE(ring_pop(&ring, &value));
}

UTEST(ring, pop_order) {
    RING_CREATE(5, 1);
    uint8_t bytes[5] = {0xde, 0xad, 0xbe, 0xaf, 0xc0};

    for (size_t i = 0; i < 5; i++) {
        ASSERT_TRUE(ring_push(&ring, &bytes[i]));
    }

    uint8_t byte = 0x00;
    for (size_t i = 0; i < 5; i++) {
        ASSERT_TRUE(ring_pop(&ring, &byte));
        ASSERT_EQ(byte, bytes[i]);
    }
}

UTEST_MAIN()
