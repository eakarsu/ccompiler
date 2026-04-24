// EXPECT: === Error Handling Patterns ===
// EXPECT: divide(10, 3) = 3, ok
// EXPECT: divide(10, 0) = 0, error: division by zero
// EXPECT: divide(-1, 1) = 0, error: negative input
// EXPECT: parse_positive("42") = 42, ok
// EXPECT: parse_positive("-5") = 0, error: not positive
// EXPECT: parse_positive("0") = 0, error: not positive
// EXPECT: File open simulated ok
// EXPECT: File write simulated ok
// EXPECT: File close simulated ok
// EXPECT: Goto cleanup: resource A acquired
// EXPECT: Goto cleanup: resource B acquired
// EXPECT: Goto cleanup: operation succeeded
// EXPECT: Goto cleanup: releasing B
// EXPECT: Goto cleanup: releasing A
// EXPECT: Goto cleanup fail: resource A acquired
// EXPECT: Goto cleanup fail: resource B failed
// EXPECT: Goto cleanup fail: releasing A
// EXPECT: Chain: step1 ok
// EXPECT: Chain: step2 ok
// EXPECT: Chain: step3 ok
// EXPECT: Chain: final result = 42
// EXPECT: Chain fail: step1 ok
// EXPECT: Chain fail: step2 failed with code -2
// EXPECT: Retry: attempt 1 failed
// EXPECT: Retry: attempt 2 failed
// EXPECT: Retry: attempt 3 succeeded
// EXPECT: Retry result: 30
// EXPECT: All error handling tests passed

int printf(const char *fmt, ...);

typedef struct {
    int code;
    const char *message;
} Error;

typedef struct {
    int value;
    Error err;
} Result;

Result make_ok(int val) {
    Result r;
    r.value = val;
    r.err.code = 0;
    r.err.message = "ok";
    return r;
}

Result make_error(int code, const char *msg) {
    Result r;
    r.value = 0;
    r.err.code = code;
    r.err.message = msg;
    return r;
}

Result divide(int a, int b) {
    if (b == 0) return make_error(-1, "division by zero");
    if (a < 0) return make_error(-2, "negative input");
    return make_ok(a / b);
}

Result parse_positive(const char *s) {
    int neg = 0;
    int val = 0;
    int i = 0;

    if (s[i] == '-') {
        neg = 1;
        i++;
    }
    while (s[i] >= '0' && s[i] <= '9') {
        val = val * 10 + (s[i] - '0');
        i++;
    }
    if (neg) val = -val;
    if (val <= 0) return make_error(-1, "not positive");
    return make_ok(val);
}

void test_return_codes(void) {
    Result r;

    r = divide(10, 3);
    printf("divide(10, 3) = %d, %s\n", r.value, r.err.code == 0 ? "ok" : r.err.message);

    r = divide(10, 0);
    printf("divide(10, 0) = %d, error: %s\n", r.value, r.err.message);

    r = divide(-1, 1);
    printf("divide(-1, 1) = %d, error: %s\n", r.value, r.err.message);

    r = parse_positive("42");
    printf("parse_positive(\"42\") = %d, %s\n", r.value, r.err.code == 0 ? "ok" : r.err.message);

    r = parse_positive("-5");
    printf("parse_positive(\"-5\") = %d, error: %s\n", r.value, r.err.message);

    r = parse_positive("0");
    printf("parse_positive(\"0\") = %d, error: %s\n", r.value, r.err.message);
}

int sim_file_open(int ok) { return ok ? 0 : -1; }
int sim_file_write(int ok) { return ok ? 0 : -1; }
int sim_file_close(int ok) { return ok ? 0 : -1; }

void test_file_simulation(void) {
    if (sim_file_open(1) == 0) printf("File open simulated ok\n");
    if (sim_file_write(1) == 0) printf("File write simulated ok\n");
    if (sim_file_close(1) == 0) printf("File close simulated ok\n");
}

void test_goto_cleanup(int fail_b) {
    int a_acquired = 0;
    int b_acquired = 0;

    a_acquired = 1;
    if (fail_b) {
        printf("Goto cleanup fail: resource A acquired\n");
    } else {
        printf("Goto cleanup: resource A acquired\n");
    }

    if (fail_b) {
        printf("Goto cleanup fail: resource B failed\n");
        goto cleanup_a;
    }

    b_acquired = 1;
    printf("Goto cleanup: resource B acquired\n");
    printf("Goto cleanup: operation succeeded\n");

    printf("Goto cleanup: releasing B\n");

cleanup_a:
    if (fail_b) {
        printf("Goto cleanup fail: releasing A\n");
    } else {
        printf("Goto cleanup: releasing A\n");
    }
}

int step1(int input, int *out) {
    *out = input + 10;
    return 0;
}

int step2(int input, int *out, int fail) {
    if (fail) return -2;
    *out = input * 2;
    return 0;
}

int step3(int input, int *out) {
    *out = input + 2;
    return 0;
}

void test_chain(int fail) {
    int val = 10;
    int rc;

    rc = step1(val, &val);
    if (rc != 0) {
        printf("Chain%s: step1 failed with code %d\n", fail ? " fail" : "", rc);
        return;
    }
    printf("Chain%s: step1 ok\n", fail ? " fail" : "");

    rc = step2(val, &val, fail);
    if (rc != 0) {
        printf("Chain fail: step2 failed with code %d\n", rc);
        return;
    }
    printf("Chain%s: step2 ok\n", fail ? " fail" : "");

    rc = step3(val, &val);
    if (rc != 0) {
        printf("Chain%s: step3 failed with code %d\n", fail ? " fail" : "", rc);
        return;
    }
    printf("Chain%s: step3 ok\n", fail ? " fail" : "");

    printf("Chain%s: final result = %d\n", fail ? " fail" : "", val);
}

int flaky_operation(int attempt) {
    if (attempt < 3) return -1;
    return 0;
}

void test_retry(void) {
    int max_retries = 5;
    int attempt;
    int result = 10;
    int success = 0;

    for (attempt = 1; attempt <= max_retries; attempt++) {
        if (flaky_operation(attempt) == 0) {
            printf("Retry: attempt %d succeeded\n", attempt);
            result = result * attempt;
            success = 1;
            break;
        }
        printf("Retry: attempt %d failed\n", attempt);
    }

    if (success) {
        printf("Retry result: %d\n", result);
    }
}

int main(void) {
    printf("=== Error Handling Patterns ===\n");
    test_return_codes();
    test_file_simulation();
    test_goto_cleanup(0);
    test_goto_cleanup(1);
    test_chain(0);
    test_chain(1);
    test_retry();
    printf("All error handling tests passed\n");
    return 0;
}
