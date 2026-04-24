int printf(const char *fmt, ...);
// EXPECT: array: 0 5 10 15 20 25 30 35 40 45 50 55 60 65 70 \nfind 0: 0\nfind 25: 5\nfind 70: 14\nfind 35: 7\nfind 3: -1\nfind 99: -1\nfind -5: -1\nlower_bound 25: 5\nlower_bound 23: 5\nlower_bound 0: 0\nlower_bound 100: 15\nupper_bound 25: 6\nupper_bound 23: 5\ndup lower 2: 1\ndup upper 2: 4\ncount 2s: 3\ndup lower 5: 5\ndup upper 5: 7
// Test: binary search and related algorithms

int binary_search(int arr[], int n, int target) {
    int low, high, mid;
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

int lower_bound(int arr[], int n, int target) {
    int low, high, mid;
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

int upper_bound(int arr[], int n, int target) {
    int low, high, mid;
    low = 0;
    high = n;
    while (low < high) {
        mid = low + (high - low) / 2;
        if (arr[mid] <= target) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low;
}

int main(void) {
    int arr[15];
    int i;
    int idx;

    /* Fill sorted array: 0, 5, 10, 15, ..., 70 */
    for (i = 0; i < 15; i++) {
        arr[i] = i * 5;
    }

    printf("array: ");
    for (i = 0; i < 15; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    /* Search for existing elements */
    printf("find 0: %d\n", binary_search(arr, 15, 0));
    printf("find 25: %d\n", binary_search(arr, 15, 25));
    printf("find 70: %d\n", binary_search(arr, 15, 70));
    printf("find 35: %d\n", binary_search(arr, 15, 35));

    /* Search for non-existing */
    printf("find 3: %d\n", binary_search(arr, 15, 3));
    printf("find 99: %d\n", binary_search(arr, 15, 99));
    printf("find -5: %d\n", binary_search(arr, 15, -5));

    /* Lower bound */
    printf("lower_bound 25: %d\n", lower_bound(arr, 15, 25));
    printf("lower_bound 23: %d\n", lower_bound(arr, 15, 23));
    printf("lower_bound 0: %d\n", lower_bound(arr, 15, 0));
    printf("lower_bound 100: %d\n", lower_bound(arr, 15, 100));

    /* Upper bound */
    printf("upper_bound 25: %d\n", upper_bound(arr, 15, 25));
    printf("upper_bound 23: %d\n", upper_bound(arr, 15, 23));

    /* Array with duplicates */
    {
        int dup[10];
        dup[0] = 1; dup[1] = 2; dup[2] = 2; dup[3] = 2;
        dup[4] = 3; dup[5] = 5; dup[6] = 5; dup[7] = 7;
        dup[8] = 7; dup[9] = 9;
        printf("dup lower 2: %d\n", lower_bound(dup, 10, 2));
        printf("dup upper 2: %d\n", upper_bound(dup, 10, 2));
        printf("count 2s: %d\n", upper_bound(dup, 10, 2) - lower_bound(dup, 10, 2));
        printf("dup lower 5: %d\n", lower_bound(dup, 10, 5));
        printf("dup upper 5: %d\n", upper_bound(dup, 10, 5));
    }

    return 0;
}
