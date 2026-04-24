int printf(const char *fmt, ...);

// Partition Problem: can a set be divided into two subsets with equal sum?

// Check if array can be partitioned into two subsets with equal sum
int can_partition(int *arr, int n) {
    int total = 0;
    int i;
    int j;
    for (i = 0; i < n; i++) {
        total = total + arr[i];
    }
    // If total is odd, can't partition equally
    if (total % 2 != 0) return 0;

    int target = total / 2;
    // dp[j] = 1 if sum j is achievable
    int dp[201];
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

// Find the minimum difference between two subset sums
int min_partition_diff(int *arr, int n) {
    int total = 0;
    int i;
    int j;
    for (i = 0; i < n; i++) {
        total = total + arr[i];
    }

    int half = total / 2;
    int dp[201];
    for (j = 0; j <= half; j++) {
        dp[j] = 0;
    }
    dp[0] = 1;

    for (i = 0; i < n; i++) {
        for (j = half; j >= arr[i]; j--) {
            if (dp[j - arr[i]]) {
                dp[j] = 1;
            }
        }
    }

    // Find largest j <= half where dp[j] is true
    int best = 0;
    for (j = half; j >= 0; j--) {
        if (dp[j]) {
            best = j;
            break;
        }
    }

    return total - 2 * best;
}

// Count number of ways to partition into two equal-sum subsets
int count_partitions(int *arr, int n) {
    int total = 0;
    int i;
    int j;
    for (i = 0; i < n; i++) {
        total = total + arr[i];
    }
    if (total % 2 != 0) return 0;

    int target = total / 2;
    int dp[201];
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

// 3-way partition: can the array be split into 3 equal-sum subsets?
int can_3partition(int *arr, int n) {
    int total = 0;
    int i;
    for (i = 0; i < n; i++) {
        total = total + arr[i];
    }
    if (total % 3 != 0) return 0;

    int target = total / 3;

    // Greedy approach: find first subset summing to target
    // then find second, remaining is third
    int used[20];
    for (i = 0; i < n; i++) used[i] = 0;

    // Find first subset with sum = target
    int found = 0;
    int sum = 0;
    for (i = 0; i < n; i++) {
        if (sum + arr[i] <= target) {
            sum = sum + arr[i];
            used[i] = 1;
            if (sum == target) {
                found = 1;
                break;
            }
        }
    }
    if (found == 0) return 0;

    // Find second subset with sum = target
    sum = 0;
    found = 0;
    for (i = 0; i < n; i++) {
        if (used[i] == 0 && sum + arr[i] <= target) {
            sum = sum + arr[i];
            used[i] = 1;
            if (sum == target) {
                found = 1;
                break;
            }
        }
    }
    if (found == 0) return 0;

    // Verify remaining sums to target
    sum = 0;
    for (i = 0; i < n; i++) {
        if (used[i] == 0) {
            sum = sum + arr[i];
        }
    }

    return sum == target ? 1 : 0;
}

// Find actual partition assignment (0 or 1 for each element)
void find_partition(int *arr, int n, int *assignment) {
    int total = 0;
    int i;
    int j;
    for (i = 0; i < n; i++) {
        total = total + arr[i];
        assignment[i] = 0;
    }

    int target = total / 2;
    if (total % 2 != 0) return;

    // dp with tracking
    int dp[201];
    int last[201];  // last[j] = index of last item added to reach sum j
    for (j = 0; j <= target; j++) {
        dp[j] = 0;
        last[j] = -1;
    }
    dp[0] = 1;

    for (i = 0; i < n; i++) {
        for (j = target; j >= arr[i]; j--) {
            if (dp[j - arr[i]] && dp[j] == 0) {
                dp[j] = 1;
                last[j] = i;
            }
        }
    }

    // Trace back
    j = target;
    while (j > 0 && last[j] >= 0) {
        assignment[last[j]] = 1;
        j = j - arr[last[j]];
    }
}

int main() {
    // Test basic partition
    int a1[4];
    a1[0] = 1; a1[1] = 5; a1[2] = 11; a1[3] = 5;
    // EXPECT: partition1 = 1
    printf("partition1 = %d\n", can_partition(a1, 4));

    int a2[3];
    a2[0] = 1; a2[1] = 2; a2[2] = 3;
    // EXPECT: partition2 = 1
    printf("partition2 = %d\n", can_partition(a2, 3));

    int a3[3];
    a3[0] = 1; a3[1] = 2; a3[2] = 5;
    // EXPECT: partition3 = 0
    printf("partition3 = %d\n", can_partition(a3, 3));

    int a4[6];
    a4[0] = 3; a4[1] = 1; a4[2] = 1; a4[3] = 2; a4[4] = 2; a4[5] = 1;
    // EXPECT: partition4 = 1
    printf("partition4 = %d\n", can_partition(a4, 6));

    // Test minimum partition difference
    // EXPECT: min_diff1 = 0
    printf("min_diff1 = %d\n", min_partition_diff(a1, 4));
    // EXPECT: min_diff2 = 0
    printf("min_diff2 = %d\n", min_partition_diff(a2, 3));
    // EXPECT: min_diff3 = 2
    printf("min_diff3 = %d\n", min_partition_diff(a3, 3));

    int a5[4];
    a5[0] = 1; a5[1] = 6; a5[2] = 11; a5[3] = 5;
    // EXPECT: min_diff5 = 1
    printf("min_diff5 = %d\n", min_partition_diff(a5, 4));

    // Test count partitions
    int a6[4];
    a6[0] = 1; a6[1] = 2; a6[2] = 3; a6[3] = 4;
    // EXPECT: count_part(1234) = 2
    printf("count_part(1234) = %d\n", count_partitions(a6, 4));

    int a7[5];
    a7[0] = 1; a7[1] = 1; a7[2] = 1; a7[3] = 1; a7[4] = 2;
    // EXPECT: count_part(11112) = 8
    printf("count_part(11112) = %d\n", count_partitions(a7, 5));

    // Test 3-way partition
    int a8[6];
    a8[0] = 1; a8[1] = 2; a8[2] = 3; a8[3] = 3; a8[4] = 2; a8[5] = 1;
    // EXPECT: 3part1 = 0
    printf("3part1 = %d\n", can_3partition(a8, 6));

    int a9[4];
    a9[0] = 1; a9[1] = 2; a9[2] = 3; a9[3] = 5;
    // EXPECT: 3part2 = 0
    printf("3part2 = %d\n", can_3partition(a9, 4));

    // Test find_partition assignment
    int assign[6];
    find_partition(a4, 6, assign);
    int sum0 = 0;
    int sum1 = 0;
    int i;
    for (i = 0; i < 6; i++) {
        if (assign[i] == 0) sum0 = sum0 + a4[i];
        else sum1 = sum1 + a4[i];
    }
    // EXPECT: assign_equal = 1
    printf("assign_equal = %d\n", sum0 == sum1 ? 1 : 0);

    // Edge cases
    int a10[1];
    a10[0] = 0;
    // EXPECT: single_zero = 1
    printf("single_zero = %d\n", can_partition(a10, 1));

    int a11[2];
    a11[0] = 5; a11[1] = 5;
    // EXPECT: pair_equal = 1
    printf("pair_equal = %d\n", can_partition(a11, 2));

    int a12[2];
    a12[0] = 3; a12[1] = 7;
    // EXPECT: pair_unequal = 0
    printf("pair_unequal = %d\n", can_partition(a12, 2));

    return 0;
}
