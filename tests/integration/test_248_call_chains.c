int printf(const char *fmt, ...);
// EXPECT: chain5(1)=24\nchain5(5)=48\nchain5_rev(1)=15\nchain5_rev(5)=39\nchain_loop(1,1)=1\nchain_loop(1,3)=1\nchain_loop(1,5)=1\nbinary_chain(3,4)=49\nbinary_chain(2,5)=49\nfib_chain(10)=55\nbounce_a(10)=26\nbounce_b(10)=29\nbounce_c(10)=28\nbounce_d(10)=27\naccum(1)=43\naccum(5)=91\ncascade(2,3,4)=35\ncascade(1,1,1)=3\nsum_chain5_0_19=1500
// Test 248: Stress test - long call chains

int f1(int x) { return x + 1; }
int f2(int x) { return x * 2; }
int f3(int x) { return x - 3; }
int f4(int x) { return x + 7; }
int f5(int x) { return x * 3; }

int chain5(int x) {
    return f5(f4(f3(f2(f1(x)))));
}

int chain5_rev(int x) {
    return f1(f2(f3(f4(f5(x)))));
}

int chain_loop(int x, int iterations) {
    int result = x;
    int i = 0;
    while (i < iterations) {
        result = f1(result);
        result = f2(result);
        result = f3(result);
        i = i + 1;
    }
    return result;
}

int g1(int a, int b) { return a + b; }
int g2(int a, int b) { return a * b; }
int g3(int a, int b) { return a - b; }

int binary_chain(int a, int b) {
    int r1 = g1(a, b);
    int r2 = g2(a, b);
    int r3 = g3(r1, r2);
    int r4 = g1(r3, r2);
    int r5 = g2(r4, r1);
    return r5;
}

int fibonacci_chain(int n) {
    if (n <= 1) return n;
    int a = fibonacci_chain(n - 1);
    int b = fibonacci_chain(n - 2);
    return g1(a, b);
}

int bounce_a(int n);
int bounce_b(int n);
int bounce_c(int n);
int bounce_d(int n);

int bounce_a(int n) {
    if (n <= 0) return 1;
    return bounce_b(n - 1) + 1;
}
int bounce_b(int n) {
    if (n <= 0) return 2;
    return bounce_c(n - 1) + 2;
}
int bounce_c(int n) {
    if (n <= 0) return 3;
    return bounce_d(n - 1) + 3;
}
int bounce_d(int n) {
    if (n <= 0) return 4;
    return bounce_a(n - 1) + 4;
}

int accumulate_calls(int x) {
    int result = x;
    result = f1(result);
    result = f2(result);
    result = f1(result);
    result = f2(result);
    result = f1(result);
    result = f3(result);
    result = f4(result);
    result = f5(result);
    result = f3(result);
    result = f1(result);
    return result;
}

int cascade(int a, int b, int c) {
    int r1 = g1(a, g2(b, c));
    int r2 = g2(g1(a, b), c);
    int r3 = g3(g2(a, c), g1(b, c));
    return g1(r1, g1(r2, r3));
}

int main(void) {
    printf("chain5(1)=%d\n", chain5(1));
    printf("chain5(5)=%d\n", chain5(5));
    printf("chain5_rev(1)=%d\n", chain5_rev(1));
    printf("chain5_rev(5)=%d\n", chain5_rev(5));

    printf("chain_loop(1,1)=%d\n", chain_loop(1, 1));
    printf("chain_loop(1,3)=%d\n", chain_loop(1, 3));
    printf("chain_loop(1,5)=%d\n", chain_loop(1, 5));

    printf("binary_chain(3,4)=%d\n", binary_chain(3, 4));
    printf("binary_chain(2,5)=%d\n", binary_chain(2, 5));

    printf("fib_chain(10)=%d\n", fibonacci_chain(10));

    printf("bounce_a(10)=%d\n", bounce_a(10));
    printf("bounce_b(10)=%d\n", bounce_b(10));
    printf("bounce_c(10)=%d\n", bounce_c(10));
    printf("bounce_d(10)=%d\n", bounce_d(10));

    printf("accum(1)=%d\n", accumulate_calls(1));
    printf("accum(5)=%d\n", accumulate_calls(5));

    printf("cascade(2,3,4)=%d\n", cascade(2, 3, 4));
    printf("cascade(1,1,1)=%d\n", cascade(1, 1, 1));

    int i = 0;
    int sum = 0;
    while (i < 20) {
        sum = sum + chain5(i);
        i = i + 1;
    }
    printf("sum_chain5_0_19=%d\n", sum);

    return 0;
}
