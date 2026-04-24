int printf(const char *fmt, ...);
// EXPECT: === Binary Search Tests ===
// EXPECT: --- Iterative: find existing ---
// EXPECT: array: 2 5 8 12 16 23 38 56 72 91
// EXPECT: search 23: index=5
// EXPECT: search 2: index=0
// EXPECT: search 91: index=9
// EXPECT: search 12: index=3
// EXPECT: --- Iterative: not found ---
// EXPECT: search 1: index=-1
// EXPECT: search 100: index=-1
// EXPECT: search 15: index=-1
// EXPECT: --- Recursive: find existing ---
// EXPECT: search 38: index=6
// EXPECT: search 56: index=7
// EXPECT: search 8: index=2
// EXPECT: --- Recursive: not found ---
// EXPECT: search 0: index=-1
// EXPECT: search 50: index=-1
// EXPECT: --- Single element array ---
// EXPECT: search 5 in {5}: index=0
// EXPECT: search 3 in {5}: index=-1
// EXPECT: --- Two element array ---
// EXPECT: search 3 in {3,7}: index=0
// EXPECT: search 7 in {3,7}: index=1
// EXPECT: search 5 in {3,7}: index=-1
// EXPECT: --- First occurrence ---
// EXPECT: array: 1 2 2 2 3 3 4 5
// EXPECT: first 2: index=1
// EXPECT: first 3: index=4
// EXPECT: first 1: index=0
// EXPECT: first 5: index=7
// EXPECT: first 6: index=-1
// EXPECT: --- Last occurrence ---
// EXPECT: last 2: index=3
// EXPECT: last 3: index=5
// EXPECT: last 1: index=0
// EXPECT: last 5: index=7
// EXPECT: last 6: index=-1
// EXPECT: --- Count occurrences ---
// EXPECT: count of 2: 3
// EXPECT: count of 3: 2
// EXPECT: count of 4: 1
// EXPECT: count of 6: 0
// EXPECT: --- Insert position ---
// EXPECT: insert 0: pos=0
// EXPECT: insert 3: pos=2
// EXPECT: insert 6: pos=4
// EXPECT: insert 100: pos=8
// EXPECT: --- Large array search ---
// EXPECT: search 500: found=1
// EXPECT: search 999: found=1
// EXPECT: search 1001: found=0
// EXPECT: === Binary Search Done ===
// --- Iterative: find existing ---
// array: 2 5 8 12 16 23 38 56 72 91
// search 23: index=5
// search 2: index=0
// search 91: index=9
// search 12: index=3
// --- Iterative: not found ---
// search 1: index=-1
// search 100: index=-1
// search 15: index=-1
// --- Recursive: find existing ---
// search 38: index=6
// search 56: index=7
// search 8: index=2
// --- Recursive: not found ---
// search 0: index=-1
// search 50: index=-1
// --- Single element array ---
// search 5 in {5}: index=0
// search 3 in {5}: index=-1
// --- Two element array ---
// search 3 in {3,7}: index=0
// search 7 in {3,7}: index=1
// search 5 in {3,7}: index=-1
// --- First occurrence ---
// array: 1 2 2 2 3 3 4 5
// first 2: index=1
// first 3: index=4
// first 1: index=0
// first 5: index=7
// first 6: index=-1
// --- Last occurrence ---
// last 2: index=3
// last 3: index=5
// last 1: index=0
// last 5: index=7
// last 6: index=-1
// --- Count occurrences ---
// count of 2: 3
// count of 3: 2
// count of 4: 1
// count of 6: 0
// --- Insert position ---
// insert 0: pos=0
// insert 3: pos=2
// insert 6: pos=4
// insert 100: pos=8
// --- Large array search ---
// search 500: found=1
// search 999: found=1
// search 1001: found=0
// === Binary Search Done ===

int binary_search_iter(int *arr, int n, int target) {
    int lo = 0;
    int hi = n - 1;
    int mid;
    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

int binary_search_rec_helper(int *arr, int lo, int hi, int target) {
    if (lo > hi) return -1;
    int mid = lo + (hi - lo) / 2;
    if (arr[mid] == target) return mid;
    if (arr[mid] < target) return binary_search_rec_helper(arr, mid + 1, hi, target);
    return binary_search_rec_helper(arr, lo, mid - 1, target);
}

int binary_search_rec(int *arr, int n, int target) {
    return binary_search_rec_helper(arr, 0, n - 1, target);
}

// Find the first (leftmost) occurrence of target
int binary_search_first(int *arr, int n, int target) {
    int lo = 0;
    int hi = n - 1;
    int mid;
    int result = -1;
    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            result = mid;
            hi = mid - 1; // keep searching left
        } else if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return result;
}

// Find the last (rightmost) occurrence of target
int binary_search_last(int *arr, int n, int target) {
    int lo = 0;
    int hi = n - 1;
    int mid;
    int result = -1;
    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            result = mid;
            lo = mid + 1; // keep searching right
        } else if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return result;
}

// Count occurrences using first and last
int count_occurrences(int *arr, int n, int target) {
    int first = binary_search_first(arr, n, target);
    if (first == -1) return 0;
    int last = binary_search_last(arr, n, target);
    return last - first + 1;
}

// Find insert position (lower bound)
int insert_position(int *arr, int n, int target) {
    int lo = 0;
    int hi = n;
    int mid;
    while (lo < hi) {
        mid = lo + (hi - lo) / 2;
        if (arr[mid] < target)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

void print_array(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

int main() {
    printf("=== Binary Search Tests ===\n");

    int arr[10] = {2, 5, 8, 12, 16, 23, 38, 56, 72, 91};

    // Iterative: find existing
    printf("--- Iterative: find existing ---\n");
    printf("array: ");
    print_array(arr, 10);
    printf("search 23: index=%d\n", binary_search_iter(arr, 10, 23));
    printf("search 2: index=%d\n", binary_search_iter(arr, 10, 2));
    printf("search 91: index=%d\n", binary_search_iter(arr, 10, 91));
    printf("search 12: index=%d\n", binary_search_iter(arr, 10, 12));

    // Iterative: not found
    printf("--- Iterative: not found ---\n");
    printf("search 1: index=%d\n", binary_search_iter(arr, 10, 1));
    printf("search 100: index=%d\n", binary_search_iter(arr, 10, 100));
    printf("search 15: index=%d\n", binary_search_iter(arr, 10, 15));

    // Recursive: find existing
    printf("--- Recursive: find existing ---\n");
    printf("search 38: index=%d\n", binary_search_rec(arr, 10, 38));
    printf("search 56: index=%d\n", binary_search_rec(arr, 10, 56));
    printf("search 8: index=%d\n", binary_search_rec(arr, 10, 8));

    // Recursive: not found
    printf("--- Recursive: not found ---\n");
    printf("search 0: index=%d\n", binary_search_rec(arr, 10, 0));
    printf("search 50: index=%d\n", binary_search_rec(arr, 10, 50));

    // Single element
    printf("--- Single element array ---\n");
    int s1[1] = {5};
    printf("search 5 in {5}: index=%d\n", binary_search_iter(s1, 1, 5));
    printf("search 3 in {5}: index=%d\n", binary_search_iter(s1, 1, 3));

    // Two elements
    printf("--- Two element array ---\n");
    int s2[2] = {3, 7};
    printf("search 3 in {3,7}: index=%d\n", binary_search_iter(s2, 2, 3));
    printf("search 7 in {3,7}: index=%d\n", binary_search_iter(s2, 2, 7));
    printf("search 5 in {3,7}: index=%d\n", binary_search_iter(s2, 2, 5));

    // First occurrence
    printf("--- First occurrence ---\n");
    int dup[8] = {1, 2, 2, 2, 3, 3, 4, 5};
    printf("array: ");
    print_array(dup, 8);
    printf("first 2: index=%d\n", binary_search_first(dup, 8, 2));
    printf("first 3: index=%d\n", binary_search_first(dup, 8, 3));
    printf("first 1: index=%d\n", binary_search_first(dup, 8, 1));
    printf("first 5: index=%d\n", binary_search_first(dup, 8, 5));
    printf("first 6: index=%d\n", binary_search_first(dup, 8, 6));

    // Last occurrence
    printf("--- Last occurrence ---\n");
    printf("last 2: index=%d\n", binary_search_last(dup, 8, 2));
    printf("last 3: index=%d\n", binary_search_last(dup, 8, 3));
    printf("last 1: index=%d\n", binary_search_last(dup, 8, 1));
    printf("last 5: index=%d\n", binary_search_last(dup, 8, 5));
    printf("last 6: index=%d\n", binary_search_last(dup, 8, 6));

    // Count occurrences
    printf("--- Count occurrences ---\n");
    printf("count of 2: %d\n", count_occurrences(dup, 8, 2));
    printf("count of 3: %d\n", count_occurrences(dup, 8, 3));
    printf("count of 4: %d\n", count_occurrences(dup, 8, 4));
    printf("count of 6: %d\n", count_occurrences(dup, 8, 6));

    // Insert position
    printf("--- Insert position ---\n");
    int ip[8] = {1, 2, 3, 5, 7, 8, 9, 10};
    printf("insert 0: pos=%d\n", insert_position(ip, 8, 0));
    printf("insert 3: pos=%d\n", insert_position(ip, 8, 3));
    printf("insert 6: pos=%d\n", insert_position(ip, 8, 6));
    printf("insert 100: pos=%d\n", insert_position(ip, 8, 100));

    // Large array search
    printf("--- Large array search ---\n");
    int big[1000];
    int i;
    for (i = 0; i < 1000; i++) {
        big[i] = i + 1;  // 1..1000
    }
    printf("search 500: found=%d\n", binary_search_iter(big, 1000, 500) != -1);
    printf("search 999: found=%d\n", binary_search_iter(big, 1000, 999) != -1);
    printf("search 1001: found=%d\n", binary_search_iter(big, 1000, 1001) != -1);

    printf("=== Binary Search Done ===\n");
    return 0;
}
