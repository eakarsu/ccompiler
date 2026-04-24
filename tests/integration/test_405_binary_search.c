int printf(const char *fmt, ...);
// EXPECT: Search 7: 3\nSearch 1: 0\nSearch 19: 9\nSearch 4: -1\nLower 2: 1\nUpper 2: 4\nCount 2: 3\nCount 4: 2\nCount 6: 0\nRotated 5: 5\nRotated 15: 0\nRotated 20: -1
// Test: Binary search variations

int binary_search(int arr[], int n, int target) {
    int low = 0;
    int high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

int lower_bound(int arr[], int n, int target) {
    int low = 0;
    int high = n;
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] < target) low = mid + 1;
        else high = mid;
    }
    return low;
}

int upper_bound(int arr[], int n, int target) {
    int low = 0;
    int high = n;
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] <= target) low = mid + 1;
        else high = mid;
    }
    return low;
}

int count_occurrences(int arr[], int n, int target) {
    return upper_bound(arr, n, target) - lower_bound(arr, n, target);
}

int search_rotated(int arr[], int n, int target) {
    int low = 0;
    int high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == target) return mid;
        if (arr[low] <= arr[mid]) {
            if (target >= arr[low] && target < arr[mid])
                high = mid - 1;
            else
                low = mid + 1;
        } else {
            if (target > arr[mid] && target <= arr[high])
                low = mid + 1;
            else
                high = mid - 1;
        }
    }
    return -1;
}

int main(void) {
    int a[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int n = 10;

    printf("Search 7: %d\n", binary_search(a, n, 7));
    printf("Search 1: %d\n", binary_search(a, n, 1));
    printf("Search 19: %d\n", binary_search(a, n, 19));
    printf("Search 4: %d\n", binary_search(a, n, 4));

    int b[] = {1, 2, 2, 2, 3, 4, 4, 5};
    int m = 8;
    printf("Lower 2: %d\n", lower_bound(b, m, 2));
    printf("Upper 2: %d\n", upper_bound(b, m, 2));
    printf("Count 2: %d\n", count_occurrences(b, m, 2));
    printf("Count 4: %d\n", count_occurrences(b, m, 4));
    printf("Count 6: %d\n", count_occurrences(b, m, 6));

    int c[] = {15, 17, 19, 1, 3, 5, 7, 9, 11, 13};
    printf("Rotated 5: %d\n", search_rotated(c, 10, 5));
    printf("Rotated 15: %d\n", search_rotated(c, 10, 15));
    printf("Rotated 20: %d\n", search_rotated(c, 10, 20));

    return 0;
}
