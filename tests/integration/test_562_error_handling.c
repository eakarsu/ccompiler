int printf(const char *fmt, ...);
// EXPECT: add(100,200): value=300\nadd(2B,2B): ERROR(OVERFLOW)\ndiv(100,5): value=20\ndiv(100,0): ERROR(DIVZERO)\nindex[2]: value=30\nindex[10]: ERROR(RANGE)\nindex[-1]: ERROR(RANGE)\nsqrt(25): value=5\nsqrt(10): value=3\nsqrt(-4): ERROR(INVALID)\nErrors in batch: 3

enum ErrorCode {
    ERR_NONE = 0,
    ERR_OVERFLOW = 1,
    ERR_UNDERFLOW = 2,
    ERR_DIVZERO = 3,
    ERR_RANGE = 4,
    ERR_INVALID = 5
};

struct Result {
    int value;
    int error;
};

struct Result make_result(int val, int err) {
    struct Result r;
    r.value = val;
    r.error = err;
    return r;
}

char *error_name(int err) {
    if (err == ERR_NONE) return "OK";
    if (err == ERR_OVERFLOW) return "OVERFLOW";
    if (err == ERR_UNDERFLOW) return "UNDERFLOW";
    if (err == ERR_DIVZERO) return "DIVZERO";
    if (err == ERR_RANGE) return "RANGE";
    if (err == ERR_INVALID) return "INVALID";
    return "UNKNOWN";
}

struct Result safe_add(int a, int b) {
    int sum = a + b;
    if (a > 0 && b > 0 && sum < 0) return make_result(0, ERR_OVERFLOW);
    if (a < 0 && b < 0 && sum > 0) return make_result(0, ERR_UNDERFLOW);
    return make_result(sum, ERR_NONE);
}

struct Result safe_div(int a, int b) {
    if (b == 0) return make_result(0, ERR_DIVZERO);
    return make_result(a / b, ERR_NONE);
}

struct Result safe_index(int *arr, int size, int idx) {
    if (idx < 0 || idx >= size) return make_result(0, ERR_RANGE);
    return make_result(arr[idx], ERR_NONE);
}

struct Result safe_sqrt_int(int n) {
    if (n < 0) return make_result(0, ERR_INVALID);
    int i = 0;
    while (i * i <= n) i++;
    return make_result(i - 1, ERR_NONE);
}

void print_result(char *op, struct Result r) {
    if (r.error == ERR_NONE) {
        printf("%s: value=%d\n", op, r.value);
    } else {
        printf("%s: ERROR(%s)\n", op, error_name(r.error));
    }
}

int main(void) {
    struct Result r;

    r = safe_add(100, 200);
    print_result("add(100,200)", r);
    r = safe_add(2000000000, 2000000000);
    print_result("add(2B,2B)", r);

    r = safe_div(100, 5);
    print_result("div(100,5)", r);
    r = safe_div(100, 0);
    print_result("div(100,0)", r);

    int arr[5];
    arr[0] = 10; arr[1] = 20; arr[2] = 30; arr[3] = 40; arr[4] = 50;
    r = safe_index(arr, 5, 2);
    print_result("index[2]", r);
    r = safe_index(arr, 5, 10);
    print_result("index[10]", r);
    r = safe_index(arr, 5, -1);
    print_result("index[-1]", r);

    r = safe_sqrt_int(25);
    print_result("sqrt(25)", r);
    r = safe_sqrt_int(10);
    print_result("sqrt(10)", r);
    r = safe_sqrt_int(-4);
    print_result("sqrt(-4)", r);

    int errors = 0;
    int i;
    int divisors[6];
    divisors[0] = 3; divisors[1] = 0; divisors[2] = 7; divisors[3] = 0; divisors[4] = 2; divisors[5] = 0;
    for (i = 0; i < 6; i++) {
        r = safe_div(42, divisors[i]);
        if (r.error != ERR_NONE) errors++;
    }
    printf("Errors in batch: %d\n", errors);

    return 0;
}
