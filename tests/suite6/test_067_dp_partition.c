int printf(const char *fmt, ...);

int can_partition(int arr[], int n) {
    int total = 0;
    int i = 0;
    while (i < n) {
        total = total + arr[i];
        i = i + 1;
    }
    if (total % 2 != 0) return 0;
    int target = total / 2;

    int dp[200];
    i = 0;
    while (i <= target) {
        dp[i] = 0;
        i = i + 1;
    }
    dp[0] = 1;

    i = 0;
    while (i < n) {
        int j = target;
        while (j >= arr[i]) {
            if (dp[j - arr[i]]) dp[j] = 1;
            j = j - 1;
        }
        i = i + 1;
    }
    return dp[target];
}

int subset_sum(int arr[], int n, int target) {
    int dp[200];
    int i = 0;
    while (i <= target) {
        dp[i] = 0;
        i = i + 1;
    }
    dp[0] = 1;

    i = 0;
    while (i < n) {
        int j = target;
        while (j >= arr[i]) {
            if (dp[j - arr[i]]) dp[j] = 1;
            j = j - 1;
        }
        i = i + 1;
    }
    return dp[target];
}

int count_subsets_sum(int arr[], int n, int target) {
    int dp[200];
    int i = 0;
    while (i <= target) {
        dp[i] = 0;
        i = i + 1;
    }
    dp[0] = 1;

    i = 0;
    while (i < n) {
        int j = target;
        while (j >= arr[i]) {
            dp[j] = dp[j] + dp[j - arr[i]];
            j = j - 1;
        }
        i = i + 1;
    }
    return dp[target];
}

int min_diff_partition(int arr[], int n) {
    int total = 0;
    int i = 0;
    while (i < n) {
        total = total + arr[i];
        i = i + 1;
    }
    int half = total / 2;

    int dp[200];
    i = 0;
    while (i <= half) {
        dp[i] = 0;
        i = i + 1;
    }
    dp[0] = 1;

    i = 0;
    while (i < n) {
        int j = half;
        while (j >= arr[i]) {
            if (dp[j - arr[i]]) dp[j] = 1;
            j = j - 1;
        }
        i = i + 1;
    }

    int best = 0;
    i = 0;
    while (i <= half) {
        if (dp[i]) best = i;
        i = i + 1;
    }
    return total - 2 * best;
}

int partition_into_k(int arr[], int n, int k) {
    int total = 0;
    int i = 0;
    while (i < n) {
        total = total + arr[i];
        i = i + 1;
    }
    if (total % k != 0) return 0;
    return 1;
}

int max_subset_sum_no_adj(int arr[], int n) {
    if (n == 0) return 0;
    if (n == 1) return arr[0];
    int dp[60];
    dp[0] = arr[0];
    dp[1] = arr[0] > arr[1] ? arr[0] : arr[1];
    int i = 2;
    while (i < n) {
        int a = dp[i - 2] + arr[i];
        int b = dp[i - 1];
        dp[i] = a > b ? a : b;
        i = i + 1;
    }
    return dp[n - 1];
}

int main(void) {
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

    // EXPECT: subset(9) = 0
    printf("subset(9) = %d\n", subset_sum(a1, 4, 9));
    // EXPECT: subset(22) = 1
    printf("subset(22) = %d\n", subset_sum(a1, 4, 22));
    // EXPECT: subset(3) = 0
    printf("subset(3) = %d\n", subset_sum(a1, 4, 3));
    // EXPECT: subset(0) = 1
    printf("subset(0) = %d\n", subset_sum(a1, 4, 0));

    int b1[5];
    b1[0] = 1; b1[1] = 2; b1[2] = 3; b1[3] = 4; b1[4] = 5;
    // EXPECT: count_sub(5) = 3
    printf("count_sub(5) = %d\n", count_subsets_sum(b1, 5, 5));
    // EXPECT: count_sub(10) = 3
    printf("count_sub(10) = %d\n", count_subsets_sum(b1, 5, 10));

    // EXPECT: mindiff1 = 0
    printf("mindiff1 = %d\n", min_diff_partition(a1, 4));

    int c1[4];
    c1[0] = 1; c1[1] = 6; c1[2] = 11; c1[3] = 5;
    // EXPECT: mindiff2 = 1
    printf("mindiff2 = %d\n", min_diff_partition(c1, 4));

    int d1[3];
    d1[0] = 3; d1[1] = 1; d1[2] = 4;
    // EXPECT: mindiff3 = 0
    printf("mindiff3 = %d\n", min_diff_partition(d1, 3));

    int e1[4];
    e1[0] = 3; e1[1] = 3; e1[2] = 3; e1[3] = 3;
    // EXPECT: k_part(4) = 1
    printf("k_part(4) = %d\n", partition_into_k(e1, 4, 4));
    // EXPECT: k_part(3) = 1
    printf("k_part(3) = %d\n", partition_into_k(e1, 4, 3));
    // EXPECT: k_part(5) = 0
    printf("k_part(5) = %d\n", partition_into_k(e1, 4, 5));

    int f1[6];
    f1[0] = 2; f1[1] = 7; f1[2] = 9; f1[3] = 5; f1[4] = 1; f1[5] = 3;
    // EXPECT: no_adj = 15
    printf("no_adj = %d\n", max_subset_sum_no_adj(f1, 6));

    int f2[4];
    f2[0] = 3; f2[1] = 2; f2[2] = 5; f2[3] = 10;
    // EXPECT: no_adj2 = 13
    printf("no_adj2 = %d\n", max_subset_sum_no_adj(f2, 4));

    return 0;
}
