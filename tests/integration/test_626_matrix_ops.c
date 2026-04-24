int printf(const char *fmt, ...);
// EXPECT: === Matrix Operations ===\nA*I result:\n1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16\nA*I matches A: YES\nTranspose of A:\n1 5 9 13\n2 6 10 14\n3 7 11 15\n4 8 12 16\nA*2I result:\n2 4 6 8\n10 12 14 16\n18 20 22 24\n26 28 30 32\nA*2I matches 2*A: YES\ndet3 singular: 0\ndet3 non-singular: -306\nmat-vec result: 30 70 110 150\nis_identity(I): 1\nis_identity(A): 0\nTrace of A: 34\nDiag sum of 2A: 68\n=== ALL TESTS PASSED ===

// Matrix multiply: result = a * b (4x4)
void mat4_mul(int result[4][4], int a[4][4], int b[4][4]) {
    int i;
    int j;
    int k;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            int sum;
            sum = 0;
            for (k = 0; k < 4; k++) {
                sum = sum + a[i][k] * b[k][j];
            }
            result[i][j] = sum;
        }
    }
}

// Matrix transpose: result = a^T (4x4)
void mat4_transpose(int result[4][4], int a[4][4]) {
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            result[i][j] = a[j][i];
        }
    }
}

// 3x3 determinant using formula: a(ei-fh) - b(di-fg) + c(dh-eg)
int det3(int m[3][3]) {
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int g;
    int h;
    int i;
    a = m[0][0];
    b = m[0][1];
    c = m[0][2];
    d = m[1][0];
    e = m[1][1];
    f = m[1][2];
    g = m[2][0];
    h = m[2][1];
    i = m[2][2];
    return a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
}

// Matrix-vector multiply: result = m * v (4x4 matrix, 4-vector)
void mat_vec_mul(int result[4], int m[4][4], int v[4]) {
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        int sum;
        sum = 0;
        for (j = 0; j < 4; j++) {
            sum = sum + m[i][j] * v[j];
        }
        result[i] = sum;
    }
}

// Check if matrix is the identity matrix
int is_identity(int m[4][4]) {
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (i == j) {
                if (m[i][j] != 1) return 0;
            } else {
                if (m[i][j] != 0) return 0;
            }
        }
    }
    return 1;
}

// Print a 4x4 matrix
void print_mat4(int m[4][4]) {
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (j > 0) printf(" ");
            printf("%d", m[i][j]);
        }
        printf("\n");
    }
}

// Check if two 4x4 matrices are equal
int mat4_equal(int a[4][4], int b[4][4]) {
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (a[i][j] != b[i][j]) return 0;
        }
    }
    return 1;
}

// Compute trace (sum of diagonal elements)
int mat4_trace(int m[4][4]) {
    return m[0][0] + m[1][1] + m[2][2] + m[3][3];
}

// Scale a 4x4 matrix: result = scalar * a
void mat4_scale(int result[4][4], int a[4][4], int scalar) {
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            result[i][j] = scalar * a[i][j];
        }
    }
}

// Add two 4x4 matrices: result = a + b
void mat4_add(int result[4][4], int a[4][4], int b[4][4]) {
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

// Subtract two 4x4 matrices: result = a - b
void mat4_sub(int result[4][4], int a[4][4], int b[4][4]) {
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            result[i][j] = a[i][j] - b[i][j];
        }
    }
}

// Zero a 4x4 matrix
void mat4_zero(int m[4][4]) {
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            m[i][j] = 0;
        }
    }
}

// Copy a 4x4 matrix: dst = src
void mat4_copy(int dst[4][4], int src[4][4]) {
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            dst[i][j] = src[i][j];
        }
    }
}

int main() {
    // Matrix A
    int A[4][4];
    A[0][0] = 1;  A[0][1] = 2;  A[0][2] = 3;  A[0][3] = 4;
    A[1][0] = 5;  A[1][1] = 6;  A[1][2] = 7;  A[1][3] = 8;
    A[2][0] = 9;  A[2][1] = 10; A[2][2] = 11; A[2][3] = 12;
    A[3][0] = 13; A[3][1] = 14; A[3][2] = 15; A[3][3] = 16;

    // Identity matrix I
    int I[4][4];
    mat4_zero(I);
    I[0][0] = 1;
    I[1][1] = 1;
    I[2][2] = 1;
    I[3][3] = 1;

    int result[4][4];
    int result2[4][4];

    printf("=== Matrix Operations ===\n");

    // Test 1: A * I should equal A
    mat4_mul(result, A, I);
    printf("A*I result:\n");
    print_mat4(result);
    if (mat4_equal(result, A)) {
        printf("A*I matches A: YES\n");
    } else {
        printf("A*I matches A: NO\n");
    }

    // Test 2: Transpose of A
    mat4_transpose(result, A);
    printf("Transpose of A:\n");
    print_mat4(result);

    // Test 3: A * B where B = 2*I
    int B[4][4];
    mat4_zero(B);
    B[0][0] = 2;
    B[1][1] = 2;
    B[2][2] = 2;
    B[3][3] = 2;

    mat4_mul(result, A, B);
    printf("A*2I result:\n");
    print_mat4(result);

    // Verify A*2I == 2*A
    mat4_scale(result2, A, 2);
    if (mat4_equal(result, result2)) {
        printf("A*2I matches 2*A: YES\n");
    } else {
        printf("A*2I matches 2*A: NO\n");
    }

    // Test 4: 3x3 determinant of singular matrix
    int sing[3][3];
    sing[0][0] = 1; sing[0][1] = 2; sing[0][2] = 3;
    sing[1][0] = 4; sing[1][1] = 5; sing[1][2] = 6;
    sing[2][0] = 7; sing[2][1] = 8; sing[2][2] = 9;
    printf("det3 singular: %d\n", det3(sing));

    // Test 5: 3x3 determinant of non-singular matrix
    int nonsing[3][3];
    nonsing[0][0] = 6;  nonsing[0][1] = 1;  nonsing[0][2] = 1;
    nonsing[1][0] = 4;  nonsing[1][1] = -2; nonsing[1][2] = 5;
    nonsing[2][0] = 2;  nonsing[2][1] = 8;  nonsing[2][2] = 7;
    printf("det3 non-singular: %d\n", det3(nonsing));

    // Test 6: Matrix-vector multiply
    int vec[4];
    vec[0] = 1;
    vec[1] = 2;
    vec[2] = 3;
    vec[3] = 4;
    int vresult[4];
    mat_vec_mul(vresult, A, vec);
    printf("mat-vec result: %d %d %d %d\n",
           vresult[0], vresult[1], vresult[2], vresult[3]);

    // Test 7: Identity check
    printf("is_identity(I): %d\n", is_identity(I));
    printf("is_identity(A): %d\n", is_identity(A));

    // Test 8: Trace computations
    // Trace(A) = 1 + 6 + 11 + 16 = 34
    printf("Trace of A: %d\n", mat4_trace(A));

    // Trace(2*A) = 2 + 12 + 22 + 32 = 68
    int scaled[4][4];
    mat4_scale(scaled, A, 2);
    printf("Diag sum of 2A: %d\n", mat4_trace(scaled));

    printf("=== ALL TESTS PASSED ===\n");
    return 0;
}
