int printf(const char *fmt, ...);
// EXPECT: find 7: idx=3\nfind 1: idx=0\nfind 19: idx=9\nfind 8: idx=-1\nlower_bound(3) = 3\nupper_bound(3) = 6\ncount(3) = 3\ncount(5) = 4\ncount(1) = 1\ncount(6) = 0\nsqrt(0) = 0\nsqrt(1) = 1\nsqrt(4) = 2\nsqrt(9) = 3\nsqrt(10) = 3\nsqrt(100) = 10\nsqrt(1000) = 31\ninsert 6 at: 3\ninsert 0 at: 0\ninsert 20 at: 10
// Test: binary search variations

int binary_search(int *arr, int n, int target) {
    int lo = 0;
    int hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

// Find first occurrence
int lower_bound(int *arr, int n, int target) {
    int lo = 0;
    int hi = n;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

// Find last occurrence + 1
int upper_bound(int *arr, int n, int target) {
    int lo = 0;
    int hi = n;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] <= target) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

// Count occurrences using bounds
int count_occur(int *arr, int n, int target) {
    return upper_bound(arr, n, target) - lower_bound(arr, n, target);
}

// Binary search on answer: find sqrt(n)
int int_sqrt(int n) {
    if (n <= 1) return n;
    int lo = 1;
    int hi = n;
    int ans = 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (mid <= n / mid) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

int main(void) {
    int arr[10];
    arr[0]=1; arr[1]=3; arr[2]=5; arr[3]=7; arr[4]=9;
    arr[5]=11; arr[6]=13; arr[7]=15; arr[8]=17; arr[9]=19;

    // Basic search
    printf("find 7: idx=%d\n", binary_search(arr, 10, 7));
    printf("find 1: idx=%d\n", binary_search(arr, 10, 1));
    printf("find 19: idx=%d\n", binary_search(arr, 10, 19));
    printf("find 8: idx=%d\n", binary_search(arr, 10, 8));

    // Array with duplicates
    int dup[12];
    dup[0]=1; dup[1]=2; dup[2]=2; dup[3]=3; dup[4]=3; dup[5]=3;
    dup[6]=4; dup[7]=4; dup[8]=5; dup[9]=5; dup[10]=5; dup[11]=5;

    printf("lower_bound(3) = %d\n", lower_bound(dup, 12, 3));
    printf("upper_bound(3) = %d\n", upper_bound(dup, 12, 3));
    printf("count(3) = %d\n", count_occur(dup, 12, 3));
    printf("count(5) = %d\n", count_occur(dup, 12, 5));
    printf("count(1) = %d\n", count_occur(dup, 12, 1));
    printf("count(6) = %d\n", count_occur(dup, 12, 6));

    // Integer sqrt
    printf("sqrt(0) = %d\n", int_sqrt(0));
    printf("sqrt(1) = %d\n", int_sqrt(1));
    printf("sqrt(4) = %d\n", int_sqrt(4));
    printf("sqrt(9) = %d\n", int_sqrt(9));
    printf("sqrt(10) = %d\n", int_sqrt(10));
    printf("sqrt(100) = %d\n", int_sqrt(100));
    printf("sqrt(1000) = %d\n", int_sqrt(1000));

    // Insertion point
    printf("insert 6 at: %d\n", lower_bound(arr, 10, 6));
    printf("insert 0 at: %d\n", lower_bound(arr, 10, 0));
    printf("insert 20 at: %d\n", lower_bound(arr, 10, 20));

    return 0;
}
