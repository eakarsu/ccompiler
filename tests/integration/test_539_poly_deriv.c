int printf(const char *fmt, ...);
// EXPECT: p(x) = 3x^4 - 2x^3 + x^2 - 5x + 7\np'(x) = 12x^3 - 6x^2 + 2x - 5\np''(x) = 36x^2 - 12x + 2\np'''(x) = 72x - 12\np''''(x) = 72\n\nEvaluation:\nx    p(x)    p'(x)   p''(x)\n-3   328      -389     362\n-2   85       -129     170\n-1   18       -25      50\n0    7        -5       2\n1    4        3        26\n2    33       71       122\n3    190      271      290\n\nAntiderivative of p'(x) should recover p(x) (up to constant):\nIntegral of p'(x) = 3x^4 - 2x^3 + x^2 - 5x\nOriginal p(x)     = 3x^4 - 2x^3 + x^2 - 5x + 7\n\nProduct rule: d/dx[(x^2+1)(x-1)] = 3x^2 - 2x + 1
// Test: Polynomial derivative

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

void derivative(int c[], int deg, int dc[], int *ddeg) {
    int i;
    if (deg == 0) {
        dc[0] = 0;
        *ddeg = 0;
        return;
    }
    for (i = 0; i < deg; i++) {
        dc[i] = c[i + 1] * (i + 1);
    }
    *ddeg = deg - 1;
}

int main(void) {
    int i;

    // p(x) = 3x^4 - 2x^3 + x^2 - 5x + 7
    int p[5];
    p[0] = 7; p[1] = -5; p[2] = 1; p[3] = -2; p[4] = 3;
    int deg = 4;

    printf("p(x) = ");
    print_poly(p, deg);
    printf("\n");

    // First derivative
    int dp[5];
    int ddeg;
    derivative(p, deg, dp, &ddeg);
    printf("p'(x) = ");
    print_poly(dp, ddeg);
    printf("\n");

    // Second derivative
    int d2p[5];
    int d2deg;
    derivative(dp, ddeg, d2p, &d2deg);
    printf("p''(x) = ");
    print_poly(d2p, d2deg);
    printf("\n");

    // Third derivative
    int d3p[5];
    int d3deg;
    derivative(d2p, d2deg, d3p, &d3deg);
    printf("p'''(x) = ");
    print_poly(d3p, d3deg);
    printf("\n");

    // Fourth derivative (should be constant)
    int d4p[5];
    int d4deg;
    derivative(d3p, d3deg, d4p, &d4deg);
    printf("p''''(x) = ");
    print_poly(d4p, d4deg);
    printf("\n");

    // Evaluate p and p' at several points
    printf("\nEvaluation:\n");
    printf("x    p(x)    p'(x)   p''(x)\n");
    for (i = -3; i <= 3; i++) {
        printf("%-4d %-8d %-8d %d\n", i,
               eval_poly(p, deg, i),
               eval_poly(dp, ddeg, i),
               eval_poly(d2p, d2deg, i));
    }

    // Antiderivative (integration with C=0)
    printf("\nAntiderivative of p'(x) should recover p(x) (up to constant):\n");
    int anti[6];
    anti[0] = 0; // constant of integration = 0
    for (i = 0; i <= ddeg; i++) {
        anti[i + 1] = dp[i] / (i + 1);
    }
    printf("Integral of p'(x) = ");
    print_poly(anti, ddeg + 1);
    printf("\n");
    printf("Original p(x)     = ");
    print_poly(p, deg);
    printf("\n");

    // Product rule check: d/dx [f*g] = f'*g + f*g'
    // f = x^2 + 1, g = x - 1
    int f[3], g[2], fg[4];
    f[0] = 1; f[1] = 0; f[2] = 1;
    g[0] = -1; g[1] = 1;
    // fg = product
    for (i = 0; i < 4; i++) fg[i] = 0;
    int j;
    for (i = 0; i <= 2; i++)
        for (j = 0; j <= 1; j++)
            fg[i + j] = fg[i + j] + f[i] * g[j];

    int dfg[4];
    int dfg_deg;
    derivative(fg, 3, dfg, &dfg_deg);
    printf("\nProduct rule: d/dx[(x^2+1)(x-1)] = ");
    print_poly(dfg, dfg_deg);
    printf("\n");

    return 0;
}
