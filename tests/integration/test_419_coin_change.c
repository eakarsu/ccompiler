int printf(const char *fmt, ...);
// EXPECT: Min coins for 30: 2\nMin coins for 11: 2\nMin coins for 99: 9\nWays to make 5: 2\nWays to make 10: 4\nWays to make 25: 13\nCoins used: 25 5\nCoins used: 25 10 5 1\nMin coins for 14: 2\nMin coins for 5: -1
// Test: Coin change problem (min coins & count ways)

int dp_min[256];
int dp_ways[256];

int min_coins(int coins[], int n, int amount) {
    int INF = 999999;
    int i, j;
    dp_min[0] = 0;
    for (i = 1; i <= amount; i++) {
        dp_min[i] = INF;
    }
    for (i = 0; i < n; i++) {
        for (j = coins[i]; j <= amount; j++) {
            if (dp_min[j - coins[i]] + 1 < dp_min[j]) {
                dp_min[j] = dp_min[j - coins[i]] + 1;
            }
        }
    }
    if (dp_min[amount] >= INF) return -1;
    return dp_min[amount];
}

int count_ways(int coins[], int n, int amount) {
    int i, j;
    for (i = 0; i <= amount; i++) {
        dp_ways[i] = 0;
    }
    dp_ways[0] = 1;
    for (i = 0; i < n; i++) {
        for (j = coins[i]; j <= amount; j++) {
            dp_ways[j] = dp_ways[j] + dp_ways[j - coins[i]];
        }
    }
    return dp_ways[amount];
}

void print_coins_used(int coins[], int n, int amount) {
    int INF = 999999;
    int parent[256];
    int i, j;
    dp_min[0] = 0;
    parent[0] = -1;
    for (i = 1; i <= amount; i++) {
        dp_min[i] = INF;
        parent[i] = -1;
    }
    for (i = 0; i < n; i++) {
        for (j = coins[i]; j <= amount; j++) {
            if (dp_min[j - coins[i]] + 1 < dp_min[j]) {
                dp_min[j] = dp_min[j - coins[i]] + 1;
                parent[j] = coins[i];
            }
        }
    }
    printf("Coins used:");
    int a = amount;
    while (a > 0 && parent[a] != -1) {
        printf(" %d", parent[a]);
        a = a - parent[a];
    }
    printf("\n");
}

int main(void) {
    int c1[] = {1, 5, 10, 25};
    int n1 = 4;
    printf("Min coins for 30: %d\n", min_coins(c1, n1, 30));
    printf("Min coins for 11: %d\n", min_coins(c1, n1, 11));
    printf("Min coins for 99: %d\n", min_coins(c1, n1, 99));

    printf("Ways to make 5: %d\n", count_ways(c1, n1, 5));
    printf("Ways to make 10: %d\n", count_ways(c1, n1, 10));
    printf("Ways to make 25: %d\n", count_ways(c1, n1, 25));

    print_coins_used(c1, n1, 30);
    print_coins_used(c1, n1, 41);

    int c2[] = {3, 7};
    int n2 = 2;
    printf("Min coins for 14: %d\n", min_coins(c2, n2, 14));
    printf("Min coins for 5: %d\n", min_coins(c2, n2, 5));

    return 0;
}
