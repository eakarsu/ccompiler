int printf(const char *fmt, ...);

/* Stable Matching - Gale-Shapley Algorithm
   Men propose to women in order of preference.
   Women accept/reject based on their own preferences.
   Result is a stable matching. */

int pref_m[4][4];  /* men's preferences: pref_m[m][rank] = woman */
int pref_w[4][4];  /* women's preferences: pref_w[w][rank] = man */
int rank_w[4][4];  /* rank_w[w][m] = rank of man m for woman w */

int wife[4];        /* wife[m] = partner of man m, -1 if free */
int husband[4];     /* husband[w] = partner of woman w, -1 if free */
int next_proposal[4]; /* next_proposal[m] = next rank to propose */

void init_rankings(void) {
    int w;
    int m;
    int r;
    for (w = 0; w < 4; w++) {
        for (r = 0; r < 4; r++) {
            m = pref_w[w][r];
            rank_w[w][m] = r;
        }
    }
}

int woman_prefers(int w, int m1, int m2) {
    /* returns 1 if woman w prefers m1 over m2 */
    return rank_w[w][m1] < rank_w[w][m2];
}

void gale_shapley(void) {
    int m;
    int w;
    int changed;
    int current;

    for (m = 0; m < 4; m++) {
        wife[m] = -1;
        husband[m] = -1;
        next_proposal[m] = 0;
    }

    changed = 1;
    while (changed) {
        changed = 0;
        for (m = 0; m < 4; m++) {
            if (wife[m] == -1 && next_proposal[m] < 4) {
                w = pref_m[m][next_proposal[m]];
                next_proposal[m] = next_proposal[m] + 1;
                changed = 1;

                if (husband[w] == -1) {
                    wife[m] = w;
                    husband[w] = m;
                } else {
                    current = husband[w];
                    if (woman_prefers(w, m, current)) {
                        wife[current] = -1;
                        wife[m] = w;
                        husband[w] = m;
                    }
                }
            }
        }
    }
}

int is_stable(void) {
    int m;
    int w;
    int i;
    int j;
    for (m = 0; m < 4; m++) {
        w = wife[m];
        /* Check all women m prefers over w */
        for (i = 0; i < 4; i++) {
            if (pref_m[m][i] == w) break;
            /* m prefers pref_m[m][i] over w */
            j = pref_m[m][i];
            /* Does j prefer m over her current partner? */
            if (woman_prefers(j, m, husband[j])) {
                return 0; /* blocking pair found */
            }
        }
    }
    return 1;
}

void test_basic(void) {
    /* Simple preference lists:
       M0: W0 W1 W2 W3
       M1: W1 W0 W2 W3
       M2: W2 W3 W0 W1
       M3: W3 W2 W1 W0

       W0: M0 M1 M2 M3
       W1: M1 M0 M3 M2
       W2: M2 M3 M0 M1
       W3: M3 M2 M1 M0
    */
    pref_m[0][0]=0; pref_m[0][1]=1; pref_m[0][2]=2; pref_m[0][3]=3;
    pref_m[1][0]=1; pref_m[1][1]=0; pref_m[1][2]=2; pref_m[1][3]=3;
    pref_m[2][0]=2; pref_m[2][1]=3; pref_m[2][2]=0; pref_m[2][3]=1;
    pref_m[3][0]=3; pref_m[3][1]=2; pref_m[3][2]=1; pref_m[3][3]=0;

    pref_w[0][0]=0; pref_w[0][1]=1; pref_w[0][2]=2; pref_w[0][3]=3;
    pref_w[1][0]=1; pref_w[1][1]=0; pref_w[1][2]=3; pref_w[1][3]=2;
    pref_w[2][0]=2; pref_w[2][1]=3; pref_w[2][2]=0; pref_w[2][3]=1;
    pref_w[3][0]=3; pref_w[3][1]=2; pref_w[3][2]=1; pref_w[3][3]=0;

    init_rankings();
    gale_shapley();

    printf("=== Basic Stable Matching ===\n");
    // EXPECT: === Basic Stable Matching ===
    /* Each man gets his top choice since preferences align perfectly */
    printf("M0->W%d\n", wife[0]);
    // EXPECT: M0->W0
    printf("M1->W%d\n", wife[1]);
    // EXPECT: M1->W1
    printf("M2->W%d\n", wife[2]);
    // EXPECT: M2->W2
    printf("M3->W%d\n", wife[3]);
    // EXPECT: M3->W3
    printf("Stable: %d\n", is_stable());
    // EXPECT: Stable: 1
}

void test_conflict(void) {
    /* Conflicting preferences:
       M0: W0 W1 W2 W3
       M1: W0 W1 W2 W3
       M2: W0 W1 W2 W3
       M3: W0 W1 W2 W3

       W0: M3 M2 M1 M0
       W1: M3 M2 M1 M0
       W2: M3 M2 M1 M0
       W3: M3 M2 M1 M0
    */
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            pref_m[i][j] = j;         /* all men prefer W0>W1>W2>W3 */
            pref_w[i][j] = 3 - j;     /* all women prefer M3>M2>M1>M0 */
        }
    }

    init_rankings();
    gale_shapley();

    printf("=== Conflict Matching ===\n");
    // EXPECT: === Conflict Matching ===

    /* Gale-Shapley (man-proposing):
       Round 1: All men propose to W0.
         W0 gets proposals from M0,M1,M2,M3. She picks M3 (her favorite).
       Round 2: M0,M1,M2 propose to W1.
         W1 picks M2.
       Round 3: M0,M1 propose to W2.
         W2 picks M1.
       Round 4: M0 proposes to W3.
         W3 accepts M0. */
    printf("M0->W%d\n", wife[0]);
    // EXPECT: M0->W3
    printf("M1->W%d\n", wife[1]);
    // EXPECT: M1->W2
    printf("M2->W%d\n", wife[2]);
    // EXPECT: M2->W1
    printf("M3->W%d\n", wife[3]);
    // EXPECT: M3->W0
    printf("Stable: %d\n", is_stable());
    // EXPECT: Stable: 1
}

void test_rotation(void) {
    /* Cyclic preferences:
       M0: W0 W1 W2 W3
       M1: W1 W2 W3 W0
       M2: W2 W3 W0 W1
       M3: W3 W0 W1 W2

       W0: M1 M2 M3 M0
       W1: M2 M3 M0 M1
       W2: M3 M0 M1 M2
       W3: M0 M1 M2 M3
    */
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            pref_m[i][j] = (i + j) % 4;
            pref_w[i][j] = (i + j + 1) % 4;
        }
    }

    init_rankings();
    gale_shapley();

    printf("=== Rotation Matching ===\n");
    // EXPECT: === Rotation Matching ===
    /* Man-optimal: each man gets his first choice */
    printf("M0->W%d\n", wife[0]);
    // EXPECT: M0->W0
    printf("M1->W%d\n", wife[1]);
    // EXPECT: M1->W1
    printf("M2->W%d\n", wife[2]);
    // EXPECT: M2->W2
    printf("M3->W%d\n", wife[3]);
    // EXPECT: M3->W3
    printf("Stable: %d\n", is_stable());
    // EXPECT: Stable: 1
}

int main(void) {
    test_basic();
    test_conflict();
    test_rotation();

    printf("All stable matching tests passed!\n");
    // EXPECT: All stable matching tests passed!
    return 0;
}
