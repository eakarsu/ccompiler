int printf(const char *fmt, ...);

int factorial_rec(int n) {
    if (n <= 1) return 1;
    return n * factorial_rec(n - 1);
}

int factorial_iter(int n) {
    int result = 1;
    int i = 2;
    while (i <= n) {
        result = result * i;
        i = i + 1;
    }
    return result;
}

int fib_rec(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fib_rec(n - 1) + fib_rec(n - 2);
}

int fib_iter(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    int a = 0;
    int b = 1;
    int i = 2;
    while (i <= n) {
        int tmp = a + b;
        a = b;
        b = tmp;
        i = i + 1;
    }
    return b;
}

int power_rec(int base, int exp) {
    if (exp == 0) return 1;
    return base * power_rec(base, exp - 1);
}

int power_iter(int base, int exp) {
    int result = 1;
    int i = 0;
    while (i < exp) {
        result = result * base;
        i = i + 1;
    }
    return result;
}

int gcd_rec(int a, int b) {
    if (b == 0) return a;
    return gcd_rec(b, a - (a / b) * b);
}

int gcd_iter(int a, int b) {
    while (b != 0) {
        int t = a - (a / b) * b;
        a = b;
        b = t;
    }
    return a;
}

int sum_rec(int n) {
    if (n <= 0) return 0;
    return n + sum_rec(n - 1);
}

int sum_iter(int n) {
    int total = 0;
    int i = 1;
    while (i <= n) {
        total = total + i;
        i = i + 1;
    }
    return total;
}

int digit_sum_rec(int n) {
    if (n < 0) n = -n;
    if (n < 10) return n;
    return (n - (n / 10) * 10) + digit_sum_rec(n / 10);
}

int digit_sum_iter(int n) {
    if (n < 0) n = -n;
    int total = 0;
    while (n > 0) {
        total = total + (n - (n / 10) * 10);
        n = n / 10;
    }
    return total;
}

int count_digits_rec(int n) {
    if (n < 0) n = -n;
    if (n < 10) return 1;
    return 1 + count_digits_rec(n / 10);
}

int count_digits_iter(int n) {
    if (n < 0) n = -n;
    if (n == 0) return 1;
    int cnt = 0;
    while (n > 0) {
        cnt = cnt + 1;
        n = n / 10;
    }
    return cnt;
}

int reverse_num_rec_helper(int n, int acc) {
    if (n == 0) return acc;
    return reverse_num_rec_helper(n / 10, acc * 10 + (n - (n / 10) * 10));
}

int reverse_num_rec(int n) {
    return reverse_num_rec_helper(n, 0);
}

int reverse_num_iter(int n) {
    int result = 0;
    while (n > 0) {
        result = result * 10 + (n - (n / 10) * 10);
        n = n / 10;
    }
    return result;
}

int main(void) {
    // EXPECT: fact_rec: 120
    printf("fact_rec: %d\n", factorial_rec(5));

    // EXPECT: fact_iter: 120
    printf("fact_iter: %d\n", factorial_iter(5));

    // EXPECT: fact_match: 1
    printf("fact_match: %d\n", factorial_rec(7) == factorial_iter(7));

    // EXPECT: fib_rec10: 55
    printf("fib_rec10: %d\n", fib_rec(10));

    // EXPECT: fib_iter10: 55
    printf("fib_iter10: %d\n", fib_iter(10));

    // EXPECT: fib_match: 1
    printf("fib_match: %d\n", fib_rec(12) == fib_iter(12));

    // EXPECT: pow_rec: 1024
    printf("pow_rec: %d\n", power_rec(2, 10));

    // EXPECT: pow_iter: 1024
    printf("pow_iter: %d\n", power_iter(2, 10));

    // EXPECT: pow_match: 1
    printf("pow_match: %d\n", power_rec(3, 5) == power_iter(3, 5));

    // EXPECT: gcd_rec: 6
    printf("gcd_rec: %d\n", gcd_rec(48, 18));

    // EXPECT: gcd_iter: 6
    printf("gcd_iter: %d\n", gcd_iter(48, 18));

    // EXPECT: gcd_match: 1
    printf("gcd_match: %d\n", gcd_rec(100, 75) == gcd_iter(100, 75));

    // EXPECT: sum_rec: 55
    printf("sum_rec: %d\n", sum_rec(10));

    // EXPECT: sum_iter: 55
    printf("sum_iter: %d\n", sum_iter(10));

    // EXPECT: dsum_rec: 15
    printf("dsum_rec: %d\n", digit_sum_rec(12345));

    // EXPECT: dsum_iter: 15
    printf("dsum_iter: %d\n", digit_sum_iter(12345));

    // EXPECT: digits_rec: 5
    printf("digits_rec: %d\n", count_digits_rec(12345));

    // EXPECT: digits_iter: 5
    printf("digits_iter: %d\n", count_digits_iter(12345));

    // EXPECT: rev_rec: 54321
    printf("rev_rec: %d\n", reverse_num_rec(12345));

    // EXPECT: rev_iter: 54321
    printf("rev_iter: %d\n", reverse_num_iter(12345));

    // EXPECT: rev_match: 1
    printf("rev_match: %d\n", reverse_num_rec(9876) == reverse_num_iter(9876));

    return 0;
}
