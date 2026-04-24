int printf(const char *fmt, ...);
// EXPECT: p(x) = 2x^3+3x^2-x+5\np(0) = 5\np(1) = 9\np(2) = 31\np(-1) = 7\nq(x) = x^2-2x+1\np+q = 2x^3+4x^2-3x+6\n(x+1)(x-1) = x^2-1\neval at x=3: 8\np'(x) = 6x^2+6x-1\np'(1) = 11\n(x+2)^2 = x^2+4x+4
// Test: polynomial evaluation and operations

void print_poly(int *coeffs, int degree) {
    int i;
    int first = 1;
    for (i = degree; i >= 0; i--) {
        if (coeffs[i] == 0) continue;
        if (!first && coeffs[i] > 0) printf("+");
        if (i == 0) {
            printf("%d", coeffs[i]);
        } else if (i == 1) {
            if (coeffs[i] == 1) printf("x");
            else if (coeffs[i] == -1) printf("-x");
            else printf("%dx", coeffs[i]);
        } else {
            if (coeffs[i] == 1) printf("x^%d", i);
            else if (coeffs[i] == -1) printf("-x^%d", i);
            else printf("%dx^%d", coeffs[i], i);
        }
        first = 0;
    }
    if (first) printf("0");
    printf("\n");
}

// Evaluate polynomial using Horner's method
// coeffs[i] = coefficient of x^i
int eval_horner(int *coeffs, int degree, int x) {
    int result = coeffs[degree];
    int i;
    for (i = degree - 1; i >= 0; i--) {
        result = result * x + coeffs[i];
    }
    return result;
}

// Add two polynomials
int poly_add(int *a, int da, int *b, int db, int *c) {
    int max_d = da;
    if (db > max_d) max_d = db;
    int i;
    for (i = 0; i <= max_d; i++) {
        int av = (i <= da) ? a[i] : 0;
        int bv = (i <= db) ? b[i] : 0;
        c[i] = av + bv;
    }
    return max_d;
}

// Multiply two polynomials
int poly_mul(int *a, int da, int *b, int db, int *c) {
    int dc = da + db;
    int i;
    for (i = 0; i <= dc; i++) c[i] = 0;
    int j;
    for (i = 0; i <= da; i++) {
        for (j = 0; j <= db; j++) {
            c[i + j] = c[i + j] + a[i] * b[j];
        }
    }
    return dc;
}

// Derivative
int poly_deriv(int *p, int degree, int *d) {
    if (degree == 0) { d[0] = 0; return 0; }
    int i;
    for (i = 1; i <= degree; i++) {
        d[i - 1] = p[i] * i;
    }
    return degree - 1;
}

int main(void) {
    // p(x) = 2x^3 + 3x^2 - x + 5
    int p[4]; p[0]=5; p[1]=-1; p[2]=3; p[3]=2;
    printf("p(x) = ");
    print_poly(p, 3);

    // Evaluate
    printf("p(0) = %d\n", eval_horner(p, 3, 0));
    printf("p(1) = %d\n", eval_horner(p, 3, 1));
    printf("p(2) = %d\n", eval_horner(p, 3, 2));
    printf("p(-1) = %d\n", eval_horner(p, 3, -1));

    // q(x) = x^2 - 2x + 1
    int q[3]; q[0]=1; q[1]=-2; q[2]=1;
    printf("q(x) = ");
    print_poly(q, 2);

    // Add
    int sum[4];
    int sd = poly_add(p, 3, q, 2, sum);
    printf("p+q = ");
    print_poly(sum, sd);

    // Multiply (x+1)(x-1) = x^2 - 1
    int m1[2]; m1[0]=1; m1[1]=1;  // x+1
    int m2[2]; m2[0]=-1; m2[1]=1; // x-1
    int prod[3];
    int pd = poly_mul(m1, 1, m2, 1, prod);
    printf("(x+1)(x-1) = ");
    print_poly(prod, pd);
    printf("eval at x=3: %d\n", eval_horner(prod, pd, 3));

    // Derivative
    int dp[3];
    int dd = poly_deriv(p, 3, dp);
    printf("p'(x) = ");
    print_poly(dp, dd);
    printf("p'(1) = %d\n", eval_horner(dp, dd, 1));

    // (x+2)^2 = x^2 + 4x + 4
    int sq1[2]; sq1[0]=2; sq1[1]=1;
    int sq_result[3];
    int sqd = poly_mul(sq1, 1, sq1, 1, sq_result);
    printf("(x+2)^2 = ");
    print_poly(sq_result, sqd);

    return 0;
}
