int printf(const char *fmt, ...);

// Moving average filter with various window sizes

void moving_avg(int *x, int *y, int len, int window) {
    int i;
    int k;
    for (i = 0; i < len; i++) {
        int sum = 0;
        int count = 0;
        for (k = 0; k < window; k++) {
            int idx = i - k;
            if (idx >= 0) {
                sum = sum + x[idx];
                count = count + 1;
            }
        }
        y[i] = sum / count;
    }
}

// Centered moving average: uses window/2 on each side
void centered_avg(int *x, int *y, int len, int half_win) {
    int i;
    int k;
    for (i = 0; i < len; i++) {
        int sum = 0;
        int count = 0;
        for (k = -half_win; k <= half_win; k++) {
            int idx = i + k;
            if (idx >= 0 && idx < len) {
                sum = sum + x[idx];
                count = count + 1;
            }
        }
        y[i] = sum / count;
    }
}

// Exponential moving average (integer): y[n] = (alpha*x[n] + (100-alpha)*y[n-1]) / 100
void ema(int *x, int *y, int len, int alpha) {
    int i;
    y[0] = x[0];
    for (i = 1; i < len; i++) {
        y[i] = (alpha * x[i] + (100 - alpha) * y[i - 1]) / 100;
    }
}

// Weighted moving average: recent samples weighted more
void weighted_avg(int *x, int *y, int len, int window) {
    int i;
    int k;
    for (i = 0; i < len; i++) {
        int sum = 0;
        int weight_sum = 0;
        for (k = 0; k < window; k++) {
            int idx = i - k;
            if (idx >= 0) {
                int w = window - k;  // higher weight for recent
                sum = sum + w * x[idx];
                weight_sum = weight_sum + w;
            }
        }
        y[i] = sum / weight_sum;
    }
}

int main() {
    int x[10];
    x[0] = 10; x[1] = 20; x[2] = 30; x[3] = 40; x[4] = 50;
    x[5] = 60; x[6] = 70; x[7] = 80; x[8] = 90; x[9] = 100;

    // Test 1: Window size 1 (identity)
    int y1[10];
    moving_avg(x, y1, 10, 1);
    printf("MA1: %d %d %d %d %d\n", y1[0], y1[1], y1[2], y1[3], y1[4]);
    // EXPECT: MA1: 10 20 30 40 50

    // Test 2: Window size 2
    int y2[10];
    moving_avg(x, y2, 10, 2);
    // y[0] = 10/1 = 10  (only 1 sample available)
    // y[1] = (20+10)/2 = 15
    // y[2] = (30+20)/2 = 25
    // y[3] = (40+30)/2 = 35
    // y[4] = (50+40)/2 = 45
    printf("MA2: %d %d %d %d %d\n", y2[0], y2[1], y2[2], y2[3], y2[4]);
    // EXPECT: MA2: 10 15 25 35 45

    // Test 3: Window size 3
    int y3[10];
    moving_avg(x, y3, 10, 3);
    // y[0] = 10/1 = 10
    // y[1] = (20+10)/2 = 15
    // y[2] = (30+20+10)/3 = 20
    // y[3] = (40+30+20)/3 = 30
    // y[4] = (50+40+30)/3 = 40
    printf("MA3: %d %d %d %d %d\n", y3[0], y3[1], y3[2], y3[3], y3[4]);
    // EXPECT: MA3: 10 15 20 30 40

    // Test 4: Window size 5
    int y4[10];
    moving_avg(x, y4, 10, 5);
    // y[0] = 10/1 = 10
    // y[1] = 30/2 = 15
    // y[2] = 60/3 = 20
    // y[3] = 100/4 = 25
    // y[4] = 150/5 = 30
    // y[5] = 200/5 = 40
    // y[6] = 250/5 = 50
    printf("MA5: %d %d %d %d %d %d %d\n", y4[0], y4[1], y4[2], y4[3], y4[4], y4[5], y4[6]);
    // EXPECT: MA5: 10 15 20 25 30 40 50

    // Test 5: Noisy signal smoothing
    int noisy[8];
    noisy[0] = 100; noisy[1] = 120; noisy[2] = 90; noisy[3] = 110;
    noisy[4] = 95;  noisy[5] = 115; noisy[6] = 88; noisy[7] = 105;
    int ys[8];
    moving_avg(noisy, ys, 8, 3);
    // y[0] = 100/1 = 100
    // y[1] = (120+100)/2 = 110
    // y[2] = (90+120+100)/3 = 103
    // y[3] = (110+90+120)/3 = 106
    // y[4] = (95+110+90)/3 = 98
    // y[5] = (115+95+110)/3 = 106
    // y[6] = (88+115+95)/3 = 99
    // y[7] = (105+88+115)/3 = 102
    printf("Smooth: %d %d %d %d %d %d %d %d\n",
           ys[0], ys[1], ys[2], ys[3], ys[4], ys[5], ys[6], ys[7]);
    // EXPECT: Smooth: 100 110 103 106 98 106 99 102

    // Test 6: Centered moving average
    int yc[10];
    centered_avg(x, yc, 10, 1);  // half_win=1, so window of 3
    // yc[0] = (10+20)/2 = 15
    // yc[1] = (10+20+30)/3 = 20
    // yc[2] = (20+30+40)/3 = 30
    // yc[3] = (30+40+50)/3 = 40
    // yc[9] = (90+100)/2 = 95
    printf("Cent: %d %d %d %d\n", yc[0], yc[1], yc[2], yc[3]);
    // EXPECT: Cent: 15 20 30 40
    printf("Cent end: %d\n", yc[9]);
    // EXPECT: Cent end: 95

    // Test 7: Exponential moving average
    int ye[10];
    ema(x, ye, 10, 30);  // alpha=30
    // ye[0] = 10
    // ye[1] = (30*20 + 70*10)/100 = (600+700)/100 = 13
    // ye[2] = (30*30 + 70*13)/100 = (900+910)/100 = 18
    // ye[3] = (30*40 + 70*18)/100 = (1200+1260)/100 = 24
    // ye[4] = (30*50 + 70*24)/100 = (1500+1680)/100 = 31
    printf("EMA: %d %d %d %d %d\n", ye[0], ye[1], ye[2], ye[3], ye[4]);
    // EXPECT: EMA: 10 13 18 24 31

    // Test 8: Weighted moving average
    int yw[10];
    weighted_avg(x, yw, 10, 3);
    // yw[0] = (3*10)/3 = 10
    // yw[1] = (3*20 + 2*10)/(3+2) = (60+20)/5 = 16
    // yw[2] = (3*30 + 2*20 + 1*10)/(3+2+1) = (90+40+10)/6 = 23
    // yw[3] = (3*40 + 2*30 + 1*20)/6 = (120+60+20)/6 = 33
    // yw[4] = (3*50 + 2*40 + 1*30)/6 = (150+80+30)/6 = 43
    printf("WMA: %d %d %d %d %d\n", yw[0], yw[1], yw[2], yw[3], yw[4]);
    // EXPECT: WMA: 10 16 23 33 43

    printf("All moving average tests passed\n");
    // EXPECT: All moving average tests passed

    return 0;
}
