int printf(const char *fmt, ...);
// EXPECT: (2x + 3) * (x^2 - x + 1) = 2x^3 + x^2 - x + 3\n\nVerification:\nx=0: (3)*(1) = 3, product=3, match=1\nx=1: (5)*(1) = 5, product=5, match=1\nx=2: (7)*(3) = 21, product=21, match=1\nx=3: (9)*(7) = 63, product=63, match=1\n\n(x+1)^2 = x^2 + 2x + 1\n(x+1)^4 = x^4 + 4x^3 + 6x^2 + 4x + 1\n(x-1)(x+1) = x^2 - 1\n(x-1)(x^2+x+1) = x^3 - 1\n\nProduct values at x=-2..2:\n(x-1)(x^2+x+1) at x=-2: -9\n(x-1)(x^2+x+1) at x=-1: -2\n(x-1)(x^2+x+1) at x=0: -1\n(x-1)(x^2+x+1) at x=1: 0\n(x-1)(x^2+x+1) at x=2: 7
// Test: Polynomial multiplication

void print_poly(int c[], int deg) {
    int first = 1;
    int i;
    for (i = deg; i >= 0; i--) {
        if (c[i] == 0) continue;
        if (!first && c[i] > 0) printf(" + ");
        if (!first && c[i] < 0) printf(" - ");
        if (first && c[i] < 0) printf("-");
        first = 0;
        int v = c[i];
        if (v < 0) v = -v;
        if (i == 0) printf("%d", v);
        else if (i == 1) {
            if (v == 1) printf("x");
            else printf("%dx", v);
        } else {
            if (v == 1) printf("x^%d", i);
            else printf("%dx^%d", v, i);
        }
    }
    if (first) printf("0");
}

int eval_poly(int c[], int deg, int x) {
    int result = c[deg];
    int i;
    for (i = deg - 1; i >= 0; i--) {
        result = result * x + c[i];
    }
    return result;
}

int main(void) {
    int i, j;

    // Multiply (2x + 3) * (x^2 - x + 1)
    int a[3], b[4], product[10];
    a[0] = 3; a[1] = 2;
    b[0] = 1; b[1] = -1; b[2] = 1;
    int deg_a = 1, deg_b = 2;
    int deg_p = deg_a + deg_b;

    for (i = 0; i < 10; i++) product[i] = 0;
    for (i = 0; i <= deg_a; i++) {
        for (j = 0; j <= deg_b; j++) {
            product[i + j] = product[i + j] + a[i] * b[j];
        }
    }

    printf("(");
    print_poly(a, deg_a);
    printf(") * (");
    print_poly(b, deg_b);
    printf(") = ");
    print_poly(product, deg_p);
    printf("\n");

    // Verify at x = 0, 1, 2, 3
    printf("\nVerification:\n");
    for (i = 0; i <= 3; i++) {
        int va = eval_poly(a, deg_a, i);
        int vb = eval_poly(b, deg_b, i);
        int vp = eval_poly(product, deg_p, i);
        printf("x=%d: (%d)*(%d) = %d, product=%d, match=%d\n",
               i, va, vb, va * vb, vp, va * vb == vp);
    }

    // Multiply (x + 1)^2 = x^2 + 2x + 1
    int c[2], sq[10];
    c[0] = 1; c[1] = 1;
    for (i = 0; i < 10; i++) sq[i] = 0;
    for (i = 0; i <= 1; i++) {
        for (j = 0; j <= 1; j++) {
            sq[i + j] = sq[i + j] + c[i] * c[j];
        }
    }
    printf("\n(x+1)^2 = ");
    print_poly(sq, 2);
    printf("\n");

    // Compute (x+1)^4 by squaring twice
    int fourth[10];
    for (i = 0; i < 10; i++) fourth[i] = 0;
    for (i = 0; i <= 2; i++) {
        for (j = 0; j <= 2; j++) {
            fourth[i + j] = fourth[i + j] + sq[i] * sq[j];
        }
    }
    printf("(x+1)^4 = ");
    print_poly(fourth, 4);
    printf("\n");

    // (x-1)(x+1) = x^2 - 1
    int d[2], e[2], de[10];
    d[0] = -1; d[1] = 1;
    e[0] = 1; e[1] = 1;
    for (i = 0; i < 10; i++) de[i] = 0;
    for (i = 0; i <= 1; i++) {
        for (j = 0; j <= 1; j++) {
            de[i + j] = de[i + j] + d[i] * e[j];
        }
    }
    printf("(x-1)(x+1) = ");
    print_poly(de, 2);
    printf("\n");

    // (x-1)(x^2+x+1) = x^3 - 1
    int f[4];
    f[0] = 1; f[1] = 1; f[2] = 1;
    int df[10];
    for (i = 0; i < 10; i++) df[i] = 0;
    for (i = 0; i <= 1; i++) {
        for (j = 0; j <= 2; j++) {
            df[i + j] = df[i + j] + d[i] * f[j];
        }
    }
    printf("(x-1)(x^2+x+1) = ");
    print_poly(df, 3);
    printf("\n");

    // Verify
    printf("\nProduct values at x=-2..2:\n");
    for (i = -2; i <= 2; i++) {
        printf("(x-1)(x^2+x+1) at x=%d: %d\n", i, eval_poly(df, 3, i));
    }

    return 0;
}
