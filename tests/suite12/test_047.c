int printf(const char *fmt, ...);

// Constraint Satisfaction Problem: N-Queens using backtracking
// Place N queens on NxN board so no two attack each other

int board[12];      // board[col] = row of queen in that column
int col_used[12];   // is row used
int diag1[24];      // row+col diagonal
int diag2[24];      // row-col+N diagonal
int solutions;

void clear_board(int n) {
    int i = 0;
    while (i < n) {
        board[i] = -1;
        col_used[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (i < 2 * n) {
        diag1[i] = 0;
        diag2[i] = 0;
        i = i + 1;
    }
    solutions = 0;
}

int is_safe(int col, int row, int n) {
    if (col_used[row]) return 0;
    if (diag1[row + col]) return 0;
    if (diag2[row - col + n]) return 0;
    return 1;
}

void place_queen(int col, int row, int n) {
    board[col] = row;
    col_used[row] = 1;
    diag1[row + col] = 1;
    diag2[row - col + n] = 1;
}

void remove_queen(int col, int row, int n) {
    board[col] = -1;
    col_used[row] = 0;
    diag1[row + col] = 0;
    diag2[row - col + n] = 0;
}

void solve_nqueens(int col, int n) {
    if (col == n) {
        solutions = solutions + 1;
        return;
    }
    int row = 0;
    while (row < n) {
        if (is_safe(col, row, n)) {
            place_queen(col, row, n);
            solve_nqueens(col + 1, n);
            remove_queen(col, row, n);
        }
        row = row + 1;
    }
}

// Find first solution and store it
int found_first;
int first_sol[12];

void solve_first(int col, int n) {
    if (found_first) return;
    if (col == n) {
        found_first = 1;
        int i = 0;
        while (i < n) {
            first_sol[i] = board[i];
            i = i + 1;
        }
        return;
    }
    int row = 0;
    while (row < n) {
        if (found_first) return;
        if (is_safe(col, row, n)) {
            place_queen(col, row, n);
            solve_first(col + 1, n);
            remove_queen(col, row, n);
        }
        row = row + 1;
    }
}

// Verify a solution is valid
int verify_solution(int n) {
    int i = 0;
    while (i < n) {
        int j = i + 1;
        while (j < n) {
            // Same row?
            if (first_sol[i] == first_sol[j]) return 0;
            // Same diagonal?
            int dr = first_sol[i] - first_sol[j];
            if (dr < 0) dr = -dr;
            int dc = i - j;
            if (dc < 0) dc = -dc;
            if (dr == dc) return 0;
            j = j + 1;
        }
        i = i + 1;
    }
    return 1;
}

int main() {
    // 4-Queens
    clear_board(4);
    solve_nqueens(0, 4);
    printf("4-Queens solutions: %d\n", solutions);
    // EXPECT: 4-Queens solutions: 2

    // 5-Queens
    clear_board(5);
    solve_nqueens(0, 5);
    printf("5-Queens solutions: %d\n", solutions);
    // EXPECT: 5-Queens solutions: 10

    // 6-Queens
    clear_board(6);
    solve_nqueens(0, 6);
    printf("6-Queens solutions: %d\n", solutions);
    // EXPECT: 6-Queens solutions: 4

    // 7-Queens
    clear_board(7);
    solve_nqueens(0, 7);
    printf("7-Queens solutions: %d\n", solutions);
    // EXPECT: 7-Queens solutions: 40

    // 8-Queens
    clear_board(8);
    solve_nqueens(0, 8);
    printf("8-Queens solutions: %d\n", solutions);
    // EXPECT: 8-Queens solutions: 92

    // Find and display first 8-Queens solution
    clear_board(8);
    found_first = 0;
    solve_first(0, 8);
    printf("First 8-Queens: %d %d %d %d %d %d %d %d\n",
           first_sol[0], first_sol[1], first_sol[2], first_sol[3],
           first_sol[4], first_sol[5], first_sol[6], first_sol[7]);
    // EXPECT: First 8-Queens: 0 4 7 5 2 6 1 3

    // Verify the solution
    int valid = verify_solution(8);
    if (valid) {
        printf("Solution verified\n");
    } else {
        printf("Solution invalid\n");
    }
    // EXPECT: Solution verified

    // 1-Queen (trivial)
    clear_board(1);
    solve_nqueens(0, 1);
    printf("1-Queen solutions: %d\n", solutions);
    // EXPECT: 1-Queen solutions: 1

    // 2-Queens (impossible)
    clear_board(2);
    solve_nqueens(0, 2);
    printf("2-Queens solutions: %d\n", solutions);
    // EXPECT: 2-Queens solutions: 0

    // 3-Queens (impossible)
    clear_board(3);
    solve_nqueens(0, 3);
    printf("3-Queens solutions: %d\n", solutions);
    // EXPECT: 3-Queens solutions: 0

    printf("CSP done\n");
    // EXPECT: CSP done

    return 0;
}
