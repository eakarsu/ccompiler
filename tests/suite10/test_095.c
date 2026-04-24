int printf(const char *fmt, ...);

// Array of 1000 elements with computation

int arr[1000];

void fill_sequential(int *a, int n) {
    int i = 0;
    while (i < n) {
        a[i] = i + 1;
        i = i + 1;
    }
}

void fill_squares(int *a, int n) {
    int i = 0;
    while (i < n) {
        a[i] = i * i;
        i = i + 1;
    }
}

int sum_array(int *a, int n) {
    int total = 0;
    int i = 0;
    while (i < n) {
        total = total + a[i];
        i = i + 1;
    }
    return total;
}

int sum_even_indices(int *a, int n) {
    int total = 0;
    int i = 0;
    while (i < n) {
        total = total + a[i];
        i = i + 2;
    }
    return total;
}

int max_array(int *a, int n) {
    int mx = a[0];
    int i = 1;
    while (i < n) {
        if (a[i] > mx) mx = a[i];
        i = i + 1;
    }
    return mx;
}

int min_array(int *a, int n) {
    int mn = a[0];
    int i = 1;
    while (i < n) {
        if (a[i] < mn) mn = a[i];
        i = i + 1;
    }
    return mn;
}

int count_divisible(int *a, int n, int d) {
    int cnt = 0;
    int i = 0;
    while (i < n) {
        if (a[i] % d == 0) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

void reverse_array(int *a, int n) {
    int lo = 0;
    int hi = n - 1;
    while (lo < hi) {
        int tmp = a[lo];
        a[lo] = a[hi];
        a[hi] = tmp;
        lo = lo + 1;
        hi = hi - 1;
    }
}

void bubble_sort_partial(int *a, int n) {
    int i = 0;
    while (i < n - 1) {
        int j = 0;
        while (j < n - 1 - i) {
            if (a[j] > a[j + 1]) {
                int tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
            }
            j = j + 1;
        }
        i = i + 1;
    }
}

int binary_search(int *a, int n, int target) {
    int lo = 0;
    int hi = n - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (a[mid] == target) return mid;
        if (a[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

int main() {
    fill_sequential(arr, 1000);

    int s = sum_array(arr, 1000);
    printf("sum 1-1000: %d\n", s);
    // EXPECT: sum 1-1000: 500500

    int se = sum_even_indices(arr, 1000);
    printf("sum even idx: %d\n", se);
    // EXPECT: sum even idx: 250000

    int mx = max_array(arr, 1000);
    printf("max: %d\n", mx);
    // EXPECT: max: 1000

    int mn = min_array(arr, 1000);
    printf("min: %d\n", mn);
    // EXPECT: min: 1

    int cd3 = count_divisible(arr, 1000, 3);
    printf("div by 3: %d\n", cd3);
    // EXPECT: div by 3: 333

    int cd7 = count_divisible(arr, 1000, 7);
    printf("div by 7: %d\n", cd7);
    // EXPECT: div by 7: 142

    int pos = binary_search(arr, 1000, 500);
    printf("bsearch 500: %d\n", pos);
    // EXPECT: bsearch 500: 499

    pos = binary_search(arr, 1000, 1);
    printf("bsearch 1: %d\n", pos);
    // EXPECT: bsearch 1: 0

    pos = binary_search(arr, 1000, 1001);
    printf("bsearch 1001: %d\n", pos);
    // EXPECT: bsearch 1001: -1

    reverse_array(arr, 1000);
    printf("after rev first: %d\n", arr[0]);
    // EXPECT: after rev first: 1000
    printf("after rev last: %d\n", arr[999]);
    // EXPECT: after rev last: 1

    int small[20];
    int k = 0;
    while (k < 20) {
        small[k] = 20 - k;
        k = k + 1;
    }
    bubble_sort_partial(small, 20);
    printf("sorted first: %d\n", small[0]);
    // EXPECT: sorted first: 1
    printf("sorted last: %d\n", small[19]);
    // EXPECT: sorted last: 20

    fill_squares(arr, 100);
    int sq_sum = sum_array(arr, 100);
    printf("sum squares 0-99: %d\n", sq_sum);
    // EXPECT: sum squares 0-99: 328350

    return 0;
}
