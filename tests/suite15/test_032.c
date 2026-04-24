int printf(const char *fmt, ...);

// IIR filter (first order, integer scaled)
// y[n] = (a * x[n] + b * y[n-1]) / SCALE
// This implements a first-order recursive filter with integer arithmetic

void iir_first_order(int *x, int *y, int len, int a, int b, int scale) {
    int i;
    y[0] = (a * x[0]) / scale;
    for (i = 1; i < len; i++) {
        y[i] = (a * x[i] + b * y[i - 1]) / scale;
    }
}

// Second order IIR: y[n] = (a0*x[n] + a1*x[n-1] - b1*y[n-1]) / scale
void iir_second_order(int *x, int *y, int len, int a0, int a1, int b1, int scale) {
    y[0] = (a0 * x[0]) / scale;
    int i;
    for (i = 1; i < len; i++) {
        int xprev = x[i - 1];
        int yprev = y[i - 1];
        y[i] = (a0 * x[i] + a1 * xprev - b1 * yprev) / scale;
    }
}

// Compute filter stability: |b| < scale means stable
int is_stable(int b, int scale) {
    int abs_b = b;
    if (abs_b < 0) abs_b = -abs_b;
    return abs_b < scale;
}

int main() {
    // Test 1: Low-pass IIR with a=20, b=80, scale=100
    // This is like y[n] = 0.2*x[n] + 0.8*y[n-1]
    int x1[8];
    int y1[8];
    int i;
    for (i = 0; i < 8; i++) {
        x1[i] = 100;  // step input
    }
    iir_first_order(x1, y1, 8, 20, 80, 100);
    // y[0] = (20*100)/100 = 20
    // y[1] = (20*100 + 80*20)/100 = (2000+1600)/100 = 36
    // y[2] = (2000 + 80*36)/100 = (2000+2880)/100 = 48
    // y[3] = (2000 + 80*48)/100 = (2000+3840)/100 = 58
    // y[4] = (2000 + 80*58)/100 = (2000+4640)/100 = 66
    // y[5] = (2000 + 80*66)/100 = (2000+5280)/100 = 72
    // y[6] = (2000 + 80*72)/100 = (2000+5760)/100 = 77
    // y[7] = (2000 + 80*77)/100 = (2000+6160)/100 = 81
    printf("IIR LP step: %d %d %d %d %d %d %d %d\n",
           y1[0], y1[1], y1[2], y1[3], y1[4], y1[5], y1[6], y1[7]);
    // EXPECT: IIR LP step: 20 36 48 58 66 72 77 81

    // Test 2: Impulse response of same filter
    int x2[8];
    int y2[8];
    x2[0] = 100;
    for (i = 1; i < 8; i++) x2[i] = 0;
    iir_first_order(x2, y2, 8, 20, 80, 100);
    // y[0] = 20
    // y[1] = (0 + 80*20)/100 = 16
    // y[2] = (0 + 80*16)/100 = 12
    // y[3] = (0 + 80*12)/100 = 9
    // y[4] = (0 + 80*9)/100 = 7
    // y[5] = (0 + 80*7)/100 = 5
    // y[6] = (0 + 80*5)/100 = 4
    // y[7] = (0 + 80*4)/100 = 3
    printf("IIR imp: %d %d %d %d %d %d %d %d\n",
           y2[0], y2[1], y2[2], y2[3], y2[4], y2[5], y2[6], y2[7]);
    // EXPECT: IIR imp: 20 16 12 9 7 5 4 3

    // Test 3: High-pass IIR: a=80, b=20, scale=100
    int y3[8];
    iir_first_order(x1, y3, 8, 80, 20, 100);
    // y[0] = (80*100)/100 = 80
    // y[1] = (80*100 + 20*80)/100 = (8000+1600)/100 = 96
    // y[2] = (8000 + 20*96)/100 = (8000+1920)/100 = 99
    // y[3] = (8000 + 20*99)/100 = (8000+1980)/100 = 99
    // y[4] = (8000 + 20*99)/100 = 99
    // y[5] = 99, y[6] = 99, y[7] = 99
    printf("IIR HP step: %d %d %d %d\n", y3[0], y3[1], y3[2], y3[3]);
    // EXPECT: IIR HP step: 80 96 99 99

    // Test 4: Stability check
    printf("Stable b=80 s=100: %d\n", is_stable(80, 100));
    // EXPECT: Stable b=80 s=100: 1
    printf("Stable b=100 s=100: %d\n", is_stable(100, 100));
    // EXPECT: Stable b=100 s=100: 0
    printf("Stable b=-50 s=100: %d\n", is_stable(-50, 100));
    // EXPECT: Stable b=-50 s=100: 1
    printf("Stable b=-100 s=100: %d\n", is_stable(-100, 100));
    // EXPECT: Stable b=-100 s=100: 0

    // Test 5: Second order IIR on step
    int y5[8];
    iir_second_order(x1, y5, 8, 30, 30, 40, 100);
    // y[0] = (30*100)/100 = 30
    // y[1] = (30*100 + 30*100 - 40*30)/100 = (3000+3000-1200)/100 = 48
    // y[2] = (3000 + 30*100 - 40*48)/100 = (3000+3000-1920)/100 = 40
    // y[3] = (3000 + 3000 - 40*40)/100 = (3000+3000-1600)/100 = 44
    // y[4] = (3000 + 3000 - 40*44)/100 = (6000-1760)/100 = 42
    // y[5] = (3000 + 3000 - 40*42)/100 = (6000-1680)/100 = 43
    // y[6] = (3000 + 3000 - 40*43)/100 = (6000-1720)/100 = 42
    // y[7] = (3000 + 3000 - 40*42)/100 = (6000-1680)/100 = 43
    printf("IIR2 step: %d %d %d %d %d %d %d %d\n",
           y5[0], y5[1], y5[2], y5[3], y5[4], y5[5], y5[6], y5[7]);
    // EXPECT: IIR2 step: 30 48 40 44 42 43 42 43

    // Test 6: IIR with negative feedback (oscillation damping)
    int x6[8];
    int y6[8];
    x6[0] = 1000; for (i = 1; i < 8; i++) x6[i] = 0;
    iir_first_order(x6, y6, 8, 50, -50, 100);
    // y[0] = (50*1000)/100 = 500
    // y[1] = (0 + (-50)*500)/100 = -250
    // y[2] = (0 + (-50)*(-250))/100 = 125
    // y[3] = (0 + (-50)*125)/100 = -62
    // y[4] = (0 + (-50)*(-62))/100 = 31
    // y[5] = (0 + (-50)*31)/100 = -15
    // y[6] = (0 + (-50)*(-15))/100 = 7
    // y[7] = (0 + (-50)*7)/100 = -3
    printf("IIR osc: %d %d %d %d %d %d %d %d\n",
           y6[0], y6[1], y6[2], y6[3], y6[4], y6[5], y6[6], y6[7]);
    // EXPECT: IIR osc: 500 -250 125 -62 31 -15 7 -3

    // Test 7: Sum of impulse response (DC gain check)
    int sum = 0;
    for (i = 0; i < 8; i++) {
        sum = sum + y2[i];
    }
    printf("Impulse sum: %d\n", sum);
    // EXPECT: Impulse sum: 76

    printf("All IIR tests passed\n");
    // EXPECT: All IIR tests passed

    return 0;
}
