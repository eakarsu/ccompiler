int printf(const char *fmt, ...);
// EXPECT: === Matrix Addition ===\nA[0][0]=1 A[0][1]=2 A[0][2]=3\nA[1][0]=4 A[1][1]=5 A[1][2]=6\nA[2][0]=7 A[2][1]=8 A[2][2]=9\nB[0][0]=9 B[0][1]=8 B[0][2]=7\nB[1][0]=6 B[1][1]=5 B[1][2]=4\nB[2][0]=3 B[2][1]=2 B[2][2]=1\nC=A+B:\nC[0][0]=10 C[0][1]=10 C[0][2]=10\nC[1][0]=10 C[1][1]=10 C[1][2]=10\nC[2][0]=10 C[2][1]=10 C[2][2]=10\n=== Matrix Subtraction ===\nD=A-B:\nD[0][0]=-8 D[0][1]=-6 D[0][2]=-4\nD[1][0]=-2 D[1][1]=0 D[1][2]=2\nD[2][0]=4 D[2][1]=6 D[2][2]=8\n=== Matrix Multiply ===\nE=A*B:\nE[0][0]=30 E[0][1]=24 E[0][2]=18\nE[1][0]=84 E[1][1]=69 E[1][2]=54\nE[2][0]=138 E[2][1]=114 E[2][2]=90\n=== Transpose ===\nT=transpose(A):\nT[0][0]=1 T[0][1]=4 T[0][2]=7\nT[1][0]=2 T[1][1]=5 T[1][2]=8\nT[2][0]=3 T[2][1]=6 T[2][2]=9\n=== Trace ===\ntrace(A)=15\ntrace(B)=15\ntrace(E)=189\n=== Identity Check ===\nI[0][0]=1 I[0][1]=0 I[0][2]=0\nI[1][0]=0 I[1][1]=1 I[1][2]=0\nI[2][0]=0 I[2][1]=0 I[2][2]=1\nis_identity(I)=1\nis_identity(A)=0\n=== Matrix-Vector Multiply ===\nv[0]=1 v[1]=2 v[2]=3\nr=A*v:\nr[0]=14\nr[1]=32\nr[2]=50\n=== Determinant 2x2 ===\ndet2x2([[1,2],[3,4]])=-2\ndet2x2([[5,6],[7,8]])=-2\ndet2x2([[2,0],[0,3]])=6\n=== Determinant 3x3 ===\ndet3x3(A)=0\ndet3x3(B)=0\n=== Scalar Multiply ===\nS=3*A:\nS[0][0]=3 S[0][1]=6 S[0][2]=9\nS[1][0]=12 S[1][1]=15 S[1][2]=18\nS[2][0]=21 S[2][1]=24 S[2][2]=27\n=== Matrix Power A^2 ===\nA2=A*A:\nP[0][0]=30 P[0][1]=36 P[0][2]=42\nP[1][0]=66 P[1][1]=81 P[1][2]=96\nP[2][0]=102 P[2][1]=126 P[2][2]=150\n=== Symmetric Check ===\ns[0][0]=1 s[0][1]=2 s[0][2]=3\ns[1][0]=2 s[1][1]=5 s[1][2]=6\ns[2][0]=3 s[2][1]=6 s[2][2]=9\nis_symmetric(sym)=1\nis_symmetric(A)=0\n=== Row Sum and Col Sum ===\nrowsum(A,0)=6\nrowsum(A,1)=15\nrowsum(A,2)=24\ncolsum(A,0)=12\ncolsum(A,1)=15\ncolsum(A,2)=18\n=== Diagonal Elements ===\ndiag(A): 1 5 9\nanti_diag(A): 3 5 7\n=== Matrix Equality ===\nA==A: 1\nA==B: 0\n=== Zero Matrix ===\nZ[0][0]=0 Z[0][1]=0 Z[0][2]=0\nZ[1][0]=0 Z[1][1]=0 Z[1][2]=0\nZ[2][0]=0 Z[2][1]=0 Z[2][2]=0\nis_zero(Z)=1\nis_zero(A)=0\n=== 2x2 Operations ===\nm2[0][0]=1 m2[0][1]=2\nm2[1][0]=3 m2[1][1]=4\nm2sq[0][0]=7 m2sq[0][1]=10\nm2sq[1][0]=15 m2sq[1][1]=22\ntrace2x2=5\ndet2x2=-2\n=== Max and Min Element ===\nmax(A)=9\nmin(A)=1\nmax(B)=9\nmin(B)=1\n=== Flatten Matrix ===\nflat(A): 1 2 3 4 5 6 7 8 9\n=== Matrix Negate ===\nN[0][0]=-1 N[0][1]=-2 N[0][2]=-3\nN[1][0]=-4 N[1][1]=-5 N[1][2]=-6\nN[2][0]=-7 N[2][1]=-8 N[2][2]=-9\n=== Row Swap ===\nbefore swap: row0=1,2,3 row2=7,8,9\nafter swap: row0=7,8,9 row2=1,2,3\n=== Column Extract ===\ncol0 of A: 1 4 7\ncol1 of A: 2 5 8\ncol2 of A: 3 6 9\n=== Dot Product ===\ndot([1,2,3],[4,5,6])=32\ndot([1,0,0],[0,1,0])=0\n=== Cross Product ===\ncross([1,0,0],[0,1,0])=(0,0,1)\ncross([1,2,3],[4,5,6])=(-3,6,-3)\n=== Done ===

// --- Globals for 3x3 matrices stored as int[9], row-major ---

int g_mat_a[9];
int g_mat_b[9];
int g_mat_c[9];
int g_mat_d[9];
int g_mat_e[9];
int g_mat_t[9];
int g_mat_s[9];
int g_mat_a2[9];
int g_identity[9];
int g_sym[9];
int g_zero[9];
int g_neg[9];
int g_vec[3];
int g_rvec[3];

void mat_set(int *m, int r, int c, int val) {
    m[r * 3 + c] = val;
}

int mat_get(int *m, int r, int c) {
    return m[r * 3 + c];
}

void mat_init(int *m, int v0, int v1, int v2,
              int v3, int v4, int v5,
              int v6, int v7, int v8) {
    m[0] = v0;
    m[1] = v1;
    m[2] = v2;
    m[3] = v3;
    m[4] = v4;
    m[5] = v5;
    m[6] = v6;
    m[7] = v7;
    m[8] = v8;
}

void mat_add(int *a, int *b, int *out) {
    int i;
    i = 0;
    while (i < 9) {
        out[i] = a[i] + b[i];
        i = i + 1;
    }
}

void mat_sub(int *a, int *b, int *out) {
    int i;
    i = 0;
    while (i < 9) {
        out[i] = a[i] - b[i];
        i = i + 1;
    }
}

void mat_mul(int *a, int *b, int *out) {
    int r;
    int c;
    int k;
    int sum;
    r = 0;
    while (r < 3) {
        c = 0;
        while (c < 3) {
            sum = 0;
            k = 0;
            while (k < 3) {
                sum = sum + mat_get(a, r, k) * mat_get(b, k, c);
                k = k + 1;
            }
            mat_set(out, r, c, sum);
            c = c + 1;
        }
        r = r + 1;
    }
}

void mat_transpose(int *m, int *out) {
    int r;
    int c;
    r = 0;
    while (r < 3) {
        c = 0;
        while (c < 3) {
            mat_set(out, c, r, mat_get(m, r, c));
            c = c + 1;
        }
        r = r + 1;
    }
}

int mat_trace(int *m) {
    return mat_get(m, 0, 0) + mat_get(m, 1, 1) + mat_get(m, 2, 2);
}

void mat_print_labeled(int *m, char label) {
    int r;
    int c;
    r = 0;
    while (r < 3) {
        c = 0;
        while (c < 3) {
            printf("%c[%d][%d]=%d", label, r, c, mat_get(m, r, c));
            if (c < 2) {
                printf(" ");
            }
            c = c + 1;
        }
        printf("\n");
        r = r + 1;
    }
}

int mat_is_identity(int *m) {
    int r;
    int c;
    int expected;
    r = 0;
    while (r < 3) {
        c = 0;
        while (c < 3) {
            if (r == c) {
                expected = 1;
            } else {
                expected = 0;
            }
            if (mat_get(m, r, c) != expected) {
                return 0;
            }
            c = c + 1;
        }
        r = r + 1;
    }
    return 1;
}

void mat_make_identity(int *m) {
    int r;
    int c;
    r = 0;
    while (r < 3) {
        c = 0;
        while (c < 3) {
            if (r == c) {
                mat_set(m, r, c, 1);
            } else {
                mat_set(m, r, c, 0);
            }
            c = c + 1;
        }
        r = r + 1;
    }
}

void mat_vec_mul(int *m, int *v, int *out) {
    int r;
    int k;
    int sum;
    r = 0;
    while (r < 3) {
        sum = 0;
        k = 0;
        while (k < 3) {
            sum = sum + mat_get(m, r, k) * v[k];
            k = k + 1;
        }
        out[r] = sum;
        r = r + 1;
    }
}

int det2x2(int a, int b, int c, int d) {
    return a * d - b * c;
}

int det3x3(int *m) {
    int a00;
    int a01;
    int a02;
    int a10;
    int a11;
    int a12;
    int a20;
    int a21;
    int a22;
    a00 = mat_get(m, 0, 0);
    a01 = mat_get(m, 0, 1);
    a02 = mat_get(m, 0, 2);
    a10 = mat_get(m, 1, 0);
    a11 = mat_get(m, 1, 1);
    a12 = mat_get(m, 1, 2);
    a20 = mat_get(m, 2, 0);
    a21 = mat_get(m, 2, 1);
    a22 = mat_get(m, 2, 2);
    return a00 * (a11 * a22 - a12 * a21)
         - a01 * (a10 * a22 - a12 * a20)
         + a02 * (a10 * a21 - a11 * a20);
}

void mat_scalar_mul(int *m, int scalar, int *out) {
    int i;
    i = 0;
    while (i < 9) {
        out[i] = m[i] * scalar;
        i = i + 1;
    }
}

int mat_is_symmetric(int *m) {
    int r;
    int c;
    r = 0;
    while (r < 3) {
        c = 0;
        while (c < 3) {
            if (mat_get(m, r, c) != mat_get(m, c, r)) {
                return 0;
            }
            c = c + 1;
        }
        r = r + 1;
    }
    return 1;
}

int mat_row_sum(int *m, int row) {
    int sum;
    int c;
    sum = 0;
    c = 0;
    while (c < 3) {
        sum = sum + mat_get(m, row, c);
        c = c + 1;
    }
    return sum;
}

int mat_col_sum(int *m, int col) {
    int sum;
    int r;
    sum = 0;
    r = 0;
    while (r < 3) {
        sum = sum + mat_get(m, r, col);
        r = r + 1;
    }
    return sum;
}

int mat_equal(int *a, int *b) {
    int i;
    i = 0;
    while (i < 9) {
        if (a[i] != b[i]) {
            return 0;
        }
        i = i + 1;
    }
    return 1;
}

void mat_zero_fill(int *m) {
    int i;
    i = 0;
    while (i < 9) {
        m[i] = 0;
        i = i + 1;
    }
}

int mat_is_zero(int *m) {
    int i;
    i = 0;
    while (i < 9) {
        if (m[i] != 0) {
            return 0;
        }
        i = i + 1;
    }
    return 1;
}

int mat_max(int *m) {
    int mx;
    int i;
    mx = m[0];
    i = 1;
    while (i < 9) {
        if (m[i] > mx) {
            mx = m[i];
        }
        i = i + 1;
    }
    return mx;
}

int mat_min(int *m) {
    int mn;
    int i;
    mn = m[0];
    i = 1;
    while (i < 9) {
        if (m[i] < mn) {
            mn = m[i];
        }
        i = i + 1;
    }
    return mn;
}

void mat_negate(int *m, int *out) {
    int i;
    i = 0;
    while (i < 9) {
        out[i] = -m[i];
        i = i + 1;
    }
}

void mat_row_swap(int *m, int r1, int r2) {
    int c;
    int tmp;
    c = 0;
    while (c < 3) {
        tmp = mat_get(m, r1, c);
        mat_set(m, r1, c, mat_get(m, r2, c));
        mat_set(m, r2, c, tmp);
        c = c + 1;
    }
}

void mat_col_extract(int *m, int col, int *out) {
    int r;
    r = 0;
    while (r < 3) {
        out[r] = mat_get(m, r, col);
        r = r + 1;
    }
}

int vec_dot(int *a, int *b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void vec_cross(int *a, int *b, int *out) {
    out[0] = a[1] * b[2] - a[2] * b[1];
    out[1] = a[2] * b[0] - a[0] * b[2];
    out[2] = a[0] * b[1] - a[1] * b[0];
}

void mat2_init(int *m, int a, int b, int c, int d) {
    m[0] = a;
    m[1] = b;
    m[2] = c;
    m[3] = d;
}

void mat2_mul(int *a, int *b, int *out) {
    out[0] = a[0] * b[0] + a[1] * b[2];
    out[1] = a[0] * b[1] + a[1] * b[3];
    out[2] = a[2] * b[0] + a[3] * b[2];
    out[3] = a[2] * b[1] + a[3] * b[3];
}

int mat2_trace(int *m) {
    return m[0] + m[3];
}

int mat2_det(int *m) {
    return m[0] * m[3] - m[1] * m[2];
}

int main() {
    int m2[4];
    int m2sq[4];
    int i;
    int col_out[3];
    int v1[3];
    int v2[3];
    int cross_out[3];
    int swap_mat[9];

    // Initialize A = [[1,2,3],[4,5,6],[7,8,9]]
    mat_init(g_mat_a, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    // Initialize B = [[9,8,7],[6,5,4],[3,2,1]]
    mat_init(g_mat_b, 9, 8, 7, 6, 5, 4, 3, 2, 1);

    printf("=== Matrix Addition ===\n");
    mat_print_labeled(g_mat_a, 'A');
    mat_print_labeled(g_mat_b, 'B');

    mat_add(g_mat_a, g_mat_b, g_mat_c);
    printf("C=A+B:\n");
    mat_print_labeled(g_mat_c, 'C');

    printf("=== Matrix Subtraction ===\n");
    mat_sub(g_mat_a, g_mat_b, g_mat_d);
    printf("D=A-B:\n");
    mat_print_labeled(g_mat_d, 'D');

    printf("=== Matrix Multiply ===\n");
    mat_mul(g_mat_a, g_mat_b, g_mat_e);
    printf("E=A*B:\n");
    mat_print_labeled(g_mat_e, 'E');

    printf("=== Transpose ===\n");
    mat_transpose(g_mat_a, g_mat_t);
    printf("T=transpose(A):\n");
    mat_print_labeled(g_mat_t, 'T');

    printf("=== Trace ===\n");
    printf("trace(A)=%d\n", mat_trace(g_mat_a));
    printf("trace(B)=%d\n", mat_trace(g_mat_b));
    printf("trace(E)=%d\n", mat_trace(g_mat_e));

    printf("=== Identity Check ===\n");
    mat_make_identity(g_identity);
    mat_print_labeled(g_identity, 'I');
    printf("is_identity(I)=%d\n", mat_is_identity(g_identity));
    printf("is_identity(A)=%d\n", mat_is_identity(g_mat_a));

    printf("=== Matrix-Vector Multiply ===\n");
    g_vec[0] = 1;
    g_vec[1] = 2;
    g_vec[2] = 3;
    printf("v[0]=%d v[1]=%d v[2]=%d\n", g_vec[0], g_vec[1], g_vec[2]);
    mat_vec_mul(g_mat_a, g_vec, g_rvec);
    printf("r=A*v:\n");
    printf("r[0]=%d\n", g_rvec[0]);
    printf("r[1]=%d\n", g_rvec[1]);
    printf("r[2]=%d\n", g_rvec[2]);

    printf("=== Determinant 2x2 ===\n");
    printf("det2x2([[1,2],[3,4]])=%d\n", det2x2(1, 2, 3, 4));
    printf("det2x2([[5,6],[7,8]])=%d\n", det2x2(5, 6, 7, 8));
    printf("det2x2([[2,0],[0,3]])=%d\n", det2x2(2, 0, 0, 3));

    printf("=== Determinant 3x3 ===\n");
    printf("det3x3(A)=%d\n", det3x3(g_mat_a));
    printf("det3x3(B)=%d\n", det3x3(g_mat_b));

    printf("=== Scalar Multiply ===\n");
    mat_scalar_mul(g_mat_a, 3, g_mat_s);
    printf("S=3*A:\n");
    mat_print_labeled(g_mat_s, 'S');

    printf("=== Matrix Power A^2 ===\n");
    mat_mul(g_mat_a, g_mat_a, g_mat_a2);
    printf("A2=A*A:\n");
    mat_print_labeled(g_mat_a2, 'P');

    printf("=== Symmetric Check ===\n");
    mat_init(g_sym, 1, 2, 3, 2, 5, 6, 3, 6, 9);
    mat_print_labeled(g_sym, 's');
    printf("is_symmetric(sym)=%d\n", mat_is_symmetric(g_sym));
    printf("is_symmetric(A)=%d\n", mat_is_symmetric(g_mat_a));

    printf("=== Row Sum and Col Sum ===\n");
    printf("rowsum(A,0)=%d\n", mat_row_sum(g_mat_a, 0));
    printf("rowsum(A,1)=%d\n", mat_row_sum(g_mat_a, 1));
    printf("rowsum(A,2)=%d\n", mat_row_sum(g_mat_a, 2));
    printf("colsum(A,0)=%d\n", mat_col_sum(g_mat_a, 0));
    printf("colsum(A,1)=%d\n", mat_col_sum(g_mat_a, 1));
    printf("colsum(A,2)=%d\n", mat_col_sum(g_mat_a, 2));

    printf("=== Diagonal Elements ===\n");
    printf("diag(A): %d %d %d\n",
        mat_get(g_mat_a, 0, 0),
        mat_get(g_mat_a, 1, 1),
        mat_get(g_mat_a, 2, 2));
    printf("anti_diag(A): %d %d %d\n",
        mat_get(g_mat_a, 0, 2),
        mat_get(g_mat_a, 1, 1),
        mat_get(g_mat_a, 2, 0));

    printf("=== Matrix Equality ===\n");
    printf("A==A: %d\n", mat_equal(g_mat_a, g_mat_a));
    printf("A==B: %d\n", mat_equal(g_mat_a, g_mat_b));

    printf("=== Zero Matrix ===\n");
    mat_zero_fill(g_zero);
    mat_print_labeled(g_zero, 'Z');
    printf("is_zero(Z)=%d\n", mat_is_zero(g_zero));
    printf("is_zero(A)=%d\n", mat_is_zero(g_mat_a));

    printf("=== 2x2 Operations ===\n");
    mat2_init(m2, 1, 2, 3, 4);
    printf("m2[0][0]=%d m2[0][1]=%d\n", m2[0], m2[1]);
    printf("m2[1][0]=%d m2[1][1]=%d\n", m2[2], m2[3]);
    mat2_mul(m2, m2, m2sq);
    printf("m2sq[0][0]=%d m2sq[0][1]=%d\n", m2sq[0], m2sq[1]);
    printf("m2sq[1][0]=%d m2sq[1][1]=%d\n", m2sq[2], m2sq[3]);
    printf("trace2x2=%d\n", mat2_trace(m2));
    printf("det2x2=%d\n", mat2_det(m2));

    printf("=== Max and Min Element ===\n");
    printf("max(A)=%d\n", mat_max(g_mat_a));
    printf("min(A)=%d\n", mat_min(g_mat_a));
    printf("max(B)=%d\n", mat_max(g_mat_b));
    printf("min(B)=%d\n", mat_min(g_mat_b));

    printf("=== Flatten Matrix ===\n");
    printf("flat(A):");
    i = 0;
    while (i < 9) {
        printf(" %d", g_mat_a[i]);
        i = i + 1;
    }
    printf("\n");

    printf("=== Matrix Negate ===\n");
    mat_negate(g_mat_a, g_neg);
    mat_print_labeled(g_neg, 'N');

    printf("=== Row Swap ===\n");
    mat_init(swap_mat, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    printf("before swap: row0=%d,%d,%d row2=%d,%d,%d\n",
        mat_get(swap_mat, 0, 0), mat_get(swap_mat, 0, 1), mat_get(swap_mat, 0, 2),
        mat_get(swap_mat, 2, 0), mat_get(swap_mat, 2, 1), mat_get(swap_mat, 2, 2));
    mat_row_swap(swap_mat, 0, 2);
    printf("after swap: row0=%d,%d,%d row2=%d,%d,%d\n",
        mat_get(swap_mat, 0, 0), mat_get(swap_mat, 0, 1), mat_get(swap_mat, 0, 2),
        mat_get(swap_mat, 2, 0), mat_get(swap_mat, 2, 1), mat_get(swap_mat, 2, 2));

    printf("=== Column Extract ===\n");
    mat_col_extract(g_mat_a, 0, col_out);
    printf("col0 of A: %d %d %d\n", col_out[0], col_out[1], col_out[2]);
    mat_col_extract(g_mat_a, 1, col_out);
    printf("col1 of A: %d %d %d\n", col_out[0], col_out[1], col_out[2]);
    mat_col_extract(g_mat_a, 2, col_out);
    printf("col2 of A: %d %d %d\n", col_out[0], col_out[1], col_out[2]);

    printf("=== Dot Product ===\n");
    v1[0] = 1; v1[1] = 2; v1[2] = 3;
    v2[0] = 4; v2[1] = 5; v2[2] = 6;
    printf("dot([1,2,3],[4,5,6])=%d\n", vec_dot(v1, v2));
    v1[0] = 1; v1[1] = 0; v1[2] = 0;
    v2[0] = 0; v2[1] = 1; v2[2] = 0;
    printf("dot([1,0,0],[0,1,0])=%d\n", vec_dot(v1, v2));

    printf("=== Cross Product ===\n");
    v1[0] = 1; v1[1] = 0; v1[2] = 0;
    v2[0] = 0; v2[1] = 1; v2[2] = 0;
    vec_cross(v1, v2, cross_out);
    printf("cross([1,0,0],[0,1,0])=(%d,%d,%d)\n",
        cross_out[0], cross_out[1], cross_out[2]);
    v1[0] = 1; v1[1] = 2; v1[2] = 3;
    v2[0] = 4; v2[1] = 5; v2[2] = 6;
    vec_cross(v1, v2, cross_out);
    printf("cross([1,2,3],[4,5,6])=(%d,%d,%d)\n",
        cross_out[0], cross_out[1], cross_out[2]);

    printf("=== Done ===\n");

    return 0;
}
