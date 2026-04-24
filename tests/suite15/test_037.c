int printf(const char *fmt, ...);

// Window functions (integer approximations, scaled by 1000)
// Rectangular, Triangular, Hamming approximation

// Rectangular window: all 1s
void rectangular_window(int *w, int n) {
    int i;
    for (i = 0; i < n; i++) {
        w[i] = 1000;
    }
}

// Triangular (Bartlett) window: rises linearly to center, falls back
// w[i] = 1000 * (1 - |2*i/(N-1) - 1|)
// Integer version: w[i] = 2000 * min(i, N-1-i) / (N-1)
void triangular_window(int *w, int n) {
    int i;
    for (i = 0; i < n; i++) {
        int d = i;
        int e = n - 1 - i;
        int mn = d;
        if (e < mn) mn = e;
        w[i] = (2000 * mn) / (n - 1);
    }
}

// Hamming window approximation (integer):
// w[i] = 540 - 460 * cos(2*pi*i/(N-1))
// We approximate cos using a polynomial:
// cos(x) ~ 1 - x^2/2 + x^4/24 (scaled)
// For simplicity, use a lookup table approach for small N

// Cosine approximation: input angle in units of pi/1000, output scaled by 1000
// cos(0) = 1000, cos(pi/2)=0, cos(pi)=-1000
// Using parabolic approximation: cos(x) ~ 1000 - (2000*x*x)/(pi^2)
// where x is in range [0, pi] mapped to [0, 1000]
int cos_approx(int x_milli) {
    // x_milli is angle * 1000 / pi, in range [0, 2000] representing [0, 2*pi]
    // Normalize to [0, 2000]
    while (x_milli < 0) x_milli = x_milli + 2000;
    while (x_milli >= 2000) x_milli = x_milli - 2000;

    // Map to [0, 1000] for first half
    int neg = 0;
    if (x_milli > 1000) {
        x_milli = 2000 - x_milli;
        // cos is symmetric, so cos(2pi-x) = cos(x), no negation needed for mapping
    }
    // Now x_milli in [0, 1000] representing [0, pi]
    // cos(0)=1000, cos(500)=0, cos(1000)=-1000
    // Parabolic: cos ~ 1000 - 4*x*x/1000  (where x is in [0, 1000])
    // At x=0: 1000, at x=500: 1000-1000=0, at x=1000: 1000-4000=-3000 (wrong!)
    // Better: use piecewise linear approximation
    // [0, 333] -> [1000, 500]
    // [333, 667] -> [500, -500]
    // [667, 1000] -> [-500, -1000]
    if (x_milli <= 333) {
        return 1000 - (1500 * x_milli) / 1000;
    } else if (x_milli <= 667) {
        return 500 - (3000 * (x_milli - 333)) / 1000;
    } else {
        return -500 - (1500 * (x_milli - 667)) / 1000;
    }
}

// Hamming window using cos approximation
// w[i] = 540 - 460 * cos(2*pi*i/(N-1))
// cos arg in our units: 2*pi*i/(N-1) -> 2000*i/(N-1)
void hamming_window(int *w, int n) {
    int i;
    for (i = 0; i < n; i++) {
        int angle = (2000 * i) / (n - 1);
        int c = cos_approx(angle);
        w[i] = 540 - (460 * c) / 1000;
    }
}

// Apply window to signal: y[i] = x[i] * w[i] / 1000
void apply_window(int *x, int *w, int *y, int n) {
    int i;
    for (i = 0; i < n; i++) {
        y[i] = (x[i] * w[i]) / 1000;
    }
}

// Window energy: sum of w[i]^2
int window_energy(int *w, int n) {
    int i;
    int sum = 0;
    for (i = 0; i < n; i++) {
        sum = sum + (w[i] * w[i]) / 1000;
    }
    return sum;
}

int main() {
    int w[8];
    int i;

    // Test 1: Rectangular window
    rectangular_window(w, 5);
    printf("Rect: %d %d %d %d %d\n", w[0], w[1], w[2], w[3], w[4]);
    // EXPECT: Rect: 1000 1000 1000 1000 1000

    // Test 2: Triangular window N=5
    triangular_window(w, 5);
    // w[0] = 2000*min(0,4)/4 = 0
    // w[1] = 2000*min(1,3)/4 = 2000*1/4 = 500
    // w[2] = 2000*min(2,2)/4 = 2000*2/4 = 1000
    // w[3] = 2000*min(3,1)/4 = 2000*1/4 = 500
    // w[4] = 2000*min(4,0)/4 = 0
    printf("Tri5: %d %d %d %d %d\n", w[0], w[1], w[2], w[3], w[4]);
    // EXPECT: Tri5: 0 500 1000 500 0

    // Test 3: Triangular window N=7
    int w7[7];
    triangular_window(w7, 7);
    // w[0] = 2000*min(0,6)/6 = 0
    // w[1] = 2000*min(1,5)/6 = 2000/6 = 333
    // w[2] = 2000*min(2,4)/6 = 4000/6 = 666
    // w[3] = 2000*min(3,3)/6 = 6000/6 = 1000
    // w[4] = 666, w[5] = 333, w[6] = 0
    printf("Tri7: %d %d %d %d %d %d %d\n",
           w7[0], w7[1], w7[2], w7[3], w7[4], w7[5], w7[6]);
    // EXPECT: Tri7: 0 333 666 1000 666 333 0

    // Test 4: Triangular is symmetric
    int sym = 1;
    for (i = 0; i < 7; i++) {
        if (w7[i] != w7[6 - i]) sym = 0;
    }
    printf("Tri symmetric: %d\n", sym);
    // EXPECT: Tri symmetric: 1

    // Test 5: Cos approximation tests
    printf("cos(0): %d\n", cos_approx(0));
    // EXPECT: cos(0): 1000
    printf("cos(1000): %d\n", cos_approx(1000));
    // EXPECT: cos(1000): -999

    // Test 6: Apply rectangular window to signal (should be identity)
    int sig[5];
    sig[0] = 10; sig[1] = 20; sig[2] = 30; sig[3] = 40; sig[4] = 50;
    int wr[5];
    rectangular_window(wr, 5);
    int yr[5];
    apply_window(sig, wr, yr, 5);
    printf("Rect apply: %d %d %d %d %d\n", yr[0], yr[1], yr[2], yr[3], yr[4]);
    // EXPECT: Rect apply: 10 20 30 40 50

    // Test 7: Apply triangular window to constant signal
    int csig[5];
    csig[0] = 100; csig[1] = 100; csig[2] = 100; csig[3] = 100; csig[4] = 100;
    int wt[5];
    triangular_window(wt, 5);
    int yt[5];
    apply_window(csig, wt, yt, 5);
    // yt[0] = 100*0/1000 = 0
    // yt[1] = 100*500/1000 = 50
    // yt[2] = 100*1000/1000 = 100
    // yt[3] = 50, yt[4] = 0
    printf("Tri apply: %d %d %d %d %d\n", yt[0], yt[1], yt[2], yt[3], yt[4]);
    // EXPECT: Tri apply: 0 50 100 50 0

    // Test 8: Window energy of rectangular
    int e_rect = window_energy(wr, 5);
    // 5 * 1000*1000/1000 = 5000
    printf("Energy rect: %d\n", e_rect);
    // EXPECT: Energy rect: 5000

    // Test 9: Window energy of triangular (N=5)
    int e_tri = window_energy(wt, 5);
    // 0 + 500*500/1000 + 1000*1000/1000 + 250000/1000 + 0
    // = 0 + 250 + 1000 + 250 + 0 = 1500
    printf("Energy tri: %d\n", e_tri);
    // EXPECT: Energy tri: 1500

    // Test 10: Hamming window (N=5)
    int wh[5];
    hamming_window(wh, 5);
    // i=0: angle=0, cos=1000, w = 540 - 460*1000/1000 = 540-460 = 80
    // i=1: angle=500, cos=0,   w = 540 - 0 = 540
    // i=2: angle=1000, cos=-1000, w = 540 - 460*(-1000)/1000 = 540+460 = 1000
    // i=3: angle=1500, cos=0 (by symmetry of cos(2pi-x)=cos(x))
    //      1500 > 1000, so x_milli = 2000-1500 = 500, cos(500)=0, w=540
    // i=4: angle=2000 -> normalized to 0, cos=1000, w=80
    printf("Hamming: %d %d %d %d %d\n", wh[0], wh[1], wh[2], wh[3], wh[4]);
    // EXPECT: Hamming: 80 540 999 540 80

    // Test 11: Hamming window is symmetric
    int hsym = 1;
    for (i = 0; i < 5; i++) {
        if (wh[i] != wh[4 - i]) hsym = 0;
    }
    printf("Hamming symmetric: %d\n", hsym);
    // EXPECT: Hamming symmetric: 1

    printf("All window tests passed\n");
    // EXPECT: All window tests passed

    return 0;
}
