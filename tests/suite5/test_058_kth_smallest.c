int printf(const char *fmt, ...);

void swap(int arr[], int i, int j) {
    int tmp;
    tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}

int partition(int arr[], int lo, int hi) {
    int pivot;
    int i;
    int j;
    pivot = arr[hi];
    i = lo;
    j = lo;
    while (j < hi) {
        if (arr[j] <= pivot) {
            swap(arr, i, j);
            i = i + 1;
        }
        j = j + 1;
    }
    swap(arr, i, hi);
    return i;
}

int quickselect(int arr[], int lo, int hi, int k) {
    int p;
    if (lo == hi) {
        return arr[lo];
    }
    p = partition(arr, lo, hi);
    if (k == p) {
        return arr[p];
    }
    if (k < p) {
        return quickselect(arr, lo, p - 1, k);
    }
    return quickselect(arr, p + 1, hi, k);
}

int kth_smallest(int arr[], int n, int k) {
    return quickselect(arr, 0, n - 1, k - 1);
}

int find_min(int arr[], int n) {
    int min;
    int i;
    min = arr[0];
    i = 1;
    while (i < n) {
        if (arr[i] < min) {
            min = arr[i];
        }
        i = i + 1;
    }
    return min;
}

int find_max(int arr[], int n) {
    int max;
    int i;
    max = arr[0];
    i = 1;
    while (i < n) {
        if (arr[i] > max) {
            max = arr[i];
        }
        i = i + 1;
    }
    return max;
}

int find_median(int arr[], int n) {
    return kth_smallest(arr, n, (n + 1) / 2);
}

void copy_array(int src[], int dst[], int n) {
    int i;
    i = 0;
    while (i < n) {
        dst[i] = src[i];
        i = i + 1;
    }
}

int main(void) {
    int a[10];
    int b[10];
    int c[7];
    int d[7];
    int r;

    a[0] = 7; a[1] = 10; a[2] = 4; a[3] = 3; a[4] = 20;
    a[5] = 15; a[6] = 1; a[7] = 8; a[8] = 12; a[9] = 5;

    copy_array(a, b, 10);
    r = kth_smallest(b, 10, 1);
    // EXPECT: k1: 1
    printf("k1: %d\n", r);

    copy_array(a, b, 10);
    r = kth_smallest(b, 10, 2);
    // EXPECT: k2: 3
    printf("k2: %d\n", r);

    copy_array(a, b, 10);
    r = kth_smallest(b, 10, 3);
    // EXPECT: k3: 4
    printf("k3: %d\n", r);

    copy_array(a, b, 10);
    r = kth_smallest(b, 10, 5);
    // EXPECT: k5: 7
    printf("k5: %d\n", r);

    copy_array(a, b, 10);
    r = kth_smallest(b, 10, 10);
    // EXPECT: k10: 20
    printf("k10: %d\n", r);

    r = find_min(a, 10);
    // EXPECT: min: 1
    printf("min: %d\n", r);

    r = find_max(a, 10);
    // EXPECT: max: 20
    printf("max: %d\n", r);

    c[0] = 15; c[1] = 3; c[2] = 9; c[3] = 21; c[4] = 7;
    c[5] = 12; c[6] = 18;
    copy_array(c, d, 7);
    r = find_median(d, 7);
    // EXPECT: median7: 12
    printf("median7: %d\n", r);

    c[0] = 5; c[1] = 5; c[2] = 5; c[3] = 5; c[4] = 5;
    c[5] = 5; c[6] = 5;
    copy_array(c, d, 7);
    r = find_median(d, 7);
    // EXPECT: med_same: 5
    printf("med_same: %d\n", r);

    c[0] = 1; c[1] = 2; c[2] = 3; c[3] = 4; c[4] = 5;
    c[5] = 6; c[6] = 7;
    copy_array(c, d, 7);
    r = kth_smallest(d, 7, 4);
    // EXPECT: mid_sorted: 4
    printf("mid_sorted: %d\n", r);

    c[0] = 100;
    copy_array(c, d, 1);
    r = kth_smallest(d, 1, 1);
    // EXPECT: single: 100
    printf("single: %d\n", r);

    c[0] = 30; c[1] = 10; c[2] = 20;
    copy_array(c, d, 3);
    r = kth_smallest(d, 3, 1);
    // EXPECT: t3_k1: 10
    printf("t3_k1: %d\n", r);

    copy_array(c, d, 3);
    r = kth_smallest(d, 3, 2);
    // EXPECT: t3_k2: 20
    printf("t3_k2: %d\n", r);

    copy_array(c, d, 3);
    r = kth_smallest(d, 3, 3);
    // EXPECT: t3_k3: 30
    printf("t3_k3: %d\n", r);

    a[0] = 50; a[1] = 40; a[2] = 30; a[3] = 20; a[4] = 10;
    a[5] = 60; a[6] = 70; a[7] = 80; a[8] = 90; a[9] = 100;
    copy_array(a, b, 10);
    r = find_median(b, 10);
    // EXPECT: med10: 50
    printf("med10: %d\n", r);

    return 0;
}
