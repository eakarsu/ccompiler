int printf(const char *fmt, ...);

// Polynomial evaluation using Horner's method with x1000 scaling
// Coefficients and x are all scaled by 1000

// Horner's method: evaluate polynomial a[0] + a[1]*x + a[2]*x^2 + ...
// All values scaled by 1000
// Result: evaluated value in x1000
int horner_eval(int coeffs[], int degree, int x) {
    long long result;
    int i;

    result = (long long)coeffs[degree];
    for (i = degree - 1; i >= 0; i--) {
        result = result * x / 1000 + coeffs[i];
    }
    return (int)result;
}

// Polynomial addition: c = a + b (both degree n)
void poly_add(int a[], int b[], int c[], int n) {
    int i;
    for (i = 0; i <= n; i++) {
        c[i] = a[i] + b[i];
    }
}

// Polynomial subtraction: c = a - b (both degree n)
void poly_sub(int a[], int b[], int c[], int n) {
    int i;
    for (i = 0; i <= n; i++) {
        c[i] = a[i] - b[i];
    }
}

// Polynomial multiplication: c = a * b
// a has degree da, b has degree db
// c has degree da+db, result values scaled x1000
void poly_mul(int a[], int da, int b[], int db, int c[]) {
    int i;
    int j;

    for (i = 0; i <= da + db; i++) {
        c[i] = 0;
    }
    for (i = 0; i <= da; i++) {
        for (j = 0; j <= db; j++) {
            c[i + j] = c[i + j] + (int)(((long long)a[i] * b[j]) / 1000);
        }
    }
}

// Polynomial derivative: b = a' (degree n-1)
// If a = c0 + c1*x + c2*x^2 + ...
// a' = c1 + 2*c2*x + 3*c3*x^2 + ...
void poly_deriv(int a[], int n, int b[]) {
    int i;
    for (i = 1; i <= n; i++) {
        b[i - 1] = a[i] * i;
    }
}

void print_poly(int coeffs[], int degree) {
    int i;
    for (i = 0; i <= degree; i++) {
        if (i > 0) printf(" + ");
        printf("%d*x^%d", coeffs[i], i);
    }
    printf("\n");
}

void test_horner(void) {
    int p[4];
    int val;

    printf("=== Horner's Method (x1000) ===\n"); // EXPECT: === Horner's Method (x1000) ===

    // p(x) = 1 + 2x + 3x^2 (scaled: 1000 + 2000*x + 3000*x^2)
    p[0] = 1000;
    p[1] = 2000;
    p[2] = 3000;

    // p(0) = 1000
    val = horner_eval(p, 2, 0);
    printf("p(0) = %d\n", val); // EXPECT: p(0) = 1000

    // p(1) = 1+2+3 = 6 -> 6000
    val = horner_eval(p, 2, 1000);
    printf("p(1) = %d\n", val); // EXPECT: p(1) = 6000

    // p(2) = 1+4+12 = 17 -> 17000
    val = horner_eval(p, 2, 2000);
    printf("p(2) = %d\n", val); // EXPECT: p(2) = 17000

    // p(-1) = 1-2+3 = 2 -> 2000
    val = horner_eval(p, 2, -1000);
    printf("p(-1) = %d\n", val); // EXPECT: p(-1) = 2000

    // p(3) = 1+6+27 = 34 -> 34000
    val = horner_eval(p, 2, 3000);
    printf("p(3) = %d\n", val); // EXPECT: p(3) = 34000
}

void test_poly_ops(void) {
    int a[3];
    int b[3];
    int c[3];

    printf("=== Poly Add/Sub ===\n"); // EXPECT: === Poly Add/Sub ===

    // a = 1 + 2x + 3x^2
    a[0] = 1000; a[1] = 2000; a[2] = 3000;
    // b = 4 + 5x + 6x^2
    b[0] = 4000; b[1] = 5000; b[2] = 6000;

    poly_add(a, b, c, 2);
    printf("add: %d %d %d\n", c[0], c[1], c[2]); // EXPECT: add: 5000 7000 9000

    poly_sub(a, b, c, 2);
    printf("sub: %d %d %d\n", c[0], c[1], c[2]); // EXPECT: sub: -3000 -3000 -3000
}

void test_poly_mul(void) {
    int a[2];
    int b[2];
    int c[3];
    int val;

    printf("=== Poly Multiply ===\n"); // EXPECT: === Poly Multiply ===

    // (1 + x) * (1 + x) = 1 + 2x + x^2
    a[0] = 1000; a[1] = 1000;
    b[0] = 1000; b[1] = 1000;

    poly_mul(a, 1, b, 1, c);
    printf("(1+x)^2 coeffs: %d %d %d\n", c[0], c[1], c[2]); // EXPECT: (1+x)^2 coeffs: 1000 2000 1000

    // Evaluate at x=2: 1+4+4 = 9 -> 9000
    val = horner_eval(c, 2, 2000);
    printf("(1+x)^2 at x=2: %d\n", val); // EXPECT: (1+x)^2 at x=2: 9000

    // (2 + 3x) * (1 - x) = 2 + x - 3x^2
    a[0] = 2000; a[1] = 3000;
    b[0] = 1000; b[1] = -1000;

    poly_mul(a, 1, b, 1, c);
    printf("(2+3x)(1-x) coeffs: %d %d %d\n", c[0], c[1], c[2]); // EXPECT: (2+3x)(1-x) coeffs: 2000 1000 -3000

    // Evaluate at x=1: 2+1-3 = 0
    val = horner_eval(c, 2, 1000);
    printf("(2+3x)(1-x) at x=1: %d\n", val); // EXPECT: (2+3x)(1-x) at x=1: 0
}

void test_derivative(void) {
    int a[4];
    int da[3];
    int val;

    printf("=== Poly Derivative ===\n"); // EXPECT: === Poly Derivative ===

    // a = 5 + 3x + 2x^2 + x^3
    // a' = 3 + 4x + 3x^2
    a[0] = 5000; a[1] = 3000; a[2] = 2000; a[3] = 1000;

    poly_deriv(a, 3, da);
    printf("deriv coeffs: %d %d %d\n", da[0], da[1], da[2]); // EXPECT: deriv coeffs: 3000 4000 3000

    // Evaluate derivative at x=0: 3 -> 3000
    val = horner_eval(da, 2, 0);
    printf("a'(0) = %d\n", val); // EXPECT: a'(0) = 3000

    // Evaluate derivative at x=1: 3+4+3 = 10 -> 10000
    val = horner_eval(da, 2, 1000);
    printf("a'(1) = %d\n", val); // EXPECT: a'(1) = 10000

    // Evaluate original at x=1: 5+3+2+1 = 11 -> 11000
    val = horner_eval(a, 3, 1000);
    printf("a(1) = %d\n", val); // EXPECT: a(1) = 11000
}

int main(void) {
    test_horner();
    test_poly_ops();
    test_poly_mul();
    test_derivative();
    printf("DONE\n"); // EXPECT: DONE
    return 0;
}
