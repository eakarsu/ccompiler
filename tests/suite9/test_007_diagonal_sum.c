int printf(const char *fmt, ...);

int trace(int *m, int n) {
    int sum;
    int i;
    sum = 0;
    for (i = 0; i < n; i = i + 1) {
        sum = sum + *(m + i * n + i);
    }
    return sum;
}

int anti_diagonal_sum(int *m, int n) {
    int sum;
    int i;
    sum = 0;
    for (i = 0; i < n; i = i + 1) {
        sum = sum + *(m + i * n + (n - 1 - i));
    }
    return sum;
}

int all_diagonals_sum(int *m, int n) {
    /* Sum of all diagonal elements (both main and anti) minus center if odd */
    int sum;
    int i;
    sum = 0;
    for (i = 0; i < n; i = i + 1) {
        sum = sum + *(m + i * n + i);
        sum = sum + *(m + i * n + (n - 1 - i));
    }
    /* Subtract center element counted twice for odd-sized matrices */
    if (n % 2 != 0) {
        int mid;
        mid = n / 2;
        sum = sum - *(m + mid * n + mid);
    }
    return sum;
}

int row_sum(int *m, int n, int row) {
    int sum;
    int j;
    sum = 0;
    for (j = 0; j < n; j = j + 1) {
        sum = sum + *(m + row * n + j);
    }
    return sum;
}

int col_sum(int *m, int n, int col) {
    int sum;
    int i;
    sum = 0;
    for (i = 0; i < n; i = i + 1) {
        sum = sum + *(m + i * n + col);
    }
    return sum;
}

int is_magic_square(int *m, int n) {
    int magic_sum;
    int i;
    int j;

    magic_sum = row_sum(m, n, 0);

    /* Check all rows */
    for (i = 1; i < n; i = i + 1) {
        if (row_sum(m, n, i) != magic_sum) return 0;
    }
    /* Check all columns */
    for (j = 0; j < n; j = j + 1) {
        if (col_sum(m, n, j) != magic_sum) return 0;
    }
    /* Check main diagonal */
    if (trace(m, n) != magic_sum) return 0;
    /* Check anti-diagonal */
    if (anti_diagonal_sum(m, n) != magic_sum) return 0;

    return 1;
}

int total_sum(int *m, int rows, int cols) {
    int sum;
    int i;
    int j;
    sum = 0;
    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            sum = sum + *(m + i * cols + j);
        }
    }
    return sum;
}

int main() {
    int a[3][3];
    a[0][0] = 1; a[0][1] = 2; a[0][2] = 3;
    a[1][0] = 4; a[1][1] = 5; a[1][2] = 6;
    a[2][0] = 7; a[2][1] = 8; a[2][2] = 9;

    // EXPECT: trace 3x3: 15
    printf("trace 3x3: %d\n", trace((int *)a, 3));

    // EXPECT: anti diag: 15
    printf("anti diag: %d\n", anti_diagonal_sum((int *)a, 3));

    // EXPECT: all diags: 25
    printf("all diags: %d\n", all_diagonals_sum((int *)a, 3));

    // EXPECT: row 0 sum: 6
    printf("row 0 sum: %d\n", row_sum((int *)a, 3, 0));

    // EXPECT: row 1 sum: 15
    printf("row 1 sum: %d\n", row_sum((int *)a, 3, 1));

    // EXPECT: row 2 sum: 24
    printf("row 2 sum: %d\n", row_sum((int *)a, 3, 2));

    // EXPECT: col 0 sum: 12
    printf("col 0 sum: %d\n", col_sum((int *)a, 3, 0));

    // EXPECT: col 1 sum: 15
    printf("col 1 sum: %d\n", col_sum((int *)a, 3, 1));

    // EXPECT: col 2 sum: 18
    printf("col 2 sum: %d\n", col_sum((int *)a, 3, 2));

    // EXPECT: total: 45
    printf("total: %d\n", total_sum((int *)a, 3, 3));

    /* Magic square test */
    int magic[3][3];
    magic[0][0] = 2; magic[0][1] = 7; magic[0][2] = 6;
    magic[1][0] = 9; magic[1][1] = 5; magic[1][2] = 1;
    magic[2][0] = 4; magic[2][1] = 3; magic[2][2] = 8;

    // EXPECT: is magic: 1
    printf("is magic: %d\n", is_magic_square((int *)magic, 3));

    // EXPECT: magic trace: 15
    printf("magic trace: %d\n", trace((int *)magic, 3));

    // EXPECT: not magic: 0
    printf("not magic: %d\n", is_magic_square((int *)a, 3));

    /* 4x4 trace */
    int b[4][4];
    int i;
    int j;
    int v;
    v = 1;
    for (i = 0; i < 4; i = i + 1)
        for (j = 0; j < 4; j = j + 1) {
            b[i][j] = v;
            v = v + 1;
        }

    // EXPECT: trace 4x4: 34
    printf("trace 4x4: %d\n", trace((int *)b, 4));

    // EXPECT: anti 4x4: 34
    printf("anti 4x4: %d\n", anti_diagonal_sum((int *)b, 4));

    // EXPECT: Done
    printf("Done\n");

    return 0;
}
