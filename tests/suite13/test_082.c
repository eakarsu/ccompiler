int printf(const char *fmt, ...);

// Longest Increasing Subsequence (LIS) - O(n^2) DP approach

int lis_length(int *arr, int n) {
    int dp[64];
    int i;
    int j;
    int max_len;

    for (i = 0; i < n; i++) {
        dp[i] = 1;
    }

    for (i = 1; i < n; i++) {
        for (j = 0; j < i; j++) {
            if (arr[j] < arr[i]) {
                if (dp[j] + 1 > dp[i]) {
                    dp[i] = dp[j] + 1;
                }
            }
        }
    }

    max_len = 0;
    for (i = 0; i < n; i++) {
        if (dp[i] > max_len) {
            max_len = dp[i];
        }
    }
    return max_len;
}

// Also reconstruct the LIS itself, storing result in out[], return length
int lis_reconstruct(int *arr, int n, int *out) {
    int dp[64];
    int parent[64];
    int i;
    int j;
    int max_len;
    int max_idx;
    int len;
    int temp[64];

    for (i = 0; i < n; i++) {
        dp[i] = 1;
        parent[i] = -1;
    }

    for (i = 1; i < n; i++) {
        for (j = 0; j < i; j++) {
            if (arr[j] < arr[i] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
                parent[i] = j;
            }
        }
    }

    max_len = 0;
    max_idx = 0;
    for (i = 0; i < n; i++) {
        if (dp[i] > max_len) {
            max_len = dp[i];
            max_idx = i;
        }
    }

    // Trace back
    len = max_len;
    i = max_idx;
    j = len - 1;
    while (i != -1) {
        temp[j] = arr[i];
        j = j - 1;
        i = parent[i];
    }

    for (i = 0; i < len; i++) {
        out[i] = temp[i];
    }

    return len;
}

// Longest non-decreasing subsequence (allows equal)
int lnds_length(int *arr, int n) {
    int dp[64];
    int i;
    int j;
    int max_len;

    for (i = 0; i < n; i++) {
        dp[i] = 1;
    }

    for (i = 1; i < n; i++) {
        for (j = 0; j < i; j++) {
            if (arr[j] <= arr[i]) {
                if (dp[j] + 1 > dp[i]) {
                    dp[i] = dp[j] + 1;
                }
            }
        }
    }

    max_len = 0;
    for (i = 0; i < n; i++) {
        if (dp[i] > max_len) {
            max_len = dp[i];
        }
    }
    return max_len;
}

// Number of LIS of given length
int count_lis(int *arr, int n) {
    int dp[64];
    int cnt[64];
    int i;
    int j;
    int max_len;
    int total;

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
    }

    max_len = 0;
    for (i = 0; i < n; i++) {
        if (dp[i] > max_len) max_len = dp[i];
    }

    total = 0;
    for (i = 0; i < n; i++) {
        if (dp[i] == max_len) total = total + cnt[i];
    }
    return total;
}

int main() {
    int arr1[8];
    int arr2[10];
    int arr3[6];
    int arr4[7];
    int out[64];
    int len;
    int i;

    // arr1 = {10, 9, 2, 5, 3, 7, 101, 18}
    arr1[0] = 10; arr1[1] = 9; arr1[2] = 2; arr1[3] = 5;
    arr1[4] = 3; arr1[5] = 7; arr1[6] = 101; arr1[7] = 18;

    printf("%d\n", lis_length(arr1, 8));
    // EXPECT: 4

    // Reconstruct: one valid LIS is 2,5,7,101 or 2,5,7,18 or 2,3,7,101 etc
    len = lis_reconstruct(arr1, 8, out);
    printf("%d\n", len);
    // EXPECT: 4
    // The reconstruction follows leftmost-longest path: 2,5,7,101
    printf("%d %d %d %d\n", out[0], out[1], out[2], out[3]);
    // EXPECT: 2 5 7 101

    // arr2 = {0, 1, 0, 3, 2, 3}
    arr2[0] = 0; arr2[1] = 1; arr2[2] = 0; arr2[3] = 3;
    arr2[4] = 2; arr2[5] = 3;
    printf("%d\n", lis_length(arr2, 6));
    // EXPECT: 4

    // arr3 = {7, 7, 7, 7, 7}
    arr3[0] = 7; arr3[1] = 7; arr3[2] = 7; arr3[3] = 7; arr3[4] = 7;
    printf("%d\n", lis_length(arr3, 5));
    // EXPECT: 1

    // Non-decreasing on same array allows all 5
    printf("%d\n", lnds_length(arr3, 5));
    // EXPECT: 5

    // Sorted ascending: {1,2,3,4,5,6}
    arr3[0] = 1; arr3[1] = 2; arr3[2] = 3;
    arr3[3] = 4; arr3[4] = 5; arr3[5] = 6;
    printf("%d\n", lis_length(arr3, 6));
    // EXPECT: 6

    // Sorted descending: {6,5,4,3,2,1}
    arr3[0] = 6; arr3[1] = 5; arr3[2] = 4;
    arr3[3] = 3; arr3[4] = 2; arr3[5] = 1;
    printf("%d\n", lis_length(arr3, 6));
    // EXPECT: 1

    // Count LIS: {1,3,5,4,7}
    // LIS length=4: sequences: 1,3,5,7 and 1,3,4,7 => 2
    arr4[0] = 1; arr4[1] = 3; arr4[2] = 5; arr4[3] = 4; arr4[4] = 7;
    printf("%d\n", count_lis(arr4, 5));
    // EXPECT: 2

    // Count LIS: {2,2,2} => length 1, count=3
    arr3[0] = 2; arr3[1] = 2; arr3[2] = 2;
    printf("%d\n", count_lis(arr3, 3));
    // EXPECT: 3

    // Single element
    arr3[0] = 42;
    printf("%d\n", lis_length(arr3, 1));
    // EXPECT: 1

    // Empty
    printf("%d\n", lis_length(arr3, 0));
    // EXPECT: 0

    printf("done\n");
    // EXPECT: done

    return 0;
}
