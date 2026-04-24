int printf(const char *fmt, ...);

// Fixed-point iteration and numerical differentiation with x1000 scaling

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

// ============================================================
// Fixed-point iteration: find x such that g(x) = x
// g(x) = (x + N*1000/x) / 2  converges to sqrt(N) (Babylonian method)
// x is x1000, returns x1000
int fp_sqrt(int n) {
    int x;
    int prev;
    int i;

    x = n * 500; // initial guess: n/2 * 1000
    if (x == 0) x = 1000;

    for (i = 0; i < 50; i++) {
        prev = x;
        x = (x + (int)((long long)n * 1000000 / x)) / 2;
        if (abs_val(x - prev) <= 1) break;
    }
    return x;
}

// Fixed point: g(x) = (x^2 + N*1000) / (2*x) for sqrt(N)
// Same as above but written differently
int fp_sqrt2(int n) {
    int x;
    int prev;
    int i;

    x = n * 1000;
    if (x == 0) return 0;

    for (i = 0; i < 50; i++) {
        prev = x;
        x = (int)(((long long)x * x / 1000 + (long long)n * 1000) * 1000 / (2 * x));
        if (abs_val(x - prev) <= 1) break;
    }
    return x;
}

// ============================================================
// Numerical differentiation using central difference
// f'(x) ~ (f(x+h) - f(x-h)) / (2*h)
// All x1000

int f_square(int x) {
    return (int)(((long long)x * x) / 1000);
}

int f_cube(int x) {
    long long xl;
    xl = (long long)x;
    return (int)(xl * xl / 1000 * xl / 1000);
}

// Numerical derivative of f at x, using step h (all x1000)
int num_deriv(int (*f)(int), int x, int h) {
    int fp;
    int fm;
    fp = f(x + h);
    fm = f(x - h);
    return (int)(((long long)fp - fm) * 1000 / (2 * h));
}

// Second derivative using central difference
// f''(x) ~ (f(x+h) - 2*f(x) + f(x-h)) / h^2
int num_deriv2(int (*f)(int), int x, int h) {
    int fp;
    int fc;
    int fm;
    fp = f(x + h);
    fc = f(x);
    fm = f(x - h);
    return (int)(((long long)fp - 2 * (long long)fc + fm) * 1000 * 1000 / ((long long)h * h));
}

// ============================================================
// Newton's method with numerical derivative
int newton_numerical(int (*f)(int), int x0, int tol) {
    int x;
    int fx;
    int fpx;
    int i;
    int h;

    x = x0;
    h = 1; // small step for derivative (0.001 in real)

    for (i = 0; i < 100; i++) {
        fx = f(x);
        if (abs_val(fx) < tol) break;
        fpx = num_deriv(f, x, h);
        if (fpx == 0) break;
        x = x - (int)((long long)fx * 1000 / fpx);
    }
    return x;
}

// f(x) = x^2 - 4 (roots at x = +/- 2)
int f_sq_minus4(int x) {
    return (int)(((long long)x * x) / 1000 - 4000);
}

// f(x) = x^2 - x - 2 = (x-2)(x+1) (roots at x=2, x=-1)
int f_quad(int x) {
    return (int)(((long long)x * x) / 1000 - x - 2000);
}

void test_fixed_point_sqrt(void) {
    int r;
    printf("=== Fixed-Point sqrt ===\n"); // EXPECT: === Fixed-Point sqrt ===

    r = fp_sqrt(2);
    printf("sqrt(2)*1000 = %d\n", r); // EXPECT: sqrt(2)*1000 = 1414

    r = fp_sqrt(9);
    printf("sqrt(9)*1000 = %d\n", r); // EXPECT: sqrt(9)*1000 = 3000

    r = fp_sqrt(25);
    printf("sqrt(25)*1000 = %d\n", r); // EXPECT: sqrt(25)*1000 = 5000

    r = fp_sqrt2(4);
    printf("sqrt2(4)*1000 = %d\n", r); // EXPECT: sqrt2(4)*1000 = 2000

    r = fp_sqrt2(16);
    printf("sqrt2(16)*1000 = %d\n", r); // EXPECT: sqrt2(16)*1000 = 4000
}

void test_numerical_deriv(void) {
    int d;
    printf("=== Numerical Derivative ===\n"); // EXPECT: === Numerical Derivative ===

    // d/dx(x^2) = 2x
    // At x=3 (3000), derivative = 6 (6000)
    d = num_deriv(f_square, 3000, 1);
    printf("d(x^2)/dx at 3 = %d\n", d); // EXPECT: d(x^2)/dx at 3 = 6000

    // At x=5 (5000), derivative = 10 (10000)
    d = num_deriv(f_square, 5000, 1);
    printf("d(x^2)/dx at 5 = %d\n", d); // EXPECT: d(x^2)/dx at 5 = 10000

    // d/dx(x^3) = 3x^2
    // At x=2 (2000), derivative = 12 (12000)
    d = num_deriv(f_cube, 2000, 1);
    printf("d(x^3)/dx at 2 = %d\n", d); // EXPECT: d(x^3)/dx at 2 = 12000

    // Second derivative of x^2 = 2 (2000)
    d = num_deriv2(f_square, 3000, 100);
    printf("d2(x^2)/dx2 at 3 = %d\n", d); // EXPECT: d2(x^2)/dx2 at 3 = 2000

    // Second derivative of x^3 = 6x, at x=2 = 12 (12000)
    d = num_deriv2(f_cube, 2000, 100);
    printf("d2(x^3)/dx2 at 2 = %d\n", d); // EXPECT: d2(x^3)/dx2 at 2 = 12000
}

void test_newton_numerical(void) {
    int root;
    printf("=== Newton with Numerical Deriv ===\n"); // EXPECT: === Newton with Numerical Deriv ===

    // x^2 - 4 = 0, start from x=3
    root = newton_numerical(f_sq_minus4, 3000, 1);
    printf("root of x^2-4 near 3: %d\n", root); // EXPECT: root of x^2-4 near 3: 2000

    // x^2 - 4 = 0, start from x=-3
    root = newton_numerical(f_sq_minus4, -3000, 1);
    printf("root of x^2-4 near -3: %d\n", root); // EXPECT: root of x^2-4 near -3: -2000

    // (x-2)(x+1) = 0, start from x=3
    root = newton_numerical(f_quad, 3000, 1);
    printf("root of x^2-x-2 near 3: %d\n", root); // EXPECT: root of x^2-x-2 near 3: 2000

    // (x-2)(x+1) = 0, start from x=-3
    root = newton_numerical(f_quad, -3000, 1);
    printf("root of x^2-x-2 near -3: %d\n", root); // EXPECT: root of x^2-x-2 near -3: -1000
}

int main(void) {
    test_fixed_point_sqrt();
    test_numerical_deriv();
    test_newton_numerical();
    printf("DONE\n"); // EXPECT: DONE
    return 0;
}
