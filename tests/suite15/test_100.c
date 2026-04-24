int printf(const char *fmt, ...);

/* Maximum Bipartite Matching
   Uses augmenting paths (Hungarian-style / Hopcroft-Karp simplified).
   Left nodes [0..n-1], Right nodes [0..m-1].
   Find maximum matching. */

int adj_mat[8][8];  /* adjacency matrix: adj_mat[left][right] */
int match_l[8];     /* match_l[i] = right node matched to left i, or -1 */
int match_r[8];     /* match_r[j] = left node matched to right j, or -1 */
int visited[8];     /* used in DFS */
int n_left;
int n_right;

/* Try to find augmenting path from left node u */
int augment(int u) {
    int v;
    for (v = 0; v < n_right; v++) {
        if (adj_mat[u][v] && !visited[v]) {
            visited[v] = 1;
            /* If right node v is free, or we can find alt path from match_r[v] */
            if (match_r[v] == -1 || augment(match_r[v])) {
                match_l[u] = v;
                match_r[v] = u;
                return 1;
            }
        }
    }
    return 0;
}

int max_matching(void) {
    int i;
    int j;
    int result = 0;

    for (i = 0; i < n_left; i++) match_l[i] = -1;
    for (j = 0; j < n_right; j++) match_r[j] = -1;

    for (i = 0; i < n_left; i++) {
        for (j = 0; j < n_right; j++) visited[j] = 0;
        if (augment(i)) {
            result = result + 1;
        }
    }
    return result;
}

void clear_adj(void) {
    int i;
    int j;
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
            adj_mat[i][j] = 0;
}

/* Minimum vertex cover = max matching (Konig's theorem) */
int min_vertex_cover(void) {
    return max_matching();
}

/* Maximum independent set = n - max matching (for bipartite) */
int max_independent_set(void) {
    return n_left + n_right - max_matching();
}

void test_perfect_matching(void) {
    int m;

    printf("=== Perfect Matching ===\n");
    // EXPECT: === Perfect Matching ===

    /* 3x3: L0-R0, L1-R1, L2-R2 all connected (identity) */
    n_left = 3;
    n_right = 3;
    clear_adj();
    adj_mat[0][0] = 1;
    adj_mat[1][1] = 1;
    adj_mat[2][2] = 1;

    m = max_matching();
    printf("Matching: %d\n", m);
    // EXPECT: Matching: 3
}

void test_complete_bipartite(void) {
    int m;
    int i;
    int j;

    printf("=== Complete Bipartite K3,3 ===\n");
    // EXPECT: === Complete Bipartite K3,3 ===

    n_left = 3;
    n_right = 3;
    clear_adj();
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            adj_mat[i][j] = 1;

    m = max_matching();
    printf("Matching: %d\n", m);
    // EXPECT: Matching: 3
}

void test_unbalanced(void) {
    int m;

    printf("=== Unbalanced 4x2 ===\n");
    // EXPECT: === Unbalanced 4x2 ===

    /* 4 left, 2 right. Max matching = 2 */
    n_left = 4;
    n_right = 2;
    clear_adj();
    adj_mat[0][0] = 1;
    adj_mat[1][0] = 1;
    adj_mat[1][1] = 1;
    adj_mat[2][1] = 1;
    adj_mat[3][0] = 1;
    adj_mat[3][1] = 1;

    m = max_matching();
    printf("Matching: %d\n", m);
    // EXPECT: Matching: 2
}

void test_augmenting_path(void) {
    int m;

    printf("=== Augmenting Path Needed ===\n");
    // EXPECT: === Augmenting Path Needed ===

    /* L0->R0, L1->R0,R1
       If L0 matches R0 first, L1 needs augmenting path.
       L1 tries R0 (taken by L0). Can L0 rematch to something else? No.
       L1 tries R1 -> matches.
       Result: L0-R0, L1-R1. Matching = 2. */
    n_left = 2;
    n_right = 2;
    clear_adj();
    adj_mat[0][0] = 1;
    adj_mat[1][0] = 1;
    adj_mat[1][1] = 1;

    m = max_matching();
    printf("Matching: %d\n", m);
    // EXPECT: Matching: 2

    printf("L0->R%d\n", match_l[0]);
    // EXPECT: L0->R0
    printf("L1->R%d\n", match_l[1]);
    // EXPECT: L1->R1
}

void test_no_matching(void) {
    int m;

    printf("=== No Edges ===\n");
    // EXPECT: === No Edges ===

    n_left = 3;
    n_right = 3;
    clear_adj();

    m = max_matching();
    printf("Matching: %d\n", m);
    // EXPECT: Matching: 0
}

void test_complex(void) {
    int m;

    printf("=== Complex Matching ===\n");
    // EXPECT: === Complex Matching ===

    /* 5 left, 5 right
       L0: R0, R1
       L1: R0, R2
       L2: R1, R2, R3
       L3: R3, R4
       L4: R4

       Possible perfect matching? Let's trace:
       L0 matches R0.
       L1 tries R0 (taken). Augment: L0 can match R1. So L0->R1, L1->R0.
       L2 tries R1 (taken by L0). Augment: L0 can match... L0 only has R0,R1.
         Try R0: taken by L1. L1 can match R2. So L1->R2, L0->R0, L2->R1.
       L3 tries R3. Matches.
       L4 tries R4. Matches.
       Total: 5.
    */
    n_left = 5;
    n_right = 5;
    clear_adj();
    adj_mat[0][0] = 1; adj_mat[0][1] = 1;
    adj_mat[1][0] = 1; adj_mat[1][2] = 1;
    adj_mat[2][1] = 1; adj_mat[2][2] = 1; adj_mat[2][3] = 1;
    adj_mat[3][3] = 1; adj_mat[3][4] = 1;
    adj_mat[4][4] = 1;

    m = max_matching();
    printf("Matching: %d\n", m);
    // EXPECT: Matching: 5

    /* Konig's theorem: min vertex cover = max matching = 5 */
    printf("Min vertex cover: %d\n", m);
    // EXPECT: Min vertex cover: 5

    /* Max independent set = 10 - 5 = 5 */
    printf("Max independent set: %d\n", max_independent_set());
    // EXPECT: Max independent set: 5
}

void test_konig(void) {
    int m;

    printf("=== Konig's Theorem ===\n");
    // EXPECT: === Konig's Theorem ===

    /* L0: R0, R1
       L1: R1, R2
       L2: R2
       Matching: L0-R0, L1-R1, L2-R2 = 3
       Min vertex cover = 3
       Independent set = 6 - 3 = 3
    */
    n_left = 3;
    n_right = 3;
    clear_adj();
    adj_mat[0][0] = 1; adj_mat[0][1] = 1;
    adj_mat[1][1] = 1; adj_mat[1][2] = 1;
    adj_mat[2][2] = 1;

    m = max_matching();
    printf("Max matching: %d\n", m);
    // EXPECT: Max matching: 3
    printf("Min vertex cover: %d\n", m);
    // EXPECT: Min vertex cover: 3

    n_left = 3;
    n_right = 3;
    printf("Max indep set: %d\n", max_independent_set());
    // EXPECT: Max indep set: 3
}

void test_job_assignment(void) {
    int m;

    printf("=== Job Assignment ===\n");
    // EXPECT: === Job Assignment ===

    /* 4 workers, 4 jobs. Each worker can do some jobs.
       W0: J0, J1
       W1: J1, J2
       W2: J0, J2, J3
       W3: J3

       Trace:
       W0 matches J0.
       W1 tries J1. Matches.
       W2 tries J0 (taken by W0). Augment: W0 tries J1 (taken by W1).
         W1 tries J2. Matches. So W1->J2, W0->J1, W2->J0.
       W3 tries J3. Matches.
       Total: 4.
    */
    n_left = 4;
    n_right = 4;
    clear_adj();
    adj_mat[0][0] = 1; adj_mat[0][1] = 1;
    adj_mat[1][1] = 1; adj_mat[1][2] = 1;
    adj_mat[2][0] = 1; adj_mat[2][2] = 1; adj_mat[2][3] = 1;
    adj_mat[3][3] = 1;

    m = max_matching();
    printf("Jobs assigned: %d\n", m);
    // EXPECT: Jobs assigned: 4

    printf("W0->J%d\n", match_l[0]);
    // EXPECT: W0->J1
    printf("W1->J%d\n", match_l[1]);
    // EXPECT: W1->J2
    printf("W2->J%d\n", match_l[2]);
    // EXPECT: W2->J0
    printf("W3->J%d\n", match_l[3]);
    // EXPECT: W3->J3
}

int main(void) {
    test_perfect_matching();
    test_complete_bipartite();
    test_unbalanced();
    test_augmenting_path();
    test_no_matching();
    test_complex();
    test_konig();
    test_job_assignment();

    printf("All bipartite matching tests passed!\n");
    // EXPECT: All bipartite matching tests passed!
    return 0;
}
