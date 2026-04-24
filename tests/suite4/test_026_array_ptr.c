int printf(const char *fmt, ...);

int sum_ptr(int *p, int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        s = s + p[i];
    }
    return s;
}

int sum_bracket(int *p, int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        s = s + *(p + i);
    }
    return s;
}

void copy_array(int *dst, int *src, int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        dst[i] = src[i];
    }
}

void add_arrays(int *dst, int *a, int *b, int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        *(dst + i) = *(a + i) + *(b + i);
    }
}

int equal_arrays(int *a, int *b, int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    return 1;
}

void scale_array(int *a, int n, int factor) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        a[i] = a[i] * factor;
    }
}

int main(void) {
    int arr[5];
    arr[0] = 2;
    arr[1] = 4;
    arr[2] = 6;
    arr[3] = 8;
    arr[4] = 10;

    int *p = arr;

    /* arr[i] and p[i] should be identical */
    // EXPECT: 2
    printf("%d\n", arr[0]);
    // EXPECT: 2
    printf("%d\n", p[0]);

    // EXPECT: 6
    printf("%d\n", arr[2]);
    // EXPECT: 6
    printf("%d\n", p[2]);

    // EXPECT: 10
    printf("%d\n", arr[4]);
    // EXPECT: 10
    printf("%d\n", *(p + 4));

    /* Both sum functions should give same result */
    // EXPECT: 30
    printf("%d\n", sum_ptr(arr, 5));
    // EXPECT: 30
    printf("%d\n", sum_bracket(arr, 5));

    /* Array decay: passing arr is same as passing &arr[0] */
    // EXPECT: 30
    printf("%d\n", sum_ptr(&arr[0], 5));

    /* *(arr + i) == arr[i] */
    // EXPECT: 4
    printf("%d\n", *(arr + 1));
    // EXPECT: 8
    printf("%d\n", *(arr + 3));

    /* Copy array test */
    int copy[5];
    copy_array(copy, arr, 5);
    // EXPECT: 1
    printf("%d\n", equal_arrays(arr, copy, 5));

    // EXPECT: 2
    printf("%d\n", copy[0]);
    // EXPECT: 10
    printf("%d\n", copy[4]);

    /* Modify copy, check original unchanged */
    copy[0] = 99;
    // EXPECT: 2
    printf("%d\n", arr[0]);
    // EXPECT: 99
    printf("%d\n", copy[0]);

    /* Add arrays */
    int a[4];
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    a[3] = 4;
    int b[4];
    b[0] = 10;
    b[1] = 20;
    b[2] = 30;
    b[3] = 40;
    int c[4];
    add_arrays(c, a, b, 4);
    // EXPECT: 11
    printf("%d\n", c[0]);
    // EXPECT: 22
    printf("%d\n", c[1]);
    // EXPECT: 33
    printf("%d\n", c[2]);
    // EXPECT: 44
    printf("%d\n", c[3]);

    /* Scale array */
    scale_array(a, 4, 3);
    // EXPECT: 3
    printf("%d\n", a[0]);
    // EXPECT: 6
    printf("%d\n", a[1]);
    // EXPECT: 12
    printf("%d\n", a[3]);

    /* Pointer to middle of array */
    int *mid = arr + 2;
    // EXPECT: 6
    printf("%d\n", mid[0]);
    // EXPECT: 8
    printf("%d\n", mid[1]);
    // EXPECT: 4
    printf("%d\n", mid[-1]);

    /* sizeof difference: pointer vs array */
    // EXPECT: 8
    printf("%d\n", sizeof(p));
    // EXPECT: 20
    printf("%d\n", sizeof(arr));

    return 0;
}
