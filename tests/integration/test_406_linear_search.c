int printf(const char *fmt, ...);
// EXPECT: Linear 7: 2\nLinear 10: 9\nLinear 11: -1\nSentinel 7: 2\nSentinel 10: 9\nSentinel 11: -1\nFirst 5: 4\nLast 5: 10\nCount 5: 3\nCount 1: 2\nMin idx: 1
// Test: Linear search with sentinel

int linear_search(int arr[], int n, int target) {
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] == target) return i;
    }
    return -1;
}

int sentinel_search(int arr[], int n, int target) {
    int last = arr[n - 1];
    arr[n - 1] = target;
    int i = 0;
    while (arr[i] != target) {
        i++;
    }
    arr[n - 1] = last;
    if (i < n - 1 || arr[n - 1] == target) {
        return i;
    }
    return -1;
}

int find_first(int arr[], int n, int target) {
    int result = -1;
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] == target) {
            result = i;
            break;
        }
    }
    return result;
}

int find_last(int arr[], int n, int target) {
    int result = -1;
    int i;
    for (i = n - 1; i >= 0; i--) {
        if (arr[i] == target) {
            result = i;
            break;
        }
    }
    return result;
}

int count_all(int arr[], int n, int target) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] == target) count++;
    }
    return count;
}

int find_min_index(int arr[], int n) {
    int min_idx = 0;
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] < arr[min_idx]) min_idx = i;
    }
    return min_idx;
}

int main(void) {
    int a[] = {4, 2, 7, 1, 9, 3, 8, 5, 6, 10};
    int n = 10;

    printf("Linear 7: %d\n", linear_search(a, n, 7));
    printf("Linear 10: %d\n", linear_search(a, n, 10));
    printf("Linear 11: %d\n", linear_search(a, n, 11));

    printf("Sentinel 7: %d\n", sentinel_search(a, n, 7));
    printf("Sentinel 10: %d\n", sentinel_search(a, n, 10));
    printf("Sentinel 11: %d\n", sentinel_search(a, n, 11));

    int b[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    int m = 11;
    printf("First 5: %d\n", find_first(b, m, 5));
    printf("Last 5: %d\n", find_last(b, m, 5));
    printf("Count 5: %d\n", count_all(b, m, 5));
    printf("Count 1: %d\n", count_all(b, m, 1));
    printf("Min idx: %d\n", find_min_index(b, m));

    return 0;
}
