int printf(const char *fmt, ...);
// EXPECT: NQ4:sol=2 first:1 3 0 2 NQ5:sol=10 first:0 2 4 1 3 NQ6:sol=4 first:1 3 5 0 2 4 NQ1:sol=1 NQ2:sol=0 NQ3:sol=0 SS1:t11=1 sub:3 7 1 SS2:t6=1 sub:3 1 2 SS3:t100=0 SS4:t21=1 SS5:t0=1 P3:cnt=6 [123] [132] [213] [231] [312] [321] P4:cnt=24 P1:cnt=1 C42:cnt=6 [12] [13] [14] [23] [24] [34] C53:cnt=10 [123] [124] [125] [134] [135] [145] [234] [235] [245] [345] C51:cnt=5 C55:cnt=1 SS6:t15=1 sub:1 2 3 4 5 DONE

/* =========================================================================
 * Test 056: Backtracking Algorithms
 * =========================================================================
 * N-Queens (N=4,5,6), subset sum, permutation generation,
 * combination generation.
 * ========================================================================= */

/* ---- N-Queens ---- */

int queen_col[10];
int nq_solutions;

int nq_safe(int row, int col, int n) {
    int i;
    for (i = 0; i < row; i = i + 1) {
        if (queen_col[i] == col) {
            return 0;
        }
        if (queen_col[i] - i == col - row) {
            return 0;
        }
        if (queen_col[i] + i == col + row) {
            return 0;
        }
    }
    return 1;
}

void nq_solve(int row, int n) {
    int col;
    if (row == n) {
        nq_solutions = nq_solutions + 1;
        return;
    }
    for (col = 0; col < n; col = col + 1) {
        if (nq_safe(row, col, n)) {
            queen_col[row] = col;
            nq_solve(row + 1, n);
        }
    }
}

/* Print first solution found for N-Queens */
int nq_first_found;
int nq_first_sol[10];

void nq_solve_first(int row, int n) {
    int col;
    if (nq_first_found) {
        return;
    }
    if (row == n) {
        nq_first_found = 1;
        {
            int k;
            for (k = 0; k < n; k = k + 1) {
                nq_first_sol[k] = queen_col[k];
            }
        }
        return;
    }
    for (col = 0; col < n; col = col + 1) {
        if (nq_safe(row, col, n)) {
            queen_col[row] = col;
            nq_solve_first(row + 1, n);
            if (nq_first_found) {
                return;
            }
        }
    }
}

/* ---- Subset Sum ---- */

int ss_set[20];
int ss_included[20];
int ss_found;
int ss_set_size;

void ss_solve(int idx, int current_sum, int target) {
    int i;
    if (ss_found) {
        return;
    }
    if (current_sum == target) {
        ss_found = 1;
        return;
    }
    if (idx >= ss_set_size) {
        return;
    }
    if (current_sum > target) {
        return;
    }
    /* Include current element */
    ss_included[idx] = 1;
    ss_solve(idx + 1, current_sum + ss_set[idx], target);
    if (ss_found) {
        return;
    }
    /* Exclude current element */
    ss_included[idx] = 0;
    ss_solve(idx + 1, current_sum, target);
}

/* ---- Permutations ---- */

int perm_arr[8];
int perm_used[8];
int perm_result[8];
int perm_count;
int perm_n;

/* Store up to 24 permutations (for n<=4) */
int perm_store[24][8];
int perm_stored;
int perm_max_store;

void perm_generate(int depth) {
    int i;
    int j;
    if (depth == perm_n) {
        perm_count = perm_count + 1;
        if (perm_stored < perm_max_store) {
            for (j = 0; j < perm_n; j = j + 1) {
                perm_store[perm_stored][j] = perm_result[j];
            }
            perm_stored = perm_stored + 1;
        }
        return;
    }
    for (i = 0; i < perm_n; i = i + 1) {
        if (perm_used[i] == 0) {
            perm_used[i] = 1;
            perm_result[depth] = perm_arr[i];
            perm_generate(depth + 1);
            perm_used[i] = 0;
        }
    }
}

/* ---- Combinations ---- */

int comb_arr[10];
int comb_result[10];
int comb_count;
int comb_n;
int comb_k;

int comb_store[20][10];
int comb_stored;
int comb_max_store;

void comb_generate(int start, int depth) {
    int i;
    int j;
    if (depth == comb_k) {
        comb_count = comb_count + 1;
        if (comb_stored < comb_max_store) {
            for (j = 0; j < comb_k; j = j + 1) {
                comb_store[comb_stored][j] = comb_result[j];
            }
            comb_stored = comb_stored + 1;
        }
        return;
    }
    for (i = start; i < comb_n; i = i + 1) {
        comb_result[depth] = comb_arr[i];
        comb_generate(i + 1, depth + 1);
    }
}

/* =========================================================================
 * Test functions
 * ========================================================================= */

void test_nqueens_4(void) {
    int i;

    printf("NQ4:");
    nq_solutions = 0;
    nq_solve(0, 4);
    printf("sol=%d", nq_solutions);

    nq_first_found = 0;
    nq_solve_first(0, 4);
    printf(" first:");
    for (i = 0; i < 4; i = i + 1) {
        printf("%d ", nq_first_sol[i]);
    }
}

void test_nqueens_5(void) {
    printf("NQ5:");
    nq_solutions = 0;
    nq_solve(0, 5);
    printf("sol=%d", nq_solutions);

    nq_first_found = 0;
    nq_solve_first(0, 5);
    printf(" first:");
    {
        int i;
        for (i = 0; i < 5; i = i + 1) {
            printf("%d ", nq_first_sol[i]);
        }
    }
}

void test_nqueens_6(void) {
    printf("NQ6:");
    nq_solutions = 0;
    nq_solve(0, 6);
    printf("sol=%d", nq_solutions);

    nq_first_found = 0;
    nq_solve_first(0, 6);
    printf(" first:");
    {
        int i;
        for (i = 0; i < 6; i = i + 1) {
            printf("%d ", nq_first_sol[i]);
        }
    }
}

void test_nqueens_1(void) {
    printf("NQ1:");
    nq_solutions = 0;
    nq_solve(0, 1);
    printf("sol=%d ", nq_solutions);
}

void test_nqueens_2(void) {
    printf("NQ2:");
    nq_solutions = 0;
    nq_solve(0, 2);
    printf("sol=%d ", nq_solutions);
}

void test_nqueens_3(void) {
    printf("NQ3:");
    nq_solutions = 0;
    nq_solve(0, 3);
    printf("sol=%d ", nq_solutions);
}

void test_subset_sum_1(void) {
    int i;
    printf("SS1:");

    ss_set[0] = 3; ss_set[1] = 7; ss_set[2] = 1;
    ss_set[3] = 8; ss_set[4] = 2;
    ss_set_size = 5;

    for (i = 0; i < 5; i = i + 1) {
        ss_included[i] = 0;
    }
    ss_found = 0;
    ss_solve(0, 0, 11);
    printf("t11=%d", ss_found);

    if (ss_found) {
        printf(" sub:");
        for (i = 0; i < 5; i = i + 1) {
            if (ss_included[i]) {
                printf("%d ", ss_set[i]);
            }
        }
    }
}

void test_subset_sum_2(void) {
    int i;
    printf("SS2:");

    for (i = 0; i < 5; i = i + 1) {
        ss_included[i] = 0;
    }
    ss_found = 0;
    ss_solve(0, 0, 6);
    printf("t6=%d", ss_found);

    if (ss_found) {
        printf(" sub:");
        for (i = 0; i < 5; i = i + 1) {
            if (ss_included[i]) {
                printf("%d ", ss_set[i]);
            }
        }
    }
}

void test_subset_sum_3(void) {
    printf("SS3:");

    ss_found = 0;
    {
        int i;
        for (i = 0; i < 5; i = i + 1) {
            ss_included[i] = 0;
        }
    }
    ss_solve(0, 0, 100);
    printf("t100=%d ", ss_found);
}

void test_subset_sum_exact(void) {
    printf("SS4:");

    ss_found = 0;
    {
        int i;
        for (i = 0; i < 5; i = i + 1) {
            ss_included[i] = 0;
        }
    }
    ss_solve(0, 0, 21);
    printf("t21=%d ", ss_found);
}

void test_subset_sum_zero(void) {
    printf("SS5:");

    ss_found = 0;
    {
        int i;
        for (i = 0; i < 5; i = i + 1) {
            ss_included[i] = 0;
        }
    }
    ss_solve(0, 0, 0);
    printf("t0=%d ", ss_found);
}

void test_perms_3(void) {
    int i;
    int j;

    printf("P3:");
    perm_arr[0] = 1; perm_arr[1] = 2; perm_arr[2] = 3;
    perm_n = 3;
    perm_count = 0;
    perm_stored = 0;
    perm_max_store = 6;
    {
        int k;
        for (k = 0; k < 3; k = k + 1) {
            perm_used[k] = 0;
        }
    }

    perm_generate(0);
    printf("cnt=%d", perm_count);

    for (i = 0; i < perm_stored; i = i + 1) {
        printf(" [");
        for (j = 0; j < 3; j = j + 1) {
            printf("%d", perm_store[i][j]);
        }
        printf("]");
    }
    printf(" ");
}

void test_perms_4(void) {
    printf("P4:");
    perm_arr[0] = 1; perm_arr[1] = 2; perm_arr[2] = 3; perm_arr[3] = 4;
    perm_n = 4;
    perm_count = 0;
    perm_stored = 0;
    perm_max_store = 0;
    {
        int k;
        for (k = 0; k < 4; k = k + 1) {
            perm_used[k] = 0;
        }
    }

    perm_generate(0);
    printf("cnt=%d ", perm_count);
}

void test_perms_1(void) {
    printf("P1:");
    perm_arr[0] = 5;
    perm_n = 1;
    perm_count = 0;
    perm_stored = 0;
    perm_max_store = 1;
    perm_used[0] = 0;

    perm_generate(0);
    printf("cnt=%d ", perm_count);
}

void test_comb_4_2(void) {
    int i;
    int j;

    printf("C42:");
    comb_arr[0] = 1; comb_arr[1] = 2; comb_arr[2] = 3; comb_arr[3] = 4;
    comb_n = 4;
    comb_k = 2;
    comb_count = 0;
    comb_stored = 0;
    comb_max_store = 10;

    comb_generate(0, 0);
    printf("cnt=%d", comb_count);

    for (i = 0; i < comb_stored; i = i + 1) {
        printf(" [");
        for (j = 0; j < 2; j = j + 1) {
            printf("%d", comb_store[i][j]);
        }
        printf("]");
    }
    printf(" ");
}

void test_comb_5_3(void) {
    printf("C53:");
    comb_arr[0] = 1; comb_arr[1] = 2; comb_arr[2] = 3;
    comb_arr[3] = 4; comb_arr[4] = 5;
    comb_n = 5;
    comb_k = 3;
    comb_count = 0;
    comb_stored = 0;
    comb_max_store = 20;

    comb_generate(0, 0);
    printf("cnt=%d", comb_count);

    {
        int i;
        int j;
        for (i = 0; i < comb_stored; i = i + 1) {
            printf(" [");
            for (j = 0; j < 3; j = j + 1) {
                printf("%d", comb_store[i][j]);
            }
            printf("]");
        }
    }
    printf(" ");
}

void test_comb_5_1(void) {
    printf("C51:");
    comb_arr[0] = 1; comb_arr[1] = 2; comb_arr[2] = 3;
    comb_arr[3] = 4; comb_arr[4] = 5;
    comb_n = 5;
    comb_k = 1;
    comb_count = 0;
    comb_stored = 0;
    comb_max_store = 0;

    comb_generate(0, 0);
    printf("cnt=%d ", comb_count);
}

void test_comb_5_5(void) {
    printf("C55:");
    comb_n = 5;
    comb_k = 5;
    comb_count = 0;
    comb_stored = 0;
    comb_max_store = 0;

    comb_generate(0, 0);
    printf("cnt=%d ", comb_count);
}

void test_subset_sum_larger(void) {
    int i;
    printf("SS6:");

    ss_set[0] = 1; ss_set[1] = 2; ss_set[2] = 3;
    ss_set[3] = 4; ss_set[4] = 5; ss_set[5] = 6;
    ss_set[6] = 7;
    ss_set_size = 7;

    for (i = 0; i < 7; i = i + 1) {
        ss_included[i] = 0;
    }
    ss_found = 0;
    ss_solve(0, 0, 15);
    printf("t15=%d", ss_found);

    if (ss_found) {
        printf(" sub:");
        for (i = 0; i < 7; i = i + 1) {
            if (ss_included[i]) {
                printf("%d ", ss_set[i]);
            }
        }
    }
    printf("DONE\n");
}

int main(void) {
    test_nqueens_4();
    test_nqueens_5();
    test_nqueens_6();
    test_nqueens_1();
    test_nqueens_2();
    test_nqueens_3();
    test_subset_sum_1();
    test_subset_sum_2();
    test_subset_sum_3();
    test_subset_sum_exact();
    test_subset_sum_zero();
    test_perms_3();
    test_perms_4();
    test_perms_1();
    test_comb_4_2();
    test_comb_5_3();
    test_comb_5_1();
    test_comb_5_5();
    test_subset_sum_larger();
    return 0;
}
