int printf(const char *fmt, ...);

// Chess piece movement validation
// Board 8x8, pieces: 0=empty, 1=pawn, 2=rook, 3=knight, 4=bishop, 5=queen, 6=king
// Color: positive=white, negative=black

int board[64];

void clear_board(void) {
    int i;
    for (i = 0; i < 64; i++) board[i] = 0;
}

void place(int row, int col, int piece) {
    board[row * 8 + col] = piece;
}

int get_piece(int row, int col) {
    if (row < 0 || row > 7 || col < 0 || col > 7) return 99;
    return board[row * 8 + col];
}

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

// Check if path is clear between two squares (for rook/bishop/queen)
int path_clear(int r1, int c1, int r2, int c2) {
    int dr = 0;
    int dc = 0;
    if (r2 > r1) dr = 1;
    else if (r2 < r1) dr = -1;
    if (c2 > c1) dc = 1;
    else if (c2 < c1) dc = -1;
    int r = r1 + dr;
    int c = c1 + dc;
    while (r != r2 || c != c2) {
        if (get_piece(r, c) != 0) return 0;
        r += dr;
        c += dc;
    }
    return 1;
}

// Validate pawn move (simplified: white moves up, no en passant)
int valid_pawn(int r1, int c1, int r2, int c2, int color) {
    int dir;
    if (color > 0) dir = -1; // white moves up (decreasing row)
    else dir = 1;            // black moves down

    // Forward one
    if (c1 == c2 && r2 == r1 + dir && get_piece(r2, c2) == 0) return 1;
    // Forward two from starting row
    int start_row;
    if (color > 0) start_row = 6;
    else start_row = 1;
    if (c1 == c2 && r1 == start_row && r2 == r1 + 2 * dir &&
        get_piece(r1 + dir, c1) == 0 && get_piece(r2, c2) == 0) return 1;
    // Capture diagonally
    if (abs_val(c2 - c1) == 1 && r2 == r1 + dir) {
        int target = get_piece(r2, c2);
        if (target != 0 && (target > 0) != (color > 0)) return 1;
    }
    return 0;
}

int valid_rook(int r1, int c1, int r2, int c2) {
    if (r1 != r2 && c1 != c2) return 0;
    return path_clear(r1, c1, r2, c2);
}

int valid_knight(int r1, int c1, int r2, int c2) {
    int dr = abs_val(r2 - r1);
    int dc = abs_val(c2 - c1);
    return (dr == 2 && dc == 1) || (dr == 1 && dc == 2);
}

int valid_bishop(int r1, int c1, int r2, int c2) {
    int dr = abs_val(r2 - r1);
    int dc = abs_val(c2 - c1);
    if (dr != dc || dr == 0) return 0;
    return path_clear(r1, c1, r2, c2);
}

int valid_queen(int r1, int c1, int r2, int c2) {
    return valid_rook(r1, c1, r2, c2) || valid_bishop(r1, c1, r2, c2);
}

int valid_king(int r1, int c1, int r2, int c2) {
    int dr = abs_val(r2 - r1);
    int dc = abs_val(c2 - c1);
    return dr <= 1 && dc <= 1 && (dr + dc > 0);
}

// General move validator
int valid_move(int r1, int c1, int r2, int c2) {
    if (r2 < 0 || r2 > 7 || c2 < 0 || c2 > 7) return 0;
    int piece = get_piece(r1, c1);
    if (piece == 0) return 0;
    int target = get_piece(r2, c2);
    // Can't capture own piece
    if (target != 0 && (piece > 0) == (target > 0)) return 0;
    int type = abs_val(piece);
    if (type == 1) return valid_pawn(r1, c1, r2, c2, piece);
    if (type == 2) return valid_rook(r1, c1, r2, c2);
    if (type == 3) return valid_knight(r1, c1, r2, c2);
    if (type == 4) return valid_bishop(r1, c1, r2, c2);
    if (type == 5) return valid_queen(r1, c1, r2, c2);
    if (type == 6) return valid_king(r1, c1, r2, c2);
    return 0;
}

// Count all valid moves for a piece at (r,c)
int count_moves(int r, int c) {
    int count = 0;
    int tr;
    int tc;
    for (tr = 0; tr < 8; tr++) {
        for (tc = 0; tc < 8; tc++) {
            if (tr == r && tc == c) continue;
            if (valid_move(r, c, tr, tc)) count++;
        }
    }
    return count;
}

int main(void) {
    // Test 1: Knight moves from center
    clear_board();
    place(4, 4, 3); // white knight at e4
    int moves = count_moves(4, 4);
    printf("Knight center moves: %d\n", moves);
    // EXPECT: Knight center moves: 8

    // Test 2: Knight in corner
    clear_board();
    place(0, 0, 3);
    moves = count_moves(0, 0);
    printf("Knight corner moves: %d\n", moves);
    // EXPECT: Knight corner moves: 2

    // Test 3: Rook on empty board
    clear_board();
    place(3, 3, 2);
    moves = count_moves(3, 3);
    printf("Rook center moves: %d\n", moves);
    // EXPECT: Rook center moves: 14

    // Test 4: Rook blocked by own piece
    clear_board();
    place(3, 3, 2);
    place(3, 5, 1); // own pawn blocking
    moves = count_moves(3, 3);
    printf("Rook blocked moves: %d\n", moves);
    // EXPECT: Rook blocked moves: 11

    // Test 5: Bishop diagonal moves
    clear_board();
    place(3, 3, 4);
    moves = count_moves(3, 3);
    printf("Bishop center moves: %d\n", moves);
    // EXPECT: Bishop center moves: 13

    // Test 6: Queen combines rook and bishop
    clear_board();
    place(3, 3, 5);
    moves = count_moves(3, 3);
    printf("Queen center moves: %d\n", moves);
    // EXPECT: Queen center moves: 27

    // Test 7: King moves
    clear_board();
    place(4, 4, 6);
    moves = count_moves(4, 4);
    printf("King center moves: %d\n", moves);
    // EXPECT: King center moves: 8

    // Test 8: King in corner
    clear_board();
    place(0, 0, 6);
    moves = count_moves(0, 0);
    printf("King corner moves: %d\n", moves);
    // EXPECT: King corner moves: 3

    // Test 9: Pawn forward moves (white at starting row)
    clear_board();
    place(6, 3, 1); // white pawn at d2
    moves = count_moves(6, 3);
    printf("Pawn start moves: %d\n", moves);
    // EXPECT: Pawn start moves: 2

    // Test 10: Pawn capture
    clear_board();
    place(6, 3, 1);  // white pawn
    place(5, 4, -1); // black pawn diagonal
    moves = count_moves(6, 3);
    printf("Pawn with capture: %d\n", moves);
    // EXPECT: Pawn with capture: 3

    // Test 11: Pawn blocked
    clear_board();
    place(6, 3, 1);
    place(5, 3, -2); // black rook blocking
    moves = count_moves(6, 3);
    printf("Pawn blocked moves: %d\n", moves);
    // EXPECT: Pawn blocked moves: 0

    // Test 12: Path clear check
    clear_board();
    place(0, 0, 2); // rook at a8
    place(0, 3, -3); // enemy knight at d8
    int v = valid_move(0, 0, 0, 3);
    printf("Rook capture: %d\n", v);
    // EXPECT: Rook capture: 1
    int v2 = valid_move(0, 0, 0, 5);
    printf("Rook through piece: %d\n", v2);
    // EXPECT: Rook through piece: 0

    // Test 13: Knight jumps over pieces
    clear_board();
    place(0, 0, 3);
    place(0, 1, 1); place(1, 0, 1); place(1, 1, 1); // surrounded
    v = valid_move(0, 0, 2, 1);
    printf("Knight jumps: %d\n", v);
    // EXPECT: Knight jumps: 1

    printf("Chess move tests passed\n");
    // EXPECT: Chess move tests passed
    return 0;
}
