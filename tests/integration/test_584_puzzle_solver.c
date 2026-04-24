int printf(const char *fmt, ...);
// EXPECT: Puzzle 1:\n0 2 0 0 \n0 0 0 1 \n1 0 0 0 \n0 0 2 0 \nNo solution\n\nPuzzle 2:\n0 0 3 0 \n3 0 0 0 \n0 0 0 3 \n0 4 0 0 \nNo solution\nTotal sum of both solutions: 19
#define N 4

int is_valid(int grid[N][N], int row, int col, int num) {
    int i;
    for (i = 0; i < N; i++) {
        if (grid[row][i] == num) return 0;
    }
    for (i = 0; i < N; i++) {
        if (grid[i][col] == num) return 0;
    }
    int box_r = (row / 2) * 2;
    int box_c = (col / 2) * 2;
    int r;
    int c;
    for (r = box_r; r < box_r + 2; r++) {
        for (c = box_c; c < box_c + 2; c++) {
            if (grid[r][c] == num) return 0;
        }
    }
    return 1;
}

int find_empty(int grid[N][N], int *out_row, int *out_col) {
    int r;
    int c;
    for (r = 0; r < N; r++) {
        for (c = 0; c < N; c++) {
            if (grid[r][c] == 0) {
                *out_row = r;
                *out_col = c;
                return 1;
            }
        }
    }
    return 0;
}

int solve(int grid[N][N]) {
    int row;
    int col;
    if (!find_empty(grid, &row, &col)) return 1;

    int num;
    for (num = 1; num <= N; num++) {
        if (is_valid(grid, row, col, num)) {
            grid[row][col] = num;
            if (solve(grid)) return 1;
            grid[row][col] = 0;
        }
    }
    return 0;
}

void print_grid(int grid[N][N]) {
    int r;
    int c;
    for (r = 0; r < N; r++) {
        for (c = 0; c < N; c++) {
            printf("%d ", grid[r][c]);
        }
        printf("\n");
    }
}

int verify(int grid[N][N]) {
    int r;
    int c;
    int num;
    for (r = 0; r < N; r++) {
        for (c = 0; c < N; c++) {
            if (grid[r][c] < 1 || grid[r][c] > N) return 0;
            num = grid[r][c];
            grid[r][c] = 0;
            if (!is_valid(grid, r, c, num)) {
                grid[r][c] = num;
                return 0;
            }
            grid[r][c] = num;
        }
    }
    return 1;
}

int main(void) {
    int grid1[N][N];
    grid1[0][0] = 0; grid1[0][1] = 2; grid1[0][2] = 0; grid1[0][3] = 0;
    grid1[1][0] = 0; grid1[1][1] = 0; grid1[1][2] = 0; grid1[1][3] = 1;
    grid1[2][0] = 1; grid1[2][1] = 0; grid1[2][2] = 0; grid1[2][3] = 0;
    grid1[3][0] = 0; grid1[3][1] = 0; grid1[3][2] = 2; grid1[3][3] = 0;

    printf("Puzzle 1:\n");
    print_grid(grid1);

    if (solve(grid1)) {
        printf("Solution:\n");
        print_grid(grid1);
        printf("Valid: %d\n", verify(grid1));
    } else {
        printf("No solution\n");
    }

    int grid2[N][N];
    grid2[0][0] = 0; grid2[0][1] = 0; grid2[0][2] = 3; grid2[0][3] = 0;
    grid2[1][0] = 3; grid2[1][1] = 0; grid2[1][2] = 0; grid2[1][3] = 0;
    grid2[2][0] = 0; grid2[2][1] = 0; grid2[2][2] = 0; grid2[2][3] = 3;
    grid2[3][0] = 0; grid2[3][1] = 4; grid2[3][2] = 0; grid2[3][3] = 0;

    printf("\nPuzzle 2:\n");
    print_grid(grid2);

    if (solve(grid2)) {
        printf("Solution:\n");
        print_grid(grid2);
        printf("Valid: %d\n", verify(grid2));
    } else {
        printf("No solution\n");
    }

    int sum = 0;
    int r;
    int c;
    for (r = 0; r < N; r++) {
        for (c = 0; c < N; c++) {
            sum = sum + grid1[r][c] + grid2[r][c];
        }
    }
    printf("Total sum of both solutions: %d\n", sum);

    return 0;
}
