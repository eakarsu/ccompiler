int printf(const char *fmt, ...);

/* test_022_array_basics.c
   Tests: 1D arrays, initialization, indexing, array as pointer,
          arrays of different sizes, partial init with zeros
*/

int sum(int *a, int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i++) {
        s += a[i];
    }
    return s;
}

int max_val(int *a, int n) {
    int m = a[0];
    int i;
    for (i = 1; i < n; i++) {
        if (a[i] > m) m = a[i];
    }
    return m;
}

int min_val(int *a, int n) {
    int m = a[0];
    int i;
    for (i = 1; i < n; i++) {
        if (a[i] < m) m = a[i];
    }
    return m;
}

void fill(int *a, int n, int val) {
    int i;
    for (i = 0; i < n; i++) a[i] = val;
}

int main(void) {
    /* basic 1D array declaration and init */
    int a[5] = {1, 2, 3, 4, 5};
    printf("%d\n", a[0]);
    // EXPECT: 1
    printf("%d\n", a[4]);
    // EXPECT: 5

    /* modification */
    a[2] = 99;
    printf("%d\n", a[2]);
    // EXPECT: 99

    /* sum via function (array decays to pointer) */
    int b[5] = {10, 20, 30, 40, 50};
    printf("%d\n", sum(b, 5));
    // EXPECT: 150

    /* max and min */
    printf("%d\n", max_val(b, 5));
    // EXPECT: 50
    printf("%d\n", min_val(b, 5));
    // EXPECT: 10

    /* partial initialization: rest zeroed */
    int c[8] = {3, 6};
    printf("%d\n", c[0]);
    // EXPECT: 3
    printf("%d\n", c[1]);
    // EXPECT: 6
    printf("%d\n", c[2]);
    // EXPECT: 0
    printf("%d\n", c[7]);
    // EXPECT: 0

    /* zero init with {0} */
    int z[6] = {0};
    int i;
    int all_zero = 1;
    for (i = 0; i < 6; i++) {
        if (z[i] != 0) all_zero = 0;
    }
    printf("%d\n", all_zero);
    // EXPECT: 1

    /* array of size 1 */
    int single[1] = {42};
    printf("%d\n", single[0]);
    // EXPECT: 42

    /* large array filled via function */
    int big[10];
    fill(big, 10, 7);
    printf("%d\n", big[0]);
    // EXPECT: 7
    printf("%d\n", big[9]);
    // EXPECT: 7
    printf("%d\n", sum(big, 10));
    // EXPECT: 70

    /* array indexing via pointer */
    int *p = b;
    printf("%d\n", p[0]);
    // EXPECT: 10
    printf("%d\n", p[3]);
    // EXPECT: 40

    /* iterate with index */
    int evens[5] = {2, 4, 6, 8, 10};
    int total = 0;
    for (i = 0; i < 5; i++) total += evens[i];
    printf("%d\n", total);
    // EXPECT: 30

    /* reverse traversal */
    int last = evens[4];
    int second_last = evens[3];
    printf("%d\n", last);
    // EXPECT: 10
    printf("%d\n", second_last);
    // EXPECT: 8

    /* char array as byte store */
    char bytes[4] = {10, 20, 30, 40};
    printf("%d\n", (int)bytes[0]);
    // EXPECT: 10
    printf("%d\n", (int)bytes[3]);
    // EXPECT: 40

    /* array length inferred from initializer */
    int inferred[] = {5, 10, 15, 20};
    printf("%d\n", inferred[0]);
    // EXPECT: 5
    printf("%d\n", inferred[3]);
    // EXPECT: 20

    /* nested indexing: compute manually */
    int mat[3] = {2, 3, 4};
    int result = mat[0] * mat[1] + mat[2];
    printf("%d\n", result);
    // EXPECT: 10

    /* write through pointer then read via array name */
    int arr[4] = {1, 1, 1, 1};
    int *pa = arr;
    pa[0] = 9;
    pa[2] = 8;
    printf("%d\n", arr[0]);
    // EXPECT: 9
    printf("%d\n", arr[2]);
    // EXPECT: 8
    printf("%d\n", arr[1]);
    // EXPECT: 1

    /* sum using pointer arithmetic */
    int nums[6] = {1, 2, 3, 4, 5, 6};
    int s2 = 0;
    int *ptr = nums;
    int *end = nums + 6;
    while (ptr < end) {
        s2 += *ptr;
        ptr++;
    }
    printf("%d\n", s2);
    // EXPECT: 21

    /* comparison of array elements */
    int cmp[3] = {5, 5, 6};
    if (cmp[0] == cmp[1]) printf("eq\n");
    // EXPECT: eq
    if (cmp[1] < cmp[2])  printf("lt\n");
    // EXPECT: lt

    return 0;
}
