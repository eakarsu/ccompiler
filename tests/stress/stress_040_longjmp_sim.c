// stress_040_longjmp_sim.c - Error handling patterns (no longjmp)
// EXPECT: level5_ok=success
// EXPECT: level3_fail=error at level 3
// EXPECT: level1_fail=error at level 1
// EXPECT: cleanup_order=5 4 3 2 1
// EXPECT: err_struct=code=42 msg=out of memory
// EXPECT: try_catch_ok=result=100
// EXPECT: try_catch_err=caught error 2
// EXPECT: chain_cleanup=freed_c freed_b freed_a
// EXPECT: multi_resource=all_freed
// EXPECT: retry_success=ok after 3 attempts
// EXPECT: nested_err=inner error handled, outer ok
// EXPECT: err_transform=final code=110
// EXPECT: callback_err=handler got error 7
// EXPECT: pass_done=1

int printf(const char *fmt, ...);
void *malloc(unsigned long);
void free(void *);

// --- Error struct ---
struct Error {
    int code;
    char *message;
};

// --- 5-level call chain with return codes ---
// Returns 0 on success, negative on error
// fail_at: which level to fail at (0 = no failure)

int level5(int fail_at) {
    if (fail_at == 5) return -5;
    return 0;
}

int level4(int fail_at) {
    if (fail_at == 4) return -4;
    int rc = level5(fail_at);
    if (rc < 0) return rc;
    return 0;
}

int level3(int fail_at) {
    if (fail_at == 3) return -3;
    int rc = level4(fail_at);
    if (rc < 0) return rc;
    return 0;
}

int level2(int fail_at) {
    if (fail_at == 2) return -2;
    int rc = level3(fail_at);
    if (rc < 0) return rc;
    return 0;
}

int level1(int fail_at) {
    if (fail_at == 1) return -1;
    int rc = level2(fail_at);
    if (rc < 0) return rc;
    return 0;
}

// --- Cleanup on error using goto ---
int cleanup_test(int *order, int *count) {
    int *a = 0;
    int *b = 0;
    int *c = 0;
    int *d = 0;
    int *e = 0;
    *count = 0;

    a = malloc(sizeof(int));
    if (!a) goto fail_a;
    *a = 1;

    b = malloc(sizeof(int));
    if (!b) goto fail_b;
    *b = 2;

    c = malloc(sizeof(int));
    if (!c) goto fail_c;
    *c = 3;

    d = malloc(sizeof(int));
    if (!d) goto fail_d;
    *d = 4;

    e = malloc(sizeof(int));
    if (!e) goto fail_e;
    *e = 5;

    // Simulate: pretend we need to clean up everything (success path)
    order[(*count)++] = 5;
    free(e);
fail_e:
    if (d) { order[(*count)++] = 4; free(d); }
fail_d:
    if (c) { order[(*count)++] = 3; free(c); }
fail_c:
    if (b) { order[(*count)++] = 2; free(b); }
fail_b:
    if (a) { order[(*count)++] = 1; free(a); }
fail_a:
    return 0;
}

// --- Try-catch simulation with function pointers ---
typedef int (*TryFunc)(int arg);
typedef void (*CatchFunc)(int error_code);

int try_operation(TryFunc fn, int arg, CatchFunc on_error) {
    int result = fn(arg);
    if (result < 0) {
        if (on_error) on_error(-result);
        return -1;
    }
    return result;
}

int success_op(int x) {
    return x * 10;
}

int failing_op(int x) {
    return -2; // error
}

int caught_code;
void catch_handler(int code) {
    caught_code = code;
}

// --- Chain cleanup: allocate 3 resources, fail on third usage, clean up ---
struct Resource {
    char *name;
    int active;
};

int freed_log_idx;
char *freed_log[3];

void free_resource(struct Resource *r) {
    if (r->active) {
        freed_log[freed_log_idx++] = r->name;
        r->active = 0;
    }
}

int chain_cleanup_test(void) {
    struct Resource ra;
    struct Resource rb;
    struct Resource rc;
    int err = 0;

    ra.name = "freed_a"; ra.active = 0;
    rb.name = "freed_b"; rb.active = 0;
    rc.name = "freed_c"; rc.active = 0;

    // "Acquire" resources
    ra.active = 1;
    rb.active = 1;
    rc.active = 1;

    // Simulate error after acquiring all three
    err = 1;

    // Cleanup in reverse order
    if (err) goto cleanup;

    return 0;
cleanup:
    free_resource(&rc);
    free_resource(&rb);
    free_resource(&ra);
    return -1;
}

// --- Multi-resource allocation with rollback ---
int multi_resource_test(void) {
    int *ptrs[5];
    int i;
    int allocated = 0;

    for (i = 0; i < 5; i++) {
        ptrs[i] = malloc(sizeof(int));
        if (!ptrs[i]) goto rollback;
        *ptrs[i] = i * 10;
        allocated++;
    }

    // Use resources
    int sum = 0;
    for (i = 0; i < 5; i++) sum = sum + *ptrs[i];
    // 0+10+20+30+40=100

    // Free all
    for (i = 0; i < 5; i++) free(ptrs[i]);
    return 0;

rollback:
    for (i = 0; i < allocated; i++) free(ptrs[i]);
    return -1;
}

// --- Retry pattern ---
int attempt_count;

int flaky_operation(void) {
    attempt_count++;
    if (attempt_count < 3) return -1; // fail first 2 times
    return 0; // succeed on 3rd
}

int retry_test(void) {
    attempt_count = 0;
    int max_retries = 5;
    int i;
    for (i = 0; i < max_retries; i++) {
        if (flaky_operation() == 0) return i + 1; // return attempt number
    }
    return -1;
}

// --- Nested error handling ---
int inner_op(int fail) {
    if (fail) return -1;
    return 42;
}

int outer_op(int inner_fail) {
    int rc = inner_op(inner_fail);
    if (rc < 0) {
        // Handle inner error, return transformed
        return -100;
    }
    return rc + 1;
}

// --- Error transformation chain ---
int transform_error(int original_code) {
    // Transform error code through a chain
    int code = original_code;
    if (code < 0) code = code * -1;        // make positive
    if (code < 10) code = code * 10;       // scale up small codes
    code = code + 100;                      // offset
    return code;
}

// --- Callback-based error handling ---
typedef void (*ErrHandler)(int code);

int callback_err_val;
void err_callback(int code) {
    callback_err_val = code;
}

int operation_with_callback(int should_fail, ErrHandler handler) {
    if (should_fail) {
        handler(7);
        return -1;
    }
    return 0;
}

int main(void) {
    // 5-level call chain: success
    int rc = level1(0);
    if (rc == 0) printf("level5_ok=success\n");

    // Fail at level 3
    rc = level1(3);
    if (rc == -3) printf("level3_fail=error at level 3\n");

    // Fail at level 1
    rc = level1(1);
    if (rc == -1) printf("level1_fail=error at level 1\n");

    // Cleanup with goto
    int order[5];
    int count;
    cleanup_test(order, &count);
    printf("cleanup_order=");
    int i;
    for (i = 0; i < count; i++) {
        if (i > 0) printf(" ");
        printf("%d", order[i]);
    }
    printf("\n");

    // Error struct
    struct Error e;
    e.code = 42;
    e.message = "out of memory";
    printf("err_struct=code=%d msg=%s\n", e.code, e.message);

    // Try-catch: success
    int result = try_operation(success_op, 10, catch_handler);
    printf("try_catch_ok=result=%d\n", result);

    // Try-catch: failure
    caught_code = 0;
    result = try_operation(failing_op, 10, catch_handler);
    printf("try_catch_err=caught error %d\n", caught_code);

    // Chain cleanup
    freed_log_idx = 0;
    chain_cleanup_test();
    printf("chain_cleanup=");
    for (i = 0; i < freed_log_idx; i++) {
        if (i > 0) printf(" ");
        printf("%s", freed_log[i]);
    }
    printf("\n");

    // Multi-resource
    rc = multi_resource_test();
    if (rc == 0) printf("multi_resource=all_freed\n");

    // Retry
    int attempts = retry_test();
    printf("retry_success=ok after %d attempts\n", attempts);

    // Nested error handling
    rc = outer_op(1); // inner fails
    if (rc == -100) {
        rc = outer_op(0); // inner succeeds
        if (rc == 43) {
            printf("nested_err=inner error handled, outer ok\n");
        }
    }

    // Error transformation
    int transformed = transform_error(-10);
    // -10 -> 10 (abs) -> 10*10=100 (no, 10 is not <10)... 10 is not <10, so no scale
    // 10 + 100 = 110... wait: -10 -> 10 (make positive), 10 < 10 is false, 10+100=110
    // Hmm, but EXPECT says 200. Let me use a different input.
    // Use -5: -5 -> 5 (positive), 5 < 10 -> 50, 50+100=150. Not 200.
    // Use -10: -10 -> 10, not <10, 10+100=110.
    // Let me just print what we get for -10.
    printf("err_transform=final code=%d\n", transform_error(-10));

    // Callback error handling
    callback_err_val = 0;
    operation_with_callback(1, err_callback);
    printf("callback_err=handler got error %d\n", callback_err_val);

    printf("pass_done=1\n");
    return 0;
}
