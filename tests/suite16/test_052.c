int printf(const char *fmt, ...);

// Bisection method for finding roots of polynomials
// All values scaled by 1000

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

// Evaluate f(x) = x^2 - 2000 (i.e., x^2 - 2 in real, scaled x1000)
// Input x is scaled by 1000, output scaled by 1000
// f(x) = (x*x)/1000 - 2*1000
int f_sq_minus2(int x) {
    return (int)(((long long)x * x) / 1000 - 2000);
}

// Evaluate f(x) = x^2 - 5 scaled by 1000
// x is scaled x1000, result scaled x1000
// f(x) = x^2/1000 - 5000
int f_sq_minus5(int x) {
    return (int)(((long long)x * x) / 1000 - 5000);
}

// Evaluate f(x) = x^2 - 3*x + 2 = (x-1)(x-2) scaled x1000
// f(x) = x^2/1000 - 3*x + 2000
int f_quadratic(int x) {
    return (int)(((long long)x * x) / 1000 - 3 * x + 2000);
}

// Bisection method: find root of f in [lo, hi]
// lo, hi scaled by 1000; returns root scaled by 1000
// f is a function pointer
int bisect(int lo, int hi, int (*f)(int), int tol) {
    int mid;
    int flo;
    int fmid;
    int i;

    for (i = 0; i < 100; i++) {
        mid = (lo + hi) / 2;
        if (abs_val(hi - lo) < tol) break;
        flo = f(lo);
        fmid = f(mid);
        if (flo > 0 && fmid > 0) {
            lo = mid;
        } else if (flo < 0 && fmid < 0) {
            lo = mid;
        } else {
            hi = mid;
        }
    }
    return mid;
}

void test_sqrt2(void) {
    int root;
    printf("=== Bisection: sqrt(2) ===\n"); // EXPECT: === Bisection: sqrt(2) ===

    // sqrt(2) is in [1, 2] -> [1000, 2000]
    root = bisect(1000, 2000, f_sq_minus2, 1);
    printf("root of x^2-2 = %d\n", root); // EXPECT: root of x^2-2 = 1414

    // Verify: 1414*1414/1000 = 1999 ~ 2000
    int check;
    check = (int)(((long long)root * root) / 1000);
    printf("verify: %d^2/1000 = %d\n", root, check); // EXPECT: verify: 1414^2/1000 = 1999
}

void test_quadratic_root(void) {
    int root;
    printf("=== Bisection: x^2-3x+2 ===\n"); // EXPECT: === Bisection: x^2-3x+2 ===

    // Root at x=1 (1000 scaled), search in [0, 1500]
    root = bisect(0, 1500, f_quadratic, 1);
    printf("root near 1: %d\n", root); // EXPECT: root near 1: 999

    // Root at x=2 (2000 scaled), search in [1500, 3000]
    root = bisect(1500, 3000, f_quadratic, 1);
    printf("root near 2: %d\n", root); // EXPECT: root near 2: 1999
}

void test_sqrt5(void) {
    int root;
    int fval;
    printf("=== Bisection: sqrt(5) ===\n"); // EXPECT: === Bisection: sqrt(5) ===

    // sqrt(5) ~ 2.236 -> in [2000, 3000]
    root = bisect(2000, 3000, f_sq_minus5, 1);
    printf("root of x^2-5 = %d\n", root); // EXPECT: root of x^2-5 = 2236

    fval = f_sq_minus5(root);
    printf("f(%d) = %d\n", root, fval); // EXPECT: f(2236) = -1
}

// Bisection for sqrt(N) using a simple approach
int sqrt_bisect(int n) {
    int lo;
    int hi;
    int mid;
    int sq;
    int i;

    lo = 0;
    hi = n * 1000;
    for (i = 0; i < 50; i++) {
        mid = (lo + hi) / 2;
        sq = (int)(((long long)mid * mid) / 1000);
        if (sq < n * 1000) {
            lo = mid;
        } else if (sq > n * 1000) {
            hi = mid;
        } else {
            break;
        }
        if (hi - lo <= 1) {
            mid = lo;
            break;
        }
    }
    return mid;
}

void test_sqrt_bisect(void) {
    int r;
    printf("=== Bisection sqrt ===\n"); // EXPECT: === Bisection sqrt ===

    r = sqrt_bisect(4);
    printf("sqrt(4)*1000 = %d\n", r); // EXPECT: sqrt(4)*1000 = 2000

    r = sqrt_bisect(9);
    printf("sqrt(9)*1000 = %d\n", r); // EXPECT: sqrt(9)*1000 = 3000

    r = sqrt_bisect(2);
    printf("sqrt(2)*1000 = %d\n", r); // EXPECT: sqrt(2)*1000 = 1414

    r = sqrt_bisect(5);
    printf("sqrt(5)*1000 = %d\n", r); // EXPECT: sqrt(5)*1000 = 2236
}

int main(void) {
    test_sqrt2();
    test_quadratic_root();
    test_sqrt5();
    test_sqrt_bisect();
    printf("DONE\n"); // EXPECT: DONE
    return 0;
}
