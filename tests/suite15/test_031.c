int printf(const char *fmt, ...);

// FIR filter with coefficient array
// y[n] = sum(h[k] * x[n-k]) for k=0..M-1
// All values scaled by 100 to avoid floats

void fir_filter(int *x, int xlen, int *h, int hlen, int *y) {
    int i;
    int k;
    for (i = 0; i < xlen; i++) {
        int acc = 0;
        for (k = 0; k < hlen; k++) {
            if (i - k >= 0) {
                acc = acc + h[k] * x[i - k];
            }
        }
        y[i] = acc / 100;
    }
}

void print_array(int *a, int len) {
    int i;
    for (i = 0; i < len; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

int main() {
    // Test 1: Simple averaging filter (h = {50, 50} means 0.5, 0.5 scaled by 100)
    int x1[6];
    x1[0] = 100; x1[1] = 200; x1[2] = 300; x1[3] = 400; x1[4] = 500; x1[5] = 600;
    int h1[2];
    h1[0] = 50; h1[1] = 50;
    int y1[6];
    fir_filter(x1, 6, h1, 2, y1);

    // y[0] = (50*100)/100 = 50
    // y[1] = (50*200 + 50*100)/100 = 150
    // y[2] = (50*300 + 50*200)/100 = 250
    // y[3] = (50*400 + 50*300)/100 = 350
    // y[4] = (50*500 + 50*400)/100 = 450
    // y[5] = (50*600 + 50*500)/100 = 550
    printf("FIR avg: %d %d %d %d %d %d\n", y1[0], y1[1], y1[2], y1[3], y1[4], y1[5]);
    // EXPECT: FIR avg: 50 150 250 350 450 550

    // Test 2: Differencing filter h = {100, -100}
    int h2[2];
    h2[0] = 100; h2[1] = -100;
    int y2[6];
    fir_filter(x1, 6, h2, 2, y2);
    // y[0] = (100*100)/100 = 100
    // y[1] = (100*200 + (-100)*100)/100 = 100
    // y[2] = (100*300 + (-100)*200)/100 = 100
    // y[3..5] all = 100
    printf("FIR diff: %d %d %d %d %d %d\n", y2[0], y2[1], y2[2], y2[3], y2[4], y2[5]);
    // EXPECT: FIR diff: 100 100 100 100 100 100

    // Test 3: 3-tap low pass h = {25, 50, 25}
    int x3[5];
    x3[0] = 0; x3[1] = 100; x3[2] = 0; x3[3] = 100; x3[4] = 0;
    int h3[3];
    h3[0] = 25; h3[1] = 50; h3[2] = 25;
    int y3[5];
    fir_filter(x3, 5, h3, 3, y3);
    // y[0] = (25*0)/100 = 0
    // y[1] = (25*100 + 50*0)/100 = 25
    // y[2] = (25*0 + 50*100 + 25*0)/100 = 50
    // y[3] = (25*100 + 50*0 + 25*100)/100 = 50
    // y[4] = (25*0 + 50*100 + 25*0)/100 = 50
    printf("FIR lp: %d %d %d %d %d\n", y3[0], y3[1], y3[2], y3[3], y3[4]);
    // EXPECT: FIR lp: 0 25 50 50 50

    // Test 4: impulse response of a 4-tap filter
    int imp[6];
    imp[0] = 100; imp[1] = 0; imp[2] = 0; imp[3] = 0; imp[4] = 0; imp[5] = 0;
    int h4[4];
    h4[0] = 10; h4[1] = 20; h4[2] = 30; h4[3] = 40;
    int y4[6];
    fir_filter(imp, 6, h4, 4, y4);
    // y[0] = (10*100)/100 = 10
    // y[1] = (20*100)/100 = 20
    // y[2] = (30*100)/100 = 30
    // y[3] = (40*100)/100 = 40
    // y[4] = 0, y[5] = 0
    printf("FIR imp: %d %d %d %d %d %d\n", y4[0], y4[1], y4[2], y4[3], y4[4], y4[5]);
    // EXPECT: FIR imp: 10 20 30 40 0 0

    // Test 5: step response
    int step[6];
    step[0] = 100; step[1] = 100; step[2] = 100; step[3] = 100; step[4] = 100; step[5] = 100;
    int y5[6];
    fir_filter(step, 6, h4, 4, y5);
    // y[0] = (10*100)/100 = 10
    // y[1] = (10*100 + 20*100)/100 = 30
    // y[2] = (10*100 + 20*100 + 30*100)/100 = 60
    // y[3] = (10*100 + 20*100 + 30*100 + 40*100)/100 = 100
    // y[4] = same = 100
    // y[5] = same = 100
    printf("FIR step: %d %d %d %d %d %d\n", y5[0], y5[1], y5[2], y5[3], y5[4], y5[5]);
    // EXPECT: FIR step: 10 30 60 100 100 100

    // Test 6: DC gain of filter
    int dc_gain = 0;
    int i;
    for (i = 0; i < 4; i++) {
        dc_gain = dc_gain + h4[i];
    }
    printf("DC gain: %d\n", dc_gain);
    // EXPECT: DC gain: 100

    // Test 7: high-pass filter on DC signal should give ~0
    int h_hp[3];
    h_hp[0] = 50; h_hp[1] = -100; h_hp[2] = 50;
    int dc_sig[5];
    dc_sig[0] = 200; dc_sig[1] = 200; dc_sig[2] = 200; dc_sig[3] = 200; dc_sig[4] = 200;
    int y7[5];
    fir_filter(dc_sig, 5, h_hp, 3, y7);
    // y[0] = (50*200)/100 = 100
    // y[1] = (50*200 + (-100)*200)/100 = -100
    // y[2] = (50*200 + (-100)*200 + 50*200)/100 = 0
    // y[3] = same = 0
    // y[4] = same = 0
    printf("HP on DC: %d %d %d %d %d\n", y7[0], y7[1], y7[2], y7[3], y7[4]);
    // EXPECT: HP on DC: 100 -100 0 0 0

    printf("All FIR tests passed\n");
    // EXPECT: All FIR tests passed

    return 0;
}
