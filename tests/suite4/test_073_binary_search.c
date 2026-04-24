int printf(const char *fmt, ...);

int binary_search(int *arr, int n, int target) {
    int low;
    int high;
    int mid;
    low = 0;
    high = n - 1;
    while (low <= high) {
        mid = low + (high - low) / 2;
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

int binary_search_first(int *arr, int n, int target) {
    int low;
    int high;
    int mid;
    int result;
    low = 0;
    high = n - 1;
    result = -1;
    while (low <= high) {
        mid = low + (high - low) / 2;
        if (arr[mid] == target) {
            result = mid;
            high = mid - 1;
        } else if (arr[mid] < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return result;
}

int binary_search_last(int *arr, int n, int target) {
    int low;
    int high;
    int mid;
    int result;
    low = 0;
    high = n - 1;
    result = -1;
    while (low <= high) {
        mid = low + (high - low) / 2;
        if (arr[mid] == target) {
            result = mid;
            low = mid + 1;
        } else if (arr[mid] < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return result;
}

int count_occurrences(int *arr, int n, int target) {
    int first;
    int last;
    first = binary_search_first(arr, n, target);
    if (first == -1) {
        return 0;
    }
    last = binary_search_last(arr, n, target);
    return last - first + 1;
}

int lower_bound(int *arr, int n, int target) {
    int low;
    int high;
    int mid;
    low = 0;
    high = n;
    while (low < high) {
        mid = low + (high - low) / 2;
        if (arr[mid] < target) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low;
}

int main(void) {
    int a[10];
    int b[8];
    int idx;

    a[0] = 2; a[1] = 5; a[2] = 8; a[3] = 12; a[4] = 16;
    a[5] = 23; a[6] = 38; a[7] = 56; a[8] = 72; a[9] = 91;

    idx = binary_search(a, 10, 23);
    // EXPECT: find 23: 5
    printf("find 23: %d\n", idx);

    idx = binary_search(a, 10, 2);
    // EXPECT: find 2: 0
    printf("find 2: %d\n", idx);

    idx = binary_search(a, 10, 91);
    // EXPECT: find 91: 9
    printf("find 91: %d\n", idx);

    idx = binary_search(a, 10, 12);
    // EXPECT: find 12: 3
    printf("find 12: %d\n", idx);

    idx = binary_search(a, 10, 1);
    // EXPECT: not found 1: -1
    printf("not found 1: %d\n", idx);

    idx = binary_search(a, 10, 100);
    // EXPECT: not found 100: -1
    printf("not found 100: %d\n", idx);

    idx = binary_search(a, 10, 10);
    // EXPECT: not found 10: -1
    printf("not found 10: %d\n", idx);

    b[0] = 1; b[1] = 3; b[2] = 3; b[3] = 3;
    b[4] = 5; b[5] = 7; b[6] = 7; b[7] = 9;

    idx = binary_search_first(b, 8, 3);
    // EXPECT: first 3: 1
    printf("first 3: %d\n", idx);

    idx = binary_search_last(b, 8, 3);
    // EXPECT: last 3: 3
    printf("last 3: %d\n", idx);

    // EXPECT: count 3: 3
    printf("count 3: %d\n", count_occurrences(b, 8, 3));

    // EXPECT: count 7: 2
    printf("count 7: %d\n", count_occurrences(b, 8, 7));

    // EXPECT: count 1: 1
    printf("count 1: %d\n", count_occurrences(b, 8, 1));

    // EXPECT: count 4: 0
    printf("count 4: %d\n", count_occurrences(b, 8, 4));

    idx = lower_bound(b, 8, 3);
    // EXPECT: lower 3: 1
    printf("lower 3: %d\n", idx);

    idx = lower_bound(b, 8, 6);
    // EXPECT: lower 6: 5
    printf("lower 6: %d\n", idx);

    idx = lower_bound(b, 8, 0);
    // EXPECT: lower 0: 0
    printf("lower 0: %d\n", idx);

    idx = lower_bound(b, 8, 10);
    // EXPECT: lower 10: 8
    printf("lower 10: %d\n", idx);

    idx = binary_search(a, 1, 2);
    // EXPECT: single found: 0
    printf("single found: %d\n", idx);

    idx = binary_search(a, 1, 5);
    // EXPECT: single not: -1
    printf("single not: %d\n", idx);

    // EXPECT: done
    printf("done\n");

    return 0;
}
