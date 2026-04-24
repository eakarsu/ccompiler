int printf(const char *fmt, ...);
// EXPECT: === N-Queens solution counts ===\nN=1: 1 solutions\nN=2: 0 solutions\nN=3: 0 solutions\nN=4: 2 solutions\nN=5: 10 solutions\nN=6: 4 solutions\nN=7: 40 solutions\nN=8: 92 solutions\n=== First solution for 5-Queens ===\nQ . . . . \n. . Q . . \n. . . . Q \n. Q . . . \n. . . Q . \n\n=== First solution for 6-Queens ===\n. Q . . . . \n. . . Q . . \n. . . . . Q \nQ . . . . . \n. . Q . . . \n. . . . Q . \n\n=== Rook placements ===\nrooks(1)=1\nrooks(2)=2\nrooks(3)=6\nrooks(4)=24\nrooks(5)=120\nrooks(6)=720
// Test 235: N-Queens on small board

int queens[12];
int solutions;

int is_safe(int row, int col) {
    int i = 0;
    while (i < row) {
        if (queens[i] == col) return 0;
        int diff = row - i;
        if (queens[i] - col == diff || col - queens[i] == diff) return 0;
        i = i + 1;
    }
    return 1;
}

void solve_nqueens(int row, int n) {
    if (row == n) {
        solutions = solutions + 1;
        return;
    }
    int col = 0;
    while (col < n) {
        if (is_safe(row, col)) {
            queens[row] = col;
            solve_nqueens(row + 1, n);
        }
        col = col + 1;
    }
}

void print_solution(int n) {
    int i = 0;
    while (i < n) {
        int j = 0;
        while (j < n) {
            if (queens[i] == j) printf("Q ");
            else printf(". ");
            j = j + 1;
        }
        printf("\n");
        i = i + 1;
    }
    printf("\n");
}

int first_solution_count;

void solve_first(int row, int n) {
    if (first_solution_count > 0) return;
    if (row == n) {
        first_solution_count = 1;
        print_solution(n);
        return;
    }
    int col = 0;
    while (col < n) {
        if (first_solution_count > 0) return;
        if (is_safe(row, col)) {
            queens[row] = col;
            solve_first(row + 1, n);
        }
        col = col + 1;
    }
}

int rook_placements(int n, int row, int used_cols) {
    if (row == n) return 1;
    int count = 0;
    int col = 0;
    while (col < n) {
        int bit = 1;
        int shift = 0;
        while (shift < col) { bit = bit * 2; shift = shift + 1; }
        if ((used_cols / bit) - ((used_cols / bit) / 2) * 2 == 0) {
            count = count + rook_placements(n, row + 1, used_cols + bit);
        }
        col = col + 1;
    }
    return count;
}

int main(void) {
    printf("=== N-Queens solution counts ===\n");
    int n = 1;
    while (n <= 8) {
        solutions = 0;
        solve_nqueens(0, n);
        printf("N=%d: %d solutions\n", n, solutions);
        n = n + 1;
    }

    printf("=== First solution for 5-Queens ===\n");
    first_solution_count = 0;
    solve_first(0, 5);

    printf("=== First solution for 6-Queens ===\n");
    first_solution_count = 0;
    solve_first(0, 6);

    printf("=== Rook placements ===\n");
    n = 1;
    while (n <= 6) {
        printf("rooks(%d)=%d\n", n, rook_placements(n, 0, 0));
        n = n + 1;
    }

    return 0;
}
