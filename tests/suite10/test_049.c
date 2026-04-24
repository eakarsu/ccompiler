int printf(const char *fmt, ...);

// EXPECT: === Difference Equation Solver ===
// Difference Equation Solver (integer-based)
// Solves linear constant-coefficient difference equations
// y[n] = b0*x[n] + b1*x[n-1] + ... - a1*y[n-1] - a2*y[n-2] - ...
// All coefficients scaled by 100 to simulate fixed-point

struct DiffEq {
    int b[4];
    int a[4];
    int num_b;
    int num_a;
    int x_hist[4];
    int y_hist[4];
};

void deq_init(struct DiffEq *eq, int *b, int num_b, int *a, int num_a) {
    int i;
    eq->num_b = num_b;
    eq->num_a = num_a;
    for (i = 0; i < num_b; i++) eq->b[i] = b[i];
    for (i = num_b; i < 4; i++) eq->b[i] = 0;
    for (i = 0; i < num_a; i++) eq->a[i] = a[i];
    for (i = num_a; i < 4; i++) eq->a[i] = 0;
    for (i = 0; i < 4; i++) {
        eq->x_hist[i] = 0;
        eq->y_hist[i] = 0;
    }
}

int deq_step(struct DiffEq *eq, int x_in) {
    int y;
    int i;

    for (i = 3; i > 0; i--) {
        eq->x_hist[i] = eq->x_hist[i - 1];
    }
    eq->x_hist[0] = x_in;

    y = 0;
    for (i = 0; i < eq->num_b; i++) {
        y = y + eq->b[i] * eq->x_hist[i];
    }
    for (i = 0; i < eq->num_a; i++) {
        y = y - eq->a[i] * eq->y_hist[i];
    }
    y = y / 100;

    for (i = 3; i > 0; i--) {
        eq->y_hist[i] = eq->y_hist[i - 1];
    }
    eq->y_hist[0] = y;

    return y;
}

void test_first_order_lowpass(void) {
    struct DiffEq eq;
    int b[1];
    int a[1];
    int i;
    int out;

    b[0] = 20;
    a[0] = -80;

    deq_init(&eq, b, 1, a, 1);

    printf("First-order low-pass (step response):\n"); // EXPECT: First-order low-pass (step response):
    for (i = 0; i < 10; i++) {
        out = deq_step(&eq, 100);
        printf("  y[%d] = %d\n", i, out);
    }
    // EXPECT:   y[0] = 20
    // EXPECT:   y[1] = 36
    // EXPECT:   y[2] = 48
    // EXPECT:   y[3] = 58
    // EXPECT:   y[4] = 66
    // EXPECT:   y[5] = 72
    // EXPECT:   y[6] = 77
    // EXPECT:   y[7] = 81
    // EXPECT:   y[8] = 84
    // EXPECT:   y[9] = 87
}

void test_integrator(void) {
    struct DiffEq eq;
    int b[1];
    int a[1];
    int i;
    int out;

    b[0] = 100;
    a[0] = -100;

    deq_init(&eq, b, 1, a, 1);

    printf("Integrator (cumulative sum):\n"); // EXPECT: Integrator (cumulative sum):
    for (i = 0; i < 6; i++) {
        out = deq_step(&eq, 10);
        printf("  y[%d] = %d\n", i, out);
    }
    // EXPECT:   y[0] = 10
    // EXPECT:   y[1] = 20
    // EXPECT:   y[2] = 30
    // EXPECT:   y[3] = 40
    // EXPECT:   y[4] = 50
    // EXPECT:   y[5] = 60
}

void test_differentiator(void) {
    struct DiffEq eq;
    int b[2];
    int signal[8];
    int i;
    int out;

    b[0] = 100;
    b[1] = -100;

    deq_init(&eq, b, 2, (int*)0, 0);

    signal[0] = 10;
    signal[1] = 30;
    signal[2] = 60;
    signal[3] = 100;
    signal[4] = 100;
    signal[5] = 80;
    signal[6] = 50;
    signal[7] = 10;

    printf("Differentiator:\n"); // EXPECT: Differentiator:
    for (i = 0; i < 8; i++) {
        out = deq_step(&eq, signal[i]);
        printf("  y[%d] = %d\n", i, out);
    }
    // EXPECT:   y[0] = 10
    // EXPECT:   y[1] = 20
    // EXPECT:   y[2] = 30
    // EXPECT:   y[3] = 40
    // EXPECT:   y[4] = 0
    // EXPECT:   y[5] = -20
    // EXPECT:   y[6] = -30
    // EXPECT:   y[7] = -40
}

void test_second_order(void) {
    struct DiffEq eq;
    int b[1];
    int a[2];
    int i;
    int out;

    b[0] = 10;
    a[0] = -120;
    a[1] = 50;

    deq_init(&eq, b, 1, a, 2);

    printf("Second-order (step response):\n"); // EXPECT: Second-order (step response):
    for (i = 0; i < 8; i++) {
        out = deq_step(&eq, 100);
        printf("  y[%d] = %d\n", i, out);
    }
    // EXPECT:   y[0] = 10
    // EXPECT:   y[1] = 22
    // EXPECT:   y[2] = 31
    // EXPECT:   y[3] = 36
    // EXPECT:   y[4] = 37
    // EXPECT:   y[5] = 36
    // EXPECT:   y[6] = 34
    // EXPECT:   y[7] = 32
}

void test_impulse_response(void) {
    struct DiffEq eq;
    int b[2];
    int a[1];
    int i;
    int out;

    b[0] = 50;
    b[1] = 50;
    a[0] = -50;

    deq_init(&eq, b, 2, a, 1);

    printf("Impulse response:\n"); // EXPECT: Impulse response:
    for (i = 0; i < 8; i++) {
        int inp;
        if (i == 0) {
            inp = 100;
        } else {
            inp = 0;
        }
        out = deq_step(&eq, inp);
        printf("  h[%d] = %d\n", i, out);
    }
    // EXPECT:   h[0] = 50
    // EXPECT:   h[1] = 75
    // EXPECT:   h[2] = 37
    // EXPECT:   h[3] = 18
    // EXPECT:   h[4] = 9
    // EXPECT:   h[5] = 4
    // EXPECT:   h[6] = 2
    // EXPECT:   h[7] = 1
}

int main(void) {
    printf("=== Difference Equation Solver ===\n");
    test_first_order_lowpass();
    test_integrator();
    test_differentiator();
    test_second_order();
    test_impulse_response();
    printf("All difference equation tests passed.\n"); // EXPECT: All difference equation tests passed.
    return 0;
}
