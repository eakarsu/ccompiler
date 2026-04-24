int printf(const char *fmt, ...);
// EXPECT: === matrix A ===\n1 2 3\n4 5 6\n7 8 9\ntrace=15\ndet3=0\n=== transpose ===\n1 4 7\n2 5 8\n3 6 9\n=== A*I ===\n1 2 3\n4 5 6\n7 8 9\n=== A+B ===\n10 10 10\n10 10 10\n10 10 10\n=== A*B ===\n30 24 18\n84 69 54\n138 114 90\ndet2=-14
// Test 230: Matrix utility functions

int mat_a[4][4];
int mat_b[4][4];
int mat_c[4][4];

void mat_zero(int m[4][4]) {
    int i = 0;
    while (i < 4) {
        int j = 0;
        while (j < 4) {
            m[i][j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }
}

void mat_identity(int m[4][4]) {
    mat_zero(m);
    int i = 0;
    while (i < 4) {
        m[i][i] = 1;
        i = i + 1;
    }
}

void mat_print(int m[4][4], int n) {
    int i = 0;
    while (i < n) {
        int j = 0;
        while (j < n) {
            if (j > 0) printf(" ");
            printf("%d", m[i][j]);
            j = j + 1;
        }
        printf("\n");
        i = i + 1;
    }
}

void mat_mul(int a[4][4], int b[4][4], int c[4][4], int n) {
    int i = 0;
    while (i < n) {
        int j = 0;
        while (j < n) {
            c[i][j] = 0;
            int k = 0;
            while (k < n) {
                c[i][j] = c[i][j] + a[i][k] * b[k][j];
                k = k + 1;
            }
            j = j + 1;
        }
        i = i + 1;
    }
}

void mat_add(int a[4][4], int b[4][4], int c[4][4], int n) {
    int i = 0;
    while (i < n) {
        int j = 0;
        while (j < n) {
            c[i][j] = a[i][j] + b[i][j];
            j = j + 1;
        }
        i = i + 1;
    }
}

void mat_transpose(int src[4][4], int dst[4][4], int n) {
    int i = 0;
    while (i < n) {
        int j = 0;
        while (j < n) {
            dst[j][i] = src[i][j];
            j = j + 1;
        }
        i = i + 1;
    }
}

int mat_trace(int m[4][4], int n) {
    int t = 0;
    int i = 0;
    while (i < n) {
        t = t + m[i][i];
        i = i + 1;
    }
    return t;
}

int mat_det_2x2(int m[4][4]) {
    return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

int mat_det_3x3(int m[4][4]) {
    return m[0][0] * (m[1][1]*m[2][2] - m[1][2]*m[2][1])
         - m[0][1] * (m[1][0]*m[2][2] - m[1][2]*m[2][0])
         + m[0][2] * (m[1][0]*m[2][1] - m[1][1]*m[2][0]);
}

int main(void) {
    mat_a[0][0] = 1; mat_a[0][1] = 2; mat_a[0][2] = 3;
    mat_a[1][0] = 4; mat_a[1][1] = 5; mat_a[1][2] = 6;
    mat_a[2][0] = 7; mat_a[2][1] = 8; mat_a[2][2] = 9;

    printf("=== matrix A ===\n");
    mat_print(mat_a, 3);

    printf("trace=%d\n", mat_trace(mat_a, 3));
    printf("det3=%d\n", mat_det_3x3(mat_a));

    mat_transpose(mat_a, mat_b, 3);
    printf("=== transpose ===\n");
    mat_print(mat_b, 3);

    mat_identity(mat_b);
    mat_mul(mat_a, mat_b, mat_c, 3);
    printf("=== A*I ===\n");
    mat_print(mat_c, 3);

    mat_b[0][0] = 9; mat_b[0][1] = 8; mat_b[0][2] = 7;
    mat_b[1][0] = 6; mat_b[1][1] = 5; mat_b[1][2] = 4;
    mat_b[2][0] = 3; mat_b[2][1] = 2; mat_b[2][2] = 1;

    mat_add(mat_a, mat_b, mat_c, 3);
    printf("=== A+B ===\n");
    mat_print(mat_c, 3);

    mat_mul(mat_a, mat_b, mat_c, 3);
    printf("=== A*B ===\n");
    mat_print(mat_c, 3);

    int m2[4][4];
    m2[0][0] = 3; m2[0][1] = 8;
    m2[1][0] = 4; m2[1][1] = 6;
    printf("det2=%d\n", mat_det_2x2(m2));

    return 0;
}
