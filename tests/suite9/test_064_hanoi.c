int printf(const char *fmt, ...);

// Towers of Hanoi solver
// Pegs: A=0, B=1, C=2
// Track state with arrays

int pegs[3][20]; // each peg can hold up to 20 disks
int peg_top[3];  // index of top disk on each peg (-1 = empty)
int move_count;
int move_log_from[100];
int move_log_to[100];

void init_pegs(int n) {
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        peg_top[i] = -1;
        for (j = 0; j < 20; j++) {
            pegs[i][j] = 0;
        }
    }
    move_count = 0;
    // Place disks on peg 0 (largest at bottom)
    for (i = 0; i < n; i++) {
        peg_top[0] = i;
        pegs[0][i] = n - i; // disk sizes: n, n-1, ..., 1
    }
}

void move_disk(int from, int to) {
    int disk = pegs[from][peg_top[from]];
    pegs[from][peg_top[from]] = 0;
    peg_top[from] = peg_top[from] - 1;
    peg_top[to] = peg_top[to] + 1;
    pegs[to][peg_top[to]] = disk;
    if (move_count < 100) {
        move_log_from[move_count] = from;
        move_log_to[move_count] = to;
    }
    move_count++;
}

void hanoi(int n, int from, int to, int aux) {
    if (n == 1) {
        move_disk(from, to);
        return;
    }
    hanoi(n - 1, from, aux, to);
    move_disk(from, to);
    hanoi(n - 1, aux, to, from);
}

char peg_letter(int p) {
    if (p == 0) return 65; // 'A'
    if (p == 1) return 66; // 'B'
    return 67;             // 'C'
}

int is_valid_state(void) {
    int p;
    for (p = 0; p < 3; p++) {
        int i;
        for (i = 0; i < peg_top[p]; i++) {
            if (pegs[p][i] <= pegs[p][i + 1]) {
                return 0; // smaller disk below larger
            }
        }
    }
    return 1;
}

int peg_size(int p) {
    return peg_top[p] + 1;
}

int top_disk(int p) {
    if (peg_top[p] < 0) return 0;
    return pegs[p][peg_top[p]];
}

int main(void) {
    // Test 1: Solve for 1 disk
    init_pegs(1);
    hanoi(1, 0, 2, 1);
    printf("1 disk: %d moves\n", move_count);
    // EXPECT: 1 disk: 1 moves

    // Test 2: Verify 1-disk solution
    printf("Peg A size: %d Peg C size: %d\n", peg_size(0), peg_size(2));
    // EXPECT: Peg A size: 0 Peg C size: 1

    // Test 3: Solve for 2 disks
    init_pegs(2);
    hanoi(2, 0, 2, 1);
    printf("2 disks: %d moves\n", move_count);
    // EXPECT: 2 disks: 3 moves

    // Test 4: Solve for 3 disks
    init_pegs(3);
    hanoi(3, 0, 2, 1);
    printf("3 disks: %d moves\n", move_count);
    // EXPECT: 3 disks: 7 moves

    // Test 5: All disks on peg C after solving
    printf("Peg A: %d Peg B: %d Peg C: %d\n", peg_size(0), peg_size(1), peg_size(2));
    // EXPECT: Peg A: 0 Peg B: 0 Peg C: 3

    // Test 6: Verify disk order on peg C (bottom to top: 3, 2, 1)
    printf("C bottom: %d mid: %d top: %d\n", pegs[2][0], pegs[2][1], pegs[2][2]);
    // EXPECT: C bottom: 3 mid: 2 top: 1

    // Test 7: Valid state check
    int valid = is_valid_state();
    printf("Valid state: %d\n", valid);
    // EXPECT: Valid state: 1

    // Test 8: Solve for 4 disks
    init_pegs(4);
    hanoi(4, 0, 2, 1);
    printf("4 disks: %d moves\n", move_count);
    // EXPECT: 4 disks: 15 moves

    // Test 9: Print first 7 moves of 3-disk solution
    init_pegs(3);
    hanoi(3, 0, 2, 1);
    printf("3-disk moves:\n");
    // EXPECT: 3-disk moves:
    int i;
    for (i = 0; i < 7; i++) {
        int f = move_log_from[i];
        int t = move_log_to[i];
        char *fname;
        char *tname;
        if (f == 0) fname = "A";
        else if (f == 1) fname = "B";
        else fname = "C";
        if (t == 0) tname = "A";
        else if (t == 1) tname = "B";
        else tname = "C";
        printf("%s->%s\n", fname, tname);
    }
    // EXPECT: A->C
    // EXPECT: A->B
    // EXPECT: C->B
    // EXPECT: A->C
    // EXPECT: B->A
    // EXPECT: B->C
    // EXPECT: A->C

    // Test 10: 5 disks = 31 moves
    init_pegs(5);
    hanoi(5, 0, 2, 1);
    printf("5 disks: %d moves\n", move_count);
    // EXPECT: 5 disks: 31 moves

    // Test 11: Top disk on C after 5-disk solve
    int td = top_disk(2);
    printf("Top disk on C: %d\n", td);
    // EXPECT: Top disk on C: 1

    // Test 12: Formula check: 2^n - 1
    int expected = 1;
    int n;
    for (n = 0; n < 6; n++) expected = expected * 2;
    expected = expected - 1;
    init_pegs(6);
    hanoi(6, 0, 2, 1);
    printf("6 disks: %d expected: %d match: %d\n", move_count, expected, move_count == expected);
    // EXPECT: 6 disks: 63 expected: 63 match: 1

    printf("Hanoi tests passed\n");
    // EXPECT: Hanoi tests passed
    return 0;
}
