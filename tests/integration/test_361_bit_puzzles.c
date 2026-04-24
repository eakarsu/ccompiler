int printf(const char *fmt, ...);
// EXPECT: turn_off_rightmost:\n  12 (1100) -> 8\n  10 (1010) -> 8\n  8  (1000) -> 0\n  7  (0111) -> 6\n  1  (0001) -> 0\nisolate_rightmost:\n  12 (1100) -> 4\n  10 (1010) -> 2\n  8  (1000) -> 8\n  7  (0111) -> 1\npropagate_rightmost:\n  12 (1100) -> 15\n  10 (1010) -> 11\n  80 -> 95\nexactly_one_bit:\n  0: 0\n  1: 1\n  2: 1\n  3: 0\n  4: 1\n  7: 0\n  8: 1\n  16: 1\nturn_on_rightmost_zero:\n  6 (0110) -> 7\n  5 (0101) -> 7\n  3 (0011) -> 7\n  0 (0000) -> 1\nodd/even:\n  0 is even\n  1 is odd\n  2 is even\n  3 is odd\n  4 is even\n  7 is odd\n  8 is even\n  16 is even\n  trailing zeros of 1: 0\n  trailing zeros of 4: 2\n  trailing zeros of 12: 2\n  trailing zeros of 16: 4\n  trailing zeros of 96: 5
// Test: bit manipulation puzzles

// Turn off the rightmost 1-bit
int turn_off_rightmost(int n) {
    return n & (n - 1);
}

// Isolate the rightmost 1-bit
int isolate_rightmost(int n) {
    return n & (-n);
}

// Right propagate the rightmost 1-bit
int propagate_rightmost(int n) {
    return n | (n - 1);
}

// Check if n has exactly one bit set
int exactly_one_bit(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

// Turn on the rightmost 0-bit
int turn_on_rightmost_zero(int n) {
    return n | (n + 1);
}

int main(void) {
    // Turn off rightmost 1-bit
    printf("turn_off_rightmost:\n");
    printf("  12 (1100) -> %d\n", turn_off_rightmost(12));
    printf("  10 (1010) -> %d\n", turn_off_rightmost(10));
    printf("  8  (1000) -> %d\n", turn_off_rightmost(8));
    printf("  7  (0111) -> %d\n", turn_off_rightmost(7));
    printf("  1  (0001) -> %d\n", turn_off_rightmost(1));

    // Isolate rightmost 1-bit
    printf("isolate_rightmost:\n");
    printf("  12 (1100) -> %d\n", isolate_rightmost(12));
    printf("  10 (1010) -> %d\n", isolate_rightmost(10));
    printf("  8  (1000) -> %d\n", isolate_rightmost(8));
    printf("  7  (0111) -> %d\n", isolate_rightmost(7));

    // Propagate rightmost 1-bit
    printf("propagate_rightmost:\n");
    printf("  12 (1100) -> %d\n", propagate_rightmost(12));
    printf("  10 (1010) -> %d\n", propagate_rightmost(10));
    printf("  80 -> %d\n", propagate_rightmost(80));

    // Exactly one bit
    printf("exactly_one_bit:\n");
    int vals[8];
    vals[0] = 0; vals[1] = 1; vals[2] = 2; vals[3] = 3;
    vals[4] = 4; vals[5] = 7; vals[6] = 8; vals[7] = 16;
    int i;
    for (i = 0; i < 8; i++) {
        printf("  %d: %d\n", vals[i], exactly_one_bit(vals[i]));
    }

    // Turn on rightmost 0-bit
    printf("turn_on_rightmost_zero:\n");
    printf("  6 (0110) -> %d\n", turn_on_rightmost_zero(6));
    printf("  5 (0101) -> %d\n", turn_on_rightmost_zero(5));
    printf("  3 (0011) -> %d\n", turn_on_rightmost_zero(3));
    printf("  0 (0000) -> %d\n", turn_on_rightmost_zero(0));

    // Determine if integer is odd/even using bit test
    printf("odd/even:\n");
    for (i = 0; i < 8; i++) {
        int odd = vals[i] & 1;
        printf("  %d is %s\n", vals[i], odd ? "odd" : "even");
    }

    // Count trailing zeros
    int tz_vals[5];
    tz_vals[0] = 1;   // 0 trailing zeros
    tz_vals[1] = 4;   // 2 trailing zeros
    tz_vals[2] = 12;  // 2 trailing zeros
    tz_vals[3] = 16;  // 4 trailing zeros
    tz_vals[4] = 96;  // 5 trailing zeros
    for (i = 0; i < 5; i++) {
        int n = tz_vals[i];
        int isolated = n & (-n);
        int tz = 0;
        int tmp = isolated;
        while (tmp > 1) {
            tmp = tmp >> 1;
            tz++;
        }
        printf("  trailing zeros of %d: %d\n", n, tz);
    }

    return 0;
}
