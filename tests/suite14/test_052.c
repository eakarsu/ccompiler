int printf(const char *fmt, ...);

// Sudoku validator: check rows, cols, and 3x3 boxes

int check_row(int grid[9][9], int row) {
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

int check_col(int grid[9][9], int col) {
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

int check_box(int grid[9][9], int box_row, int box_col) {
    int seen[10];
    int i;
    for (i = 0; i < 10; i++) seen[i] = 0;
    int r, c;
    for (r = box_row; r < box_row + 3; r++) {
        for (c = box_col; c < box_col + 3; c++) {
            int val = grid[r][c];
            if (val < 1 || val > 9) return 0;
            if (seen[val]) return 0;
            seen[val] = 1;
        }
    }
    return 1;
}

int validate_sudoku(int grid[9][9]) {
    int i, r, c;
    for (i = 0; i < 9; i++) {
        if (!check_row(grid, i)) return 0;
        if (!check_col(grid, i)) return 0;
    }
    for (r = 0; r < 9; r += 3) {
        for (c = 0; c < 9; c += 3) {
            if (!check_box(grid, r, c)) return 0;
        }
    }
    return 1;
}

int count_valid_rows(int grid[9][9]) {
    int count = 0;
    int i;
    for (i = 0; i < 9; i++) {
        if (check_row(grid, i)) count++;
    }
    return count;
}

int count_valid_cols(int grid[9][9]) {
    int count = 0;
    int i;
    for (i = 0; i < 9; i++) {
        if (check_col(grid, i)) count++;
    }
    return count;
}

int count_valid_boxes(int grid[9][9]) {
    int count = 0;
    int r, c;
    for (r = 0; r < 9; r += 3) {
        for (c = 0; c < 9; c += 3) {
            if (check_box(grid, r, c)) count++;
        }
    }
    return count;
}

int row_sum(int grid[9][9], int row) {
    int sum = 0;
    int i;
    for (i = 0; i < 9; i++) sum += grid[row][i];
    return sum;
}

int main() {
    // Valid completed sudoku
    int g[9][9];
    // Row 0
    g[0][0]=5; g[0][1]=3; g[0][2]=4; g[0][3]=6; g[0][4]=7; g[0][5]=8; g[0][6]=9; g[0][7]=1; g[0][8]=2;
    // Row 1
    g[1][0]=6; g[1][1]=7; g[1][2]=2; g[1][3]=1; g[1][4]=9; g[1][5]=5; g[1][6]=3; g[1][7]=4; g[1][8]=8;
    // Row 2
    g[2][0]=1; g[2][1]=9; g[2][2]=8; g[2][3]=3; g[2][4]=4; g[2][5]=2; g[2][6]=5; g[2][7]=6; g[2][8]=7;
    // Row 3
    g[3][0]=8; g[3][1]=5; g[3][2]=9; g[3][3]=7; g[3][4]=6; g[3][5]=1; g[3][6]=4; g[3][7]=2; g[3][8]=3;
    // Row 4
    g[4][0]=4; g[4][1]=2; g[4][2]=6; g[4][3]=8; g[4][4]=5; g[4][5]=3; g[4][6]=7; g[4][7]=9; g[4][8]=1;
    // Row 5
    g[5][0]=7; g[5][1]=1; g[5][2]=3; g[5][3]=9; g[5][4]=2; g[5][5]=4; g[5][6]=8; g[5][7]=5; g[5][8]=6;
    // Row 6
    g[6][0]=9; g[6][1]=6; g[6][2]=1; g[6][3]=5; g[6][4]=3; g[6][5]=7; g[6][6]=2; g[6][7]=8; g[6][8]=4;
    // Row 7
    g[7][0]=2; g[7][1]=8; g[7][2]=7; g[7][3]=4; g[7][4]=1; g[7][5]=9; g[7][6]=6; g[7][7]=3; g[7][8]=5;
    // Row 8
    g[8][0]=3; g[8][1]=4; g[8][2]=5; g[8][3]=2; g[8][4]=8; g[8][5]=6; g[8][6]=1; g[8][7]=7; g[8][8]=9;

    printf("valid_rows: %d\n", count_valid_rows(g));
    // EXPECT: valid_rows: 9
    printf("valid_cols: %d\n", count_valid_cols(g));
    // EXPECT: valid_cols: 9
    printf("valid_boxes: %d\n", count_valid_boxes(g));
    // EXPECT: valid_boxes: 9
    printf("valid_sudoku: %d\n", validate_sudoku(g));
    // EXPECT: valid_sudoku: 1

    // Each row should sum to 45
    printf("row0_sum: %d\n", row_sum(g, 0));
    // EXPECT: row0_sum: 45
    printf("row4_sum: %d\n", row_sum(g, 4));
    // EXPECT: row4_sum: 45
    printf("row8_sum: %d\n", row_sum(g, 8));
    // EXPECT: row8_sum: 45

    // Now corrupt it: duplicate in row 0
    g[0][0] = 3; // was 5, now duplicate 3
    printf("after_corrupt_row0: %d\n", check_row(g, 0));
    // EXPECT: after_corrupt_row0: 0
    printf("valid_after_corrupt: %d\n", validate_sudoku(g));
    // EXPECT: valid_after_corrupt: 0

    // Fix row 0 but corrupt col 0
    g[0][0] = 5;
    g[1][0] = 5; // was 6, now duplicate 5 in col 0
    printf("check_col0: %d\n", check_col(g, 0));
    // EXPECT: check_col0: 0
    printf("check_row1_still: %d\n", check_row(g, 1));
    // EXPECT: check_row1_still: 0

    // Restore and test box
    g[1][0] = 6;
    printf("restored_valid: %d\n", validate_sudoku(g));
    // EXPECT: restored_valid: 1

    // Corrupt a box
    g[0][0] = 7; // box(0,0) now has 7 twice (row0col0 and row2col2? no, row2col2=8)
    // box(0,0): 7,3,4 / 6,7,2 / 1,9,8 -> 7 appears twice
    printf("box00_corrupt: %d\n", check_box(g, 0, 0));
    // EXPECT: box00_corrupt: 0

    g[0][0] = 5; // restore
    printf("final_valid: %d\n", validate_sudoku(g));
    // EXPECT: final_valid: 1

    printf("done\n");
    // EXPECT: done
    return 0;
}
