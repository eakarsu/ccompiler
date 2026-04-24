int printf(const char *fmt, ...);

struct Candidate {
    char name[16];
    int votes;
    int eliminated;
    int id;
};

struct Voter {
    int choices[8];
    int num_choices;
};

int str_copy(char *dst, char *src) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
    return i;
}

int str_eq(char *a, char *b) {
    int i;
    i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) {
            return 0;
        }
        i = i + 1;
    }
    if (a[i] != b[i]) {
        return 0;
    }
    return 1;
}

int init_candidate(struct Candidate *c, int id, char *name) {
    c->id = id;
    str_copy(c->name, name);
    c->votes = 0;
    c->eliminated = 0;
    return 0;
}

int init_voter(struct Voter *v, int c0, int c1, int c2, int n) {
    v->choices[0] = c0;
    v->choices[1] = c1;
    v->choices[2] = c2;
    v->num_choices = n;
    return 0;
}

int tally_simple(struct Candidate *cands, int ncands, int *votes, int nvotes) {
    int i;
    i = 0;
    while (i < ncands) {
        cands[i].votes = 0;
        i = i + 1;
    }
    i = 0;
    while (i < nvotes) {
        if (votes[i] >= 0 && votes[i] < ncands) {
            cands[votes[i]].votes = cands[votes[i]].votes + 1;
        }
        i = i + 1;
    }
    return 0;
}

int find_winner(struct Candidate *cands, int ncands) {
    int i;
    int best;
    int best_votes;
    best = -1;
    best_votes = -1;
    i = 0;
    while (i < ncands) {
        if (cands[i].eliminated == 0 && cands[i].votes > best_votes) {
            best_votes = cands[i].votes;
            best = i;
        }
        i = i + 1;
    }
    return best;
}

int check_tie(struct Candidate *cands, int ncands) {
    int i;
    int max_votes;
    int count_with_max;

    max_votes = -1;
    i = 0;
    while (i < ncands) {
        if (cands[i].eliminated == 0 && cands[i].votes > max_votes) {
            max_votes = cands[i].votes;
        }
        i = i + 1;
    }

    count_with_max = 0;
    i = 0;
    while (i < ncands) {
        if (cands[i].eliminated == 0 && cands[i].votes == max_votes) {
            count_with_max = count_with_max + 1;
        }
        i = i + 1;
    }

    if (count_with_max > 1) {
        return 1;
    }
    return 0;
}

int total_votes(struct Candidate *cands, int ncands) {
    int i;
    int total;
    total = 0;
    i = 0;
    while (i < ncands) {
        if (cands[i].eliminated == 0) {
            total = total + cands[i].votes;
        }
        i = i + 1;
    }
    return total;
}

int count_active_candidates(struct Candidate *cands, int ncands) {
    int i;
    int count;
    count = 0;
    i = 0;
    while (i < ncands) {
        if (cands[i].eliminated == 0) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

int find_lowest(struct Candidate *cands, int ncands) {
    int i;
    int worst;
    int worst_votes;
    worst = -1;
    worst_votes = 999999;
    i = 0;
    while (i < ncands) {
        if (cands[i].eliminated == 0 && cands[i].votes < worst_votes) {
            worst_votes = cands[i].votes;
            worst = i;
        }
        i = i + 1;
    }
    return worst;
}

int get_top_choice(struct Voter *v, struct Candidate *cands, int ncands) {
    int i;
    i = 0;
    while (i < v->num_choices) {
        if (v->choices[i] >= 0 && v->choices[i] < ncands) {
            if (cands[v->choices[i]].eliminated == 0) {
                return v->choices[i];
            }
        }
        i = i + 1;
    }
    return -1;
}

int ranked_choice_round(struct Candidate *cands, int ncands, struct Voter *voters, int nvoters) {
    int i;
    int choice;
    int lowest;

    /* Reset votes */
    i = 0;
    while (i < ncands) {
        cands[i].votes = 0;
        i = i + 1;
    }

    /* Tally based on top remaining choice */
    i = 0;
    while (i < nvoters) {
        choice = get_top_choice(&voters[i], cands, ncands);
        if (choice >= 0) {
            cands[choice].votes = cands[choice].votes + 1;
        }
        i = i + 1;
    }

    /* Check for majority */
    i = 0;
    while (i < ncands) {
        if (cands[i].eliminated == 0 && cands[i].votes * 2 > nvoters) {
            return i;
        }
        i = i + 1;
    }

    /* Eliminate lowest */
    lowest = find_lowest(cands, ncands);
    if (lowest >= 0) {
        cands[lowest].eliminated = 1;
    }

    return -1;
}

int main(void) {
    struct Candidate cands[6];
    struct Voter voters[12];
    int simple_votes[10];
    int result;
    int i;

    /* ===== Test 1: Simple plurality vote ===== */
    init_candidate(&cands[0], 0, "Alice");
    init_candidate(&cands[1], 1, "Bob");
    init_candidate(&cands[2], 2, "Carol");

    simple_votes[0] = 0;
    simple_votes[1] = 1;
    simple_votes[2] = 0;
    simple_votes[3] = 2;
    simple_votes[4] = 0;
    simple_votes[5] = 1;
    simple_votes[6] = 0;

    tally_simple(cands, 3, simple_votes, 7);

    // EXPECT: 4
    printf("%d\n", cands[0].votes);

    // EXPECT: 2
    printf("%d\n", cands[1].votes);

    // EXPECT: 1
    printf("%d\n", cands[2].votes);

    result = find_winner(cands, 3);
    // EXPECT: 0
    printf("%d\n", result);

    // EXPECT: 7
    printf("%d\n", total_votes(cands, 3));

    // EXPECT: 0
    printf("%d\n", check_tie(cands, 3));

    /* ===== Test 2: Tie detection ===== */
    init_candidate(&cands[0], 0, "Dan");
    init_candidate(&cands[1], 1, "Eve");

    simple_votes[0] = 0;
    simple_votes[1] = 1;
    simple_votes[2] = 0;
    simple_votes[3] = 1;

    tally_simple(cands, 2, simple_votes, 4);

    // EXPECT: 2
    printf("%d\n", cands[0].votes);

    // EXPECT: 2
    printf("%d\n", cands[1].votes);

    // EXPECT: 1
    printf("%d\n", check_tie(cands, 2));

    /* ===== Test 3: Ranked choice voting ===== */
    /* 4 candidates: A=0, B=1, C=2, D=3 */
    init_candidate(&cands[0], 0, "Alpha");
    init_candidate(&cands[1], 1, "Beta");
    init_candidate(&cands[2], 2, "Gamma");
    init_candidate(&cands[3], 3, "Delta");

    /* 9 voters with ranked preferences */
    init_voter(&voters[0], 0, 1, 2, 3);
    init_voter(&voters[1], 0, 1, 2, 3);
    init_voter(&voters[2], 0, 2, 1, 3);
    init_voter(&voters[3], 1, 0, 2, 3);
    init_voter(&voters[4], 1, 2, 0, 3);
    init_voter(&voters[5], 2, 1, 0, 3);
    init_voter(&voters[6], 2, 1, 0, 3);
    init_voter(&voters[7], 3, 2, 1, 3);
    init_voter(&voters[8], 3, 0, 1, 3);

    // EXPECT: 4
    printf("%d\n", count_active_candidates(cands, 4));

    /* Round 1: A=3, B=2, C=2, D=2. No majority(>4.5). D is found first lowest at 1 -- actually B,C,D all have 2. find_lowest returns first with min. */
    result = ranked_choice_round(cands, 4, voters, 9);
    // EXPECT: -1
    printf("%d\n", result);

    /* After round 1: votes are A=3, B=2, C=2, D=2. Lowest is B(index 1). B eliminated. */
    // EXPECT: 3
    printf("%d\n", cands[0].votes);

    // EXPECT: 1
    printf("%d\n", cands[1].eliminated);

    // EXPECT: 3
    printf("%d\n", count_active_candidates(cands, 4));

    /* Round 2: A gets 3, B eliminated, C gets 2+B's voters reroute.
       Voter 3(1,0,2)->0(Alpha), Voter 4(1,2,0)->2(Gamma).
       A=3+1=4, C=2+1=3, D=2. No majority(>4.5). D eliminated. */
    result = ranked_choice_round(cands, 4, voters, 9);
    // EXPECT: -1
    printf("%d\n", result);

    // EXPECT: 4
    printf("%d\n", cands[0].votes);

    // EXPECT: 3
    printf("%d\n", cands[2].votes);

    // EXPECT: 1
    printf("%d\n", cands[3].eliminated);

    /* Round 3: D eliminated. D's voters reroute.
       Voter 7(3,2,1)->2(Gamma, since B eliminated, C(Gamma) active). Voter 8(3,0,1)->0(Alpha).
       A=4+1=5, C=3+1=4. A has majority (5 > 4.5). */
    result = ranked_choice_round(cands, 4, voters, 9);
    // EXPECT: 0
    printf("%d\n", result);

    // EXPECT: 5
    printf("%d\n", cands[0].votes);

    /* ===== Test 4: Candidate with zero votes ===== */
    init_candidate(&cands[0], 0, "X");
    init_candidate(&cands[1], 1, "Y");
    init_candidate(&cands[2], 2, "Z");

    simple_votes[0] = 0;
    simple_votes[1] = 0;
    simple_votes[2] = 0;

    tally_simple(cands, 3, simple_votes, 3);

    // EXPECT: 3
    printf("%d\n", cands[0].votes);

    // EXPECT: 0
    printf("%d\n", cands[1].votes);

    // EXPECT: 0
    printf("%d\n", cands[2].votes);

    result = find_lowest(cands, 3);
    // EXPECT: 1
    printf("%d\n", result);

    return 0;
}
