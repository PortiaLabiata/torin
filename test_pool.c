#include "pool.h"
#include "utest.h"
#include <stddef.h>
#include <stdint.h>

#define MAKE_POOL(size, chunks) \
    static const size_t WA_SIZE = size; \
    pool_wa_t wa[WA_SIZE]; \
    pool pool; \
    pool_init_number(&pool, wa, WA_SIZE, chunks);

UTEST(pool, creation) {
    MAKE_POOL(1024, 4);
    ASSERT_EQ(pool.wa, wa);
    ASSERT_EQ(pool.top, (void*)wa);
    ASSERT_EQ(pool.cap, (size_t)4);
    ASSERT_EQ(pool.size, WA_SIZE);
    ASSERT_EQ(pool.n, (size_t)0);
}

UTEST(pool, list) {
    MAKE_POOL(8*32, 32);

    pool_chunk *chunk = pool.top;
    for (size_t i = 0; i < 31; i++) {
        ASSERT_EQ(chunk->next, (void*)(wa + i*8 + 8));
        chunk = chunk->next;
    }
}

UTEST(pool, alloc) {
    MAKE_POOL(8*16, 16);
    uint32_t *ptr = pool_alloc(&pool);
    ASSERT_NE(ptr, NULL);
    ASSERT_EQ(ptr, (void*)wa);
    ASSERT_EQ(pool.n, (size_t)1);
    ASSERT_EQ(pool.top, (pool_chunk*)(pool_wa_t*)(wa + 8));
}

UTEST(pool, multiple_alloc) {
    MAKE_POOL(8*16, 16);
    uint32_t *ptr = pool_alloc(&pool);
    ASSERT_NE(ptr, NULL);
    
    uint32_t *ptr2 = pool_alloc(&pool);
    ASSERT_NE(ptr2, NULL);
    ASSERT_EQ(ptr2, (void*)(wa + 8));
    ASSERT_EQ(pool.n, (size_t)2);
    ASSERT_EQ(pool.top, (void*)(wa + 16));
}

UTEST(pool, dealloc) {
    MAKE_POOL(8*16, 16);
    uint32_t *ptr = pool_alloc(&pool);
    ASSERT_NE(ptr, NULL);

    pool_free(&pool, ptr);
    ASSERT_EQ(pool.top, (void*)wa);
    ASSERT_EQ(pool.n, (size_t)0);
    ASSERT_EQ(pool.top->next, (void*)(wa + 8));
}

UTEST(pool, realloc) {
    MAKE_POOL(8*16, 16);
    uint32_t *ptr = pool_alloc(&pool);
    ASSERT_NE(ptr, NULL);

    pool_free(&pool, ptr);
    uint32_t *ptr2 = pool_alloc(&pool);
    ASSERT_EQ(ptr, ptr2);
}

UTEST(pool, exhaust) {
    MAKE_POOL(8*16, 16);

    for (size_t i = 0; i < 16; i++) {
        uint32_t *ptr = pool_alloc(&pool);
        ASSERT_NE(ptr, NULL);
    }
    uint32_t *ptr = pool_alloc(&pool);
    ASSERT_EQ(ptr, NULL);
}

UTEST(pool, stress) {
    MAKE_POOL(8*1024, 1024);

    for (size_t i = 0; i < 1024; i++) {
        uint32_t *ptr = pool_alloc(&pool);
        ASSERT_NE(ptr, NULL);
    }
}

UTEST_MAIN()
