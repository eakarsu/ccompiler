int printf(const char *fmt, ...);

int main(void) {
    // 2D array with row pointers
    int mat[3][4];
    mat[0][0] = 1;  mat[0][1] = 2;  mat[0][2] = 3;  mat[0][3] = 4;
    mat[1][0] = 5;  mat[1][1] = 6;  mat[1][2] = 7;  mat[1][3] = 8;
    mat[2][0] = 9;  mat[2][1] = 10; mat[2][2] = 11; mat[2][3] = 12;

    // Row pointers
    int *row0 = &mat[0][0];
    int *row1 = &mat[1][0];
    int *row2 = &mat[2][0];

    // EXPECT: row0[0]: 1
    printf("row0[0]: %d\n", row0[0]);
    // EXPECT: row1[2]: 7
    printf("row1[2]: %d\n", row1[2]);
    // EXPECT: row2[3]: 12
    printf("row2[3]: %d\n", row2[3]);

    // Sum each row via pointer arithmetic
    int sum0 = 0;
    int i = 0;
    while (i < 4) {
        sum0 = sum0 + *(row0 + i);
        i = i + 1;
    }
    // EXPECT: row0 sum: 10
    printf("row0 sum: %d\n", sum0);

    int sum1 = 0;
    i = 0;
    while (i < 4) {
        sum1 = sum1 + *(row1 + i);
        i = i + 1;
    }
    // EXPECT: row1 sum: 26
    printf("row1 sum: %d\n", sum1);

    int sum2 = 0;
    i = 0;
    while (i < 4) {
        sum2 = sum2 + *(row2 + i);
        i = i + 1;
    }
    // EXPECT: row2 sum: 42
    printf("row2 sum: %d\n", sum2);

    // Total sum
    // EXPECT: total: 78
    printf("total: %d\n", sum0 + sum1 + sum2);

    // Column sums via row pointers
    int colsum0 = row0[0] + row1[0] + row2[0];
    int colsum1 = row0[1] + row1[1] + row2[1];
    int colsum2 = row0[2] + row1[2] + row2[2];
    int colsum3 = row0[3] + row1[3] + row2[3];
    // EXPECT: col0 sum: 15
    printf("col0 sum: %d\n", colsum0);
    // EXPECT: col3 sum: 24
    printf("col3 sum: %d\n", colsum3);

    // Modify through row pointer
    *(row1 + 1) = 60;
    // EXPECT: mat[1][1] changed: 60
    printf("mat[1][1] changed: %d\n", mat[1][1]);

    // Array of row pointers
    int *rows[3];
    rows[0] = &mat[0][0];
    rows[1] = &mat[1][0];
    rows[2] = &mat[2][0];

    // Access like 2D array: rows[r][c]
    // EXPECT: rows[0][3]: 4
    printf("rows[0][3]: %d\n", rows[0][3]);
    // EXPECT: rows[2][0]: 9
    printf("rows[2][0]: %d\n", rows[2][0]);

    // Compute diagonal sum via row pointers
    int diag = rows[0][0] + rows[1][1] + rows[2][2];
    // EXPECT: diagonal: 72
    printf("diagonal: %d\n", diag);

    // Transpose into another array
    int trans[4][3];
    int r = 0;
    while (r < 3) {
        int c = 0;
        while (c < 4) {
            trans[c][r] = rows[r][c];
            c = c + 1;
        }
        r = r + 1;
    }
    // EXPECT: trans[0][0]: 1
    printf("trans[0][0]: %d\n", trans[0][0]);
    // EXPECT: trans[0][2]: 9
    printf("trans[0][2]: %d\n", trans[0][2]);
    // EXPECT: trans[3][0]: 4
    printf("trans[3][0]: %d\n", trans[3][0]);

    // Pointer arithmetic: distance between rows
    int dist = (int)(row1 - row0);
    // EXPECT: row distance: 4
    printf("row distance: %d\n", dist);

    // Walking through 2D array with single pointer
    int *flat = &mat[0][0];
    int flatsum = 0;
    i = 0;
    while (i < 12) {
        flatsum = flatsum + *(flat + i);
        i = i + 1;
    }
    // EXPECT: flat sum: 132
    printf("flat sum: %d\n", flatsum);

    return 0;
}
