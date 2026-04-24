int printf(const char *fmt, ...);

/* A saddle point is an element that is the minimum in its row
   and the maximum in its column. */

int row_min(int *m, int cols, int row) {
    int j;
    int mn;
    mn = *(m + row * cols);
    for (j = 1; j < cols; j = j + 1) {
        int val;
        val = *(m + row * cols + j);
        if (val < mn) mn = val;
    }
    return mn;
}

int col_max(int *m, int rows, int cols, int col) {
    int i;
    int mx;
    mx = *(m + col);
    for (i = 1; i < rows; i = i + 1) {
        int val;
        val = *(m + i * cols + col);
        if (val > mx) mx = val;
    }
    return mx;
}

int row_max(int *m, int cols, int row) {
    int j;
    int mx;
    mx = *(m + row * cols);
    for (j = 1; j < cols; j = j + 1) {
        int val;
        val = *(m + row * cols + j);
        if (val > mx) mx = val;
    }
    return mx;
}

int col_min(int *m, int rows, int cols, int col) {
    int i;
    int mn;
    mn = *(m + col);
    for (i = 1; i < rows; i = i + 1) {
        int val;
        val = *(m + i * cols + col);
        if (val < mn) mn = val;
    }
    return mn;
}

int find_saddle_points(int *m, int rows, int cols, int *sp_rows, int *sp_cols) {
    int count;
    int i;
    int j;
    int val;
    count = 0;

    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            val = *(m + i * cols + j);
            if (val == row_min(m, cols, i) && val == col_max(m, rows, cols, j)) {
                sp_rows[count] = i;
                sp_cols[count] = j;
                count = count + 1;
            }
        }
    }
    return count;
}

void print_matrix(int *m, int rows, int cols) {
    int i;
    int j;
    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            if (j > 0) printf(" ");
            printf("%d", *(m + i * cols + j));
        }
        printf("\n");
    }
}

int main() {
    /* Test 1: classic saddle point */
    int a[3][3];
    a[0][0] = 1; a[0][1] = 2; a[0][2] = 3;
    a[1][0] = 4; a[1][1] = 5; a[1][2] = 6;
    a[2][0] = 7; a[2][1] = 8; a[2][2] = 9;

    int sp_r[9];
    int sp_c[9];
    int count;

    // EXPECT: Matrix 1:
    printf("Matrix 1:\n");
    // EXPECT: 1 2 3
    // EXPECT: 4 5 6
    // EXPECT: 7 8 9
    print_matrix((int *)a, 3, 3);

    count = find_saddle_points((int *)a, 3, 3, sp_r, sp_c);
    // EXPECT: saddle points: 1
    printf("saddle points: %d\n", count);
    if (count > 0) {
        // EXPECT: at (2,0) val=7
        printf("at (%d,%d) val=%d\n", sp_r[0], sp_c[0], a[sp_r[0]][sp_c[0]]);
    }

    /* Test 2: no saddle point */
    int b[2][2];
    b[0][0] = 1; b[0][1] = 2;
    b[1][0] = 3; b[1][1] = 4;

    // EXPECT: Matrix 2:
    printf("Matrix 2:\n");
    // EXPECT: 1 2
    // EXPECT: 3 4
    print_matrix((int *)b, 2, 2);

    count = find_saddle_points((int *)b, 2, 2, sp_r, sp_c);
    // EXPECT: saddle points: 1
    printf("saddle points: %d\n", count);
    if (count > 0) {
        // EXPECT: at (1,0) val=3
        printf("at (%d,%d) val=%d\n", sp_r[0], sp_c[0], b[sp_r[0]][sp_c[0]]);
    }

    /* Test 3: multiple saddle points (all same value in row) */
    int c[3][3];
    c[0][0] = 5; c[0][1] = 5; c[0][2] = 5;
    c[1][0] = 5; c[1][1] = 5; c[1][2] = 5;
    c[2][0] = 5; c[2][1] = 5; c[2][2] = 5;

    count = find_saddle_points((int *)c, 3, 3, sp_r, sp_c);
    // EXPECT: uniform saddle points: 9
    printf("uniform saddle points: %d\n", count);

    /* Test 4: saddle point in middle */
    int d[3][3];
    d[0][0] = 3; d[0][1] = 1; d[0][2] = 4;
    d[1][0] = 7; d[1][1] = 5; d[1][2] = 9;
    d[2][0] = 2; d[2][1] = 0; d[2][2] = 8;

    // EXPECT: Matrix 4:
    printf("Matrix 4:\n");
    // EXPECT: 3 1 4
    // EXPECT: 7 5 9
    // EXPECT: 2 0 8
    print_matrix((int *)d, 3, 3);

    count = find_saddle_points((int *)d, 3, 3, sp_r, sp_c);
    // EXPECT: saddle points: 1
    printf("saddle points: %d\n", count);
    if (count > 0) {
        // EXPECT: at (1,1) val=5
        printf("at (%d,%d) val=%d\n", sp_r[0], sp_c[0], d[sp_r[0]][sp_c[0]]);
    }

    /* Test 5: row/col stats */
    // EXPECT: row mins: 1 4 7
    printf("row mins: %d %d %d\n", row_min((int *)a, 3, 0), row_min((int *)a, 3, 1), row_min((int *)a, 3, 2));
    // EXPECT: col maxs: 7 8 9
    printf("col maxs: %d %d %d\n", col_max((int *)a, 3, 3, 0), col_max((int *)a, 3, 3, 1), col_max((int *)a, 3, 3, 2));
    // EXPECT: row maxs: 3 6 9
    printf("row maxs: %d %d %d\n", row_max((int *)a, 3, 0), row_max((int *)a, 3, 1), row_max((int *)a, 3, 2));
    // EXPECT: col mins: 1 2 3
    printf("col mins: %d %d %d\n", col_min((int *)a, 3, 3, 0), col_min((int *)a, 3, 3, 1), col_min((int *)a, 3, 3, 2));

    /* Test 6: 4x3 rectangular */
    int e[4][3];
    e[0][0] = 10; e[0][1] = 20; e[0][2] = 30;
    e[1][0] = 5;  e[1][1] = 40; e[1][2] = 50;
    e[2][0] = 60; e[2][1] = 70; e[2][2] = 80;
    e[3][0] = 15; e[3][1] = 25; e[3][2] = 35;

    count = find_saddle_points((int *)e, 4, 3, sp_r, sp_c);
    // EXPECT: rect saddle: 1
    printf("rect saddle: %d\n", count);
    if (count > 0) {
        // EXPECT: at (2,0) val=60
        printf("at (%d,%d) val=%d\n", sp_r[0], sp_c[0], e[sp_r[0]][sp_c[0]]);
    }

    // EXPECT: Done
    printf("Done\n");

    return 0;
}
