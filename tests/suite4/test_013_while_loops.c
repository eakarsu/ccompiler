int printf(const char *fmt, ...);

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int digit_sum(int n) {
    if (n < 0) n = -n;
    int s = 0;
    while (n > 0) {
        s = s + n % 10;
        n = n / 10;
    }
    return s;
}

int count_digits(int n) {
    if (n == 0) return 1;
    if (n < 0) n = -n;
    int c = 0;
    while (n > 0) {
        c = c + 1;
        n = n / 10;
    }
    return c;
}

int reverse_number(int n) {
    int rev = 0;
    while (n > 0) {
        rev = rev * 10 + n % 10;
        n = n / 10;
    }
    return rev;
}

int is_power_of_two(int n) {
    if (n <= 0) return 0;
    while (n > 1) {
        if (n % 2 != 0) return 0;
        n = n / 2;
    }
    return 1;
}

int main(void) {
    // Test 1: simple while countdown
    int i = 10;
    int count = 0;
    while (i > 0) {
        count = count + 1;
        i = i - 1;
    }
    // EXPECT: countdown: 10
    printf("countdown: %d\n", count);

    // Test 2: while summing
    int sum = 0;
    i = 1;
    while (i <= 100) {
        sum = sum + i;
        i = i + 1;
    }
    // EXPECT: sum_100: 5050
    printf("sum_100: %d\n", sum);

    // Test 3: while that never executes
    int ran = 0;
    i = 0;
    while (i > 10) {
        ran = 1;
        i = i - 1;
    }
    // EXPECT: never_ran: 0
    printf("never_ran: %d\n", ran);

    // Test 4: while with break
    i = 0;
    while (1) {
        if (i >= 5) break;
        i = i + 1;
    }
    // EXPECT: while_break: 5
    printf("while_break: %d\n", i);

    // Test 5: while with continue
    int skip_sum = 0;
    i = 0;
    while (i < 10) {
        i = i + 1;
        if (i % 2 == 0) continue;
        skip_sum = skip_sum + i;
    }
    // odd numbers 1+3+5+7+9 = 25
    // EXPECT: skip_sum: 25
    printf("skip_sum: %d\n", skip_sum);

    // Test 6: nested while
    int product = 0;
    i = 1;
    while (i <= 3) {
        int j = 1;
        while (j <= 4) {
            product = product + i * j;
            j = j + 1;
        }
    // sum: i*(1+2+3+4) for i=1..3 = (1+2+3)*10 = 60
        i = i + 1;
    }
    // EXPECT: nested_while: 60
    printf("nested_while: %d\n", product);

    // Test 7: gcd function
    // EXPECT: gcd_12_8: 4
    printf("gcd_12_8: %d\n", gcd(12, 8));
    // EXPECT: gcd_35_14: 7
    printf("gcd_35_14: %d\n", gcd(35, 14));
    // EXPECT: gcd_17_13: 1
    printf("gcd_17_13: %d\n", gcd(17, 13));

    // Test 8: digit_sum
    // EXPECT: dsum_123: 6
    printf("dsum_123: %d\n", digit_sum(123));
    // EXPECT: dsum_9999: 36
    printf("dsum_9999: %d\n", digit_sum(9999));

    // Test 9: count_digits
    // EXPECT: digits_0: 1
    printf("digits_0: %d\n", count_digits(0));
    // EXPECT: digits_42: 2
    printf("digits_42: %d\n", count_digits(42));
    // EXPECT: digits_12345: 5
    printf("digits_12345: %d\n", count_digits(12345));

    // Test 10: reverse_number
    // EXPECT: rev_1234: 4321
    printf("rev_1234: %d\n", reverse_number(1234));
    // EXPECT: rev_100: 1
    printf("rev_100: %d\n", reverse_number(100));

    // Test 11: is_power_of_two
    // EXPECT: pow2_16: 1
    printf("pow2_16: %d\n", is_power_of_two(16));
    // EXPECT: pow2_15: 0
    printf("pow2_15: %d\n", is_power_of_two(15));
    // EXPECT: pow2_1: 1
    printf("pow2_1: %d\n", is_power_of_two(1));

    // Test 12: while collatz sequence length
    int n = 27;
    int steps = 0;
    while (n != 1) {
        if (n % 2 == 0)
            n = n / 2;
        else
            n = 3 * n + 1;
        steps = steps + 1;
    }
    // Collatz(27) takes 111 steps
    // EXPECT: collatz_27: 111
    printf("collatz_27: %d\n", steps);

    // Test 13: while finding first multiple
    i = 1;
    while (i % 7 != 0 || i % 5 != 0) {
        i = i + 1;
    }
    // LCM(5,7) = 35
    // EXPECT: first_mult: 35
    printf("first_mult: %d\n", i);

    // Test 14: while with compound condition
    int a = 100;
    int b = 1;
    while (a > b) {
        a = a - 1;
        b = b + 1;
    }
    // They meet at 50 and 51, then a=50, b=51, loop ends
    // EXPECT: meet_a: 50
    printf("meet_a: %d\n", a);
    // EXPECT: meet_b: 51
    printf("meet_b: %d\n", b);

    return 0;
}
