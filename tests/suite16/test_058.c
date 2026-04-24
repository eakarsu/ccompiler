int printf(const char *fmt, ...);

// Numerical integration using trapezoidal and Simpson's rule
// All values scaled by 1000

// f(x) = x^2, x in x1000, result in x1000
int f_square(int x) {
    return (int)(((long long)x * x) / 1000);
}

// f(x) = x, x in x1000, result in x1000
int f_linear(int x) {
    return x;
}

// f(x) = x^3 (result in x1000 when x is x1000)
// x^3 in x1000: (x*x/1000)*(x)/1000
int f_cube(int x) {
    long long xl;
    xl = (long long)x;
    return (int)(xl * xl / 1000 * xl / 1000);
}

// f(x) = 3x^2 + 2x + 1 (x in x1000, result in x1000)
int f_poly(int x) {
    long long xl;
    xl = (long long)x;
    return (int)(3 * xl * xl / 1000 + 2 * x + 1000);
}

// Trapezoidal rule: integrate f from a to b with n steps
// a, b in x1000, result in x1000
int trapezoid(int (*f)(int), int a, int b, int n) {
    long long sum;
    int h;
    int i;
    int x;

    h = (b - a) / n;
    sum = (long long)f(a) + f(b);
    for (i = 1; i < n; i++) {
        x = a + h * i;
        sum = sum + 2 * (long long)f(x);
    }
    return (int)(sum * h / 2000);
}

// Simpson's rule: integrate f from a to b with n steps (n must be even)
// a, b in x1000, result in x1000
int simpson(int (*f)(int), int a, int b, int n) {
    long long sum;
    int h;
    int i;
    int x;

    h = (b - a) / n;
    sum = (long long)f(a) + f(b);
    for (i = 1; i < n; i++) {
        x = a + h * i;
        if (i % 2 == 0) {
            sum = sum + 2 * (long long)f(x);
        } else {
            sum = sum + 4 * (long long)f(x);
        }
    }
    return (int)(sum * h / 3000);
}

void test_linear(void) {
    int result;
    printf("=== Integrate x ===\n"); // EXPECT: === Integrate x ===

    // integral of x from 0 to 1 = 0.5 -> 500
    result = trapezoid(f_linear, 0, 1000, 100);
    printf("trap x [0,1] n=100: %d\n", result); // EXPECT: trap x [0,1] n=100: 500

    result = simpson(f_linear, 0, 1000, 100);
    printf("simp x [0,1] n=100: %d\n", result); // EXPECT: simp x [0,1] n=100: 500

    // integral of x from 0 to 2 = 2 -> 2000
    result = trapezoid(f_linear, 0, 2000, 100);
    printf("trap x [0,2] n=100: %d\n", result); // EXPECT: trap x [0,2] n=100: 2000

    result = simpson(f_linear, 0, 2000, 100);
    printf("simp x [0,2] n=100: %d\n", result); // EXPECT: simp x [0,2] n=100: 2000
}

void test_square(void) {
    int result;
    printf("=== Integrate x^2 ===\n"); // EXPECT: === Integrate x^2 ===

    // integral of x^2 from 0 to 1 = 1/3 ~ 333
    result = trapezoid(f_square, 0, 1000, 10);
    printf("trap x^2 [0,1] n=10: %d\n", result); // EXPECT: trap x^2 [0,1] n=10: 335

    result = trapezoid(f_square, 0, 1000, 100);
    printf("trap x^2 [0,1] n=100: %d\n", result); // EXPECT: trap x^2 [0,1] n=100: 332

    result = simpson(f_square, 0, 1000, 10);
    printf("simp x^2 [0,1] n=10: %d\n", result); // EXPECT: simp x^2 [0,1] n=10: 333

    result = simpson(f_square, 0, 1000, 100);
    printf("simp x^2 [0,1] n=100: %d\n", result); // EXPECT: simp x^2 [0,1] n=100: 332

    // integral of x^2 from 0 to 3 = 9 -> 9000
    result = trapezoid(f_square, 0, 3000, 100);
    printf("trap x^2 [0,3] n=100: %d\n", result); // EXPECT: trap x^2 [0,3] n=100: 8999

    result = simpson(f_square, 0, 3000, 100);
    printf("simp x^2 [0,3] n=100: %d\n", result); // EXPECT: simp x^2 [0,3] n=100: 8998
}

void test_cube(void) {
    int result;
    printf("=== Integrate x^3 ===\n"); // EXPECT: === Integrate x^3 ===

    // integral of x^3 from 0 to 1 = 0.25 -> 250
    result = trapezoid(f_cube, 0, 1000, 100);
    printf("trap x^3 [0,1] n=100: %d\n", result); // EXPECT: trap x^3 [0,1] n=100: 249

    result = simpson(f_cube, 0, 1000, 100);
    printf("simp x^3 [0,1] n=100: %d\n", result); // EXPECT: simp x^3 [0,1] n=100: 249

    // integral of x^3 from 0 to 2 = 4 -> 4000
    result = trapezoid(f_cube, 0, 2000, 100);
    printf("trap x^3 [0,2] n=100: %d\n", result); // EXPECT: trap x^3 [0,2] n=100: 3998

    result = simpson(f_cube, 0, 2000, 100);
    printf("simp x^3 [0,2] n=100: %d\n", result); // EXPECT: simp x^3 [0,2] n=100: 3998
}

void test_poly(void) {
    int result;
    printf("=== Integrate 3x^2+2x+1 ===\n"); // EXPECT: === Integrate 3x^2+2x+1 ===

    // integral of 3x^2+2x+1 from 0 to 1 = 1+1+1 = 3 -> 3000
    result = trapezoid(f_poly, 0, 1000, 100);
    printf("trap poly [0,1] n=100: %d\n", result); // EXPECT: trap poly [0,1] n=100: 2999

    result = simpson(f_poly, 0, 1000, 100);
    printf("simp poly [0,1] n=100: %d\n", result); // EXPECT: simp poly [0,1] n=100: 2999

    // integral from 0 to 2 = 8+4+2 = 14 -> 14000
    result = trapezoid(f_poly, 0, 2000, 100);
    printf("trap poly [0,2] n=100: %d\n", result); // EXPECT: trap poly [0,2] n=100: 13999

    result = simpson(f_poly, 0, 2000, 100);
    printf("simp poly [0,2] n=100: %d\n", result); // EXPECT: simp poly [0,2] n=100: 13999
}

int main(void) {
    test_linear();
    test_square();
    test_cube();
    test_poly();
    printf("DONE\n"); // EXPECT: DONE
    return 0;
}
