int printf(const char *fmt, ...);

// Convolution of two integer sequences
// (y[n] = sum_{k} x[k] * h[n-k])
// Output length = xlen + hlen - 1

void convolve(int *x, int xlen, int *h, int hlen, int *y) {
    int outlen = xlen + hlen - 1;
    int n;
    for (n = 0; n < outlen; n++) {
        y[n] = 0;
    }
    int k;
    for (n = 0; n < outlen; n++) {
        int acc = 0;
        for (k = 0; k < xlen; k++) {
            int hidx = n - k;
            if (hidx >= 0 && hidx < hlen) {
                acc = acc + x[k] * h[hidx];
            }
        }
        y[n] = acc;
    }
}

// Circular convolution (mod N)
void circular_convolve(int *x, int *h, int n, int *y) {
    int i;
    int k;
    for (i = 0; i < n; i++) {
        int acc = 0;
        for (k = 0; k < n; k++) {
            int hidx = i - k;
            if (hidx < 0) hidx = hidx + n;
            acc = acc + x[k] * h[hidx];
        }
        y[i] = acc;
    }
}

// Auto-correlation: convolve signal with time-reversed self
void autocorrelation(int *x, int len, int *r) {
    int lag;
    for (lag = 0; lag < len; lag++) {
        int acc = 0;
        int i;
        for (i = 0; i < len - lag; i++) {
            acc = acc + x[i] * x[i + lag];
        }
        r[lag] = acc;
    }
}

int main() {
    // Test 1: Simple convolution [1,2,3] * [1,1] = [1,3,5,3]
    int x1[3];
    x1[0] = 1; x1[1] = 2; x1[2] = 3;
    int h1[2];
    h1[0] = 1; h1[1] = 1;
    int y1[4];  // length = 3 + 2 - 1 = 4
    convolve(x1, 3, h1, 2, y1);
    printf("Conv1: %d %d %d %d\n", y1[0], y1[1], y1[2], y1[3]);
    // EXPECT: Conv1: 1 3 5 3

    // Test 2: Convolution with impulse [0,1,0] * [a,b,c] = [0,a,b,c,0]
    int imp[3];
    imp[0] = 0; imp[1] = 1; imp[2] = 0;
    int h2[3];
    h2[0] = 5; h2[1] = 10; h2[2] = 15;
    int y2[5];  // 3+3-1 = 5
    convolve(imp, 3, h2, 3, y2);
    printf("Conv imp: %d %d %d %d %d\n", y2[0], y2[1], y2[2], y2[3], y2[4]);
    // EXPECT: Conv imp: 0 5 10 15 0

    // Test 3: Convolution is commutative
    int y3[5];
    convolve(h2, 3, imp, 3, y3);
    printf("Conv comm: %d %d %d %d %d\n", y3[0], y3[1], y3[2], y3[3], y3[4]);
    // EXPECT: Conv comm: 0 5 10 15 0

    // Test 4: Rectangular pulse convolution (triangle)
    // [1,1,1] * [1,1,1] = [1,2,3,2,1]
    int rect[3];
    rect[0] = 1; rect[1] = 1; rect[2] = 1;
    int y4[5];
    convolve(rect, 3, rect, 3, y4);
    printf("Rect conv: %d %d %d %d %d\n", y4[0], y4[1], y4[2], y4[3], y4[4]);
    // EXPECT: Rect conv: 1 2 3 2 1

    // Test 5: Convolution sum property
    // Sum of output = sum of x * sum of h
    int sum_x = 1 + 2 + 3;  // 6
    int sum_h = 1 + 1;       // 2
    int sum_y = y1[0] + y1[1] + y1[2] + y1[3];  // 1+3+5+3 = 12
    printf("Sum prop: %d = %d\n", sum_y, sum_x * sum_h);
    // EXPECT: Sum prop: 12 = 12

    // Test 6: Larger convolution
    int x6[4];
    x6[0] = 1; x6[1] = 3; x6[2] = 5; x6[3] = 7;
    int h6[3];
    h6[0] = 2; h6[1] = -1; h6[2] = 3;
    int y6[6];  // 4+3-1 = 6
    convolve(x6, 4, h6, 3, y6);
    // y[0] = 1*2 = 2
    // y[1] = 1*(-1) + 3*2 = -1+6 = 5
    // y[2] = 1*3 + 3*(-1) + 5*2 = 3-3+10 = 10
    // y[3] = 3*3 + 5*(-1) + 7*2 = 9-5+14 = 18
    // y[4] = 5*3 + 7*(-1) = 15-7 = 8
    // y[5] = 7*3 = 21
    printf("Conv6: %d %d %d %d %d %d\n", y6[0], y6[1], y6[2], y6[3], y6[4], y6[5]);
    // EXPECT: Conv6: 2 5 10 18 8 21

    // Test 7: Circular convolution
    int cx[4];
    cx[0] = 1; cx[1] = 2; cx[2] = 3; cx[3] = 4;
    int ch[4];
    ch[0] = 1; ch[1] = 0; ch[2] = 0; ch[3] = 1;
    int cy[4];
    circular_convolve(cx, ch, 4, cy);
    // cy[0] = x[0]*h[0] + x[1]*h[3] + x[2]*h[2] + x[3]*h[1]
    //       = 1*1 + 2*1 + 3*0 + 4*0 = 3
    // cy[1] = x[0]*h[1] + x[1]*h[0] + x[2]*h[3] + x[3]*h[2]
    //       = 1*0 + 2*1 + 3*1 + 4*0 = 5
    // cy[2] = x[0]*h[2] + x[1]*h[1] + x[2]*h[0] + x[3]*h[3]
    //       = 1*0 + 2*0 + 3*1 + 4*1 = 7
    // cy[3] = x[0]*h[3] + x[1]*h[2] + x[2]*h[1] + x[3]*h[0]
    //       = 1*1 + 2*0 + 3*0 + 4*1 = 5
    printf("Circ conv: %d %d %d %d\n", cy[0], cy[1], cy[2], cy[3]);
    // EXPECT: Circ conv: 3 5 7 5

    // Test 8: Auto-correlation
    int sig[4];
    sig[0] = 1; sig[1] = -1; sig[2] = 1; sig[3] = -1;
    int r[4];
    autocorrelation(sig, 4, r);
    // r[0] = 1*1 + (-1)*(-1) + 1*1 + (-1)*(-1) = 4
    // r[1] = 1*(-1) + (-1)*1 + 1*(-1) = -3
    // r[2] = 1*1 + (-1)*(-1) = 2
    // r[3] = 1*(-1) = -1
    printf("Autocorr: %d %d %d %d\n", r[0], r[1], r[2], r[3]);
    // EXPECT: Autocorr: 4 -3 2 -1

    // Test 9: Autocorrelation of constant signal
    int csig[4];
    csig[0] = 3; csig[1] = 3; csig[2] = 3; csig[3] = 3;
    int rc[4];
    autocorrelation(csig, 4, rc);
    // r[0] = 9+9+9+9 = 36
    // r[1] = 9+9+9 = 27
    // r[2] = 9+9 = 18
    // r[3] = 9
    printf("Autocorr const: %d %d %d %d\n", rc[0], rc[1], rc[2], rc[3]);
    // EXPECT: Autocorr const: 36 27 18 9

    // Test 10: Verify r[0] >= |r[k]| for all k
    int valid = 1;
    int i;
    for (i = 1; i < 4; i++) {
        int abs_ri = r[i];
        if (abs_ri < 0) abs_ri = -abs_ri;
        if (abs_ri > r[0]) valid = 0;
    }
    printf("Autocorr max at 0: %d\n", valid);
    // EXPECT: Autocorr max at 0: 1

    printf("All convolution tests passed\n");
    // EXPECT: All convolution tests passed

    return 0;
}
