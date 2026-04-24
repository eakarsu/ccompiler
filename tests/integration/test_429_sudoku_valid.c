int printf(const char *fmt, ...);
// EXPECT: Valid sudoku: 1\nInvalid sudoku (dup in row): 0\nRow 0 valid: 1\nCol 0 valid: 1\nBox (0,0) valid: 1
// Test: Sudoku validator

int grid[9][9];

int check_row(int row) {
    int seen[10];
    int i;
    for (i = 0; i < 10; i++) seen[i] = 0;
    for (i = 0; i < 9; i++) {
        int val = grid[row][i];
        if (val < 1 || val > 9) return 0;
        if (seen[val]) return 0;
        seen[val] = 1;
    }
    return 1;
}

int check_col(int col) {
    int seen[10];
    int i;
    for (i = 0; i < 10; i++) seen[i] = 0;
    for (i = 0; i < 9; i++) {
        int val = grid[i][col];
        if (val < 1 || val > 9) return 0;
        if (seen[val]) return 0;
        seen[val] = 1;
    }
    return 1;
}

int check_box(int sr, int sc) {
    int seen[10];
    int i, j;
    for (i = 0; i < 10; i++) seen[i] = 0;
    for (i = sr; i < sr + 3; i++) {
        for (j = sc; j < sc + 3; j++) {
            int val = grid[i][j];
            if (val < 1 || val > 9) return 0;
            if (seen[val]) return 0;
            seen[val] = 1;
        }
    }
    return 1;
}

int is_valid_sudoku(void) {
    int i, j;
    for (i = 0; i < 9; i++) {
        if (!check_row(i)) return 0;
        if (!check_col(i)) return 0;
    }
    for (i = 0; i < 9; i += 3) {
        for (j = 0; j < 9; j += 3) {
            if (!check_box(i, j)) return 0;
        }
    }
    return 1;
}

void fill_grid(int data[]) {
    int i, j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            grid[i][j] = data[i * 9 + j];
        }
    }
}

int main(void) {
    int valid[] = {
        5,3,4,6,7,8,9,1,2,
        6,7,2,1,9,5,3,4,8,
        1,9,8,3,4,2,5,6,7,
        8,5,9,7,6,1,4,2,3,
        4,2,6,8,5,3,7,9,1,
        7,1,3,9,2,4,8,5,6,
        9,6,1,5,3,7,2,8,4,
        2,8,7,4,1,9,6,3,5,
        3,4,5,2,8,6,1,7,9
    };
    fill_grid(valid);
    printf("Valid sudoku: %d\n", is_valid_sudoku());

    int invalid[] = {
        5,3,4,6,7,8,9,1,2,
        6,7,2,1,9,5,3,4,8,
        1,9,8,3,4,2,5,6,7,
        8,5,9,7,6,1,4,2,3,
        4,2,6,8,5,3,7,9,1,
        7,1,3,9,2,4,8,5,6,
        9,6,1,5,3,7,2,8,4,
        2,8,7,4,1,9,6,3,5,
        3,4,5,2,8,6,1,7,1
    };
    fill_grid(invalid);
    printf("Invalid sudoku (dup in row): %d\n", is_valid_sudoku());

    printf("Row 0 valid: %d\n", check_row(0));
    printf("Col 0 valid: %d\n", check_col(0));
    printf("Box (0,0) valid: %d\n", check_box(0, 0));

    return 0;
}
