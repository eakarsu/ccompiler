#include "test_harness.h"
#include "arena.h"
#include <string.h>

TEST(arena_basic_alloc) {
    Arena a;
    arena_init(&a, 0);

    void *p1 = arena_alloc(&a, 100);
    ASSERT_NOT_NULL(p1);

    void *p2 = arena_alloc(&a, 200);
    ASSERT_NOT_NULL(p2);
    ASSERT(p1 != p2);

    arena_destroy(&a);
}

TEST(arena_calloc_zeroed) {
    Arena a;
    arena_init(&a, 0);

    int *p = arena_calloc(&a, sizeof(int) * 10);
    ASSERT_NOT_NULL(p);
    for (int i = 0; i < 10; i++) {
        ASSERT_EQ(p[i], 0);
    }

    arena_destroy(&a);
}

TEST(arena_strdup) {
    Arena a;
    arena_init(&a, 0);

    char *s = arena_strdup(&a, "hello world");
    ASSERT_STR_EQ(s, "hello world");

    char *s2 = arena_strndup(&a, "hello world", 5);
    ASSERT_STR_EQ(s2, "hello");

    arena_destroy(&a);
}

TEST(arena_large_alloc) {
    Arena a;
    arena_init(&a, 1024);  /* small chunk size */

    /* Allocate more than chunk size */
    void *p = arena_alloc(&a, 2048);
    ASSERT_NOT_NULL(p);

    /* Normal alloc after large alloc should still work */
    void *p2 = arena_alloc(&a, 16);
    ASSERT_NOT_NULL(p2);

    arena_destroy(&a);
}

TEST(arena_many_allocs) {
    Arena a;
    arena_init(&a, 256);

    /* Force multiple chunks */
    for (int i = 0; i < 1000; i++) {
        void *p = arena_alloc(&a, 64);
        ASSERT_NOT_NULL(p);
    }

    arena_destroy(&a);
}

TEST(arena_reset) {
    Arena a;
    arena_init(&a, 0);

    void *p1 = arena_alloc(&a, 100);
    ASSERT_NOT_NULL(p1);

    arena_reset(&a);

    /* After reset, can reuse the same memory */
    void *p2 = arena_alloc(&a, 100);
    ASSERT_NOT_NULL(p2);

    arena_destroy(&a);
}

TEST(arena_alignment) {
    Arena a;
    arena_init(&a, 0);

    /* All allocations should be 8-byte aligned */
    for (int i = 1; i <= 17; i++) {
        void *p = arena_alloc(&a, i);
        ASSERT(((uintptr_t)p & 7) == 0);
    }

    arena_destroy(&a);
}

int main(void) {
    RUN_TEST(arena_basic_alloc);
    RUN_TEST(arena_calloc_zeroed);
    RUN_TEST(arena_strdup);
    RUN_TEST(arena_large_alloc);
    RUN_TEST(arena_many_allocs);
    RUN_TEST(arena_reset);
    RUN_TEST(arena_alignment);
    TEST_SUMMARY();
}
