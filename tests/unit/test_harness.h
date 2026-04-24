#ifndef TEST_HARNESS_H
#define TEST_HARNESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int _test_pass = 0;
static int _test_fail = 0;
static const char *_test_current = NULL;

#define TEST(name) \
    static void test_##name(void); \
    static void _run_test_##name(void) { \
        _test_current = #name; \
        test_##name(); \
        _test_pass++; \
    } \
    static void test_##name(void)

#define RUN_TEST(name) _run_test_##name()

#define ASSERT(cond) do { \
    if (!(cond)) { \
        fprintf(stderr, "  ASSERT FAILED: %s:%d: %s (in test '%s')\n", \
                __FILE__, __LINE__, #cond, _test_current); \
        _test_fail++; \
        return; \
    } \
} while(0)

#define ASSERT_EQ(a, b) do { \
    long long _a = (long long)(a), _b = (long long)(b); \
    if (_a != _b) { \
        fprintf(stderr, "  ASSERT_EQ FAILED: %s:%d: %lld != %lld (in test '%s')\n", \
                __FILE__, __LINE__, _a, _b, _test_current); \
        _test_fail++; \
        return; \
    } \
} while(0)

#define ASSERT_STR_EQ(a, b) do { \
    const char *_a = (a), *_b = (b); \
    if (strcmp(_a, _b) != 0) { \
        fprintf(stderr, "  ASSERT_STR_EQ FAILED: %s:%d: \"%s\" != \"%s\" (in test '%s')\n", \
                __FILE__, __LINE__, _a, _b, _test_current); \
        _test_fail++; \
        return; \
    } \
} while(0)

#define ASSERT_PTR_EQ(a, b) do { \
    const void *_a = (a), *_b = (b); \
    if (_a != _b) { \
        fprintf(stderr, "  ASSERT_PTR_EQ FAILED: %s:%d: %p != %p (in test '%s')\n", \
                __FILE__, __LINE__, _a, _b, _test_current); \
        _test_fail++; \
        return; \
    } \
} while(0)

#define ASSERT_NOT_NULL(p) do { \
    if ((p) == NULL) { \
        fprintf(stderr, "  ASSERT_NOT_NULL FAILED: %s:%d: got NULL (in test '%s')\n", \
                __FILE__, __LINE__, _test_current); \
        _test_fail++; \
        return; \
    } \
} while(0)

#define TEST_SUMMARY() do { \
    printf("%d passed, %d failed\n", _test_pass, _test_fail); \
    return _test_fail > 0 ? 1 : 0; \
} while(0)

#endif /* TEST_HARNESS_H */
