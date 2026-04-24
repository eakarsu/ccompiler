int printf(const char *fmt, ...);
// EXPECT: Amicable pairs up to 10000:\n(220, 284)\n(1184, 1210)\n(2620, 2924)\n(5020, 5564)\n(6232, 6368)\nTotal pairs: 5\n\nVerification of (220, 284):\nDivisors of 220: 1 2 4 5 10 11 20 22 44 55 110 \nSum = 284\nDivisors of 284: 1 2 4 71 142 \nSum = 220\n\nAliquot chain classification:\n6 -> 6 (perfect)\n28 -> 28 (perfect)\n220 -> 284 220 (amicable)\n12 -> 16 15 9 4 3 1 \n25 -> 6 6 6 6 6 6 6 \n95 -> 25 6 6 6 6 6 6 \n496 -> 496 (perfect)\n1184 -> 1210 1184 (amicable)
// Test: Amicable numbers

int sum_divisors(int n) {
    int sum = 0;
    int i;
    if (n <= 1) return 0;
    sum = 1;
    for (i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            sum = sum + i;
            if (i != n / i) {
                sum = sum + n / i;
            }
        }
    }
    return sum;
}

int main(void) {
    int i;

    // Find amicable pairs up to 10000
    // Amicable: s(a) = b and s(b) = a, where a != b
    printf("Amicable pairs up to 10000:\n");
    int pair_count = 0;
    for (i = 2; i <= 10000; i++) {
        int s = sum_divisors(i);
        if (s > i && s <= 10000) {
            int ss = sum_divisors(s);
            if (ss == i) {
                printf("(%d, %d)\n", i, s);
                pair_count++;
            }
        }
    }
    printf("Total pairs: %d\n", pair_count);

    // Verify the classic pair (220, 284)
    printf("\nVerification of (220, 284):\n");
    printf("Divisors of 220: ");
    int sum220 = 0;
    for (i = 1; i < 220; i++) {
        if (220 % i == 0) {
            printf("%d ", i);
            sum220 = sum220 + i;
        }
    }
    printf("\nSum = %d\n", sum220);

    printf("Divisors of 284: ");
    int sum284 = 0;
    for (i = 1; i < 284; i++) {
        if (284 % i == 0) {
            printf("%d ", i);
            sum284 = sum284 + i;
        }
    }
    printf("\nSum = %d\n", sum284);

    // Social chains: iterate s(n) and see if we return to n
    printf("\nAliquot chain classification:\n");
    int test_nums[8];
    test_nums[0] = 6;
    test_nums[1] = 28;
    test_nums[2] = 220;
    test_nums[3] = 12;
    test_nums[4] = 25;
    test_nums[5] = 95;
    test_nums[6] = 496;
    test_nums[7] = 1184;

    for (i = 0; i < 8; i++) {
        int n = test_nums[i];
        int chain[20];
        int len = 0;
        chain[0] = n;
        int current = n;
        int j;
        for (j = 0; j < 15; j++) {
            current = sum_divisors(current);
            if (current == 0 || current > 100000) break;
            len++;
            if (len < 20) chain[len] = current;
            if (current == n) break;
        }
        printf("%d -> ", n);
        for (j = 1; j <= len && j < 8; j++) {
            printf("%d ", chain[j]);
        }
        if (current == n) {
            if (len == 1) printf("(perfect)");
            else if (len == 2) printf("(amicable)");
            else printf("(sociable, period %d)", len);
        }
        printf("\n");
    }

    return 0;
}
