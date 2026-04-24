int printf(const char *fmt, ...);

// Tower of Hanoi: counting moves, recursive solution, and variations

// Count the minimum number of moves for n disks
int hanoi_count(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return 2 * hanoi_count(n - 1) + 1;
}

// Iterative count: 2^n - 1
int hanoi_count_iter(int n) {
    int result = 1;
    int i;
    for (i = 0; i < n; i++) {
        result = result * 2;
    }
    return result - 1;
}

// Global move counter for the simulation
int g_move_count;

// Simulate hanoi and count moves (no printing individual moves)
void hanoi_simulate(int n, int from, int to, int aux) {
    if (n <= 0) return;
    hanoi_simulate(n - 1, from, aux, to);
    g_move_count = g_move_count + 1;
    hanoi_simulate(n - 1, aux, to, from);
}

// Track tower states using arrays
// towers[0..2][0..19] represent the 3 pegs, each can hold up to 20 disks
// top[0..2] is the index of the top of each peg
int towers[3][20];
int top[3];

void init_towers(int n) {
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        top[i] = 0;
        for (j = 0; j < 20; j++) {
            towers[i][j] = 0;
        }
    }
    // Place disks on peg 0: largest at bottom
    for (i = 0; i < n; i++) {
        towers[0][i] = n - i;
        top[0] = i + 1;
    }
}

void move_disk(int from, int to) {
    int disk;
    top[from] = top[from] - 1;
    disk = towers[from][top[from]];
    towers[to][top[to]] = disk;
    top[to] = top[to] + 1;
}

void hanoi_with_towers(int n, int from, int to, int aux) {
    if (n <= 0) return;
    hanoi_with_towers(n - 1, from, aux, to);
    move_disk(from, to);
    hanoi_with_towers(n - 1, aux, to, from);
}

// Frame Stewart conjecture for 4-peg Hanoi (approximate)
// For small n, compute via recursion with optimal split
int four_peg_hanoi(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    int min_moves = 999999;
    int k;
    for (k = 1; k < n; k++) {
        int moves = 2 * four_peg_hanoi(n - k) + hanoi_count(k);
        if (moves < min_moves) {
            min_moves = moves;
        }
    }
    return min_moves;
}

// Compute the disk number that moves on step m (1-indexed)
// In standard Hanoi, step m moves disk (position of lowest set bit of m)
int disk_at_step(int m) {
    int disk = 1;
    while ((m % 2) == 0) {
        m = m / 2;
        disk = disk + 1;
    }
    return disk;
}

// Count how many times disk k is moved for n-disk Hanoi
int disk_move_count(int n, int k) {
    if (k > n || k <= 0) return 0;
    int result = 1;
    int i;
    for (i = 0; i < n - k; i++) {
        result = result * 2;
    }
    return result;
}

int main() {
    int i;

    // Test move counting (recursive)
    // EXPECT: hanoi(1) = 1
    printf("hanoi(1) = %d\n", hanoi_count(1));
    // EXPECT: hanoi(2) = 3
    printf("hanoi(2) = %d\n", hanoi_count(2));
    // EXPECT: hanoi(3) = 7
    printf("hanoi(3) = %d\n", hanoi_count(3));
    // EXPECT: hanoi(5) = 31
    printf("hanoi(5) = %d\n", hanoi_count(5));
    // EXPECT: hanoi(10) = 1023
    printf("hanoi(10) = %d\n", hanoi_count(10));

    // Verify iterative matches recursive
    int match = 1;
    for (i = 1; i <= 15; i++) {
        if (hanoi_count(i) != hanoi_count_iter(i)) {
            match = 0;
        }
    }
    // EXPECT: count_match = 1
    printf("count_match = %d\n", match);

    // Test simulation with global counter
    g_move_count = 0;
    hanoi_simulate(4, 0, 2, 1);
    // EXPECT: sim(4) = 15
    printf("sim(4) = %d\n", g_move_count);

    g_move_count = 0;
    hanoi_simulate(6, 0, 2, 1);
    // EXPECT: sim(6) = 63
    printf("sim(6) = %d\n", g_move_count);

    // Test with actual tower state tracking
    init_towers(4);
    // EXPECT: initial_top0 = 4
    printf("initial_top0 = %d\n", top[0]);
    // EXPECT: initial_top2 = 0
    printf("initial_top2 = %d\n", top[2]);

    hanoi_with_towers(4, 0, 2, 1);
    // EXPECT: final_top0 = 0
    printf("final_top0 = %d\n", top[0]);
    // EXPECT: final_top2 = 4
    printf("final_top2 = %d\n", top[2]);
    // EXPECT: bottom_disk = 4
    printf("bottom_disk = %d\n", towers[2][0]);
    // EXPECT: top_disk = 1
    printf("top_disk = %d\n", towers[2][3]);

    // Test 4-peg Hanoi
    // EXPECT: 4peg(1) = 1
    printf("4peg(1) = %d\n", four_peg_hanoi(1));
    // EXPECT: 4peg(2) = 3
    printf("4peg(2) = %d\n", four_peg_hanoi(2));
    // EXPECT: 4peg(3) = 5
    printf("4peg(3) = %d\n", four_peg_hanoi(3));
    // EXPECT: 4peg(4) = 9
    printf("4peg(4) = %d\n", four_peg_hanoi(4));

    // Test which disk moves at each step (for 3-disk problem)
    // EXPECT: step1_disk = 1
    printf("step1_disk = %d\n", disk_at_step(1));
    // EXPECT: step2_disk = 2
    printf("step2_disk = %d\n", disk_at_step(2));
    // EXPECT: step3_disk = 1
    printf("step3_disk = %d\n", disk_at_step(3));
    // EXPECT: step4_disk = 3
    printf("step4_disk = %d\n", disk_at_step(4));

    // Test how many times each disk moves in 5-disk problem
    // EXPECT: disk1_moves = 16
    printf("disk1_moves = %d\n", disk_move_count(5, 1));
    // EXPECT: disk3_moves = 4
    printf("disk3_moves = %d\n", disk_move_count(5, 3));
    // EXPECT: disk5_moves = 1
    printf("disk5_moves = %d\n", disk_move_count(5, 5));

    return 0;
}
