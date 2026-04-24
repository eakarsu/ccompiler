int printf(const char *fmt, ...);

void merge(int arr[], int temp[], int left, int mid, int right) {
    int i;
    int j;
    int k;
    i = left;
    j = mid;
    k = left;
    while (i < mid && j < right) {
        if (arr[i] <= arr[j]) {
            temp[k] = arr[i];
            i = i + 1;
        } else {
            temp[k] = arr[j];
            j = j + 1;
        }
        k = k + 1;
    }
    while (i < mid) {
        temp[k] = arr[i];
        i = i + 1;
        k = k + 1;
    }
    while (j < right) {
        temp[k] = arr[j];
        j = j + 1;
        k = k + 1;
    }
    i = left;
    while (i < right) {
        arr[i] = temp[i];
        i = i + 1;
    }
}

void merge_sort(int arr[], int temp[], int n) {
    int width;
    int left;
    int mid;
    int right;
    width = 1;
    while (width < n) {
        left = 0;
        while (left < n) {
            mid = left + width;
            if (mid > n) {
                mid = n;
            }
            right = left + width + width;
            if (right > n) {
                right = n;
            }
            if (mid < right) {
                merge(arr, temp, left, mid, right);
            }
            left = left + width + width;
        }
        width = width * 2;
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
    int temp[10];
    int b[8];
    int tb[8];
    int c[5];
    int tc[5];
    int d[1];
    int td[1];
    int e[7];
    int te[7];

    a[0] = 38; a[1] = 27; a[2] = 43; a[3] = 3; a[4] = 9;
    a[5] = 82; a[6] = 10; a[7] = 1; a[8] = 50; a[9] = 25;
    merge_sort(a, temp, 10);
    // EXPECT: sorted10: 1
    printf("sorted10: %d\n", is_sorted(a, 10));
    // EXPECT: 1 3 9 10 25 27 38 43 50 82 done
    print_array(a, 10);
    printf("done\n");

    b[0] = 5; b[1] = 4; b[2] = 3; b[3] = 2;
    b[4] = 1; b[5] = 0; b[6] = -1; b[7] = -2;
    merge_sort(b, tb, 8);
    // EXPECT: rev8: 1
    printf("rev8: %d\n", is_sorted(b, 8));
    // EXPECT: -2 -1 0 1 2 3 4 5 done
    print_array(b, 8);
    printf("done\n");

    c[0] = 1; c[1] = 2; c[2] = 3; c[3] = 4; c[4] = 5;
    merge_sort(c, tc, 5);
    // EXPECT: already: 1
    printf("already: %d\n", is_sorted(c, 5));
    // EXPECT: 1 2 3 4 5 done
    print_array(c, 5);
    printf("done\n");

    d[0] = 99;
    merge_sort(d, td, 1);
    // EXPECT: single: 99
    printf("single: %d\n", d[0]);

    c[0] = 7; c[1] = 7; c[2] = 7; c[3] = 7; c[4] = 7;
    merge_sort(c, tc, 5);
    // EXPECT: allsame: 1
    printf("allsame: %d\n", is_sorted(c, 5));
    // EXPECT: 7 7 7 7 7 done
    print_array(c, 5);
    printf("done\n");

    e[0] = 100; e[1] = -100; e[2] = 50; e[3] = -50;
    e[4] = 0; e[5] = 75; e[6] = -75;
    merge_sort(e, te, 7);
    // EXPECT: mixed7: 1
    printf("mixed7: %d\n", is_sorted(e, 7));
    // EXPECT: -100 -75 -50 0 50 75 100 done
    print_array(e, 7);
    printf("done\n");

    a[0] = 3; a[1] = 1; a[2] = 4; a[3] = 1; a[4] = 5;
    a[5] = 9; a[6] = 2; a[7] = 6; a[8] = 5; a[9] = 3;
    merge_sort(a, temp, 10);
    // EXPECT: dups: 1
    printf("dups: %d\n", is_sorted(a, 10));
    // EXPECT: 1 1 2 3 3 4 5 5 6 9 done
    print_array(a, 10);
    printf("done\n");

    b[0] = 2; b[1] = 1;
    merge_sort(b, tb, 2);
    // EXPECT: two: 1 2
    printf("two: %d %d\n", b[0], b[1]);

    c[0] = 3; c[1] = 1; c[2] = 2;
    merge_sort(c, tc, 3);
    // EXPECT: three: 1 2 3
    printf("three: %d %d %d\n", c[0], c[1], c[2]);

    return 0;
}
