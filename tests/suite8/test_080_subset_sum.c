int printf(const char *fmt, ...);

// Subset Sum Problem

// Check if there exists a subset with the given sum
int subset_sum(int *arr, int n, int target) {
    int dp[201];
    int i;
    int j;

    for (j = 0; j <= target; j++) {
        dp[j] = 0;
    }
    dp[0] = 1;

    for (i = 0; i < n; i++) {
        for (j = target; j >= arr[i]; j--) {
            if (dp[j - arr[i]]) {
                dp[j] = 1;
            }
        }
    }

    return dp[target];
}

// Count the number of subsets with the given sum
int count_subsets(int *arr, int n, int target) {
    int dp[201];
    int i;
    int j;

    for (j = 0; j <= target; j++) {
        dp[j] = 0;
    }
    dp[0] = 1;

    for (i = 0; i < n; i++) {
        for (j = target; j >= arr[i]; j--) {
            dp[j] = dp[j] + dp[j - arr[i]];
        }
    }

    return dp[target];
}

// Recursive subset sum (for verification with small inputs)
int subset_sum_rec(int *arr, int n, int target) {
    if (target == 0) return 1;
    if (n == 0) return 0;
    // If last element > target, skip it
    if (arr[n - 1] > target) {
        return subset_sum_rec(arr, n - 1, target);
    }
    // Include or exclude last element
    int inc = subset_sum_rec(arr, n - 1, target - arr[n - 1]);
    int exc = subset_sum_rec(arr, n - 1, target);
    return (inc || exc) ? 1 : 0;
}

// Find the actual subset that sums to target
// Returns count of elements in the subset, fills 'result' array
int find_subset(int *arr, int n, int target, int *result) {
    int dp[201];
    int last_idx[201];
    int i;
    int j;

    for (j = 0; j <= target; j++) {
        dp[j] = 0;
        last_idx[j] = -1;
    }
    dp[0] = 1;

    for (i = 0; i < n; i++) {
        for (j = target; j >= arr[i]; j--) {
            if (dp[j - arr[i]] && dp[j] == 0) {
                dp[j] = 1;
                last_idx[j] = i;
            }
        }
    }

    if (dp[target] == 0) return 0;

    // Trace back
    int count = 0;
    j = target;
    while (j > 0 && last_idx[j] >= 0) {
        result[count] = arr[last_idx[j]];
        j = j - arr[last_idx[j]];
        count = count + 1;
    }
    return count;
}

// Find the maximum sum subset not exceeding target
int max_subset_sum(int *arr, int n, int target) {
    int dp[201];
    int i;
    int j;

    for (j = 0; j <= target; j++) {
        dp[j] = 0;
    }
    dp[0] = 1;

    for (i = 0; i < n; i++) {
        for (j = target; j >= arr[i]; j--) {
            if (dp[j - arr[i]]) {
                dp[j] = 1;
            }
        }
    }

    for (j = target; j >= 0; j--) {
        if (dp[j]) return j;
    }
    return 0;
}

// Count subsets with sum in range [lo, hi]
int count_in_range(int *arr, int n, int lo, int hi) {
    int dp[201];
    int i;
    int j;

    for (j = 0; j <= hi; j++) {
        dp[j] = 0;
    }
    dp[0] = 1;

    for (i = 0; i < n; i++) {
        for (j = hi; j >= arr[i]; j--) {
            dp[j] = dp[j] + dp[j - arr[i]];
        }
    }

    int count = 0;
    for (j = lo; j <= hi; j++) {
        count = count + dp[j];
    }
    return count;
}

// Check if there are exactly k elements that sum to target
int subset_sum_k(int *arr, int n, int target, int k) {
    // dp[j][c] = number of ways to pick c elements summing to j
    // Flat: dp[j * (k+1) + c], max 200 * 11 = 2200
    int dp[2200];
    int i;
    int j;
    int c;

    for (j = 0; j <= target; j++) {
        for (c = 0; c <= k; c++) {
            dp[j * (k + 1) + c] = 0;
        }
    }
    dp[0] = 1;  // dp[0][0] = 1

    for (i = 0; i < n; i++) {
        for (j = target; j >= arr[i]; j--) {
            for (c = k; c >= 1; c--) {
                dp[j * (k + 1) + c] = dp[j * (k + 1) + c]
                    + dp[(j - arr[i]) * (k + 1) + (c - 1)];
            }
        }
    }

    return dp[target * (k + 1) + k] > 0 ? 1 : 0;
}

int main() {
    int a1[6];
    a1[0] = 3; a1[1] = 34; a1[2] = 4; a1[3] = 12; a1[4] = 5; a1[5] = 2;

    // EXPECT: ss(9) = 1
    printf("ss(9) = %d\n", subset_sum(a1, 6, 9));
    // EXPECT: ss(30) = 0
    printf("ss(30) = %d\n", subset_sum(a1, 6, 30));
    // EXPECT: ss(7) = 1
    printf("ss(7) = %d\n", subset_sum(a1, 6, 7));
    // EXPECT: ss(0) = 1
    printf("ss(0) = %d\n", subset_sum(a1, 6, 0));

    // Verify recursive matches DP
    // EXPECT: rec(9) = 1
    printf("rec(9) = %d\n", subset_sum_rec(a1, 6, 9));
    // EXPECT: rec(30) = 0
    printf("rec(30) = %d\n", subset_sum_rec(a1, 6, 30));

    // Count subsets
    int a2[5];
    a2[0] = 1; a2[1] = 2; a2[2] = 3; a2[3] = 4; a2[4] = 5;
    // EXPECT: count(5) = 3
    printf("count(5) = %d\n", count_subsets(a2, 5, 5));
    // EXPECT: count(10) = 3
    printf("count(10) = %d\n", count_subsets(a2, 5, 10));
    // EXPECT: count(15) = 1
    printf("count(15) = %d\n", count_subsets(a2, 5, 15));

    int a3[4];
    a3[0] = 1; a3[1] = 1; a3[2] = 1; a3[3] = 1;
    // EXPECT: count_ones(2) = 6
    printf("count_ones(2) = %d\n", count_subsets(a3, 4, 2));

    // Find actual subset
    int result[10];
    int cnt;
    cnt = find_subset(a1, 6, 9, result);
    // EXPECT: found_cnt = 2
    printf("found_cnt = %d\n", cnt);
    // Verify the found subset sums to target
    int sum = 0;
    int i;
    for (i = 0; i < cnt; i++) {
        sum = sum + result[i];
    }
    // EXPECT: found_sum = 9
    printf("found_sum = %d\n", sum);

    // Max subset sum not exceeding target
    // EXPECT: max_ss(10) = 10
    printf("max_ss(10) = %d\n", max_subset_sum(a1, 6, 10));
    // EXPECT: max_ss(20) = 20
    printf("max_ss(20) = %d\n", max_subset_sum(a1, 6, 20));
    // EXPECT: max_ss(8) = 8
    printf("max_ss(8) = %d\n", max_subset_sum(a1, 6, 8));

    // Count subsets in range
    // EXPECT: range(3,7) = 13
    printf("range(3,7) = %d\n", count_in_range(a2, 5, 3, 7));
    // EXPECT: range(1,15) = 31
    printf("range(1,15) = %d\n", count_in_range(a2, 5, 1, 15));

    // Subset sum with exactly k elements
    // EXPECT: ss_k(5,2) = 1
    printf("ss_k(5,2) = %d\n", subset_sum_k(a2, 5, 5, 2));
    // EXPECT: ss_k(5,3) = 0
    printf("ss_k(5,3) = %d\n", subset_sum_k(a2, 5, 5, 3));
    // EXPECT: ss_k(5,1) = 1
    printf("ss_k(5,1) = %d\n", subset_sum_k(a2, 5, 5, 1));
    // EXPECT: ss_k(5,4) = 0
    printf("ss_k(5,4) = %d\n", subset_sum_k(a2, 5, 5, 4));
    // EXPECT: ss_k(15,5) = 1
    printf("ss_k(15,5) = %d\n", subset_sum_k(a2, 5, 15, 5));

    // Edge cases
    int a4[1];
    a4[0] = 7;
    // EXPECT: single(7) = 1
    printf("single(7) = %d\n", subset_sum(a4, 1, 7));
    // EXPECT: single(3) = 0
    printf("single(3) = %d\n", subset_sum(a4, 1, 3));

    return 0;
}
