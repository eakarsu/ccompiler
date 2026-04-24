int printf(const char *fmt, ...);

// EXPECT: === Cross-Correlation ===
// Cross-correlation of two discrete signals (integer-based)
// R_xy[lag] = sum over n of x[n] * y[n + lag]

void cross_correlate(int *x, int *y, int len, int *result, int max_lag) {
    int lag;
    int n;
    for (lag = -max_lag; lag <= max_lag; lag++) {
        int sum = 0;
        for (n = 0; n < len; n++) {
            int yn = n + lag;
            if (yn >= 0 && yn < len) {
                sum = sum + x[n] * y[yn];
            }
        }
        result[lag + max_lag] = sum;
    }
}

void autocorrelate(int *x, int len, int *result, int max_lag) {
    cross_correlate(x, x, len, result, max_lag);
}

int find_max_lag(int *result, int total_lags, int offset) {
    int max_val = result[0];
    int max_idx = 0;
    int i;
    for (i = 1; i < total_lags; i++) {
        if (result[i] > max_val) {
            max_val = result[i];
            max_idx = i;
        }
    }
    return max_idx - offset;
}

void test_identical_signals(void) {
    int x[6];
    int result[11];
    int i;

    x[0] = 1; x[1] = 2; x[2] = 3; x[3] = 2; x[4] = 1; x[5] = 0;

    autocorrelate(x, 6, result, 5);
    printf("Autocorrelation of [1,2,3,2,1,0]:\n"); // EXPECT: Autocorrelation of [1,2,3,2,1,0]:
    for (i = 0; i < 11; i++) {
        int lag = i - 5;
        printf("  lag %d: %d\n", lag, result[i]);
    }
    // EXPECT:   lag -5: 0
    // EXPECT:   lag -4: 1
    // EXPECT:   lag -3: 4
    // EXPECT:   lag -2: 10
    // EXPECT:   lag -1: 16
    // EXPECT:   lag 0: 19
    // EXPECT:   lag 1: 16
    // EXPECT:   lag 2: 10
    // EXPECT:   lag 3: 4
    // EXPECT:   lag 4: 1
    // EXPECT:   lag 5: 0

    int peak_lag = find_max_lag(result, 11, 5);
    printf("Peak at lag: %d\n", peak_lag); // EXPECT: Peak at lag: 0
}

void test_delayed_signal(void) {
    int x[8];
    int y[8];
    int result[11];
    int i;

    x[0] = 0; x[1] = 0; x[2] = 5; x[3] = 10; x[4] = 5; x[5] = 0; x[6] = 0; x[7] = 0;
    y[0] = 0; y[1] = 0; y[2] = 0; y[3] = 0; y[4] = 5; y[5] = 10; y[6] = 5; y[7] = 0;

    cross_correlate(x, y, 8, result, 5);
    printf("Cross-correlation (delay=2):\n"); // EXPECT: Cross-correlation (delay=2):
    for (i = 0; i < 11; i++) {
        int lag = i - 5;
        printf("  lag %d: %d\n", lag, result[i]);
    }
    // EXPECT:   lag -5: 0
    // EXPECT:   lag -4: 0
    // EXPECT:   lag -3: 0
    // EXPECT:   lag -2: 0
    // EXPECT:   lag -1: 0
    // EXPECT:   lag 0: 25
    // EXPECT:   lag 1: 100
    // EXPECT:   lag 2: 150
    // EXPECT:   lag 3: 100
    // EXPECT:   lag 4: 25
    // EXPECT:   lag 5: 0

    int peak_lag = find_max_lag(result, 11, 5);
    printf("Peak at lag: %d\n", peak_lag); // EXPECT: Peak at lag: 2
}

void test_orthogonal(void) {
    int x[4];
    int y[4];
    int result[5];

    x[0] = 1; x[1] = -1; x[2] = 1; x[3] = -1;
    y[0] = 1; y[1] = 1; y[2] = -1; y[3] = -1;

    cross_correlate(x, y, 4, result, 2);
    printf("Near-orthogonal at lag 0: %d\n", result[2]); // EXPECT: Near-orthogonal at lag 0: 0
}

int main(void) {
    printf("=== Cross-Correlation ===\n");
    test_identical_signals();
    test_delayed_signal();
    test_orthogonal();
    printf("All cross-correlation tests passed.\n"); // EXPECT: All cross-correlation tests passed.
    return 0;
}
