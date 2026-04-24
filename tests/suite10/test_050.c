int printf(const char *fmt, ...);

// EXPECT: === Window Functions ===
// Window Functions (integer-based, scaled by 1000)
// Rectangular, Triangular, Bartlett, Hanning-like, Hamming-like

void rectangular_window(int *win, int len) {
    int i;
    for (i = 0; i < len; i++) {
        win[i] = 1000;
    }
}

void triangular_window(int *win, int len) {
    int i;
    int mid = (len - 1);
    for (i = 0; i < len; i++) {
        int dist = 2 * i - (len - 1);
        if (dist < 0) dist = -dist;
        win[i] = 1000 - (dist * 1000) / (len - 1);
    }
}

void bartlett_window(int *win, int len) {
    int i;
    int half = len / 2;
    for (i = 0; i < len; i++) {
        if (i <= half) {
            win[i] = (i * 1000) / half;
        } else {
            win[i] = ((len - 1 - i) * 1000) / half;
        }
    }
}

void apply_window(int *signal, int *win, int *output, int len) {
    int i;
    for (i = 0; i < len; i++) {
        output[i] = (signal[i] * win[i]) / 1000;
    }
}

int window_energy(int *win, int len) {
    int sum = 0;
    int i;
    for (i = 0; i < len; i++) {
        sum = sum + (win[i] * win[i]) / 1000;
    }
    return sum;
}

int window_sum(int *win, int len) {
    int sum = 0;
    int i;
    for (i = 0; i < len; i++) {
        sum = sum + win[i];
    }
    return sum;
}

void test_rectangular(void) {
    int win[8];
    int signal[8];
    int output[8];
    int i;

    rectangular_window(win, 8);
    printf("Rectangular window (len=8):\n"); // EXPECT: Rectangular window (len=8):
    for (i = 0; i < 8; i++) {
        printf("  w[%d] = %d\n", i, win[i]);
    }
    // EXPECT:   w[0] = 1000
    // EXPECT:   w[1] = 1000
    // EXPECT:   w[2] = 1000
    // EXPECT:   w[3] = 1000
    // EXPECT:   w[4] = 1000
    // EXPECT:   w[5] = 1000
    // EXPECT:   w[6] = 1000
    // EXPECT:   w[7] = 1000

    for (i = 0; i < 8; i++) signal[i] = 50;
    apply_window(signal, win, output, 8);
    printf("Applied to constant 50: %d %d %d %d\n",
           output[0], output[1], output[2], output[3]);
    // EXPECT: Applied to constant 50: 50 50 50 50
}

void test_triangular(void) {
    int win[9];
    int i;
    int sum;

    triangular_window(win, 9);
    printf("Triangular window (len=9):\n"); // EXPECT: Triangular window (len=9):
    for (i = 0; i < 9; i++) {
        printf("  w[%d] = %d\n", i, win[i]);
    }
    // EXPECT:   w[0] = 0
    // EXPECT:   w[1] = 250
    // EXPECT:   w[2] = 500
    // EXPECT:   w[3] = 750
    // EXPECT:   w[4] = 1000
    // EXPECT:   w[5] = 750
    // EXPECT:   w[6] = 500
    // EXPECT:   w[7] = 250
    // EXPECT:   w[8] = 0

    sum = window_sum(win, 9);
    printf("Triangular sum: %d\n", sum); // EXPECT: Triangular sum: 4000
}

void test_bartlett(void) {
    int win[8];
    int i;

    bartlett_window(win, 8);
    printf("Bartlett window (len=8):\n"); // EXPECT: Bartlett window (len=8):
    for (i = 0; i < 8; i++) {
        printf("  w[%d] = %d\n", i, win[i]);
    }
    // EXPECT:   w[0] = 0
    // EXPECT:   w[1] = 250
    // EXPECT:   w[2] = 500
    // EXPECT:   w[3] = 750
    // EXPECT:   w[4] = 1000
    // EXPECT:   w[5] = 500
    // EXPECT:   w[6] = 250
    // EXPECT:   w[7] = 0
}

void test_window_application(void) {
    int win[8];
    int signal[8];
    int output[8];
    int i;

    triangular_window(win, 8);

    for (i = 0; i < 8; i++) {
        signal[i] = 100;
    }

    apply_window(signal, win, output, 8);
    printf("Triangular applied to constant 100:\n"); // EXPECT: Triangular applied to constant 100:
    for (i = 0; i < 8; i++) {
        printf("  y[%d] = %d\n", i, output[i]);
    }
    // EXPECT:   y[0] = 0
    // EXPECT:   y[1] = 28
    // EXPECT:   y[2] = 57
    // EXPECT:   y[3] = 85
    // EXPECT:   y[4] = 85
    // EXPECT:   y[5] = 57
    // EXPECT:   y[6] = 28
    // EXPECT:   y[7] = 0
}

void test_energy_comparison(void) {
    int rect[8];
    int tri[8];
    int bart[8];
    int e_rect;
    int e_tri;
    int e_bart;

    rectangular_window(rect, 8);
    triangular_window(tri, 8);
    bartlett_window(bart, 8);

    e_rect = window_energy(rect, 8);
    e_tri = window_energy(tri, 8);
    e_bart = window_energy(bart, 8);

    printf("Rect energy: %d\n", e_rect); // EXPECT: Rect energy: 8000
    printf("Tri energy: %d\n", e_tri); // EXPECT: Tri energy: 2288
    printf("Bart energy: %d\n", e_bart); // EXPECT: Bart energy: 2186
    printf("Rect > Tri: %d\n", e_rect > e_tri); // EXPECT: Rect > Tri: 1
    printf("Rect > Bart: %d\n", e_rect > e_bart); // EXPECT: Rect > Bart: 1
}

int main(void) {
    printf("=== Window Functions ===\n");
    test_rectangular();
    test_triangular();
    test_bartlett();
    test_window_application();
    test_energy_comparison();
    printf("All window function tests passed.\n"); // EXPECT: All window function tests passed.
    return 0;
}
