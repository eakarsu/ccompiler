int printf(const char *fmt, ...);

// Matrix Addition and Scalar Multiplication
// Tests various matrix arithmetic operations

struct Mat {
    int d[4][4];
    int rows;
    int cols;
};

void mat_zero(struct Mat *m, int rows, int cols) {
    m->rows = rows;
    m->cols = cols;
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            m->d[i][j] = 0;
        }
    }
}

void mat_add(struct Mat *a, struct Mat *b, struct Mat *out) {
    out->rows = a->rows;
    out->cols = a->cols;
    int i;
    int j;
    for (i = 0; i < a->rows; i++) {
        for (j = 0; j < a->cols; j++) {
            out->d[i][j] = a->d[i][j] + b->d[i][j];
        }
    }
}

void mat_sub(struct Mat *a, struct Mat *b, struct Mat *out) {
    out->rows = a->rows;
    out->cols = a->cols;
    int i;
    int j;
    for (i = 0; i < a->rows; i++) {
        for (j = 0; j < a->cols; j++) {
            out->d[i][j] = a->d[i][j] - b->d[i][j];
        }
    }
}

void mat_scalar_mul(struct Mat *m, int s, struct Mat *out) {
    out->rows = m->rows;
    out->cols = m->cols;
    int i;
    int j;
    for (i = 0; i < m->rows; i++) {
        for (j = 0; j < m->cols; j++) {
            out->d[i][j] = m->d[i][j] * s;
        }
    }
}

void mat_negate(struct Mat *m, struct Mat *out) {
    mat_scalar_mul(m, -1, out);
}

int mat_sum_all(struct Mat *m) {
    int s = 0;
    int i;
    int j;
    for (i = 0; i < m->rows; i++) {
        for (j = 0; j < m->cols; j++) {
            s = s + m->d[i][j];
        }
    }
    return s;
}

int mat_max(struct Mat *m) {
    int mx = m->d[0][0];
    int i;
    int j;
    for (i = 0; i < m->rows; i++) {
        for (j = 0; j < m->cols; j++) {
            if (m->d[i][j] > mx) mx = m->d[i][j];
        }
    }
    return mx;
}

int mat_min(struct Mat *m) {
    int mn = m->d[0][0];
    int i;
    int j;
    for (i = 0; i < m->rows; i++) {
        for (j = 0; j < m->cols; j++) {
            if (m->d[i][j] < mn) mn = m->d[i][j];
        }
    }
    return mn;
}

int mat_equal(struct Mat *a, struct Mat *b) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    int i;
    int j;
    for (i = 0; i < a->rows; i++) {
        for (j = 0; j < a->cols; j++) {
            if (a->d[i][j] != b->d[i][j]) return 0;
        }
    }
    return 1;
}

// Linear combination: out = a*A + b*B
void mat_lincomb(struct Mat *A, int a, struct Mat *B, int b, struct Mat *out) {
    struct Mat sA;
    struct Mat sB;
    mat_scalar_mul(A, a, &sA);
    mat_scalar_mul(B, b, &sB);
    mat_add(&sA, &sB, out);
}

int main() {
    struct Mat a;
    struct Mat b;
    struct Mat c;
    struct Mat d;

    // Set up A = [[1,2,3],[4,5,6],[7,8,9]]
    mat_zero(&a, 3, 3);
    int val = 1;
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            a.d[i][j] = val;
            val = val + 1;
        }
    }

    // Set up B = [[9,8,7],[6,5,4],[3,2,1]]
    mat_zero(&b, 3, 3);
    val = 9;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            b.d[i][j] = val;
            val = val - 1;
        }
    }

    // Test 1: A + B
    mat_add(&a, &b, &c);
    // Each element: a[i][j] + b[i][j]
    // [1+9, 2+8, 3+7] = [10, 10, 10]
    // [4+6, 5+5, 6+4] = [10, 10, 10]
    // [7+3, 8+2, 9+1] = [10, 10, 10]
    printf("A+B [0]: %d %d %d\n", c.d[0][0], c.d[0][1], c.d[0][2]);
    // EXPECT: A+B [0]: 10 10 10
    printf("A+B [1]: %d %d %d\n", c.d[1][0], c.d[1][1], c.d[1][2]);
    // EXPECT: A+B [1]: 10 10 10
    printf("A+B [2]: %d %d %d\n", c.d[2][0], c.d[2][1], c.d[2][2]);
    // EXPECT: A+B [2]: 10 10 10

    printf("A+B sum: %d\n", mat_sum_all(&c));
    // EXPECT: A+B sum: 90

    // Test 2: A - B
    mat_sub(&a, &b, &c);
    // [1-9, 2-8, 3-7] = [-8, -6, -4]
    // [4-6, 5-5, 6-4] = [-2, 0, 2]
    // [7-3, 8-2, 9-1] = [4, 6, 8]
    printf("A-B [0]: %d %d %d\n", c.d[0][0], c.d[0][1], c.d[0][2]);
    // EXPECT: A-B [0]: -8 -6 -4
    printf("A-B [1]: %d %d %d\n", c.d[1][0], c.d[1][1], c.d[1][2]);
    // EXPECT: A-B [1]: -2 0 2
    printf("A-B [2]: %d %d %d\n", c.d[2][0], c.d[2][1], c.d[2][2]);
    // EXPECT: A-B [2]: 4 6 8

    // Test 3: A - B sum should be 0
    printf("A-B sum: %d\n", mat_sum_all(&c));
    // EXPECT: A-B sum: 0

    // Test 4: Scalar multiply
    mat_scalar_mul(&a, 3, &c);
    printf("3A [0]: %d %d %d\n", c.d[0][0], c.d[0][1], c.d[0][2]);
    // EXPECT: 3A [0]: 3 6 9
    printf("3A [2]: %d %d %d\n", c.d[2][0], c.d[2][1], c.d[2][2]);
    // EXPECT: 3A [2]: 21 24 27
    printf("3A sum: %d\n", mat_sum_all(&c));
    // EXPECT: 3A sum: 135

    // Test 5: Negate
    mat_negate(&a, &c);
    printf("-A [0][0]: %d\n", c.d[0][0]);
    // EXPECT: -A [0][0]: -1
    printf("-A [2][2]: %d\n", c.d[2][2]);
    // EXPECT: -A [2][2]: -9

    // Test 6: A + (-A) = 0
    mat_add(&a, &c, &d);
    printf("A + (-A) sum: %d\n", mat_sum_all(&d));
    // EXPECT: A + (-A) sum: 0

    // Test 7: Max and min
    printf("A max: %d\n", mat_max(&a));
    // EXPECT: A max: 9
    printf("A min: %d\n", mat_min(&a));
    // EXPECT: A min: 1

    // Test 8: Linear combination 2A + 3B
    mat_lincomb(&a, 2, &b, 3, &c);
    // [2*1+3*9, 2*2+3*8, 2*3+3*7] = [29, 28, 27]
    // [2*4+3*6, 2*5+3*5, 2*6+3*4] = [26, 25, 24]
    // [2*7+3*3, 2*8+3*2, 2*9+3*1] = [23, 22, 21]
    printf("2A+3B [0]: %d %d %d\n", c.d[0][0], c.d[0][1], c.d[0][2]);
    // EXPECT: 2A+3B [0]: 29 28 27
    printf("2A+3B [1]: %d %d %d\n", c.d[1][0], c.d[1][1], c.d[1][2]);
    // EXPECT: 2A+3B [1]: 26 25 24
    printf("2A+3B [2]: %d %d %d\n", c.d[2][0], c.d[2][1], c.d[2][2]);
    // EXPECT: 2A+3B [2]: 23 22 21

    printf("2A+3B sum: %d\n", mat_sum_all(&c));
    // EXPECT: 2A+3B sum: 225

    // Test 9: Scalar multiply by 0
    mat_scalar_mul(&a, 0, &c);
    printf("0*A sum: %d\n", mat_sum_all(&c));
    // EXPECT: 0*A sum: 0

    // Test 10: Commutativity A+B == B+A
    struct Mat ab;
    struct Mat ba;
    mat_add(&a, &b, &ab);
    mat_add(&b, &a, &ba);
    printf("A+B == B+A: %d\n", mat_equal(&ab, &ba));
    // EXPECT: A+B == B+A: 1

    return 0;
}
