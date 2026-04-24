int printf(const char *fmt, ...);

// EXPECT: === Downsampling / Upsampling ===
// Downsampling and Upsampling (integer-based)
// Downsample by factor M: keep every M-th sample
// Upsample by factor L: insert L-1 zeros between samples

int downsample(int *input, int inlen, int *output, int factor) {
    int outlen = 0;
    int i;
    for (i = 0; i < inlen; i = i + factor) {
        output[outlen] = input[i];
        outlen++;
    }
    return outlen;
}

int upsample(int *input, int inlen, int *output, int factor) {
    int outlen = inlen * factor;
    int i;
    int j;
    for (i = 0; i < outlen; i++) {
        output[i] = 0;
    }
    for (i = 0; i < inlen; i++) {
        output[i * factor] = input[i];
    }
    return outlen;
}

void interpolate_linear(int *upsampled, int len, int factor) {
    int i;
    int seg;
    for (seg = 0; seg < len / factor; seg++) {
        int start_idx = seg * factor;
        int end_idx = start_idx + factor;
        if (end_idx >= len) break;
        int start_val = upsampled[start_idx];
        int end_val = upsampled[end_idx];
        for (i = 1; i < factor; i++) {
            upsampled[start_idx + i] = start_val + (end_val - start_val) * i / factor;
        }
    }
}

int decimate_with_avg(int *input, int inlen, int *output, int factor) {
    int outlen = 0;
    int i;
    int j;
    for (i = 0; i + factor <= inlen; i = i + factor) {
        int sum = 0;
        for (j = 0; j < factor; j++) {
            sum = sum + input[i + j];
        }
        output[outlen] = sum / factor;
        outlen++;
    }
    return outlen;
}

void test_downsample_by2(void) {
    int input[8];
    int output[8];
    int outlen;
    int i;

    input[0] = 10; input[1] = 20; input[2] = 30; input[3] = 40;
    input[4] = 50; input[5] = 60; input[6] = 70; input[7] = 80;

    outlen = downsample(input, 8, output, 2);
    printf("Downsample by 2, len=%d:\n", outlen); // EXPECT: Downsample by 2, len=4:
    for (i = 0; i < outlen; i++) {
        printf("  y[%d] = %d\n", i, output[i]);
    }
    // EXPECT:   y[0] = 10
    // EXPECT:   y[1] = 30
    // EXPECT:   y[2] = 50
    // EXPECT:   y[3] = 70
}

void test_downsample_by3(void) {
    int input[9];
    int output[9];
    int outlen;
    int i;

    input[0] = 5; input[1] = 10; input[2] = 15;
    input[3] = 20; input[4] = 25; input[5] = 30;
    input[6] = 35; input[7] = 40; input[8] = 45;

    outlen = downsample(input, 9, output, 3);
    printf("Downsample by 3, len=%d:\n", outlen); // EXPECT: Downsample by 3, len=3:
    for (i = 0; i < outlen; i++) {
        printf("  y[%d] = %d\n", i, output[i]);
    }
    // EXPECT:   y[0] = 5
    // EXPECT:   y[1] = 20
    // EXPECT:   y[2] = 35
}

void test_upsample_by2(void) {
    int input[4];
    int output[16];
    int outlen;
    int i;

    input[0] = 10; input[1] = 20; input[2] = 30; input[3] = 40;

    outlen = upsample(input, 4, output, 2);
    printf("Upsample by 2, len=%d:\n", outlen); // EXPECT: Upsample by 2, len=8:
    for (i = 0; i < outlen; i++) {
        printf("  y[%d] = %d\n", i, output[i]);
    }
    // EXPECT:   y[0] = 10
    // EXPECT:   y[1] = 0
    // EXPECT:   y[2] = 20
    // EXPECT:   y[3] = 0
    // EXPECT:   y[4] = 30
    // EXPECT:   y[5] = 0
    // EXPECT:   y[6] = 40
    // EXPECT:   y[7] = 0
}

void test_upsample_interpolate(void) {
    int input[4];
    int output[16];
    int outlen;
    int i;

    input[0] = 0; input[1] = 100; input[2] = 200; input[3] = 300;

    outlen = upsample(input, 4, output, 3);
    interpolate_linear(output, outlen, 3);
    printf("Upsample by 3 + interpolate, len=%d:\n", outlen); // EXPECT: Upsample by 3 + interpolate, len=12:
    for (i = 0; i < outlen; i++) {
        printf("  y[%d] = %d\n", i, output[i]);
    }
    // EXPECT:   y[0] = 0
    // EXPECT:   y[1] = 33
    // EXPECT:   y[2] = 66
    // EXPECT:   y[3] = 100
    // EXPECT:   y[4] = 133
    // EXPECT:   y[5] = 166
    // EXPECT:   y[6] = 200
    // EXPECT:   y[7] = 233
    // EXPECT:   y[8] = 266
    // EXPECT:   y[9] = 300
    // EXPECT:   y[10] = 0
    // EXPECT:   y[11] = 0
}

void test_decimate(void) {
    int input[8];
    int output[8];
    int outlen;
    int i;

    input[0] = 10; input[1] = 14; input[2] = 30; input[3] = 26;
    input[4] = 50; input[5] = 54; input[6] = 70; input[7] = 66;

    outlen = decimate_with_avg(input, 8, output, 2);
    printf("Decimate by 2 (avg), len=%d:\n", outlen); // EXPECT: Decimate by 2 (avg), len=4:
    for (i = 0; i < outlen; i++) {
        printf("  y[%d] = %d\n", i, output[i]);
    }
    // EXPECT:   y[0] = 12
    // EXPECT:   y[1] = 28
    // EXPECT:   y[2] = 52
    // EXPECT:   y[3] = 68
}

int main(void) {
    printf("=== Downsampling / Upsampling ===\n");
    test_downsample_by2();
    test_downsample_by3();
    test_upsample_by2();
    test_upsample_interpolate();
    test_decimate();
    printf("All resampling tests passed.\n"); // EXPECT: All resampling tests passed.
    return 0;
}
