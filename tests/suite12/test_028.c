int printf(const char *fmt, ...);

// Low-Pass Filter (Integer IIR)
// Implements a first-order IIR low-pass filter
// y[n] = (alpha * x[n] + (1000 - alpha) * y[n-1]) / 1000
// alpha is 0..1000 (representing 0.0 to 1.0)

struct LPFilter {
    int alpha;       // filter coefficient 0..1000 (higher = less filtering)
    int prev_output; // previous output sample
};

void lpf_init(struct LPFilter *f, int alpha) {
    f->alpha = alpha;
    f->prev_output = 0;
}

int lpf_process(struct LPFilter *f, int input) {
    int output = (f->alpha * input + (1000 - f->alpha) * f->prev_output) / 1000;
    f->prev_output = output;
    return output;
}

void lpf_process_block(struct LPFilter *f, int *input, int *output, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        output[i] = lpf_process(f, input[i]);
    }
}

// Moving average filter (FIR)
int moving_avg(int *input, int pos, int window) {
    int sum = 0;
    int count = 0;
    int i;
    for (i = 0; i < window; i = i + 1) {
        int idx = pos - i;
        if (idx >= 0) {
            sum = sum + input[idx];
            count = count + 1;
        }
    }
    if (count == 0) return 0;
    return sum / count;
}

void moving_avg_block(int *input, int *output, int len, int window) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        output[i] = moving_avg(input, i, window);
    }
}

int compute_smoothness(int *signal, int len) {
    // Sum of absolute differences between consecutive samples
    int sum = 0;
    int i;
    for (i = 1; i < len; i = i + 1) {
        int diff = signal[i] - signal[i - 1];
        if (diff < 0) diff = -diff;
        sum = sum + diff;
    }
    return sum;
}

int max_val(int *buf, int len) {
    int m = buf[0];
    int i;
    for (i = 1; i < len; i = i + 1) {
        if (buf[i] > m) m = buf[i];
    }
    return m;
}

int min_val(int *buf, int len) {
    int m = buf[0];
    int i;
    for (i = 1; i < len; i = i + 1) {
        if (buf[i] < m) m = buf[i];
    }
    return m;
}

int main() {
    // Test 1: Filter a square wave
    int square[16];
    int i;
    for (i = 0; i < 16; i = i + 1) {
        if (i < 8) square[i] = 1000;
        else square[i] = -1000;
    }

    // Heavy filtering (alpha=200, very smooth)
    struct LPFilter lpf1;
    lpf_init(&lpf1, 200);
    int filtered1[16];
    lpf_process_block(&lpf1, square, filtered1, 16);

    printf("Heavy LPF: ");
    for (i = 0; i < 16; i = i + 1) {
        printf("%d", filtered1[i]);
        if (i < 15) printf(" ");
    }
    printf("\n");
    // EXPECT: Heavy LPF: 200 360 488 590 672 737 789 831 464 171 -63 -250 -400 -520 -616 -692

    // Light filtering (alpha=800)
    struct LPFilter lpf2;
    lpf_init(&lpf2, 800);
    int filtered2[16];
    lpf_process_block(&lpf2, square, filtered2, 16);

    printf("Light LPF: ");
    for (i = 0; i < 16; i = i + 1) {
        printf("%d", filtered2[i]);
        if (i < 15) printf(" ");
    }
    printf("\n");
    // EXPECT: Light LPF: 800 960 992 998 999 999 999 999 -600 -920 -984 -996 -999 -999 -999 -999

    // Smoothness comparison
    int rough = compute_smoothness(square, 16);
    printf("Square roughness: %d\n", rough);
    // EXPECT: Square roughness: 2000

    int smooth1 = compute_smoothness(filtered1, 16);
    printf("Heavy LPF roughness: %d\n", smooth1);
    // EXPECT: Heavy LPF roughness: 2154

    int smooth2 = compute_smoothness(filtered2, 16);
    printf("Light LPF roughness: %d\n", smooth2);
    // EXPECT: Light LPF roughness: 2197

    // Test 2: Filter an impulse
    int impulse[12];
    impulse[0] = 1000;
    for (i = 1; i < 12; i = i + 1) impulse[i] = 0;

    struct LPFilter lpf3;
    lpf_init(&lpf3, 300);
    int imp_out[12];
    lpf_process_block(&lpf3, impulse, imp_out, 12);

    printf("Impulse response: ");
    for (i = 0; i < 12; i = i + 1) {
        printf("%d", imp_out[i]);
        if (i < 11) printf(" ");
    }
    printf("\n");
    // EXPECT: Impulse response: 300 210 147 102 71 49 34 23 16 11 7 4

    // Test 3: Moving average filter
    int noisy[12];
    noisy[0] = 100; noisy[1] = 50; noisy[2] = 120; noisy[3] = 30;
    noisy[4] = 110; noisy[5] = 40; noisy[6] = 105; noisy[7] = 45;
    noisy[8] = 115; noisy[9] = 35; noisy[10] = 100; noisy[11] = 55;
    int avg_out[12];
    moving_avg_block(noisy, avg_out, 12, 4);

    printf("Moving avg: ");
    for (i = 0; i < 12; i = i + 1) {
        printf("%d", avg_out[i]);
        if (i < 11) printf(" ");
    }
    printf("\n");
    // EXPECT: Moving avg: 100 75 90 75 77 75 71 75 76 75 73 76

    // Range of filtered vs original
    int orig_range = max_val(noisy, 12) - min_val(noisy, 12);
    int filt_range = max_val(avg_out, 12) - min_val(avg_out, 12);
    printf("Original range: %d\n", orig_range);
    // EXPECT: Original range: 90
    printf("Filtered range: %d\n", filt_range);
    // EXPECT: Filtered range: 29

    return 0;
}
