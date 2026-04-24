int printf(const char *fmt, ...);
// EXPECT: p(x) = 3x^3 - 2x^2 + 5x - 7\np(-3) = -121 (naive: -121, match: 1)\np(-2) = -49 (naive: -49, match: 1)\np(-1) = -17 (naive: -17, match: 1)\np(0) = -7 (naive: -7, match: 1)\np(1) = -1 (naive: -1, match: 1)\np(2) = 19 (naive: 19, match: 1)\np(3) = 71 (naive: 71, match: 1)\np(4) = 173 (naive: 173, match: 1)\np(5) = 343 (naive: 343, match: 1)\n\nq(x) = x^4 + 2x^3 - 3x^2 + x - 1\nq(-3) = -4\nq(-2) = -15\nq(-1) = -6\nq(0) = -1\nq(1) = 0\nq(2) = 21\nq(3) = 110\n\nMultiple polynomials at x=2:\npoly[0](2) = 3\npoly[1](2) = 3\npoly[2](2) = 15\npoly[3](2) = 8\npoly[4](2) = 5\n\nChebyshev T_n(3):\nT_0(3) = 1\nT_1(3) = 3\nT_2(3) = 17\nT_3(3) = 99\nT_4(3) = 577\nT_5(3) = 3363\nT_6(3) = 19601\nT_7(3) = 114243\nT_8(3) = 665857\nT_9(3) = 3880899
// Test: Polynomial evaluation

// Evaluate polynomial using Horner's method
// coeffs[0] + coeffs[1]*x + coeffs[2]*x^2 + ...
int horner(int coeffs[], int degree, int x) {
    int result = coeffs[degree];
    int i;
    for (i = degree - 1; i >= 0; i--) {
        result = result * x + coeffs[i];
    }
    return result;
}

int power(int base, int exp) {
    int result = 1;
    int i;
    for (i = 0; i < exp; i++) result = result * base;
    return result;
}

// Naive evaluation for comparison
int naive_eval(int coeffs[], int degree, int x) {
    int result = 0;
    int i;
    for (i = 0; i <= degree; i++) {
        result = result + coeffs[i] * power(x, i);
    }
    return result;
}

int main(void) {
    int i, x;

    // p(x) = 3x^3 - 2x^2 + 5x - 7
    int p1[4];
    p1[0] = -7;
    p1[1] = 5;
    p1[2] = -2;
    p1[3] = 3;

    printf("p(x) = 3x^3 - 2x^2 + 5x - 7\n");
    for (x = -3; x <= 5; x++) {
        int h = horner(p1, 3, x);
        int n = naive_eval(p1, 3, x);
        printf("p(%d) = %d (naive: %d, match: %d)\n", x, h, n, h == n);
    }

    // q(x) = x^4 + 2x^3 - 3x^2 + x - 1
    int q[5];
    q[0] = -1;
    q[1] = 1;
    q[2] = -3;
    q[3] = 2;
    q[4] = 1;

    printf("\nq(x) = x^4 + 2x^3 - 3x^2 + x - 1\n");
    for (x = -3; x <= 3; x++) {
        printf("q(%d) = %d\n", x, horner(q, 4, x));
    }

    // Evaluate multiple polynomials at same point
    printf("\nMultiple polynomials at x=2:\n");
    int polys[5][4];
    int degrees[5];

    // p0 = x + 1
    polys[0][0] = 1; polys[0][1] = 1; degrees[0] = 1;
    // p1 = x^2 - 1
    polys[1][0] = -1; polys[1][1] = 0; polys[1][2] = 1; degrees[1] = 2;
    // p2 = 2x^2 + 3x + 1
    polys[2][0] = 1; polys[2][1] = 3; polys[2][2] = 2; degrees[2] = 2;
    // p3 = x^3
    polys[3][0] = 0; polys[3][1] = 0; polys[3][2] = 0; polys[3][3] = 1; degrees[3] = 3;
    // p4 = 5
    polys[4][0] = 5; degrees[4] = 0;

    for (i = 0; i < 5; i++) {
        printf("poly[%d](2) = %d\n", i, horner(polys[i], degrees[i], 2));
    }

    // Chebyshev polynomials T_n at x=3 (integer version)
    // T_0 = 1, T_1 = x, T_n = 2x*T_{n-1} - T_{n-2}
    printf("\nChebyshev T_n(3):\n");
    int cheb[10];
    cheb[0] = 1;
    cheb[1] = 3;
    for (i = 2; i < 10; i++) {
        cheb[i] = 2 * 3 * cheb[i - 1] - cheb[i - 2];
    }
    for (i = 0; i < 10; i++) {
        printf("T_%d(3) = %d\n", i, cheb[i]);
    }

    return 0;
}
