int printf(const char *fmt, ...);

void selection_sort(int *arr, int n) {
    int i;
    int j;
    int min_idx;
    int temp;
    for (i = 0; i < n - 1; i = i + 1) {
        min_idx = i;
        for (j = i + 1; j < n; j = j + 1) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }
}

int find_min(int *arr, int n) {
    int i;
    int m;
    m = arr[0];
    for (i = 1; i < n; i = i + 1) {
        if (arr[i] < m) {
            m = arr[i];
        }
    }
    return m;
}

int find_max(int *arr, int n) {
    int i;
    int m;
    m = arr[0];
    for (i = 1; i < n; i = i + 1) {
        if (arr[i] > m) {
            m = arr[i];
        }
    }
    return m;
}

void print_array(int *arr, int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", arr[i]);
    }
}

int count_comparisons(int n) {
    return n * (n - 1) / 2;
}

int main(void) {
    int a[8];
    int b[5];
    int c[6];
    int d[4];

    a[0] = 64; a[1] = 25; a[2] = 12; a[3] = 22;
    a[4] = 11; a[5] = 90; a[6] = 34; a[7] = 1;

    // EXPECT: min before: 1
    printf("min before: %d\n", find_min(a, 8));
    // EXPECT: max before: 90
    printf("max before: %d\n", find_max(a, 8));

    selection_sort(a, 8);

    // EXPECT: sorted: 1 11 12 22 25 34 64 90
    printf("sorted: ");
    print_array(a, 8);
    printf("\n");

    // EXPECT: a0: 1
    printf("a0: %d\n", a[0]);
    // EXPECT: a7: 90
    printf("a7: %d\n", a[7]);
    // EXPECT: a3: 22
    printf("a3: %d\n", a[3]);

    b[0] = 1; b[1] = 2; b[2] = 3; b[3] = 4; b[4] = 5;
    selection_sort(b, 5);
    // EXPECT: already: 1 2 3 4 5
    printf("already: ");
    print_array(b, 5);
    printf("\n");

    b[0] = 5; b[1] = 4; b[2] = 3; b[3] = 2; b[4] = 1;
    selection_sort(b, 5);
    // EXPECT: reversed: 1 2 3 4 5
    printf("reversed: ");
    print_array(b, 5);
    printf("\n");

    c[0] = -10; c[1] = 5; c[2] = -3; c[3] = 0; c[4] = 8; c[5] = -1;
    selection_sort(c, 6);
    // EXPECT: negatives: -10 -3 -1 0 5 8
    printf("negatives: ");
    print_array(c, 6);
    printf("\n");

    // EXPECT: neg min: -10
    printf("neg min: %d\n", c[0]);
    // EXPECT: neg max: 8
    printf("neg max: %d\n", c[5]);

    d[0] = 7; d[1] = 7; d[2] = 7; d[3] = 7;
    selection_sort(d, 4);
    // EXPECT: equal: 7 7 7 7
    printf("equal: ");
    print_array(d, 4);
    printf("\n");

    // EXPECT: comps 8: 28
    printf("comps 8: %d\n", count_comparisons(8));
    // EXPECT: comps 5: 10
    printf("comps 5: %d\n", count_comparisons(5));
    // EXPECT: comps 1: 0
    printf("comps 1: %d\n", count_comparisons(1));

    d[0] = 42;
    selection_sort(d, 1);
    // EXPECT: single: 42
    printf("single: %d\n", d[0]);

    a[0] = 99; a[1] = 1; a[2] = 50; a[3] = 25;
    a[4] = 75; a[5] = 10; a[6] = 60; a[7] = 30;
    selection_sort(a, 8);
    // EXPECT: mixed: 1 10 25 30 50 60 75 99
    printf("mixed: ");
    print_array(a, 8);
    printf("\n");

    // EXPECT: done
    printf("done\n");

    return 0;
}
