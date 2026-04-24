int printf(const char *fmt, ...);

// N-Queens solver: count solutions for small N

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

int is_safe(int *queens, int row, int col) {
    int i;
    for (i = 0; i < row; i++) {
        if (queens[i] == col) return 0;
        if (abs_val(queens[i] - col) == abs_val(i - row)) return 0;
    }
    return 1;
}

int solve_nqueens(int *queens, int row, int n) {
    if (row == n) return 1;
    int count = 0;
    int col;
    for (col = 0; col < n; col++) {
        if (is_safe(queens, row, col)) {
            queens[row] = col;
            count += solve_nqueens(queens, row + 1, n);
        }
    }
    return count;
}

int count_solutions(int n) {
    int queens[12];
    int i;
    for (i = 0; i < 12; i++) queens[i] = -1;
    return solve_nqueens(queens, 0, n);
}

// Store one solution for printing
int find_first_solution(int *queens, int row, int n) {
    if (row == n) return 1;
    int col;
    for (col = 0; col < n; col++) {
        if (is_safe(queens, row, col)) {
            queens[row] = col;
            if (find_first_solution(queens, row + 1, n))
                return 1;
        }
    }
    return 0;
}

void print_solution(int *queens, int n) {
    int r, c;
    for (r = 0; r < n; r++) {
        for (c = 0; c < n; c++) {
            if (queens[r] == c)
                printf("Q");
            else
                printf(".");
        }
        printf("\n");
    }
}

// Check if a specific placement is valid
int verify_solution(int *queens, int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        if (queens[i] < 0 || queens[i] >= n) return 0;
        for (j = i + 1; j < n; j++) {
            if (queens[i] == queens[j]) return 0;
            if (abs_val(queens[i] - queens[j]) == abs_val(i - j)) return 0;
        }
    }
    return 1;
}

int main() {
    // N=1: 1 solution
    printf("n1: %d\n", count_solutions(1));
    // EXPECT: n1: 1

    // N=2: 0 solutions
    printf("n2: %d\n", count_solutions(2));
    // EXPECT: n2: 0

    // N=3: 0 solutions
    printf("n3: %d\n", count_solutions(3));
    // EXPECT: n3: 0

    // N=4: 2 solutions
    printf("n4: %d\n", count_solutions(4));
    // EXPECT: n4: 2

    // N=5: 10 solutions
    printf("n5: %d\n", count_solutions(5));
    // EXPECT: n5: 10

    // N=6: 4 solutions
    printf("n6: %d\n", count_solutions(6));
    // EXPECT: n6: 4

    // N=7: 40 solutions
    printf("n7: %d\n", count_solutions(7));
    // EXPECT: n7: 40

    // N=8: 92 solutions
    printf("n8: %d\n", count_solutions(8));
    // EXPECT: n8: 92

    // First solution for N=4
    int q4[12];
    int i;
    for (i = 0; i < 12; i++) q4[i] = -1;
    find_first_solution(q4, 0, 4);
    printf("q4: %d %d %d %d\n", q4[0], q4[1], q4[2], q4[3]);
    // EXPECT: q4: 1 3 0 2
    printf("q4_valid: %d\n", verify_solution(q4, 4));
    // EXPECT: q4_valid: 1

    // First solution for N=5
    int q5[12];
    for (i = 0; i < 12; i++) q5[i] = -1;
    find_first_solution(q5, 0, 5);
    printf("q5: %d %d %d %d %d\n", q5[0], q5[1], q5[2], q5[3], q5[4]);
    // EXPECT: q5: 0 2 4 1 3
    printf("q5_valid: %d\n", verify_solution(q5, 5));
    // EXPECT: q5_valid: 1

    // Invalid placement test
    int bad[4];
    bad[0] = 0; bad[1] = 1; bad[2] = 2; bad[3] = 3;
    printf("bad_valid: %d\n", verify_solution(bad, 4));
    // EXPECT: bad_valid: 0

    // Another invalid: same column
    int bad2[4];
    bad2[0] = 0; bad2[1] = 0; bad2[2] = 0; bad2[3] = 0;
    printf("bad2_valid: %d\n", verify_solution(bad2, 4));
    // EXPECT: bad2_valid: 0

    printf("done\n");
    // EXPECT: done
    return 0;
}
