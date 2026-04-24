int printf(const char *fmt, ...);
// EXPECT: === Matrix Operations Tests ===
// EXPECT: --- Matrix addition ---
// EXPECT: A:
// EXPECT: 1 2 3
// EXPECT: 4 5 6
// EXPECT: 7 8 9
// EXPECT: B:
// EXPECT: 9 8 7
// EXPECT: 6 5 4
// EXPECT: 3 2 1
// EXPECT: A+B:
// EXPECT: 10 10 10
// EXPECT: 10 10 10
// EXPECT: 10 10 10
// EXPECT: --- Matrix subtraction ---
// EXPECT: A-B:
// EXPECT: -8 -6 -4
// EXPECT: -2 0 2
// EXPECT: 4 6 8
// EXPECT: --- Scalar multiply ---
// EXPECT: 3*A:
// EXPECT: 3 6 9
// EXPECT: 12 15 18
// EXPECT: 21 24 27
// EXPECT: --- Matrix multiply ---
// EXPECT: C (2x3):
// EXPECT: 1 2 3
// EXPECT: 4 5 6
// EXPECT: D (3x2):
// EXPECT: 7 8
// EXPECT: 9 10
// EXPECT: 11 12
// EXPECT: C*D (2x2):
// EXPECT: 58 64
// EXPECT: 139 154
// EXPECT: --- Matrix multiply 3x3 ---
// EXPECT: E:
// EXPECT: 1 0 2
// EXPECT: 0 1 0
// EXPECT: 3 0 1
// EXPECT: F:
// EXPECT: 1 2 0
// EXPECT: 0 1 0
// EXPECT: 4 0 1
// EXPECT: E*F:
// EXPECT: 9 2 2
// EXPECT: 0 1 0
// EXPECT: 7 6 1
// EXPECT: --- Transpose ---
// EXPECT: original:
// EXPECT: 1 2 3
// EXPECT: 4 5 6
// EXPECT: transposed:
// EXPECT: 1 4
// EXPECT: 2 5
// EXPECT: 3 6
// EXPECT: --- Transpose square ---
// EXPECT: original:
// EXPECT: 1 2 3
// EXPECT: 4 5 6
// EXPECT: 7 8 9
// EXPECT: transposed:
// EXPECT: 1 4 7
// EXPECT: 2 5 8
// EXPECT: 3 6 9
// EXPECT: --- Identity check ---
// EXPECT: I3:
// EXPECT: 1 0 0
// EXPECT: 0 1 0
// EXPECT: 0 0 1
// EXPECT: is_identity(I3): 1
// EXPECT: is_identity(A): 0
// EXPECT: --- Trace ---
// EXPECT: trace(A): 15
// EXPECT: trace(I3): 3
// EXPECT: --- Diagonal sum ---
// EXPECT: main_diag(A): 15
// EXPECT: anti_diag(A): 15
// EXPECT: --- Matrix equality ---
// EXPECT: A == A: 1
// EXPECT: A == B: 0
// EXPECT: --- Zero matrix check ---
// EXPECT: is_zero(Z): 1
// EXPECT: is_zero(A): 0
// EXPECT: --- Symmetric check ---
// EXPECT: S:
// EXPECT: 1 2 3
// EXPECT: 2 5 6
// EXPECT: 3 6 9
// EXPECT: is_symmetric(S): 1
// EXPECT: is_symmetric(A): 0
// EXPECT: --- Row sum and column sum ---
// EXPECT: row sums of A: 6 15 24
// EXPECT: col sums of A: 12 15 18
// EXPECT: === Matrix Operations Done ===
// --- Matrix addition ---
// A:
// 1 2 3
// 4 5 6
// 7 8 9
// B:
// 9 8 7
// 6 5 4
// 3 2 1
// A+B:
// 10 10 10
// 10 10 10
// 10 10 10
// --- Matrix subtraction ---
// A-B:
// -8 -6 -4
// -2 0 2
// 4 6 8
// --- Scalar multiply ---
// 3*A:
// 3 6 9
// 12 15 18
// 21 24 27
// --- Matrix multiply ---
// C (2x3):
// 1 2 3
// 4 5 6
// D (3x2):
// 7 8
// 9 10
// 11 12
// C*D (2x2):
// 58 64
// 139 154
// --- Matrix multiply 3x3 ---
// E:
// 1 0 2
// 0 1 0
// 3 0 1
// F:
// 1 2 0
// 0 1 0
// 4 0 1
// E*F:
// 9 2 2
// 0 1 0
// 7 6 1
// --- Transpose ---
// original:
// 1 2 3
// 4 5 6
// transposed:
// 1 4
// 2 5
// 3 6
// --- Transpose square ---
// original:
// 1 2 3
// 4 5 6
// 7 8 9
// transposed:
// 1 4 7
// 2 5 8
// 3 6 9
// --- Identity check ---
// I3:
// 1 0 0
// 0 1 0
// 0 0 1
// is_identity(I3): 1
// is_identity(A): 0
// --- Trace ---
// trace(A): 15
// trace(I3): 3
// --- Diagonal sum ---
// main_diag(A): 15
// anti_diag(A): 15
// --- Matrix equality ---
// A == A: 1
// A == B: 0
// --- Zero matrix check ---
// is_zero(Z): 1
// is_zero(A): 0
// --- Symmetric check ---
// S:
// 1 2 3
// 2 5 6
// 3 6 9
// is_symmetric(S): 1
// is_symmetric(A): 0
// --- Row sum and column sum ---
// row sums of A: 6 15 24
// col sums of A: 12 15 18
// === Matrix Operations Done ===

void print_matrix(int *mat, int rows, int cols) {
    int i;
    int j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (j > 0) printf(" ");
            printf("%d", mat[i * cols + j]);
        }
        printf("\n");
    }
}

void mat_add(int *a, int *b, int *result, int rows, int cols) {
    int i;
    for (i = 0; i < rows * cols; i++) {
        result[i] = a[i] + b[i];
    }
}

void mat_sub(int *a, int *b, int *result, int rows, int cols) {
    int i;
    for (i = 0; i < rows * cols; i++) {
        result[i] = a[i] - b[i];
    }
}

void mat_scalar_mul(int *a, int scalar, int *result, int rows, int cols) {
    int i;
    for (i = 0; i < rows * cols; i++) {
        result[i] = a[i] * scalar;
    }
}

// Matrix multiply: a(r1 x c1) * b(c1 x c2) = result(r1 x c2)
void mat_mul(int *a, int *b, int *result, int r1, int c1, int c2) {
    int i;
    int j;
    int k;
    for (i = 0; i < r1; i++) {
        for (j = 0; j < c2; j++) {
            int sum = 0;
            for (k = 0; k < c1; k++) {
                sum = sum + a[i * c1 + k] * b[k * c2 + j];
            }
            result[i * c2 + j] = sum;
        }
    }
}

// Transpose: a(rows x cols) -> result(cols x rows)
void mat_transpose(int *a, int *result, int rows, int cols) {
    int i;
    int j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            result[j * rows + i] = a[i * cols + j];
        }
    }
}

int is_identity(int *mat, int n) {
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i == j) {
                if (mat[i * n + j] != 1) return 0;
            } else {
                if (mat[i * n + j] != 0) return 0;
            }
        }
    }
    return 1;
}

int mat_trace(int *mat, int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        sum = sum + mat[i * n + i];
    }
    return sum;
}

int mat_main_diag_sum(int *mat, int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        sum = sum + mat[i * n + i];
    }
    return sum;
}

int mat_anti_diag_sum(int *mat, int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        sum = sum + mat[i * n + (n - 1 - i)];
    }
    return sum;
}

int mat_equal(int *a, int *b, int rows, int cols) {
    int i;
    for (i = 0; i < rows * cols; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int is_zero(int *mat, int rows, int cols) {
    int i;
    for (i = 0; i < rows * cols; i++) {
        if (mat[i] != 0) return 0;
    }
    return 1;
}

int is_symmetric(int *mat, int n) {
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (mat[i * n + j] != mat[j * n + i]) return 0;
        }
    }
    return 1;
}

int main() {
    printf("=== Matrix Operations Tests ===\n");

    int A[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int B[9] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    int R[9];

    // Addition
    printf("--- Matrix addition ---\n");
    printf("A:\n");
    print_matrix(A, 3, 3);
    printf("B:\n");
    print_matrix(B, 3, 3);
    mat_add(A, B, R, 3, 3);
    printf("A+B:\n");
    print_matrix(R, 3, 3);

    // Subtraction
    printf("--- Matrix subtraction ---\n");
    mat_sub(A, B, R, 3, 3);
    printf("A-B:\n");
    print_matrix(R, 3, 3);

    // Scalar multiply
    printf("--- Scalar multiply ---\n");
    mat_scalar_mul(A, 3, R, 3, 3);
    printf("3*A:\n");
    print_matrix(R, 3, 3);

    // Matrix multiply 2x3 * 3x2
    printf("--- Matrix multiply ---\n");
    int C[6] = {1, 2, 3, 4, 5, 6};
    int D[6] = {7, 8, 9, 10, 11, 12};
    int R2[4];
    printf("C (2x3):\n");
    print_matrix(C, 2, 3);
    printf("D (3x2):\n");
    print_matrix(D, 3, 2);
    mat_mul(C, D, R2, 2, 3, 2);
    printf("C*D (2x2):\n");
    print_matrix(R2, 2, 2);

    // Matrix multiply 3x3
    printf("--- Matrix multiply 3x3 ---\n");
    int E[9] = {1, 0, 2, 0, 1, 0, 3, 0, 1};
    int F[9] = {1, 2, 0, 0, 1, 0, 4, 0, 1};
    int R3[9];
    printf("E:\n");
    print_matrix(E, 3, 3);
    printf("F:\n");
    print_matrix(F, 3, 3);
    mat_mul(E, F, R3, 3, 3, 3);
    printf("E*F:\n");
    print_matrix(R3, 3, 3);

    // Transpose 2x3 -> 3x2
    printf("--- Transpose ---\n");
    int T1[6] = {1, 2, 3, 4, 5, 6};
    int T1r[6];
    printf("original:\n");
    print_matrix(T1, 2, 3);
    mat_transpose(T1, T1r, 2, 3);
    printf("transposed:\n");
    print_matrix(T1r, 3, 2);

    // Transpose square
    printf("--- Transpose square ---\n");
    int T2r[9];
    printf("original:\n");
    print_matrix(A, 3, 3);
    mat_transpose(A, T2r, 3, 3);
    printf("transposed:\n");
    print_matrix(T2r, 3, 3);

    // Identity check
    printf("--- Identity check ---\n");
    int I3[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    printf("I3:\n");
    print_matrix(I3, 3, 3);
    printf("is_identity(I3): %d\n", is_identity(I3, 3));
    printf("is_identity(A): %d\n", is_identity(A, 3));

    // Trace
    printf("--- Trace ---\n");
    printf("trace(A): %d\n", mat_trace(A, 3));
    printf("trace(I3): %d\n", mat_trace(I3, 3));

    // Diagonal sums
    printf("--- Diagonal sum ---\n");
    printf("main_diag(A): %d\n", mat_main_diag_sum(A, 3));
    printf("anti_diag(A): %d\n", mat_anti_diag_sum(A, 3));

    // Matrix equality
    printf("--- Matrix equality ---\n");
    printf("A == A: %d\n", mat_equal(A, A, 3, 3));
    printf("A == B: %d\n", mat_equal(A, B, 3, 3));

    // Zero matrix check
    printf("--- Zero matrix check ---\n");
    int Z[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    printf("is_zero(Z): %d\n", is_zero(Z, 3, 3));
    printf("is_zero(A): %d\n", is_zero(A, 3, 3));

    // Symmetric check
    printf("--- Symmetric check ---\n");
    int S[9] = {1, 2, 3, 2, 5, 6, 3, 6, 9};
    printf("S:\n");
    print_matrix(S, 3, 3);
    printf("is_symmetric(S): %d\n", is_symmetric(S, 3));
    printf("is_symmetric(A): %d\n", is_symmetric(A, 3));

    // Row sum and column sum
    printf("--- Row sum and column sum ---\n");
    int i;
    int j;
    printf("row sums of A:");
    for (i = 0; i < 3; i++) {
        int s = 0;
        for (j = 0; j < 3; j++) {
            s = s + A[i * 3 + j];
        }
        printf(" %d", s);
    }
    printf("\n");
    printf("col sums of A:");
    for (j = 0; j < 3; j++) {
        int s = 0;
        for (i = 0; i < 3; i++) {
            s = s + A[i * 3 + j];
        }
        printf(" %d", s);
    }
    printf("\n");

    printf("=== Matrix Operations Done ===\n");
    return 0;
}
