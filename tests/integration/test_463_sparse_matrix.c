int printf(const char *fmt, ...);
// EXPECT: Matrix A:\n  1  0  3\n  0  5  0\n  7  0  9\nNNZ: 5\nMatrix B:\n  0  2  0\n  0  4  0\n  6  0  0\nA + B:\n  1  2  3\n  0  9  0\n 13  0  9\nA transposed:\n  1  0  7\n  0  5  0\n  3  0  9\nA * B:\n 18  2  0\n  0 20  0\n 54 14  0
// Sparse matrix with arrays (COO format)

struct SparseMatrix {
    int rows[50];
    int cols[50];
    int vals[50];
    int nnz;
    int nrows;
    int ncols;
};

void sm_init(struct SparseMatrix *m, int nr, int nc) {
    m->nnz = 0;
    m->nrows = nr;
    m->ncols = nc;
}

void sm_set(struct SparseMatrix *m, int r, int c, int v) {
    int i;
    for (i = 0; i < m->nnz; i++) {
        if (m->rows[i] == r && m->cols[i] == c) {
            m->vals[i] = v;
            return;
        }
    }
    if (m->nnz < 50 && v != 0) {
        m->rows[m->nnz] = r;
        m->cols[m->nnz] = c;
        m->vals[m->nnz] = v;
        m->nnz++;
    }
}

int sm_get(struct SparseMatrix *m, int r, int c) {
    int i;
    for (i = 0; i < m->nnz; i++) {
        if (m->rows[i] == r && m->cols[i] == c)
            return m->vals[i];
    }
    return 0;
}

void sm_print(struct SparseMatrix *m) {
    int r;
    int c;
    for (r = 0; r < m->nrows; r++) {
        for (c = 0; c < m->ncols; c++) {
            printf("%3d", sm_get(m, r, c));
        }
        printf("\n");
    }
}

void sm_add(struct SparseMatrix *a, struct SparseMatrix *b, struct SparseMatrix *result) {
    int i;
    int r;
    int c;
    int val;
    sm_init(result, a->nrows, a->ncols);
    for (i = 0; i < a->nnz; i++) {
        sm_set(result, a->rows[i], a->cols[i], a->vals[i]);
    }
    for (i = 0; i < b->nnz; i++) {
        r = b->rows[i];
        c = b->cols[i];
        val = sm_get(result, r, c) + b->vals[i];
        sm_set(result, r, c, val);
    }
}

void sm_transpose(struct SparseMatrix *m, struct SparseMatrix *result) {
    int i;
    sm_init(result, m->ncols, m->nrows);
    for (i = 0; i < m->nnz; i++) {
        sm_set(result, m->cols[i], m->rows[i], m->vals[i]);
    }
}

void sm_multiply(struct SparseMatrix *a, struct SparseMatrix *b, struct SparseMatrix *result) {
    int i;
    int j;
    int r;
    int c;
    int val;
    sm_init(result, a->nrows, b->ncols);
    for (i = 0; i < a->nnz; i++) {
        for (j = 0; j < b->nnz; j++) {
            if (a->cols[i] == b->rows[j]) {
                r = a->rows[i];
                c = b->cols[j];
                val = sm_get(result, r, c) + a->vals[i] * b->vals[j];
                sm_set(result, r, c, val);
            }
        }
    }
}

int main(void) {
    struct SparseMatrix a;
    struct SparseMatrix b;
    struct SparseMatrix c;

    sm_init(&a, 3, 3);
    sm_set(&a, 0, 0, 1);
    sm_set(&a, 0, 2, 3);
    sm_set(&a, 1, 1, 5);
    sm_set(&a, 2, 0, 7);
    sm_set(&a, 2, 2, 9);

    printf("Matrix A:\n");
    sm_print(&a);
    printf("NNZ: %d\n", a.nnz);

    sm_init(&b, 3, 3);
    sm_set(&b, 0, 1, 2);
    sm_set(&b, 1, 1, 4);
    sm_set(&b, 2, 0, 6);

    printf("Matrix B:\n");
    sm_print(&b);

    sm_add(&a, &b, &c);
    printf("A + B:\n");
    sm_print(&c);

    sm_transpose(&a, &c);
    printf("A transposed:\n");
    sm_print(&c);

    sm_multiply(&a, &b, &c);
    printf("A * B:\n");
    sm_print(&c);

    return 0;
}
