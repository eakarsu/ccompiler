int printf(const char *fmt, ...);

// Test 098: Monadic error handling with Result structs

struct Result {
    int value;
    int is_ok;    // 1 = ok, 0 = error
    int err_code; // error code if is_ok == 0
};

struct Result result_ok(int val) {
    struct Result r;
    r.value = val;
    r.is_ok = 1;
    r.err_code = 0;
    return r;
}

struct Result result_err(int code) {
    struct Result r;
    r.value = 0;
    r.is_ok = 0;
    r.err_code = code;
    return r;
}

int result_unwrap(struct Result r) {
    return r.value;
}

int result_unwrap_or(struct Result r, int fallback) {
    if (r.is_ok) return r.value;
    return fallback;
}

void result_print(const char *label, struct Result r) {
    if (r.is_ok) {
        printf("%s: ok(%d)\n", label, r.value);
    } else {
        printf("%s: err(%d)\n", label, r.err_code);
    }
}

// Monadic bind: apply fn only if result is ok
typedef struct Result (*ResultFn)(int);

struct Result result_bind(struct Result r, ResultFn fn) {
    if (!r.is_ok) return r;
    return fn(r.value);
}

// Domain functions that may fail
struct Result safe_div(int a, int b) {
    if (b == 0) return result_err(1); // division by zero
    return result_ok(a / b);
}

struct Result safe_sqrt_int(int x) {
    int i;
    if (x < 0) return result_err(2); // negative
    for (i = 0; i * i <= x; i++) {
        if (i * i == x) return result_ok(i);
    }
    return result_err(3); // not a perfect square
}

struct Result check_positive(int x) {
    if (x <= 0) return result_err(4);
    return result_ok(x);
}

struct Result double_val(int x) {
    return result_ok(x * 2);
}

struct Result add_five(int x) {
    return result_ok(x + 5);
}

struct Result negate_if_big(int x) {
    if (x > 100) return result_err(5); // too big
    return result_ok(-x);
}

// Chain of computations
struct Result compute_chain1(int x) {
    struct Result r;
    r = result_ok(x);
    r = result_bind(r, check_positive);
    r = result_bind(r, double_val);
    r = result_bind(r, add_five);
    return r;
}

struct Result compute_chain2(int x) {
    struct Result r;
    r = result_ok(x);
    r = result_bind(r, safe_sqrt_int);
    r = result_bind(r, double_val);
    return r;
}

// Result map: apply a plain function to the value
typedef int (*IntFn)(int);

int square(int x) { return x * x; }
int inc(int x) { return x + 1; }

struct Result result_map(struct Result r, IntFn fn) {
    if (!r.is_ok) return r;
    return result_ok(fn(r.value));
}

// Collect results: count oks and errors
struct ResultStats {
    int ok_count;
    int err_count;
    int ok_sum;
};

struct ResultStats count_results(struct Result *results, int n) {
    struct ResultStats s;
    int i;
    s.ok_count = 0;
    s.err_count = 0;
    s.ok_sum = 0;
    for (i = 0; i < n; i++) {
        if (results[i].is_ok) {
            s.ok_count = s.ok_count + 1;
            s.ok_sum = s.ok_sum + results[i].value;
        } else {
            s.err_count = s.err_count + 1;
        }
    }
    return s;
}

int main(void) {
    struct Result r;
    struct Result results[8];
    struct ResultStats stats;

    // Basic ok/err
    r = result_ok(42);
    result_print("ok42", r); // EXPECT: ok42: ok(42)

    r = result_err(99);
    result_print("err99", r); // EXPECT: err99: err(99)

    // Unwrap
    r = result_ok(7);
    printf("unwrap: %d\n", result_unwrap(r)); // EXPECT: unwrap: 7

    printf("unwrap_or_ok: %d\n", result_unwrap_or(result_ok(5), -1)); // EXPECT: unwrap_or_ok: 5
    printf("unwrap_or_err: %d\n", result_unwrap_or(result_err(1), -1)); // EXPECT: unwrap_or_err: -1

    // Safe division
    result_print("10/3", safe_div(10, 3)); // EXPECT: 10/3: ok(3)
    result_print("10/0", safe_div(10, 0)); // EXPECT: 10/0: err(1)
    result_print("0/5", safe_div(0, 5)); // EXPECT: 0/5: ok(0)

    // Safe sqrt
    result_print("sqrt25", safe_sqrt_int(25)); // EXPECT: sqrt25: ok(5)
    result_print("sqrt16", safe_sqrt_int(16)); // EXPECT: sqrt16: ok(4)
    result_print("sqrt-1", safe_sqrt_int(-1)); // EXPECT: sqrt-1: err(2)
    result_print("sqrt7", safe_sqrt_int(7)); // EXPECT: sqrt7: err(3)

    // Monadic bind chains
    r = result_bind(result_ok(10), double_val);
    result_print("bind_dbl", r); // EXPECT: bind_dbl: ok(20)

    r = result_bind(result_err(9), double_val);
    result_print("bind_err", r); // EXPECT: bind_err: err(9)

    // Chain1: check_positive -> double -> add_five
    result_print("chain1(3)", compute_chain1(3)); // EXPECT: chain1(3): ok(11)
    result_print("chain1(-2)", compute_chain1(-2)); // EXPECT: chain1(-2): err(4)

    // Chain2: sqrt -> double
    result_print("chain2(16)", compute_chain2(16)); // EXPECT: chain2(16): ok(8)
    result_print("chain2(9)", compute_chain2(9)); // EXPECT: chain2(9): ok(6)
    result_print("chain2(-4)", compute_chain2(-4)); // EXPECT: chain2(-4): err(2)
    result_print("chain2(7)", compute_chain2(7)); // EXPECT: chain2(7): err(3)

    // Result map
    r = result_map(result_ok(5), square);
    result_print("map_sq5", r); // EXPECT: map_sq5: ok(25)

    r = result_map(result_err(1), square);
    result_print("map_err", r); // EXPECT: map_err: err(1)

    r = result_map(result_ok(9), inc);
    result_print("map_inc9", r); // EXPECT: map_inc9: ok(10)

    // Collect results
    results[0] = result_ok(10);
    results[1] = result_err(1);
    results[2] = result_ok(20);
    results[3] = result_ok(30);
    results[4] = result_err(2);
    stats = count_results(results, 5);
    printf("oks: %d\n", stats.ok_count); // EXPECT: oks: 3
    printf("errs: %d\n", stats.err_count); // EXPECT: errs: 2
    printf("ok_sum: %d\n", stats.ok_sum); // EXPECT: ok_sum: 60

    printf("done\n"); // EXPECT: done
    return 0;
}
