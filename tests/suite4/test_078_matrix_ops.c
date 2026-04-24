int printf(const char *fmt, ...);

void mat_add(int *a, int *b, int *c, int rows, int cols) {
    int i;
    int j;
    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            c[i * cols + j] = a[i * cols + j] + b[i * cols + j];
        }
    }
}

void mat_sub(int *a, int *b, int *c, int rows, int cols) {
    int i;
    int j;
    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            c[i * cols + j] = a[i * cols + j] - b[i * cols + j];
        }
    }
}

void mat_transpose(int *a, int *t, int rows, int cols) {
    int i;
    int j;
    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            t[j * rows + i] = a[i * cols + j];
        }
    }
}

void mat_mul(int *a, int *b, int *c, int m, int n, int p) {
    int i;
    int j;
    int k;
    int sum;
    for (i = 0; i < m; i = i + 1) {
        for (j = 0; j < p; j = j + 1) {
            sum = 0;
            for (k = 0; k < n; k = k + 1) {
                sum = sum + a[i * n + k] * b[k * p + j];
            }
            c[i * p + j] = sum;
        }
    }
}

void mat_scalar_mul(int *a, int scalar, int rows, int cols) {
    int i;
    int j;
    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            a[i * cols + j] = a[i * cols + j] * scalar;
        }
    }
}

int mat_trace(int *a, int n) {
    int i;
    int sum;
    sum = 0;
    for (i = 0; i < n; i = i + 1) {
        sum = sum + a[i * n + i];
    }
    return sum;
}

int mat_is_symmetric(int *a, int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            if (a[i * n + j] != a[j * n + i]) {
                return 0;
            }
        }
    }
    return 1;
}

void print_mat(int *a, int rows, int cols) {
    int i;
    int j;
    int first;
    first = 1;
    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            if (first) {
                first = 0;
            } else {
                printf(" ");
            }
            printf("%d", a[i * cols + j]);
        }
    }
}

int main(void) {
    int a[9];
    int b[9];
    int c[9];
    int t[9];
    int m1[4];
    int m2[4];
    int m3[4];

    a[0] = 1; a[1] = 2; a[2] = 3;
    a[3] = 4; a[4] = 5; a[5] = 6;
    a[6] = 7; a[7] = 8; a[8] = 9;

    b[0] = 9; b[1] = 8; b[2] = 7;
    b[3] = 6; b[4] = 5; b[5] = 4;
    b[6] = 3; b[7] = 2; b[8] = 1;

    mat_add(a, b, c, 3, 3);
    // EXPECT: add: 10 10 10 10 10 10 10 10 10
    printf("add: ");
    print_mat(c, 3, 3);
    printf("\n");

    mat_sub(a, b, c, 3, 3);
    // EXPECT: sub: -8 -6 -4 -2 0 2 4 6 8
    printf("sub: ");
    print_mat(c, 3, 3);
    printf("\n");

    mat_transpose(a, t, 3, 3);
    // EXPECT: trans: 1 4 7 2 5 8 3 6 9
    printf("trans: ");
    print_mat(t, 3, 3);
    printf("\n");

    // EXPECT: trace a: 15
    printf("trace a: %d\n", mat_trace(a, 3));

    // EXPECT: trace b: 15
    printf("trace b: %d\n", mat_trace(b, 3));

    mat_mul(a, b, c, 3, 3, 3);
    // EXPECT: mul: 30 24 18 84 69 54 138 114 90
    printf("mul: ");
    print_mat(c, 3, 3);
    printf("\n");

    // EXPECT: trace mul: 189
    printf("trace mul: %d\n", mat_trace(c, 3));

    m1[0] = 1; m1[1] = 2; m1[2] = 3; m1[3] = 4;
    m2[0] = 5; m2[1] = 6; m2[2] = 7; m2[3] = 8;
    mat_mul(m1, m2, m3, 2, 2, 2);
    // EXPECT: 2x2 mul: 19 22 43 50
    printf("2x2 mul: ");
    print_mat(m3, 2, 2);
    printf("\n");

    mat_scalar_mul(m1, 3, 2, 2);
    // EXPECT: scalar: 3 6 9 12
    printf("scalar: ");
    print_mat(m1, 2, 2);
    printf("\n");

    a[0] = 1; a[1] = 2; a[2] = 3;
    a[3] = 2; a[4] = 5; a[5] = 6;
    a[6] = 3; a[7] = 6; a[8] = 9;
    // EXPECT: symmetric: 1
    printf("symmetric: %d\n", mat_is_symmetric(a, 3));

    a[0] = 1; a[1] = 2; a[2] = 3;
    a[3] = 4; a[4] = 5; a[5] = 6;
    a[6] = 7; a[7] = 8; a[8] = 9;
    // EXPECT: not symmetric: 0
    printf("not symmetric: %d\n", mat_is_symmetric(a, 3));

    m1[0] = 1; m1[1] = 0; m1[2] = 0; m1[3] = 1;
    mat_mul(m1, m2, m3, 2, 2, 2);
    // EXPECT: identity mul: 5 6 7 8
    printf("identity mul: ");
    print_mat(m3, 2, 2);
    printf("\n");

    m1[0] = 0; m1[1] = 0; m1[2] = 0; m1[3] = 0;
    mat_add(m1, m2, m3, 2, 2);
    // EXPECT: zero add: 5 6 7 8
    printf("zero add: ");
    print_mat(m3, 2, 2);
    printf("\n");

    // EXPECT: trace 2x2: 13
    printf("trace 2x2: %d\n", mat_trace(m2, 2));

    m1[0] = 2; m1[1] = 0; m1[2] = 0; m1[3] = 2;
    mat_mul(m1, m2, m3, 2, 2, 2);
    // EXPECT: scale2x: 10 12 14 16
    printf("scale2x: ");
    print_mat(m3, 2, 2);
    printf("\n");

    // EXPECT: done
    printf("done\n");

    return 0;
}
