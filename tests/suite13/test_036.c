int printf(const char *fmt, ...);

// Matrix Diagonal Operations
// Main diagonal, anti-diagonal, sum, product, and checks

struct Matrix {
    int d[5][5];
    int n;
};

void mat_init_seq(struct Matrix *m, int n) {
    m->n = n;
    int val = 1;
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            m->d[i][j] = val;
            val = val + 1;
        }
    }
}

int main_diag_sum(struct Matrix *m) {
    int sum = 0;
    int i;
    for (i = 0; i < m->n; i++) {
        sum = sum + m->d[i][i];
    }
    return sum;
}

int anti_diag_sum(struct Matrix *m) {
    int sum = 0;
    int i;
    int n = m->n;
    for (i = 0; i < n; i++) {
        sum = sum + m->d[i][n - 1 - i];
    }
    return sum;
}

void get_main_diag(struct Matrix *m, int *out) {
    int i;
    for (i = 0; i < m->n; i++) {
        out[i] = m->d[i][i];
    }
}

void get_anti_diag(struct Matrix *m, int *out) {
    int i;
    int n = m->n;
    for (i = 0; i < n; i++) {
        out[i] = m->d[i][n - 1 - i];
    }
}

int is_diagonal_matrix(struct Matrix *m) {
    int i;
    int j;
    for (i = 0; i < m->n; i++) {
        for (j = 0; j < m->n; j++) {
            if (i != j && m->d[i][j] != 0) return 0;
        }
    }
    return 1;
}

int is_upper_triangular(struct Matrix *m) {
    int i;
    int j;
    for (i = 0; i < m->n; i++) {
        for (j = 0; j < i; j++) {
            if (m->d[i][j] != 0) return 0;
        }
    }
    return 1;
}

int is_lower_triangular(struct Matrix *m) {
    int i;
    int j;
    for (i = 0; i < m->n; i++) {
        for (j = i + 1; j < m->n; j++) {
            if (m->d[i][j] != 0) return 0;
        }
    }
    return 1;
}

void make_diagonal(struct Matrix *m, int n, int *vals) {
    m->n = n;
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i == j) {
                m->d[i][j] = vals[i];
            } else {
                m->d[i][j] = 0;
            }
        }
    }
}

int diag_product(struct Matrix *m) {
    int prod = 1;
    int i;
    for (i = 0; i < m->n; i++) {
        prod = prod * m->d[i][i];
    }
    return prod;
}

int main() {
    struct Matrix m;
    int diag[5];
    int anti[5];

    // Test 1: 4x4 sequential matrix diagonals
    //  1  2  3  4
    //  5  6  7  8
    //  9 10 11 12
    // 13 14 15 16
    mat_init_seq(&m, 4);

    get_main_diag(&m, diag);
    printf("main diag: %d %d %d %d\n", diag[0], diag[1], diag[2], diag[3]);
    // EXPECT: main diag: 1 6 11 16

    int mds = main_diag_sum(&m);
    printf("main diag sum: %d\n", mds);
    // EXPECT: main diag sum: 34

    get_anti_diag(&m, anti);
    printf("anti diag: %d %d %d %d\n", anti[0], anti[1], anti[2], anti[3]);
    // EXPECT: anti diag: 4 7 10 13

    int ads = anti_diag_sum(&m);
    printf("anti diag sum: %d\n", ads);
    // EXPECT: anti diag sum: 34

    // Test 2: Both diag sums equal for this matrix
    printf("diag sums equal: %d\n", mds == ads);
    // EXPECT: diag sums equal: 1

    // Test 3: Not a diagonal matrix
    printf("is diagonal: %d\n", is_diagonal_matrix(&m));
    // EXPECT: is diagonal: 0
    printf("is upper tri: %d\n", is_upper_triangular(&m));
    // EXPECT: is upper tri: 0

    // Test 4: Create a diagonal matrix
    int vals[4];
    vals[0] = 3; vals[1] = 5; vals[2] = 7; vals[3] = 2;
    make_diagonal(&m, 4, vals);
    printf("diag matrix is diagonal: %d\n", is_diagonal_matrix(&m));
    // EXPECT: diag matrix is diagonal: 1
    printf("diag matrix is upper tri: %d\n", is_upper_triangular(&m));
    // EXPECT: diag matrix is upper tri: 1
    printf("diag matrix is lower tri: %d\n", is_lower_triangular(&m));
    // EXPECT: diag matrix is lower tri: 1

    // Test 5: Diagonal product
    int dp = diag_product(&m);
    printf("diag product: %d\n", dp);
    // EXPECT: diag product: 210

    // Test 6: 3x3 matrix
    mat_init_seq(&m, 3);
    // 1 2 3
    // 4 5 6
    // 7 8 9
    printf("3x3 main diag sum: %d\n", main_diag_sum(&m));
    // EXPECT: 3x3 main diag sum: 15
    printf("3x3 anti diag sum: %d\n", anti_diag_sum(&m));
    // EXPECT: 3x3 anti diag sum: 15

    // Test 7: Upper triangular matrix
    struct Matrix ut;
    ut.n = 3;
    ut.d[0][0] = 1; ut.d[0][1] = 2; ut.d[0][2] = 3;
    ut.d[1][0] = 0; ut.d[1][1] = 4; ut.d[1][2] = 5;
    ut.d[2][0] = 0; ut.d[2][1] = 0; ut.d[2][2] = 6;
    printf("ut is upper tri: %d\n", is_upper_triangular(&ut));
    // EXPECT: ut is upper tri: 1
    printf("ut is lower tri: %d\n", is_lower_triangular(&ut));
    // EXPECT: ut is lower tri: 0
    printf("ut is diagonal: %d\n", is_diagonal_matrix(&ut));
    // EXPECT: ut is diagonal: 0
    printf("ut diag product: %d\n", diag_product(&ut));
    // EXPECT: ut diag product: 24

    // Test 8: 5x5 matrix diagonals
    mat_init_seq(&m, 5);
    //  1  2  3  4  5
    //  6  7  8  9 10
    // 11 12 13 14 15
    // 16 17 18 19 20
    // 21 22 23 24 25
    // Main diag: 1+7+13+19+25 = 65
    // Anti diag: 5+9+13+17+21 = 65
    printf("5x5 main diag sum: %d\n", main_diag_sum(&m));
    // EXPECT: 5x5 main diag sum: 65
    printf("5x5 anti diag sum: %d\n", anti_diag_sum(&m));
    // EXPECT: 5x5 anti diag sum: 65

    get_main_diag(&m, diag);
    printf("5x5 main diag: %d %d %d %d %d\n", diag[0], diag[1], diag[2], diag[3], diag[4]);
    // EXPECT: 5x5 main diag: 1 7 13 19 25

    get_anti_diag(&m, anti);
    printf("5x5 anti diag: %d %d %d %d %d\n", anti[0], anti[1], anti[2], anti[3], anti[4]);
    // EXPECT: 5x5 anti diag: 5 9 13 17 21

    return 0;
}
