int printf(const char *fmt, ...);

int linear_search(int *arr, int n, int target) {
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

int linear_search_sentinel(int *arr, int n, int target) {
    int last;
    int i;
    if (n == 0) {
        return -1;
    }
    last = arr[n - 1];
    arr[n - 1] = target;
    i = 0;
    while (arr[i] != target) {
        i = i + 1;
    }
    arr[n - 1] = last;
    if (i < n - 1 || last == target) {
        return i;
    }
    return -1;
}

int find_first(int *arr, int n, int target) {
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

int find_last(int *arr, int n, int target) {
    int i;
    int last_idx;
    last_idx = -1;
    for (i = 0; i < n; i++) {
        if (arr[i] == target) {
            last_idx = i;
        }
    }
    return last_idx;
}

int count_all(int *arr, int n, int target) {
    int i;
    int count;
    count = 0;
    for (i = 0; i < n; i++) {
        if (arr[i] == target) {
            count = count + 1;
        }
    }
    return count;
}

int find_min_index(int *arr, int n) {
    int i;
    int min_idx;
    min_idx = 0;
    for (i = 1; i < n; i++) {
        if (arr[i] < arr[min_idx]) {
            min_idx = i;
        }
    }
    return min_idx;
}

int find_max_index(int *arr, int n) {
    int i;
    int max_idx;
    max_idx = 0;
    for (i = 1; i < n; i++) {
        if (arr[i] > arr[max_idx]) {
            max_idx = i;
        }
    }
    return max_idx;
}

int search_in_range(int *arr, int start, int end, int target) {
    int i;
    for (i = start; i <= end; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

int main() {
    int arr1[10];
    int arr2[8];
    int arr3[6];
    int result;

    arr1[0] = 4; arr1[1] = 2; arr1[2] = 7; arr1[3] = 1; arr1[4] = 9;
    arr1[5] = 3; arr1[6] = 8; arr1[7] = 5; arr1[8] = 6; arr1[9] = 0;

    printf("Test 1: Basic linear search\n");
    // EXPECT: Test 1: Basic linear search
    result = linear_search(arr1, 10, 7);
    printf("Search 7: index %d\n", result);
    // EXPECT: Search 7: index 2
    result = linear_search(arr1, 10, 0);
    printf("Search 0: index %d\n", result);
    // EXPECT: Search 0: index 9
    result = linear_search(arr1, 10, 11);
    printf("Search 11: index %d\n", result);
    // EXPECT: Search 11: index -1

    printf("Test 2: Sentinel search\n");
    // EXPECT: Test 2: Sentinel search
    result = linear_search_sentinel(arr1, 10, 3);
    printf("Sentinel search 3: index %d\n", result);
    // EXPECT: Sentinel search 3: index 5
    result = linear_search_sentinel(arr1, 10, 4);
    printf("Sentinel search 4: index %d\n", result);
    // EXPECT: Sentinel search 4: index 0
    result = linear_search_sentinel(arr1, 10, 0);
    printf("Sentinel search 0: index %d\n", result);
    // EXPECT: Sentinel search 0: index 9
    result = linear_search_sentinel(arr1, 10, 99);
    printf("Sentinel search 99: index %d\n", result);
    // EXPECT: Sentinel search 99: index -1

    arr2[0] = 5; arr2[1] = 3; arr2[2] = 5; arr2[3] = 7;
    arr2[4] = 5; arr2[5] = 2; arr2[6] = 5; arr2[7] = 9;

    printf("Test 3: Find first and last occurrence\n");
    // EXPECT: Test 3: Find first and last occurrence
    printf("First 5: index %d\n", find_first(arr2, 8, 5));
    // EXPECT: First 5: index 0
    printf("Last 5: index %d\n", find_last(arr2, 8, 5));
    // EXPECT: Last 5: index 6
    printf("Count of 5: %d\n", count_all(arr2, 8, 5));
    // EXPECT: Count of 5: 4

    printf("Test 4: Min and max index\n");
    // EXPECT: Test 4: Min and max index
    printf("Min index: %d (val %d)\n", find_min_index(arr2, 8), arr2[find_min_index(arr2, 8)]);
    // EXPECT: Min index: 5 (val 2)
    printf("Max index: %d (val %d)\n", find_max_index(arr2, 8), arr2[find_max_index(arr2, 8)]);
    // EXPECT: Max index: 7 (val 9)

    printf("Test 5: Range search\n");
    // EXPECT: Test 5: Range search
    result = search_in_range(arr2, 2, 6, 5);
    printf("Range [2,6] search 5: index %d\n", result);
    // EXPECT: Range [2,6] search 5: index 2
    result = search_in_range(arr2, 3, 5, 5);
    printf("Range [3,5] search 5: index %d\n", result);
    // EXPECT: Range [3,5] search 5: index 4
    result = search_in_range(arr2, 1, 1, 5);
    printf("Range [1,1] search 5: index %d\n", result);
    // EXPECT: Range [1,1] search 5: index -1

    arr3[0] = 42;
    printf("Test 6: Edge cases\n");
    // EXPECT: Test 6: Edge cases
    result = linear_search_sentinel(arr3, 1, 42);
    printf("Single found: %d\n", result);
    // EXPECT: Single found: 0
    result = linear_search_sentinel(arr3, 1, 99);
    printf("Single not found: %d\n", result);
    // EXPECT: Single not found: -1
    printf("Count of 3 in arr2: %d\n", count_all(arr2, 8, 3));
    // EXPECT: Count of 3 in arr2: 1
    printf("Count of 100 in arr2: %d\n", count_all(arr2, 8, 100));
    // EXPECT: Count of 100 in arr2: 0

    printf("All linear search sentinel tests passed\n");
    // EXPECT: All linear search sentinel tests passed
    return 0;
}
