int printf(const char *fmt, ...);

/* test_023_2d_arrays.c
   Tests: 2D arrays int[R][C], row/column access, nested loop traversal,
          passing 2D array to functions as pointer to array
*/

/* Function accepting 2D array: pointer to row of 4 ints */
void print_row(int (*row)[4], int r) {
    int i;
    for (i = 0; i < 4; i++) {
        printf("%d", row[r][i]);
        if (i < 3) printf(" ");
    }
    printf("\n");
}

int row_sum(int (*mat)[4], int r) {
    int s = 0;
    int i;
    for (i = 0; i < 4; i++) s += mat[r][i];
    return s;
}

int col_sum(int (*mat)[4], int rows, int c) {
    int s = 0;
    int i;
    for (i = 0; i < rows; i++) s += mat[i][c];
    return s;
}

int mat_sum(int (*mat)[4], int rows) {
    int s = 0;
    int i, j;
    for (i = 0; i < rows; i++)
        for (j = 0; j < 4; j++)
            s += mat[i][j];
    return s;
}

void fill_2d(int (*mat)[3], int rows, int val) {
    int i, j;
    for (i = 0; i < rows; i++)
        for (j = 0; j < 3; j++)
            mat[i][j] = val;
}

int main(void) {
    /* basic 2D array declaration and access */
    int a[3][4] = {
        {1,  2,  3,  4},
        {5,  6,  7,  8},
        {9, 10, 11, 12}
    };

    printf("%d\n", a[0][0]);
    // EXPECT: 1
    printf("%d\n", a[0][3]);
    // EXPECT: 4
    printf("%d\n", a[1][0]);
    // EXPECT: 5
    printf("%d\n", a[1][2]);
    // EXPECT: 7
    printf("%d\n", a[2][0]);
    // EXPECT: 9
    printf("%d\n", a[2][3]);
    // EXPECT: 12

    /* row traversal */
    print_row(a, 0);
    // EXPECT: 1 2 3 4
    print_row(a, 1);
    // EXPECT: 5 6 7 8
    print_row(a, 2);
    // EXPECT: 9 10 11 12

    /* row sums */
    printf("%d\n", row_sum(a, 0));
    // EXPECT: 10
    printf("%d\n", row_sum(a, 1));
    // EXPECT: 26
    printf("%d\n", row_sum(a, 2));
    // EXPECT: 42

    /* column sums */
    printf("%d\n", col_sum(a, 3, 0));
    // EXPECT: 15
    printf("%d\n", col_sum(a, 3, 1));
    // EXPECT: 18
    printf("%d\n", col_sum(a, 3, 2));
    // EXPECT: 21
    printf("%d\n", col_sum(a, 3, 3));
    // EXPECT: 24

    /* total sum */
    printf("%d\n", mat_sum(a, 3));
    // EXPECT: 78

    /* modification */
    a[1][1] = 99;
    printf("%d\n", a[1][1]);
    // EXPECT: 99
    a[1][1] = 6;

    /* 2D array with single row */
    int row1[1][4] = {{7, 8, 9, 10}};
    printf("%d\n", row1[0][1]);
    // EXPECT: 8
    printf("%d\n", row_sum(row1, 0));
    // EXPECT: 34

    /* 2D array zero-initialized */
    int z[2][4] = {0};
    printf("%d\n", z[0][0]);
    // EXPECT: 0
    printf("%d\n", z[1][3]);
    // EXPECT: 0

    /* nested loops to fill manually */
    int b[4][4];
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            b[i][j] = i * 4 + j + 1;

    /* b[0] = 1..4, b[1] = 5..8, b[2] = 9..12, b[3] = 13..16 */
    printf("%d\n", b[0][0]);
    // EXPECT: 1
    printf("%d\n", b[3][3]);
    // EXPECT: 16
    printf("%d\n", b[2][1]);
    // EXPECT: 10

    /* total sum should be 1+2+...+16 = 136 */
    printf("%d\n", mat_sum(b, 4));
    // EXPECT: 136

    /* diagonal of b */
    int diag = 0;
    for (i = 0; i < 4; i++) diag += b[i][i];
    /* b[0][0]=1, b[1][1]=6, b[2][2]=11, b[3][3]=16 => 34 */
    printf("%d\n", diag);
    // EXPECT: 34

    /* fill_2d with custom cols=3 */
    int m[3][3];
    fill_2d(m, 3, 5);
    printf("%d\n", m[0][0]);
    // EXPECT: 5
    printf("%d\n", m[2][2]);
    // EXPECT: 5

    /* 2D array partial init */
    int p[3][4] = {{1, 2}, {3}};
    printf("%d\n", p[0][0]);
    // EXPECT: 1
    printf("%d\n", p[0][1]);
    // EXPECT: 2
    printf("%d\n", p[0][2]);
    // EXPECT: 0
    printf("%d\n", p[1][0]);
    // EXPECT: 3
    printf("%d\n", p[1][1]);
    // EXPECT: 0
    printf("%d\n", p[2][0]);
    // EXPECT: 0

    /* pointer arithmetic along rows */
    int (*rp)[4] = a;
    printf("%d\n", (*rp)[0]);
    // EXPECT: 1
    rp++;
    printf("%d\n", (*rp)[0]);
    // EXPECT: 5
    rp++;
    printf("%d\n", (*rp)[0]);
    // EXPECT: 9

    return 0;
}
