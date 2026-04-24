int printf(const char *fmt, ...);

void transpose_3x3(int src[3][3], int dst[3][3]) {
    int i;
    int j;
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 3; j = j + 1) {
            dst[j][i] = src[i][j];
        }
    }
}

void transpose_4x4(int src[4][4], int dst[4][4]) {
    int i;
    int j;
    for (i = 0; i < 4; i = i + 1) {
        for (j = 0; j < 4; j = j + 1) {
            dst[j][i] = src[i][j];
        }
    }
}

void transpose_inplace_3x3(int m[3][3]) {
    int i;
    int j;
    int tmp;
    for (i = 0; i < 3; i = i + 1) {
        for (j = i + 1; j < 3; j = j + 1) {
            tmp = m[i][j];
            m[i][j] = m[j][i];
            m[j][i] = tmp;
        }
    }
}

void print_3x3(int m[3][3]) {
    int i;
    int j;
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 3; j = j + 1) {
            if (j > 0) printf(" ");
            printf("%d", m[i][j]);
        }
        printf("\n");
    }
}

void print_4x4(int m[4][4]) {
    int i;
    int j;
    for (i = 0; i < 4; i = i + 1) {
        for (j = 0; j < 4; j = j + 1) {
            if (j > 0) printf(" ");
            printf("%d", m[i][j]);
        }
        printf("\n");
    }
}

int check_symmetric(int m[3][3]) {
    int i;
    int j;
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 3; j = j + 1) {
            if (m[i][j] != m[j][i]) return 0;
        }
    }
    return 1;
}

int main() {
    int a[3][3];
    int t[3][3];

    a[0][0] = 1; a[0][1] = 2; a[0][2] = 3;
    a[1][0] = 4; a[1][1] = 5; a[1][2] = 6;
    a[2][0] = 7; a[2][1] = 8; a[2][2] = 9;

    transpose_3x3(a, t);

    // EXPECT: Transpose of 3x3:
    printf("Transpose of 3x3:\n");
    // EXPECT: 1 4 7
    // EXPECT: 2 5 8
    // EXPECT: 3 6 9
    print_3x3(t);

    /* In-place transpose */
    int b[3][3];
    b[0][0] = 10; b[0][1] = 20; b[0][2] = 30;
    b[1][0] = 40; b[1][1] = 50; b[1][2] = 60;
    b[2][0] = 70; b[2][1] = 80; b[2][2] = 90;

    transpose_inplace_3x3(b);

    // EXPECT: In-place transpose:
    printf("In-place transpose:\n");
    // EXPECT: 10 40 70
    // EXPECT: 20 50 80
    // EXPECT: 30 60 90
    print_3x3(b);

    /* 4x4 transpose */
    int c[4][4];
    int d[4][4];
    int val;
    int i;
    int j;
    val = 1;
    for (i = 0; i < 4; i = i + 1) {
        for (j = 0; j < 4; j = j + 1) {
            c[i][j] = val;
            val = val + 1;
        }
    }

    transpose_4x4(c, d);

    // EXPECT: 4x4 transpose:
    printf("4x4 transpose:\n");
    // EXPECT: 1 5 9 13
    // EXPECT: 2 6 10 14
    // EXPECT: 3 7 11 15
    // EXPECT: 4 8 12 16
    print_4x4(d);

    /* Symmetric matrix check */
    int sym[3][3];
    sym[0][0] = 1; sym[0][1] = 2; sym[0][2] = 3;
    sym[1][0] = 2; sym[1][1] = 5; sym[1][2] = 6;
    sym[2][0] = 3; sym[2][1] = 6; sym[2][2] = 9;

    // EXPECT: Symmetric: 1
    printf("Symmetric: %d\n", check_symmetric(sym));

    sym[1][0] = 99;
    // EXPECT: Symmetric: 0
    printf("Symmetric: %d\n", check_symmetric(sym));

    // EXPECT: Done
    printf("Done\n");

    return 0;
}
