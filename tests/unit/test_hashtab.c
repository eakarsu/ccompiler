#include "test_harness.h"
#include "arena.h"
#include "hashtab.h"
#include "strintern.h"

static Arena test_arena;
static InternTab test_intern;

static void setup(void) {
    arena_init(&test_arena, 0);
    intern_init(&test_intern, &test_arena);
}

static void teardown(void) {
    intern_free(&test_intern);
    arena_destroy(&test_arena);
}

TEST(hashtab_basic) {
    setup();
    HashTab t;
    hashtab_init(&t, 0);

    const char *k1 = intern(&test_intern, "foo");
    const char *k2 = intern(&test_intern, "bar");

    ASSERT(hashtab_put(&t, k1, (void*)42));
    ASSERT(hashtab_put(&t, k2, (void*)99));

    ASSERT_EQ((long long)(uintptr_t)hashtab_get(&t, k1), 42);
    ASSERT_EQ((long long)(uintptr_t)hashtab_get(&t, k2), 99);

    hashtab_free(&t);
    teardown();
}

TEST(hashtab_update) {
    setup();
    HashTab t;
    hashtab_init(&t, 0);

    const char *k = intern(&test_intern, "key");
    ASSERT(hashtab_put(&t, k, (void*)1));
    ASSERT(!hashtab_put(&t, k, (void*)2));  /* update returns false */
    ASSERT_EQ((long long)(uintptr_t)hashtab_get(&t, k), 2);

    hashtab_free(&t);
    teardown();
}

TEST(hashtab_delete) {
    setup();
    HashTab t;
    hashtab_init(&t, 0);

    const char *k = intern(&test_intern, "key");
    hashtab_put(&t, k, (void*)1);
    ASSERT(hashtab_delete(&t, k));
    ASSERT(hashtab_get(&t, k) == NULL);
    ASSERT(!hashtab_delete(&t, k));  /* already deleted */

    hashtab_free(&t);
    teardown();
}

TEST(hashtab_many_entries) {
    setup();
    HashTab t;
    hashtab_init(&t, 0);

    /* Insert 1000 entries */
    for (int i = 0; i < 1000; i++) {
        char buf[32];
        snprintf(buf, sizeof(buf), "key_%d", i);
        const char *k = intern(&test_intern, buf);
        hashtab_put(&t, k, (void*)(uintptr_t)i);
    }

    /* Verify all entries */
    for (int i = 0; i < 1000; i++) {
        char buf[32];
        snprintf(buf, sizeof(buf), "key_%d", i);
        const char *k = intern(&test_intern, buf);
        ASSERT_EQ((long long)(uintptr_t)hashtab_get(&t, k), i);
    }

    hashtab_free(&t);
    teardown();
}

TEST(hashtab_iter) {
    setup();
    HashTab t;
    hashtab_init(&t, 0);

    const char *k1 = intern(&test_intern, "a");
    const char *k2 = intern(&test_intern, "b");
    const char *k3 = intern(&test_intern, "c");

    hashtab_put(&t, k1, (void*)1);
    hashtab_put(&t, k2, (void*)2);
    hashtab_put(&t, k3, (void*)3);

    int count = 0;
    long long sum = 0;
    HashIter it = hashtab_iter(&t);
    const char *key;
    void *val;
    while (hashtab_next(&it, &key, &val)) {
        count++;
        sum += (long long)(uintptr_t)val;
    }
    ASSERT_EQ(count, 3);
    ASSERT_EQ(sum, 6);

    hashtab_free(&t);
    teardown();
}

TEST(hashtab_clear) {
    setup();
    HashTab t;
    hashtab_init(&t, 0);

    const char *k = intern(&test_intern, "key");
    hashtab_put(&t, k, (void*)1);

    hashtab_clear(&t);
    ASSERT(hashtab_get(&t, k) == NULL);

    hashtab_free(&t);
    teardown();
}

int main(void) {
    RUN_TEST(hashtab_basic);
    RUN_TEST(hashtab_update);
    RUN_TEST(hashtab_delete);
    RUN_TEST(hashtab_many_entries);
    RUN_TEST(hashtab_iter);
    RUN_TEST(hashtab_clear);
    TEST_SUMMARY();
}
