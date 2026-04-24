int printf(const char *fmt, ...);

int binary_search(int *arr, int n, int target) {
    int lo;
    int hi;
    int mid;
    lo = 0;
    hi = n - 1;
    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return -1;
}

int lower_bound(int *arr, int n, int target) {
    int lo;
    int hi;
    int mid;
    lo = 0;
    hi = n;
    while (lo < hi) {
        mid = lo + (hi - lo) / 2;
        if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

int upper_bound(int *arr, int n, int target) {
    int lo;
    int hi;
    int mid;
    lo = 0;
    hi = n;
    while (lo < hi) {
        mid = lo + (hi - lo) / 2;
        if (arr[mid] <= target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

int count_occurrences(int *arr, int n, int target) {
    int lb;
    int ub;
    lb = lower_bound(arr, n, target);
    ub = upper_bound(arr, n, target);
    return ub - lb;
}

int binary_search_recursive_helper(int *arr, int lo, int hi, int target) {
    int mid;
    if (lo > hi) {
        return -1;
    }
    mid = lo + (hi - lo) / 2;
    if (arr[mid] == target) {
        return mid;
    } else if (arr[mid] < target) {
        return binary_search_recursive_helper(arr, mid + 1, hi, target);
    } else {
        return binary_search_recursive_helper(arr, lo, mid - 1, target);
    }
}

int binary_search_recursive(int *arr, int n, int target) {
    return binary_search_recursive_helper(arr, 0, n - 1, target);
}

int main() {
    int arr1[10];
    int arr2[8];
    int arr3[12];
    int result;
    int i;

    arr1[0] = 2; arr1[1] = 5; arr1[2] = 8; arr1[3] = 12; arr1[4] = 16;
    arr1[5] = 23; arr1[6] = 38; arr1[7] = 56; arr1[8] = 72; arr1[9] = 91;

    printf("Test 1: Find existing elements\n");
    // EXPECT: Test 1: Find existing elements
    result = binary_search(arr1, 10, 23);
    printf("Search 23: index %d\n", result);
    // EXPECT: Search 23: index 5
    result = binary_search(arr1, 10, 2);
    printf("Search 2: index %d\n", result);
    // EXPECT: Search 2: index 0
    result = binary_search(arr1, 10, 91);
    printf("Search 91: index %d\n", result);
    // EXPECT: Search 91: index 9

    printf("Test 2: Element not found\n");
    // EXPECT: Test 2: Element not found
    result = binary_search(arr1, 10, 1);
    printf("Search 1: index %d\n", result);
    // EXPECT: Search 1: index -1
    result = binary_search(arr1, 10, 100);
    printf("Search 100: index %d\n", result);
    // EXPECT: Search 100: index -1
    result = binary_search(arr1, 10, 10);
    printf("Search 10: index %d\n", result);
    // EXPECT: Search 10: index -1

    printf("Test 3: Recursive binary search\n");
    // EXPECT: Test 3: Recursive binary search
    result = binary_search_recursive(arr1, 10, 38);
    printf("Search 38: index %d\n", result);
    // EXPECT: Search 38: index 6
    result = binary_search_recursive(arr1, 10, 99);
    printf("Search 99: index %d\n", result);
    // EXPECT: Search 99: index -1

    arr2[0] = 1; arr2[1] = 3; arr2[2] = 3; arr2[3] = 3;
    arr2[4] = 5; arr2[5] = 7; arr2[6] = 7; arr2[7] = 9;

    printf("Test 4: Lower and upper bounds\n");
    // EXPECT: Test 4: Lower and upper bounds
    printf("Lower bound of 3: %d\n", lower_bound(arr2, 8, 3));
    // EXPECT: Lower bound of 3: 1
    printf("Upper bound of 3: %d\n", upper_bound(arr2, 8, 3));
    // EXPECT: Upper bound of 3: 4
    printf("Lower bound of 7: %d\n", lower_bound(arr2, 8, 7));
    // EXPECT: Lower bound of 7: 5
    printf("Upper bound of 7: %d\n", upper_bound(arr2, 8, 7));
    // EXPECT: Upper bound of 7: 7

    printf("Test 5: Count occurrences\n");
    // EXPECT: Test 5: Count occurrences
    printf("Count of 3: %d\n", count_occurrences(arr2, 8, 3));
    // EXPECT: Count of 3: 3
    printf("Count of 7: %d\n", count_occurrences(arr2, 8, 7));
    // EXPECT: Count of 7: 2
    printf("Count of 1: %d\n", count_occurrences(arr2, 8, 1));
    // EXPECT: Count of 1: 1
    printf("Count of 4: %d\n", count_occurrences(arr2, 8, 4));
    // EXPECT: Count of 4: 0

    printf("Test 6: Edge cases\n");
    // EXPECT: Test 6: Edge cases
    arr3[0] = 42;
    result = binary_search(arr3, 1, 42);
    printf("Single elem found: %d\n", result);
    // EXPECT: Single elem found: 0
    result = binary_search(arr3, 1, 99);
    printf("Single elem not found: %d\n", result);
    // EXPECT: Single elem not found: -1

    printf("All binary search tests passed\n");
    // EXPECT: All binary search tests passed
    return 0;
}
