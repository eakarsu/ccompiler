int printf(const char *fmt, ...);

// Connect Four win check
// Board is 6 rows x 7 columns, stored row-major
// 0=empty, 1=player1, 2=player2

int board[42]; // 6*7

int get_cell(int row, int col) {
    return board[row * 7 + col];
}

void set_cell(int row, int col, int val) {
    board[row * 7 + col] = val;
}

void clear_board(void) {
    int i;
    for (i = 0; i < 42; i++) board[i] = 0;
}

// Drop a piece into a column, returns the row it lands on, or -1 if full
int drop_piece(int col, int player) {
    int row;
    for (row = 5; row >= 0; row--) {
        if (get_cell(row, col) == 0) {
            set_cell(row, col, player);
            return row;
        }
    }
    return -1;
}

// Check if player has 4 in a row anywhere
int check_win(int player) {
    int r;
    int c;
    // Horizontal
    for (r = 0; r < 6; r++) {
        for (c = 0; c <= 3; c++) {
            if (get_cell(r, c) == player && get_cell(r, c+1) == player &&
                get_cell(r, c+2) == player && get_cell(r, c+3) == player)
                return 1;
        }
    }
    // Vertical
    for (r = 0; r <= 2; r++) {
        for (c = 0; c < 7; c++) {
            if (get_cell(r, c) == player && get_cell(r+1, c) == player &&
                get_cell(r+2, c) == player && get_cell(r+3, c) == player)
                return 1;
        }
    }
    // Diagonal down-right
    for (r = 0; r <= 2; r++) {
        for (c = 0; c <= 3; c++) {
            if (get_cell(r, c) == player && get_cell(r+1, c+1) == player &&
                get_cell(r+2, c+2) == player && get_cell(r+3, c+3) == player)
                return 1;
        }
    }
    // Diagonal down-left
    for (r = 0; r <= 2; r++) {
        for (c = 3; c < 7; c++) {
            if (get_cell(r, c) == player && get_cell(r+1, c-1) == player &&
                get_cell(r+2, c-2) == player && get_cell(r+3, c-3) == player)
                return 1;
        }
    }
    return 0;
}

int count_pieces(void) {
    int count = 0;
    int i;
    for (i = 0; i < 42; i++) {
        if (board[i] != 0) count++;
    }
    return count;
}

void print_board(void) {
    int r;
    int c;
    for (r = 0; r < 6; r++) {
        for (c = 0; c < 7; c++) {
            int v = get_cell(r, c);
            if (v == 0) printf(".");
            else if (v == 1) printf("%s", "X");
            else printf("%s", "O");
        }
        printf("%s", "\n");
    }
}

int main(void) {
    // Test 1: Horizontal win
    clear_board();
    drop_piece(0, 1);
    drop_piece(1, 1);
    drop_piece(2, 1);
    drop_piece(3, 1);
    int w = check_win(1);
    printf("Horizontal win: %d\n", w);
    // EXPECT: Horizontal win: 1

    // Test 2: No win yet
    clear_board();
    drop_piece(0, 1);
    drop_piece(1, 1);
    drop_piece(2, 1);
    w = check_win(1);
    printf("No win yet: %d\n", w);
    // EXPECT: No win yet: 0

    // Test 3: Vertical win
    clear_board();
    drop_piece(3, 2);
    drop_piece(3, 2);
    drop_piece(3, 2);
    drop_piece(3, 2);
    w = check_win(2);
    printf("Vertical win: %d\n", w);
    // EXPECT: Vertical win: 1

    // Test 4: Diagonal win (down-right)
    clear_board();
    // Build a diagonal: (5,0), (4,1), (3,2), (2,3) all player 1
    // Col 0: one piece
    drop_piece(0, 1);  // row 5
    // Col 1: need piece at row 4, so put filler at row 5
    drop_piece(1, 2);  // row 5 filler
    drop_piece(1, 1);  // row 4
    // Col 2: need piece at row 3, so two fillers
    drop_piece(2, 2);  // row 5
    drop_piece(2, 2);  // row 4
    drop_piece(2, 1);  // row 3
    // Col 3: need piece at row 2, so three fillers
    drop_piece(3, 2);  // row 5
    drop_piece(3, 2);  // row 4
    drop_piece(3, 2);  // row 3
    drop_piece(3, 1);  // row 2
    w = check_win(1);
    printf("Diagonal win: %d\n", w);
    // EXPECT: Diagonal win: 1

    // Test 5: Count pieces
    int cnt = count_pieces();
    printf("Pieces on board: %d\n", cnt);
    // EXPECT: Pieces on board: 10

    // Test 6: Column overflow
    clear_board();
    int i;
    for (i = 0; i < 6; i++) {
        drop_piece(0, 1);
    }
    int result = drop_piece(0, 2);
    printf("Column full result: %d\n", result);
    // EXPECT: Column full result: -1

    // Test 7: Diagonal win (down-left)
    clear_board();
    // (5,6), (4,5), (3,4), (2,3) all player 1
    drop_piece(6, 1);  // row 5
    drop_piece(5, 2);  // row 5
    drop_piece(5, 1);  // row 4
    drop_piece(4, 2);  // row 5
    drop_piece(4, 2);  // row 4
    drop_piece(4, 1);  // row 3
    drop_piece(3, 2);  // row 5
    drop_piece(3, 2);  // row 4
    drop_piece(3, 2);  // row 3
    drop_piece(3, 1);  // row 2
    w = check_win(1);
    printf("Diagonal left win: %d\n", w);
    // EXPECT: Diagonal left win: 1

    // Test 8: Print small board state
    clear_board();
    drop_piece(3, 1);
    drop_piece(3, 2);
    drop_piece(4, 1);
    printf("Board:\n");
    // EXPECT: Board:
    print_board();
    // EXPECT: .......
    // EXPECT: .......
    // EXPECT: .......
    // EXPECT: .......
    // EXPECT: ...O...
    // EXPECT: ...XX..

    // Test 9: Both players, no win
    clear_board();
    drop_piece(0, 1);
    drop_piece(1, 2);
    drop_piece(2, 1);
    drop_piece(3, 2);
    int w1 = check_win(1);
    int w2 = check_win(2);
    printf("P1 win: %d P2 win: %d\n", w1, w2);
    // EXPECT: P1 win: 0 P2 win: 0

    // Test 10: Multiple stacks
    clear_board();
    drop_piece(0, 1);
    drop_piece(0, 2);
    drop_piece(0, 1);
    drop_piece(0, 2);
    int r0 = get_cell(5, 0);
    int r1 = get_cell(4, 0);
    int r2 = get_cell(3, 0);
    int r3 = get_cell(2, 0);
    printf("Stack: %d %d %d %d\n", r0, r1, r2, r3);
    // EXPECT: Stack: 1 2 1 2

    printf("Connect four tests passed\n");
    // EXPECT: Connect four tests passed
    return 0;
}
