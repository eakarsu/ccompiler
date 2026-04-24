int printf(const char *fmt, ...);
// EXPECT: multiply_shift tests:\n  3 * 4 = 12\n  7 * 8 = 56\n  12 * 11 = 132\n  0 * 100 = 0\n  1 * 42 = 42\n  -5 * 3 = -15\n  -4 * -6 = 24\n  100 * 100 = 10000\nall match: 1\nconstant multiply (x=13):\n  x*3 = 39 (shift: 39)\n  x*5 = 65 (shift: 65)\n  x*7 = 91 (shift: 91)\n  x*9 = 117 (shift: 117)\n  x*10 = 130 (shift: 130)\n  x*15 = 195 (shift: 195)\n  x*100 = 1300 (shift: 1300)\npower of 2 multiply:\n  7 * 1 = 7\n  7 * 2 = 14\n  7 * 4 = 28\n  7 * 8 = 56\n  7 * 16 = 112\n  7 * 32 = 224
// Test: multiply with shifts

int multiply_shift(int a, int b) {
    int result = 0;
    int multiplicand = a;
    int multiplier = b;
    int neg = 0;

    if (multiplicand < 0) {
        multiplicand = -multiplicand;
        neg = !neg;
    }
    if (multiplier < 0) {
        multiplier = -multiplier;
        neg = !neg;
    }

    while (multiplier > 0) {
        if (multiplier & 1) {
            result = result + multiplicand;
        }
        multiplicand = multiplicand << 1;
        multiplier = multiplier >> 1;
    }

    if (neg) result = -result;
    return result;
}

// Multiply by specific constants using shifts
int mul3(int x) { return (x << 1) + x; }
int mul5(int x) { return (x << 2) + x; }
int mul7(int x) { return (x << 3) - x; }
int mul9(int x) { return (x << 3) + x; }
int mul10(int x) { return (x << 3) + (x << 1); }
int mul15(int x) { return (x << 4) - x; }
int mul100(int x) { return (x << 6) + (x << 5) + (x << 2); }

int main(void) {
    // Basic shift multiplication
    printf("multiply_shift tests:\n");
    printf("  3 * 4 = %d\n", multiply_shift(3, 4));
    printf("  7 * 8 = %d\n", multiply_shift(7, 8));
    printf("  12 * 11 = %d\n", multiply_shift(12, 11));
    printf("  0 * 100 = %d\n", multiply_shift(0, 100));
    printf("  1 * 42 = %d\n", multiply_shift(1, 42));
    printf("  -5 * 3 = %d\n", multiply_shift(-5, 3));
    printf("  -4 * -6 = %d\n", multiply_shift(-4, -6));
    printf("  100 * 100 = %d\n", multiply_shift(100, 100));

    // Compare with regular multiplication
    int i;
    int all_match = 1;
    for (i = 0; i < 20; i++) {
        int a = i * 3 + 1;
        int b = i * 7 + 2;
        if (multiply_shift(a, b) != a * b) {
            all_match = 0;
        }
    }
    printf("all match: %d\n", all_match);

    // Constant multipliers
    int x = 13;
    printf("constant multiply (x=%d):\n", x);
    printf("  x*3 = %d (shift: %d)\n", x * 3, mul3(x));
    printf("  x*5 = %d (shift: %d)\n", x * 5, mul5(x));
    printf("  x*7 = %d (shift: %d)\n", x * 7, mul7(x));
    printf("  x*9 = %d (shift: %d)\n", x * 9, mul9(x));
    printf("  x*10 = %d (shift: %d)\n", x * 10, mul10(x));
    printf("  x*15 = %d (shift: %d)\n", x * 15, mul15(x));
    printf("  x*100 = %d (shift: %d)\n", x * 100, mul100(x));

    // Power of 2 multiplication
    printf("power of 2 multiply:\n");
    int val = 7;
    for (i = 0; i < 6; i++) {
        printf("  %d * %d = %d\n", val, 1 << i, val << i);
    }

    return 0;
}
