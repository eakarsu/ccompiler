#include "test_harness.h"
#include "arena.h"
#include "strintern.h"

TEST(intern_basic) {
    Arena a;
    arena_init(&a, 0);
    InternTab tab;
    intern_init(&tab, &a);

    const char *s1 = intern(&tab, "hello");
    const char *s2 = intern(&tab, "hello");
    ASSERT_PTR_EQ(s1, s2);  /* same pointer */
    ASSERT_STR_EQ(s1, "hello");

    intern_free(&tab);
    arena_destroy(&a);
}

TEST(intern_different_strings) {
    Arena a;
    arena_init(&a, 0);
    InternTab tab;
    intern_init(&tab, &a);

    const char *s1 = intern(&tab, "foo");
    const char *s2 = intern(&tab, "bar");
    ASSERT(s1 != s2);
    ASSERT_STR_EQ(s1, "foo");
    ASSERT_STR_EQ(s2, "bar");

    intern_free(&tab);
    arena_destroy(&a);
}

TEST(intern_n) {
    Arena a;
    arena_init(&a, 0);
    InternTab tab;
    intern_init(&tab, &a);

    const char *s1 = intern_n(&tab, "hello world", 5);
    const char *s2 = intern(&tab, "hello");
    ASSERT_PTR_EQ(s1, s2);  /* "hello" interned same way */
    ASSERT_STR_EQ(s1, "hello");

    intern_free(&tab);
    arena_destroy(&a);
}

TEST(intern_many) {
    Arena a;
    arena_init(&a, 0);
    InternTab tab;
    intern_init(&tab, &a);

    const char *ptrs[500];
    for (int i = 0; i < 500; i++) {
        char buf[32];
        snprintf(buf, sizeof(buf), "str_%d", i);
        ptrs[i] = intern(&tab, buf);
    }

    /* Verify all are still valid and unique */
    for (int i = 0; i < 500; i++) {
        char buf[32];
        snprintf(buf, sizeof(buf), "str_%d", i);
        const char *s = intern(&tab, buf);
        ASSERT_PTR_EQ(s, ptrs[i]);
        ASSERT_STR_EQ(s, buf);
    }

    intern_free(&tab);
    arena_destroy(&a);
}

TEST(intern_empty_string) {
    Arena a;
    arena_init(&a, 0);
    InternTab tab;
    intern_init(&tab, &a);

    const char *s1 = intern(&tab, "");
    const char *s2 = intern(&tab, "");
    ASSERT_PTR_EQ(s1, s2);
    ASSERT_STR_EQ(s1, "");

    intern_free(&tab);
    arena_destroy(&a);
}

int main(void) {
    RUN_TEST(intern_basic);
    RUN_TEST(intern_different_strings);
    RUN_TEST(intern_n);
    RUN_TEST(intern_many);
    RUN_TEST(intern_empty_string);
    TEST_SUMMARY();
}
