int printf(const char *fmt, ...);

// EXPECT: === Discrete Convolution ===
// Convolution of two discrete signals (integer-based)
// (f * g)[n] = sum over k of f[k] * g[n - k]

void convolve(int *x, int xlen, int *h, int hlen, int *out, int *outlen) {
    int n;
    int k;
    *outlen = xlen + hlen - 1;
    for (n = 0; n < *outlen; n++) {
        out[n] = 0;
    }
    for (n = 0; n < *outlen; n++) {
        for (k = 0; k < xlen; k++) {
            int idx = n - k;
            if (idx >= 0 && idx < hlen) {
                out[n] = out[n] + x[k] * h[idx];
            }
        }
    }
}

void test_basic_conv(void) {
    int x[4];
    int h[3];
    int out[8];
    int outlen;
    int i;

    x[0] = 1; x[1] = 2; x[2] = 3; x[3] = 4;
    h[0] = 1; h[1] = 1; h[2] = 1;

    convolve(x, 4, h, 3, out, &outlen);
    printf("Basic conv length: %d\n", outlen); // EXPECT: Basic conv length: 6
    for (i = 0; i < outlen; i++) {
        printf("  y[%d] = %d\n", i, out[i]);
    }
    // EXPECT:   y[0] = 1
    // EXPECT:   y[1] = 3
    // EXPECT:   y[2] = 6
    // EXPECT:   y[3] = 9
    // EXPECT:   y[4] = 7
    // EXPECT:   y[5] = 4
}

void test_impulse_conv(void) {
    int x[5];
    int h[1];
    int out[8];
    int outlen;
    int i;

    x[0] = 10; x[1] = 20; x[2] = 30; x[3] = 40; x[4] = 50;
    h[0] = 1;

    convolve(x, 5, h, 1, out, &outlen);
    printf("Impulse conv length: %d\n", outlen); // EXPECT: Impulse conv length: 5
    for (i = 0; i < outlen; i++) {
        printf("  y[%d] = %d\n", i, out[i]);
    }
    // EXPECT:   y[0] = 10
    // EXPECT:   y[1] = 20
    // EXPECT:   y[2] = 30
    // EXPECT:   y[3] = 40
    // EXPECT:   y[4] = 50
}

void test_step_conv(void) {
    int x[4];
    int h[4];
    int out[8];
    int outlen;
    int i;

    x[0] = 1; x[1] = 1; x[2] = 1; x[3] = 1;
    h[0] = 1; h[1] = 1; h[2] = 1; h[3] = 1;

    convolve(x, 4, h, 4, out, &outlen);
    printf("Step * step length: %d\n", outlen); // EXPECT: Step * step length: 7
    for (i = 0; i < outlen; i++) {
        printf("  y[%d] = %d\n", i, out[i]);
    }
    // EXPECT:   y[0] = 1
    // EXPECT:   y[1] = 2
    // EXPECT:   y[2] = 3
    // EXPECT:   y[3] = 4
    // EXPECT:   y[4] = 3
    // EXPECT:   y[5] = 2
    // EXPECT:   y[6] = 1
}

void test_scaled_conv(void) {
    int x[3];
    int h[3];
    int out[8];
    int outlen;
    int i;

    x[0] = 2; x[1] = 3; x[2] = 4;
    h[0] = 5; h[1] = 6; h[2] = 7;

    convolve(x, 3, h, 3, out, &outlen);
    printf("Scaled conv:\n"); // EXPECT: Scaled conv:
    for (i = 0; i < outlen; i++) {
        printf("  y[%d] = %d\n", i, out[i]);
    }
    // EXPECT:   y[0] = 10
    // EXPECT:   y[1] = 27
    // EXPECT:   y[2] = 52
    // EXPECT:   y[3] = 45
    // EXPECT:   y[4] = 28
}

void test_antisymmetric(void) {
    int x[3];
    int h[3];
    int out[8];
    int outlen;
    int i;

    x[0] = 1; x[1] = -1; x[2] = 1;
    h[0] = 1; h[1] = 2; h[2] = 1;

    convolve(x, 3, h, 3, out, &outlen);
    printf("Antisymmetric conv:\n"); // EXPECT: Antisymmetric conv:
    for (i = 0; i < outlen; i++) {
        printf("  y[%d] = %d\n", i, out[i]);
    }
    // EXPECT:   y[0] = 1
    // EXPECT:   y[1] = 1
    // EXPECT:   y[2] = 0
    // EXPECT:   y[3] = 1
    // EXPECT:   y[4] = 1
}

int main(void) {
    printf("=== Discrete Convolution ===\n");
    test_basic_conv();
    test_impulse_conv();
    test_step_conv();
    test_scaled_conv();
    test_antisymmetric();
    printf("All convolution tests passed.\n"); // EXPECT: All convolution tests passed.
    return 0;
}
