int printf(const char *fmt, ...);

// EXPECT: === Zero-Crossing Counter ===
// Zero-Crossing Counter for integer signals
// Counts transitions through zero (sign changes)

int sign(int x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

int count_zero_crossings(int *signal, int len) {
    int count = 0;
    int i;
    int prev_sign;
    int curr_sign;

    prev_sign = sign(signal[0]);
    for (i = 1; i < len; i++) {
        curr_sign = sign(signal[i]);
        if (curr_sign != 0 && prev_sign != 0 && curr_sign != prev_sign) {
            count++;
        }
        if (curr_sign != 0) {
            prev_sign = curr_sign;
        }
    }
    return count;
}

int count_rising_crossings(int *signal, int len) {
    int count = 0;
    int i;
    for (i = 1; i < len; i++) {
        if (signal[i - 1] < 0 && signal[i] >= 0) {
            count++;
        }
    }
    return count;
}

int count_falling_crossings(int *signal, int len) {
    int count = 0;
    int i;
    for (i = 1; i < len; i++) {
        if (signal[i - 1] >= 0 && signal[i] < 0) {
            count++;
        }
    }
    return count;
}

int estimate_frequency_x100(int *signal, int len, int sample_rate) {
    int crossings = count_zero_crossings(signal, len);
    return (crossings * sample_rate * 100) / (2 * (len - 1));
}

void test_alternating(void) {
    int signal[10];
    int zc;
    int i;

    for (i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            signal[i] = 100;
        } else {
            signal[i] = -100;
        }
    }

    zc = count_zero_crossings(signal, 10);
    printf("Alternating signal crossings: %d\n", zc); // EXPECT: Alternating signal crossings: 9
}

void test_sine_like(void) {
    int signal[16];
    int zc;
    int rising;
    int falling;

    signal[0] = 0;
    signal[1] = 50;
    signal[2] = 87;
    signal[3] = 100;
    signal[4] = 87;
    signal[5] = 50;
    signal[6] = 0;
    signal[7] = -50;
    signal[8] = -87;
    signal[9] = -100;
    signal[10] = -87;
    signal[11] = -50;
    signal[12] = 0;
    signal[13] = 50;
    signal[14] = 87;
    signal[15] = 100;

    zc = count_zero_crossings(signal, 16);
    rising = count_rising_crossings(signal, 16);
    falling = count_falling_crossings(signal, 16);

    printf("Sine-like crossings: %d\n", zc); // EXPECT: Sine-like crossings: 2
    printf("Rising crossings: %d\n", rising); // EXPECT: Rising crossings: 1
    printf("Falling crossings: %d\n", falling); // EXPECT: Falling crossings: 1
}

void test_constant_positive(void) {
    int signal[5];
    int zc;
    signal[0] = 5; signal[1] = 10; signal[2] = 15; signal[3] = 20; signal[4] = 25;
    zc = count_zero_crossings(signal, 5);
    printf("Positive signal crossings: %d\n", zc); // EXPECT: Positive signal crossings: 0
}

void test_with_zeros(void) {
    int signal[8];
    int zc;

    signal[0] = 5;
    signal[1] = 0;
    signal[2] = 0;
    signal[3] = -5;
    signal[4] = 0;
    signal[5] = 0;
    signal[6] = 0;
    signal[7] = 5;

    zc = count_zero_crossings(signal, 8);
    printf("Signal with zeros crossings: %d\n", zc); // EXPECT: Signal with zeros crossings: 2
}

void test_frequency_estimate(void) {
    int signal[12];
    int freq;

    signal[0] = 10;
    signal[1] = 20;
    signal[2] = 10;
    signal[3] = -10;
    signal[4] = -20;
    signal[5] = -10;
    signal[6] = 10;
    signal[7] = 20;
    signal[8] = 10;
    signal[9] = -10;
    signal[10] = -20;
    signal[11] = -10;

    freq = estimate_frequency_x100(signal, 12, 1000);
    printf("Estimated freq x100: %d\n", freq); // EXPECT: Estimated freq x100: 13636
}

void test_noisy_crossings(void) {
    int signal[10];
    int zc;

    signal[0] = 10;
    signal[1] = 5;
    signal[2] = -2;
    signal[3] = 1;
    signal[4] = -3;
    signal[5] = -8;
    signal[6] = -15;
    signal[7] = -5;
    signal[8] = 3;
    signal[9] = 12;

    zc = count_zero_crossings(signal, 10);
    printf("Noisy signal crossings: %d\n", zc); // EXPECT: Noisy signal crossings: 4
}

int main(void) {
    printf("=== Zero-Crossing Counter ===\n");
    test_alternating();
    test_sine_like();
    test_constant_positive();
    test_with_zeros();
    test_frequency_estimate();
    test_noisy_crossings();
    printf("All zero-crossing tests passed.\n"); // EXPECT: All zero-crossing tests passed.
    return 0;
}
