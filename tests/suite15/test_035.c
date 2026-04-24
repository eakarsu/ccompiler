int printf(const char *fmt, ...);

// Cross-correlation computation
// r_xy[lag] = sum_{i} x[i] * y[i + lag]

void cross_correlate(int *x, int *y, int len, int *r, int max_lag) {
    int lag;
    for (lag = 0; lag < max_lag; lag++) {
        int acc = 0;
        int i;
        for (i = 0; i < len - lag; i++) {
            acc = acc + x[i] * y[i + lag];
        }
        r[lag] = acc;
    }
}

// Negative lag cross-correlation: r[-lag] = sum x[i+lag] * y[i]
void cross_correlate_neg(int *x, int *y, int len, int *r, int max_lag) {
    int lag;
    for (lag = 0; lag < max_lag; lag++) {
        int acc = 0;
        int i;
        for (i = 0; i < len - lag; i++) {
            acc = acc + x[i + lag] * y[i];
        }
        r[lag] = acc;
    }
}

// Normalized cross-correlation (integer approx): r_xy / sqrt(r_xx[0] * r_yy[0])
// We approximate by scaling: result * 1000 / sqrt_approx
int isqrt(int n) {
    if (n <= 0) return 0;
    int x = n;
    int y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

int normalized_xcorr(int *x, int *y, int len, int lag) {
    int rxx = 0;
    int ryy = 0;
    int rxy = 0;
    int i;
    for (i = 0; i < len - lag; i++) {
        rxy = rxy + x[i] * y[i + lag];
    }
    for (i = 0; i < len; i++) {
        rxx = rxx + x[i] * x[i];
        ryy = ryy + y[i] * y[i];
    }
    int denom = isqrt(rxx * ryy);
    if (denom == 0) return 0;
    return (rxy * 1000) / denom;
}

// Find lag with maximum correlation
int find_max_lag(int *r, int max_lag) {
    int best = 0;
    int best_val = r[0];
    int i;
    for (i = 1; i < max_lag; i++) {
        if (r[i] > best_val) {
            best_val = r[i];
            best = i;
        }
    }
    return best;
}

int main() {
    // Test 1: Cross-correlation of identical signals
    int x1[5];
    x1[0] = 1; x1[1] = 2; x1[2] = 3; x1[3] = 2; x1[4] = 1;
    int r1[5];
    cross_correlate(x1, x1, 5, r1, 5);
    // lag 0: 1+4+9+4+1 = 19
    // lag 1: 1*2+2*3+3*2+2*1 = 2+6+6+2 = 16
    // lag 2: 1*3+2*2+3*1 = 3+4+3 = 10
    // lag 3: 1*2+2*1 = 2+2 = 4
    // lag 4: 1*1 = 1
    printf("Xcorr self: %d %d %d %d %d\n", r1[0], r1[1], r1[2], r1[3], r1[4]);
    // EXPECT: Xcorr self: 19 16 10 4 1

    // Test 2: Maximum at lag 0 for self-correlation
    int max_l = find_max_lag(r1, 5);
    printf("Max lag self: %d\n", max_l);
    // EXPECT: Max lag self: 0

    // Test 3: Cross-correlation of shifted signals
    int x2[6];
    x2[0] = 0; x2[1] = 0; x2[2] = 1; x2[3] = 2; x2[4] = 1; x2[5] = 0;
    int y2[6];
    y2[0] = 1; y2[1] = 2; y2[2] = 1; y2[3] = 0; y2[4] = 0; y2[5] = 0;
    // y is x shifted left by 2
    int r2[6];
    cross_correlate(x2, y2, 6, r2, 6);
    // lag 0: 0*1+0*2+1*1+2*0+1*0+0*0 = 1
    // lag 1: 0*2+0*1+1*0+2*0+1*0 = 0
    // lag 2: 0*1+0*0+1*0+2*0 = 0
    // lag 3: 0*0+0*0+1*0 = 0
    // lag 4: 0*0+0*0 = 0
    // lag 5: 0*0 = 0
    printf("Xcorr shift: %d %d %d %d\n", r2[0], r2[1], r2[2], r2[3]);
    // EXPECT: Xcorr shift: 1 0 0 0

    // Now try negative lag correlation (equivalent to correlating y with x)
    int rn[6];
    cross_correlate_neg(x2, y2, 6, rn, 4);
    // neg lag 0: same as r2[0] = 0*1+0*2+1*1+2*0+1*0+0*0 = 1
    // neg lag 1: x[1]*y[0]+x[2]*y[1]+x[3]*y[2]+x[4]*y[3]+x[5]*y[4]
    //          = 0*1+1*2+2*1+1*0+0*0 = 4
    // neg lag 2: x[2]*y[0]+x[3]*y[1]+x[4]*y[2]+x[5]*y[3]
    //          = 1*1+2*2+1*1+0*0 = 6
    // neg lag 3: x[3]*y[0]+x[4]*y[1]+x[5]*y[2]
    //          = 2*1+1*2+0*1 = 4
    printf("Xcorr neg: %d %d %d %d\n", rn[0], rn[1], rn[2], rn[3]);
    // EXPECT: Xcorr neg: 1 4 6 4

    // Peak at negative lag 2 confirms signal is shifted by 2
    int peak_neg = find_max_lag(rn, 4);
    printf("Peak neg lag: %d\n", peak_neg);
    // EXPECT: Peak neg lag: 2

    // Test 4: Cross-correlation of orthogonal signals
    int xa[4];
    xa[0] = 1; xa[1] = -1; xa[2] = 1; xa[3] = -1;
    int ya[4];
    ya[0] = 1; ya[1] = 1; ya[2] = -1; ya[3] = -1;
    int ra[4];
    cross_correlate(xa, ya, 4, ra, 4);
    // lag 0: 1*1+(-1)*1+1*(-1)+(-1)*(-1) = 1-1-1+1 = 0
    // lag 1: 1*1+(-1)*(-1)+1*(-1) = 1+1-1 = 1
    // lag 2: 1*(-1)+(-1)*(-1) = -1+1 = 0
    // lag 3: 1*(-1) = -1
    printf("Xcorr orth: %d %d %d %d\n", ra[0], ra[1], ra[2], ra[3]);
    // EXPECT: Xcorr orth: 0 1 0 -1

    // Test 5: isqrt tests
    printf("sqrt 0: %d\n", isqrt(0));
    // EXPECT: sqrt 0: 0
    printf("sqrt 1: %d\n", isqrt(1));
    // EXPECT: sqrt 1: 1
    printf("sqrt 4: %d\n", isqrt(4));
    // EXPECT: sqrt 4: 2
    printf("sqrt 9: %d\n", isqrt(9));
    // EXPECT: sqrt 9: 3
    printf("sqrt 100: %d\n", isqrt(100));
    // EXPECT: sqrt 100: 10
    printf("sqrt 99: %d\n", isqrt(99));
    // EXPECT: sqrt 99: 9

    // Test 6: Normalized cross-correlation of identical signals
    int xn[4];
    xn[0] = 3; xn[1] = 4; xn[2] = 0; xn[3] = 0;
    // rxx = 9+16+0+0 = 25, ryy = 25
    // denom = sqrt(25*25) = 25
    // rxy at lag 0 = 9+16+0+0 = 25
    // normalized = 25*1000/25 = 1000
    int nc = normalized_xcorr(xn, xn, 4, 0);
    printf("Norm xcorr self: %d\n", nc);
    // EXPECT: Norm xcorr self: 1000

    // Test 7: Normalized cross-correlation with shift
    int xn2[4];
    xn2[0] = 0; xn2[1] = 3; xn2[2] = 4; xn2[3] = 0;
    int nc2 = normalized_xcorr(xn, xn2, 4, 1);
    // rxy lag1: x[0]*y[1]+x[1]*y[2]+x[2]*y[3] = 3*3+4*4+0*0 = 25
    // rxx = 25, ryy = 0+9+16+0 = 25
    // denom = sqrt(25*25) = 25
    // result = 25*1000/25 = 1000
    printf("Norm xcorr shift: %d\n", nc2);
    // EXPECT: Norm xcorr shift: 1000

    printf("All cross-correlation tests passed\n");
    // EXPECT: All cross-correlation tests passed

    return 0;
}
