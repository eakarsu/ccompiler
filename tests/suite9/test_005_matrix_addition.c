int printf(const char *fmt, ...);

void mat_add(int *a, int *b, int *c, int rows, int cols) {
    int i;
    int j;
    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            *(c + i * cols + j) = *(a + i * cols + j) + *(b + i * cols + j);
        }
    }
}

void mat_sub(int *a, int *b, int *c, int rows, int cols) {
    int i;
    int j;
    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            *(c + i * cols + j) = *(a + i * cols + j) - *(b + i * cols + j);
        }
    }
}

void mat_scalar_mul(int *m, int scalar, int rows, int cols) {
    int i;
    int j;
    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            *(m + i * cols + j) = *(m + i * cols + j) * scalar;
        }
    }
}

int mat_equals(int *a, int *b, int rows, int cols) {
    int i;
    int j;
    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            if (*(a + i * cols + j) != *(b + i * cols + j)) return 0;
        }
    }
    return 1;
}

void mat_negate(int *src, int *dst, int rows, int cols) {
    int i;
    int j;
    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            *(dst + i * cols + j) = -(*(src + i * cols + j));
        }
    }
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

int main() {
    int a[3][3];
    int b[3][3];
    int c[3][3];

    a[0][0] = 1; a[0][1] = 2; a[0][2] = 3;
    a[1][0] = 4; a[1][1] = 5; a[1][2] = 6;
    a[2][0] = 7; a[2][1] = 8; a[2][2] = 9;

    b[0][0] = 9; b[0][1] = 8; b[0][2] = 7;
    b[1][0] = 6; b[1][1] = 5; b[1][2] = 4;
    b[2][0] = 3; b[2][1] = 2; b[2][2] = 1;

    mat_add((int *)a, (int *)b, (int *)c, 3, 3);

    // EXPECT: A + B:
    printf("A + B:\n");
    // EXPECT: 10 10 10
    // EXPECT: 10 10 10
    // EXPECT: 10 10 10
    print_mat((int *)c, 3, 3);

    mat_sub((int *)a, (int *)b, (int *)c, 3, 3);

    // EXPECT: A - B:
    printf("A - B:\n");
    // EXPECT: -8 -6 -4
    // EXPECT: -2 0 2
    // EXPECT: 4 6 8
    print_mat((int *)c, 3, 3);

    /* Scalar multiply */
    int d[2][3];
    d[0][0] = 1; d[0][1] = 2; d[0][2] = 3;
    d[1][0] = 4; d[1][1] = 5; d[1][2] = 6;

    mat_scalar_mul((int *)d, 3, 2, 3);

    // EXPECT: 3 * D:
    printf("3 * D:\n");
    // EXPECT: 3 6 9
    // EXPECT: 12 15 18
    print_mat((int *)d, 2, 3);

    /* A + B == B + A (commutativity) */
    int c2[3][3];
    mat_add((int *)a, (int *)b, (int *)c, 3, 3);
    mat_add((int *)b, (int *)a, (int *)c2, 3, 3);

    // EXPECT: commutative: 1
    printf("commutative: %d\n", mat_equals((int *)c, (int *)c2, 3, 3));

    /* Negate */
    int neg[2][3];
    int orig[2][3];
    orig[0][0] = 1; orig[0][1] = -2; orig[0][2] = 3;
    orig[1][0] = -4; orig[1][1] = 5; orig[1][2] = -6;

    mat_negate((int *)orig, (int *)neg, 2, 3);

    // EXPECT: Negated:
    printf("Negated:\n");
    // EXPECT: -1 2 -3
    // EXPECT: 4 -5 6
    print_mat((int *)neg, 2, 3);

    /* A + (-A) == zero */
    int sum[2][3];
    mat_add((int *)orig, (int *)neg, (int *)sum, 2, 3);
    int zero[2][3];
    int i;
    int j;
    for (i = 0; i < 2; i = i + 1)
        for (j = 0; j < 3; j = j + 1)
            zero[i][j] = 0;

    // EXPECT: A + (-A) == 0: 1
    printf("A + (-A) == 0: %d\n", mat_equals((int *)sum, (int *)zero, 2, 3));

    // EXPECT: Done
    printf("Done\n");

    return 0;
}
