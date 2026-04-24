int printf(const char *fmt, ...);

void bubble_sort(int *arr, int n) {
    int i;
    int j;
    int temp;
    for (i = 0; i < n - 1; i = i + 1) {
        for (j = 0; j < n - 1 - i; j = j + 1) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int is_sorted(int *arr, int n) {
    int i;
    for (i = 0; i < n - 1; i = i + 1) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
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

int count_swaps(int *arr, int n) {
    int i;
    int j;
    int temp;
    int swaps;
    swaps = 0;
    for (i = 0; i < n - 1; i = i + 1) {
        for (j = 0; j < n - 1 - i; j = j + 1) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swaps = swaps + 1;
            }
        }
    }
    return swaps;
}

int main(void) {
    int a[8];
    int b[5];
    int c[6];
    int d[1];
    int e[3];
    int swaps;

    a[0] = 64; a[1] = 34; a[2] = 25; a[3] = 12;
    a[4] = 22; a[5] = 11; a[6] = 90; a[7] = 1;

    // EXPECT: unsorted: 0
    printf("unsorted: %d\n", is_sorted(a, 8));

    bubble_sort(a, 8);
    // EXPECT: sorted: 1
    printf("sorted: %d\n", is_sorted(a, 8));

    // EXPECT: arr: 1 11 12 22 25 34 64 90
    printf("arr: ");
    print_array(a, 8);
    printf("\n");

    // EXPECT: first: 1
    printf("first: %d\n", a[0]);
    // EXPECT: last: 90
    printf("last: %d\n", a[7]);

    b[0] = 5; b[1] = 4; b[2] = 3; b[3] = 2; b[4] = 1;
    swaps = count_swaps(b, 5);
    // EXPECT: reverse swaps: 10
    printf("reverse swaps: %d\n", swaps);

    // EXPECT: rev sorted: 1 2 3 4 5
    printf("rev sorted: ");
    print_array(b, 5);
    printf("\n");

    c[0] = 1; c[1] = 2; c[2] = 3; c[3] = 4; c[4] = 5; c[5] = 6;
    swaps = count_swaps(c, 6);
    // EXPECT: already sorted swaps: 0
    printf("already sorted swaps: %d\n", swaps);

    d[0] = 42;
    bubble_sort(d, 1);
    // EXPECT: single: 42
    printf("single: %d\n", d[0]);

    e[0] = 3; e[1] = 3; e[2] = 3;
    bubble_sort(e, 3);
    // EXPECT: equal: 3 3 3
    printf("equal: ");
    print_array(e, 3);
    printf("\n");

    e[0] = -5; e[1] = 10; e[2] = -3;
    bubble_sort(e, 3);
    // EXPECT: neg: -5 -3 10
    printf("neg: ");
    print_array(e, 3);
    printf("\n");

    // EXPECT: neg first: -5
    printf("neg first: %d\n", e[0]);
    // EXPECT: neg last: 10
    printf("neg last: %d\n", e[2]);

    a[0] = 100; a[1] = 50; a[2] = 75; a[3] = 25;
    a[4] = 0; a[5] = 99; a[6] = 1; a[7] = 50;
    bubble_sort(a, 8);
    // EXPECT: mixed: 0 1 25 50 50 75 99 100
    printf("mixed: ");
    print_array(a, 8);
    printf("\n");

    // EXPECT: mid: 50
    printf("mid: %d\n", a[3]);

    // EXPECT: done
    printf("done\n");

    return 0;
}
