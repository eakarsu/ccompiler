int printf(const char *fmt, ...);

void bubble_sort(int *arr, int n) {
    int i;
    int j;
    for (i = 0; i < n - 1; i = i + 1) {
        for (j = 0; j < n - 1 - i; j = j + 1) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

int linear_search(int *arr, int n, int target) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

int binary_search(int *arr, int n, int target) {
    int lo = 0;
    int hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return -1;
}

void reverse_array(int *arr, int n) {
    int i = 0;
    int j = n - 1;
    while (i < j) {
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
        i = i + 1;
        j = j - 1;
    }
}

int sum_array(int *arr, int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        s = s + arr[i];
    }
    return s;
}

int max_element(int *arr, int n) {
    int m = arr[0];
    int i;
    for (i = 1; i < n; i = i + 1) {
        if (arr[i] > m) {
            m = arr[i];
        }
    }
    return m;
}

int min_element(int *arr, int n) {
    int m = arr[0];
    int i;
    for (i = 1; i < n; i = i + 1) {
        if (arr[i] < m) {
            m = arr[i];
        }
    }
    return m;
}

int count_value(int *arr, int n, int val) {
    int c = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (arr[i] == val) {
            c = c + 1;
        }
    }
    return c;
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

void selection_sort(int *arr, int n) {
    int i;
    int j;
    for (i = 0; i < n - 1; i = i + 1) {
        int min_idx = i;
        for (j = i + 1; j < n; j = j + 1) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            int tmp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = tmp;
        }
    }
}

void rotate_left(int *arr, int n) {
    int first = arr[0];
    int i;
    for (i = 0; i < n - 1; i = i + 1) {
        arr[i] = arr[i + 1];
    }
    arr[n - 1] = first;
}

int main(void) {
    /* Bubble sort test */
    int a[6];
    a[0] = 5;
    a[1] = 3;
    a[2] = 8;
    a[3] = 1;
    a[4] = 9;
    a[5] = 2;

    // EXPECT: 0
    printf("%d\n", is_sorted(a, 6));

    bubble_sort(a, 6);
    // EXPECT: 1
    printf("%d\n", a[0]);
    // EXPECT: 2
    printf("%d\n", a[1]);
    // EXPECT: 3
    printf("%d\n", a[2]);
    // EXPECT: 5
    printf("%d\n", a[3]);
    // EXPECT: 8
    printf("%d\n", a[4]);
    // EXPECT: 9
    printf("%d\n", a[5]);

    // EXPECT: 1
    printf("%d\n", is_sorted(a, 6));

    /* Linear search */
    // EXPECT: 2
    printf("%d\n", linear_search(a, 6, 3));
    // EXPECT: 5
    printf("%d\n", linear_search(a, 6, 9));
    // EXPECT: -1
    printf("%d\n", linear_search(a, 6, 7));

    /* Binary search on sorted array */
    // EXPECT: 0
    printf("%d\n", binary_search(a, 6, 1));
    // EXPECT: 4
    printf("%d\n", binary_search(a, 6, 8));
    // EXPECT: -1
    printf("%d\n", binary_search(a, 6, 4));

    /* Reverse */
    int b[5];
    b[0] = 10;
    b[1] = 20;
    b[2] = 30;
    b[3] = 40;
    b[4] = 50;
    reverse_array(b, 5);
    // EXPECT: 50
    printf("%d\n", b[0]);
    // EXPECT: 30
    printf("%d\n", b[2]);
    // EXPECT: 10
    printf("%d\n", b[4]);

    /* Sum, max, min */
    // EXPECT: 150
    printf("%d\n", sum_array(b, 5));
    // EXPECT: 50
    printf("%d\n", max_element(b, 5));
    // EXPECT: 10
    printf("%d\n", min_element(b, 5));

    /* Count value */
    int c[7];
    c[0] = 1;
    c[1] = 3;
    c[2] = 3;
    c[3] = 2;
    c[4] = 3;
    c[5] = 1;
    c[6] = 2;
    // EXPECT: 3
    printf("%d\n", count_value(c, 7, 3));
    // EXPECT: 2
    printf("%d\n", count_value(c, 7, 1));
    // EXPECT: 0
    printf("%d\n", count_value(c, 7, 5));

    /* Selection sort */
    int d[5];
    d[0] = 44;
    d[1] = 11;
    d[2] = 55;
    d[3] = 22;
    d[4] = 33;
    selection_sort(d, 5);
    // EXPECT: 11
    printf("%d\n", d[0]);
    // EXPECT: 22
    printf("%d\n", d[1]);
    // EXPECT: 55
    printf("%d\n", d[4]);
    // EXPECT: 1
    printf("%d\n", is_sorted(d, 5));

    /* Rotate left */
    int e[4];
    e[0] = 1;
    e[1] = 2;
    e[2] = 3;
    e[3] = 4;
    rotate_left(e, 4);
    // EXPECT: 2
    printf("%d\n", e[0]);
    // EXPECT: 3
    printf("%d\n", e[1]);
    // EXPECT: 4
    printf("%d\n", e[2]);
    // EXPECT: 1
    printf("%d\n", e[3]);

    /* Rotate left 3 more times to get back to original */
    rotate_left(e, 4);
    rotate_left(e, 4);
    rotate_left(e, 4);
    // EXPECT: 1
    printf("%d\n", e[0]);
    // EXPECT: 4
    printf("%d\n", e[3]);

    return 0;
}
