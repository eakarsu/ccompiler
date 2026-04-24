int printf(const char *fmt, ...);

// LU Decomposition with x1000 scaling
// Doolittle algorithm: A = L * U
// L has 1s on diagonal (1000 scaled), U is upper triangular
// Max 4x4 matrices

// LU decomposition for n x n matrix (max 4)
// a: input matrix (n*n), destroyed/overwritten
// l: output lower triangular (n*n)
// u: output upper triangular (n*n)
// All values x1000
void lu_decompose(int a[], int l[], int u[], int n) {
    int i;
    int j;
    int k;
    long long sum;

    // Initialize L and U to zero
    for (i = 0; i < n * n; i++) {
        l[i] = 0;
        u[i] = 0;
    }

    for (i = 0; i < n; i++) {
        // Upper triangular
        for (k = i; k < n; k++) {
            sum = 0;
            for (j = 0; j < i; j++) {
                sum = sum + (long long)l[i * n + j] * u[j * n + k];
            }
            u[i * n + k] = a[i * n + k] - (int)(sum / 1000);
        }

        // Lower triangular
        for (k = i; k < n; k++) {
            if (i == k) {
                l[i * n + i] = 1000; // L diagonal = 1.0 (scaled)
            } else {
                sum = 0;
                for (j = 0; j < i; j++) {
                    sum = sum + (long long)l[k * n + j] * u[j * n + i];
                }
                // l[k][i] = (a[k][i] - sum) / u[i][i]
                l[k * n + i] = (int)(((long long)(a[k * n + i] - (int)(sum / 1000))) * 1000 / u[i * n + i]);
            }
        }
    }
}

// Multiply L * U to verify decomposition
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

// Forward substitution: solve L*y = b
// L is lower triangular with 1s on diagonal (x1000)
void forward_sub(int l[], int b[], int y[], int n) {
    int i;
    int j;
    long long sum;

    for (i = 0; i < n; i++) {
        sum = 0;
        for (j = 0; j < i; j++) {
            sum = sum + (long long)l[i * n + j] * y[j];
        }
        y[i] = b[i] - (int)(sum / 1000);
    }
}

// Back substitution: solve U*x = y
void back_sub(int u[], int y[], int x[], int n) {
    int i;
    int j;
    long long sum;

    for (i = n - 1; i >= 0; i--) {
        sum = 0;
        for (j = i + 1; j < n; j++) {
            sum = sum + (long long)u[i * n + j] * x[j];
        }
        x[i] = (int)(((long long)(y[i] - (int)(sum / 1000))) * 1000 / u[i * n + i]);
    }
}

void test_lu_2x2(void) {
    int a[4];
    int l[4];
    int u[4];
    int prod[4];

    printf("=== LU Decomposition 2x2 ===\n"); // EXPECT: === LU Decomposition 2x2 ===

    // A = [[4, 3], [6, 3]] * 1000
    a[0] = 4000; a[1] = 3000;
    a[2] = 6000; a[3] = 3000;

    lu_decompose(a, l, u, 2);

    printf("L: %d %d %d %d\n", l[0], l[1], l[2], l[3]); // EXPECT: L: 1000 0 1500 1000
    printf("U: %d %d %d %d\n", u[0], u[1], u[2], u[3]); // EXPECT: U: 4000 3000 0 -1500

    // Verify L*U = A
    mat_mul(l, u, prod, 2);
    printf("L*U: %d %d %d %d\n", prod[0], prod[1], prod[2], prod[3]); // EXPECT: L*U: 4000 3000 6000 3000
}

void test_lu_3x3(void) {
    int a[9];
    int l[9];
    int u[9];
    int prod[9];

    printf("=== LU Decomposition 3x3 ===\n"); // EXPECT: === LU Decomposition 3x3 ===

    // A = [[2, -1, 0], [-1, 2, -1], [0, -1, 2]] * 1000
    a[0] = 2000;  a[1] = -1000; a[2] = 0;
    a[3] = -1000; a[4] = 2000;  a[5] = -1000;
    a[6] = 0;     a[7] = -1000; a[8] = 2000;

    lu_decompose(a, l, u, 3);

    printf("L row0: %d %d %d\n", l[0], l[1], l[2]); // EXPECT: L row0: 1000 0 0
    printf("L row1: %d %d %d\n", l[3], l[4], l[5]); // EXPECT: L row1: -500 1000 0
    printf("L row2: %d %d %d\n", l[6], l[7], l[8]); // EXPECT: L row2: 0 -666 1000
    printf("U row0: %d %d %d\n", u[0], u[1], u[2]); // EXPECT: U row0: 2000 -1000 0
    printf("U row1: %d %d %d\n", u[3], u[4], u[5]); // EXPECT: U row1: 0 1500 -1000
    printf("U row2: %d %d %d\n", u[6], u[7], u[8]); // EXPECT: U row2: 0 0 1334

    // Verify L*U = A
    mat_mul(l, u, prod, 3);
    printf("L*U row0: %d %d %d\n", prod[0], prod[1], prod[2]); // EXPECT: L*U row0: 2000 -1000 0
    printf("L*U row1: %d %d %d\n", prod[3], prod[4], prod[5]); // EXPECT: L*U row1: -1000 2000 -1000
    printf("L*U row2: %d %d %d\n", prod[6], prod[7], prod[8]); // EXPECT: L*U row2: 0 -999 2000
}

void test_lu_solve(void) {
    int a[4];
    int l[4];
    int u[4];
    int b[2];
    int y[2];
    int x[2];

    printf("=== LU Solve 2x2 ===\n"); // EXPECT: === LU Solve 2x2 ===

    // Solve: [[4,3],[6,3]] * x = [10, 12]
    // Solution: x = [1, 2]
    a[0] = 4000; a[1] = 3000;
    a[2] = 6000; a[3] = 3000;

    lu_decompose(a, l, u, 2);

    b[0] = 10000; b[1] = 12000;

    forward_sub(l, b, y, 2);
    printf("y = [%d, %d]\n", y[0], y[1]); // EXPECT: y = [10000, -3000]

    back_sub(u, y, x, 2);
    printf("x = [%d, %d]\n", x[0], x[1]); // EXPECT: x = [1000, 2000]
}

int main(void) {
    test_lu_2x2();
    test_lu_3x3();
    test_lu_solve();
    printf("DONE\n"); // EXPECT: DONE
    return 0;
}
