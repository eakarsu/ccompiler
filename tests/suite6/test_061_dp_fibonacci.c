int printf(const char *fmt, ...);

int fib_recursive(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fib_recursive(n - 1) + fib_recursive(n - 2);
}

int memo[50];

void init_memo(void) {
    int i = 0;
    while (i < 50) {
        memo[i] = -1;
        i = i + 1;
    }
}

int fib_memoized(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    if (memo[n] != -1) return memo[n];
    memo[n] = fib_memoized(n - 1) + fib_memoized(n - 2);
    return memo[n];
}

int fib_bottom_up(int n) {
    int dp[50];
    dp[0] = 0;
    dp[1] = 1;
    int i = 2;
    while (i <= n) {
        dp[i] = dp[i - 1] + dp[i - 2];
        i = i + 1;
    }
    return dp[n];
}

int fib_space_opt(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    int prev2 = 0;
    int prev1 = 1;
    int i = 2;
    int cur = 0;
    while (i <= n) {
        cur = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
        i = i + 1;
    }
    return prev1;
}

int fib_sum(int n) {
    int total = 0;
    int i = 0;
    while (i <= n) {
        total = total + fib_bottom_up(i);
        i = i + 1;
    }
    return total;
}

int fib_is_even(int n) {
    return fib_bottom_up(n) % 2 == 0;
}

int tribonacci(int n) {
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 1;
    int dp[50];
    dp[0] = 0;
    dp[1] = 0;
    dp[2] = 1;
    int i = 3;
    while (i <= n) {
        dp[i] = dp[i - 1] + dp[i - 2] + dp[i - 3];
        i = i + 1;
    }
    return dp[n];
}

int count_fib_digits(int n) {
    int val = fib_bottom_up(n);
    if (val == 0) return 1;
    int count = 0;
    int v = val;
    if (v < 0) v = -v;
    while (v > 0) {
        count = count + 1;
        v = v / 10;
    }
    return count;
}

int main(void) {
    // EXPECT: fib(0) = 0
    printf("fib(0) = %d\n", fib_recursive(0));
    // EXPECT: fib(1) = 1
    printf("fib(1) = %d\n", fib_recursive(1));
    // EXPECT: fib(5) = 5
    printf("fib(5) = %d\n", fib_recursive(5));
    // EXPECT: fib(10) = 55
    printf("fib(10) = %d\n", fib_recursive(10));

    init_memo();
    // EXPECT: memo_fib(15) = 610
    printf("memo_fib(15) = %d\n", fib_memoized(15));
    // EXPECT: memo_fib(20) = 6765
    printf("memo_fib(20) = %d\n", fib_memoized(20));
    // EXPECT: memo_fib(25) = 75025
    printf("memo_fib(25) = %d\n", fib_memoized(25));

    // EXPECT: bu_fib(30) = 832040
    printf("bu_fib(30) = %d\n", fib_bottom_up(30));
    // EXPECT: bu_fib(0) = 0
    printf("bu_fib(0) = %d\n", fib_bottom_up(0));
    // EXPECT: bu_fib(1) = 1
    printf("bu_fib(1) = %d\n", fib_bottom_up(1));

    // EXPECT: so_fib(30) = 832040
    printf("so_fib(30) = %d\n", fib_space_opt(30));
    // EXPECT: so_fib(12) = 144
    printf("so_fib(12) = %d\n", fib_space_opt(12));

    // EXPECT: fib_sum(6) = 20
    printf("fib_sum(6) = %d\n", fib_sum(6));
    // EXPECT: fib_sum(10) = 143
    printf("fib_sum(10) = %d\n", fib_sum(10));

    // EXPECT: fib_even(3) = 1
    printf("fib_even(3) = %d\n", fib_is_even(3));
    // EXPECT: fib_even(6) = 1
    printf("fib_even(6) = %d\n", fib_is_even(6));

    // EXPECT: tribo(5) = 4
    printf("tribo(5) = %d\n", tribonacci(5));
    // EXPECT: tribo(7) = 13
    printf("tribo(7) = %d\n", tribonacci(7));
    // EXPECT: tribo(10) = 81
    printf("tribo(10) = %d\n", tribonacci(10));

    // EXPECT: digits(10) = 2
    printf("digits(10) = %d\n", count_fib_digits(10));
    // EXPECT: digits(20) = 4
    printf("digits(20) = %d\n", count_fib_digits(20));

    // EXPECT: all_methods_match = 1
    int match = (fib_recursive(10) == fib_bottom_up(10)) &&
                (fib_bottom_up(10) == fib_space_opt(10));
    printf("all_methods_match = %d\n", match);

    return 0;
}
