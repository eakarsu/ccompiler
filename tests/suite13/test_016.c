int printf(const char *fmt, ...);

// Binary search variants: standard, lower bound, upper bound
// All operate on sorted arrays

// Standard binary search: returns index of target, or -1
int binary_search(int *arr, int n, int target, int *iterations) {
    *iterations = 0;
    int lo = 0;
    int hi = n - 1;
    while (lo <= hi) {
        *iterations = *iterations + 1;
        int mid = lo + (hi - lo) / 2;
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

// Lower bound: first position where arr[i] >= target
int lower_bound(int *arr, int n, int target) {
    int lo = 0;
    int hi = n;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

// Upper bound: first position where arr[i] > target
int upper_bound(int *arr, int n, int target) {
    int lo = 0;
    int hi = n;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] <= target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

// Count occurrences using lower/upper bound
int count_occurrences(int *arr, int n, int target) {
    return upper_bound(arr, n, target) - lower_bound(arr, n, target);
}

// Find insertion point to maintain sorted order
int insertion_point(int *arr, int n, int target) {
    return lower_bound(arr, n, target);
}

// Binary search on answer: find smallest x where x*x >= n
int sqrt_floor(int n) {
    if (n <= 1) return n;
    int lo = 1;
    int hi = n;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (mid <= n / mid) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo - 1;
}

// Find peak element in a bitonic array (increases then decreases)
int find_peak(int *arr, int n) {
    int lo = 0;
    int hi = n - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] < arr[mid + 1]) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

// Find first occurrence of target
int find_first(int *arr, int n, int target) {
    int lb = lower_bound(arr, n, target);
    if (lb < n && arr[lb] == target) return lb;
    return -1;
}

// Find last occurrence of target
int find_last(int *arr, int n, int target) {
    int ub = upper_bound(arr, n, target);
    if (ub > 0 && arr[ub - 1] == target) return ub - 1;
    return -1;
}

int main() {
    int iters;

    // Test 1: Basic binary search
    int a1[10];
    a1[0] = 2; a1[1] = 5; a1[2] = 8; a1[3] = 12; a1[4] = 16;
    a1[5] = 23; a1[6] = 38; a1[7] = 56; a1[8] = 72; a1[9] = 91;

    int idx = binary_search(a1, 10, 23, &iters);
    printf("find 23: idx=%d iters=%d\n", idx, iters);
    // EXPECT: find 23: idx=5 iters=3

    idx = binary_search(a1, 10, 2, &iters);
    printf("find 2: idx=%d iters=%d\n", idx, iters);
    // EXPECT: find 2: idx=0 iters=3

    idx = binary_search(a1, 10, 91, &iters);
    printf("find 91: idx=%d iters=%d\n", idx, iters);
    // EXPECT: find 91: idx=9 iters=4

    idx = binary_search(a1, 10, 50, &iters);
    printf("find 50: idx=%d iters=%d\n", idx, iters);
    // EXPECT: find 50: idx=-1 iters=4

    // Test 2: Lower and upper bound with duplicates
    int a2[12];
    a2[0] = 1; a2[1] = 2; a2[2] = 2; a2[3] = 3; a2[4] = 3; a2[5] = 3;
    a2[6] = 5; a2[7] = 5; a2[8] = 7; a2[9] = 8; a2[10] = 8; a2[11] = 9;

    int lb = lower_bound(a2, 12, 3);
    int ub = upper_bound(a2, 12, 3);
    printf("3: lower=%d upper=%d count=%d\n", lb, ub, ub - lb);
    // EXPECT: 3: lower=3 upper=6 count=3

    lb = lower_bound(a2, 12, 5);
    ub = upper_bound(a2, 12, 5);
    printf("5: lower=%d upper=%d count=%d\n", lb, ub, ub - lb);
    // EXPECT: 5: lower=6 upper=8 count=2

    lb = lower_bound(a2, 12, 4);
    ub = upper_bound(a2, 12, 4);
    printf("4: lower=%d upper=%d count=%d\n", lb, ub, ub - lb);
    // EXPECT: 4: lower=6 upper=6 count=0

    // Test 3: Count occurrences
    printf("count 8: %d\n", count_occurrences(a2, 12, 8));
    // EXPECT: count 8: 2
    printf("count 1: %d\n", count_occurrences(a2, 12, 1));
    // EXPECT: count 1: 1
    printf("count 6: %d\n", count_occurrences(a2, 12, 6));
    // EXPECT: count 6: 0

    // Test 4: Insertion point
    printf("insert 4: %d\n", insertion_point(a2, 12, 4));
    // EXPECT: insert 4: 6
    printf("insert 0: %d\n", insertion_point(a2, 12, 0));
    // EXPECT: insert 0: 0
    printf("insert 10: %d\n", insertion_point(a2, 12, 10));
    // EXPECT: insert 10: 12

    // Test 5: Integer square root
    printf("sqrt 0: %d\n", sqrt_floor(0));
    // EXPECT: sqrt 0: 0
    printf("sqrt 1: %d\n", sqrt_floor(1));
    // EXPECT: sqrt 1: 1
    printf("sqrt 16: %d\n", sqrt_floor(16));
    // EXPECT: sqrt 16: 4
    printf("sqrt 26: %d\n", sqrt_floor(26));
    // EXPECT: sqrt 26: 5
    printf("sqrt 100: %d\n", sqrt_floor(100));
    // EXPECT: sqrt 100: 10

    // Test 6: Peak finding in bitonic array
    int b1[7];
    b1[0] = 1; b1[1] = 3; b1[2] = 8; b1[3] = 12; b1[4] = 9; b1[5] = 5; b1[6] = 2;
    int peak = find_peak(b1, 7);
    printf("peak idx: %d val: %d\n", peak, b1[peak]);
    // EXPECT: peak idx: 3 val: 12

    int b2[5];
    b2[0] = 10; b2[1] = 20; b2[2] = 30; b2[3] = 25; b2[4] = 15;
    peak = find_peak(b2, 5);
    printf("peak2 idx: %d val: %d\n", peak, b2[peak]);
    // EXPECT: peak2 idx: 2 val: 30

    // Test 7: First and last occurrence
    int first = find_first(a2, 12, 3);
    int last = find_last(a2, 12, 3);
    printf("3: first=%d last=%d\n", first, last);
    // EXPECT: 3: first=3 last=5

    first = find_first(a2, 12, 4);
    last = find_last(a2, 12, 4);
    printf("4: first=%d last=%d\n", first, last);
    // EXPECT: 4: first=-1 last=-1

    printf("done\n");
    // EXPECT: done

    return 0;
}
