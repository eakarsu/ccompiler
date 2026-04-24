int printf(const char *fmt, ...);
// EXPECT: Armstrong numbers up to 10000:\n0 1 2 3 4 5 6 7 8 9 153 370 371 407 1634 8208 9474 \nCount: 17\n\nDetailed breakdown:\n153 = 1^3 + 5^3 + 3^3\n370 = 3^3 + 7^3 + 0^3\n371 = 3^3 + 7^3 + 1^3\n407 = 4^3 + 0^3 + 7^3\n1634 = 1^4 + 6^4 + 3^4 + 4^4\n8208 = 8^4 + 2^4 + 0^4 + 8^4\n9474 = 9^4 + 4^4 + 7^4 + 4^4\n\nPluperfect digital invariants (sum of d^d) up to 10000:\n1 3435 \n\nDudeney numbers (cube root = digit sum):\n1^3 = 1, digit_sum = 1\n8^3 = 512, digit_sum = 8\n17^3 = 4913, digit_sum = 17\n18^3 = 5832, digit_sum = 18\n26^3 = 17576, digit_sum = 26\n27^3 = 19683, digit_sum = 27
// Test: Armstrong/narcissistic numbers

int count_digits(int n) {
    if (n == 0) return 1;
    int count = 0;
    while (n > 0) {
        count++;
        n = n / 10;
    }
    return count;
}

int power(int base, int exp) {
    int result = 1;
    int i;
    for (i = 0; i < exp; i++) {
        result = result * base;
    }
    return result;
}

int is_armstrong(int n) {
    int digits = count_digits(n);
    int sum = 0;
    int temp = n;
    while (temp > 0) {
        int d = temp % 10;
        sum = sum + power(d, digits);
        temp = temp / 10;
    }
    return sum == n;
}

int main(void) {
    int i;

    // Find Armstrong numbers up to 10000
    printf("Armstrong numbers up to 10000:\n");
    int arm_count = 0;
    for (i = 0; i <= 9999; i++) {
        if (is_armstrong(i)) {
            printf("%d ", i);
            arm_count++;
        }
    }
    printf("\nCount: %d\n", arm_count);

    // Show the computation for each Armstrong number
    printf("\nDetailed breakdown:\n");
    for (i = 0; i <= 9999; i++) {
        if (is_armstrong(i) && i >= 10) {
            int digits = count_digits(i);
            printf("%d = ", i);
            int temp = i;
            int first = 1;
            // Extract digits in forward order
            int darr[5];
            int nd = 0;
            int t = i;
            while (t > 0) {
                darr[nd] = t % 10;
                nd++;
                t = t / 10;
            }
            int j;
            for (j = nd - 1; j >= 0; j--) {
                if (!first) printf(" + ");
                printf("%d^%d", darr[j], digits);
                first = 0;
            }
            printf("\n");
        }
    }

    // Pluperfect digital invariant: sum of d^d
    printf("\nPluperfect digital invariants (sum of d^d) up to 10000:\n");
    for (i = 1; i <= 10000; i++) {
        int sum = 0;
        int temp = i;
        while (temp > 0) {
            int d = temp % 10;
            if (d > 0) {
                sum = sum + power(d, d);
            }
            temp = temp / 10;
        }
        if (sum == i) {
            printf("%d ", i);
        }
    }
    printf("\n");

    // Dudeney numbers: perfect cube where digit sum = cube root
    printf("\nDudeney numbers (cube root = digit sum):\n");
    for (i = 1; i <= 100; i++) {
        int cube = i * i * i;
        int ds = 0;
        int temp = cube;
        while (temp > 0) {
            ds = ds + temp % 10;
            temp = temp / 10;
        }
        if (ds == i) {
            printf("%d^3 = %d, digit_sum = %d\n", i, cube, ds);
        }
    }

    return 0;
}
