int printf(const char *fmt, ...);
// EXPECT: LIS length: 6\nLIS: 10 22 33 50 60 80\nLIS binary: 6\nLIS length: 3\nLIS: 3 10 20\nDecreasing LIS: 1\nIncreasing LIS: 5
// Test: Longest increasing subsequence

int lis_length(int arr[], int n) {
    int dp[64];
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

void print_lis(int arr[], int n) {
    int dp[64];
    int parent[64];
    int i, j;
    int max_len = 1;
    int max_idx = 0;

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
        if (dp[i] > max_len) {
            max_len = dp[i];
            max_idx = i;
        }
    }

    int seq[64];
    int len = 0;
    int idx = max_idx;
    while (idx != -1) {
        seq[len] = arr[idx];
        len++;
        idx = parent[idx];
    }

    for (i = len - 1; i >= 0; i--) {
        if (i < len - 1) printf(" ");
        printf("%d", seq[i]);
    }
    printf("\n");
}

int lis_binary(int arr[], int n) {
    int tails[64];
    int size = 0;
    int i, lo, hi, mid;

    for (i = 0; i < n; i++) {
        lo = 0;
        hi = size;
        while (lo < hi) {
            mid = (lo + hi) / 2;
            if (tails[mid] < arr[i]) lo = mid + 1;
            else hi = mid;
        }
        tails[lo] = arr[i];
        if (lo == size) size++;
    }
    return size;
}

int main(void) {
    int a[] = {10, 22, 9, 33, 21, 50, 41, 60, 80};
    int n = 9;
    printf("LIS length: %d\n", lis_length(a, n));
    printf("LIS: ");
    print_lis(a, n);
    printf("LIS binary: %d\n", lis_binary(a, n));

    int b[] = {3, 10, 2, 1, 20};
    printf("LIS length: %d\n", lis_length(b, 5));
    printf("LIS: ");
    print_lis(b, 5);

    int c[] = {5, 4, 3, 2, 1};
    printf("Decreasing LIS: %d\n", lis_length(c, 5));

    int d[] = {1, 2, 3, 4, 5};
    printf("Increasing LIS: %d\n", lis_length(d, 5));

    return 0;
}
