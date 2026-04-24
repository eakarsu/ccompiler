int printf(const char *fmt, ...);

// Matrix Transpose and Trace
// Tests transpose operation and trace (sum of diagonal) on matrices

struct Matrix {
    int data[4][4];
    int rows;
    int cols;
};

void mat_init(struct Matrix *m, int rows, int cols) {
    m->rows = rows;
    m->cols = cols;
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            m->data[i][j] = 0;
        }
    }
}

void mat_transpose(struct Matrix *src, struct Matrix *dst) {
    dst->rows = src->cols;
    dst->cols = src->rows;
    int i;
    int j;
    for (i = 0; i < src->rows; i++) {
        for (j = 0; j < src->cols; j++) {
            dst->data[j][i] = src->data[i][j];
        }
    }
}

int mat_trace(struct Matrix *m) {
    int sum = 0;
    int i;
    int n = m->rows;
    if (m->cols < n) n = m->cols;
    for (i = 0; i < n; i++) {
        sum = sum + m->data[i][i];
    }
    return sum;
}

int mat_is_symmetric(struct Matrix *m) {
    if (m->rows != m->cols) return 0;
    int i;
    int j;
    for (i = 0; i < m->rows; i++) {
        for (j = 0; j < m->cols; j++) {
            if (m->data[i][j] != m->data[j][i]) return 0;
        }
    }
    return 1;
}

void mat_add(struct Matrix *a, struct Matrix *b, struct Matrix *out) {
    out->rows = a->rows;
    out->cols = a->cols;
    int i;
    int j;
    for (i = 0; i < a->rows; i++) {
        for (j = 0; j < a->cols; j++) {
            out->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
}

int mat_equal(struct Matrix *a, struct Matrix *b) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    int i;
    int j;
    for (i = 0; i < a->rows; i++) {
        for (j = 0; j < a->cols; j++) {
            if (a->data[i][j] != b->data[i][j]) return 0;
        }
    }
    return 1;
}

int main() {
    struct Matrix a;
    struct Matrix t;
    struct Matrix tt;

    // Test 1: Transpose of 3x3
    mat_init(&a, 3, 3);
    a.data[0][0] = 1; a.data[0][1] = 2; a.data[0][2] = 3;
    a.data[1][0] = 4; a.data[1][1] = 5; a.data[1][2] = 6;
    a.data[2][0] = 7; a.data[2][1] = 8; a.data[2][2] = 9;

    mat_transpose(&a, &t);
    printf("T[0]: %d %d %d\n", t.data[0][0], t.data[0][1], t.data[0][2]);
    // EXPECT: T[0]: 1 4 7
    printf("T[1]: %d %d %d\n", t.data[1][0], t.data[1][1], t.data[1][2]);
    // EXPECT: T[1]: 2 5 8
    printf("T[2]: %d %d %d\n", t.data[2][0], t.data[2][1], t.data[2][2]);
    // EXPECT: T[2]: 3 6 9

    // Test 2: Transpose of transpose equals original
    mat_transpose(&t, &tt);
    printf("T(T(A)) == A: %d\n", mat_equal(&a, &tt));
    // EXPECT: T(T(A)) == A: 1

    // Test 3: Trace of A
    int tr = mat_trace(&a);
    printf("trace(A): %d\n", tr);
    // EXPECT: trace(A): 15

    // Test 4: Trace is invariant under transpose
    int tr_t = mat_trace(&t);
    printf("trace(T) == trace(A): %d\n", tr == tr_t);
    // EXPECT: trace(T) == trace(A): 1

    // Test 5: Symmetric matrix check
    printf("A symmetric: %d\n", mat_is_symmetric(&a));
    // EXPECT: A symmetric: 0

    // Test 6: Create a symmetric matrix: A + A^T
    struct Matrix sym;
    mat_add(&a, &t, &sym);
    printf("A+T symmetric: %d\n", mat_is_symmetric(&sym));
    // EXPECT: A+T symmetric: 1

    // A+T: [1+1, 2+4, 3+7] = [2, 6, 10]
    //       [4+2, 5+5, 6+8] = [6, 10, 14]
    //       [7+3, 8+6, 9+9] = [10, 14, 18]
    printf("sym[0]: %d %d %d\n", sym.data[0][0], sym.data[0][1], sym.data[0][2]);
    // EXPECT: sym[0]: 2 6 10
    printf("sym[1]: %d %d %d\n", sym.data[1][0], sym.data[1][1], sym.data[1][2]);
    // EXPECT: sym[1]: 6 10 14
    printf("sym[2]: %d %d %d\n", sym.data[2][0], sym.data[2][1], sym.data[2][2]);
    // EXPECT: sym[2]: 10 14 18

    // Test 7: Trace of A+T = 2 * trace(A)
    int tr_sym = mat_trace(&sym);
    printf("trace(A+T): %d\n", tr_sym);
    // EXPECT: trace(A+T): 30

    // Test 8: Transpose of a 2x4 matrix
    struct Matrix rect;
    mat_init(&rect, 2, 4);
    rect.data[0][0] = 10; rect.data[0][1] = 20; rect.data[0][2] = 30; rect.data[0][3] = 40;
    rect.data[1][0] = 50; rect.data[1][1] = 60; rect.data[1][2] = 70; rect.data[1][3] = 80;

    struct Matrix rect_t;
    mat_transpose(&rect, &rect_t);
    printf("rect_t dims: %d x %d\n", rect_t.rows, rect_t.cols);
    // EXPECT: rect_t dims: 4 x 2
    printf("rect_t[0]: %d %d\n", rect_t.data[0][0], rect_t.data[0][1]);
    // EXPECT: rect_t[0]: 10 50
    printf("rect_t[3]: %d %d\n", rect_t.data[3][0], rect_t.data[3][1]);
    // EXPECT: rect_t[3]: 40 80

    // Test 9: Identity is symmetric
    struct Matrix id;
    mat_init(&id, 3, 3);
    id.data[0][0] = 1; id.data[1][1] = 1; id.data[2][2] = 1;
    printf("Identity symmetric: %d\n", mat_is_symmetric(&id));
    // EXPECT: Identity symmetric: 1
    printf("Identity trace: %d\n", mat_trace(&id));
    // EXPECT: Identity trace: 3

    return 0;
}
