int printf(const char *fmt, ...);
// EXPECT: A=(0,0,1),(0,2,2),(1,1,3),(2,0,4),(2,2,5) NNZ_A=5 G1=1 G2=0 G3=3 G4=4 G5=5 T=(0,0,1),(0,2,4),(1,1,3),(2,0,2),(2,2,5) NNZ_T=5 R0=2 R1=1 R2=2 C0=2 C1=1 C2=2 SUM=15 TR=9 FR=55 MA=5 SYM=0 B=(0,1,1),(1,0,2),(1,2,3),(2,1,4) APB=(0,0,1),(0,1,1),(0,2,2),(1,0,2),(1,1,3),(1,2,3),(2,0,4),(2,1,4),(2,2,5) NNZ_APB=9 AMB=(0,0,1),(0,1,-1),(0,2,2),(1,0,-2),(1,1,3),(1,2,-3),(2,0,4),(2,1,-4),(2,2,5) NNZ_AMB=9 SC2=(0,0,2),(0,2,4),(1,1,6),(2,0,8),(2,2,10) SC0NNZ=0 AMB2=(0,1,9),(1,0,6),(1,2,9),(2,1,24) NNZ_MUL=4 SYM2=1 TR2=4 TR_I=3 SYM_I=1 FR_I=3 
/* ====================================================================== */
/* Test 047: Sparse Matrix Operations                                     */
/* Sparse matrix using arrays of (row,col,val) triples                    */
/* Addition, transpose, multiplication (small)                            */
/* ====================================================================== */

/* Sparse matrix representation:
   rows[i], cols[i], vals[i] for i = 0..nnz-1
   nrows, ncols = dimensions
*/

/* Print sparse matrix entries */
void print_sparse(int *rows, int *cols, int *vals, int nnz) {
    int i;
    i = 0;
    while (i < nnz) {
        printf("(%d,%d,%d)", rows[i], cols[i], vals[i]);
        if (i < nnz - 1) printf(",");
        i = i + 1;
    }
}

/* Get value at (r,c) from sparse matrix */
int sparse_get(int *rows, int *cols, int *vals, int nnz, int r, int c) {
    int i;
    i = 0;
    while (i < nnz) {
        if (rows[i] == r && cols[i] == c) return vals[i];
        i = i + 1;
    }
    return 0;
}

/* Transpose sparse matrix */
int sparse_transpose(int *rows, int *cols, int *vals, int nnz,
                     int *trows, int *tcols, int *tvals) {
    int i;
    i = 0;
    while (i < nnz) {
        trows[i] = cols[i];
        tcols[i] = rows[i];
        tvals[i] = vals[i];
        i = i + 1;
    }
    return nnz;
}

/* Sort sparse matrix by (row, col) using bubble sort */
void sparse_sort(int *rows, int *cols, int *vals, int nnz) {
    int i;
    int j;
    int tr;
    int tc;
    int tv;

    i = 0;
    while (i < nnz - 1) {
        j = 0;
        while (j < nnz - 1 - i) {
            if (rows[j] > rows[j + 1] ||
                (rows[j] == rows[j + 1] && cols[j] > cols[j + 1])) {
                tr = rows[j]; rows[j] = rows[j+1]; rows[j+1] = tr;
                tc = cols[j]; cols[j] = cols[j+1]; cols[j+1] = tc;
                tv = vals[j]; vals[j] = vals[j+1]; vals[j+1] = tv;
            }
            j = j + 1;
        }
        i = i + 1;
    }
}

/* Add two sparse matrices: C = A + B */
/* Both matrices must have same dimensions */
/* Returns number of non-zero entries in result */
int sparse_add(int *ar, int *ac, int *av, int annz,
               int *br, int *bc, int *bv, int bnnz,
               int *cr, int *cc, int *cv) {
    int cnnz;
    int i;
    int j;
    int found;
    int val;

    /* Copy A entries to C */
    cnnz = 0;
    i = 0;
    while (i < annz) {
        cr[cnnz] = ar[i];
        cc[cnnz] = ac[i];
        cv[cnnz] = av[i];
        cnnz = cnnz + 1;
        i = i + 1;
    }

    /* Add B entries */
    i = 0;
    while (i < bnnz) {
        /* Check if (br[i], bc[i]) already in C */
        found = -1;
        j = 0;
        while (j < cnnz) {
            if (cr[j] == br[i] && cc[j] == bc[i]) {
                found = j;
                j = cnnz; /* break */
            }
            j = j + 1;
        }

        if (found >= 0) {
            cv[found] = cv[found] + bv[i];
        } else {
            cr[cnnz] = br[i];
            cc[cnnz] = bc[i];
            cv[cnnz] = bv[i];
            cnnz = cnnz + 1;
        }
        i = i + 1;
    }

    /* Remove zeros */
    i = 0;
    j = 0;
    while (i < cnnz) {
        if (cv[i] != 0) {
            cr[j] = cr[i];
            cc[j] = cc[i];
            cv[j] = cv[i];
            j = j + 1;
        }
        i = i + 1;
    }
    cnnz = j;

    sparse_sort(cr, cc, cv, cnnz);
    return cnnz;
}

/* Multiply two sparse matrices: C = A * B */
/* A is m x k, B is k x n, C is m x n */
/* Returns nnz of result */
int sparse_multiply(int *ar, int *ac, int *av, int annz,
                    int *br, int *bc, int *bv, int bnnz,
                    int *cr, int *cc, int *cv, int m, int n) {
    int cnnz;
    int i;
    int j;
    int r;
    int c;
    int val;
    int found;
    int k;

    cnnz = 0;

    /* For each entry in A */
    i = 0;
    while (i < annz) {
        /* For each entry in B where B's row == A's col */
        j = 0;
        while (j < bnnz) {
            if (ac[i] == br[j]) {
                r = ar[i];
                c = bc[j];
                val = av[i] * bv[j];

                /* Find if (r,c) already in C */
                found = -1;
                k = 0;
                while (k < cnnz) {
                    if (cr[k] == r && cc[k] == c) {
                        found = k;
                        k = cnnz;
                    }
                    k = k + 1;
                }

                if (found >= 0) {
                    cv[found] = cv[found] + val;
                } else {
                    cr[cnnz] = r;
                    cc[cnnz] = c;
                    cv[cnnz] = val;
                    cnnz = cnnz + 1;
                }
            }
            j = j + 1;
        }
        i = i + 1;
    }

    /* Remove zeros */
    i = 0;
    j = 0;
    while (i < cnnz) {
        if (cv[i] != 0) {
            cr[j] = cr[i];
            cc[j] = cc[i];
            cv[j] = cv[i];
            j = j + 1;
        }
        i = i + 1;
    }
    cnnz = j;

    sparse_sort(cr, cc, cv, cnnz);
    return cnnz;
}

/* Scale sparse matrix by constant */
int sparse_scale(int *rows, int *cols, int *vals, int nnz, int scalar,
                 int *rr, int *rc, int *rv) {
    int i;
    int cnt;

    cnt = 0;
    i = 0;
    while (i < nnz) {
        if (vals[i] * scalar != 0) {
            rr[cnt] = rows[i];
            rc[cnt] = cols[i];
            rv[cnt] = vals[i] * scalar;
            cnt = cnt + 1;
        }
        i = i + 1;
    }
    return cnt;
}

/* Count non-zeros in given row */
int count_row_nnz(int *rows, int nnz, int r) {
    int count;
    int i;
    count = 0;
    i = 0;
    while (i < nnz) {
        if (rows[i] == r) count = count + 1;
        i = i + 1;
    }
    return count;
}

/* Count non-zeros in given column */
int count_col_nnz(int *cols, int nnz, int c) {
    int count;
    int i;
    count = 0;
    i = 0;
    while (i < nnz) {
        if (cols[i] == c) count = count + 1;
        i = i + 1;
    }
    return count;
}

/* Sum of all values */
int sparse_sum(int *vals, int nnz) {
    int sum;
    int i;
    sum = 0;
    i = 0;
    while (i < nnz) {
        sum = sum + vals[i];
        i = i + 1;
    }
    return sum;
}

/* Trace of sparse matrix (sum of diagonal elements) */
int sparse_trace(int *rows, int *cols, int *vals, int nnz) {
    int trace;
    int i;
    trace = 0;
    i = 0;
    while (i < nnz) {
        if (rows[i] == cols[i]) trace = trace + vals[i];
        i = i + 1;
    }
    return trace;
}

/* Check if sparse matrix is symmetric */
int sparse_is_symmetric(int *rows, int *cols, int *vals, int nnz) {
    int i;
    int val_t;
    i = 0;
    while (i < nnz) {
        val_t = sparse_get(rows, cols, vals, nnz, cols[i], rows[i]);
        if (val_t != vals[i]) return 0;
        i = i + 1;
    }
    return 1;
}

/* Frobenius norm squared (sum of squares of all entries) */
int sparse_frobenius_sq(int *vals, int nnz) {
    int sum;
    int i;
    sum = 0;
    i = 0;
    while (i < nnz) {
        sum = sum + vals[i] * vals[i];
        i = i + 1;
    }
    return sum;
}

/* Max absolute value in sparse matrix */
int sparse_max_abs(int *vals, int nnz) {
    int maxv;
    int i;
    int av;

    if (nnz == 0) return 0;
    maxv = vals[0];
    if (maxv < 0) maxv = -maxv;

    i = 1;
    while (i < nnz) {
        av = vals[i];
        if (av < 0) av = -av;
        if (av > maxv) maxv = av;
        i = i + 1;
    }
    return maxv;
}

/* Subtract sparse matrices: C = A - B */
int sparse_sub(int *ar, int *ac, int *av, int annz,
               int *br, int *bc, int *bv, int bnnz,
               int *cr, int *cc, int *cv) {
    /* Negate B, then add */
    int nb[50];
    int i;

    i = 0;
    while (i < bnnz) {
        nb[i] = -bv[i];
        i = i + 1;
    }
    return sparse_add(ar, ac, av, annz, br, bc, nb, bnnz, cr, cc, cv);
}

int main(void) {
    /* Matrix A (3x3): */
    /* [ 1 0 2 ] */
    /* [ 0 3 0 ] */
    /* [ 4 0 5 ] */
    int ar[10]; int ac_arr[10]; int av[10]; int annz;
    int br[10]; int bc_arr[10]; int bv[10]; int bnnz;
    int cr[50]; int cc_arr[50]; int cv[50]; int cnnz;
    int tr[10]; int tc[10]; int tv[10]; int tnnz;
    int sr[10]; int sc[10]; int sv[10]; int snnz;

    ar[0]=0; ac_arr[0]=0; av[0]=1;
    ar[1]=0; ac_arr[1]=2; av[1]=2;
    ar[2]=1; ac_arr[2]=1; av[2]=3;
    ar[3]=2; ac_arr[3]=0; av[3]=4;
    ar[4]=2; ac_arr[4]=2; av[4]=5;
    annz = 5;

    printf("A="); print_sparse(ar, ac_arr, av, annz); printf(" ");
    printf("NNZ_A=%d ", annz);

    /* Get specific values */
    printf("G1=%d ", sparse_get(ar, ac_arr, av, annz, 0, 0));
    printf("G2=%d ", sparse_get(ar, ac_arr, av, annz, 0, 1));
    printf("G3=%d ", sparse_get(ar, ac_arr, av, annz, 1, 1));
    printf("G4=%d ", sparse_get(ar, ac_arr, av, annz, 2, 0));
    printf("G5=%d ", sparse_get(ar, ac_arr, av, annz, 2, 2));

    /* Transpose */
    tnnz = sparse_transpose(ar, ac_arr, av, annz, tr, tc, tv);
    sparse_sort(tr, tc, tv, tnnz);
    printf("T="); print_sparse(tr, tc, tv, tnnz); printf(" ");
    printf("NNZ_T=%d ", tnnz);

    /* Row/column counts */
    printf("R0=%d ", count_row_nnz(ar, annz, 0));
    printf("R1=%d ", count_row_nnz(ar, annz, 1));
    printf("R2=%d ", count_row_nnz(ar, annz, 2));
    printf("C0=%d ", count_col_nnz(ac_arr, annz, 0));
    printf("C1=%d ", count_col_nnz(ac_arr, annz, 1));
    printf("C2=%d ", count_col_nnz(ac_arr, annz, 2));

    /* Sum, trace, Frobenius */
    printf("SUM=%d ", sparse_sum(av, annz));
    printf("TR=%d ", sparse_trace(ar, ac_arr, av, annz));
    printf("FR=%d ", sparse_frobenius_sq(av, annz));
    printf("MA=%d ", sparse_max_abs(av, annz));

    /* Symmetry check */
    printf("SYM=%d ", sparse_is_symmetric(ar, ac_arr, av, annz));

    /* Matrix B (3x3): */
    /* [ 0 1 0 ] */
    /* [ 2 0 3 ] */
    /* [ 0 4 0 ] */
    br[0]=0; bc_arr[0]=1; bv[0]=1;
    br[1]=1; bc_arr[1]=0; bv[1]=2;
    br[2]=1; bc_arr[2]=2; bv[2]=3;
    br[3]=2; bc_arr[3]=1; bv[3]=4;
    bnnz = 4;

    printf("B="); print_sparse(br, bc_arr, bv, bnnz); printf(" ");

    /* Add A + B */
    cnnz = sparse_add(ar, ac_arr, av, annz, br, bc_arr, bv, bnnz, cr, cc_arr, cv);
    printf("APB="); print_sparse(cr, cc_arr, cv, cnnz); printf(" ");
    printf("NNZ_APB=%d ", cnnz);

    /* Subtract A - B */
    cnnz = sparse_sub(ar, ac_arr, av, annz, br, bc_arr, bv, bnnz, cr, cc_arr, cv);
    printf("AMB="); print_sparse(cr, cc_arr, cv, cnnz); printf(" ");
    printf("NNZ_AMB=%d ", cnnz);

    /* Scale A by 2 */
    snnz = sparse_scale(ar, ac_arr, av, annz, 2, sr, sc, sv);
    printf("SC2="); print_sparse(sr, sc, sv, snnz); printf(" ");

    /* Scale A by 0 */
    snnz = sparse_scale(ar, ac_arr, av, annz, 0, sr, sc, sv);
    printf("SC0NNZ=%d ", snnz);

    /* Multiply A * B (3x3 * 3x3 = 3x3) */
    cnnz = sparse_multiply(ar, ac_arr, av, annz, br, bc_arr, bv, bnnz,
                           cr, cc_arr, cv, 3, 3);
    printf("AMB2="); print_sparse(cr, cc_arr, cv, cnnz); printf(" ");
    printf("NNZ_MUL=%d ", cnnz);

    /* Symmetric matrix test */
    /* S = [[1,2],[2,3]] */
    ar[0]=0; ac_arr[0]=0; av[0]=1;
    ar[1]=0; ac_arr[1]=1; av[1]=2;
    ar[2]=1; ac_arr[2]=0; av[2]=2;
    ar[3]=1; ac_arr[3]=1; av[3]=3;
    annz = 4;
    printf("SYM2=%d ", sparse_is_symmetric(ar, ac_arr, av, annz));

    /* Trace of symmetric */
    printf("TR2=%d ", sparse_trace(ar, ac_arr, av, annz));

    /* Identity matrix 3x3 */
    ar[0]=0; ac_arr[0]=0; av[0]=1;
    ar[1]=1; ac_arr[1]=1; av[1]=1;
    ar[2]=2; ac_arr[2]=2; av[2]=1;
    annz = 3;
    printf("TR_I=%d ", sparse_trace(ar, ac_arr, av, annz));
    printf("SYM_I=%d ", sparse_is_symmetric(ar, ac_arr, av, annz));
    printf("FR_I=%d ", sparse_frobenius_sq(av, annz));

    printf("\n");
    return 0;
}
