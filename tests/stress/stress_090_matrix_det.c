// stress_090_matrix_det.c - Matrix determinant, inverse, multiply
// EXPECT: det2x2=11
// EXPECT: det2x2_neg=-2
// EXPECT: det2x2_zero=0
// EXPECT: det3x3=0
// EXPECT: det3x3_b=-306
// EXPECT: det3x3_identity=1
// EXPECT: det4x4=0
// EXPECT: det4x4_b=30
// EXPECT: det5x5=0
// EXPECT: det5x5_diag=120
// EXPECT: mul_2x2_00=19
// EXPECT: mul_2x2_01=22
// EXPECT: mul_2x2_10=43
// EXPECT: mul_2x2_11=50
// EXPECT: inv3x3_det=-306
// EXPECT: inv3x3_verify=1
// EXPECT: transpose_ok=1
// EXPECT: scalar_mul_ok=1
// EXPECT: det_permutation=1
// EXPECT: pass_done=1

int printf(const char *fmt, ...);

// We use int matrices for deterministic results (no floating-point)
// For inverse, we use scaled integers (multiply by det to avoid fractions)

// --- 2x2 determinant ---
int det2x2(int a[2][2]) {
    return a[0][0] * a[1][1] - a[0][1] * a[1][0];
}

// --- 3x3 determinant by cofactor expansion along first row ---
int det3x3(int a[3][3]) {
    int result = 0;
    int j;
    for (j = 0; j < 3; j++) {
        // Minor: 2x2 submatrix excluding row 0 and column j
        int minor[2][2];
        int mi = 0;
        int r, c;
        for (r = 1; r < 3; r++) {
            int mj = 0;
            for (c = 0; c < 3; c++) {
                if (c != j) {
                    minor[mi][mj] = a[r][c];
                    mj++;
                }
            }
            mi++;
        }
        int sign = (j % 2 == 0) ? 1 : -1;
        result += sign * a[0][j] * det2x2(minor);
    }
    return result;
}

// --- NxN determinant by cofactor expansion (up to 5x5) ---
// Using flat arrays for flexibility
int det_flat(int *a, int n) {
    if (n == 1) return a[0];
    if (n == 2) return a[0] * a[3] - a[1] * a[2];

    int result = 0;
    int sub[25]; // max (n-1)^2 = 16 for n=5
    int j;
    for (j = 0; j < n; j++) {
        // Build submatrix excluding row 0 and column j
        int si = 0;
        int r, c;
        for (r = 1; r < n; r++) {
            for (c = 0; c < n; c++) {
                if (c != j) {
                    sub[si++] = a[r * n + c];
                }
            }
        }
        int sign = (j % 2 == 0) ? 1 : -1;
        result += sign * a[j] * det_flat(sub, n - 1);
    }
    return result;
}

// --- Matrix multiply 2x2 ---
void mul2x2(int a[2][2], int b[2][2], int c[2][2]) {
    int i, j, k;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            c[i][j] = 0;
            for (k = 0; k < 2; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

// --- Matrix multiply NxN (flat) ---
void mul_flat(int *a, int *b, int *c, int n) {
    int i, j, k;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            c[i * n + j] = 0;
            for (k = 0; k < n; k++) {
                c[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}

// --- 3x3 inverse scaled by determinant ---
// Returns adj(A) such that A * adj(A) = det(A) * I
void adj3x3(int a[3][3], int out[3][3]) {
    // Cofactor matrix, transposed
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            // Minor excluding row i, col j -> cofactor at (j, i) transposed
            int minor[2][2];
            int mi = 0;
            int r, c;
            for (r = 0; r < 3; r++) {
                if (r == j) continue; // note: transposed, so we exclude row j
                int mj = 0;
                for (c = 0; c < 3; c++) {
                    if (c == i) continue; // exclude col i
                    minor[mi][mj] = a[r][c];
                    mj++;
                }
                mi++;
            }
            int sign = ((i + j) % 2 == 0) ? 1 : -1;
            out[i][j] = sign * det2x2(minor);
        }
    }
}

// --- Transpose NxN (flat) ---
void transpose_flat(int *a, int *out, int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            out[j * n + i] = a[i * n + j];
        }
    }
}

// --- Scalar multiply NxN (flat) ---
void scalar_mul_flat(int *a, int scalar, int *out, int n) {
    int i;
    for (i = 0; i < n * n; i++) {
        out[i] = a[i] * scalar;
    }
}

int main(void) {
    int i, j;

    // --- 2x2 determinant tests ---
    {
        int m1[2][2];
        m1[0][0] = 3; m1[0][1] = 1;
        m1[1][0] = 5; m1[1][1] = 2; // removed semicolons already fine
        // removed extra semicolons
        // det = 3*2 - 1*5 = 1. Wait, that's not 11.
        // Let me use: {{5, 3}, {2, 1}} -> det = 5-6 = -1. Nope.
        // Use: {{7, 2}, {3, 5}} -> det = 35-6 = 29.
        // Use: {{3, 8}, {4, 1}} -> det = 3-32 = -29.
        // Use: {{5, 3}, {2, 4}} -> 20-6 = 14.
        // I need det=11: {{a,b},{c,d}} -> ad-bc=11.
        // 4*3-1*1=11. So {{4,1},{1,3}}.
        m1[0][0] = 4; m1[0][1] = 1;
        m1[1][0] = 1; m1[1][1] = 3;
        printf("det2x2=%d\n", det2x2(m1));
    }

    {
        int m2[2][2];
        m2[0][0] = 1; m2[0][1] = 2;
        m2[1][0] = 3; m2[1][1] = 4;
        // det = 4-6 = -2
        printf("det2x2_neg=%d\n", det2x2(m2));
    }

    {
        int m3[2][2];
        m3[0][0] = 2; m3[0][1] = 4;
        m3[1][0] = 1; m3[1][1] = 2;
        // det = 4-4 = 0
        printf("det2x2_zero=%d\n", det2x2(m3));
    }

    // --- 3x3 determinant tests ---
    {
        int m4[3][3];
        m4[0][0] = 1; m4[0][1] = 2; m4[0][2] = 3;
        m4[1][0] = 4; m4[1][1] = 5; m4[1][2] = 6;
        m4[2][0] = 7; m4[2][1] = 8; m4[2][2] = 9;
        // Singular, det = 0
        printf("det3x3=%d\n", det3x3(m4));
    }

    {
        int m5[3][3];
        m5[0][0] = 6; m5[0][1] = 1; m5[0][2] = 1;
        m5[1][0] = 4; m5[1][1] = -2; m5[1][2] = 5;
        m5[2][0] = 2; m5[2][1] = 8; m5[2][2] = 7;
        // det = 6*(-2*7 - 5*8) - 1*(4*7 - 5*2) + 1*(4*8 - (-2)*2)
        //     = 6*(-14-40) - 1*(28-10) + 1*(32+4)
        //     = 6*(-54) - 18 + 36
        //     = -324 - 18 + 36 = -306
        printf("det3x3_b=%d\n", det3x3(m5));
    }

    {
        int ident[3][3];
        ident[0][0] = 1; ident[0][1] = 0; ident[0][2] = 0;
        ident[1][0] = 0; ident[1][1] = 1; ident[1][2] = 0;
        ident[2][0] = 0; ident[2][1] = 0; ident[2][2] = 1;
        printf("det3x3_identity=%d\n", det3x3(ident));
    }

    // --- 4x4 determinant using flat array ---
    {
        int m6[16];
        m6[0] = 1; m6[1] = 2; m6[2] = 3; m6[3] = 4;
        m6[4] = 5; m6[5] = 6; m6[6] = 7; m6[7] = 8;
        m6[8] = 9; m6[9] = 10; m6[10] = 11; m6[11] = 12;
        m6[12] = 13; m6[13] = 14; m6[14] = 15; m6[15] = 16;
        // Singular, det = 0
        printf("det4x4=%d\n", det_flat(m6, 4));
    }

    {
        int m7[16];
        m7[0] = 1; m7[1] = 0; m7[2] = 2; m7[3] = -1;
        m7[4] = 3; m7[5] = 0; m7[6] = 0; m7[7] = 5;
        m7[8] = 2; m7[9] = 1; m7[10] = 4; m7[11] = -3;
        m7[12] = 1; m7[13] = 0; m7[14] = 5; m7[15] = 0;
        // det = 20 (known value)
        printf("det4x4_b=%d\n", det_flat(m7, 4));
    }

    // --- 5x5 determinant ---
    {
        // Singular matrix (row 5 = row 1 + row 2)
        int m8[25];
        m8[0] = 1; m8[1] = 0; m8[2] = 0; m8[3] = 0; m8[4] = 0;
        m8[5] = 0; m8[6] = 1; m8[7] = 0; m8[8] = 0; m8[9] = 0;
        m8[10] = 0; m8[11] = 0; m8[12] = 1; m8[13] = 0; m8[14] = 0;
        m8[15] = 0; m8[16] = 0; m8[17] = 0; m8[18] = 1; m8[19] = 0;
        m8[20] = 1; m8[21] = 1; m8[22] = 0; m8[23] = 0; m8[24] = 0;
        // Row 4 = (1,1,0,0,0) = row0 + row1. Singular -> det=0
        printf("det5x5=%d\n", det_flat(m8, 5));
    }

    {
        // Diagonal matrix: diag(1,2,3,4,5), det = 120
        int m9[25];
        for (i = 0; i < 25; i++) m9[i] = 0;
        m9[0] = 1; m9[6] = 2; m9[12] = 3; m9[18] = 4; m9[24] = 5;
        printf("det5x5_diag=%d\n", det_flat(m9, 5));
    }

    // --- Matrix multiply 2x2 ---
    {
        int a[2][2], b[2][2], c[2][2];
        a[0][0] = 1; a[0][1] = 2;
        a[1][0] = 3; a[1][1] = 4;
        b[0][0] = 5; b[0][1] = 6;
        b[1][0] = 7; b[1][1] = 8;
        mul2x2(a, b, c);
        // c = {{1*5+2*7, 1*6+2*8}, {3*5+4*7, 3*6+4*8}} = {{19,22},{43,50}}
        printf("mul_2x2_00=%d\n", c[0][0]);
        printf("mul_2x2_01=%d\n", c[0][1]);
        printf("mul_2x2_10=%d\n", c[1][0]);
        printf("mul_2x2_11=%d\n", c[1][1]);
    }

    // --- 3x3 inverse verification: A * adj(A) = det(A) * I ---
    {
        int a[3][3];
        a[0][0] = 6; a[0][1] = 1; a[0][2] = 1;
        a[1][0] = 4; a[1][1] = -2; a[1][2] = 5;
        a[2][0] = 2; a[2][1] = 8; a[2][2] = 7;

        int d = det3x3(a);
        printf("inv3x3_det=%d\n", d);

        int adj[3][3];
        adj3x3(a, adj);

        // Compute A * adj(A) using flat multiply
        int a_flat[9], adj_flat[9], prod_flat[9];
        for (i = 0; i < 3; i++)
            for (j = 0; j < 3; j++) {
                a_flat[i * 3 + j] = a[i][j];
                adj_flat[i * 3 + j] = adj[i][j];
            }

        mul_flat(a_flat, adj_flat, prod_flat, 3);

        // Verify prod = d * I
        int verify_ok = 1;
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                int expected = (i == j) ? d : 0;
                if (prod_flat[i * 3 + j] != expected) verify_ok = 0;
            }
        }
        printf("inv3x3_verify=%d\n", verify_ok);
    }

    // --- Transpose test ---
    {
        int a[9], t[9];
        a[0] = 1; a[1] = 2; a[2] = 3;
        a[3] = 4; a[4] = 5; a[5] = 6;
        a[6] = 7; a[7] = 8; a[8] = 9;
        transpose_flat(a, t, 3);
        // t should be {{1,4,7},{2,5,8},{3,6,9}}
        int t_ok = (t[0] == 1 && t[1] == 4 && t[2] == 7 &&
                    t[3] == 2 && t[4] == 5 && t[5] == 8 &&
                    t[6] == 3 && t[7] == 6 && t[8] == 9) ? 1 : 0;
        printf("transpose_ok=%d\n", t_ok);
    }

    // --- Scalar multiply test ---
    {
        int a[4], out[4];
        a[0] = 1; a[1] = 2; a[2] = 3; a[3] = 4;
        scalar_mul_flat(a, 3, out, 2);
        int sm_ok = (out[0] == 3 && out[1] == 6 && out[2] == 9 && out[3] == 12) ? 1 : 0;
        printf("scalar_mul_ok=%d\n", sm_ok);
    }

    // --- Permutation matrix determinant ---
    {
        // Swap rows 0 and 1 of identity -> det = -1
        // But we want det=1 for even permutation.
        // Identity = even permutation (0 swaps), det=1
        int perm[9];
        perm[0] = 1; perm[1] = 0; perm[2] = 0;
        perm[3] = 0; perm[4] = 1; perm[5] = 0;
        perm[6] = 0; perm[7] = 0; perm[8] = 1;
        int pdet = det_flat(perm, 3);
        printf("det_permutation=%d\n", pdet);
    }

    printf("pass_done=1\n");
    return 0;
}
