// EXPECT: === Polynomial Operation Tests ===
// EXPECT: Evaluate p(x) = 3x^2 + 2x + 1:
// EXPECT:   p(0) = 1
// EXPECT:   p(1) = 6
// EXPECT:   p(2) = 17
// EXPECT:   p(3) = 34
// EXPECT:   p(-1) = 2
// EXPECT:   p(10) = 321
// EXPECT: Evaluate p(x) = x^3 - 2x + 5:
// EXPECT:   p(0) = 5
// EXPECT:   p(1) = 4
// EXPECT:   p(2) = 9
// EXPECT:   p(-1) = 6
// EXPECT:   p(3) = 26
// EXPECT: Horner vs direct: MATCH
// EXPECT: Polynomial addition:
// EXPECT:   (3x^2 + 2x + 1) + (x^2 - x + 4) = 4x^2 + 1x + 5
// EXPECT:   Verify sum at x=2: 4*4+1*2+5 = 23
// EXPECT:   Direct: 17 + 6 = 23: OK
// EXPECT: Polynomial multiply:
// EXPECT:   (x + 1) * (x - 1) = x^2 + 0x - 1
// EXPECT:   Verify at x=3: 8
// EXPECT:   Direct: 4 * 2 = 8: OK
// EXPECT:   (2x + 3) * (x^2 + 1) = 2x^3 + 3x^2 + 2x + 3
// EXPECT:   Verify at x=2: 35
// EXPECT: Polynomial derivative:
// EXPECT:   d/dx(3x^2 + 2x + 1) = 6x + 2
// EXPECT:   d/dx at x=0: 2
// EXPECT:   d/dx at x=1: 8
// EXPECT:   d/dx at x=5: 32
// EXPECT:   d/dx(x^4) = 4x^3
// EXPECT:   d/dx(x^4) at x=2: 32
// EXPECT:   d/dx(5) = 0
// EXPECT: All polynomial tests passed!

int printf(const char *fmt, ...);

// Polynomials stored as arrays of coefficients
// index i = coefficient of x^i
// e.g. 3x^2 + 2x + 1 = {1, 2, 3}

// Evaluate polynomial at x using Horner's method
int poly_eval(int *coeffs, int degree, int x) {
    int result = coeffs[degree];
    int i;
    for (i = degree - 1; i >= 0; i--) {
        result = result * x + coeffs[i];
    }
    return result;
}

// Direct evaluation (for verification)
int poly_eval_direct(int *coeffs, int degree, int x) {
    int result = 0;
    int i;
    for (i = 0; i <= degree; i++) {
        int power = 1;
        int j;
        for (j = 0; j < i; j++) {
            power = power * x;
        }
        result = result + coeffs[i] * power;
    }
    return result;
}

// Add two polynomials: result = a + b
// Returns degree of result
int poly_add(int *a, int da, int *b, int db, int *result) {
    int max_deg = da > db ? da : db;
    int i;
    for (i = 0; i <= max_deg; i++) {
        int va = (i <= da) ? a[i] : 0;
        int vb = (i <= db) ? b[i] : 0;
        result[i] = va + vb;
    }
    // Find actual degree
    int rd = max_deg;
    while (rd > 0 && result[rd] == 0) rd = rd - 1;
    return rd;
}

// Multiply two polynomials: result = a * b
// Returns degree of result
int poly_mul(int *a, int da, int *b, int db, int *result) {
    int rd = da + db;
    int i;
    for (i = 0; i <= rd; i++) {
        result[i] = 0;
    }
    for (i = 0; i <= da; i++) {
        int j;
        for (j = 0; j <= db; j++) {
            result[i + j] = result[i + j] + a[i] * b[j];
        }
    }
    while (rd > 0 && result[rd] == 0) rd = rd - 1;
    return rd;
}

// Derivative of polynomial
// Returns degree of derivative
int poly_derivative(int *coeffs, int degree, int *result) {
    if (degree == 0) {
        result[0] = 0;
        return 0;
    }
    int i;
    for (i = 1; i <= degree; i++) {
        result[i - 1] = coeffs[i] * i;
    }
    int rd = degree - 1;
    while (rd > 0 && result[rd] == 0) rd = rd - 1;
    return rd;
}

void print_poly(int *coeffs, int degree) {
    int i;
    for (i = degree; i >= 0; i--) {
        if (i == degree) {
            if (coeffs[i] != 1 || i == 0) printf("%d", coeffs[i]);
        } else if (i > 0) {
            if (coeffs[i] >= 0) printf(" + %d", coeffs[i]);
            else printf(" - %d", -coeffs[i]);
        } else {
            if (coeffs[i] >= 0) printf(" + %d", coeffs[i]);
            else printf(" - %d", -coeffs[i]);
        }
        if (i > 1) printf("x^%d", i);
        else if (i == 1) printf("x");
    }
}

int main() {
    printf("=== Polynomial Operation Tests ===\n");

    // p(x) = 3x^2 + 2x + 1 = {1, 2, 3}
    int p1[3];
    p1[0] = 1; p1[1] = 2; p1[2] = 3;
    printf("Evaluate p(x) = 3x^2 + 2x + 1:\n");
    printf("  p(0) = %d\n", poly_eval(p1, 2, 0));
    printf("  p(1) = %d\n", poly_eval(p1, 2, 1));
    printf("  p(2) = %d\n", poly_eval(p1, 2, 2));
    printf("  p(3) = %d\n", poly_eval(p1, 2, 3));
    printf("  p(-1) = %d\n", poly_eval(p1, 2, -1));
    printf("  p(10) = %d\n", poly_eval(p1, 2, 10));

    // p(x) = x^3 - 2x + 5 = {5, -2, 0, 1}
    int p2[4];
    p2[0] = 5; p2[1] = -2; p2[2] = 0; p2[3] = 1;
    printf("Evaluate p(x) = x^3 - 2x + 5:\n");
    printf("  p(0) = %d\n", poly_eval(p2, 3, 0));
    printf("  p(1) = %d\n", poly_eval(p2, 3, 1));
    printf("  p(2) = %d\n", poly_eval(p2, 3, 2));
    printf("  p(-1) = %d\n", poly_eval(p2, 3, -1));
    printf("  p(3) = %d\n", poly_eval(p2, 3, 3));

    // Verify Horner vs direct
    int match = 1;
    int x;
    for (x = -5; x <= 5; x++) {
        if (poly_eval(p1, 2, x) != poly_eval_direct(p1, 2, x)) match = 0;
        if (poly_eval(p2, 3, x) != poly_eval_direct(p2, 3, x)) match = 0;
    }
    printf("Horner vs direct: %s\n", match ? "MATCH" : "MISMATCH");

    // Addition: (3x^2 + 2x + 1) + (x^2 - x + 4)
    int p3[3];
    p3[0] = 4; p3[1] = -1; p3[2] = 1;
    int sum[10];
    int sd = poly_add(p1, 2, p3, 2, sum);
    printf("Polynomial addition:\n");
    printf("  (3x^2 + 2x + 1) + (x^2 - x + 4) = %dx^2 + %dx + %d\n",
           sum[2], sum[1], sum[0]);
    int val_sum = poly_eval(sum, sd, 2);
    printf("  Verify sum at x=2: %d*4+%d*2+%d = %d\n", sum[2], sum[1], sum[0], val_sum);
    int val_p1 = poly_eval(p1, 2, 2);
    int val_p3 = poly_eval(p3, 2, 2);
    printf("  Direct: %d + %d = %d: %s\n", val_p1, val_p3, val_p1 + val_p3,
           val_sum == val_p1 + val_p3 ? "OK" : "FAIL");

    // Multiply: (x+1) * (x-1) = x^2 - 1
    printf("Polynomial multiply:\n");
    int a[2]; a[0] = 1; a[1] = 1;   // x+1
    int b[2]; b[0] = -1; b[1] = 1;  // x-1
    int prod[10];
    int pd = poly_mul(a, 1, b, 1, prod);
    printf("  (x + 1) * (x - 1) = x^2 + %dx - %d\n", prod[1], -prod[0]);
    int vp = poly_eval(prod, pd, 3);
    printf("  Verify at x=3: %d\n", vp);
    int va = poly_eval(a, 1, 3);
    int vb = poly_eval(b, 1, 3);
    printf("  Direct: %d * %d = %d: %s\n", va, vb, va * vb,
           vp == va * vb ? "OK" : "FAIL");

    // (2x+3) * (x^2+1) = 2x^3 + 3x^2 + 2x + 3
    int c[2]; c[0] = 3; c[1] = 2;       // 2x+3
    int d[3]; d[0] = 1; d[1] = 0; d[2] = 1; // x^2+1
    int prod2[10];
    int pd2 = poly_mul(c, 1, d, 2, prod2);
    printf("  (2x + 3) * (x^2 + 1) = %dx^3 + %dx^2 + %dx + %d\n",
           prod2[3], prod2[2], prod2[1], prod2[0]);
    printf("  Verify at x=2: %d\n", poly_eval(prod2, pd2, 2));

    // Derivative
    printf("Polynomial derivative:\n");
    int deriv[10];
    int dd = poly_derivative(p1, 2, deriv);
    printf("  d/dx(3x^2 + 2x + 1) = %dx + %d\n", deriv[1], deriv[0]);
    printf("  d/dx at x=0: %d\n", poly_eval(deriv, dd, 0));
    printf("  d/dx at x=1: %d\n", poly_eval(deriv, dd, 1));
    printf("  d/dx at x=5: %d\n", poly_eval(deriv, dd, 5));

    // d/dx(x^4) = 4x^3
    int p4[5];
    p4[0] = 0; p4[1] = 0; p4[2] = 0; p4[3] = 0; p4[4] = 1;
    int dd2 = poly_derivative(p4, 4, deriv);
    printf("  d/dx(x^4) = %dx^3\n", deriv[3]);
    printf("  d/dx(x^4) at x=2: %d\n", poly_eval(deriv, dd2, 2));

    // d/dx(5) = 0
    int p5[1]; p5[0] = 5;
    poly_derivative(p5, 0, deriv);
    printf("  d/dx(5) = %d\n", deriv[0]);

    printf("All polynomial tests passed!\n");
    return 0;
}
