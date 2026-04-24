int printf(const char *fmt, ...);

// Newton's method for square root using x1000 scaling
// sqrt(N) with scaled integers: x * x / SCALE approximates x^2/SCALE

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

// Compute sqrt(n) * 1000 using Newton's method
// We want x such that x*x = n*1000*1000
// Newton: x_new = (x + n*1000000/x) / 2
int sqrt_scaled(int n) {
    int target;
    int x;
    int prev;
    int i;

    if (n == 0) return 0;
    target = n * 1000; // We want result * 1000
    x = target;        // Initial guess

    // Iterate Newton's method
    for (i = 0; i < 50; i++) {
        prev = x;
        // x_new = (x + n*1000000/x) / 2
        x = (x + (n * 1000000) / x) / 2;
        if (abs_val(x - prev) <= 1) break;
    }
    return x;
}

// Verify: sqrt(4) = 2.000, sqrt(9) = 3.000, sqrt(2) ~ 1.414
void test_sqrt(void) {
    int r;

    printf("=== Newton sqrt (x1000) ===\n"); // EXPECT: === Newton sqrt (x1000) ===

    r = sqrt_scaled(4);
    printf("sqrt(4)*1000 = %d\n", r); // EXPECT: sqrt(4)*1000 = 2000

    r = sqrt_scaled(9);
    printf("sqrt(9)*1000 = %d\n", r); // EXPECT: sqrt(9)*1000 = 3000

    r = sqrt_scaled(16);
    printf("sqrt(16)*1000 = %d\n", r); // EXPECT: sqrt(16)*1000 = 4000

    r = sqrt_scaled(25);
    printf("sqrt(25)*1000 = %d\n", r); // EXPECT: sqrt(25)*1000 = 5000

    r = sqrt_scaled(2);
    printf("sqrt(2)*1000 = %d\n", r); // EXPECT: sqrt(2)*1000 = 1414

    r = sqrt_scaled(3);
    printf("sqrt(3)*1000 = %d\n", r); // EXPECT: sqrt(3)*1000 = 1732

    r = sqrt_scaled(10);
    printf("sqrt(10)*1000 = %d\n", r); // EXPECT: sqrt(10)*1000 = 3162

    r = sqrt_scaled(0);
    printf("sqrt(0)*1000 = %d\n", r); // EXPECT: sqrt(0)*1000 = 0

    r = sqrt_scaled(1);
    printf("sqrt(1)*1000 = %d\n", r); // EXPECT: sqrt(1)*1000 = 1000

    r = sqrt_scaled(100);
    printf("sqrt(100)*1000 = %d\n", r); // EXPECT: sqrt(100)*1000 = 10000
}

// Newton's method for cube root using x1000 scaling
// cbrt(n) * 1000
// Newton: x_new = (2*x + n*1000000000/(x*x)) / 3
int cbrt_scaled(int n) {
    int x;
    int prev;
    int i;
    long long nl;
    long long xl;

    if (n == 0) return 0;
    nl = (long long)n;
    x = n * 333; // rough initial guess
    if (x < 1) x = 1;

    for (i = 0; i < 100; i++) {
        prev = x;
        xl = (long long)x;
        // x_new = (2*x + n*10^9 / (x*x)) / 3
        x = (int)((2 * xl + nl * 1000000000LL / (xl * xl)) / 3);
        if (abs_val(x - prev) <= 1) break;
    }
    return x;
}

void test_cbrt(void) {
    int r;

    printf("=== Newton cbrt (x1000) ===\n"); // EXPECT: === Newton cbrt (x1000) ===

    r = cbrt_scaled(8);
    printf("cbrt(8)*1000 = %d\n", r); // EXPECT: cbrt(8)*1000 = 2000

    r = cbrt_scaled(27);
    printf("cbrt(27)*1000 = %d\n", r); // EXPECT: cbrt(27)*1000 = 3000

    r = cbrt_scaled(64);
    printf("cbrt(64)*1000 = %d\n", r); // EXPECT: cbrt(64)*1000 = 4000

    r = cbrt_scaled(1);
    printf("cbrt(1)*1000 = %d\n", r); // EXPECT: cbrt(1)*1000 = 1000

    r = cbrt_scaled(125);
    printf("cbrt(125)*1000 = %d\n", r); // EXPECT: cbrt(125)*1000 = 5000
}

int main(void) {
    test_sqrt();
    test_cbrt();
    printf("DONE\n"); // EXPECT: DONE
    return 0;
}
