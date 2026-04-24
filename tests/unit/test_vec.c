#include "test_harness.h"
#include "vec.h"

TEST(vec_basic) {
    Vec(int) v = {0};
    vec_push(&v, 10);
    vec_push(&v, 20);
    vec_push(&v, 30);

    ASSERT_EQ(v.len, 3);
    ASSERT_EQ(v.data[0], 10);
    ASSERT_EQ(v.data[1], 20);
    ASSERT_EQ(v.data[2], 30);

    vec_free(&v);
}

TEST(vec_pop) {
    Vec(int) v = {0};
    vec_push(&v, 1);
    vec_push(&v, 2);
    vec_push(&v, 3);

    ASSERT_EQ(vec_pop(&v), 3);
    ASSERT_EQ(vec_pop(&v), 2);
    ASSERT_EQ(v.len, 1);

    vec_free(&v);
}

TEST(vec_last) {
    Vec(int) v = {0};
    vec_push(&v, 42);
    vec_push(&v, 99);
    ASSERT_EQ(vec_last(&v), 99);
    vec_free(&v);
}

TEST(vec_clear) {
    Vec(int) v = {0};
    vec_push(&v, 1);
    vec_push(&v, 2);
    vec_clear(&v);
    ASSERT_EQ(v.len, 0);
    ASSERT(v.cap > 0);  /* capacity preserved */
    vec_free(&v);
}

TEST(vec_many_elements) {
    Vec(int) v = {0};
    for (int i = 0; i < 10000; i++) {
        vec_push(&v, i);
    }
    ASSERT_EQ(v.len, 10000);
    for (int i = 0; i < 10000; i++) {
        ASSERT_EQ(v.data[i], i);
    }
    vec_free(&v);
}

TEST(vec_reserve) {
    Vec(int) v = {0};
    vec_reserve(&v, 100);
    ASSERT(v.cap >= 100);
    ASSERT_EQ(v.len, 0);
    vec_free(&v);
}

TEST(vec_pointers) {
    Vec(const char *) v = {0};
    vec_push(&v, "hello");
    vec_push(&v, "world");
    ASSERT_EQ(v.len, 2);
    ASSERT_STR_EQ(v.data[0], "hello");
    ASSERT_STR_EQ(v.data[1], "world");
    vec_free(&v);
}

int main(void) {
    RUN_TEST(vec_basic);
    RUN_TEST(vec_pop);
    RUN_TEST(vec_last);
    RUN_TEST(vec_clear);
    RUN_TEST(vec_many_elements);
    RUN_TEST(vec_reserve);
    RUN_TEST(vec_pointers);
    TEST_SUMMARY();
}
