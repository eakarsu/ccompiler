int printf(const char *fmt, ...);

/* Integer row echelon form using scaled pivoting (no division, only integer ops).
   We do forward elimination: for each pivot column, find row with nonzero entry,
   swap, then eliminate below using cross-multiplication to stay in integers. */

void swap_rows(int *m, int r1, int r2, int cols) {
    int j;
    int tmp;
    for (j = 0; j < cols; j = j + 1) {
        tmp = *(m + r1 * cols + j);
        *(m + r1 * cols + j) = *(m + r2 * cols + j);
        *(m + r2 * cols + j) = tmp;
    }
}

int row_echelon_int(int *m, int rows, int cols) {
    int pivot_row;
    int col;
    int i;
    int j;
    int pivot_val;
    int target_val;
    int swaps;

    pivot_row = 0;
    swaps = 0;

    for (col = 0; col < cols && pivot_row < rows; col = col + 1) {
        /* Find nonzero pivot in this column */
        int found;
        found = -1;
        for (i = pivot_row; i < rows; i = i + 1) {
            if (*(m + i * cols + col) != 0) {
                found = i;
                break;
            }
        }
        if (found == -1) continue;

        if (found != pivot_row) {
            swap_rows(m, pivot_row, found, cols);
            swaps = swaps + 1;
        }

        pivot_val = *(m + pivot_row * cols + col);

        /* Eliminate below */
        for (i = pivot_row + 1; i < rows; i = i + 1) {
            target_val = *(m + i * cols + col);
            if (target_val != 0) {
                for (j = 0; j < cols; j = j + 1) {
                    *(m + i * cols + j) = pivot_val * (*(m + i * cols + j))
                                        - target_val * (*(m + pivot_row * cols + j));
                }
            }
        }

        pivot_row = pivot_row + 1;
    }

    return swaps;
}

void print_mat(int *m, int rows, int cols) {
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

int count_zero_rows(int *m, int rows, int cols) {
    int count;
    int i;
    int j;
    int all_zero;
    count = 0;
    for (i = 0; i < rows; i = i + 1) {
        all_zero = 1;
        for (j = 0; j < cols; j = j + 1) {
            if (*(m + i * cols + j) != 0) {
                all_zero = 0;
                break;
            }
        }
        if (all_zero) count = count + 1;
    }
    return count;
}

int main() {
    /* Test 1: simple 3x3 */
    int a[3][3];
    a[0][0] = 2; a[0][1] = 1; a[0][2] = -1;
    a[1][0] = -3; a[1][1] = -1; a[1][2] = 2;
    a[2][0] = -2; a[2][1] = 1; a[2][2] = 2;

    int sw;
    sw = row_echelon_int((int *)a, 3, 3);

    // EXPECT: REF 3x3:
    printf("REF 3x3:\n");
    // EXPECT: 2 1 -1
    // EXPECT: 0 1 1
    // EXPECT: 0 0 -2
    print_mat((int *)a, 3, 3);

    // EXPECT: swaps: 0
    printf("swaps: %d\n", sw);

    // EXPECT: zero rows: 0
    printf("zero rows: %d\n", count_zero_rows((int *)a, 3, 3));

    /* Test 2: singular matrix */
    int b[3][3];
    b[0][0] = 1; b[0][1] = 2; b[0][2] = 3;
    b[1][0] = 2; b[1][1] = 4; b[1][2] = 6;
    b[2][0] = 1; b[2][1] = 1; b[2][2] = 1;

    row_echelon_int((int *)b, 3, 3);

    // EXPECT: REF singular:
    printf("REF singular:\n");
    // EXPECT: 1 2 3
    // EXPECT: 0 -1 -2
    // EXPECT: 0 0 0
    print_mat((int *)b, 3, 3);

    /* Test 3: 2x3 system */
    int c[2][3];
    c[0][0] = 1; c[0][1] = 3; c[0][2] = 5;
    c[1][0] = 2; c[1][1] = 4; c[1][2] = 8;

    row_echelon_int((int *)c, 2, 3);

    // EXPECT: REF 2x3:
    printf("REF 2x3:\n");
    // EXPECT: 1 3 5
    // EXPECT: 0 -2 -2
    print_mat((int *)c, 2, 3);

    /* Test 4: needs row swap */
    int d[3][3];
    d[0][0] = 0; d[0][1] = 1; d[0][2] = 2;
    d[1][0] = 3; d[1][1] = 4; d[1][2] = 5;
    d[2][0] = 6; d[2][1] = 7; d[2][2] = 8;

    sw = row_echelon_int((int *)d, 3, 3);

    // EXPECT: REF with swap:
    printf("REF with swap:\n");
    // EXPECT: 3 4 5
    // EXPECT: 0 1 2
    // EXPECT: 0 0 0
    print_mat((int *)d, 3, 3);

    // EXPECT: swaps needed: 1
    printf("swaps needed: %d\n", sw);

    // EXPECT: Done
    printf("Done\n");

    return 0;
}
