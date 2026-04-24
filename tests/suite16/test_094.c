int printf(const char *fmt, ...);

// Test 094: Continuation-passing style (CPS) in C

typedef void (*IntCont)(int);

// Global result storage for continuations
int g_result;
int g_trace[20];
int g_trace_len;

void store_result(int x) {
    g_result = x;
}

void trace_result(int x) {
    g_trace[g_trace_len] = x;
    g_trace_len = g_trace_len + 1;
}

// CPS arithmetic: instead of returning, pass result to continuation
void add_cps(int a, int b, IntCont k) {
    k(a + b);
}

void mul_cps(int a, int b, IntCont k) {
    k(a * b);
}

void sub_cps(int a, int b, IntCont k) {
    k(a - b);
}

void negate_cps(int a, IntCont k) {
    k(-a);
}

void abs_cps(int a, IntCont k) {
    if (a < 0) {
        k(-a);
    } else {
        k(a);
    }
}

void max_cps(int a, int b, IntCont k) {
    if (a > b) {
        k(a);
    } else {
        k(b);
    }
}

void min_cps(int a, int b, IntCont k) {
    if (a < b) {
        k(a);
    } else {
        k(b);
    }
}

// Factorial in CPS using iteration (avoiding deep recursion)
void fact_cps(int n, IntCont k) {
    int result;
    int i;
    result = 1;
    for (i = 2; i <= n; i++) {
        result = result * i;
    }
    k(result);
}

// Fibonacci in CPS using iteration
void fib_cps(int n, IntCont k) {
    int a;
    int b;
    int temp;
    int i;
    if (n <= 0) { k(0); return; }
    if (n == 1) { k(1); return; }
    a = 0;
    b = 1;
    for (i = 2; i <= n; i++) {
        temp = a + b;
        a = b;
        b = temp;
    }
    k(b);
}

// Sum of array in CPS
void sum_array_cps(int *arr, int len, IntCont k) {
    int total;
    int i;
    total = 0;
    for (i = 0; i < len; i++) {
        total = total + arr[i];
    }
    k(total);
}

// Map-reduce in CPS: apply fn then sum
typedef int (*MapFn)(int);

int square(int x) { return x * x; }
int inc(int x) { return x + 1; }

void map_reduce_cps(int *arr, int len, MapFn fn, IntCont k) {
    int total;
    int i;
    total = 0;
    for (i = 0; i < len; i++) {
        total = total + fn(arr[i]);
    }
    k(total);
}

// Chain: compute (a+b) * c in CPS style
void chain_add_mul(int a, int b, int c) {
    // We simulate chaining by storing intermediate
    add_cps(a, b, store_result);
    mul_cps(g_result, c, store_result);
}

void print_trace(const char *label) {
    int i;
    printf("%s:", label);
    for (i = 0; i < g_trace_len; i++) {
        printf(" %d", g_trace[i]);
    }
    printf("\n");
}

int main(void) {
    int arr[5];

    add_cps(3, 4, store_result);
    printf("add_cps(3,4): %d\n", g_result); // EXPECT: add_cps(3,4): 7

    mul_cps(5, 6, store_result);
    printf("mul_cps(5,6): %d\n", g_result); // EXPECT: mul_cps(5,6): 30

    sub_cps(10, 3, store_result);
    printf("sub_cps(10,3): %d\n", g_result); // EXPECT: sub_cps(10,3): 7

    negate_cps(42, store_result);
    printf("neg_cps(42): %d\n", g_result); // EXPECT: neg_cps(42): -42

    abs_cps(-7, store_result);
    printf("abs_cps(-7): %d\n", g_result); // EXPECT: abs_cps(-7): 7

    abs_cps(9, store_result);
    printf("abs_cps(9): %d\n", g_result); // EXPECT: abs_cps(9): 9

    max_cps(3, 8, store_result);
    printf("max_cps(3,8): %d\n", g_result); // EXPECT: max_cps(3,8): 8

    min_cps(3, 8, store_result);
    printf("min_cps(3,8): %d\n", g_result); // EXPECT: min_cps(3,8): 3

    fact_cps(5, store_result);
    printf("fact_cps(5): %d\n", g_result); // EXPECT: fact_cps(5): 120

    fact_cps(7, store_result);
    printf("fact_cps(7): %d\n", g_result); // EXPECT: fact_cps(7): 5040

    fib_cps(10, store_result);
    printf("fib_cps(10): %d\n", g_result); // EXPECT: fib_cps(10): 55

    fib_cps(0, store_result);
    printf("fib_cps(0): %d\n", g_result); // EXPECT: fib_cps(0): 0

    fib_cps(1, store_result);
    printf("fib_cps(1): %d\n", g_result); // EXPECT: fib_cps(1): 1

    arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5;
    sum_array_cps(arr, 5, store_result);
    printf("sum_arr: %d\n", g_result); // EXPECT: sum_arr: 15

    map_reduce_cps(arr, 5, square, store_result);
    printf("sum_sq: %d\n", g_result); // EXPECT: sum_sq: 55

    map_reduce_cps(arr, 5, inc, store_result);
    printf("sum_inc: %d\n", g_result); // EXPECT: sum_inc: 20

    chain_add_mul(3, 4, 5);
    printf("chain(3+4)*5: %d\n", g_result); // EXPECT: chain(3+4)*5: 35

    // Trace: compute factorials 1..5 and trace them
    g_trace_len = 0;
    fact_cps(1, trace_result);
    fact_cps(2, trace_result);
    fact_cps(3, trace_result);
    fact_cps(4, trace_result);
    fact_cps(5, trace_result);
    print_trace("facts"); // EXPECT: facts: 1 2 6 24 120

    printf("done\n"); // EXPECT: done
    return 0;
}
