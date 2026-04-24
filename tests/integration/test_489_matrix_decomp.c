int printf(const char *fmt, ...);
// EXPECT: === Matrix Decomposition ===\nMatrix A:\n   2    1    3 \n   1    4    2 \n   3    2    5 \nSymmetric: yes\nDiagonal: no\nTrace: 11\nDet(3x3): 3\nTranspose:\n   2    1    3 \n   1    4    2 \n   3    2    5 \nA * A^T:\n  14   12   23 \n  12   21   21 \n  23   21   38 \nResult symmetric: yes\nDiagonal matrix:\n   5    0    0 \n   0    3    0 \n   0    0    7 \nIs diagonal: yes\nDet: 105\n2x2 det: 10

struct Matrix {
    int data[4][4];
    int rows;
    int cols;
};

void init_matrix(struct Matrix *m, int r, int c) {
    m->rows = r;
    m->cols = c;
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            m->data[i][j] = 0;
        }
    }
}

void set_identity(struct Matrix *m, int n) {
    init_matrix(m, n, n);
    int i;
    for (i = 0; i < n; i++) {
        m->data[i][i] = 100;
    }
}

void print_matrix(struct Matrix *m) {
    int i, j;
    for (i = 0; i < m->rows; i++) {
        for (j = 0; j < m->cols; j++) {
            printf("%4d ", m->data[i][j]);
        }
        printf("\n");
    }
}

void mat_mul(struct Matrix *a, struct Matrix *b, struct Matrix *result) {
    init_matrix(result, a->rows, b->cols);
    int i, j, k;
    for (i = 0; i < a->rows; i++) {
        for (j = 0; j < b->cols; j++) {
            int sum = 0;
            for (k = 0; k < a->cols; k++) {
                sum = sum + a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
}

void transpose(struct Matrix *m, struct Matrix *result) {
    init_matrix(result, m->cols, m->rows);
    int i, j;
    for (i = 0; i < m->rows; i++) {
        for (j = 0; j < m->cols; j++) {
            result->data[j][i] = m->data[i][j];
        }
    }
}

int trace(struct Matrix *m) {
    int t = 0;
    int i;
    int n = m->rows;
    if (m->cols < n) n = m->cols;
    for (i = 0; i < n; i++) {
        t = t + m->data[i][i];
    }
    return t;
}

int det2x2(struct Matrix *m) {
    return m->data[0][0] * m->data[1][1] - m->data[0][1] * m->data[1][0];
}

int det3x3(struct Matrix *m) {
    int d = 0;
    d = d + m->data[0][0] * (m->data[1][1] * m->data[2][2] - m->data[1][2] * m->data[2][1]);
    d = d - m->data[0][1] * (m->data[1][0] * m->data[2][2] - m->data[1][2] * m->data[2][0]);
    d = d + m->data[0][2] * (m->data[1][0] * m->data[2][1] - m->data[1][1] * m->data[2][0]);
    return d;
}

int is_symmetric(struct Matrix *m) {
    int i, j;
    for (i = 0; i < m->rows; i++) {
        for (j = 0; j < m->cols; j++) {
            if (m->data[i][j] != m->data[j][i]) return 0;
        }
    }
    return 1;
}

int is_diagonal(struct Matrix *m) {
    int i, j;
    for (i = 0; i < m->rows; i++) {
        for (j = 0; j < m->cols; j++) {
            if (i != j && m->data[i][j] != 0) return 0;
        }
    }
    return 1;
}

int main(void) {
    struct Matrix a;
    struct Matrix b;
    struct Matrix result;
    struct Matrix trans;

    printf("=== Matrix Decomposition ===\n");

    init_matrix(&a, 3, 3);
    a.data[0][0] = 2; a.data[0][1] = 1; a.data[0][2] = 3;
    a.data[1][0] = 1; a.data[1][1] = 4; a.data[1][2] = 2;
    a.data[2][0] = 3; a.data[2][1] = 2; a.data[2][2] = 5;

    printf("Matrix A:\n");
    print_matrix(&a);
    printf("Symmetric: %s\n", is_symmetric(&a) ? "yes" : "no");
    printf("Diagonal: %s\n", is_diagonal(&a) ? "yes" : "no");
    printf("Trace: %d\n", trace(&a));
    printf("Det(3x3): %d\n", det3x3(&a));

    transpose(&a, &trans);
    printf("Transpose:\n");
    print_matrix(&trans);

    mat_mul(&a, &trans, &result);
    printf("A * A^T:\n");
    print_matrix(&result);
    printf("Result symmetric: %s\n", is_symmetric(&result) ? "yes" : "no");

    struct Matrix diag;
    init_matrix(&diag, 3, 3);
    diag.data[0][0] = 5; diag.data[1][1] = 3; diag.data[2][2] = 7;
    printf("Diagonal matrix:\n");
    print_matrix(&diag);
    printf("Is diagonal: %s\n", is_diagonal(&diag) ? "yes" : "no");
    printf("Det: %d\n", det3x3(&diag));

    struct Matrix m2;
    init_matrix(&m2, 2, 2);
    m2.data[0][0] = 4; m2.data[0][1] = 7;
    m2.data[1][0] = 2; m2.data[1][1] = 6;
    printf("2x2 det: %d\n", det2x2(&m2));

    return 0;
}
