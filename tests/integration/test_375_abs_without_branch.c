int printf(const char *fmt, ...);
// EXPECT: branchless_abs:\n  abs(0) = 0\n  abs(1) = 1\n  abs(-1) = 1\n  abs(42) = 42\n  abs(-42) = 42\n  abs(100) = 100\n  abs(-100) = 100\nbranchless_max:\n  max(3, 5) = 5\n  max(5, 3) = 5\n  max(-1, 1) = 1\n  max(7, 7) = 7\n  max(0, -5) = 0\nbranchless_min:\n  min(3, 5) = 3\n  min(5, 3) = 3\n  min(-1, 1) = -1\n  min(7, 7) = 7\nbranchless_clamp [0,100]:\n  clamp(-50) = 0\n  clamp(0) = 0\n  clamp(50) = 50\n  clamp(100) = 100\n  clamp(150) = 100\n  clamp(-1) = 0\n  clamp(101) = 100\nbranchless_sign:\n  sign(-10) = -1\n  sign(0) = 0\n  sign(10) = 1\nabs all correct: 1
// Test: abs without branch

int branchless_abs(int x) {
    int mask = x >> 31;  // all 1s if negative, all 0s if positive
    return (x + mask) ^ mask;
}

int branchless_max(int a, int b) {
    int diff = a - b;
    int mask = diff >> 31;  // 0 if a >= b, -1 if a < b
    return a - (diff & mask);
}

int branchless_min(int a, int b) {
    int diff = a - b;
    int mask = diff >> 31;
    return b + (diff & mask);
}

int branchless_clamp(int val, int lo, int hi) {
    val = branchless_max(val, lo);
    val = branchless_min(val, hi);
    return val;
}

int branchless_sign(int x) {
    // Returns -1, 0, or 1
    return (x > 0) - (x < 0);
}

int main(void) {
    // Branchless abs
    printf("branchless_abs:\n");
    printf("  abs(0) = %d\n", branchless_abs(0));
    printf("  abs(1) = %d\n", branchless_abs(1));
    printf("  abs(-1) = %d\n", branchless_abs(-1));
    printf("  abs(42) = %d\n", branchless_abs(42));
    printf("  abs(-42) = %d\n", branchless_abs(-42));
    printf("  abs(100) = %d\n", branchless_abs(100));
    printf("  abs(-100) = %d\n", branchless_abs(-100));

    // Branchless max
    printf("branchless_max:\n");
    printf("  max(3, 5) = %d\n", branchless_max(3, 5));
    printf("  max(5, 3) = %d\n", branchless_max(5, 3));
    printf("  max(-1, 1) = %d\n", branchless_max(-1, 1));
    printf("  max(7, 7) = %d\n", branchless_max(7, 7));
    printf("  max(0, -5) = %d\n", branchless_max(0, -5));

    // Branchless min
    printf("branchless_min:\n");
    printf("  min(3, 5) = %d\n", branchless_min(3, 5));
    printf("  min(5, 3) = %d\n", branchless_min(5, 3));
    printf("  min(-1, 1) = %d\n", branchless_min(-1, 1));
    printf("  min(7, 7) = %d\n", branchless_min(7, 7));

    // Branchless clamp
    printf("branchless_clamp [0,100]:\n");
    int test_vals[7];
    test_vals[0] = -50;
    test_vals[1] = 0;
    test_vals[2] = 50;
    test_vals[3] = 100;
    test_vals[4] = 150;
    test_vals[5] = -1;
    test_vals[6] = 101;
    int i;
    for (i = 0; i < 7; i++) {
        printf("  clamp(%d) = %d\n", test_vals[i],
               branchless_clamp(test_vals[i], 0, 100));
    }

    // Branchless sign
    printf("branchless_sign:\n");
    printf("  sign(-10) = %d\n", branchless_sign(-10));
    printf("  sign(0) = %d\n", branchless_sign(0));
    printf("  sign(10) = %d\n", branchless_sign(10));

    // Verify abs correctness
    int all_correct = 1;
    for (i = -50; i <= 50; i++) {
        int expected = i < 0 ? -i : i;
        if (branchless_abs(i) != expected) all_correct = 0;
    }
    printf("abs all correct: %d\n", all_correct);

    return 0;
}
