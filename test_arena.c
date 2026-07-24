#include "arena.h"
#include "utest.h"
#include <stddef.h>
#include <stdint.h>

#define MAKE_ARENA(size) \
    static const size_t WA_SIZE = size; \
    wa_t wa[WA_SIZE]; \
    arena arena; \
    arena_init(&arena, WA_SIZE, wa);

UTEST(arena, creation) {
    MAKE_ARENA(16);
    ASSERT_EQ(arena.wa, wa);
    ASSERT_EQ(arena.offset_bytes, (size_t)0);
    ASSERT_EQ(arena.cap_bytes, (size_t)WA_SIZE);
    ASSERT_EQ(arena_free(&arena), (size_t)16);
}

UTEST(arena, alloc) {
    MAKE_ARENA(256); 
    uint8_t *ptr = arena_alloc(&arena, 4, alignof(uint8_t)); 
    ASSERT_NE(ptr, NULL);
    ASSERT_EQ(arena.offset_bytes, (size_t)4);
    ASSERT_EQ(arena.cap_bytes, (size_t)WA_SIZE);
    ASSERT_EQ(arena_free(&arena), (size_t)(256 - 4));
}

UTEST(arena, alloc_fail) {
    MAKE_ARENA(256);
    uint8_t *ptr = arena_alloc(&arena, WA_SIZE + 24, alignof(uint8_t));
    ASSERT_EQ(ptr, NULL);
    ASSERT_EQ(arena.offset_bytes, (size_t)0);
    ASSERT_EQ(arena.cap_bytes, (size_t)WA_SIZE);
    ASSERT_EQ(arena_free(&arena), (size_t)WA_SIZE);
}

UTEST(arena, alloc_align) {
    MAKE_ARENA(256);

    uint32_t *ptr = arena_alloc(&arena, sizeof(uint32_t), alignof(uint32_t));
    ASSERT_NE(ptr, NULL);
    ASSERT_EQ(arena.offset_bytes, (size_t)4);
    ASSERT_EQ(arena_free(&arena), (size_t)(WA_SIZE - 4));

    struct test {
        uint8_t a;
        uint16_t b;
    } *ptr2 = arena_alloc(&arena, sizeof(struct test), alignof(struct test));
    ASSERT_NE(ptr2, NULL);
    ASSERT_EQ(arena.offset_bytes, (size_t)(8));
    ASSERT_EQ(arena_free(&arena), (size_t)(WA_SIZE - 8));
}

UTEST(arena, off_by_one) {
    MAKE_ARENA(4);
    uint32_t *ptr = arena_alloc(&arena, sizeof(uint32_t), alignof(uint32_t));
    ASSERT_NE(ptr, NULL);
    ASSERT_EQ(arena.offset_bytes, (size_t)4);
}

UTEST(arena, free) {
    MAKE_ARENA(4);
    (void)arena_alloc(&arena, sizeof(uint32_t), alignof(uint32_t));
    arena_dealloc(&arena);
    ASSERT_EQ(arena.offset_bytes, (size_t)0);
}

UTEST(arena, realloc) {
    MAKE_ARENA(4);
    (void)arena_alloc(&arena, sizeof(uint32_t), alignof(uint32_t));
    arena_dealloc(&arena);
    ASSERT_NE(NULL, arena_alloc(&arena, sizeof(uint32_t), alignof(uint32_t)));
}

UTEST(arena, stress) {
    MAKE_ARENA(1024);
    struct test {
        uint8_t a;
        uint16_t b;
    };

    for (int i = 0; i < 256; i++) {
        struct test *ptr = arena_alloc(&arena, sizeof(struct test), alignof(struct test));
        ASSERT_NE(ptr, NULL);
    }

    ASSERT_EQ(arena.offset_bytes, (size_t)1024);
}

UTEST_MAIN()
