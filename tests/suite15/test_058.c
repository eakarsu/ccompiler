int printf(const char *fmt, ...);

// Election simulation: comparing voting methods
// Plurality, runoff, and Borda count
// 5 candidates, 100 voters with preference rankings

typedef struct {
    int state;
} RNG;

void rng_init(RNG *r, int seed) {
    r->state = seed % 32749;
}

int rng_next(RNG *r) {
    r->state = (1021 * r->state + 13) % 32749;
    return r->state;
}

// Generate voter preferences (rankings of 5 candidates)
// prefs[voter * 5 + rank] = candidate_id (0-4)
void generate_voters(int *prefs, int num_voters, RNG *rng) {
    int v;
    int i;
    int j;
    for (v = 0; v < num_voters; v++) {
        int perm[5];
        for (i = 0; i < 5; i++) perm[i] = i;
        // Fisher-Yates shuffle
        for (i = 4; i > 0; i--) {
            j = rng_next(rng) % (i + 1);
            int tmp = perm[i];
            perm[i] = perm[j];
            perm[j] = tmp;
        }
        for (i = 0; i < 5; i++) {
            prefs[v * 5 + i] = perm[i];
        }
    }
}

// Plurality: count first-choice votes, highest wins
int plurality_winner(int *prefs, int num_voters) {
    int counts[5];
    int i;
    for (i = 0; i < 5; i++) counts[i] = 0;
    for (i = 0; i < num_voters; i++) {
        int first_choice = prefs[i * 5 + 0];
        counts[first_choice]++;
    }
    int best = 0;
    for (i = 1; i < 5; i++) {
        if (counts[i] > counts[best]) best = i;
    }
    return best;
}

int plurality_votes(int *prefs, int num_voters, int cand) {
    int count = 0;
    int i;
    for (i = 0; i < num_voters; i++) {
        if (prefs[i * 5 + 0] == cand) count++;
    }
    return count;
}

// Runoff: top two from plurality, then head-to-head
int runoff_winner(int *prefs, int num_voters) {
    int counts[5];
    int i;
    for (i = 0; i < 5; i++) counts[i] = 0;
    for (i = 0; i < num_voters; i++) {
        counts[prefs[i * 5 + 0]]++;
    }
    // Find top 2
    int first = 0;
    int second = 1;
    if (counts[second] > counts[first]) {
        int tmp = first;
        first = second;
        second = tmp;
    }
    for (i = 2; i < 5; i++) {
        if (counts[i] > counts[first]) {
            second = first;
            first = i;
        } else if (counts[i] > counts[second]) {
            second = i;
        }
    }

    // Head to head between first and second
    int votes_first = 0;
    int votes_second = 0;
    for (i = 0; i < num_voters; i++) {
        int j;
        for (j = 0; j < 5; j++) {
            if (prefs[i * 5 + j] == first) {
                votes_first++;
                break;
            }
            if (prefs[i * 5 + j] == second) {
                votes_second++;
                break;
            }
        }
    }
    if (votes_first >= votes_second) return first;
    return second;
}

// Borda count: 4 points for 1st, 3 for 2nd, ..., 0 for 5th
int borda_winner(int *prefs, int num_voters) {
    int scores[5];
    int i;
    int j;
    for (i = 0; i < 5; i++) scores[i] = 0;
    for (i = 0; i < num_voters; i++) {
        for (j = 0; j < 5; j++) {
            int cand = prefs[i * 5 + j];
            scores[cand] = scores[cand] + (4 - j);
        }
    }
    int best = 0;
    for (i = 1; i < 5; i++) {
        if (scores[i] > scores[best]) best = i;
    }
    return best;
}

int borda_score(int *prefs, int num_voters, int cand) {
    int score = 0;
    int i;
    int j;
    for (i = 0; i < num_voters; i++) {
        for (j = 0; j < 5; j++) {
            if (prefs[i * 5 + j] == cand) {
                score = score + (4 - j);
            }
        }
    }
    return score;
}

// Count Condorcet wins for a candidate
int condorcet_wins(int *prefs, int num_voters, int cand) {
    int wins = 0;
    int opp;
    for (opp = 0; opp < 5; opp++) {
        if (opp == cand) continue;
        int votes_cand = 0;
        int votes_opp = 0;
        int i;
        int j;
        for (i = 0; i < num_voters; i++) {
            for (j = 0; j < 5; j++) {
                if (prefs[i * 5 + j] == cand) {
                    votes_cand++;
                    break;
                }
                if (prefs[i * 5 + j] == opp) {
                    votes_opp++;
                    break;
                }
            }
        }
        if (votes_cand > votes_opp) wins++;
    }
    return wins;
}

int main(void) {
    RNG rng;
    rng_init(&rng, 42);

    int prefs[500]; // 100 voters x 5 candidates
    generate_voters(prefs, 100, &rng);

    printf("Voter 0: %d %d %d %d %d\n",
           prefs[0], prefs[1], prefs[2], prefs[3], prefs[4]);
    // EXPECT: Voter 0: 0 2 4 3 1

    int pw = plurality_winner(prefs, 100);
    printf("Plurality winner: %d\n", pw);
    // EXPECT: Plurality winner: 1

    int i;
    printf("Plurality votes:");
    for (i = 0; i < 5; i++) {
        printf(" %d", plurality_votes(prefs, 100, i));
    }
    printf("\n");
    // EXPECT: Plurality votes: 16 28 15 19 22

    int rw = runoff_winner(prefs, 100);
    printf("Runoff winner: %d\n", rw);
    // EXPECT: Runoff winner: 4

    int bw = borda_winner(prefs, 100);
    printf("Borda winner: %d\n", bw);
    // EXPECT: Borda winner: 4

    printf("Borda scores:");
    for (i = 0; i < 5; i++) {
        printf(" %d", borda_score(prefs, 100, i));
    }
    printf("\n");
    // EXPECT: Borda scores: 195 208 185 199 213

    printf("Condorcet wins:");
    for (i = 0; i < 5; i++) {
        printf(" %d", condorcet_wins(prefs, 100, i));
    }
    printf("\n");
    // EXPECT: Condorcet wins: 1 2 0 1 4

    int all_agree = (pw == rw && rw == bw) ? 1 : 0;
    printf("All methods agree: %d\n", all_agree);
    // EXPECT: All methods agree: 0

    // Check if Condorcet winner exists (someone beats all others)
    int condorcet_winner = -1;
    for (i = 0; i < 5; i++) {
        if (condorcet_wins(prefs, 100, i) == 4) {
            condorcet_winner = i;
        }
    }
    printf("Condorcet winner: %d\n", condorcet_winner);
    // EXPECT: Condorcet winner: 4

    // Plurality disagrees with Condorcet here
    int plur_agrees_condorcet = (pw == condorcet_winner) ? 1 : 0;
    printf("Plurality=Condorcet: %d\n", plur_agrees_condorcet);
    // EXPECT: Plurality=Condorcet: 0

    int borda_agrees_condorcet = (bw == condorcet_winner) ? 1 : 0;
    printf("Borda=Condorcet: %d\n", borda_agrees_condorcet);
    // EXPECT: Borda=Condorcet: 1

    return 0;
}
