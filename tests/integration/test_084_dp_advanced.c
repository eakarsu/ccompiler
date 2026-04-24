int printf(const char *fmt, ...);
// EXPECT: 10 22 8 10 4 8 4 6 3 1 5 5 1 1 0 1 9 7 0 2 2 -1 2 4 4 5 1 4 1 3 5 0 6 10 -1 8 89 987 4 12 110 7 7 5 6 28 20 5 1 5

// DP: rod cutting, egg drop, longest increasing subsequence, partition equal subset sum

// Rod cutting: given prices for lengths 1..n, find max revenue for rod of length n
int rod_prices[20];
int rod_dp[20];

int rod_cutting(int n) {
    int i;
    int j;
    int mx;
    rod_dp[0] = 0;
    for (i = 1; i <= n; i = i + 1) {
        mx = -1;
        for (j = 1; j <= i; j = j + 1) {
            if (rod_prices[j] + rod_dp[i - j] > mx) {
                mx = rod_prices[j] + rod_dp[i - j];
            }
        }
        rod_dp[i] = mx;
    }
    return rod_dp[n];
}

// Egg drop problem: given e eggs and f floors, minimum trials in worst case
int egg_dp[10][50];

int egg_drop(int eggs, int floors) {
    int i;
    int j;
    int k;
    int res;
    int low;
    int high;
    int mid;
    int when_breaks;
    int when_survives;
    int worse;
    int best;

    for (i = 0; i <= eggs; i = i + 1) {
        for (j = 0; j <= floors; j = j + 1) {
            egg_dp[i][j] = 0;
        }
    }
    // Base cases
    for (j = 0; j <= floors; j = j + 1) {
        egg_dp[1][j] = j; // 1 egg, j floors: j trials
    }
    for (i = 0; i <= eggs; i = i + 1) {
        egg_dp[i][0] = 0;
        egg_dp[i][1] = 1;
    }

    for (i = 2; i <= eggs; i = i + 1) {
        for (j = 2; j <= floors; j = j + 1) {
            best = j; // worst case can't be more than j
            for (k = 1; k <= j; k = k + 1) {
                when_breaks = egg_dp[i - 1][k - 1];
                when_survives = egg_dp[i][j - k];
                if (when_breaks > when_survives) {
                    worse = when_breaks;
                } else {
                    worse = when_survives;
                }
                res = worse + 1;
                if (res < best) best = res;
            }
            egg_dp[i][j] = best;
        }
    }
    return egg_dp[eggs][floors];
}

// Longest Increasing Subsequence length
int lis_dp[50];

int lis_length(int arr[], int n) {
    int i;
    int j;
    int mx;
    for (i = 0; i < n; i = i + 1) {
        lis_dp[i] = 1;
    }
    for (i = 1; i < n; i = i + 1) {
        for (j = 0; j < i; j = j + 1) {
            if (arr[j] < arr[i] && lis_dp[j] + 1 > lis_dp[i]) {
                lis_dp[i] = lis_dp[j] + 1;
            }
        }
    }
    mx = 0;
    for (i = 0; i < n; i = i + 1) {
        if (lis_dp[i] > mx) mx = lis_dp[i];
    }
    return mx;
}

// Longest Decreasing Subsequence
int lds_length(int arr[], int n) {
    int i;
    int j;
    int mx;
    for (i = 0; i < n; i = i + 1) {
        lis_dp[i] = 1;
    }
    for (i = 1; i < n; i = i + 1) {
        for (j = 0; j < i; j = j + 1) {
            if (arr[j] > arr[i] && lis_dp[j] + 1 > lis_dp[i]) {
                lis_dp[i] = lis_dp[j] + 1;
            }
        }
    }
    mx = 0;
    for (i = 0; i < n; i = i + 1) {
        if (lis_dp[i] > mx) mx = lis_dp[i];
    }
    return mx;
}

// Partition Equal Subset Sum: can array be split into two subsets with equal sum?
int part_dp[2][500];

int can_partition(int arr[], int n) {
    int total;
    int half;
    int i;
    int j;
    int cur;
    int prev;
    total = 0;
    for (i = 0; i < n; i = i + 1) {
        total = total + arr[i];
    }
    if (total % 2 != 0) return 0;
    half = total / 2;

    // dp[j] = 1 if sum j achievable
    cur = 0;
    prev = 1;
    for (j = 0; j <= half; j = j + 1) {
        part_dp[cur][j] = 0;
        part_dp[prev][j] = 0;
    }
    part_dp[cur][0] = 1;
    part_dp[prev][0] = 1;

    for (i = 0; i < n; i = i + 1) {
        prev = cur;
        cur = 1 - cur;
        for (j = 0; j <= half; j = j + 1) {
            part_dp[cur][j] = part_dp[prev][j];
        }
        for (j = arr[i]; j <= half; j = j + 1) {
            if (part_dp[prev][j - arr[i]]) {
                part_dp[cur][j] = 1;
            }
        }
    }
    return part_dp[cur][half];
}

// 0/1 Knapsack
int knapsack_dp[2][200];

int knapsack(int weights[], int values[], int n, int capacity) {
    int i;
    int j;
    int cur;
    int prev;
    cur = 0;
    prev = 1;
    for (j = 0; j <= capacity; j = j + 1) {
        knapsack_dp[0][j] = 0;
        knapsack_dp[1][j] = 0;
    }
    for (i = 0; i < n; i = i + 1) {
        prev = cur;
        cur = 1 - cur;
        for (j = 0; j <= capacity; j = j + 1) {
            knapsack_dp[cur][j] = knapsack_dp[prev][j];
            if (j >= weights[i]) {
                if (knapsack_dp[prev][j - weights[i]] + values[i] > knapsack_dp[cur][j]) {
                    knapsack_dp[cur][j] = knapsack_dp[prev][j - weights[i]] + values[i];
                }
            }
        }
    }
    return knapsack_dp[cur][capacity];
}

// Coin change: minimum coins to make amount
int coin_dp[200];

int coin_change(int coins[], int nc, int amount) {
    int i;
    int j;
    for (i = 0; i <= amount; i = i + 1) {
        coin_dp[i] = 9999;
    }
    coin_dp[0] = 0;
    for (i = 0; i < nc; i = i + 1) {
        for (j = coins[i]; j <= amount; j = j + 1) {
            if (coin_dp[j - coins[i]] + 1 < coin_dp[j]) {
                coin_dp[j] = coin_dp[j - coins[i]] + 1;
            }
        }
    }
    if (coin_dp[amount] == 9999) return -1;
    return coin_dp[amount];
}

// Number of ways to make coin change
int coin_ways(int coins[], int nc, int amount) {
    int ways[200];
    int i;
    int j;
    for (i = 0; i <= amount; i = i + 1) {
        ways[i] = 0;
    }
    ways[0] = 1;
    for (i = 0; i < nc; i = i + 1) {
        for (j = coins[i]; j <= amount; j = j + 1) {
            ways[j] = ways[j] + ways[j - coins[i]];
        }
    }
    return ways[amount];
}

// Edit distance between two integer arrays
int edit_dp[30][30];

int edit_distance(int a[], int na, int b[], int nb) {
    int i;
    int j;
    int cost;
    int mn;
    for (i = 0; i <= na; i = i + 1) edit_dp[i][0] = i;
    for (j = 0; j <= nb; j = j + 1) edit_dp[0][j] = j;
    for (i = 1; i <= na; i = i + 1) {
        for (j = 1; j <= nb; j = j + 1) {
            if (a[i - 1] == b[j - 1]) {
                cost = 0;
            } else {
                cost = 1;
            }
            mn = edit_dp[i - 1][j] + 1;
            if (edit_dp[i][j - 1] + 1 < mn) mn = edit_dp[i][j - 1] + 1;
            if (edit_dp[i - 1][j - 1] + cost < mn) mn = edit_dp[i - 1][j - 1] + cost;
            edit_dp[i][j] = mn;
        }
    }
    return edit_dp[na][nb];
}

// Longest Common Subsequence
int lcs_dp[30][30];

int lcs_length(int a[], int na, int b[], int nb) {
    int i;
    int j;
    for (i = 0; i <= na; i = i + 1) lcs_dp[i][0] = 0;
    for (j = 0; j <= nb; j = j + 1) lcs_dp[0][j] = 0;
    for (i = 1; i <= na; i = i + 1) {
        for (j = 1; j <= nb; j = j + 1) {
            if (a[i - 1] == b[j - 1]) {
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
    return lcs_dp[na][nb];
}

// Maximum subarray sum (Kadane)
int max_subarray(int arr[], int n) {
    int max_ending;
    int max_so_far;
    int i;
    max_ending = arr[0];
    max_so_far = arr[0];
    for (i = 1; i < n; i = i + 1) {
        if (max_ending + arr[i] > arr[i]) {
            max_ending = max_ending + arr[i];
        } else {
            max_ending = arr[i];
        }
        if (max_ending > max_so_far) {
            max_so_far = max_ending;
        }
    }
    return max_so_far;
}

// Climbing stairs: n ways to climb n stairs (1 or 2 steps)
int climb_stairs(int n) {
    int dp[50];
    int i;
    dp[0] = 1;
    dp[1] = 1;
    for (i = 2; i <= n; i = i + 1) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    return dp[n];
}

// House robber: max sum of non-adjacent elements
int house_robber(int arr[], int n) {
    int dp[50];
    int i;
    if (n == 0) return 0;
    if (n == 1) return arr[0];
    dp[0] = arr[0];
    if (arr[1] > arr[0]) {
        dp[1] = arr[1];
    } else {
        dp[1] = arr[0];
    }
    for (i = 2; i < n; i = i + 1) {
        if (dp[i - 1] > dp[i - 2] + arr[i]) {
            dp[i] = dp[i - 1];
        } else {
            dp[i] = dp[i - 2] + arr[i];
        }
    }
    return dp[n - 1];
}

// Minimum path sum in grid
int grid[10][10];
int path_dp[10][10];

int min_path_sum(int rows, int cols) {
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

// Unique paths in grid (no obstacles)
int unique_paths(int m, int n) {
    int dp[10][10];
    int i;
    int j;
    for (i = 0; i < m; i = i + 1) dp[i][0] = 1;
    for (j = 0; j < n; j = j + 1) dp[0][j] = 1;
    for (i = 1; i < m; i = i + 1) {
        for (j = 1; j < n; j = j + 1) {
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
        }
    }
    return dp[m - 1][n - 1];
}

// Longest palindromic subsequence length
int lps_dp[20][20];

int lps_length(int arr[], int n) {
    int i;
    int j;
    int len;
    for (i = 0; i < n; i = i + 1) {
        lps_dp[i][i] = 1;
    }
    for (len = 2; len <= n; len = len + 1) {
        for (i = 0; i <= n - len; i = i + 1) {
            j = i + len - 1;
            if (arr[i] == arr[j]) {
                if (len == 2) {
                    lps_dp[i][j] = 2;
                } else {
                    lps_dp[i][j] = lps_dp[i + 1][j - 1] + 2;
                }
            } else {
                if (lps_dp[i + 1][j] > lps_dp[i][j - 1]) {
                    lps_dp[i][j] = lps_dp[i + 1][j];
                } else {
                    lps_dp[i][j] = lps_dp[i][j - 1];
                }
            }
        }
    }
    return lps_dp[0][n - 1];
}

int main(void) {
    int arr[20];
    int arr2[20];
    int coins[10];
    int weights[10];
    int values[10];
    int i;

    // Test 1-3: Rod cutting
    // Prices: len 1=1, 2=5, 3=8, 4=9, 5=10, 6=17, 7=17, 8=20
    rod_prices[1]=1; rod_prices[2]=5; rod_prices[3]=8; rod_prices[4]=9;
    rod_prices[5]=10; rod_prices[6]=17; rod_prices[7]=17; rod_prices[8]=20;
    printf("%d", rod_cutting(4));  // max(1+dp[3], 5+dp[2], 8+dp[1], 9) = max(1+8,5+5,8+1,9) = max(9,10,9,9) = 10
    printf(" %d", rod_cutting(8)); // 22
    printf(" %d", rod_cutting(3)); // 8

    // Test 4-7: Egg drop
    printf(" %d", egg_drop(1, 10)); // 10
    printf(" %d", egg_drop(2, 10)); // 4
    printf(" %d", egg_drop(2, 36)); // 8
    printf(" %d", egg_drop(3, 14)); // 4

    // Test 8-11: LIS
    arr[0]=10; arr[1]=22; arr[2]=9; arr[3]=33; arr[4]=21;
    arr[5]=50; arr[6]=41; arr[7]=60; arr[8]=80;
    printf(" %d", lis_length(arr, 9)); // 6: 10,22,33,50,60,80

    arr[0]=3; arr[1]=10; arr[2]=2; arr[3]=1; arr[4]=20;
    printf(" %d", lis_length(arr, 5)); // 3: 3,10,20

    arr[0]=5; arr[1]=4; arr[2]=3; arr[3]=2; arr[4]=1;
    printf(" %d", lis_length(arr, 5)); // 1

    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    printf(" %d", lis_length(arr, 5)); // 5

    // Test 12-13: LDS
    arr[0]=5; arr[1]=4; arr[2]=3; arr[3]=2; arr[4]=1;
    printf(" %d", lds_length(arr, 5)); // 5

    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    printf(" %d", lds_length(arr, 5)); // 1

    // Test 14-16: Partition equal subset
    arr[0]=1; arr[1]=5; arr[2]=11; arr[3]=5;
    printf(" %d", can_partition(arr, 4)); // 1 (1+5+5=11)

    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=5;
    printf(" %d", can_partition(arr, 4)); // 0 (sum=11, odd)

    arr[0]=3; arr[1]=3; arr[2]=3; arr[3]=3;
    printf(" %d", can_partition(arr, 4)); // 1 (6=6)

    // Test 17-19: Knapsack
    weights[0]=1; weights[1]=3; weights[2]=4; weights[3]=5;
    values[0]=1;  values[1]=4;  values[2]=5;  values[3]=7;
    printf(" %d", knapsack(weights, values, 4, 7)); // 9 (3+4 -> 4+5)

    weights[0]=2; weights[1]=3; weights[2]=4; weights[3]=5;
    values[0]=3;  values[1]=4;  values[2]=5;  values[3]=6;
    printf(" %d", knapsack(weights, values, 4, 5)); // 7 (2+3 -> 3+4)

    weights[0]=10;
    values[0]=100;
    printf(" %d", knapsack(weights, values, 1, 5)); // 0 (can't fit)

    // Test 20-23: Coin change min
    coins[0]=1; coins[1]=5; coins[2]=10; coins[3]=25;
    printf(" %d", coin_change(coins, 4, 30)); // 2 (25+5)
    printf(" %d", coin_change(coins, 4, 11)); // 2 (10+1)

    coins[0]=2;
    printf(" %d", coin_change(coins, 1, 3)); // -1

    coins[0]=1; coins[1]=3; coins[2]=4;
    printf(" %d", coin_change(coins, 3, 6)); // 2 (3+3)

    // Test 24-26: Coin ways
    coins[0]=1; coins[1]=2; coins[2]=5;
    printf(" %d", coin_ways(coins, 3, 5)); // 4: (5),(2+2+1),(2+1+1+1),(1*5)

    coins[0]=1; coins[1]=2; coins[2]=3;
    printf(" %d", coin_ways(coins, 3, 4)); // 4: (1111),(112),(22),(13)

    coins[0]=2; coins[1]=5; coins[2]=3; coins[3]=6;
    printf(" %d", coin_ways(coins, 4, 10)); // 5

    // Test 27-29: Edit distance
    arr[0]=1; arr[1]=2; arr[2]=3;
    arr2[0]=1; arr2[1]=3; arr2[2]=3;
    printf(" %d", edit_distance(arr, 3, arr2, 3)); // 1

    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4;
    arr2[0]=5; arr2[1]=6; arr2[2]=7; arr2[3]=8;
    printf(" %d", edit_distance(arr, 4, arr2, 4)); // 4

    arr[0]=1; arr[1]=2;
    arr2[0]=1; arr2[1]=2; arr2[2]=3;
    printf(" %d", edit_distance(arr, 2, arr2, 3)); // 1

    // Test 30-32: LCS
    arr[0]=1; arr[1]=3; arr[2]=4; arr[3]=1;
    arr2[0]=1; arr2[1]=4; arr2[2]=1; arr2[3]=3;
    printf(" %d", lcs_length(arr, 4, arr2, 4)); // 2 (1,4 or 1,1 or 1,3)

    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    arr2[0]=1; arr2[1]=2; arr2[2]=3; arr2[3]=4; arr2[4]=5;
    printf(" %d", lcs_length(arr, 5, arr2, 5)); // 5

    arr[0]=1; arr[1]=2; arr[2]=3;
    arr2[0]=4; arr2[1]=5; arr2[2]=6;
    printf(" %d", lcs_length(arr, 3, arr2, 3)); // 0

    // Test 33-35: Max subarray
    arr[0]=-2; arr[1]=1; arr[2]=-3; arr[3]=4; arr[4]=-1;
    arr[5]=2; arr[6]=1; arr[7]=-5; arr[8]=4;
    printf(" %d", max_subarray(arr, 9)); // 6 (4,-1,2,1)

    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4;
    printf(" %d", max_subarray(arr, 4)); // 10

    arr[0]=-1; arr[1]=-2; arr[2]=-3;
    printf(" %d", max_subarray(arr, 3)); // -1

    // Test 36-38: Climbing stairs
    printf(" %d", climb_stairs(5));  // 8
    printf(" %d", climb_stairs(10)); // 89
    printf(" %d", climb_stairs(15)); // 987

    // Test 39-41: House robber
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=1;
    printf(" %d", house_robber(arr, 4)); // 4 (1+3)

    arr[0]=2; arr[1]=7; arr[2]=9; arr[3]=3; arr[4]=1;
    printf(" %d", house_robber(arr, 5)); // 12 (2+9+1)

    arr[0]=5; arr[1]=5; arr[2]=10; arr[3]=100; arr[4]=10; arr[5]=5;
    printf(" %d", house_robber(arr, 6)); // 110 (5+100+5)

    // Test 42-44: Min path sum
    grid[0][0]=1; grid[0][1]=3; grid[0][2]=1;
    grid[1][0]=1; grid[1][1]=5; grid[1][2]=1;
    grid[2][0]=4; grid[2][1]=2; grid[2][2]=1;
    printf(" %d", min_path_sum(3, 3)); // 7 (1->3->1->1->1)

    grid[0][0]=1; grid[0][1]=2;
    grid[1][0]=3; grid[1][1]=4;
    printf(" %d", min_path_sum(2, 2)); // 7 (1->2->4)

    grid[0][0]=5;
    printf(" %d", min_path_sum(1, 1)); // 5

    // Test 45-47: Unique paths
    printf(" %d", unique_paths(3, 3)); // 6
    printf(" %d", unique_paths(3, 7)); // 28
    printf(" %d", unique_paths(4, 4)); // 20

    // Test 48-50: Longest palindromic subsequence
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=2; arr[4]=1;
    printf(" %d", lps_length(arr, 5)); // 5 (whole thing)

    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    printf(" %d", lps_length(arr, 5)); // 1

    arr[0]=1; arr[1]=3; arr[2]=5; arr[3]=3; arr[4]=1; arr[5]=7;
    printf(" %d", lps_length(arr, 6)); // 5 (1,3,5,3,1)

    printf("\n");
    return 0;
}
