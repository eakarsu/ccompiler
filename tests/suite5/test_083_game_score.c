int printf(const char *fmt, ...);

struct ScoreEntry {
    int score;
    int player_id;
};

struct Leaderboard {
    struct ScoreEntry entries[10];
    int count;
};

void init_board(struct Leaderboard *lb) {
    lb->count = 0;
    int i;
    for (i = 0; i < 10; i = i + 1) {
        lb->entries[i].score = 0;
        lb->entries[i].player_id = 0;
    }
}

int insert_score(struct Leaderboard *lb, int score, int pid) {
    /* Insert in sorted (descending) order, return rank (1-based) */
    /* Returns 0 if not inserted (board full and score too low) */
    int pos = lb->count;
    int i;

    /* Find insertion position */
    for (i = 0; i < lb->count; i = i + 1) {
        if (score > lb->entries[i].score) {
            pos = i;
            i = lb->count; /* break */
        }
    }

    if (pos >= 10) {
        return 0;
    }

    /* Shift entries down */
    i = lb->count;
    if (i > 9) i = 9;
    while (i > pos) {
        lb->entries[i] = lb->entries[i - 1];
        i = i - 1;
    }

    lb->entries[pos].score = score;
    lb->entries[pos].player_id = pid;
    if (lb->count < 10) {
        lb->count = lb->count + 1;
    }
    return pos + 1;
}

int get_rank(struct Leaderboard *lb, int pid) {
    int i;
    for (i = 0; i < lb->count; i = i + 1) {
        if (lb->entries[i].player_id == pid) {
            return i + 1;
        }
    }
    return 0;
}

int get_score_at(struct Leaderboard *lb, int rank) {
    if (rank < 1 || rank > lb->count) return -1;
    return lb->entries[rank - 1].score;
}

int total_scores(struct Leaderboard *lb) {
    int sum = 0;
    int i;
    for (i = 0; i < lb->count; i = i + 1) {
        sum = sum + lb->entries[i].score;
    }
    return sum;
}

int average_score(struct Leaderboard *lb) {
    if (lb->count == 0) return 0;
    return total_scores(lb) / lb->count;
}

int is_high_score(struct Leaderboard *lb, int score) {
    if (lb->count < 10) return 1;
    return score > lb->entries[lb->count - 1].score;
}

int main(void) {
    struct Leaderboard lb;
    init_board(&lb);
    int rank;

    // EXPECT: Count: 0
    printf("Count: %d\n", lb.count);

    rank = insert_score(&lb, 500, 1);
    // EXPECT: Insert p1 rank: 1
    printf("Insert p1 rank: %d\n", rank);

    rank = insert_score(&lb, 800, 2);
    // EXPECT: Insert p2 rank: 1
    printf("Insert p2 rank: %d\n", rank);

    rank = insert_score(&lb, 300, 3);
    // EXPECT: Insert p3 rank: 3
    printf("Insert p3 rank: %d\n", rank);

    rank = insert_score(&lb, 600, 4);
    // EXPECT: Insert p4 rank: 2
    printf("Insert p4 rank: %d\n", rank);

    // EXPECT: Count: 4
    printf("Count: %d\n", lb.count);

    // EXPECT: Top score: 800
    printf("Top score: %d\n", get_score_at(&lb, 1));
    // EXPECT: 2nd score: 600
    printf("2nd score: %d\n", get_score_at(&lb, 2));
    // EXPECT: 3rd score: 500
    printf("3rd score: %d\n", get_score_at(&lb, 3));
    // EXPECT: 4th score: 300
    printf("4th score: %d\n", get_score_at(&lb, 4));

    // EXPECT: P1 rank: 3
    printf("P1 rank: %d\n", get_rank(&lb, 1));
    // EXPECT: P2 rank: 1
    printf("P2 rank: %d\n", get_rank(&lb, 2));

    // EXPECT: Total: 2200
    printf("Total: %d\n", total_scores(&lb));
    // EXPECT: Average: 550
    printf("Average: %d\n", average_score(&lb));

    /* Insert more scores */
    insert_score(&lb, 750, 5);
    insert_score(&lb, 450, 6);
    insert_score(&lb, 900, 7);
    insert_score(&lb, 350, 8);
    insert_score(&lb, 700, 9);
    insert_score(&lb, 550, 10);

    // EXPECT: Count: 10
    printf("Count: %d\n", lb.count);
    // EXPECT: New top: 900
    printf("New top: %d\n", get_score_at(&lb, 1));
    // EXPECT: New 2nd: 800
    printf("New 2nd: %d\n", get_score_at(&lb, 2));

    /* Now board is full, try low score */
    // EXPECT: IsHigh 200: 0
    printf("IsHigh 200: %d\n", is_high_score(&lb, 200));
    // EXPECT: IsHigh 999: 1
    printf("IsHigh 999: %d\n", is_high_score(&lb, 999));

    rank = insert_score(&lb, 950, 11);
    // EXPECT: Insert 950 rank: 1
    printf("Insert 950 rank: %d\n", rank);
    // EXPECT: Still count: 10
    printf("Still count: %d\n", lb.count);
    // EXPECT: New top after: 950
    printf("New top after: %d\n", get_score_at(&lb, 1));

    return 0;
}
