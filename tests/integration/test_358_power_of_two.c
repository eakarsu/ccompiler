int printf(const char *fmt, ...);
// EXPECT: Power of 2 checks:\n  0: 0\n  1: 1\n  2: 1\n  3: 0\n  4: 1\n  5: 0\n  8: 1\n  15: 0\n  16: 1\n  64: 1\n  100: 0\n  128: 1\nNext power of 2:\n  1 -> 1\n  3 -> 4\n  5 -> 8\n  9 -> 16\n  17 -> 32\n  100 -> 128\nPrev power of 2:\n  1 -> 1\n  3 -> 2\n  5 -> 4\n  9 -> 8\n  17 -> 16\n  100 -> 64\nLog2 floor:\n  log2(1) = 0\n  log2(2) = 1\n  log2(4) = 2\n  log2(8) = 3\n  log2(10) = 3\n  log2(100) = 6\n  log2(1024) = 10\nAlign to 8:\n  1 -> 8\n  2 -> 8\n  3 -> 8\n  4 -> 8\n  5 -> 8\n  6 -> 8\n  7 -> 8\n  8 -> 8\n  9 -> 16\n  10 -> 16\n  11 -> 16\n  12 -> 16\n  13 -> 16\n  14 -> 16\n  15 -> 16\n  16 -> 16
// Test: power of two detection

int is_power_of_2(int n) {
    if (n <= 0) return 0;
    return (n & (n - 1)) == 0;
}

int next_power_of_2(int n) {
    if (n <= 1) return 1;
    int p = 1;
    while (p < n) {
        p = p << 1;
    }
    return p;
}

int prev_power_of_2(int n) {
    if (n <= 0) return 0;
    int p = 1;
    while (p <= n) {
        p = p << 1;
    }
    return p >> 1;
}

int log2_floor(int n) {
    if (n <= 0) return -1;
    int result = 0;
    int val = n;
    while (val > 1) {
        val = val >> 1;
        result++;
    }
    return result;
}

int main(void) {
    // Test power of 2 detection
    int i;
    printf("Power of 2 checks:\n");
    int test_vals[12];
    test_vals[0] = 0;
    test_vals[1] = 1;
    test_vals[2] = 2;
    test_vals[3] = 3;
    test_vals[4] = 4;
    test_vals[5] = 5;
    test_vals[6] = 8;
    test_vals[7] = 15;
    test_vals[8] = 16;
    test_vals[9] = 64;
    test_vals[10] = 100;
    test_vals[11] = 128;

    for (i = 0; i < 12; i++) {
        printf("  %d: %d\n", test_vals[i], is_power_of_2(test_vals[i]));
    }

    // Next power of 2
    printf("Next power of 2:\n");
    int next_vals[6];
    next_vals[0] = 1;
    next_vals[1] = 3;
    next_vals[2] = 5;
    next_vals[3] = 9;
    next_vals[4] = 17;
    next_vals[5] = 100;
    for (i = 0; i < 6; i++) {
        printf("  %d -> %d\n", next_vals[i], next_power_of_2(next_vals[i]));
    }

    // Previous power of 2
    printf("Prev power of 2:\n");
    for (i = 0; i < 6; i++) {
        printf("  %d -> %d\n", next_vals[i], prev_power_of_2(next_vals[i]));
    }

    // Log2 floor
    printf("Log2 floor:\n");
    int log_vals[7];
    log_vals[0] = 1;
    log_vals[1] = 2;
    log_vals[2] = 4;
    log_vals[3] = 8;
    log_vals[4] = 10;
    log_vals[5] = 100;
    log_vals[6] = 1024;
    for (i = 0; i < 7; i++) {
        printf("  log2(%d) = %d\n", log_vals[i], log2_floor(log_vals[i]));
    }

    // Alignment to power of 2
    printf("Align to 8:\n");
    for (i = 1; i <= 16; i++) {
        int aligned = (i + 7) & ~7;
        printf("  %d -> %d\n", i, aligned);
    }

    return 0;
}
