int printf(const char *fmt, ...);

// 15-Puzzle Solvability Checker
// A 4x4 sliding puzzle with tiles 1-15 and one blank (0)
// Solvability depends on inversions count and blank row position

int puzzle[16];

void set_puzzle(int vals[]) {
    int i;
    for (i = 0; i < 16; i++) {
        puzzle[i] = vals[i];
    }
}

// Count inversions: pairs (i,j) where i<j but puzzle[i]>puzzle[j], ignoring blank
int count_inversions(void) {
    int inv = 0;
    int i;
    int j;
    for (i = 0; i < 16; i++) {
        if (puzzle[i] == 0) continue;
        for (j = i + 1; j < 16; j++) {
            if (puzzle[j] == 0) continue;
            if (puzzle[i] > puzzle[j]) inv++;
        }
    }
    return inv;
}

// Find row of blank (0) from bottom, 1-indexed
int blank_row_from_bottom(void) {
    int i;
    for (i = 0; i < 16; i++) {
        if (puzzle[i] == 0) {
            int row = i / 4; // 0-indexed from top
            return 4 - row;  // convert to 1-indexed from bottom
        }
    }
    return -1;
}

// Find position of blank
int blank_pos(void) {
    int i;
    for (i = 0; i < 16; i++) {
        if (puzzle[i] == 0) return i;
    }
    return -1;
}

// For a 4x4 puzzle:
// Solvable if:
//   - blank on even row from bottom + odd inversions, OR
//   - blank on odd row from bottom + even inversions
int is_solvable(void) {
    int inv = count_inversions();
    int blank_row = blank_row_from_bottom();
    if (blank_row % 2 == 0) {
        return inv % 2 == 1;
    } else {
        return inv % 2 == 0;
    }
}

// Check if puzzle is in solved state
int is_solved(void) {
    int i;
    for (i = 0; i < 15; i++) {
        if (puzzle[i] != i + 1) return 0;
    }
    if (puzzle[15] != 0) return 0;
    return 1;
}

// Manhattan distance of a single tile
int tile_manhattan(int pos, int val) {
    if (val == 0) return 0;
    int cur_row = pos / 4;
    int cur_col = pos % 4;
    int goal_row = (val - 1) / 4;
    int goal_col = (val - 1) % 4;
    int dr = cur_row - goal_row;
    int dc = cur_col - goal_col;
    if (dr < 0) dr = -dr;
    if (dc < 0) dc = -dc;
    return dr + dc;
}

// Total Manhattan distance (heuristic for solving)
int total_manhattan(void) {
    int total = 0;
    int i;
    for (i = 0; i < 16; i++) {
        total += tile_manhattan(i, puzzle[i]);
    }
    return total;
}

// Count tiles in correct position
int tiles_in_place(void) {
    int count = 0;
    int i;
    for (i = 0; i < 15; i++) {
        if (puzzle[i] == i + 1) count++;
    }
    if (puzzle[15] == 0) count++;
    return count;
}

// Swap blank with adjacent tile (direction: 0=up,1=down,2=left,3=right)
int move_blank(int dir) {
    int pos = blank_pos();
    int row = pos / 4;
    int col = pos % 4;
    int new_pos = -1;
    if (dir == 0 && row > 0) new_pos = pos - 4;
    else if (dir == 1 && row < 3) new_pos = pos + 4;
    else if (dir == 2 && col > 0) new_pos = pos - 1;
    else if (dir == 3 && col < 3) new_pos = pos + 1;
    if (new_pos == -1) return 0;
    puzzle[pos] = puzzle[new_pos];
    puzzle[new_pos] = 0;
    return 1;
}

void print_puzzle(void) {
    int i;
    for (i = 0; i < 16; i++) {
        if (i > 0 && i % 4 == 0) printf("%s", "\n");
        if (i % 4 > 0) printf(" ");
        printf("%d", puzzle[i]);
    }
    printf("%s", "\n");
}

int main(void) {
    int vals[16];

    // Test 1: Solved puzzle
    vals[0]=1; vals[1]=2; vals[2]=3; vals[3]=4;
    vals[4]=5; vals[5]=6; vals[6]=7; vals[7]=8;
    vals[8]=9; vals[9]=10; vals[10]=11; vals[11]=12;
    vals[12]=13; vals[13]=14; vals[14]=15; vals[15]=0;
    set_puzzle(vals);
    printf("Solved: %d\n", is_solved());
    // EXPECT: Solved: 1

    // Test 2: Inversions in solved state
    printf("Inversions (solved): %d\n", count_inversions());
    // EXPECT: Inversions (solved): 0

    // Test 3: Solvability of solved state
    printf("Solvable (solved): %d\n", is_solvable());
    // EXPECT: Solvable (solved): 1

    // Test 4: Manhattan distance of solved state
    printf("Manhattan (solved): %d\n", total_manhattan());
    // EXPECT: Manhattan (solved): 0

    // Test 5: One move from solved
    vals[0]=1; vals[1]=2; vals[2]=3; vals[3]=4;
    vals[4]=5; vals[5]=6; vals[6]=7; vals[7]=8;
    vals[8]=9; vals[9]=10; vals[10]=11; vals[11]=0;
    vals[12]=13; vals[13]=14; vals[14]=15; vals[15]=12;
    set_puzzle(vals);
    printf("Solvable (1 move): %d\n", is_solvable());
    // EXPECT: Solvable (1 move): 1
    printf("Manhattan (1 move): %d\n", total_manhattan());
    // EXPECT: Manhattan (1 move): 1

    // Test 6: Unsolvable configuration
    vals[0]=1; vals[1]=2; vals[2]=3; vals[3]=4;
    vals[4]=5; vals[5]=6; vals[6]=7; vals[7]=8;
    vals[8]=9; vals[9]=10; vals[10]=11; vals[11]=12;
    vals[12]=13; vals[13]=15; vals[14]=14; vals[15]=0;
    set_puzzle(vals);
    int inv = count_inversions();
    printf("Inversions (swapped 14,15): %d\n", inv);
    // EXPECT: Inversions (swapped 14,15): 1
    printf("Solvable (swapped): %d\n", is_solvable());
    // EXPECT: Solvable (swapped): 0

    // Test 7: Blank position
    int bp = blank_pos();
    int br = blank_row_from_bottom();
    printf("Blank pos: %d row from bottom: %d\n", bp, br);
    // EXPECT: Blank pos: 15 row from bottom: 1

    // Test 8: Move blank up
    move_blank(0); // move up
    bp = blank_pos();
    printf("After move up, blank at: %d\n", bp);
    // EXPECT: After move up, blank at: 11

    // Test 9: Tiles in place count
    vals[0]=1; vals[1]=2; vals[2]=3; vals[3]=4;
    vals[4]=5; vals[5]=6; vals[6]=7; vals[7]=8;
    vals[8]=9; vals[9]=10; vals[10]=0; vals[11]=11;
    vals[12]=13; vals[13]=14; vals[14]=15; vals[15]=12;
    set_puzzle(vals);
    int tip = tiles_in_place();
    printf("Tiles in place: %d\n", tip);
    // EXPECT: Tiles in place: 13

    // Test 10: Scrambled but solvable
    vals[0]=5; vals[1]=1; vals[2]=2; vals[3]=3;
    vals[4]=9; vals[5]=6; vals[6]=7; vals[7]=4;
    vals[8]=13; vals[9]=10; vals[10]=11; vals[11]=8;
    vals[12]=0; vals[13]=14; vals[14]=15; vals[15]=12;
    set_puzzle(vals);
    inv = count_inversions();
    printf("Scrambled inversions: %d\n", inv);
    // EXPECT: Scrambled inversions: 18
    printf("Scrambled solvable: %d\n", is_solvable());
    // EXPECT: Scrambled solvable: 1

    // Test 11: Manhattan of scrambled
    int md = total_manhattan();
    printf("Scrambled manhattan: %d\n", md);
    // EXPECT: Scrambled manhattan: 9

    // Test 12: Invalid move (blank at corner)
    int ok = move_blank(2); // try left when blank is at col 0
    printf("Invalid move result: %d\n", ok);
    // EXPECT: Invalid move result: 0

    // Test 13: Print puzzle
    vals[0]=1; vals[1]=2; vals[2]=3; vals[3]=4;
    vals[4]=5; vals[5]=6; vals[6]=7; vals[7]=8;
    vals[8]=9; vals[9]=10; vals[10]=11; vals[11]=12;
    vals[12]=13; vals[13]=14; vals[14]=15; vals[15]=0;
    set_puzzle(vals);
    printf("Puzzle:\n");
    // EXPECT: Puzzle:
    print_puzzle();
    // EXPECT: 1 2 3 4
    // EXPECT: 5 6 7 8
    // EXPECT: 9 10 11 12
    // EXPECT: 13 14 15 0

    printf("15-puzzle tests passed\n");
    // EXPECT: 15-puzzle tests passed
    return 0;
}
