int printf(const char *fmt, ...);

// Coin Change Problem: minimum coins and number of ways

// Minimum coins to make amount using given denominations
// Returns -1 if impossible
int min_coins(int *coins, int num_coins, int amount) {
    int dp[201];  // dp[i] = min coins for amount i
    int i;
    int j;

    // Initialize with large value
    for (i = 0; i <= amount; i++) {
        dp[i] = 9999;
    }
    dp[0] = 0;

    for (i = 1; i <= amount; i++) {
        for (j = 0; j < num_coins; j++) {
            if (coins[j] <= i) {
                int sub = dp[i - coins[j]];
                if (sub != 9999 && sub + 1 < dp[i]) {
                    dp[i] = sub + 1;
                }
            }
        }
    }

    if (dp[amount] == 9999) return -1;
    return dp[amount];
}

// Count number of ways to make change
int count_ways(int *coins, int num_coins, int amount) {
    int dp[201];
    int i;
    int j;

    for (i = 0; i <= amount; i++) {
        dp[i] = 0;
    }
    dp[0] = 1;

    for (i = 0; i < num_coins; i++) {
        for (j = coins[i]; j <= amount; j++) {
            dp[j] = dp[j] + dp[j - coins[i]];
        }
    }

    return dp[amount];
}

// Find which coins are used in the minimum solution
// stores the result in 'used' array, returns count of coins used
int find_coins_used(int *coins, int num_coins, int amount, int *used) {
    int dp[201];
    int parent[201];  // which coin was used at each step
    int i;
    int j;

    for (i = 0; i <= amount; i++) {
        dp[i] = 9999;
        parent[i] = -1;
    }
    dp[0] = 0;

    for (i = 1; i <= amount; i++) {
        for (j = 0; j < num_coins; j++) {
            if (coins[j] <= i && dp[i - coins[j]] != 9999) {
                if (dp[i - coins[j]] + 1 < dp[i]) {
                    dp[i] = dp[i - coins[j]] + 1;
                    parent[i] = coins[j];
                }
            }
        }
    }

    // Trace back the solution
    int count = 0;
    int rem = amount;
    while (rem > 0 && parent[rem] != -1) {
        used[count] = parent[rem];
        rem = rem - parent[rem];
        count = count + 1;
    }
    return count;
}

// Greedy coin change (works for standard denominations)
int greedy_coins(int *coins, int num_coins, int amount) {
    int count = 0;
    int i;
    // Assumes coins are sorted in ascending order
    for (i = num_coins - 1; i >= 0; i--) {
        while (amount >= coins[i]) {
            amount = amount - coins[i];
            count = count + 1;
        }
    }
    if (amount > 0) return -1;
    return count;
}

// Check if exact change is possible
int can_make_change(int *coins, int num_coins, int amount) {
    int dp[201];
    int i;
    int j;
    for (i = 0; i <= amount; i++) dp[i] = 0;
    dp[0] = 1;

    for (i = 0; i < num_coins; i++) {
        for (j = coins[i]; j <= amount; j++) {
            if (dp[j - coins[i]]) dp[j] = 1;
        }
    }
    return dp[amount];
}

int main() {
    int coins1[3];
    coins1[0] = 1;
    coins1[1] = 5;
    coins1[2] = 10;

    // EXPECT: min_coins(11) = 2
    printf("min_coins(11) = %d\n", min_coins(coins1, 3, 11));
    // EXPECT: min_coins(15) = 2
    printf("min_coins(15) = %d\n", min_coins(coins1, 3, 15));
    // EXPECT: min_coins(23) = 5
    printf("min_coins(23) = %d\n", min_coins(coins1, 3, 23));
    // EXPECT: min_coins(1) = 1
    printf("min_coins(1) = %d\n", min_coins(coins1, 3, 1));

    int coins2[3];
    coins2[0] = 3;
    coins2[1] = 7;
    coins2[2] = 11;

    // EXPECT: min_coins2(14) = 2
    printf("min_coins2(14) = %d\n", min_coins(coins2, 3, 14));
    // EXPECT: min_coins2(2) = -1
    printf("min_coins2(2) = %d\n", min_coins(coins2, 3, 2));
    // EXPECT: min_coins2(21) = 3
    printf("min_coins2(21) = %d\n", min_coins(coins2, 3, 21));

    // Test count_ways
    // EXPECT: ways(10) = 4
    printf("ways(10) = %d\n", count_ways(coins1, 3, 10));
    // EXPECT: ways(15) = 6
    printf("ways(15) = %d\n", count_ways(coins1, 3, 15));
    // EXPECT: ways(5) = 2
    printf("ways(5) = %d\n", count_ways(coins1, 3, 5));

    int coins3[4];
    coins3[0] = 1;
    coins3[1] = 5;
    coins3[2] = 10;
    coins3[3] = 25;
    // EXPECT: ways_us(30) = 18
    printf("ways_us(30) = %d\n", count_ways(coins3, 4, 30));
    // EXPECT: ways_us(50) = 49
    printf("ways_us(50) = %d\n", count_ways(coins3, 4, 50));

    // Test find_coins_used
    int used[20];
    int n_used;
    n_used = find_coins_used(coins1, 3, 23, used);
    // EXPECT: n_used(23) = 5
    printf("n_used(23) = %d\n", n_used);

    // Verify the coins sum to 23
    int sum = 0;
    int i;
    for (i = 0; i < n_used; i++) {
        sum = sum + used[i];
    }
    // EXPECT: sum_used = 23
    printf("sum_used = %d\n", sum);

    // Test greedy
    // EXPECT: greedy(11) = 2
    printf("greedy(11) = %d\n", greedy_coins(coins1, 3, 11));
    // EXPECT: greedy(28) = 6
    printf("greedy(28) = %d\n", greedy_coins(coins1, 3, 28));

    // Test can_make_change
    // EXPECT: can_change(14) = 1
    printf("can_change(14) = %d\n", can_make_change(coins2, 3, 14));
    // EXPECT: can_change(2) = 0
    printf("can_change(2) = %d\n", can_make_change(coins2, 3, 2));
    // EXPECT: can_change(10) = 1
    printf("can_change(10) = %d\n", can_make_change(coins2, 3, 10));

    return 0;
}
