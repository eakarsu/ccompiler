int printf(const char *fmt, ...);

void swap(int arr[], int i, int j) {
    int tmp;
    tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}

int median_of_three(int arr[], int lo, int hi) {
    int mid;
    mid = lo + (hi - lo) / 2;
    if (arr[lo] > arr[mid]) {
        swap(arr, lo, mid);
    }
    if (arr[lo] > arr[hi]) {
        swap(arr, lo, hi);
    }
    if (arr[mid] > arr[hi]) {
        swap(arr, mid, hi);
    }
    return mid;
}

int lomuto_partition(int arr[], int lo, int hi) {
    int pivot;
    int i;
    int j;
    pivot = arr[hi];
    i = lo;
    j = lo;
    while (j < hi) {
        if (arr[j] < pivot) {
            swap(arr, i, j);
            i = i + 1;
        }
        j = j + 1;
    }
    swap(arr, i, hi);
    return i;
}

void quicksort(int arr[], int lo, int hi) {
    int pivot_idx;
    int p;
    if (lo < hi) {
        pivot_idx = median_of_three(arr, lo, hi);
        swap(arr, pivot_idx, hi);
        p = lomuto_partition(arr, lo, hi);
        quicksort(arr, lo, p - 1);
        quicksort(arr, p + 1, hi);
    }
}

int is_sorted(int arr[], int n) {
    int i;
    i = 0;
    while (i < n - 1) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
        i = i + 1;
    }
    return 1;
}

void print_array(int arr[], int n) {
    int i;
    i = 0;
    while (i < n) {
        printf("%d ", arr[i]);
        i = i + 1;
    }
}

int main(void) {
    int a[10];
    int b[8];
    int c[5];
    int d[1];
    int e[6];
    int f[3];

    a[0] = 64; a[1] = 25; a[2] = 12; a[3] = 22; a[4] = 11;
    a[5] = 90; a[6] = 33; a[7] = 45; a[8] = 77; a[9] = 55;
    quicksort(a, 0, 9);
    // EXPECT: qs10: 1
    printf("qs10: %d\n", is_sorted(a, 10));
    // EXPECT: 11 12 22 25 33 45 55 64 77 90 done
    print_array(a, 10);
    printf("done\n");

    b[0] = 8; b[1] = 7; b[2] = 6; b[3] = 5;
    b[4] = 4; b[5] = 3; b[6] = 2; b[7] = 1;
    quicksort(b, 0, 7);
    // EXPECT: rev8: 1
    printf("rev8: %d\n", is_sorted(b, 8));
    // EXPECT: 1 2 3 4 5 6 7 8 done
    print_array(b, 8);
    printf("done\n");

    c[0] = 1; c[1] = 2; c[2] = 3; c[3] = 4; c[4] = 5;
    quicksort(c, 0, 4);
    // EXPECT: already5: 1
    printf("already5: %d\n", is_sorted(c, 5));

    d[0] = 42;
    quicksort(d, 0, 0);
    // EXPECT: single: 42
    printf("single: %d\n", d[0]);

    e[0] = 3; e[1] = 3; e[2] = 1; e[3] = 1; e[4] = 2; e[5] = 2;
    quicksort(e, 0, 5);
    // EXPECT: dups6: 1
    printf("dups6: %d\n", is_sorted(e, 6));
    // EXPECT: 1 1 2 2 3 3 done
    print_array(e, 6);
    printf("done\n");

    a[0] = -10; a[1] = 5; a[2] = -3; a[3] = 8; a[4] = -1;
    a[5] = 0; a[6] = 7; a[7] = -8; a[8] = 2; a[9] = -5;
    quicksort(a, 0, 9);
    // EXPECT: neg10: 1
    printf("neg10: %d\n", is_sorted(a, 10));
    // EXPECT: -10 -8 -5 -3 -1 0 2 5 7 8 done
    print_array(a, 10);
    printf("done\n");

    f[0] = 30; f[1] = 10; f[2] = 20;
    quicksort(f, 0, 2);
    // EXPECT: three: 10 20 30
    printf("three: %d %d %d\n", f[0], f[1], f[2]);

    a[0] = 5; a[1] = 5; a[2] = 5; a[3] = 5; a[4] = 5;
    a[5] = 5; a[6] = 5; a[7] = 5; a[8] = 5; a[9] = 5;
    quicksort(a, 0, 9);
    // EXPECT: allsame: 1
    printf("allsame: %d\n", is_sorted(a, 10));
    // EXPECT: 5 5 5 5 5 5 5 5 5 5 done
    print_array(a, 10);
    printf("done\n");

    f[0] = 2; f[1] = 1;
    quicksort(f, 0, 1);
    // EXPECT: pair: 1 2
    printf("pair: %d %d\n", f[0], f[1]);

    a[0] = 1; a[1] = 100; a[2] = 50; a[3] = 25; a[4] = 75;
    quicksort(a, 0, 4);
    // EXPECT: spread: 1
    printf("spread: %d\n", is_sorted(a, 5));

    return 0;
}
