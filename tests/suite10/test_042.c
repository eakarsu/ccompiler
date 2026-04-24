int printf(const char *fmt, ...);

// EXPECT: === FIR Filter (Integer Coefficients) ===
// FIR Filter with integer coefficients
// y[n] = sum(h[k] * x[n-k]) for k = 0..N-1
// All values scaled by 100 to avoid floating point

struct FIRFilter {
    int coeffs[8];
    int num_taps;
    int delay_line[8];
    int delay_idx;
};

void fir_init(struct FIRFilter *f, int *coeffs, int num_taps) {
    int i;
    f->num_taps = num_taps;
    f->delay_idx = 0;
    for (i = 0; i < num_taps; i++) {
        f->coeffs[i] = coeffs[i];
        f->delay_line[i] = 0;
    }
    for (i = num_taps; i < 8; i++) {
        f->delay_line[i] = 0;
    }
}

int fir_process(struct FIRFilter *f, int input) {
    int sum;
    int i;
    int idx;

    f->delay_line[f->delay_idx] = input;
    sum = 0;

    for (i = 0; i < f->num_taps; i++) {
        idx = f->delay_idx - i;
        if (idx < 0) {
            idx = idx + f->num_taps;
        }
        sum = sum + f->coeffs[i] * f->delay_line[idx];
    }
    f->delay_idx = (f->delay_idx + 1) % f->num_taps;

    return sum / 100;
}

void test_lowpass(void) {
    struct FIRFilter f;
    int coeffs[4];
    int signal[8];
    int i;
    int out;

    coeffs[0] = 25;
    coeffs[1] = 25;
    coeffs[2] = 25;
    coeffs[3] = 25;

    signal[0] = 100;
    signal[1] = 0;
    signal[2] = 100;
    signal[3] = 0;
    signal[4] = 100;
    signal[5] = 0;
    signal[6] = 100;
    signal[7] = 0;

    fir_init(&f, coeffs, 4);
    printf("Low-pass FIR (uniform weights):\n"); // EXPECT: Low-pass FIR (uniform weights):
    for (i = 0; i < 8; i++) {
        out = fir_process(&f, signal[i]);
        printf("  x[%d]=%d -> y=%d\n", i, signal[i], out);
    }
    // EXPECT:   x[0]=100 -> y=25
    // EXPECT:   x[1]=0 -> y=25
    // EXPECT:   x[2]=100 -> y=50
    // EXPECT:   x[3]=0 -> y=50
    // EXPECT:   x[4]=100 -> y=50
    // EXPECT:   x[5]=0 -> y=50
    // EXPECT:   x[6]=100 -> y=50
    // EXPECT:   x[7]=0 -> y=50
}

void test_highpass(void) {
    struct FIRFilter f;
    int coeffs[3];
    int signal[8];
    int i;
    int out;

    coeffs[0] = 50;
    coeffs[1] = -100;
    coeffs[2] = 50;

    signal[0] = 10;
    signal[1] = 20;
    signal[2] = 30;
    signal[3] = 40;
    signal[4] = 50;
    signal[5] = 60;
    signal[6] = 70;
    signal[7] = 80;

    fir_init(&f, coeffs, 3);
    printf("High-pass FIR (difference):\n"); // EXPECT: High-pass FIR (difference):
    for (i = 0; i < 8; i++) {
        out = fir_process(&f, signal[i]);
        printf("  x[%d]=%d -> y=%d\n", i, signal[i], out);
    }
    // EXPECT:   x[0]=10 -> y=5
    // EXPECT:   x[1]=20 -> y=0
    // EXPECT:   x[2]=30 -> y=0
    // EXPECT:   x[3]=40 -> y=0
    // EXPECT:   x[4]=50 -> y=0
    // EXPECT:   x[5]=60 -> y=0
    // EXPECT:   x[6]=70 -> y=0
    // EXPECT:   x[7]=80 -> y=0
}

void test_impulse_response(void) {
    struct FIRFilter f;
    int coeffs[5];
    int i;
    int out;

    coeffs[0] = 10;
    coeffs[1] = 30;
    coeffs[2] = 40;
    coeffs[3] = 30;
    coeffs[4] = 10;

    fir_init(&f, coeffs, 5);
    printf("Impulse response (5-tap):\n"); // EXPECT: Impulse response (5-tap):

    for (i = 0; i < 8; i++) {
        int input;
        if (i == 0) {
            input = 100;
        } else {
            input = 0;
        }
        out = fir_process(&f, input);
        printf("  h[%d] = %d\n", i, out);
    }
    // EXPECT:   h[0] = 10
    // EXPECT:   h[1] = 30
    // EXPECT:   h[2] = 40
    // EXPECT:   h[3] = 30
    // EXPECT:   h[4] = 10
    // EXPECT:   h[5] = 0
    // EXPECT:   h[6] = 0
    // EXPECT:   h[7] = 0
}

int main(void) {
    printf("=== FIR Filter (Integer Coefficients) ===\n");
    test_lowpass();
    test_highpass();
    test_impulse_response();
    printf("All FIR filter tests passed.\n"); // EXPECT: All FIR filter tests passed.
    return 0;
}
