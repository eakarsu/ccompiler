int printf(const char *fmt, ...);

int min_coins(int coins[], int n, int amount) {
    int dp[200];
    int i = 0;
    while (i <= amount) {
        dp[i] = 99999;
        i = i + 1;
    }
    dp[0] = 0;

    i = 1;
    while (i <= amount) {
        int j = 0;
        while (j < n) {
            if (coins[j] <= i && dp[i - coins[j]] + 1 < dp[i]) {
                dp[i] = dp[i - coins[j]] + 1;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    if (dp[amount] >= 99999) return -1;
    return dp[amount];
}

int count_ways(int coins[], int n, int amount) {
    int dp[200];
    int i = 0;
    while (i <= amount) {
        dp[i] = 0;
        i = i + 1;
    }
    dp[0] = 1;

    i = 0;
    while (i < n) {
        int j = coins[i];
        while (j <= amount) {
            dp[j] = dp[j] + dp[j - coins[i]];
            j = j + 1;
        }
        i = i + 1;
    }
    return dp[amount];
}

int count_permutations(int coins[], int n, int amount) {
    int dp[200];
    int i = 0;
    while (i <= amount) {
        dp[i] = 0;
        i = i + 1;
    }
    dp[0] = 1;

    i = 1;
    while (i <= amount) {
        int j = 0;
        while (j < n) {
            if (coins[j] <= i) {
                dp[i] = dp[i] + dp[i - coins[j]];
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return dp[amount];
}

int trace_coins(int coins[], int n, int amount, int used[]) {
    int dp[200];
    int from[200];
    int i = 0;
    while (i <= amount) {
        dp[i] = 99999;
        from[i] = -1;
        i = i + 1;
    }
    dp[0] = 0;

    i = 1;
    while (i <= amount) {
        int j = 0;
        while (j < n) {
            if (coins[j] <= i && dp[i - coins[j]] + 1 < dp[i]) {
                dp[i] = dp[i - coins[j]] + 1;
                from[i] = coins[j];
            }
            j = j + 1;
        }
        i = i + 1;
    }
    if (dp[amount] >= 99999) return 0;

    int count = 0;
    int rem = amount;
    while (rem > 0) {
        used[count] = from[rem];
        rem = rem - from[rem];
        count = count + 1;
    }
    return count;
}

int max_value_coins(int coins[], int n, int amount) {
    int dp[200];
    int i = 0;
    while (i <= amount) {
        dp[i] = -99999;
        i = i + 1;
    }
    dp[0] = 0;

    i = 1;
    while (i <= amount) {
        int j = 0;
        while (j < n) {
            if (coins[j] <= i && dp[i - coins[j]] != -99999) {
                int val = dp[i - coins[j]] + 1;
                if (val > dp[i]) dp[i] = val;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    if (dp[amount] < 0) return -1;
    return dp[amount];
}

int main(void) {
    int c1[3];
    c1[0] = 1; c1[1] = 5; c1[2] = 10;

    // EXPECT: min(11) = 2
    printf("min(11) = %d\n", min_coins(c1, 3, 11));
    // EXPECT: min(15) = 2
    printf("min(15) = %d\n", min_coins(c1, 3, 15));
    // EXPECT: min(1) = 1
    printf("min(1) = %d\n", min_coins(c1, 3, 1));
    // EXPECT: min(0) = 0
    printf("min(0) = %d\n", min_coins(c1, 3, 0));
    // EXPECT: min(6) = 2
    printf("min(6) = %d\n", min_coins(c1, 3, 6));
    // EXPECT: min(30) = 3
    printf("min(30) = %d\n", min_coins(c1, 3, 30));

    // EXPECT: ways(10) = 4
    printf("ways(10) = %d\n", count_ways(c1, 3, 10));
    // EXPECT: ways(15) = 6
    printf("ways(15) = %d\n", count_ways(c1, 3, 15));
    // EXPECT: ways(5) = 2
    printf("ways(5) = %d\n", count_ways(c1, 3, 5));
    // EXPECT: ways(0) = 1
    printf("ways(0) = %d\n", count_ways(c1, 3, 0));

    // EXPECT: perms(5) = 2
    printf("perms(5) = %d\n", count_permutations(c1, 3, 5));
    // EXPECT: perms(10) = 9
    printf("perms(10) = %d\n", count_permutations(c1, 3, 10));

    int used[50];
    int cnt = trace_coins(c1, 3, 11, used);
    // EXPECT: trace(11) count=2
    printf("trace(11) count=%d\n", cnt);

    int sum = 0;
    int i = 0;
    while (i < cnt) {
        sum = sum + used[i];
        i = i + 1;
    }
    // EXPECT: trace(11) sum=11
    printf("trace(11) sum=%d\n", sum);

    int c2[3];
    c2[0] = 3; c2[1] = 7; c2[2] = 11;
    // EXPECT: min2(5) = -1
    printf("min2(5) = %d\n", min_coins(c2, 3, 5));
    // EXPECT: min2(14) = 2
    printf("min2(14) = %d\n", min_coins(c2, 3, 14));

    int c3[2];
    c3[0] = 1; c3[1] = 2;
    // EXPECT: ways3(4) = 3
    printf("ways3(4) = %d\n", count_ways(c3, 2, 4));
    // EXPECT: perms3(4) = 5
    printf("perms3(4) = %d\n", count_permutations(c3, 2, 4));

    // EXPECT: maxval(10) = 10
    printf("maxval(10) = %d\n", max_value_coins(c3, 2, 10));

    return 0;
}
