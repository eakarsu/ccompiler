int printf(const char *fmt, ...);
// EXPECT: 1:0\n2:1\n3:5\n4:55\n5:610\n6:6765\n7:55\n8:6765\n9:7\n10:13\n11:3\n12:0\n13:22\n14:16\n15:4\n16:3\n17:0\n18:1\n19:2\n20:2\n21:0\n22:1\n23:2\n24:-1\n25:2\n26:4\n27:10\n28:3\n29:0\n30:3\n31:3\n32:6\n33:6\n34:-1\n35:4\n36:4\n37:1\n38:1\n39:2\n40:8\n41:89\n42:4\n43:12\n44:5\n45:7\n46:7\n47:6\n48:2\n49:20\n50:1\n51:13\n52:144
// =============================================================================
// TEST 023: Dynamic Programming
// =============================================================================
// Tests classic DP problems: Fibonacci, knapsack (small), longest common
// subsequence, coin change, edit distance, and more.
// =============================================================================

// -----------------------------------------------------------------------------
// DP tables (global to avoid stack issues)
// -----------------------------------------------------------------------------
int dp[100];
int dp2[50][50];
int memo[50];

// -----------------------------------------------------------------------------
// Fibonacci with memoization
// -----------------------------------------------------------------------------
int fib_cache[50];
int fib_inited;

void fib_init(void) {
    int i;
    for (i = 0; i < 50; i = i + 1) {
        fib_cache[i] = -1;
    }
    fib_inited = 1;
}

int fib(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    if (fib_cache[n] != -1) return fib_cache[n];
    fib_cache[n] = fib(n - 1) + fib(n - 2);
    return fib_cache[n];
}

// -----------------------------------------------------------------------------
// Fibonacci iterative (bottom-up)
// -----------------------------------------------------------------------------
int fib_iter(int n) {
    int i;
    if (n <= 0) return 0;
    if (n == 1) return 1;
    dp[0] = 0;
    dp[1] = 1;
    for (i = 2; i <= n; i = i + 1) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    return dp[n];
}

// -----------------------------------------------------------------------------
// 0/1 Knapsack problem (small)
// weights and values arrays, capacity W, n items
// Returns max value
// -----------------------------------------------------------------------------
int knap_w[20];
int knap_v[20];
int knap_dp[21][101]; // [items+1][capacity+1]

int knapsack(int n, int W) {
    int i;
    int w;
    for (i = 0; i <= n; i = i + 1) {
        for (w = 0; w <= W; w = w + 1) {
            knap_dp[i][w] = 0;
        }
    }
    for (i = 1; i <= n; i = i + 1) {
        for (w = 0; w <= W; w = w + 1) {
            knap_dp[i][w] = knap_dp[i - 1][w];
            if (knap_w[i - 1] <= w) {
                int include;
                include = knap_dp[i - 1][w - knap_w[i - 1]] + knap_v[i - 1];
                if (include > knap_dp[i][w]) {
                    knap_dp[i][w] = include;
                }
            }
        }
    }
    return knap_dp[n][W];
}

// -----------------------------------------------------------------------------
// Longest Common Subsequence
// Uses two integer arrays (treating ints as characters)
// -----------------------------------------------------------------------------
int lcs_a[50];
int lcs_b[50];
int lcs_dp[51][51];

int lcs(int m, int n) {
    int i;
    int j;
    for (i = 0; i <= m; i = i + 1) {
        for (j = 0; j <= n; j = j + 1) {
            lcs_dp[i][j] = 0;
        }
    }
    for (i = 1; i <= m; i = i + 1) {
        for (j = 1; j <= n; j = j + 1) {
            if (lcs_a[i - 1] == lcs_b[j - 1]) {
                lcs_dp[i][j] = lcs_dp[i - 1][j - 1] + 1;
            } else {
                if (lcs_dp[i - 1][j] > lcs_dp[i][j - 1]) {
                    lcs_dp[i][j] = lcs_dp[i - 1][j];
                } else {
                    lcs_dp[i][j] = lcs_dp[i][j - 1];
                }
            }
        }
    }
    return lcs_dp[m][n];
}

// -----------------------------------------------------------------------------
// Coin change: minimum coins to make amount
// coins array, num_coins, amount
// Returns min coins or -1 if impossible
// -----------------------------------------------------------------------------
int coins[10];
int coin_dp[200];

int coin_change(int num_coins, int amount) {
    int i;
    int j;
    int big;
    big = 9999;
    for (i = 0; i <= amount; i = i + 1) {
        coin_dp[i] = big;
    }
    coin_dp[0] = 0;
    for (i = 1; i <= amount; i = i + 1) {
        for (j = 0; j < num_coins; j = j + 1) {
            if (coins[j] <= i) {
                if (coin_dp[i - coins[j]] + 1 < coin_dp[i]) {
                    coin_dp[i] = coin_dp[i - coins[j]] + 1;
                }
            }
        }
    }
    if (coin_dp[amount] >= big) return -1;
    return coin_dp[amount];
}

// -----------------------------------------------------------------------------
// Coin change: count number of ways to make amount
// -----------------------------------------------------------------------------
int ways_dp[200];

int coin_ways(int num_coins, int amount) {
    int i;
    int j;
    for (i = 0; i <= amount; i = i + 1) {
        ways_dp[i] = 0;
    }
    ways_dp[0] = 1;
    for (i = 0; i < num_coins; i = i + 1) {
        for (j = coins[i]; j <= amount; j = j + 1) {
            ways_dp[j] = ways_dp[j] + ways_dp[j - coins[i]];
        }
    }
    return ways_dp[amount];
}

// -----------------------------------------------------------------------------
// Edit distance between two int arrays
// (treating ints as character codes)
// -----------------------------------------------------------------------------
int ed_a[30];
int ed_b[30];
int ed_dp[31][31];

int edit_distance(int m, int n) {
    int i;
    int j;
    int cost;
    int del_cost;
    int ins;
    int rep;
    int mn;
    for (i = 0; i <= m; i = i + 1) ed_dp[i][0] = i;
    for (j = 0; j <= n; j = j + 1) ed_dp[0][j] = j;
    for (i = 1; i <= m; i = i + 1) {
        for (j = 1; j <= n; j = j + 1) {
            if (ed_a[i - 1] == ed_b[j - 1]) {
                cost = 0;
            } else {
                cost = 1;
            }
            del_cost = ed_dp[i - 1][j] + 1;
            ins = ed_dp[i][j - 1] + 1;
            rep = ed_dp[i - 1][j - 1] + cost;
            mn = del_cost;
            if (ins < mn) mn = ins;
            if (rep < mn) mn = rep;
            ed_dp[i][j] = mn;
        }
    }
    return ed_dp[m][n];
}

// -----------------------------------------------------------------------------
// Maximum subarray sum (Kadane's algorithm)
// -----------------------------------------------------------------------------
int kadane_arr[50];

int max_subarray_sum(int n) {
    int i;
    int max_ending;
    int max_so_far;
    max_ending = kadane_arr[0];
    max_so_far = kadane_arr[0];
    for (i = 1; i < n; i = i + 1) {
        if (max_ending + kadane_arr[i] > kadane_arr[i]) {
            max_ending = max_ending + kadane_arr[i];
        } else {
            max_ending = kadane_arr[i];
        }
        if (max_ending > max_so_far) {
            max_so_far = max_ending;
        }
    }
    return max_so_far;
}

// -----------------------------------------------------------------------------
// Longest increasing subsequence length
// -----------------------------------------------------------------------------
int lis_arr[50];
int lis_dp_arr[50];

int lis(int n) {
    int i;
    int j;
    int max_len;
    for (i = 0; i < n; i = i + 1) lis_dp_arr[i] = 1;
    for (i = 1; i < n; i = i + 1) {
        for (j = 0; j < i; j = j + 1) {
            if (lis_arr[j] < lis_arr[i]) {
                if (lis_dp_arr[j] + 1 > lis_dp_arr[i]) {
                    lis_dp_arr[i] = lis_dp_arr[j] + 1;
                }
            }
        }
    }
    max_len = 0;
    for (i = 0; i < n; i = i + 1) {
        if (lis_dp_arr[i] > max_len) max_len = lis_dp_arr[i];
    }
    return max_len;
}

// -----------------------------------------------------------------------------
// Climbing stairs: number of ways to climb n stairs (1 or 2 steps)
// -----------------------------------------------------------------------------
int climb_dp[100];

int climb_stairs(int n) {
    int i;
    if (n <= 0) return 0;
    if (n == 1) return 1;
    climb_dp[1] = 1;
    climb_dp[2] = 2;
    for (i = 3; i <= n; i = i + 1) {
        climb_dp[i] = climb_dp[i - 1] + climb_dp[i - 2];
    }
    return climb_dp[n];
}

// -----------------------------------------------------------------------------
// House robber: max sum of non-adjacent elements
// -----------------------------------------------------------------------------
int rob_arr[50];
int rob_dp_arr[50];

int house_robber(int n) {
    int i;
    if (n == 0) return 0;
    if (n == 1) return rob_arr[0];
    rob_dp_arr[0] = rob_arr[0];
    if (rob_arr[1] > rob_arr[0]) {
        rob_dp_arr[1] = rob_arr[1];
    } else {
        rob_dp_arr[1] = rob_arr[0];
    }
    for (i = 2; i < n; i = i + 1) {
        int take;
        int skip;
        take = rob_dp_arr[i - 2] + rob_arr[i];
        skip = rob_dp_arr[i - 1];
        if (take > skip) {
            rob_dp_arr[i] = take;
        } else {
            rob_dp_arr[i] = skip;
        }
    }
    return rob_dp_arr[n - 1];
}

// -----------------------------------------------------------------------------
// Min cost path in a grid (top-left to bottom-right)
// Can only move right or down
// -----------------------------------------------------------------------------
int grid[10][10];
int path_dp[10][10];

int min_cost_path(int rows, int cols) {
    int i;
    int j;
    path_dp[0][0] = grid[0][0];
    for (i = 1; i < rows; i = i + 1) {
        path_dp[i][0] = path_dp[i - 1][0] + grid[i][0];
    }
    for (j = 1; j < cols; j = j + 1) {
        path_dp[0][j] = path_dp[0][j - 1] + grid[0][j];
    }
    for (i = 1; i < rows; i = i + 1) {
        for (j = 1; j < cols; j = j + 1) {
            if (path_dp[i - 1][j] < path_dp[i][j - 1]) {
                path_dp[i][j] = path_dp[i - 1][j] + grid[i][j];
            } else {
                path_dp[i][j] = path_dp[i][j - 1] + grid[i][j];
            }
        }
    }
    return path_dp[rows - 1][cols - 1];
}

// -----------------------------------------------------------------------------
// Count paths in grid (top-left to bottom-right, only right/down)
// -----------------------------------------------------------------------------
int cnt_paths[10][10];

int count_paths(int rows, int cols) {
    int i;
    int j;
    for (i = 0; i < rows; i = i + 1) cnt_paths[i][0] = 1;
    for (j = 0; j < cols; j = j + 1) cnt_paths[0][j] = 1;
    for (i = 1; i < rows; i = i + 1) {
        for (j = 1; j < cols; j = j + 1) {
            cnt_paths[i][j] = cnt_paths[i - 1][j] + cnt_paths[i][j - 1];
        }
    }
    return cnt_paths[rows - 1][cols - 1];
}

// =============================================================================
// Main test function
// =============================================================================
int main(void) {
    int test_num;
    int v;
    int i;

    test_num = 0;

    // ---- Fibonacci tests ----
    fib_init();
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, fib(0)); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, fib(1)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, fib(5)); // 5

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, fib(10)); // 55

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, fib(15)); // 610

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, fib(20)); // 6765

    // Iterative fib
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, fib_iter(10)); // 55

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, fib_iter(20)); // 6765

    // ---- Knapsack tests ----
    knap_w[0] = 2; knap_v[0] = 3;
    knap_w[1] = 3; knap_v[1] = 4;
    knap_w[2] = 4; knap_v[2] = 5;
    knap_w[3] = 5; knap_v[3] = 6;

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, knapsack(4, 5)); // 7

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, knapsack(4, 10)); // 12

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, knapsack(4, 2)); // 3

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, knapsack(4, 0)); // 0

    knap_w[0] = 1; knap_v[0] = 6;
    knap_w[1] = 2; knap_v[1] = 10;
    knap_w[2] = 3; knap_v[2] = 12;

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, knapsack(3, 5)); // 22

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, knapsack(3, 3)); // 16

    // ---- LCS tests ----
    lcs_a[0] = 65; lcs_a[1] = 66; lcs_a[2] = 67; lcs_a[3] = 66;
    lcs_a[4] = 68; lcs_a[5] = 65; lcs_a[6] = 66;
    lcs_b[0] = 66; lcs_b[1] = 68; lcs_b[2] = 67; lcs_b[3] = 65; lcs_b[4] = 66;

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, lcs(7, 5)); // 4

    lcs_a[0] = 1; lcs_a[1] = 2; lcs_a[2] = 3;
    lcs_b[0] = 1; lcs_b[1] = 2; lcs_b[2] = 3;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, lcs(3, 3)); // 3

    lcs_a[0] = 1; lcs_a[1] = 2;
    lcs_b[0] = 3; lcs_b[1] = 4;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, lcs(2, 2)); // 0

    lcs_a[0] = 1; lcs_a[1] = 3; lcs_a[2] = 5;
    lcs_b[0] = 2; lcs_b[1] = 3; lcs_b[2] = 4;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, lcs(3, 3)); // 1

    // ---- Coin change tests ----
    coins[0] = 1; coins[1] = 5; coins[2] = 10; coins[3] = 25;

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, coin_change(4, 30)); // 2

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, coin_change(4, 11)); // 2

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, coin_change(4, 0)); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, coin_change(4, 1)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, coin_change(4, 6)); // 2

    coins[0] = 3; coins[1] = 7;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, coin_change(2, 2)); // -1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, coin_change(2, 10)); // 2

    coins[0] = 1; coins[1] = 2; coins[2] = 5;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, coin_ways(3, 5)); // 4

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, coin_ways(3, 10)); // 10

    // ---- Edit distance tests ----
    ed_a[0] = 107; ed_a[1] = 105; ed_a[2] = 116; ed_a[3] = 116; ed_a[4] = 101; ed_a[5] = 110;
    ed_b[0] = 115; ed_b[1] = 105; ed_b[2] = 116; ed_b[3] = 116; ed_b[4] = 105; ed_b[5] = 110; ed_b[6] = 103;

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, edit_distance(6, 7)); // 3

    ed_a[0] = 1; ed_a[1] = 2; ed_a[2] = 3;
    ed_b[0] = 1; ed_b[1] = 2; ed_b[2] = 3;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, edit_distance(3, 3)); // 0

    ed_b[0] = 1; ed_b[1] = 2; ed_b[2] = 3;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, edit_distance(0, 3)); // 3

    ed_a[0] = 1; ed_a[1] = 2; ed_a[2] = 3;
    ed_b[0] = 4; ed_b[1] = 5; ed_b[2] = 6;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, edit_distance(3, 3)); // 3

    // ---- Maximum subarray sum ----
    kadane_arr[0] = -2; kadane_arr[1] = 1; kadane_arr[2] = -3;
    kadane_arr[3] = 4; kadane_arr[4] = -1; kadane_arr[5] = 2;
    kadane_arr[6] = 1; kadane_arr[7] = -5; kadane_arr[8] = 4;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, max_subarray_sum(9)); // 6

    kadane_arr[0] = 1; kadane_arr[1] = 2; kadane_arr[2] = 3;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, max_subarray_sum(3)); // 6

    kadane_arr[0] = -1; kadane_arr[1] = -2; kadane_arr[2] = -3;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, max_subarray_sum(3)); // -1

    // ---- LIS tests ----
    lis_arr[0] = 10; lis_arr[1] = 9; lis_arr[2] = 2;
    lis_arr[3] = 5; lis_arr[4] = 3; lis_arr[5] = 7;
    lis_arr[6] = 101; lis_arr[7] = 18;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, lis(8)); // 4

    lis_arr[0] = 1; lis_arr[1] = 2; lis_arr[2] = 3; lis_arr[3] = 4;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, lis(4)); // 4

    lis_arr[0] = 4; lis_arr[1] = 3; lis_arr[2] = 2; lis_arr[3] = 1;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, lis(4)); // 1

    // ---- Climbing stairs ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, climb_stairs(1)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, climb_stairs(2)); // 2

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, climb_stairs(5)); // 8

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, climb_stairs(10)); // 89

    // ---- House robber ----
    rob_arr[0] = 1; rob_arr[1] = 2; rob_arr[2] = 3; rob_arr[3] = 1;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, house_robber(4)); // 4

    rob_arr[0] = 2; rob_arr[1] = 7; rob_arr[2] = 9; rob_arr[3] = 3; rob_arr[4] = 1;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, house_robber(5)); // 12

    rob_arr[0] = 5;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, house_robber(1)); // 5

    // ---- Min cost path ----
    grid[0][0] = 1; grid[0][1] = 3; grid[0][2] = 1;
    grid[1][0] = 1; grid[1][1] = 5; grid[1][2] = 1;
    grid[2][0] = 4; grid[2][1] = 2; grid[2][2] = 1;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, min_cost_path(3, 3)); // 7

    grid[0][0] = 1; grid[0][1] = 2;
    grid[1][0] = 3; grid[1][1] = 4;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, min_cost_path(2, 2)); // 7

    // ---- Count paths ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_paths(3, 3)); // 6

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_paths(2, 2)); // 2

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_paths(4, 4)); // 20

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_paths(1, 1)); // 1

    // ---- Additional Fibonacci values ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, fib(7)); // 13

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, fib(12)); // 144

    return 0;
}
