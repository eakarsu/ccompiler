int printf(const char *fmt, ...);
// EXPECT: === 4-Queens ===\nSolution 1: 1 3 0 2\nSolution 2: 2 0 3 1\nTotal 4-queens solutions: 2\n\n=== 5-Queens ===\nSolution 1: 0 2 4 1 3\nSolution 2: 0 3 1 4 2\nSolution 3: 1 3 0 2 4\nTotal 5-queens solutions: 10\n\n=== 6-Queens ===\nSolution 1: 1 3 5 0 2 4\nSolution 2: 2 5 1 4 0 3\nSolution 3: 3 0 4 1 5 2\nTotal 6-queens solutions: 4\n\n=== 7-Queens ===\nSolution 1: 0 2 4 6 1 3 5\nSolution 2: 0 3 6 2 5 1 4\nSolution 3: 0 4 1 5 2 6 3\nTotal 7-queens solutions: 40\n\nFirst 4-queens board:\nSolution 1: 1 3 0 2\nSolution 2: 2 0 3 1\n. Q . . \n. . . Q \nQ . . . \n. . Q . 
// Test: N-queens problem (4x4 and 5x5)

int board[8];
int solutions;

int is_safe(int row, int col) {
    int i;
    for (i = 0; i < row; i++) {
        if (board[i] == col) return 0;
        int diff = row - i;
        if (board[i] - col == diff || board[i] - col == -diff) return 0;
    }
    return 1;
}

void solve(int row, int n) {
    if (row == n) {
        solutions++;
        if (solutions <= 3) {
            printf("Solution %d: ", solutions);
            int i;
            for (i = 0; i < n; i++) {
                if (i > 0) printf(" ");
                printf("%d", board[i]);
            }
            printf("\n");
        }
        return;
    }
    int col;
    for (col = 0; col < n; col++) {
        if (is_safe(row, col)) {
            board[row] = col;
            solve(row + 1, n);
        }
    }
}

void print_board(int n) {
    int r, c;
    for (r = 0; r < n; r++) {
        for (c = 0; c < n; c++) {
            if (board[r] == c) printf("Q ");
            else printf(". ");
        }
        printf("\n");
    }
}

int main(void) {
    printf("=== 4-Queens ===\n");
    solutions = 0;
    solve(0, 4);
    printf("Total 4-queens solutions: %d\n", solutions);

    printf("\n=== 5-Queens ===\n");
    solutions = 0;
    solve(0, 5);
    printf("Total 5-queens solutions: %d\n", solutions);

    printf("\n=== 6-Queens ===\n");
    solutions = 0;
    solve(0, 6);
    printf("Total 6-queens solutions: %d\n", solutions);

    printf("\n=== 7-Queens ===\n");
    solutions = 0;
    solve(0, 7);
    printf("Total 7-queens solutions: %d\n", solutions);

    printf("\nFirst 4-queens board:\n");
    solutions = 0;
    solve(0, 4);
    board[0] = 1; board[1] = 3; board[2] = 0; board[3] = 2;
    print_board(4);

    return 0;
}
