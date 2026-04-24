int printf(const char *fmt, ...);
// EXPECT: a(x) = 3x^3 + 2x^2 - x + 5\nb(x) = x^4 - 2x^2 + 3x - 1\na+b  = x^4 + 3x^3 + 2x + 4\na-b  = -x^4 + 3x^3 + 4x^2 - 4x + 6\n\nVerification at x = -2, -1, 0, 1, 2:\nx=-2: a=-9, b=1, a+b=-8, sum=-8, match=1\nx=-1: a=5, b=-5, a+b=0, sum=0, match=1\nx=0: a=5, b=-1, a+b=4, sum=4, match=1\nx=1: a=9, b=1, a+b=10, sum=10, match=1\nx=2: a=35, b=13, a+b=48, sum=48, match=1\n\n3*a(x) = 9x^3 + 6x^2 - 3x + 15\n\nSum of three polys = x^3 + x^2 + x + 4
// Test: Polynomial addition

void print_poly(int coeffs[], int degree) {
    int first = 1;
    int i;
    for (i = degree; i >= 0; i--) {
        if (coeffs[i] == 0) continue;
        if (!first && coeffs[i] > 0) printf(" + ");
        if (!first && coeffs[i] < 0) printf(" - ");
        if (first && coeffs[i] < 0) printf("-");
        first = 0;

        int c = coeffs[i];
        if (c < 0) c = -c;

        if (i == 0) {
            printf("%d", c);
        } else if (i == 1) {
            if (c == 1) printf("x");
            else printf("%dx", c);
        } else {
            if (c == 1) printf("x^%d", i);
            else printf("%dx^%d", c, i);
        }
    }
    if (first) printf("0");
}

int main(void) {
    int i;

    // Add two polynomials
    int a[6], b[6], sum[6];
    int deg_a = 3, deg_b = 4;

    // a(x) = 3x^3 + 2x^2 - x + 5
    a[0] = 5; a[1] = -1; a[2] = 2; a[3] = 3; a[4] = 0; a[5] = 0;

    // b(x) = x^4 - 2x^2 + 3x - 1
    b[0] = -1; b[1] = 3; b[2] = -2; b[3] = 0; b[4] = 1; b[5] = 0;

    printf("a(x) = ");
    print_poly(a, deg_a);
    printf("\n");

    printf("b(x) = ");
    print_poly(b, deg_b);
    printf("\n");

    // Sum
    int deg_sum = deg_a;
    if (deg_b > deg_sum) deg_sum = deg_b;
    for (i = 0; i <= deg_sum; i++) {
        sum[i] = a[i] + b[i];
    }

    printf("a+b  = ");
    print_poly(sum, deg_sum);
    printf("\n");

    // Difference
    int diff[6];
    for (i = 0; i <= deg_sum; i++) {
        diff[i] = a[i] - b[i];
    }
    printf("a-b  = ");
    print_poly(diff, deg_sum);
    printf("\n");

    // Verify by evaluating at several points
    printf("\nVerification at x = -2, -1, 0, 1, 2:\n");
    int xs[5];
    xs[0] = -2; xs[1] = -1; xs[2] = 0; xs[3] = 1; xs[4] = 2;

    for (i = 0; i < 5; i++) {
        int x = xs[i];
        int va = a[0], vb = b[0], vs = sum[0];
        int px = 1;
        int j;
        for (j = 1; j <= deg_sum; j++) {
            px = px * x;
            va = va + a[j] * px;
            vb = vb + b[j] * px;
            vs = vs + sum[j] * px;
        }
        printf("x=%d: a=%d, b=%d, a+b=%d, sum=%d, match=%d\n",
               x, va, vb, va + vb, vs, va + vb == vs);
    }

    // Scalar multiplication
    int scaled[6];
    int scalar = 3;
    for (i = 0; i <= deg_a; i++) {
        scaled[i] = a[i] * scalar;
    }
    printf("\n3*a(x) = ");
    print_poly(scaled, deg_a);
    printf("\n");

    // Sum of multiple polynomials
    int polys[3][5];
    // p0 = x^2 + 1
    polys[0][0] = 1; polys[0][1] = 0; polys[0][2] = 1; polys[0][3] = 0; polys[0][4] = 0;
    // p1 = 2x + 3
    polys[1][0] = 3; polys[1][1] = 2; polys[1][2] = 0; polys[1][3] = 0; polys[1][4] = 0;
    // p2 = x^3 - x
    polys[2][0] = 0; polys[2][1] = -1; polys[2][2] = 0; polys[2][3] = 1; polys[2][4] = 0;

    int total[5];
    for (i = 0; i < 5; i++) total[i] = 0;
    int j;
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 5; i++) {
            total[i] = total[i] + polys[j][i];
        }
    }
    printf("\nSum of three polys = ");
    print_poly(total, 3);
    printf("\n");

    return 0;
}
