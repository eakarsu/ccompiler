int printf(const char *fmt, ...);
// EXPECT: 2^n: 1 2 4 8 16 32 64 128 256 512 1024 \n3^n: 1 3 9 27 81 243 729 2187 \nfast match: OK\nsqrt(0) = 0\nsqrt(1) = 1\nsqrt(4) = 2\nsqrt(9) = 3\nsqrt(16) = 4\nsqrt(100) = 10\nsqrt(50) = 7\nsum i^1 (1..5) = 15\nsum i^2 (1..5) = 55\nsum i^3 (1..5) = 225\nperfect squares: 1 4 9 16 25 
// Test: power functions and exponentiation

int power(int base, int exp) {
    int result, i;
    result = 1;
    for (i = 0; i < exp; i++) {
        result = result * base;
    }
    return result;
}

int fast_power(int base, int exp) {
    int result;
    result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = result * base;
        }
        base = base * base;
        exp = exp / 2;
    }
    return result;
}

int int_sqrt(int n) {
    int low, high, mid, sq;
    low = 0;
    high = n;
    if (high > 46340) high = 46340;
    while (low <= high) {
        mid = (low + high) / 2;
        sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

int main(void) {
    int i, j;
    int r;

    /* Powers of 2 */
    printf("2^n: ");
    for (i = 0; i <= 10; i++) {
        printf("%d ", power(2, i));
    }
    printf("\n");

    /* Powers of 3 */
    printf("3^n: ");
    for (i = 0; i <= 7; i++) {
        printf("%d ", power(3, i));
    }
    printf("\n");

    /* Verify fast_power matches power */
    printf("fast match: ");
    for (i = 2; i <= 5; i++) {
        for (j = 0; j <= 5; j++) {
            if (power(i, j) != fast_power(i, j)) {
                printf("FAIL ");
            }
        }
    }
    printf("OK\n");

    /* Integer square root */
    printf("sqrt(0) = %d\n", int_sqrt(0));
    printf("sqrt(1) = %d\n", int_sqrt(1));
    printf("sqrt(4) = %d\n", int_sqrt(4));
    printf("sqrt(9) = %d\n", int_sqrt(9));
    printf("sqrt(16) = %d\n", int_sqrt(16));
    printf("sqrt(100) = %d\n", int_sqrt(100));
    printf("sqrt(50) = %d\n", int_sqrt(50));

    /* Sum of powers: 1^k + 2^k + ... + n^k */
    for (j = 1; j <= 3; j++) {
        r = 0;
        for (i = 1; i <= 5; i++) {
            r += power(i, j);
        }
        printf("sum i^%d (1..5) = %d\n", j, r);
    }

    /* Check perfect squares */
    printf("perfect squares: ");
    for (i = 1; i <= 25; i++) {
        r = int_sqrt(i);
        if (r * r == i) {
            printf("%d ", i);
        }
    }
    printf("\n");

    return 0;
}
