int printf(const char *fmt, ...);
// EXPECT: divide_shift tests:\n  100 / 10 = 10\n  255 / 16 = 15\n  42 / 7 = 6\n  100 / 3 = 33\n  1 / 1 = 1\n  0 / 5 = 0\n  -15 / 3 = -5\n  15 / -3 = -5\nremainder tests:\n  100 % 10 = 0\n  100 % 3 = 1\n  255 % 16 = 15\n  42 % 5 = 2\nall match: 1\npower of 2 division:\n  1000 / 2 = 500\n  1000 / 4 = 250\n  1000 / 8 = 125\n  1000 / 16 = 62\n  1000 / 32 = 31\nsum=150 avg=30\nsqrt(144) = 12
// Test: divide with shifts

int divide_shift(int dividend, int divisor) {
    if (divisor == 0) return -1;  // error
    int neg = 0;
    int a = dividend;
    int b = divisor;
    if (a < 0) { a = -a; neg = !neg; }
    if (b < 0) { b = -b; neg = !neg; }

    int quotient = 0;
    int i;
    for (i = 30; i >= 0; i--) {
        if ((a >> i) >= b) {
            quotient = quotient | (1 << i);
            a = a - (b << i);
        }
    }

    if (neg) quotient = -quotient;
    return quotient;
}

int remainder_shift(int dividend, int divisor) {
    int q = divide_shift(dividend, divisor);
    return dividend - q * divisor;
}

int main(void) {
    // Basic division
    printf("divide_shift tests:\n");
    printf("  100 / 10 = %d\n", divide_shift(100, 10));
    printf("  255 / 16 = %d\n", divide_shift(255, 16));
    printf("  42 / 7 = %d\n", divide_shift(42, 7));
    printf("  100 / 3 = %d\n", divide_shift(100, 3));
    printf("  1 / 1 = %d\n", divide_shift(1, 1));
    printf("  0 / 5 = %d\n", divide_shift(0, 5));
    printf("  -15 / 3 = %d\n", divide_shift(-15, 3));
    printf("  15 / -3 = %d\n", divide_shift(15, -3));

    // Remainder
    printf("remainder tests:\n");
    printf("  100 %% 10 = %d\n", remainder_shift(100, 10));
    printf("  100 %% 3 = %d\n", remainder_shift(100, 3));
    printf("  255 %% 16 = %d\n", remainder_shift(255, 16));
    printf("  42 %% 5 = %d\n", remainder_shift(42, 5));

    // Compare with regular division
    int i;
    int all_match = 1;
    for (i = 1; i <= 20; i++) {
        int a = i * 17 + 3;
        int b = i + 1;
        if (divide_shift(a, b) != a / b) {
            all_match = 0;
        }
    }
    printf("all match: %d\n", all_match);

    // Power of 2 division (fast path with shift)
    printf("power of 2 division:\n");
    int val = 1000;
    printf("  %d / 2 = %d\n", val, val >> 1);
    printf("  %d / 4 = %d\n", val, val >> 2);
    printf("  %d / 8 = %d\n", val, val >> 3);
    printf("  %d / 16 = %d\n", val, val >> 4);
    printf("  %d / 32 = %d\n", val, val >> 5);

    // Division for average
    int nums[5];
    nums[0] = 10; nums[1] = 20; nums[2] = 30; nums[3] = 40; nums[4] = 50;
    int sum = 0;
    for (i = 0; i < 5; i++) sum = sum + nums[i];
    printf("sum=%d avg=%d\n", sum, divide_shift(sum, 5));

    // Integer square root via binary search with shifts
    int n = 144;
    int lo = 0;
    int hi = n;
    while (lo <= hi) {
        int mid = (lo + hi) >> 1;
        int sq = mid * mid;
        if (sq == n) {
            printf("sqrt(%d) = %d\n", n, mid);
            break;
        }
        if (sq < n) lo = mid + 1;
        else hi = mid - 1;
    }

    return 0;
}
