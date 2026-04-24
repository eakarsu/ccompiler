int printf(const char *fmt, ...);
// EXPECT: === LIS tests ===\nLIS [10,9,2,5,3,7,101,18] = 4\nLIS [0,1,0,3,2,3] = 4\nLIS [7,7,7,7] = 1\nLIS count [1,3,5,4,7] = 2\n=== LDS test ===\nLDS [10,9,2,5,3,7] = 4\n=== Bitonic ===\nBitonic [1,11,2,10,4,5,2,1] = 6\n=== Rod cutting ===\nRod cut [1,5,8,9,10,17,17,20] len=8: 22\nRod cut [2,5,7,8] len=4: 10\nRod cut with cost=1 [1,5,8,9,10,17,17,20] len=8: 21\nRod cut trace [1,5,8,9,10,17,17,20] len=8: pieces=2\n=== MCM tests ===\nMCM [10,30,5,60] = 4500\nMCM [40,20,30,10,30] = 26000\nMCM [10,20,30,40] = 18000\n=== Palindrome partitioning ===\nMinCuts [1,2,1] = 0\nMinCuts [1,2,3,2,1] = 0\nMinCuts [1,2,3,4] = 3\nMinCuts [1,2,1,2,1] = 0\nMinCuts [1,2,3,1,2] = 4\n=== Palindromic subsequences ===\nPalSubseq [1,2,1] = 5\nPalSubseq [1,2,3,2,1] = 13\n=== LCS tests ===\nLCS [1,3,4,1,2] [1,2,3,4] = 3\nLCS [1,2,3] [4,5,6] = 0\n=== Edit distance ===\nED [1,2,3] [1,3,2] = 2\nED [1,2,3,4] [1,2,3,4] = 0\n=== Knapsack ===\nKS w=[1,2,3] v=[6,10,12] cap=5: 22\nKS w=[2,3,4,5] v=[3,4,5,6] cap=5: 7\n=== Min coins ===\nMinCoins [1,5,10,25] amount=30: 2\nMinCoins [1,3,4] amount=6: 2\n=== Coin ways ===\nCoinWays [1,2,5] amount=5: 4\n=== Kadane ===\nMaxSub [-2,1,-3,4,-1,2,1,-5,4] = 6\n=== Grid paths with obstacles ===\nPaths 3x3: 2\n=== Min path sum ===\nMinPath 3x3: 7\n=== Egg drop ===\nEggDrop(2,10) = 4\nEggDrop(2,6) = 3\n=== Longest palindromic subseq ===\nLPS [1,2,3,2,1,4] = 5\nAll DP advanced tests passed!
// ============================================================
// test_076_dp_advanced.c
// Longest increasing subsequence, rod cutting, matrix chain
// (variant), palindrome partitioning (small).
// ============================================================

// ---- Max of two ints ----
int max_val(int a, int b) {
    return (a > b) ? a : b;
}

// ---- Min of two ints ----
int min_val(int a, int b) {
    return (a < b) ? a : b;
}

// ============================================================
// LONGEST INCREASING SUBSEQUENCE (LIS)
// ============================================================

// ---- LIS using O(n^2) DP ----
int lis_dp(int arr[], int n) {
    int dp[100];
    int i, j;
    int max_len = 1;

    for (i = 0; i < n; i++) dp[i] = 1;

    for (i = 1; i < n; i++) {
        for (j = 0; j < i; j++) {
            if (arr[j] < arr[i] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
            }
        }
        if (dp[i] > max_len) max_len = dp[i];
    }
    return max_len;
}

// ---- Count number of LIS ----
int count_lis(int arr[], int n) {
    int dp[100];
    int cnt[100];
    int i, j;
    int max_len = 1;

    for (i = 0; i < n; i++) {
        dp[i] = 1;
        cnt[i] = 1;
    }

    for (i = 1; i < n; i++) {
        for (j = 0; j < i; j++) {
            if (arr[j] < arr[i]) {
                if (dp[j] + 1 > dp[i]) {
                    dp[i] = dp[j] + 1;
                    cnt[i] = cnt[j];
                } else if (dp[j] + 1 == dp[i]) {
                    cnt[i] = cnt[i] + cnt[j];
                }
            }
        }
        if (dp[i] > max_len) max_len = dp[i];
    }

    int total = 0;
    for (i = 0; i < n; i++) {
        if (dp[i] == max_len) total = total + cnt[i];
    }
    return total;
}

// ---- Longest Decreasing Subsequence ----
int lds(int arr[], int n) {
    int dp[100];
    int i, j;
    int max_len = 1;

    for (i = 0; i < n; i++) dp[i] = 1;

    for (i = 1; i < n; i++) {
        for (j = 0; j < i; j++) {
            if (arr[j] > arr[i] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
            }
        }
        if (dp[i] > max_len) max_len = dp[i];
    }
    return max_len;
}

// ---- Longest Bitonic Subsequence ----
// LIS from left + LDS from right - 1
int longest_bitonic(int arr[], int n) {
    int lis_arr[100], lds_arr[100];
    int i, j;

    // Compute LIS ending at each position
    for (i = 0; i < n; i++) lis_arr[i] = 1;
    for (i = 1; i < n; i++) {
        for (j = 0; j < i; j++) {
            if (arr[j] < arr[i] && lis_arr[j] + 1 > lis_arr[i]) {
                lis_arr[i] = lis_arr[j] + 1;
            }
        }
    }

    // Compute LDS starting at each position (= LIS from right)
    for (i = 0; i < n; i++) lds_arr[i] = 1;
    for (i = n - 2; i >= 0; i--) {
        for (j = n - 1; j > i; j--) {
            if (arr[j] < arr[i] && lds_arr[j] + 1 > lds_arr[i]) {
                lds_arr[i] = lds_arr[j] + 1;
            }
        }
    }

    int max_len = 0;
    for (i = 0; i < n; i++) {
        int len = lis_arr[i] + lds_arr[i] - 1;
        if (len > max_len) max_len = len;
    }
    return max_len;
}

// ============================================================
// ROD CUTTING
// ============================================================

// ---- Rod cutting: maximize revenue ----
// prices[i] = price for rod of length i+1
int rod_cutting(int prices[], int n) {
    int dp[100];
    int i, j;
    dp[0] = 0;

    for (i = 1; i <= n; i++) {
        dp[i] = 0;
        for (j = 1; j <= i; j++) {
            int val = prices[j - 1] + dp[i - j];
            if (val > dp[i]) dp[i] = val;
        }
    }
    return dp[n];
}

// ---- Rod cutting with cost: each cut costs c ----
int rod_cutting_with_cost(int prices[], int n, int cut_cost) {
    int dp[100];
    int i, j;
    dp[0] = 0;

    for (i = 1; i <= n; i++) {
        dp[i] = prices[i - 1]; // No cut: use whole piece
        for (j = 1; j < i; j++) {
            int val = prices[j - 1] + dp[i - j] - cut_cost;
            if (val > dp[i]) dp[i] = val;
        }
    }
    return dp[n];
}

// ---- Rod cutting: return the cuts made ----
// Fills cuts[] with the lengths of each piece, returns number of pieces
int rod_cutting_trace(int prices[], int n, int cuts[]) {
    int dp[100];
    int choice[100]; // choice[i] = length of first piece for rod of length i
    int i, j;
    dp[0] = 0;

    for (i = 1; i <= n; i++) {
        dp[i] = 0;
        choice[i] = 0;
        for (j = 1; j <= i; j++) {
            int val = prices[j - 1] + dp[i - j];
            if (val > dp[i]) {
                dp[i] = val;
                choice[i] = j;
            }
        }
    }

    // Trace back
    int num_cuts = 0;
    int remaining = n;
    while (remaining > 0) {
        cuts[num_cuts] = choice[remaining];
        remaining = remaining - choice[remaining];
        num_cuts++;
    }
    return num_cuts;
}

// ============================================================
// MATRIX CHAIN MULTIPLICATION
// ============================================================

// ---- MCM: minimum multiplications ----
// dims[0..n] such that matrix i has dimensions dims[i] x dims[i+1]
int mcm(int dims[], int n) {
    // n matrices, dims has n+1 elements
    int dp[20][20];
    int i, j, k, L;

    for (i = 0; i < n; i++) dp[i][i] = 0;

    for (L = 2; L <= n; L++) {
        for (i = 0; i <= n - L; i++) {
            j = i + L - 1;
            dp[i][j] = 999999999;
            for (k = i; k < j; k++) {
                int cost = dp[i][k] + dp[k + 1][j] +
                           dims[i] * dims[k + 1] * dims[j + 1];
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                }
            }
        }
    }
    return dp[0][n - 1];
}

// ---- MCM with parenthesization trace ----
// Returns minimum cost and fills split[][] for reconstruction
int mcm_trace(int dims[], int n, int split[20][20]) {
    int dp[20][20];
    int i, j, k, L;

    for (i = 0; i < n; i++) dp[i][i] = 0;

    for (L = 2; L <= n; L++) {
        for (i = 0; i <= n - L; i++) {
            j = i + L - 1;
            dp[i][j] = 999999999;
            for (k = i; k < j; k++) {
                int cost = dp[i][k] + dp[k + 1][j] +
                           dims[i] * dims[k + 1] * dims[j + 1];
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                    split[i][j] = k;
                }
            }
        }
    }
    return dp[0][n - 1];
}

// ============================================================
// PALINDROME PARTITIONING
// ============================================================

// ---- Check if subarray is palindrome (using int array) ----
int is_palindrome(int arr[], int start, int end) {
    while (start < end) {
        if (arr[start] != arr[end]) return 0;
        start++;
        end--;
    }
    return 1;
}

// ---- Minimum palindrome partitions ----
// Given array of ints, find min cuts so each partition is palindrome
int min_palindrome_cuts(int arr[], int n) {
    int dp[50]; // dp[i] = min cuts for arr[0..i]
    int pal[50][50]; // pal[i][j] = 1 if arr[i..j] is palindrome
    int i, j, L;

    // Precompute palindrome table
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            pal[i][j] = 0;
        }
        pal[i][i] = 1;
    }

    for (L = 2; L <= n; L++) {
        for (i = 0; i <= n - L; i++) {
            j = i + L - 1;
            if (L == 2) {
                pal[i][j] = (arr[i] == arr[j]) ? 1 : 0;
            } else {
                pal[i][j] = (arr[i] == arr[j] && pal[i + 1][j - 1]) ? 1 : 0;
            }
        }
    }

    for (i = 0; i < n; i++) {
        if (pal[0][i]) {
            dp[i] = 0;
        } else {
            dp[i] = i; // max cuts
            for (j = 1; j <= i; j++) {
                if (pal[j][i] && dp[j - 1] + 1 < dp[i]) {
                    dp[i] = dp[j - 1] + 1;
                }
            }
        }
    }
    return dp[n - 1];
}

// ---- Count palindromic subsequences ----
// Not partition but subsequences
int count_palindromic_subseq(int arr[], int n) {
    int dp[30][30];
    int i, j, L;

    for (i = 0; i < 30; i++)
        for (j = 0; j < 30; j++)
            dp[i][j] = 0;

    for (i = 0; i < n; i++) {
        dp[i][i] = 1;
    }

    for (L = 2; L <= n; L++) {
        for (i = 0; i <= n - L; i++) {
            j = i + L - 1;
            if (arr[i] == arr[j]) {
                dp[i][j] = dp[i + 1][j] + dp[i][j - 1] + 1;
            } else {
                dp[i][j] = dp[i + 1][j] + dp[i][j - 1] - dp[i + 1][j - 1];
            }
        }
    }
    return dp[0][n - 1];
}

// ============================================================
// OTHER DP PROBLEMS
// ============================================================

// ---- Longest Common Subsequence ----
int lcs(int a[], int na, int b[], int nb) {
    int dp[30][30];
    int i, j;

    for (i = 0; i <= na; i++) dp[i][0] = 0;
    for (j = 0; j <= nb; j++) dp[0][j] = 0;

    for (i = 1; i <= na; i++) {
        for (j = 1; j <= nb; j++) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max_val(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[na][nb];
}

// ---- Edit Distance ----
int edit_distance(int a[], int na, int b[], int nb) {
    int dp[30][30];
    int i, j;

    for (i = 0; i <= na; i++) dp[i][0] = i;
    for (j = 0; j <= nb; j++) dp[0][j] = j;

    for (i = 1; i <= na; i++) {
        for (j = 1; j <= nb; j++) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                int m = dp[i - 1][j - 1]; // replace
                if (dp[i - 1][j] < m) m = dp[i - 1][j]; // delete
                if (dp[i][j - 1] < m) m = dp[i][j - 1]; // insert
                dp[i][j] = m + 1;
            }
        }
    }
    return dp[na][nb];
}

// ---- 0/1 Knapsack ----
int knapsack01(int weights[], int values[], int n, int capacity) {
    int dp[50][200];
    int i, w;

    for (i = 0; i <= n; i++) {
        for (w = 0; w <= capacity; w++) {
            dp[i][w] = 0;
        }
    }

    for (i = 1; i <= n; i++) {
        for (w = 0; w <= capacity; w++) {
            dp[i][w] = dp[i - 1][w]; // don't take item i
            if (weights[i - 1] <= w) {
                int take = dp[i - 1][w - weights[i - 1]] + values[i - 1];
                if (take > dp[i][w]) dp[i][w] = take;
            }
        }
    }
    return dp[n][capacity];
}

// ---- Coin change: minimum coins ----
int min_coins(int coins[], int nc, int amount) {
    int dp[500];
    int i, j;
    dp[0] = 0;
    for (i = 1; i <= amount; i++) dp[i] = 999999;

    for (i = 1; i <= amount; i++) {
        for (j = 0; j < nc; j++) {
            if (coins[j] <= i && dp[i - coins[j]] + 1 < dp[i]) {
                dp[i] = dp[i - coins[j]] + 1;
            }
        }
    }
    return (dp[amount] >= 999999) ? -1 : dp[amount];
}

// ---- Coin change: count ways ----
int count_coin_ways(int coins[], int nc, int amount) {
    int dp[500];
    int i, j;
    for (i = 0; i <= amount; i++) dp[i] = 0;
    dp[0] = 1;

    for (j = 0; j < nc; j++) {
        for (i = coins[j]; i <= amount; i++) {
            dp[i] = dp[i] + dp[i - coins[j]];
        }
    }
    return dp[amount];
}

// ---- Maximum sum subarray (Kadane's) ----
int max_subarray_sum(int arr[], int n) {
    int max_so_far = arr[0];
    int max_ending = arr[0];
    int i;
    for (i = 1; i < n; i++) {
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

// ---- Number of ways to reach (m,n) from (0,0) with obstacles ----
int unique_paths_obstacles(int grid[10][10], int m, int n) {
    int dp[10][10];
    int i, j;

    if (grid[0][0] == 1) return 0;

    dp[0][0] = 1;
    for (i = 1; i < m; i++) {
        dp[i][0] = (grid[i][0] == 1) ? 0 : dp[i - 1][0];
    }
    for (j = 1; j < n; j++) {
        dp[0][j] = (grid[0][j] == 1) ? 0 : dp[0][j - 1];
    }

    for (i = 1; i < m; i++) {
        for (j = 1; j < n; j++) {
            if (grid[i][j] == 1) {
                dp[i][j] = 0;
            } else {
                dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
            }
        }
    }
    return dp[m - 1][n - 1];
}

// ---- Minimum path sum in grid ----
int min_path_sum(int grid[10][10], int m, int n) {
    int dp[10][10];
    int i, j;

    dp[0][0] = grid[0][0];
    for (i = 1; i < m; i++) dp[i][0] = dp[i - 1][0] + grid[i][0];
    for (j = 1; j < n; j++) dp[0][j] = dp[0][j - 1] + grid[0][j];

    for (i = 1; i < m; i++) {
        for (j = 1; j < n; j++) {
            dp[i][j] = min_val(dp[i - 1][j], dp[i][j - 1]) + grid[i][j];
        }
    }
    return dp[m - 1][n - 1];
}

// ---- Egg drop problem ----
// k eggs, n floors
int egg_drop(int k, int n) {
    int dp[10][100];
    int i, j, x;

    for (i = 0; i <= k; i++) {
        dp[i][0] = 0;
        dp[i][1] = 1;
    }
    for (j = 0; j <= n; j++) {
        dp[0][j] = 0;
        dp[1][j] = j;
    }

    for (i = 2; i <= k; i++) {
        for (j = 2; j <= n; j++) {
            dp[i][j] = 999999;
            for (x = 1; x <= j; x++) {
                int worst = 1 + max_val(dp[i - 1][x - 1], dp[i][j - x]);
                if (worst < dp[i][j]) dp[i][j] = worst;
            }
        }
    }
    return dp[k][n];
}

// ---- Longest palindromic subsequence ----
int longest_palindromic_subseq(int arr[], int n) {
    int dp[30][30];
    int i, j, L;

    for (i = 0; i < n; i++) dp[i][i] = 1;

    for (L = 2; L <= n; L++) {
        for (i = 0; i <= n - L; i++) {
            j = i + L - 1;
            if (arr[i] == arr[j]) {
                dp[i][j] = dp[i + 1][j - 1] + 2;
            } else {
                dp[i][j] = max_val(dp[i + 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[0][n - 1];
}

int main() {
    // === LIS tests ===
    printf("=== LIS tests ===\n");
    {
        int arr[8];
        arr[0]=10; arr[1]=9; arr[2]=2; arr[3]=5;
        arr[4]=3; arr[5]=7; arr[6]=101; arr[7]=18;
        printf("LIS [10,9,2,5,3,7,101,18] = %d\n", lis_dp(arr, 8));
    }
    {
        int arr[6];
        arr[0]=0; arr[1]=1; arr[2]=0; arr[3]=3; arr[4]=2; arr[5]=3;
        printf("LIS [0,1,0,3,2,3] = %d\n", lis_dp(arr, 6));
    }
    {
        int arr[4];
        arr[0]=7; arr[1]=7; arr[2]=7; arr[3]=7;
        printf("LIS [7,7,7,7] = %d\n", lis_dp(arr, 4));
    }
    {
        int arr[5];
        arr[0]=1; arr[1]=3; arr[2]=5; arr[3]=4; arr[4]=7;
        printf("LIS count [1,3,5,4,7] = %d\n", count_lis(arr, 5));
    }

    // === LDS ===
    printf("=== LDS test ===\n");
    {
        int arr[6];
        arr[0]=10; arr[1]=9; arr[2]=2; arr[3]=5; arr[4]=3; arr[5]=7;
        printf("LDS [10,9,2,5,3,7] = %d\n", lds(arr, 6));
    }

    // === Bitonic ===
    printf("=== Bitonic ===\n");
    {
        int arr[8];
        arr[0]=1; arr[1]=11; arr[2]=2; arr[3]=10;
        arr[4]=4; arr[5]=5; arr[6]=2; arr[7]=1;
        printf("Bitonic [1,11,2,10,4,5,2,1] = %d\n", longest_bitonic(arr, 8));
    }

    // === Rod cutting ===
    printf("=== Rod cutting ===\n");
    {
        int prices[8];
        prices[0]=1; prices[1]=5; prices[2]=8; prices[3]=9;
        prices[4]=10; prices[5]=17; prices[6]=17; prices[7]=20;
        printf("Rod cut [1,5,8,9,10,17,17,20] len=8: %d\n", rod_cutting(prices, 8));
    }
    {
        int prices[4];
        prices[0]=2; prices[1]=5; prices[2]=7; prices[3]=8;
        printf("Rod cut [2,5,7,8] len=4: %d\n", rod_cutting(prices, 4));
    }
    {
        int prices[8];
        prices[0]=1; prices[1]=5; prices[2]=8; prices[3]=9;
        prices[4]=10; prices[5]=17; prices[6]=17; prices[7]=20;
        printf("Rod cut with cost=1 [1,5,8,9,10,17,17,20] len=8: %d\n",
               rod_cutting_with_cost(prices, 8, 1));
    }
    {
        int prices[8];
        prices[0]=1; prices[1]=5; prices[2]=8; prices[3]=9;
        prices[4]=10; prices[5]=17; prices[6]=17; prices[7]=20;
        int cuts[20];
        int nc = rod_cutting_trace(prices, 8, cuts);
        printf("Rod cut trace [1,5,8,9,10,17,17,20] len=8: pieces=%d\n", nc);
    }

    // === MCM ===
    printf("=== MCM tests ===\n");
    {
        int dims[4];
        dims[0]=10; dims[1]=30; dims[2]=5; dims[3]=60;
        printf("MCM [10,30,5,60] = %d\n", mcm(dims, 3));
    }
    {
        int dims[5];
        dims[0]=40; dims[1]=20; dims[2]=30; dims[3]=10; dims[4]=30;
        printf("MCM [40,20,30,10,30] = %d\n", mcm(dims, 4));
    }
    {
        int dims[4];
        dims[0]=10; dims[1]=20; dims[2]=30; dims[3]=40;
        printf("MCM [10,20,30,40] = %d\n", mcm(dims, 3));
    }

    // === Palindrome partitioning ===
    printf("=== Palindrome partitioning ===\n");
    {
        int a[3]; a[0]=1; a[1]=2; a[2]=1;
        printf("MinCuts [1,2,1] = %d\n", min_palindrome_cuts(a, 3));
    }
    {
        int a[5]; a[0]=1; a[1]=2; a[2]=3; a[3]=2; a[4]=1;
        printf("MinCuts [1,2,3,2,1] = %d\n", min_palindrome_cuts(a, 5));
    }
    {
        int a[4]; a[0]=1; a[1]=2; a[2]=3; a[3]=4;
        printf("MinCuts [1,2,3,4] = %d\n", min_palindrome_cuts(a, 4));
    }
    {
        int a[5]; a[0]=1; a[1]=2; a[2]=1; a[3]=2; a[4]=1;
        printf("MinCuts [1,2,1,2,1] = %d\n", min_palindrome_cuts(a, 5));
    }
    {
        int a[5]; a[0]=1; a[1]=2; a[2]=3; a[3]=1; a[4]=2;
        printf("MinCuts [1,2,3,1,2] = %d\n", min_palindrome_cuts(a, 5));
    }

    // === Palindromic subsequences ===
    printf("=== Palindromic subsequences ===\n");
    {
        int a[3]; a[0]=1; a[1]=2; a[2]=1;
        printf("PalSubseq [1,2,1] = %d\n", count_palindromic_subseq(a, 3));
    }
    {
        int a[5]; a[0]=1; a[1]=2; a[2]=3; a[3]=2; a[4]=1;
        printf("PalSubseq [1,2,3,2,1] = %d\n", count_palindromic_subseq(a, 5));
    }

    // === LCS ===
    printf("=== LCS tests ===\n");
    {
        int a[5]; a[0]=1; a[1]=3; a[2]=4; a[3]=1; a[4]=2;
        int b[4]; b[0]=1; b[1]=2; b[2]=3; b[3]=4;
        printf("LCS [1,3,4,1,2] [1,2,3,4] = %d\n", lcs(a, 5, b, 4));
    }
    {
        int a[3]; a[0]=1; a[1]=2; a[2]=3;
        int b[3]; b[0]=4; b[1]=5; b[2]=6;
        printf("LCS [1,2,3] [4,5,6] = %d\n", lcs(a, 3, b, 3));
    }

    // === Edit distance ===
    printf("=== Edit distance ===\n");
    {
        int a[3]; a[0]=1; a[1]=2; a[2]=3;
        int b[3]; b[0]=1; b[1]=3; b[2]=2;
        printf("ED [1,2,3] [1,3,2] = %d\n", edit_distance(a, 3, b, 3));
    }
    {
        int a[4]; a[0]=1; a[1]=2; a[2]=3; a[3]=4;
        int b[4]; b[0]=1; b[1]=2; b[2]=3; b[3]=4;
        printf("ED [1,2,3,4] [1,2,3,4] = %d\n", edit_distance(a, 4, b, 4));
    }

    // === Knapsack ===
    printf("=== Knapsack ===\n");
    {
        int w[3]; w[0]=1; w[1]=2; w[2]=3;
        int v[3]; v[0]=6; v[1]=10; v[2]=12;
        printf("KS w=[1,2,3] v=[6,10,12] cap=5: %d\n", knapsack01(w, v, 3, 5));
    }
    {
        int w[4]; w[0]=2; w[1]=3; w[2]=4; w[3]=5;
        int v[4]; v[0]=3; v[1]=4; v[2]=5; v[3]=6;
        printf("KS w=[2,3,4,5] v=[3,4,5,6] cap=5: %d\n", knapsack01(w, v, 4, 5));
    }

    // === Min coins ===
    printf("=== Min coins ===\n");
    {
        int c[4]; c[0]=1; c[1]=5; c[2]=10; c[3]=25;
        printf("MinCoins [1,5,10,25] amount=30: %d\n", min_coins(c, 4, 30));
    }
    {
        int c[3]; c[0]=1; c[1]=3; c[2]=4;
        printf("MinCoins [1,3,4] amount=6: %d\n", min_coins(c, 3, 6));
    }

    // === Coin ways ===
    printf("=== Coin ways ===\n");
    {
        int c[3]; c[0]=1; c[1]=2; c[2]=5;
        printf("CoinWays [1,2,5] amount=5: %d\n", count_coin_ways(c, 3, 5));
    }

    // === Kadane ===
    printf("=== Kadane ===\n");
    {
        int a[9]; a[0]=-2; a[1]=1; a[2]=-3; a[3]=4; a[4]=-1;
        a[5]=2; a[6]=1; a[7]=-5; a[8]=4;
        printf("MaxSub [-2,1,-3,4,-1,2,1,-5,4] = %d\n", max_subarray_sum(a, 9));
    }

    // === Grid paths with obstacles ===
    printf("=== Grid paths with obstacles ===\n");
    {
        int grid[10][10];
        int i, j;
        for (i = 0; i < 3; i++)
            for (j = 0; j < 3; j++)
                grid[i][j] = 0;
        grid[1][1] = 1; // obstacle
        printf("Paths 3x3: %d\n", unique_paths_obstacles(grid, 3, 3));
    }

    // === Min path sum ===
    printf("=== Min path sum ===\n");
    {
        int grid[10][10];
        grid[0][0]=1; grid[0][1]=3; grid[0][2]=1;
        grid[1][0]=1; grid[1][1]=5; grid[1][2]=1;
        grid[2][0]=4; grid[2][1]=2; grid[2][2]=1;
        printf("MinPath 3x3: %d\n", min_path_sum(grid, 3, 3));
    }

    // === Egg drop ===
    printf("=== Egg drop ===\n");
    printf("EggDrop(2,10) = %d\n", egg_drop(2, 10));
    printf("EggDrop(2,6) = %d\n", egg_drop(2, 6));

    // === Longest palindromic subsequence ===
    printf("=== Longest palindromic subseq ===\n");
    {
        int a[6]; a[0]=1; a[1]=2; a[2]=3; a[3]=2; a[4]=1; a[5]=4;
        printf("LPS [1,2,3,2,1,4] = %d\n", longest_palindromic_subseq(a, 6));
    }

    printf("All DP advanced tests passed!\n");
    return 0;
}
