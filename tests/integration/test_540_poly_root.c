int printf(const char *fmt, ...);
// EXPECT: p(x) = x^3 - 6x^2 + 11x - 6\nSearching for integer roots in [-5, 10]:\nRoot found: x = 1\nRoot found: x = 2\nRoot found: x = 3\n\nSign changes:\np(-2) = -60\np(-1) = -24\np(0) = -6\np(1) = 0\np(2) = 0\np(3) = 0\np(4) = 6\np(5) = 24\n\nBisection for x^2 = 2 (finding sqrt(2)):\nsqrt(2) ~ 1.414\n\nBisection for sqrt(3):\nsqrt(3) ~ 1.732\n\nBisection for x^3 - x - 1 = 0:\nRoot ~ 1.000\n\np(x) = 2x^3 + 3x^2 - 8x + 3\nTesting possible rational roots (x2 to avoid fractions):\nRoot: x = -3\nRoot: x = 1/2\nRoot: x = 1
// Test: Polynomial root finding by bisection (integer-scaled)

// Evaluate polynomial at x (x is scaled by 1000)
// p(x) = a0 + a1*x + a2*x^2 + ...
// Returns p(x/1000) * 1000^deg to keep integer
int eval_scaled(int c[], int deg, int x_1000) {
    // Use Horner's method with scaling
    // We compute c[deg]*x^deg + ... but x = x_1000/1000
    // To avoid overflow, we evaluate step by step
    int result = c[deg];
    int i;
    for (i = deg - 1; i >= 0; i--) {
        result = result * x_1000 / 1000 + c[i];
    }
    return result;
}

// Simple integer polynomial evaluation
int eval_poly(int c[], int deg, int x) {
    int result = c[deg];
    int i;
    for (i = deg - 1; i >= 0; i--) {
        result = result * x + c[i];
    }
    return result;
}

int main(void) {
    int i;

    // Find integer roots by exhaustive search for small polynomials
    // p(x) = x^3 - 6x^2 + 11x - 6 = (x-1)(x-2)(x-3)
    int p1[4];
    p1[0] = -6; p1[1] = 11; p1[2] = -6; p1[3] = 1;

    printf("p(x) = x^3 - 6x^2 + 11x - 6\n");
    printf("Searching for integer roots in [-5, 10]:\n");
    for (i = -5; i <= 10; i++) {
        int val = eval_poly(p1, 3, i);
        if (val == 0) {
            printf("Root found: x = %d\n", i);
        }
    }

    // Evaluate to show sign changes
    printf("\nSign changes:\n");
    for (i = -2; i <= 5; i++) {
        int val = eval_poly(p1, 3, i);
        printf("p(%d) = %d\n", i, val);
    }

    // Bisection for p(x) = x^2 - 2 (root near sqrt(2) ~ 1.414)
    // Scale by 1000: find x such that x^2/1000 - 2 ~ 0
    printf("\nBisection for x^2 = 2 (finding sqrt(2)):\n");
    int lo = 1000; // 1.000
    int hi = 2000; // 2.000
    for (i = 0; i < 20; i++) {
        int mid = (lo + hi) / 2;
        // mid^2 / 1000 compared to 2000
        int val = mid / 1 * mid / 1000 - 2000;
        if (val > 0) {
            hi = mid;
        } else {
            lo = mid;
        }
    }
    printf("sqrt(2) ~ %d.%03d\n", lo / 1000, lo % 1000);

    // Bisection for x^2 - 3 (finding sqrt(3) ~ 1.732)
    printf("\nBisection for sqrt(3):\n");
    lo = 1000; hi = 2000;
    for (i = 0; i < 20; i++) {
        int mid = (lo + hi) / 2;
        int val = mid / 1 * mid / 1000 - 3000;
        if (val > 0) hi = mid;
        else lo = mid;
    }
    printf("sqrt(3) ~ %d.%03d\n", lo / 1000, lo % 1000);

    // Find roots of x^3 - x - 1 = 0 (real root near 1.3247)
    printf("\nBisection for x^3 - x - 1 = 0:\n");
    lo = 1000; hi = 2000;
    for (i = 0; i < 20; i++) {
        int mid = (lo + hi) / 2;
        // mid^3/1000000 - mid - 1000
        int val = (mid / 10) * (mid / 10) * (mid / 10) / 100 - mid - 1000;
        if (val > 0) hi = mid;
        else lo = mid;
    }
    printf("Root ~ %d.%03d\n", lo / 1000, lo % 1000);

    // Rational root theorem check: p(x) = 2x^3 + 3x^2 - 8x + 3
    // Possible rational roots: +-{1,3,1/2,3/2}
    int p2[4];
    p2[0] = 3; p2[1] = -8; p2[2] = 3; p2[3] = 2;
    printf("\np(x) = 2x^3 + 3x^2 - 8x + 3\n");
    printf("Testing possible rational roots (x2 to avoid fractions):\n");
    // Test 2*x at integers: p(n/2) where n = -6..6
    for (i = -6; i <= 6; i++) {
        // p(i/2) = 2*(i/2)^3 + 3*(i/2)^2 - 8*(i/2) + 3
        // = i^3/4 + 3i^2/4 - 4i + 3
        // multiply by 4: i^3 + 3i^2 - 16i + 12
        int val = i * i * i + 3 * i * i - 16 * i + 12;
        if (val == 0) {
            if (i % 2 == 0)
                printf("Root: x = %d\n", i / 2);
            else
                printf("Root: x = %d/2\n", i);
        }
    }

    return 0;
}
