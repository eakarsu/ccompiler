int printf(const char *fmt, ...);

// N-Queens: count solutions for small board sizes

int board[8];

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

int is_safe(int row, int col) {
    int i;
    for (i = 0; i < row; i++) {
        if (board[i] == col) return 0;
        if (abs_val(board[i] - col) == abs_val(i - row)) return 0;
    }
    return 1;
}

int solve_nqueens(int n, int row) {
    int count;
    int col;
    if (row == n) {
        return 1;
    }
    count = 0;
    for (col = 0; col < n; col++) {
        if (is_safe(row, col)) {
            board[row] = col;
            count = count + solve_nqueens(n, row + 1);
        }
    }
    return count;
}

void print_solution(int n) {
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (board[i] == j) {
                printf("Q");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}

int find_first_solution(int n, int row) {
    int col;
    if (row == n) return 1;
    for (col = 0; col < n; col++) {
        if (is_safe(row, col)) {
            board[row] = col;
            if (find_first_solution(n, row + 1)) return 1;
        }
    }
    return 0;
}

int main() {
    int count1;
    int count2;
    int count3;
    int count4;
    int count5;

    count1 = solve_nqueens(1, 0);
    // EXPECT: N=1 solutions: 1
    printf("N=1 solutions: %d\n", count1);

    count2 = solve_nqueens(2, 0);
    // EXPECT: N=2 solutions: 0
    printf("N=2 solutions: %d\n", count2);

    count3 = solve_nqueens(3, 0);
    // EXPECT: N=3 solutions: 0
    printf("N=3 solutions: %d\n", count3);

    count4 = solve_nqueens(4, 0);
    // EXPECT: N=4 solutions: 2
    printf("N=4 solutions: %d\n", count4);

    count5 = solve_nqueens(5, 0);
    // EXPECT: N=5 solutions: 10
    printf("N=5 solutions: %d\n", count5);

    // EXPECT: First solution for N=4:
    printf("First solution for N=4:\n");
    find_first_solution(4, 0);
    // EXPECT: .Q..
    // EXPECT: ...Q
    // EXPECT: Q...
    // EXPECT: ..Q.
    print_solution(4);

    // EXPECT: First solution for N=5:
    printf("First solution for N=5:\n");
    find_first_solution(5, 0);
    // EXPECT: Q....
    // EXPECT: ..Q..
    // EXPECT: ....Q
    // EXPECT: .Q...
    // EXPECT: ...Q.
    print_solution(5);

    // EXPECT: N-Queens done
    printf("N-Queens done\n");

    return 0;
}
