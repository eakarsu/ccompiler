int printf(const char *fmt, ...);
// EXPECT: 2x2 determinants:\n|1 2| = -2\n|3 4|\n\n|5 3| = 29\n|2 7|\n\n3x3 matrix:\n| 1 2 3 |\n| 4 5 6 |\n| 7 8 9 |\ndet = 0\n\n3x3 matrix:\n| 2 -1 3 |\n| 1 4 -2 |\n| 3 5 1 |\ndet = 14\n\ndet(I) = 1\n\n4x4 matrix:\n| 1 2 3 4 |\n| 5 6 7 8 |\n| 2 6 4 8 |\n| 3 1 1 2 |\ndet = 72\n\ndet(2A) = 112, 2^3 * det(A) = 112
// Test: Matrix determinant

// 2x2 determinant
int det2(int a, int b, int c, int d) {
    return a * d - b * c;
}

// 3x3 determinant using cofactor expansion
int det3(int m[3][3]) {
    return m[0][0] * det2(m[1][1], m[1][2], m[2][1], m[2][2])
         - m[0][1] * det2(m[1][0], m[1][2], m[2][0], m[2][2])
         + m[0][2] * det2(m[1][0], m[1][1], m[2][0], m[2][1]);
}

// 4x4 determinant using cofactor expansion
int det4(int m[4][4]) {
    int result = 0;
    int j, k, l;
    int sign = 1;
    for (j = 0; j < 4; j++) {
        // Build 3x3 minor
        int minor[3][3];
        int mi, mj;
        for (k = 1; k < 4; k++) {
            mj = 0;
            for (l = 0; l < 4; l++) {
                if (l == j) continue;
                minor[k - 1][mj] = m[k][l];
                mj++;
            }
        }
        result = result + sign * m[0][j] * det3(minor);
        sign = -sign;
    }
    return result;
}

int main(void) {
    int i, j;

    // 2x2 determinants
    printf("2x2 determinants:\n");
    printf("|1 2| = %d\n", det2(1, 2, 3, 4));
    printf("|3 4|\n\n");
    printf("|5 3| = %d\n", det2(5, 3, 2, 7));
    printf("|2 7|\n\n");

    // 3x3 determinants
    int m1[3][3];
    m1[0][0] = 1; m1[0][1] = 2; m1[0][2] = 3;
    m1[1][0] = 4; m1[1][1] = 5; m1[1][2] = 6;
    m1[2][0] = 7; m1[2][1] = 8; m1[2][2] = 9;

    printf("3x3 matrix:\n");
    for (i = 0; i < 3; i++) {
        printf("| %d %d %d |\n", m1[i][0], m1[i][1], m1[i][2]);
    }
    printf("det = %d\n\n", det3(m1));

    int m2[3][3];
    m2[0][0] = 2; m2[0][1] = -1; m2[0][2] = 3;
    m2[1][0] = 1; m2[1][1] = 4; m2[1][2] = -2;
    m2[2][0] = 3; m2[2][1] = 5; m2[2][2] = 1;

    printf("3x3 matrix:\n");
    for (i = 0; i < 3; i++) {
        printf("| %d %d %d |\n", m2[i][0], m2[i][1], m2[i][2]);
    }
    printf("det = %d\n\n", det3(m2));

    // Identity matrix
    int eye[3][3];
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            eye[i][j] = (i == j) ? 1 : 0;
    printf("det(I) = %d\n", det3(eye));

    // 4x4 determinant
    int m4[4][4];
    m4[0][0] = 1; m4[0][1] = 2; m4[0][2] = 3; m4[0][3] = 4;
    m4[1][0] = 5; m4[1][1] = 6; m4[1][2] = 7; m4[1][3] = 8;
    m4[2][0] = 2; m4[2][1] = 6; m4[2][2] = 4; m4[2][3] = 8;
    m4[3][0] = 3; m4[3][1] = 1; m4[3][2] = 1; m4[3][3] = 2;

    printf("\n4x4 matrix:\n");
    for (i = 0; i < 4; i++) {
        printf("| %d %d %d %d |\n", m4[i][0], m4[i][1], m4[i][2], m4[i][3]);
    }
    printf("det = %d\n", det4(m4));

    // Property: det(A*k) = k^n * det(A) for nxn matrix
    int m3[3][3];
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            m3[i][j] = m2[i][j] * 2;
    printf("\ndet(2A) = %d, 2^3 * det(A) = %d\n", det3(m3), 8 * det3(m2));

    return 0;
}
