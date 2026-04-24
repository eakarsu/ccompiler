int printf(const char *fmt, ...);

int det_2x2(int m[2][2]) {
    return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

int det_3x3(int m[3][3]) {
    int d;
    d = m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
      - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
      + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    return d;
}

void minor_3x3(int m[3][3], int row, int col, int sub[2][2]) {
    int si;
    int sj;
    int i;
    int j;
    si = 0;
    for (i = 0; i < 3; i = i + 1) {
        if (i == row) continue;
        sj = 0;
        for (j = 0; j < 3; j = j + 1) {
            if (j == col) continue;
            sub[si][sj] = m[i][j];
            sj = sj + 1;
        }
        si = si + 1;
    }
}

int cofactor_3x3(int m[3][3], int row, int col) {
    int sub[2][2];
    int d;
    int sign;
    minor_3x3(m, row, col, sub);
    d = det_2x2(sub);
    sign = 1;
    if ((row + col) % 2 != 0) sign = -1;
    return sign * d;
}

int det_3x3_cofactor(int m[3][3]) {
    int d;
    int j;
    d = 0;
    for (j = 0; j < 3; j = j + 1) {
        d = d + m[0][j] * cofactor_3x3(m, 0, j);
    }
    return d;
}

int det_4x4(int m[4][4]) {
    int d;
    int j;
    int sign;
    int sub[3][3];
    int si;
    int sj;
    int ii;
    int jj;

    d = 0;
    for (j = 0; j < 4; j = j + 1) {
        /* extract 3x3 minor */
        si = 0;
        for (ii = 1; ii < 4; ii = ii + 1) {
            sj = 0;
            for (jj = 0; jj < 4; jj = jj + 1) {
                if (jj == j) continue;
                sub[si][sj] = m[ii][jj];
                sj = sj + 1;
            }
            si = si + 1;
        }
        sign = 1;
        if (j % 2 != 0) sign = -1;
        d = d + sign * m[0][j] * det_3x3(sub);
    }
    return d;
}

int main() {
    int a[2][2];
    a[0][0] = 3; a[0][1] = 8;
    a[1][0] = 4; a[1][1] = 6;

    // EXPECT: det 2x2: -14
    printf("det 2x2: %d\n", det_2x2(a));

    int b[2][2];
    b[0][0] = 1; b[0][1] = 0;
    b[1][0] = 0; b[1][1] = 1;

    // EXPECT: det identity 2x2: 1
    printf("det identity 2x2: %d\n", det_2x2(b));

    int c[3][3];
    c[0][0] = 6;  c[0][1] = 1; c[0][2] = 1;
    c[1][0] = 4;  c[1][1] = -2; c[1][2] = 5;
    c[2][0] = 2;  c[2][1] = 8; c[2][2] = 7;

    // EXPECT: det 3x3: -306
    printf("det 3x3: %d\n", det_3x3(c));

    // EXPECT: det 3x3 cofactor: -306
    printf("det 3x3 cofactor: %d\n", det_3x3_cofactor(c));

    int d[3][3];
    d[0][0] = 1; d[0][1] = 2; d[0][2] = 3;
    d[1][0] = 4; d[1][1] = 5; d[1][2] = 6;
    d[2][0] = 7; d[2][1] = 8; d[2][2] = 9;

    // EXPECT: det singular: 0
    printf("det singular: %d\n", det_3x3(d));

    int e[4][4];
    e[0][0] = 1; e[0][1] = 0; e[0][2] = 0; e[0][3] = 0;
    e[1][0] = 0; e[1][1] = 1; e[1][2] = 0; e[1][3] = 0;
    e[2][0] = 0; e[2][1] = 0; e[2][2] = 1; e[2][3] = 0;
    e[3][0] = 0; e[3][1] = 0; e[3][2] = 0; e[3][3] = 1;

    // EXPECT: det 4x4 identity: 1
    printf("det 4x4 identity: %d\n", det_4x4(e));

    int f[4][4];
    f[0][0] = 1; f[0][1] = 2; f[0][2] = 3; f[0][3] = 4;
    f[1][0] = 5; f[1][1] = 6; f[1][2] = 7; f[1][3] = 8;
    f[2][0] = 2; f[2][1] = 6; f[2][2] = 4; f[2][3] = 8;
    f[3][0] = 3; f[3][1] = 1; f[3][2] = 1; f[3][3] = 2;

    // EXPECT: det 4x4: 72
    printf("det 4x4: %d\n", det_4x4(f));

    // EXPECT: Done
    printf("Done\n");

    return 0;
}
