int printf(const char *fmt, ...);

// Coin Change Problem: minimum coins and number of ways

// Minimum number of coins to make amount
// Returns -1 if not possible
int min_coins(int *coins, int n, int amount) {
    int dp[256];
    int i;
    int j;
    int big;

    big = amount + 1;
    for (i = 0; i <= amount; i++) {
        dp[i] = big;
    }
    dp[0] = 0;

    for (i = 1; i <= amount; i++) {
        for (j = 0; j < n; j++) {
            if (coins[j] <= i) {
                if (dp[i - coins[j]] + 1 < dp[i]) {
                    dp[i] = dp[i - coins[j]] + 1;
                }
            }
        }
    }

    if (dp[amount] > amount) return -1;
    return dp[amount];
}

// Number of ways to make change (order doesn't matter)
int count_ways(int *coins, int n, int amount) {
    int dp[256];
    int i;
    int j;

    for (i = 0; i <= amount; i++) {
        dp[i] = 0;
    }
    dp[0] = 1;

    // For combinations (not permutations), iterate coins in outer loop
    for (i = 0; i < n; i++) {
        for (j = coins[i]; j <= amount; j++) {
            dp[j] = dp[j] + dp[j - coins[i]];
        }
    }

    return dp[amount];
}

// Number of ways (order matters = permutations)
int count_perms(int *coins, int n, int amount) {
    int dp[256];
    int i;
    int j;

    for (i = 0; i <= amount; i++) {
        dp[i] = 0;
    }
    dp[0] = 1;

    // Iterate amount in outer loop for permutations
    for (i = 1; i <= amount; i++) {
        for (j = 0; j < n; j++) {
            if (coins[j] <= i) {
                dp[i] = dp[i] + dp[i - coins[j]];
            }
        }
    }

    return dp[amount];
}

// Find which coins are used in minimum solution
// Store coins used in result[], return count
int min_coins_trace(int *coins, int n, int amount, int *result) {
    int dp[256];
    int parent[256];
    int i;
    int j;
    int big;
    int count;
    int rem;

    big = amount + 1;
    for (i = 0; i <= amount; i++) {
        dp[i] = big;
        parent[i] = -1;
    }
    dp[0] = 0;

    for (i = 1; i <= amount; i++) {
        for (j = 0; j < n; j++) {
            if (coins[j] <= i) {
                if (dp[i - coins[j]] + 1 < dp[i]) {
                    dp[i] = dp[i - coins[j]] + 1;
                    parent[i] = coins[j];
                }
            }
        }
    }

    if (dp[amount] > amount) return 0;

    count = 0;
    rem = amount;
    while (rem > 0) {
        result[count] = parent[rem];
        count = count + 1;
        rem = rem - parent[rem];
    }
    return count;
}

int main() {
    int coins1[3];
    int coins2[3];
    int coins3[4];
    int result[32];
    int cnt;
    int i;
    int sum;

    // coins = {1, 5, 10}, amount = 11
    coins1[0] = 1; coins1[1] = 5; coins1[2] = 10;
    printf("%d\n", min_coins(coins1, 3, 11));
    // EXPECT: 2

    // amount = 0
    printf("%d\n", min_coins(coins1, 3, 0));
    // EXPECT: 0

    // amount = 1
    printf("%d\n", min_coins(coins1, 3, 1));
    // EXPECT: 1

    // amount = 15: 10+5 = 2 coins
    printf("%d\n", min_coins(coins1, 3, 15));
    // EXPECT: 2

    // amount = 23: 10+10+1+1+1 = 5 coins
    printf("%d\n", min_coins(coins1, 3, 23));
    // EXPECT: 5

    // coins = {3, 7, 11}, amount = 5: impossible
    coins2[0] = 3; coins2[1] = 7; coins2[2] = 11;
    printf("%d\n", min_coins(coins2, 3, 5));
    // EXPECT: -1

    // amount = 14: 7+7 = 2 coins
    printf("%d\n", min_coins(coins2, 3, 14));
    // EXPECT: 2

    // amount = 6: 3+3 = 2 coins
    printf("%d\n", min_coins(coins2, 3, 6));
    // EXPECT: 2

    // Ways to make change: coins {1,5,10}, amount=10
    // 10, 5+5, 5+1*5, 1*10 => 4
    printf("%d\n", count_ways(coins1, 3, 10));
    // EXPECT: 4

    // Ways for amount=5: {5}, {1*5} => 2
    printf("%d\n", count_ways(coins1, 3, 5));
    // EXPECT: 2

    // Ways for amount=1: {1} => 1
    printf("%d\n", count_ways(coins1, 3, 1));
    // EXPECT: 1

    // Ways for amount=0: 1 (empty set)
    printf("%d\n", count_ways(coins1, 3, 0));
    // EXPECT: 1

    // Permutations: coins {1,2}, amount=3
    // 1+1+1, 1+2, 2+1 => 3
    coins3[0] = 1; coins3[1] = 2;
    printf("%d\n", count_perms(coins3, 2, 3));
    // EXPECT: 3

    // Permutations: coins {1,2}, amount=4
    // 1111, 112, 121, 211, 22 => 5
    printf("%d\n", count_perms(coins3, 2, 4));
    // EXPECT: 5

    // Trace min coins: {1,5,10}, amount=13 => 10+1+1+1 = 4 coins
    cnt = min_coins_trace(coins1, 3, 13, result);
    printf("%d\n", cnt);
    // EXPECT: 4
    // Verify traced coins sum to amount
    sum = 0;
    for (i = 0; i < cnt; i++) {
        sum = sum + result[i];
    }
    printf("%d\n", sum);
    // EXPECT: 13

    // Trace: amount=25 => 10+10+5 = 3 coins
    cnt = min_coins_trace(coins1, 3, 25, result);
    printf("%d\n", cnt);
    // EXPECT: 3
    sum = 0;
    for (i = 0; i < cnt; i++) {
        sum = sum + result[i];
    }
    printf("%d\n", sum);
    // EXPECT: 25

    printf("done\n");
    // EXPECT: done

    return 0;
}
