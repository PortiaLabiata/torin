#include "arena.h"
#include "utest.h"
#include <stddef.h>
#include <stdint.h>

#define MAKE_ARENA(size, wa_name, arena_name) \
    static const size_t WA_SIZE = size; \
    wa_t wa_name[WA_SIZE]; \
    arena arena_name; \
    arena_init(&arena_name, WA_SIZE, wa_name);

UTEST(arena, creation) {
    MAKE_ARENA(16, wa, arena);
    ASSERT_EQ(arena.arena, wa);
    ASSERT_EQ(arena.offset, (size_t)0);
    ASSERT_EQ(arena.cap, (size_t)WA_SIZE);
}

UTEST(arena, alloc) {
    MAKE_ARENA(256, wa, arena); 
    uint8_t *ptr = arena_alloc(&arena, 4, alignof(uint8_t)); 
    ASSERT_NE(ptr, NULL);
    ASSERT_EQ(arena.offset, (size_t)4);
    ASSERT_EQ(arena.cap, (size_t)WA_SIZE);
}

UTEST(arena, alloc_fail) {
    MAKE_ARENA(256, wa, arena);
    uint8_t *ptr = arena_alloc(&arena, WA_SIZE + 24, alignof(uint8_t));
    ASSERT_EQ(ptr, NULL);
    ASSERT_EQ(arena.offset, (size_t)0);
    ASSERT_EQ(arena.cap, (size_t)WA_SIZE);
}

UTEST(arena, alloc_align) {
    MAKE_ARENA(256, wa, arena);

    uint32_t *ptr = arena_alloc(&arena, sizeof(uint32_t), alignof(uint32_t));
    ASSERT_NE(ptr, NULL);
    ASSERT_EQ(arena.offset, (size_t)4);

    struct test {
        uint8_t a;
        uint16_t b;
    } *ptr2 = arena_alloc(&arena, sizeof(struct test), alignof(struct test));
    ASSERT_NE(ptr2, NULL);
    ASSERT_EQ(arena.offset, (size_t)(8));
}

UTEST(arena, off_by_one) {
    MAKE_ARENA(4, wa, arena);
    uint32_t *ptr = arena_alloc(&arena, sizeof(uint32_t), alignof(uint32_t));
    ASSERT_NE(ptr, NULL);
    ASSERT_EQ(arena.offset, (size_t)4);
}

UTEST(arena, free) {
    MAKE_ARENA(4, wa, arena);
    (void)arena_alloc(&arena, sizeof(uint32_t), alignof(uint32_t));
    arena_free(&arena);
    ASSERT_EQ(arena.offset, (size_t)0);
}

UTEST(arena, realloc) {
    MAKE_ARENA(4, wa, arena);
    (void)arena_alloc(&arena, sizeof(uint32_t), alignof(uint32_t));
    arena_free(&arena);
    ASSERT_NE(NULL, arena_alloc(&arena, sizeof(uint32_t), alignof(uint32_t)));
}

UTEST(arena, stress) {
    MAKE_ARENA(1024, wa, arena);
    struct test {
        uint8_t a;
        uint16_t b;
    };

    for (int i = 0; i < 256; i++) {
        struct test *ptr = arena_alloc(&arena, sizeof(struct test), alignof(struct test));
        ASSERT_NE(ptr, NULL);
    }

    ASSERT_EQ(arena.offset, (size_t)1024);
}

UTEST_MAIN()
