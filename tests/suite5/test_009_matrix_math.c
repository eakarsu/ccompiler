int printf(const char *fmt, ...);

void mat_zero(int m[3][3]) {
    int i = 0;
    while (i < 3) {
        int j = 0;
        while (j < 3) {
            m[i][j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }
}

void mat_identity(int m[3][3]) {
    mat_zero(m);
    m[0][0] = 1;
    m[1][1] = 1;
    m[2][2] = 1;
}

void mat_mul(int a[3][3], int b[3][3], int c[3][3]) {
    int i = 0;
    while (i < 3) {
        int j = 0;
        while (j < 3) {
            c[i][j] = 0;
            int k = 0;
            while (k < 3) {
                c[i][j] = c[i][j] + a[i][k] * b[k][j];
                k = k + 1;
            }
            j = j + 1;
        }
        i = i + 1;
    }
}

int mat_trace(int m[3][3]) {
    return m[0][0] + m[1][1] + m[2][2];
}

void mat_transpose(int m[3][3], int t[3][3]) {
    int i = 0;
    while (i < 3) {
        int j = 0;
        while (j < 3) {
            t[i][j] = m[j][i];
            j = j + 1;
        }
        i = i + 1;
    }
}

int mat_det(int m[3][3]) {
    int d = 0;
    d = d + m[0][0] * (m[1][1]*m[2][2] - m[1][2]*m[2][1]);
    d = d - m[0][1] * (m[1][0]*m[2][2] - m[1][2]*m[2][0]);
    d = d + m[0][2] * (m[1][0]*m[2][1] - m[1][1]*m[2][0]);
    return d;
}

int is_identity(int m[3][3]) {
    int i = 0;
    while (i < 3) {
        int j = 0;
        while (j < 3) {
            int expected = 0;
            if (i == j) expected = 1;
            if (m[i][j] != expected) return 0;
            j = j + 1;
        }
        i = i + 1;
    }
    return 1;
}

int mat_sum_all(int m[3][3]) {
    int sum = 0;
    int i = 0;
    while (i < 3) {
        int j = 0;
        while (j < 3) {
            sum = sum + m[i][j];
            j = j + 1;
        }
        i = i + 1;
    }
    return sum;
}

int mat_max(int m[3][3]) {
    int mx = m[0][0];
    int i = 0;
    while (i < 3) {
        int j = 0;
        while (j < 3) {
            if (m[i][j] > mx) mx = m[i][j];
            j = j + 1;
        }
        i = i + 1;
    }
    return mx;
}

void mat_add(int a[3][3], int b[3][3], int c[3][3]) {
    int i = 0;
    while (i < 3) {
        int j = 0;
        while (j < 3) {
            c[i][j] = a[i][j] + b[i][j];
            j = j + 1;
        }
        i = i + 1;
    }
}

int is_symmetric(int m[3][3]) {
    int i = 0;
    while (i < 3) {
        int j = i + 1;
        while (j < 3) {
            if (m[i][j] != m[j][i]) return 0;
            j = j + 1;
        }
        i = i + 1;
    }
    return 1;
}

int main(void) {
    int a[3][3];
    a[0][0] = 1; a[0][1] = 2; a[0][2] = 3;
    a[1][0] = 4; a[1][1] = 5; a[1][2] = 6;
    a[2][0] = 7; a[2][1] = 8; a[2][2] = 9;

    // EXPECT: trace(a) = 15
    printf("trace(a) = %d\n", mat_trace(a));
    // EXPECT: det(a) = 0
    printf("det(a) = %d\n", mat_det(a));
    // EXPECT: sum(a) = 45
    printf("sum(a) = %d\n", mat_sum_all(a));
    // EXPECT: max(a) = 9
    printf("max(a) = %d\n", mat_max(a));

    int b[3][3];
    b[0][0] = 2; b[0][1] = 0; b[0][2] = 1;
    b[1][0] = 3; b[1][1] = 1; b[1][2] = 0;
    b[2][0] = 1; b[2][1] = 2; b[2][2] = 1;

    // EXPECT: det(b) = 7
    printf("det(b) = %d\n", mat_det(b));

    int c[3][3];
    mat_mul(a, b, c);
    // EXPECT: mul[0][0] = 11
    printf("mul[0][0] = %d\n", c[0][0]);
    // EXPECT: mul[0][1] = 8
    printf("mul[0][1] = %d\n", c[0][1]);
    // EXPECT: mul[1][0] = 29
    printf("mul[1][0] = %d\n", c[1][0]);
    // EXPECT: mul[2][2] = 16
    printf("mul[2][2] = %d\n", c[2][2]);

    int t[3][3];
    mat_transpose(a, t);
    // EXPECT: t[0][1] = 4
    printf("t[0][1] = %d\n", t[0][1]);
    // EXPECT: t[1][0] = 2
    printf("t[1][0] = %d\n", t[1][0]);

    int id[3][3];
    mat_identity(id);
    // EXPECT: is_id(id) = 1
    printf("is_id(id) = %d\n", is_identity(id));
    // EXPECT: is_id(a) = 0
    printf("is_id(a) = %d\n", is_identity(a));

    int d[3][3];
    mat_add(a, b, d);
    // EXPECT: add[0][0] = 3
    printf("add[0][0] = %d\n", d[0][0]);
    // EXPECT: add[1][1] = 6
    printf("add[1][1] = %d\n", d[1][1]);

    int sym[3][3];
    sym[0][0] = 1; sym[0][1] = 2; sym[0][2] = 3;
    sym[1][0] = 2; sym[1][1] = 4; sym[1][2] = 5;
    sym[2][0] = 3; sym[2][1] = 5; sym[2][2] = 6;
    // EXPECT: is_sym(sym) = 1
    printf("is_sym(sym) = %d\n", is_symmetric(sym));
    // EXPECT: is_sym(a) = 0
    printf("is_sym(a) = %d\n", is_symmetric(a));

    return 0;
}
