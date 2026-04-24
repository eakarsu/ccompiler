int printf(const char *fmt, ...);

void mat_mul_3x3(int a[3][3], int b[3][3], int c[3][3]) {
    int i;
    int j;
    int k;
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 3; j = j + 1) {
            c[i][j] = 0;
            for (k = 0; k < 3; k = k + 1) {
                c[i][j] = c[i][j] + a[i][k] * b[k][j];
            }
        }
    }
}

void print_matrix_3x3(int m[3][3]) {
    int i;
    int j;
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 3; j = j + 1) {
            if (j > 0) {
                printf(" ");
            }
            printf("%d", m[i][j]);
        }
        printf("\n");
    }
}

void mat_mul_2x2(int a[2][2], int b[2][2], int c[2][2]) {
    int i;
    int j;
    int k;
    for (i = 0; i < 2; i = i + 1) {
        for (j = 0; j < 2; j = j + 1) {
            c[i][j] = 0;
            for (k = 0; k < 2; k = k + 1) {
                c[i][j] = c[i][j] + a[i][k] * b[k][j];
            }
        }
    }
}

void print_matrix_2x2(int m[2][2]) {
    int i;
    int j;
    for (i = 0; i < 2; i = i + 1) {
        for (j = 0; j < 2; j = j + 1) {
            if (j > 0) {
                printf(" ");
            }
            printf("%d", m[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int a[3][3];
    int b[3][3];
    int c[3][3];
    int i;
    int j;

    /* Identity * matrix = matrix */
    a[0][0] = 1; a[0][1] = 0; a[0][2] = 0;
    a[1][0] = 0; a[1][1] = 1; a[1][2] = 0;
    a[2][0] = 0; a[2][1] = 0; a[2][2] = 1;

    b[0][0] = 5; b[0][1] = 3; b[0][2] = 1;
    b[1][0] = 2; b[1][1] = 7; b[1][2] = 4;
    b[2][0] = 6; b[2][1] = 8; b[2][2] = 9;

    mat_mul_3x3(a, b, c);

    // EXPECT: Identity * B:
    printf("Identity * B:\n");
    // EXPECT: 5 3 1
    // EXPECT: 2 7 4
    // EXPECT: 6 8 9
    print_matrix_3x3(c);

    /* A * B where A and B are non-trivial */
    a[0][0] = 1; a[0][1] = 2; a[0][2] = 3;
    a[1][0] = 4; a[1][1] = 5; a[1][2] = 6;
    a[2][0] = 7; a[2][1] = 8; a[2][2] = 9;

    b[0][0] = 9; b[0][1] = 8; b[0][2] = 7;
    b[1][0] = 6; b[1][1] = 5; b[1][2] = 4;
    b[2][0] = 3; b[2][1] = 2; b[2][2] = 1;

    mat_mul_3x3(a, b, c);

    // EXPECT: A * B:
    printf("A * B:\n");
    // EXPECT: 30 24 18
    // EXPECT: 84 69 54
    // EXPECT: 138 114 90
    print_matrix_3x3(c);

    /* 2x2 multiplication */
    int p[2][2];
    int q[2][2];
    int r[2][2];

    p[0][0] = 3; p[0][1] = 7;
    p[1][0] = 2; p[1][1] = 5;

    q[0][0] = 1; q[0][1] = 4;
    q[1][0] = 6; q[1][1] = 2;

    mat_mul_2x2(p, q, r);

    // EXPECT: 2x2 result:
    printf("2x2 result:\n");
    // EXPECT: 45 26
    // EXPECT: 32 18
    print_matrix_2x2(r);

    // EXPECT: Done
    printf("Done\n");

    return 0;
}
