int printf(const char *fmt, ...);

struct Matrix {
    int rows;
    int cols;
    int data[16];
};

int mat_get(struct Matrix *m, int r, int c) {
    return m->data[r * m->cols + c];
}

void mat_set(struct Matrix *m, int r, int c, int val) {
    m->data[r * m->cols + c] = val;
}

void mat_init(struct Matrix *m, int rows, int cols) {
    m->rows = rows;
    m->cols = cols;
    int i;
    for (i = 0; i < 16; i = i + 1) {
        m->data[i] = 0;
    }
}

void mat_identity(struct Matrix *m, int n) {
    mat_init(m, n, n);
    int i;
    for (i = 0; i < n; i = i + 1) {
        mat_set(m, i, i, 1);
    }
}

void mat_multiply(struct Matrix *result, struct Matrix *a, struct Matrix *b) {
    mat_init(result, a->rows, b->cols);
    int i, j, k;
    for (i = 0; i < a->rows; i = i + 1) {
        for (j = 0; j < b->cols; j = j + 1) {
            int sum = 0;
            for (k = 0; k < a->cols; k = k + 1) {
                sum = sum + mat_get(a, i, k) * mat_get(b, k, j);
            }
            mat_set(result, i, j, sum);
        }
    }
}

void mat_transpose(struct Matrix *dst, struct Matrix *src) {
    mat_init(dst, src->cols, src->rows);
    int i, j;
    for (i = 0; i < src->rows; i = i + 1) {
        for (j = 0; j < src->cols; j = j + 1) {
            mat_set(dst, j, i, mat_get(src, i, j));
        }
    }
}

void mat_add(struct Matrix *result, struct Matrix *a, struct Matrix *b) {
    mat_init(result, a->rows, a->cols);
    int i, j;
    for (i = 0; i < a->rows; i = i + 1) {
        for (j = 0; j < a->cols; j = j + 1) {
            mat_set(result, i, j, mat_get(a, i, j) + mat_get(b, i, j));
        }
    }
}

int mat_trace(struct Matrix *m) {
    int t = 0;
    int i;
    int n = m->rows;
    if (m->cols < n) n = m->cols;
    for (i = 0; i < n; i = i + 1) {
        t = t + mat_get(m, i, i);
    }
    return t;
}

int mat_sum_all(struct Matrix *m) {
    int s = 0;
    int i, j;
    for (i = 0; i < m->rows; i = i + 1) {
        for (j = 0; j < m->cols; j = j + 1) {
            s = s + mat_get(m, i, j);
        }
    }
    return s;
}

void mat_scale(struct Matrix *m, int factor) {
    int i, j;
    for (i = 0; i < m->rows; i = i + 1) {
        for (j = 0; j < m->cols; j = j + 1) {
            mat_set(m, i, j, mat_get(m, i, j) * factor);
        }
    }
}

int main(void) {
    struct Matrix a;
    mat_init(&a, 2, 2);
    mat_set(&a, 0, 0, 1); mat_set(&a, 0, 1, 2);
    mat_set(&a, 1, 0, 3); mat_set(&a, 1, 1, 4);

    // EXPECT: a[0][0]=1 a[0][1]=2
    printf("a[0][0]=%d a[0][1]=%d\n", mat_get(&a, 0, 0), mat_get(&a, 0, 1));
    // EXPECT: a[1][0]=3 a[1][1]=4
    printf("a[1][0]=%d a[1][1]=%d\n", mat_get(&a, 1, 0), mat_get(&a, 1, 1));

    // EXPECT: trace a=5
    printf("trace a=%d\n", mat_trace(&a));
    // EXPECT: sum a=10
    printf("sum a=%d\n", mat_sum_all(&a));

    struct Matrix b;
    mat_init(&b, 2, 2);
    mat_set(&b, 0, 0, 5); mat_set(&b, 0, 1, 6);
    mat_set(&b, 1, 0, 7); mat_set(&b, 1, 1, 8);

    struct Matrix c;
    mat_multiply(&c, &a, &b);
    // EXPECT: mul[0][0]=19 mul[0][1]=22
    printf("mul[0][0]=%d mul[0][1]=%d\n", mat_get(&c, 0, 0), mat_get(&c, 0, 1));
    // EXPECT: mul[1][0]=43 mul[1][1]=50
    printf("mul[1][0]=%d mul[1][1]=%d\n", mat_get(&c, 1, 0), mat_get(&c, 1, 1));

    // EXPECT: trace mul=69
    printf("trace mul=%d\n", mat_trace(&c));

    struct Matrix t;
    mat_transpose(&t, &a);
    // EXPECT: trans[0][0]=1 trans[0][1]=3
    printf("trans[0][0]=%d trans[0][1]=%d\n", mat_get(&t, 0, 0), mat_get(&t, 0, 1));
    // EXPECT: trans[1][0]=2 trans[1][1]=4
    printf("trans[1][0]=%d trans[1][1]=%d\n", mat_get(&t, 1, 0), mat_get(&t, 1, 1));

    struct Matrix sum;
    mat_add(&sum, &a, &b);
    // EXPECT: add[0][0]=6 add[1][1]=12
    printf("add[0][0]=%d add[1][1]=%d\n", mat_get(&sum, 0, 0), mat_get(&sum, 1, 1));

    // EXPECT: sum_all add=36
    printf("sum_all add=%d\n", mat_sum_all(&sum));

    struct Matrix id;
    mat_identity(&id, 3);
    // EXPECT: id trace=3
    printf("id trace=%d\n", mat_trace(&id));
    // EXPECT: id sum=3
    printf("id sum=%d\n", mat_sum_all(&id));

    mat_scale(&a, 10);
    // EXPECT: scaled a[0][0]=10 a[1][1]=40
    printf("scaled a[0][0]=%d a[1][1]=%d\n", mat_get(&a, 0, 0), mat_get(&a, 1, 1));

    // EXPECT: scaled sum=100
    printf("scaled sum=%d\n", mat_sum_all(&a));

    return 0;
}
