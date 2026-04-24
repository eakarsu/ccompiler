int printf(const char *fmt, ...);
// EXPECT: before: 64 25 12 22 11 90 45 78 33 55\nafter: 11 12 22 25 33 45 55 64 78 90\nsorted: 1\nalready: 1 2 3 4 5 6 7 8\nreversed: 1 2 3 4 5 6 7 8\ndupes: 1 1 2 2 3 3\nlarge: 2 7 9 13 18 23 32 39 45 50 55 61 66 74 88\nsorted: 1
// Test 213: Recursive quicksort

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int *arr, int lo, int hi) {
    int pivot = arr[hi];
    int i = lo - 1;
    int j = lo;
    while (j < hi) {
        if (arr[j] <= pivot) {
            i = i + 1;
            swap(&arr[i], &arr[j]);
        }
        j = j + 1;
    }
    swap(&arr[i + 1], &arr[hi]);
    return i + 1;
}

void quicksort(int *arr, int lo, int hi) {
    if (lo >= hi) return;
    int p = partition(arr, lo, hi);
    quicksort(arr, lo, p - 1);
    quicksort(arr, p + 1, hi);
}

void print_array(int *arr, int n) {
    int i = 0;
    while (i < n) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
        i = i + 1;
    }
    printf("\n");
}

int is_sorted(int *arr, int n) {
    int i = 1;
    while (i < n) {
        if (arr[i] < arr[i - 1]) return 0;
        i = i + 1;
    }
    return 1;
}

int main(void) {
    int a[10];
    a[0] = 64; a[1] = 25; a[2] = 12; a[3] = 22; a[4] = 11;
    a[5] = 90; a[6] = 45; a[7] = 78; a[8] = 33; a[9] = 55;
    printf("before: ");
    print_array(a, 10);
    quicksort(a, 0, 9);
    printf("after: ");
    print_array(a, 10);
    printf("sorted: %d\n", is_sorted(a, 10));

    int b[8];
    b[0] = 1; b[1] = 2; b[2] = 3; b[3] = 4; b[4] = 5; b[5] = 6; b[6] = 7; b[7] = 8;
    quicksort(b, 0, 7);
    printf("already: ");
    print_array(b, 8);

    int c[8];
    c[0] = 8; c[1] = 7; c[2] = 6; c[3] = 5; c[4] = 4; c[5] = 3; c[6] = 2; c[7] = 1;
    quicksort(c, 0, 7);
    printf("reversed: ");
    print_array(c, 8);

    int d[6];
    d[0] = 3; d[1] = 3; d[2] = 1; d[3] = 1; d[4] = 2; d[5] = 2;
    quicksort(d, 0, 5);
    printf("dupes: ");
    print_array(d, 6);

    int e[15];
    e[0] = 50; e[1] = 23; e[2] = 9; e[3] = 18; e[4] = 61;
    e[5] = 32; e[6] = 45; e[7] = 7; e[8] = 74; e[9] = 13;
    e[10] = 88; e[11] = 39; e[12] = 2; e[13] = 66; e[14] = 55;
    quicksort(e, 0, 14);
    printf("large: ");
    print_array(e, 15);
    printf("sorted: %d\n", is_sorted(e, 15));

    return 0;
}
