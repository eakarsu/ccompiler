int printf(const char *fmt, ...);

int linear_search(int arr[], int n, int target) {
    int i;
    i = 0;
    while (i < n) {
        if (arr[i] == target) {
            return i;
        }
        i = i + 1;
    }
    return -1;
}

int sentinel_search(int arr[], int n, int target) {
    int last;
    int i;
    last = arr[n - 1];
    arr[n - 1] = target;
    i = 0;
    while (arr[i] != target) {
        i = i + 1;
    }
    arr[n - 1] = last;
    if (i < n - 1) {
        return i;
    }
    if (last == target) {
        return n - 1;
    }
    return -1;
}

int binary_search(int arr[], int n, int target) {
    int lo;
    int hi;
    int mid;
    lo = 0;
    hi = n - 1;
    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            return mid;
        }
        if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return -1;
}

int interpolation_search(int arr[], int n, int target) {
    int lo;
    int hi;
    int pos;
    int range;
    lo = 0;
    hi = n - 1;
    while (lo <= hi && target >= arr[lo] && target <= arr[hi]) {
        if (arr[hi] == arr[lo]) {
            if (arr[lo] == target) {
                return lo;
            }
            return -1;
        }
        range = arr[hi] - arr[lo];
        pos = lo + ((target - arr[lo]) * (hi - lo)) / range;
        if (pos < lo || pos > hi) {
            return -1;
        }
        if (arr[pos] == target) {
            return pos;
        }
        if (arr[pos] < target) {
            lo = pos + 1;
        } else {
            hi = pos - 1;
        }
    }
    return -1;
}

int binary_search_first(int arr[], int n, int target) {
    int lo;
    int hi;
    int mid;
    int result;
    lo = 0;
    hi = n - 1;
    result = -1;
    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            result = mid;
            hi = mid - 1;
        } else if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return result;
}

int binary_search_last(int arr[], int n, int target) {
    int lo;
    int hi;
    int mid;
    int result;
    lo = 0;
    hi = n - 1;
    result = -1;
    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            result = mid;
            lo = mid + 1;
        } else if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return result;
}

int main(void) {
    int a[10];
    int r;

    a[0] = 2; a[1] = 5; a[2] = 8; a[3] = 12; a[4] = 16;
    a[5] = 23; a[6] = 38; a[7] = 56; a[8] = 72; a[9] = 91;

    r = linear_search(a, 10, 23);
    // EXPECT: lin_found: 5
    printf("lin_found: %d\n", r);

    r = linear_search(a, 10, 99);
    // EXPECT: lin_miss: -1
    printf("lin_miss: %d\n", r);

    r = linear_search(a, 10, 2);
    // EXPECT: lin_first: 0
    printf("lin_first: %d\n", r);

    r = linear_search(a, 10, 91);
    // EXPECT: lin_last: 9
    printf("lin_last: %d\n", r);

    r = binary_search(a, 10, 38);
    // EXPECT: bin_found: 6
    printf("bin_found: %d\n", r);

    r = binary_search(a, 10, 50);
    // EXPECT: bin_miss: -1
    printf("bin_miss: %d\n", r);

    r = binary_search(a, 10, 2);
    // EXPECT: bin_first: 0
    printf("bin_first: %d\n", r);

    r = binary_search(a, 10, 91);
    // EXPECT: bin_last: 9
    printf("bin_last: %d\n", r);

    r = interpolation_search(a, 10, 56);
    // EXPECT: interp_found: 7
    printf("interp_found: %d\n", r);

    r = interpolation_search(a, 10, 99);
    // EXPECT: interp_miss: -1
    printf("interp_miss: %d\n", r);

    r = sentinel_search(a, 10, 12);
    // EXPECT: sent_found: 3
    printf("sent_found: %d\n", r);

    r = sentinel_search(a, 10, 100);
    // EXPECT: sent_miss: -1
    printf("sent_miss: %d\n", r);

    r = sentinel_search(a, 10, 91);
    // EXPECT: sent_last: 9
    printf("sent_last: %d\n", r);

    a[0] = 1; a[1] = 3; a[2] = 5; a[3] = 5; a[4] = 5;
    a[5] = 7; a[6] = 9; a[7] = 11; a[8] = 13; a[9] = 15;

    r = binary_search_first(a, 10, 5);
    // EXPECT: first5: 2
    printf("first5: %d\n", r);

    r = binary_search_last(a, 10, 5);
    // EXPECT: last5: 4
    printf("last5: %d\n", r);

    r = binary_search_first(a, 10, 99);
    // EXPECT: first_miss: -1
    printf("first_miss: %d\n", r);

    return 0;
}
