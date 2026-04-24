int printf(const char *fmt, ...);

int matrix_g[3][3];

void fill_matrix(int m[3][3]) {
    int i;
    int j;
    int val = 1;
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 3; j = j + 1) {
            m[i][j] = val;
            val = val + 1;
        }
    }
}

int sum_matrix(int m[3][3]) {
    int s = 0;
    int i;
    int j;
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 3; j = j + 1) {
            s = s + m[i][j];
        }
    }
    return s;
}

int row_sum(int m[3][3], int row) {
    int s = 0;
    int j;
    for (j = 0; j < 3; j = j + 1) {
        s = s + m[row][j];
    }
    return s;
}

int col_sum(int m[3][3], int col) {
    int s = 0;
    int i;
    for (i = 0; i < 3; i = i + 1) {
        s = s + m[i][col];
    }
    return s;
}

int trace(int m[3][3]) {
    return m[0][0] + m[1][1] + m[2][2];
}

void transpose(int src[3][3], int dst[3][3]) {
    int i;
    int j;
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 3; j = j + 1) {
            dst[j][i] = src[i][j];
        }
    }
}

int main(void) {
    int m[3][3];

    fill_matrix(m);
    /* m = {{1,2,3},{4,5,6},{7,8,9}} */

    // EXPECT: 1
    printf("%d\n", m[0][0]);
    // EXPECT: 2
    printf("%d\n", m[0][1]);
    // EXPECT: 3
    printf("%d\n", m[0][2]);
    // EXPECT: 4
    printf("%d\n", m[1][0]);
    // EXPECT: 5
    printf("%d\n", m[1][1]);
    // EXPECT: 9
    printf("%d\n", m[2][2]);

    // EXPECT: 45
    printf("%d\n", sum_matrix(m));

    /* row sums: 1+2+3=6, 4+5+6=15, 7+8+9=24 */
    // EXPECT: 6
    printf("%d\n", row_sum(m, 0));
    // EXPECT: 15
    printf("%d\n", row_sum(m, 1));
    // EXPECT: 24
    printf("%d\n", row_sum(m, 2));

    /* col sums: 1+4+7=12, 2+5+8=15, 3+6+9=18 */
    // EXPECT: 12
    printf("%d\n", col_sum(m, 0));
    // EXPECT: 15
    printf("%d\n", col_sum(m, 1));
    // EXPECT: 18
    printf("%d\n", col_sum(m, 2));

    /* trace = 1+5+9 = 15 */
    // EXPECT: 15
    printf("%d\n", trace(m));

    int t[3][3];
    transpose(m, t);
    /* t = {{1,4,7},{2,5,8},{3,6,9}} */
    // EXPECT: 1
    printf("%d\n", t[0][0]);
    // EXPECT: 4
    printf("%d\n", t[0][1]);
    // EXPECT: 7
    printf("%d\n", t[0][2]);
    // EXPECT: 2
    printf("%d\n", t[1][0]);
    // EXPECT: 9
    printf("%d\n", t[2][2]);

    /* identity matrix check */
    int id[3][3];
    int i;
    int j;
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 3; j = j + 1) {
            if (i == j) {
                id[i][j] = 1;
            } else {
                id[i][j] = 0;
            }
        }
    }
    // EXPECT: 1
    printf("%d\n", id[0][0]);
    // EXPECT: 0
    printf("%d\n", id[0][1]);
    // EXPECT: 1
    printf("%d\n", id[1][1]);
    // EXPECT: 0
    printf("%d\n", id[2][0]);
    // EXPECT: 3
    printf("%d\n", trace(id));

    /* global matrix */
    fill_matrix(matrix_g);
    // EXPECT: 45
    printf("%d\n", sum_matrix(matrix_g));

    /* 2D array as flat memory: m[1][2] == *(*(m+1)+2) */
    int flat_val = *(*(m + 1) + 2);
    // EXPECT: 6
    printf("%d\n", flat_val);

    /* Multiply identity by m: result should be m */
    int result[3][3];
    int k;
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 3; j = j + 1) {
            result[i][j] = 0;
            for (k = 0; k < 3; k = k + 1) {
                result[i][j] = result[i][j] + id[i][k] * m[k][j];
            }
        }
    }
    // EXPECT: 1
    printf("%d\n", result[0][0]);
    // EXPECT: 5
    printf("%d\n", result[1][1]);
    // EXPECT: 9
    printf("%d\n", result[2][2]);
    // EXPECT: 45
    printf("%d\n", sum_matrix(result));

    return 0;
}
