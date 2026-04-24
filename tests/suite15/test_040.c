int printf(const char *fmt, ...);

// Upsampling and downsampling (sample rate conversion)

// Upsample by factor L: insert L-1 zeros between each sample
void upsample(int *x, int xlen, int *y, int factor) {
    int i;
    int j;
    for (i = 0; i < xlen * factor; i++) {
        y[i] = 0;
    }
    for (i = 0; i < xlen; i++) {
        y[i * factor] = x[i];
    }
}

// Downsample by factor M: keep every M-th sample
void downsample(int *x, int xlen, int *y, int factor) {
    int i;
    int out = 0;
    for (i = 0; i < xlen; i = i + factor) {
        y[out] = x[i];
        out = out + 1;
    }
}

// Upsample with linear interpolation
void upsample_interp(int *x, int xlen, int *y, int factor) {
    int i;
    int j;
    for (i = 0; i < xlen - 1; i++) {
        for (j = 0; j < factor; j++) {
            // Linear interpolation between x[i] and x[i+1]
            y[i * factor + j] = x[i] + (x[i + 1] - x[i]) * j / factor;
        }
    }
    // Last sample
    y[(xlen - 1) * factor] = x[xlen - 1];
}

// Downsample with averaging (anti-aliasing)
void downsample_avg(int *x, int xlen, int *y, int factor) {
    int i;
    int j;
    int out = 0;
    for (i = 0; i <= xlen - factor; i = i + factor) {
        int sum = 0;
        for (j = 0; j < factor; j++) {
            sum = sum + x[i + j];
        }
        y[out] = sum / factor;
        out = out + 1;
    }
}

// Polyphase decomposition: split signal into M phases
void polyphase_decompose(int *x, int xlen, int *p0, int *p1, int factor) {
    int i;
    int c0 = 0;
    int c1 = 0;
    for (i = 0; i < xlen; i++) {
        if (i % factor == 0) {
            p0[c0] = x[i];
            c0 = c0 + 1;
        } else {
            p1[c1] = x[i];
            c1 = c1 + 1;
        }
    }
}

// Verify perfect reconstruction: upsample then downsample with same factor
int verify_reconstruction(int *x, int xlen, int factor) {
    int up[64];
    int down[16];
    upsample(x, xlen, up, factor);
    downsample(up, xlen * factor, down, factor);
    int i;
    for (i = 0; i < xlen; i++) {
        if (down[i] != x[i]) return 0;
    }
    return 1;
}

// Compute output length for downsample
int downsample_len(int xlen, int factor) {
    return (xlen + factor - 1) / factor;
}

// Compute output length for upsample
int upsample_len(int xlen, int factor) {
    return xlen * factor;
}

int main() {
    // Test 1: Upsample by 2
    int x1[4];
    x1[0] = 1; x1[1] = 2; x1[2] = 3; x1[3] = 4;
    int y1[8];
    upsample(x1, 4, y1, 2);
    printf("Up2: %d %d %d %d %d %d %d %d\n",
           y1[0], y1[1], y1[2], y1[3], y1[4], y1[5], y1[6], y1[7]);
    // EXPECT: Up2: 1 0 2 0 3 0 4 0

    // Test 2: Upsample by 3
    int x2[3];
    x2[0] = 10; x2[1] = 20; x2[2] = 30;
    int y2[9];
    upsample(x2, 3, y2, 3);
    printf("Up3: %d %d %d %d %d %d %d %d %d\n",
           y2[0], y2[1], y2[2], y2[3], y2[4], y2[5], y2[6], y2[7], y2[8]);
    // EXPECT: Up3: 10 0 0 20 0 0 30 0 0

    // Test 3: Downsample by 2
    int x3[8];
    x3[0] = 1; x3[1] = 2; x3[2] = 3; x3[3] = 4;
    x3[4] = 5; x3[5] = 6; x3[6] = 7; x3[7] = 8;
    int y3[4];
    downsample(x3, 8, y3, 2);
    printf("Down2: %d %d %d %d\n", y3[0], y3[1], y3[2], y3[3]);
    // EXPECT: Down2: 1 3 5 7

    // Test 4: Downsample by 3
    int y4[3];
    downsample(x3, 8, y4, 3);
    // indices 0, 3, 6 -> 1, 4, 7
    printf("Down3: %d %d %d\n", y4[0], y4[1], y4[2]);
    // EXPECT: Down3: 1 4 7

    // Test 5: Upsample with interpolation by 2
    int x5[4];
    x5[0] = 0; x5[1] = 100; x5[2] = 200; x5[3] = 0;
    int y5[7]; // (4-1)*2 + 1 = 7
    upsample_interp(x5, 4, y5, 2);
    // y[0] = 0
    // y[1] = 0 + (100-0)*1/2 = 50
    // y[2] = 100
    // y[3] = 100 + (200-100)*1/2 = 150
    // y[4] = 200
    // y[5] = 200 + (0-200)*1/2 = 100
    // y[6] = 0
    printf("Interp2: %d %d %d %d %d %d %d\n",
           y5[0], y5[1], y5[2], y5[3], y5[4], y5[5], y5[6]);
    // EXPECT: Interp2: 0 50 100 150 200 100 0

    // Test 6: Upsample with interpolation by 4
    int x6[3];
    x6[0] = 0; x6[1] = 100; x6[2] = 0;
    int y6[9]; // (3-1)*4 + 1 = 9
    upsample_interp(x6, 3, y6, 4);
    // y[0] = 0
    // y[1] = 0 + 100*1/4 = 25
    // y[2] = 0 + 100*2/4 = 50
    // y[3] = 0 + 100*3/4 = 75
    // y[4] = 100
    // y[5] = 100 + (0-100)*1/4 = 75
    // y[6] = 100 + (0-100)*2/4 = 50
    // y[7] = 100 + (0-100)*3/4 = 25
    // y[8] = 0
    printf("Interp4: %d %d %d %d %d %d %d %d %d\n",
           y6[0], y6[1], y6[2], y6[3], y6[4], y6[5], y6[6], y6[7], y6[8]);
    // EXPECT: Interp4: 0 25 50 75 100 75 50 25 0

    // Test 7: Downsample with averaging
    int x7[8];
    x7[0] = 10; x7[1] = 20; x7[2] = 30; x7[3] = 40;
    x7[4] = 50; x7[5] = 60; x7[6] = 70; x7[7] = 80;
    int y7[4];
    downsample_avg(x7, 8, y7, 2);
    // y[0] = (10+20)/2 = 15
    // y[1] = (30+40)/2 = 35
    // y[2] = (50+60)/2 = 55
    // y[3] = (70+80)/2 = 75
    printf("DownAvg2: %d %d %d %d\n", y7[0], y7[1], y7[2], y7[3]);
    // EXPECT: DownAvg2: 15 35 55 75

    // Test 8: Downsample average by 4
    int y8[2];
    downsample_avg(x7, 8, y8, 4);
    // y[0] = (10+20+30+40)/4 = 25
    // y[1] = (50+60+70+80)/4 = 65
    printf("DownAvg4: %d %d\n", y8[0], y8[1]);
    // EXPECT: DownAvg4: 25 65

    // Test 9: Perfect reconstruction test
    int pr = verify_reconstruction(x1, 4, 2);
    printf("Perfect recon (L=2): %d\n", pr);
    // EXPECT: Perfect recon (L=2): 1
    int pr3 = verify_reconstruction(x2, 3, 3);
    printf("Perfect recon (L=3): %d\n", pr3);
    // EXPECT: Perfect recon (L=3): 1

    // Test 10: Polyphase decomposition
    int pp0[4];
    int pp1[4];
    polyphase_decompose(x3, 8, pp0, pp1, 2);
    // p0: indices 0,2,4,6 -> 1,3,5,7
    // p1: indices 1,3,5,7 -> 2,4,6,8
    printf("Poly p0: %d %d %d %d\n", pp0[0], pp0[1], pp0[2], pp0[3]);
    // EXPECT: Poly p0: 1 3 5 7
    printf("Poly p1: %d %d %d %d\n", pp1[0], pp1[1], pp1[2], pp1[3]);
    // EXPECT: Poly p1: 2 4 6 8

    // Test 11: Length calculations
    printf("Up len 4x3: %d\n", upsample_len(4, 3));
    // EXPECT: Up len 4x3: 12
    printf("Down len 10/3: %d\n", downsample_len(10, 3));
    // EXPECT: Down len 10/3: 4
    printf("Down len 9/3: %d\n", downsample_len(9, 3));
    // EXPECT: Down len 9/3: 3

    // Test 12: Downsample preserves DC level (with averaging)
    int dc[8];
    int k;
    for (k = 0; k < 8; k++) dc[k] = 42;
    int ydc[4];
    downsample_avg(dc, 8, ydc, 2);
    printf("DC preserved: %d %d %d %d\n", ydc[0], ydc[1], ydc[2], ydc[3]);
    // EXPECT: DC preserved: 42 42 42 42

    // Test 13: Interpolation preserves endpoints
    int xe[3];
    xe[0] = 50; xe[1] = 100; xe[2] = 50;
    int ye[5];
    upsample_interp(xe, 3, ye, 2);
    // y[0]=50, y[1]=75, y[2]=100, y[3]=75, y[4]=50
    printf("Interp ends: %d %d %d %d %d\n", ye[0], ye[1], ye[2], ye[3], ye[4]);
    // EXPECT: Interp ends: 50 75 100 75 50

    printf("All resampling tests passed\n");
    // EXPECT: All resampling tests passed

    return 0;
}
