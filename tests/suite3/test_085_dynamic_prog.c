int printf(const char *fmt, ...);

/* Fibonacci with memoization (table-based) */
int fib(int n) {
    int memo[50];
    int i;
    memo[0] = 0;
    memo[1] = 1;
    for (i = 2; i <= n; i++) {
        memo[i] = memo[i-1] + memo[i-2];
    }
    return memo[n];
}

/* Coin change: minimum coins to make amount using denominations in coins[] */
/* Returns -1 if impossible */
int coin_change(int amount, int coins[], int nc) {
    int dp[201];
    int i, j;
    dp[0] = 0;
    for (i = 1; i <= amount; i++) dp[i] = 99999;
    for (i = 1; i <= amount; i++) {
        for (j = 0; j < nc; j++) {
            if (coins[j] <= i && dp[i - coins[j]] + 1 < dp[i]) {
                dp[i] = dp[i - coins[j]] + 1;
            }
        }
    }
    return dp[amount] == 99999 ? -1 : dp[amount];
}

/* Longest Common Subsequence length of two strings */
int lcs_len(char *a, int la, char *b, int lb) {
    int dp[20][20];
    int i, j;
    for (i = 0; i <= la; i++) dp[i][0] = 0;
    for (j = 0; j <= lb; j++) dp[0][j] = 0;
    for (i = 1; i <= la; i++) {
        for (j = 1; j <= lb; j++) {
            if (a[i-1] == b[j-1]) {
                dp[i][j] = dp[i-1][j-1] + 1;
            } else {
                int x = dp[i-1][j];
                int y = dp[i][j-1];
                dp[i][j] = x > y ? x : y;
            }
        }
    }
    return dp[la][lb];
}

/* 0/1 Knapsack: max value with capacity W, n items */
int knapsack(int W, int weights[], int values[], int n) {
    int dp[100];
    int i, w;
    for (w = 0; w <= W; w++) dp[w] = 0;
    for (i = 0; i < n; i++) {
        for (w = W; w >= weights[i]; w--) {
            int candidate = dp[w - weights[i]] + values[i];
            if (candidate > dp[w]) dp[w] = candidate;
        }
    }
    return dp[W];
}

/* Edit distance (Levenshtein) between two strings */
int edit_dist(char *a, int la, char *b, int lb) {
    int dp[20][20];
    int i, j;
    for (i = 0; i <= la; i++) dp[i][0] = i;
    for (j = 0; j <= lb; j++) dp[0][j] = j;
    for (i = 1; i <= la; i++) {
        for (j = 1; j <= lb; j++) {
            if (a[i-1] == b[j-1]) {
                dp[i][j] = dp[i-1][j-1];
            } else {
                int del = dp[i-1][j] + 1;
                int ins = dp[i][j-1] + 1;
                int rep = dp[i-1][j-1] + 1;
                int m = del < ins ? del : ins;
                dp[i][j] = m < rep ? m : rep;
            }
        }
    }
    return dp[la][lb];
}

/* Longest Increasing Subsequence length */
int lis_len(int arr[], int n) {
    int dp[50];
    int i, j, best;
    for (i = 0; i < n; i++) dp[i] = 1;
    for (i = 1; i < n; i++) {
        for (j = 0; j < i; j++) {
            if (arr[j] < arr[i] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
            }
        }
    }
    best = 0;
    for (i = 0; i < n; i++) {
        if (dp[i] > best) best = dp[i];
    }
    return best;
}

/* Maximum subarray sum (Kadane's algorithm) */
int max_subarray(int arr[], int n) {
    int best = arr[0], cur = arr[0], i;
    for (i = 1; i < n; i++) {
        cur = arr[i] > cur + arr[i] ? arr[i] : cur + arr[i];
        if (cur > best) best = cur;
    }
    return best;
}

int main(void) {
    /* Fibonacci */
    printf("%d\n", fib(0));   // EXPECT: 0
    printf("%d\n", fib(1));   // EXPECT: 1
    printf("%d\n", fib(5));   // EXPECT: 5
    printf("%d\n", fib(10));  // EXPECT: 55
    printf("%d\n", fib(15));  // EXPECT: 610
    printf("%d\n", fib(20));  // EXPECT: 6765

    /* coin_change */
    {
        int coins1[] = {1, 5, 10, 25};
        printf("%d\n", coin_change(11, coins1, 4));   // EXPECT: 2
        /* 10+1 */
        printf("%d\n", coin_change(30, coins1, 4));   // EXPECT: 2
        /* 25+5 */
        printf("%d\n", coin_change(41, coins1, 4));   // EXPECT: 4
        /* 25+10+5+1 */

        int coins2[] = {2};
        printf("%d\n", coin_change(3, coins2, 1));    // EXPECT: -1
    }

    /* lcs_len */
    {
        char a[] = "ABCBDAB";
        char b[] = "BDCAB";
        printf("%d\n", lcs_len(a, 7, b, 5));   // EXPECT: 4
        /* BCAB */

        char c[] = "AGGTAB";
        char d[] = "GXTXAYB";
        printf("%d\n", lcs_len(c, 6, d, 7));   // EXPECT: 4
        /* GTAB */

        char e[] = "ABC";
        char f[] = "AC";
        printf("%d\n", lcs_len(e, 3, f, 2));   // EXPECT: 2
    }

    /* knapsack */
    {
        int w[] = {2, 3, 4, 5};
        int v[] = {3, 4, 5, 6};
        printf("%d\n", knapsack(5, w, v, 4));   // EXPECT: 7
        /* items 0(w=2,v=3) + 1(w=3,v=4) = weight 5, value 7 */
        printf("%d\n", knapsack(8, w, v, 4));   // EXPECT: 10
        /* items 1(w=3,v=4) + 2(w=4,v=5) + ... best: 3+5=weight5 v7? */
        /* weights {2,3,4,5} values {3,4,5,6}  W=8 */
        /* try 2+3+? = 5 -> v7 with room 3 left: add nothing useful */
        /* try 2+4=6 -> v8, try 3+5=8 -> v10 */
        printf("%d\n", knapsack(10, w, v, 4));  // EXPECT: 13
        /* 2+3+5=10 -> v3+v4+v6=13 */
    }

    /* edit_dist */
    {
        char a[] = "kitten";
        char b[] = "sitting";
        printf("%d\n", edit_dist(a, 6, b, 7));  // EXPECT: 3

        char c[] = "abc";
        char d[] = "abc";
        printf("%d\n", edit_dist(c, 3, d, 3));  // EXPECT: 0

        char e[] = "abc";
        char f[] = "xyz";
        printf("%d\n", edit_dist(e, 3, f, 3));  // EXPECT: 3
    }

    /* lis_len */
    {
        int a[] = {10, 9, 2, 5, 3, 7, 101, 18};
        printf("%d\n", lis_len(a, 8));  // EXPECT: 4
        /* 2,3,7,18 or 2,5,7,18 */

        int b[] = {0, 1, 0, 3, 2, 3};
        printf("%d\n", lis_len(b, 6));  // EXPECT: 4
        /* 0,1,2,3 */

        int c[] = {7, 7, 7};
        printf("%d\n", lis_len(c, 3));  // EXPECT: 1
    }

    /* max_subarray */
    {
        int a[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
        printf("%d\n", max_subarray(a, 9));  // EXPECT: 6
        /* [4,-1,2,1] = 6 */

        int b[] = {1, 2, 3, 4, 5};
        printf("%d\n", max_subarray(b, 5));  // EXPECT: 15

        int c[] = {-3, -1, -2};
        printf("%d\n", max_subarray(c, 3));  // EXPECT: -1
    }

    return 0;
}
