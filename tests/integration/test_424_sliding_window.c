int printf(const char *fmt, ...);
// EXPECT: Window max (k=3): 3 3 5 5 6 7\nWindow max (k=4): 3 5 5 6 7\nMax sum k=3: 9\nMax sum k=2: 6\nLongest unique 'abcabcbb': 3\nLongest unique 'bbbbb': 1\nLongest unique 'pwwkew': 3\nMin window >= 7: 2
// Test: Sliding window maximum

void sliding_window_max(int arr[], int n, int k) {
    int i, j;
    printf("Window max (k=%d):", k);
    for (i = 0; i <= n - k; i++) {
        int mx = arr[i];
        for (j = 1; j < k; j++) {
            if (arr[i + j] > mx) mx = arr[i + j];
        }
        printf(" %d", mx);
    }
    printf("\n");
}

int max_sum_subarray(int arr[], int n, int k) {
    int sum = 0;
    int i;
    for (i = 0; i < k; i++) {
        sum = sum + arr[i];
    }
    int max_sum = sum;
    for (i = k; i < n; i++) {
        sum = sum + arr[i] - arr[i - k];
        if (sum > max_sum) max_sum = sum;
    }
    return max_sum;
}

int longest_unique_substr(char *s) {
    int n = 0;
    while (s[n] != 0) n++;

    int count[128];
    int i;
    for (i = 0; i < 128; i++) count[i] = 0;

    int left = 0;
    int max_len = 0;
    for (i = 0; i < n; i++) {
        count[(int)s[i]]++;
        while (count[(int)s[i]] > 1) {
            count[(int)s[left]]--;
            left++;
        }
        int len = i - left + 1;
        if (len > max_len) max_len = len;
    }
    return max_len;
}

int min_window_sum_ge(int arr[], int n, int target) {
    int left = 0;
    int sum = 0;
    int min_len = n + 1;
    int i;
    for (i = 0; i < n; i++) {
        sum = sum + arr[i];
        while (sum >= target) {
            int len = i - left + 1;
            if (len < min_len) min_len = len;
            sum = sum - arr[left];
            left++;
        }
    }
    if (min_len > n) return 0;
    return min_len;
}

int main(void) {
    int a[] = {1, 3, -1, -3, 5, 3, 6, 7};
    sliding_window_max(a, 8, 3);
    sliding_window_max(a, 8, 4);

    int b[] = {2, 1, 5, 1, 3, 2};
    printf("Max sum k=3: %d\n", max_sum_subarray(b, 6, 3));
    printf("Max sum k=2: %d\n", max_sum_subarray(b, 6, 2));

    char s1[] = "abcabcbb";
    printf("Longest unique '%s': %d\n", s1, longest_unique_substr(s1));
    char s2[] = "bbbbb";
    printf("Longest unique '%s': %d\n", s2, longest_unique_substr(s2));
    char s3[] = "pwwkew";
    printf("Longest unique '%s': %d\n", s3, longest_unique_substr(s3));

    int c[] = {2, 3, 1, 2, 4, 3};
    printf("Min window >= 7: %d\n", min_window_sum_ge(c, 6, 7));

    return 0;
}
