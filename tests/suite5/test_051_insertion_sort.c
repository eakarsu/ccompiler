int printf(const char *fmt, ...);

void insertion_sort(int arr[], int n) {
    int i;
    int j;
    int key;
    i = 1;
    while (i < n) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
        i = i + 1;
    }
}

int is_nearly_sorted(int arr[], int n) {
    int count;
    int i;
    count = 0;
    i = 0;
    while (i < n - 1) {
        if (arr[i] > arr[i + 1]) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

void insertion_sort_optimized(int arr[], int n) {
    int i;
    int j;
    int key;
    int swaps;
    swaps = 0;
    i = 1;
    while (i < n) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
            swaps = swaps + 1;
        }
        arr[j + 1] = key;
        i = i + 1;
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
    int a[8];
    int b[5];
    int c[6];
    int d[10];
    int e[4];
    int i;
    int inv;

    a[0] = 5; a[1] = 2; a[2] = 8; a[3] = 1;
    a[4] = 9; a[5] = 3; a[6] = 7; a[7] = 4;

    insertion_sort(a, 8);
    // EXPECT: sorted: 1
    printf("sorted: %d\n", is_sorted(a, 8));

    // EXPECT: 1 2 3 4 5 7 8 9 done
    print_array(a, 8);
    printf("done\n");

    b[0] = 1; b[1] = 2; b[2] = 3; b[3] = 4; b[4] = 5;
    inv = is_nearly_sorted(b, 5);
    // EXPECT: inversions: 0
    printf("inversions: %d\n", inv);

    b[0] = 1; b[1] = 3; b[2] = 2; b[3] = 4; b[4] = 5;
    inv = is_nearly_sorted(b, 5);
    // EXPECT: inversions: 1
    printf("inversions: %d\n", inv);

    b[0] = 5; b[1] = 4; b[2] = 3; b[3] = 2; b[4] = 1;
    inv = is_nearly_sorted(b, 5);
    // EXPECT: inversions: 4
    printf("inversions: %d\n", inv);

    insertion_sort(b, 5);
    // EXPECT: sorted_rev: 1
    printf("sorted_rev: %d\n", is_sorted(b, 5));
    // EXPECT: 1 2 3 4 5 done
    print_array(b, 5);
    printf("done\n");

    c[0] = 3; c[1] = 3; c[2] = 1; c[3] = 1; c[4] = 2; c[5] = 2;
    insertion_sort(c, 6);
    // EXPECT: dup_sorted: 1
    printf("dup_sorted: %d\n", is_sorted(c, 6));
    // EXPECT: 1 1 2 2 3 3 done
    print_array(c, 6);
    printf("done\n");

    d[0] = 42;
    insertion_sort(d, 1);
    // EXPECT: single: 42
    printf("single: %d\n", d[0]);

    e[0] = 10; e[1] = 10; e[2] = 10; e[3] = 10;
    insertion_sort(e, 4);
    // EXPECT: all_same: 1
    printf("all_same: %d\n", is_sorted(e, 4));
    // EXPECT: 10 10 10 10 done
    print_array(e, 4);
    printf("done\n");

    d[0] = 100; d[1] = 90; d[2] = 80; d[3] = 70; d[4] = 60;
    d[5] = 50; d[6] = 40; d[7] = 30; d[8] = 20; d[9] = 10;
    insertion_sort(d, 10);
    // EXPECT: worst_sorted: 1
    printf("worst_sorted: %d\n", is_sorted(d, 10));
    // EXPECT: 10 20 30 40 50 60 70 80 90 100 done
    print_array(d, 10);
    printf("done\n");

    d[0] = -5; d[1] = 3; d[2] = -1; d[3] = 7; d[4] = -3;
    insertion_sort(d, 5);
    // EXPECT: neg_sorted: 1
    printf("neg_sorted: %d\n", is_sorted(d, 5));
    // EXPECT: -5 -3 -1 3 7 done
    print_array(d, 5);
    printf("done\n");

    return 0;
}
