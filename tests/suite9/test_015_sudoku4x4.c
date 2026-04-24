int printf(const char *fmt, ...);

// 4x4 Sudoku solver using backtracking

int grid[4][4];

int is_valid_place(int row, int col, int num) {
    int i;
    int box_r;
    int box_c;
    int r;
    int c;

    // Check row
    for (i = 0; i < 4; i++) {
        if (grid[row][i] == num) return 0;
    }
    // Check column
    for (i = 0; i < 4; i++) {
        if (grid[i][col] == num) return 0;
    }
    // Check 2x2 box
    box_r = (row / 2) * 2;
    box_c = (col / 2) * 2;
    for (r = box_r; r < box_r + 2; r++) {
        for (c = box_c; c < box_c + 2; c++) {
            if (grid[r][c] == num) return 0;
        }
    }
    return 1;
}

int solve_sudoku(void) {
    int row;
    int col;
    int num;
    int found;

    found = 0;
    row = -1;
    col = -1;
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 4; col++) {
            if (grid[row][col] == 0) {
                found = 1;
                goto done_search;
            }
        }
    }
done_search:
    if (found == 0) return 1; // All cells filled

    for (num = 1; num <= 4; num++) {
        if (is_valid_place(row, col, num)) {
            grid[row][col] = num;
            if (solve_sudoku()) return 1;
            grid[row][col] = 0;
        }
    }
    return 0;
}

void print_grid(void) {
    int r;
    int c;
    for (r = 0; r < 4; r++) {
        for (c = 0; c < 4; c++) {
            if (c > 0) printf(" ");
            printf("%d", grid[r][c]);
        }
        printf("\n");
    }
}

int is_complete(void) {
    int r;
    int c;
    for (r = 0; r < 4; r++) {
        for (c = 0; c < 4; c++) {
            if (grid[r][c] == 0) return 0;
        }
    }
    return 1;
}

int is_valid_solution(void) {
    int r;
    int c;
    int i;
    int seen[5];
    int box_r;
    int box_c;
    int rr;
    int cc;

    // Check rows
    for (r = 0; r < 4; r++) {
        for (i = 1; i <= 4; i++) seen[i] = 0;
        for (c = 0; c < 4; c++) {
            if (grid[r][c] < 1 || grid[r][c] > 4) return 0;
            if (seen[grid[r][c]]) return 0;
            seen[grid[r][c]] = 1;
        }
    }
    // Check columns
    for (c = 0; c < 4; c++) {
        for (i = 1; i <= 4; i++) seen[i] = 0;
        for (r = 0; r < 4; r++) {
            if (seen[grid[r][c]]) return 0;
            seen[grid[r][c]] = 1;
        }
    }
    // Check 2x2 boxes
    for (box_r = 0; box_r < 4; box_r = box_r + 2) {
        for (box_c = 0; box_c < 4; box_c = box_c + 2) {
            for (i = 1; i <= 4; i++) seen[i] = 0;
            for (rr = box_r; rr < box_r + 2; rr++) {
                for (cc = box_c; cc < box_c + 2; cc++) {
                    if (seen[grid[rr][cc]]) return 0;
                    seen[grid[rr][cc]] = 1;
                }
            }
        }
    }
    return 1;
}

int solution_count;

void count_solutions(void) {
    int row;
    int col;
    int num;
    int found;

    found = 0;
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 4; col++) {
            if (grid[row][col] == 0) {
                found = 1;
                goto done2;
            }
        }
    }
done2:
    if (found == 0) {
        solution_count = solution_count + 1;
        return;
    }
    for (num = 1; num <= 4; num++) {
        if (is_valid_place(row, col, num)) {
            grid[row][col] = num;
            count_solutions();
            grid[row][col] = 0;
        }
    }
}

int main() {
    int result;
    int valid;

    // Puzzle 1
    grid[0][0] = 0; grid[0][1] = 0; grid[0][2] = 0; grid[0][3] = 4;
    grid[1][0] = 0; grid[1][1] = 4; grid[1][2] = 0; grid[1][3] = 0;
    grid[2][0] = 4; grid[2][1] = 0; grid[2][2] = 0; grid[2][3] = 0;
    grid[3][0] = 0; grid[3][1] = 0; grid[3][2] = 4; grid[3][3] = 0;

    result = solve_sudoku();
    // EXPECT: Puzzle 1 solved: 1
    printf("Puzzle 1 solved: %d\n", result);

    // EXPECT: Solution:
    printf("Solution:\n");
    // EXPECT: 1 2 3 4
    // EXPECT: 3 4 1 2
    // EXPECT: 4 1 2 3
    // EXPECT: 2 3 4 1
    print_grid();

    valid = is_valid_solution();
    // EXPECT: Valid: 1
    printf("Valid: %d\n", valid);

    // Puzzle 2: count solutions for empty grid
    grid[0][0] = 0; grid[0][1] = 0; grid[0][2] = 0; grid[0][3] = 0;
    grid[1][0] = 0; grid[1][1] = 0; grid[1][2] = 0; grid[1][3] = 0;
    grid[2][0] = 0; grid[2][1] = 0; grid[2][2] = 0; grid[2][3] = 0;
    grid[3][0] = 0; grid[3][1] = 0; grid[3][2] = 0; grid[3][3] = 0;
    solution_count = 0;
    count_solutions();
    // EXPECT: Empty 4x4 solutions: 288
    printf("Empty 4x4 solutions: %d\n", solution_count);

    // EXPECT: Sudoku done
    printf("Sudoku done\n");

    return 0;
}
