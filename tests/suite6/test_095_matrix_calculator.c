int printf(const char *fmt, ...);

/* We use flat arrays: m[i*N + j] for an NxN matrix */

void mat3_zero(int *m) {
    int i;
    i = 0;
    while (i < 9) {
        m[i] = 0;
        i = i + 1;
    }
}

void mat3_identity(int *m) {
    int i;
    int j;
    i = 0;
    while (i < 3) {
        j = 0;
        while (j < 3) {
            if (i == j) m[i * 3 + j] = 1;
            else m[i * 3 + j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }
}

void mat3_add(int *a, int *b, int *result) {
    int i;
    i = 0;
    while (i < 9) {
        result[i] = a[i] + b[i];
        i = i + 1;
    }
}

void mat3_sub(int *a, int *b, int *result) {
    int i;
    i = 0;
    while (i < 9) {
        result[i] = a[i] - b[i];
        i = i + 1;
    }
}

void mat3_mul(int *a, int *b, int *result) {
    int i;
    int j;
    int k;
    int sum;
    i = 0;
    while (i < 3) {
        j = 0;
        while (j < 3) {
            sum = 0;
            k = 0;
            while (k < 3) {
                sum = sum + a[i * 3 + k] * b[k * 3 + j];
                k = k + 1;
            }
            result[i * 3 + j] = sum;
            j = j + 1;
        }
        i = i + 1;
    }
}

void mat3_transpose(int *m, int *result) {
    int i;
    int j;
    i = 0;
    while (i < 3) {
        j = 0;
        while (j < 3) {
            result[j * 3 + i] = m[i * 3 + j];
            j = j + 1;
        }
        i = i + 1;
    }
}

int mat3_det(int *m) {
    /* det = a(ei-fh) - b(di-fg) + c(dh-eg) */
    int a; int b; int c; int d; int e; int f; int g; int h; int ii;
    a = m[0]; b = m[1]; c = m[2];
    d = m[3]; e = m[4]; f = m[5];
    g = m[6]; h = m[7]; ii = m[8];
    return a * (e * ii - f * h) - b * (d * ii - f * g) + c * (d * h - e * g);
}

int mat3_trace(int *m) {
    return m[0] + m[4] + m[8];
}

void mat3_scalar_mul(int *m, int s, int *result) {
    int i;
    i = 0;
    while (i < 9) {
        result[i] = m[i] * s;
        i = i + 1;
    }
}

int mat3_sum_all(int *m) {
    int i;
    int sum;
    sum = 0;
    i = 0;
    while (i < 9) {
        sum = sum + m[i];
        i = i + 1;
    }
    return sum;
}

int mat3_equals(int *a, int *b) {
    int i;
    i = 0;
    while (i < 9) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return 1;
}

/* 4x4 determinant using cofactor expansion along first row */
int mat4_det(int *m) {
    int det;
    int sign;
    int j;
    int sub[9];
    int si;
    int r;
    int c;
    det = 0;
    j = 0;
    sign = 1;
    while (j < 4) {
        /* Build 3x3 submatrix excluding row 0, col j */
        si = 0;
        r = 1;
        while (r < 4) {
            c = 0;
            while (c < 4) {
                if (c != j) {
                    sub[si] = m[r * 4 + c];
                    si = si + 1;
                }
                c = c + 1;
            }
            r = r + 1;
        }
        det = det + sign * m[j] * mat3_det(sub);
        sign = sign * -1;
        j = j + 1;
    }
    return det;
}

int main(void) {
    int a[9];
    int b[9];
    int c[9];
    int r[9];
    int ident[9];
    int m4[16];

    /* a = [[1,2,3],[4,5,6],[7,8,9]] */
    a[0] = 1; a[1] = 2; a[2] = 3;
    a[3] = 4; a[4] = 5; a[5] = 6;
    a[6] = 7; a[7] = 8; a[8] = 9;

    /* b = [[9,8,7],[6,5,4],[3,2,1]] */
    b[0] = 9; b[1] = 8; b[2] = 7;
    b[3] = 6; b[4] = 5; b[5] = 4;
    b[6] = 3; b[7] = 2; b[8] = 1;

    /* Add a+b */
    mat3_add(a, b, r);
    /* All elements should be 10 */
    // EXPECT: 10
    printf("%d\n", r[0]);
    // EXPECT: 10
    printf("%d\n", r[4]);
    // EXPECT: 10
    printf("%d\n", r[8]);

    /* Sum of a+b = 10*9 = 90 */
    // EXPECT: 90
    printf("%d\n", mat3_sum_all(r));

    /* Sub a-b */
    mat3_sub(a, b, r);
    /* r[0] = 1-9 = -8, r[4] = 5-5 = 0, r[8] = 9-1 = 8 */
    // EXPECT: -8
    printf("%d\n", r[0]);
    // EXPECT: 0
    printf("%d\n", r[4]);
    // EXPECT: 8
    printf("%d\n", r[8]);

    /* Multiply a*b */
    mat3_mul(a, b, r);
    /* r[0][0] = 1*9+2*6+3*3 = 9+12+9 = 30 */
    // EXPECT: 30
    printf("%d\n", r[0]);
    /* r[1][1] = 4*8+5*5+6*2 = 32+25+12 = 69 */
    // EXPECT: 69
    printf("%d\n", r[4]);
    /* r[2][2] = 7*7+8*4+9*1 = 49+32+9 = 90 */
    // EXPECT: 90
    printf("%d\n", r[8]);

    /* Determinant of a: 1(5*9-6*8) - 2(4*9-6*7) + 3(4*8-5*7) = 1(-3) - 2(-6) + 3(-3) = -3+12-9 = 0 */
    // EXPECT: 0
    printf("%d\n", mat3_det(a));

    /* Trace of a: 1+5+9 = 15 */
    // EXPECT: 15
    printf("%d\n", mat3_trace(a));

    /* Transpose of a */
    mat3_transpose(a, r);
    /* r = [[1,4,7],[2,5,8],[3,6,9]] */
    // EXPECT: 4
    printf("%d\n", r[1]);
    // EXPECT: 2
    printf("%d\n", r[3]);

    /* Identity matrix */
    mat3_identity(ident);
    // EXPECT: 1
    printf("%d\n", ident[0]);
    // EXPECT: 0
    printf("%d\n", ident[1]);
    // EXPECT: 1
    printf("%d\n", ident[4]);

    /* a * identity = a */
    mat3_mul(a, ident, r);
    // EXPECT: 1
    printf("%d\n", mat3_equals(a, r));

    /* Scalar multiply a by 2 */
    mat3_scalar_mul(a, 2, r);
    // EXPECT: 2
    printf("%d\n", r[0]);
    // EXPECT: 18
    printf("%d\n", r[8]);

    /* det of identity = 1 */
    // EXPECT: 1
    printf("%d\n", mat3_det(ident));

    /* c = [[2,1,1],[1,3,2],[1,0,0]] */
    c[0] = 2; c[1] = 1; c[2] = 1;
    c[3] = 1; c[4] = 3; c[5] = 2;
    c[6] = 1; c[7] = 0; c[8] = 0;
    /* det = 2(0-0) - 1(0-2) + 1(0-3) = 0+2-3 = -1 */
    // EXPECT: -1
    printf("%d\n", mat3_det(c));

    /* 4x4 identity determinant */
    m4[0]=1; m4[1]=0; m4[2]=0; m4[3]=0;
    m4[4]=0; m4[5]=1; m4[6]=0; m4[7]=0;
    m4[8]=0; m4[9]=0; m4[10]=1; m4[11]=0;
    m4[12]=0; m4[13]=0; m4[14]=0; m4[15]=1;
    // EXPECT: 1
    printf("%d\n", mat4_det(m4));

    /* 4x4 matrix: [[1,2,3,4],[5,6,7,8],[2,6,4,8],[3,1,1,2]] */
    m4[0]=1; m4[1]=2; m4[2]=3; m4[3]=4;
    m4[4]=5; m4[5]=6; m4[6]=7; m4[7]=8;
    m4[8]=2; m4[9]=6; m4[10]=4; m4[11]=8;
    m4[12]=3; m4[13]=1; m4[14]=1; m4[15]=2;
    /* Determinant computed */
    // EXPECT: 72
    printf("%d\n", mat4_det(m4));

    return 0;
}
