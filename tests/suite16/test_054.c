int printf(const char *fmt, ...);

// Matrix operations with x1000 scaling
// Matrices stored as flat arrays, row-major order
// All values scaled by 1000

// Get element at (r, c) in an n x n matrix
int mat_get(int m[], int n, int r, int c) {
    return m[r * n + c];
}

// Set element at (r, c) in an n x n matrix
void mat_set(int m[], int n, int r, int c, int val) {
    m[r * n + c] = val;
}

// Matrix addition: c = a + b (n x n)
void mat_add(int a[], int b[], int c[], int n) {
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            c[i * n + j] = a[i * n + j] + b[i * n + j];
        }
    }
}

// Matrix subtraction: c = a - b (n x n)
void mat_sub(int a[], int b[], int c[], int n) {
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            c[i * n + j] = a[i * n + j] - b[i * n + j];
        }
    }
}

// Matrix multiply: c = a * b (n x n), values x1000
void mat_mul(int a[], int b[], int c[], int n) {
    int i;
    int j;
    int k;
    long long sum;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            sum = 0;
            for (k = 0; k < n; k++) {
                sum = sum + (long long)a[i * n + k] * b[k * n + j];
            }
            c[i * n + j] = (int)(sum / 1000);
        }
    }
}

// Matrix transpose: b = transpose(a) (n x n)
void mat_transpose(int a[], int b[], int n) {
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            b[j * n + i] = a[i * n + j];
        }
    }
}

// Trace of a matrix (sum of diagonal elements)
int mat_trace(int m[], int n) {
    int i;
    int sum;
    sum = 0;
    for (i = 0; i < n; i++) {
        sum = sum + m[i * n + i];
    }
    return sum;
}

// Matrix-vector multiply: y = A * x (n x n matrix, n vector)
void mat_vec_mul(int a[], int x[], int y[], int n) {
    int i;
    int j;
    long long sum;

    for (i = 0; i < n; i++) {
        sum = 0;
        for (j = 0; j < n; j++) {
            sum = sum + (long long)a[i * n + j] * x[j];
        }
        y[i] = (int)(sum / 1000);
    }
}

void test_add_sub(void) {
    int a[4];
    int b[4];
    int c[4];

    printf("=== Matrix Add/Sub 2x2 ===\n"); // EXPECT: === Matrix Add/Sub 2x2 ===

    // a = [[1, 2], [3, 4]] * 1000
    a[0] = 1000; a[1] = 2000; a[2] = 3000; a[3] = 4000;
    // b = [[5, 6], [7, 8]] * 1000
    b[0] = 5000; b[1] = 6000; b[2] = 7000; b[3] = 8000;

    mat_add(a, b, c, 2);
    printf("a+b: %d %d %d %d\n", c[0], c[1], c[2], c[3]); // EXPECT: a+b: 6000 8000 10000 12000

    mat_sub(a, b, c, 2);
    printf("a-b: %d %d %d %d\n", c[0], c[1], c[2], c[3]); // EXPECT: a-b: -4000 -4000 -4000 -4000
}

void test_multiply(void) {
    int a[4];
    int b[4];
    int c[4];

    printf("=== Matrix Multiply 2x2 ===\n"); // EXPECT: === Matrix Multiply 2x2 ===

    // a = [[1, 2], [3, 4]] * 1000
    a[0] = 1000; a[1] = 2000; a[2] = 3000; a[3] = 4000;
    // b = [[5, 6], [7, 8]] * 1000
    b[0] = 5000; b[1] = 6000; b[2] = 7000; b[3] = 8000;

    // a*b = [[19, 22], [43, 50]] * 1000
    mat_mul(a, b, c, 2);
    printf("a*b: %d %d %d %d\n", c[0], c[1], c[2], c[3]); // EXPECT: a*b: 19000 22000 43000 50000

    // Identity * a = a
    b[0] = 1000; b[1] = 0; b[2] = 0; b[3] = 1000;
    mat_mul(b, a, c, 2);
    printf("I*a: %d %d %d %d\n", c[0], c[1], c[2], c[3]); // EXPECT: I*a: 1000 2000 3000 4000
}

void test_transpose(void) {
    int a[9];
    int b[9];

    printf("=== Matrix Transpose 3x3 ===\n"); // EXPECT: === Matrix Transpose 3x3 ===

    // a = [[1,2,3],[4,5,6],[7,8,9]] * 1000
    a[0] = 1000; a[1] = 2000; a[2] = 3000;
    a[3] = 4000; a[4] = 5000; a[5] = 6000;
    a[6] = 7000; a[7] = 8000; a[8] = 9000;

    mat_transpose(a, b, 3);
    printf("row0: %d %d %d\n", b[0], b[1], b[2]); // EXPECT: row0: 1000 4000 7000
    printf("row1: %d %d %d\n", b[3], b[4], b[5]); // EXPECT: row1: 2000 5000 8000
    printf("row2: %d %d %d\n", b[6], b[7], b[8]); // EXPECT: row2: 3000 6000 9000
}

void test_trace(void) {
    int a[9];
    int t;

    printf("=== Matrix Trace ===\n"); // EXPECT: === Matrix Trace ===

    a[0] = 1000; a[1] = 2000; a[2] = 3000;
    a[3] = 4000; a[4] = 5000; a[5] = 6000;
    a[6] = 7000; a[7] = 8000; a[8] = 9000;

    t = mat_trace(a, 3);
    printf("trace = %d\n", t); // EXPECT: trace = 15000

    // 2x2 identity
    int id[4];
    id[0] = 1000; id[1] = 0; id[2] = 0; id[3] = 1000;
    t = mat_trace(id, 2);
    printf("trace(I2) = %d\n", t); // EXPECT: trace(I2) = 2000
}

void test_mat_vec(void) {
    int a[4];
    int x[2];
    int y[2];

    printf("=== Matrix-Vector Multiply ===\n"); // EXPECT: === Matrix-Vector Multiply ===

    a[0] = 1000; a[1] = 2000; a[2] = 3000; a[3] = 4000;
    x[0] = 1000; x[1] = 1000;

    mat_vec_mul(a, x, y, 2);
    printf("A*[1,1] = [%d, %d]\n", y[0], y[1]); // EXPECT: A*[1,1] = [3000, 7000]

    x[0] = 2000; x[1] = 3000;
    mat_vec_mul(a, x, y, 2);
    printf("A*[2,3] = [%d, %d]\n", y[0], y[1]); // EXPECT: A*[2,3] = [8000, 18000]
}

void test_3x3_mul(void) {
    int a[9];
    int b[9];
    int c[9];

    printf("=== 3x3 Multiply ===\n"); // EXPECT: === 3x3 Multiply ===

    // Identity matrix
    a[0] = 1000; a[1] = 0;    a[2] = 0;
    a[3] = 0;    a[4] = 1000; a[5] = 0;
    a[6] = 0;    a[7] = 0;    a[8] = 1000;

    b[0] = 2000; b[1] = 3000; b[2] = 4000;
    b[3] = 5000; b[4] = 6000; b[5] = 7000;
    b[6] = 8000; b[7] = 9000; b[8] = 10000;

    mat_mul(a, b, c, 3);
    printf("I*B row0: %d %d %d\n", c[0], c[1], c[2]); // EXPECT: I*B row0: 2000 3000 4000
    printf("I*B row1: %d %d %d\n", c[3], c[4], c[5]); // EXPECT: I*B row1: 5000 6000 7000
    printf("I*B row2: %d %d %d\n", c[6], c[7], c[8]); // EXPECT: I*B row2: 8000 9000 10000
}

int main(void) {
    test_add_sub();
    test_multiply();
    test_transpose();
    test_trace();
    test_mat_vec();
    test_3x3_mul();
    printf("DONE\n"); // EXPECT: DONE
    return 0;
}
