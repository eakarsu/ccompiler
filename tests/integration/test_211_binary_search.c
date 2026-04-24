int printf(const char *fmt, ...);
// EXPECT: find 12: 3\nfind 23: 5\nfind 91: 9\nfind 2: 0\nfind 50: -1\nfind 1: -1\ncount 3: 3\ncount 4: 4\ncount 1: 1\ncount 6: 0\nrot min: 0\nrot find 0: 4\nrot find 5: 1\nrot find 3: -1
// Test 211: Recursive binary search

int binary_search(int *arr, int lo, int hi, int target) {
    if (lo > hi) return -1;
    int mid = lo + (hi - lo) / 2;
    if (arr[mid] == target) return mid;
    if (arr[mid] > target) return binary_search(arr, lo, mid - 1, target);
    return binary_search(arr, mid + 1, hi, target);
}

int lower_bound(int *arr, int lo, int hi, int target) {
    if (lo > hi) return lo;
    int mid = lo + (hi - lo) / 2;
    if (arr[mid] < target) return lower_bound(arr, mid + 1, hi, target);
    return lower_bound(arr, lo, mid - 1, target);
}

int upper_bound(int *arr, int lo, int hi, int target) {
    if (lo > hi) return lo;
    int mid = lo + (hi - lo) / 2;
    if (arr[mid] <= target) return upper_bound(arr, mid + 1, hi, target);
    return upper_bound(arr, lo, mid - 1, target);
}

int count_occurrences(int *arr, int n, int target) {
    int lb = lower_bound(arr, 0, n - 1, target);
    int ub = upper_bound(arr, 0, n - 1, target);
    return ub - lb;
}

int find_min_rotated(int *arr, int lo, int hi) {
    if (lo == hi) return arr[lo];
    int mid = lo + (hi - lo) / 2;
    if (arr[mid] > arr[hi]) return find_min_rotated(arr, mid + 1, hi);
    return find_min_rotated(arr, lo, mid);
}

int search_rotated(int *arr, int lo, int hi, int target) {
    if (lo > hi) return -1;
    int mid = lo + (hi - lo) / 2;
    if (arr[mid] == target) return mid;
    if (arr[lo] <= arr[mid]) {
        if (target >= arr[lo] && target < arr[mid])
            return search_rotated(arr, lo, mid - 1, target);
        return search_rotated(arr, mid + 1, hi, target);
    }
    if (target > arr[mid] && target <= arr[hi])
        return search_rotated(arr, mid + 1, hi, target);
    return search_rotated(arr, lo, mid - 1, target);
}

int main(void) {
    int arr[10];
    arr[0] = 2; arr[1] = 5; arr[2] = 8; arr[3] = 12; arr[4] = 16;
    arr[5] = 23; arr[6] = 38; arr[7] = 56; arr[8] = 72; arr[9] = 91;

    printf("find 12: %d\n", binary_search(arr, 0, 9, 12));
    printf("find 23: %d\n", binary_search(arr, 0, 9, 23));
    printf("find 91: %d\n", binary_search(arr, 0, 9, 91));
    printf("find 2: %d\n", binary_search(arr, 0, 9, 2));
    printf("find 50: %d\n", binary_search(arr, 0, 9, 50));
    printf("find 1: %d\n", binary_search(arr, 0, 9, 1));

    int dup[12];
    dup[0] = 1; dup[1] = 2; dup[2] = 2; dup[3] = 3;
    dup[4] = 3; dup[5] = 3; dup[6] = 4; dup[7] = 4;
    dup[8] = 4; dup[9] = 4; dup[10] = 5; dup[11] = 5;

    printf("count 3: %d\n", count_occurrences(dup, 12, 3));
    printf("count 4: %d\n", count_occurrences(dup, 12, 4));
    printf("count 1: %d\n", count_occurrences(dup, 12, 1));
    printf("count 6: %d\n", count_occurrences(dup, 12, 6));

    int rot[7];
    rot[0] = 4; rot[1] = 5; rot[2] = 6; rot[3] = 7;
    rot[4] = 0; rot[5] = 1; rot[6] = 2;

    printf("rot min: %d\n", find_min_rotated(rot, 0, 6));
    printf("rot find 0: %d\n", search_rotated(rot, 0, 6, 0));
    printf("rot find 5: %d\n", search_rotated(rot, 0, 6, 5));
    printf("rot find 3: %d\n", search_rotated(rot, 0, 6, 3));

    return 0;
}
