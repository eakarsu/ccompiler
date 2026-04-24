int printf(const char *fmt, ...);

// Matrix inverse, eigenvalue estimation, and power iteration
// with x1000 scaling. Max 3x3 matrices.

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

// 2x2 determinant: (a*d - b*c) / 1000
int det2x2(int m[]) {
    return (int)(((long long)m[0] * m[3] - (long long)m[1] * m[2]) / 1000);
}

// 2x2 matrix inverse: inv = adj / det
// m: 2x2 input, inv: 2x2 output, all x1000
// Returns 0 if singular
int inv2x2(int m[], int inv[]) {
    int d;
    d = det2x2(m);
    if (abs_val(d) < 1) return 0;

    inv[0] = (int)((long long)m[3] * 1000 / d);
    inv[1] = (int)((long long)(-m[1]) * 1000 / d);
    inv[2] = (int)((long long)(-m[2]) * 1000 / d);
    inv[3] = (int)((long long)m[0] * 1000 / d);
    return 1;
}

// 2x2 matrix multiply: c = a * b
void mul2x2(int a[], int b[], int c[]) {
    int i;
    int j;
    int k;
    long long sum;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            sum = 0;
            for (k = 0; k < 2; k++) {
                sum = sum + (long long)a[i * 2 + k] * b[k * 2 + j];
            }
            c[i * 2 + j] = (int)(sum / 1000);
        }
    }
}

// Matrix-vector multiply for 3x3: y = A*x
void matvec3(int a[], int x[], int y[]) {
    int i;
    int j;
    long long sum;
    for (i = 0; i < 3; i++) {
        sum = 0;
        for (j = 0; j < 3; j++) {
            sum = sum + (long long)a[i * 3 + j] * x[j];
        }
        y[i] = (int)(sum / 1000);
    }
}

// Dot product of two 3-vectors (x1000)
int dot3(int a[], int b[]) {
    long long s;
    s = (long long)a[0] * b[0] + (long long)a[1] * b[1] + (long long)a[2] * b[2];
    return (int)(s / 1000);
}

// Compute vector norm (approximate, integer sqrt of sum of squares)
int vec_norm3(int v[]) {
    long long ss;
    int x;
    int prev;
    int i;

    ss = ((long long)v[0] * v[0] + (long long)v[1] * v[1] + (long long)v[2] * v[2]) / 1000;
    // Newton sqrt
    x = (int)ss;
    if (x <= 0) x = 1;
    for (i = 0; i < 50; i++) {
        prev = x;
        x = (int)((x + (int)(ss * 1000 / x)) / 2);
        if (abs_val(x - prev) <= 1) break;
    }
    return x;
}

// Normalize a 3-vector (scale so norm ~ 1000)
void normalize3(int v[], int out[]) {
    int n;
    n = vec_norm3(v);
    if (n == 0) {
        out[0] = 0; out[1] = 0; out[2] = 0;
        return;
    }
    out[0] = (int)((long long)v[0] * 1000 / n);
    out[1] = (int)((long long)v[1] * 1000 / n);
    out[2] = (int)((long long)v[2] * 1000 / n);
}

// Power iteration: find dominant eigenvalue of 3x3 matrix
// Returns eigenvalue * 1000
int power_iteration(int a[], int iters) {
    int v[3];
    int w[3];
    int nv[3];
    int eigenval;
    int i;

    // Initial vector
    v[0] = 1000; v[1] = 0; v[2] = 0;

    for (i = 0; i < iters; i++) {
        matvec3(a, v, w);
        normalize3(w, nv);
        v[0] = nv[0]; v[1] = nv[1]; v[2] = nv[2];
    }

    // Eigenvalue = v^T * A * v
    matvec3(a, v, w);
    eigenval = dot3(v, w);

    return eigenval;
}

void test_inv2x2(void) {
    int m[4];
    int inv[4];
    int prod[4];
    int ok;

    printf("=== 2x2 Inverse ===\n"); // EXPECT: === 2x2 Inverse ===

    // A = [[2, 1], [1, 1]] -> det = 1
    // inv = [[1, -1], [-1, 2]]
    m[0] = 2000; m[1] = 1000; m[2] = 1000; m[3] = 1000;
    ok = inv2x2(m, inv);
    printf("ok=%d inv: %d %d %d %d\n", ok, inv[0], inv[1], inv[2], inv[3]); // EXPECT: ok=1 inv: 1000 -1000 -1000 2000

    // Verify A * A^-1 = I
    mul2x2(m, inv, prod);
    printf("A*inv: %d %d %d %d\n", prod[0], prod[1], prod[2], prod[3]); // EXPECT: A*inv: 1000 0 0 1000

    // A = [[4, 7], [2, 6]] -> det = 10
    m[0] = 4000; m[1] = 7000; m[2] = 2000; m[3] = 6000;
    ok = inv2x2(m, inv);
    printf("ok=%d inv: %d %d %d %d\n", ok, inv[0], inv[1], inv[2], inv[3]); // EXPECT: ok=1 inv: 600 -700 -200 400

    mul2x2(m, inv, prod);
    printf("A*inv: %d %d %d %d\n", prod[0], prod[1], prod[2], prod[3]); // EXPECT: A*inv: 1000 0 0 1000

    // Singular matrix: [[1, 2], [2, 4]]
    m[0] = 1000; m[1] = 2000; m[2] = 2000; m[3] = 4000;
    ok = inv2x2(m, inv);
    printf("singular: ok=%d\n", ok); // EXPECT: singular: ok=0
}

void test_power_iteration(void) {
    int a[9];
    int ev;

    printf("=== Power Iteration ===\n"); // EXPECT: === Power Iteration ===

    // Diagonal matrix: eigenvalues 5, 2, 1
    a[0] = 5000; a[1] = 0;    a[2] = 0;
    a[3] = 0;    a[4] = 2000; a[5] = 0;
    a[6] = 0;    a[7] = 0;    a[8] = 1000;
    ev = power_iteration(a, 30);
    printf("eigenval diag [5,2,1]: %d\n", ev); // EXPECT: eigenval diag [5,2,1]: 5000

    // Symmetric: [[3, 1, 0], [1, 3, 1], [0, 1, 3]]
    // Eigenvalues: 3+sqrt(2), 3, 3-sqrt(2) ~ 4.414, 3, 1.586
    a[0] = 3000; a[1] = 1000; a[2] = 0;
    a[3] = 1000; a[4] = 3000; a[5] = 1000;
    a[6] = 0;    a[7] = 1000; a[8] = 3000;
    ev = power_iteration(a, 50);
    printf("eigenval sym: %d\n", ev); // EXPECT: eigenval sym: 4409

    // Identity: eigenvalue = 1
    a[0] = 1000; a[1] = 0;    a[2] = 0;
    a[3] = 0;    a[4] = 1000; a[5] = 0;
    a[6] = 0;    a[7] = 0;    a[8] = 1000;
    ev = power_iteration(a, 30);
    printf("eigenval I3: %d\n", ev); // EXPECT: eigenval I3: 1000
}

void test_vector_ops(void) {
    int v[3];
    int nv[3];
    int n;

    printf("=== Vector Operations ===\n"); // EXPECT: === Vector Operations ===

    v[0] = 3000; v[1] = 4000; v[2] = 0;
    n = vec_norm3(v);
    printf("norm [3,4,0] = %d\n", n); // EXPECT: norm [3,4,0] = 5000

    normalize3(v, nv);
    printf("normalized: %d %d %d\n", nv[0], nv[1], nv[2]); // EXPECT: normalized: 600 800 0

    v[0] = 1000; v[1] = 1000; v[2] = 1000;
    n = vec_norm3(v);
    printf("norm [1,1,1] = %d\n", n); // EXPECT: norm [1,1,1] = 1732

    int d;
    int a[3];
    int b[3];
    a[0] = 1000; a[1] = 0; a[2] = 0;
    b[0] = 0; b[1] = 1000; b[2] = 0;
    d = dot3(a, b);
    printf("dot [1,0,0].[0,1,0] = %d\n", d); // EXPECT: dot [1,0,0].[0,1,0] = 0

    a[0] = 1000; a[1] = 2000; a[2] = 3000;
    b[0] = 4000; b[1] = 5000; b[2] = 6000;
    d = dot3(a, b);
    printf("dot [1,2,3].[4,5,6] = %d\n", d); // EXPECT: dot [1,2,3].[4,5,6] = 32000
}

int main(void) {
    test_inv2x2();
    test_power_iteration();
    test_vector_ops();
    printf("DONE\n"); // EXPECT: DONE
    return 0;
}
