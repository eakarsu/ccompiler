int printf(const char *fmt, ...);

int factorial(int n) {
    int result = 1;
    int i = 2;
    while (i <= n) {
        result = result * i;
        i = i + 1;
    }
    return result;
}

int binomial(int n, int k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    int result = 1;
    int i = 0;
    while (i < k) {
        result = result * (n - i);
        result = result / (i + 1);
        i = i + 1;
    }
    return result;
}

int permutations(int n, int r) {
    int result = 1;
    int i = 0;
    while (i < r) {
        result = result * (n - i);
        i = i + 1;
    }
    return result;
}

int derangements(int n) {
    if (n == 0) return 1;
    if (n == 1) return 0;
    int dp[15];
    dp[0] = 1;
    dp[1] = 0;
    int i = 2;
    while (i <= n) {
        dp[i] = (i - 1) * (dp[i - 1] + dp[i - 2]);
        i = i + 1;
    }
    return dp[n];
}

int stirling2(int n, int k) {
    if (n == 0 && k == 0) return 1;
    if (n == 0 || k == 0) return 0;
    if (k == 1 || k == n) return 1;
    int dp[10][10];
    int i = 0;
    while (i <= n) {
        int j = 0;
        while (j <= k) {
            dp[i][j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }
    dp[0][0] = 1;
    i = 1;
    while (i <= n) {
        int j = 1;
        while (j <= i && j <= k) {
            dp[i][j] = j * dp[i-1][j] + dp[i-1][j-1];
            j = j + 1;
        }
        i = i + 1;
    }
    return dp[n][k];
}

int catalan(int n) {
    return binomial(2 * n, n) / (n + 1);
}

int multichoose(int n, int k) {
    return binomial(n + k - 1, k);
}

int sum_binomial_row(int n) {
    int sum = 0;
    int k = 0;
    while (k <= n) {
        sum = sum + binomial(n, k);
        k = k + 1;
    }
    return sum;
}

int central_binomial(int n) {
    return binomial(2 * n, n);
}

int alternating_binomial(int n) {
    int sum = 0;
    int k = 0;
    int sign = 1;
    while (k <= n) {
        sum = sum + sign * binomial(n, k);
        sign = 0 - sign;
        k = k + 1;
    }
    return sum;
}

int count_subsets(int n, int min_size) {
    int count = 0;
    int k = min_size;
    while (k <= n) {
        count = count + binomial(n, k);
        k = k + 1;
    }
    return count;
}

int main(void) {
    // EXPECT: 0! = 1
    printf("0! = %d\n", factorial(0));
    // EXPECT: 5! = 120
    printf("5! = %d\n", factorial(5));
    // EXPECT: 8! = 40320
    printf("8! = %d\n", factorial(8));

    // EXPECT: C(5,2) = 10
    printf("C(5,2) = %d\n", binomial(5, 2));
    // EXPECT: C(10,3) = 120
    printf("C(10,3) = %d\n", binomial(10, 3));
    // EXPECT: C(8,4) = 70
    printf("C(8,4) = %d\n", binomial(8, 4));

    // EXPECT: P(5,3) = 60
    printf("P(5,3) = %d\n", permutations(5, 3));
    // EXPECT: P(8,2) = 56
    printf("P(8,2) = %d\n", permutations(8, 2));

    // EXPECT: D(3) = 2
    printf("D(3) = %d\n", derangements(3));
    // EXPECT: D(4) = 9
    printf("D(4) = %d\n", derangements(4));
    // EXPECT: D(5) = 44
    printf("D(5) = %d\n", derangements(5));

    // EXPECT: S(4,2) = 7
    printf("S(4,2) = %d\n", stirling2(4, 2));
    // EXPECT: S(5,3) = 25
    printf("S(5,3) = %d\n", stirling2(5, 3));

    // EXPECT: cat(4) = 14
    printf("cat(4) = %d\n", catalan(4));

    // EXPECT: mc(3,2) = 6
    printf("mc(3,2) = %d\n", multichoose(3, 2));

    // EXPECT: row_sum(6) = 64
    printf("row_sum(6) = %d\n", sum_binomial_row(6));
    // EXPECT: cbin(4) = 70
    printf("cbin(4) = %d\n", central_binomial(4));
    // EXPECT: alt(5) = 0
    printf("alt(5) = %d\n", alternating_binomial(5));

    // EXPECT: subsets(5,2) = 26
    printf("subsets(5,2) = %d\n", count_subsets(5, 2));

    return 0;
}
