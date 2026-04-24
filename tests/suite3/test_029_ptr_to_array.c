int printf(const char *fmt, ...);

/* test_029_ptr_to_array.c
   Tests: pointer to array type int (*p)[N], multidimensional access via
          pointer-to-array, array parameter decay rules, row pointer
          iteration over 2D arrays
*/

/* Accept pointer to array-of-4: standard 2D array passing */
int row_sum_4(int (*p)[4], int nrows) {
    int s = 0;
    int i, j;
    for (i = 0; i < nrows; i++)
        for (j = 0; j < 4; j++)
            s += p[i][j];
    return s;
}

/* Accept pointer to array-of-3 */
void print_row3(int (*p)[3]) {
    printf("%d %d %d\n", (*p)[0], (*p)[1], (*p)[2]);
}

/* Return pointer to a specific row of a 2D array */
int *get_row(int (*mat)[5], int r) {
    return mat[r];
}

/* Sum a 1D array via int* (array decays) */
int sum1d(int *p, int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i++) s += p[i];
    return s;
}

/* Transpose a 3x3 matrix into another 3x3 */
void transpose3(int (*src)[3], int (*dst)[3]) {
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            dst[j][i] = src[i][j];
}

int main(void) {
    /* pointer to array of 4 */
    int a[3][4] = {
        {1,  2,  3,  4},
        {5,  6,  7,  8},
        {9, 10, 11, 12}
    };
    int (*p4)[4] = a;

    printf("%d\n", (*p4)[0]);
    // EXPECT: 1
    printf("%d\n", (*p4)[3]);
    // EXPECT: 4

    /* advance pointer to next row */
    p4++;
    printf("%d\n", (*p4)[0]);
    // EXPECT: 5
    printf("%d\n", (*p4)[2]);
    // EXPECT: 7

    p4++;
    printf("%d\n", (*p4)[0]);
    // EXPECT: 9
    printf("%d\n", (*p4)[3]);
    // EXPECT: 12

    /* row sum via pointer-to-array function */
    printf("%d\n", row_sum_4(a, 3));
    // EXPECT: 78

    /* pointer difference between rows */
    int (*rp0)[4] = a;
    int (*rp2)[4] = a + 2;
    int row_diff = (int)(rp2 - rp0);
    printf("%d\n", row_diff);
    // EXPECT: 2

    /* pointer to array of 3 */
    int b[4][3] = {
        {10, 11, 12},
        {20, 21, 22},
        {30, 31, 32},
        {40, 41, 42}
    };
    print_row3(&b[0]);
    // EXPECT: 10 11 12
    print_row3(&b[1]);
    // EXPECT: 20 21 22
    print_row3(&b[2]);
    // EXPECT: 30 31 32
    print_row3(&b[3]);
    // EXPECT: 40 41 42

    /* get_row: array of 5 */
    int c[3][5] = {
        {100, 101, 102, 103, 104},
        {200, 201, 202, 203, 204},
        {300, 301, 302, 303, 304}
    };
    int *row1 = get_row(c, 1);
    printf("%d\n", row1[0]);
    // EXPECT: 200
    printf("%d\n", row1[4]);
    // EXPECT: 204

    int *row2 = get_row(c, 2);
    printf("%d\n", row2[2]);
    // EXPECT: 302

    /* array parameter decay: 2D array passed to 1D sum */
    printf("%d\n", sum1d(c[0], 5));
    // EXPECT: 510
    printf("%d\n", sum1d(c[1], 5));
    // EXPECT: 1010
    printf("%d\n", sum1d(c[2], 5));
    // EXPECT: 1510

    /* transpose */
    int src3[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    int dst3[3][3] = {0};
    transpose3(src3, dst3);

    /* dst3[0] = {1,4,7}, dst3[1] = {2,5,8}, dst3[2] = {3,6,9} */
    printf("%d\n", dst3[0][0]);
    // EXPECT: 1
    printf("%d\n", dst3[0][1]);
    // EXPECT: 4
    printf("%d\n", dst3[0][2]);
    // EXPECT: 7
    printf("%d\n", dst3[1][0]);
    // EXPECT: 2
    printf("%d\n", dst3[1][1]);
    // EXPECT: 5
    printf("%d\n", dst3[2][2]);
    // EXPECT: 9

    /* pointer to single-element-row array */
    int single_row[1][4] = {{7, 8, 9, 10}};
    int (*sp)[4] = single_row;
    printf("%d\n", (*sp)[0]);
    // EXPECT: 7
    printf("%d\n", (*sp)[3]);
    // EXPECT: 10

    /* use p[i][j] syntax on pointer to array */
    int (*pa)[4] = a;
    printf("%d\n", pa[0][0]);
    // EXPECT: 1
    printf("%d\n", pa[1][1]);
    // EXPECT: 6
    printf("%d\n", pa[2][2]);
    // EXPECT: 11

    /* walk all elements via pointer to array */
    int total = 0;
    int i, j;
    int (*walk)[3] = b;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            total += walk[i][j];
        }
    }
    /* 10+11+12 + 20+21+22 + 30+31+32 + 40+41+42 = 312 */
    printf("%d\n", total);
    // EXPECT: 312

    /* comparison of pointers to rows */
    int (*rowA)[3] = b;
    int (*rowB)[3] = b + 3;
    if (rowA < rowB) printf("A<B\n");
    // EXPECT: A<B
    int rdiff = (int)(rowB - rowA);
    printf("%d\n", rdiff);
    // EXPECT: 3

    return 0;
}
