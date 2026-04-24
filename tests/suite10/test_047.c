int printf(const char *fmt, ...);

// EXPECT: === Signal Energy Computation ===
// Signal Energy Computation (integer-based)
// Energy = sum of x[n]^2
// Power = Energy / N
// RMS (scaled by 100) = sqrt(Power) * 100

int isqrt(int n) {
    int x;
    int x1;
    if (n <= 0) return 0;
    x = n;
    x1 = (x + 1) / 2;
    while (x1 < x) {
        x = x1;
        x1 = (x + n / x) / 2;
    }
    return x;
}

int compute_energy(int *signal, int len) {
    int energy = 0;
    int i;
    for (i = 0; i < len; i++) {
        energy = energy + signal[i] * signal[i];
    }
    return energy;
}

int compute_power(int *signal, int len) {
    int energy = compute_energy(signal, len);
    return energy / len;
}

int compute_rms_x100(int *signal, int len) {
    int power = compute_power(signal, len);
    return isqrt(power * 10000);
}

int compute_avg_abs(int *signal, int len) {
    int sum = 0;
    int i;
    for (i = 0; i < len; i++) {
        int val = signal[i];
        if (val < 0) val = -val;
        sum = sum + val;
    }
    return sum / len;
}

int signal_snr_x10(int *signal, int *noise, int len) {
    int sig_energy = compute_energy(signal, len);
    int noise_energy = compute_energy(noise, len);
    if (noise_energy == 0) return 9999;
    return (sig_energy * 10) / noise_energy;
}

void test_constant_signal(void) {
    int signal[8];
    int i;
    int energy;
    int power;
    int rms;

    for (i = 0; i < 8; i++) {
        signal[i] = 10;
    }

    energy = compute_energy(signal, 8);
    power = compute_power(signal, 8);
    rms = compute_rms_x100(signal, 8);

    printf("Constant signal energy: %d\n", energy); // EXPECT: Constant signal energy: 800
    printf("Constant signal power: %d\n", power); // EXPECT: Constant signal power: 100
    printf("Constant signal RMS x100: %d\n", rms); // EXPECT: Constant signal RMS x100: 1000
}

void test_alternating_signal(void) {
    int signal[8];
    int i;
    int energy;
    int power;
    int rms;

    for (i = 0; i < 8; i++) {
        if (i % 2 == 0) {
            signal[i] = 50;
        } else {
            signal[i] = -50;
        }
    }

    energy = compute_energy(signal, 8);
    power = compute_power(signal, 8);
    rms = compute_rms_x100(signal, 8);

    printf("Alternating energy: %d\n", energy); // EXPECT: Alternating energy: 20000
    printf("Alternating power: %d\n", power); // EXPECT: Alternating power: 2500
    printf("Alternating RMS x100: %d\n", rms); // EXPECT: Alternating RMS x100: 5000
}

void test_impulse(void) {
    int signal[8];
    int i;
    int energy;
    int power;

    for (i = 0; i < 8; i++) signal[i] = 0;
    signal[3] = 100;

    energy = compute_energy(signal, 8);
    power = compute_power(signal, 8);

    printf("Impulse energy: %d\n", energy); // EXPECT: Impulse energy: 10000
    printf("Impulse power: %d\n", power); // EXPECT: Impulse power: 1250
}

void test_avg_abs(void) {
    int signal[6];
    int avg;

    signal[0] = -10;
    signal[1] = 20;
    signal[2] = -30;
    signal[3] = 40;
    signal[4] = -50;
    signal[5] = 60;

    avg = compute_avg_abs(signal, 6);
    printf("Average absolute value: %d\n", avg); // EXPECT: Average absolute value: 35
}

void test_snr(void) {
    int signal[4];
    int noise[4];
    int snr;

    signal[0] = 100;
    signal[1] = 100;
    signal[2] = 100;
    signal[3] = 100;

    noise[0] = 10;
    noise[1] = -10;
    noise[2] = 10;
    noise[3] = -10;

    snr = signal_snr_x10(signal, noise, 4);
    printf("SNR x10: %d\n", snr); // EXPECT: SNR x10: 1000
}

void test_zero_signal(void) {
    int signal[4];
    int energy;
    int rms;

    signal[0] = 0; signal[1] = 0; signal[2] = 0; signal[3] = 0;

    energy = compute_energy(signal, 4);
    rms = compute_rms_x100(signal, 4);

    printf("Zero energy: %d\n", energy); // EXPECT: Zero energy: 0
    printf("Zero RMS x100: %d\n", rms); // EXPECT: Zero RMS x100: 0
}

void test_isqrt(void) {
    printf("isqrt(0) = %d\n", isqrt(0)); // EXPECT: isqrt(0) = 0
    printf("isqrt(1) = %d\n", isqrt(1)); // EXPECT: isqrt(1) = 1
    printf("isqrt(4) = %d\n", isqrt(4)); // EXPECT: isqrt(4) = 2
    printf("isqrt(100) = %d\n", isqrt(100)); // EXPECT: isqrt(100) = 10
    printf("isqrt(10000) = %d\n", isqrt(10000)); // EXPECT: isqrt(10000) = 100
}

int main(void) {
    printf("=== Signal Energy Computation ===\n");
    test_constant_signal();
    test_alternating_signal();
    test_impulse();
    test_avg_abs();
    test_snr();
    test_zero_signal();
    test_isqrt();
    printf("All energy tests passed.\n"); // EXPECT: All energy tests passed.
    return 0;
}
