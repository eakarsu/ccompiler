int printf(const char *fmt, ...);

// Connect-4 board evaluation
// Board: 7 columns x 6 rows, 0=empty, 1=red, 2=yellow

typedef struct {
    int cells[6][7];
    int col_height[7]; // next free row in each column
} Board;

void board_init(Board *b) {
    int r, c;
    for (r = 0; r < 6; r++)
        for (c = 0; c < 7; c++)
            b->cells[r][c] = 0;
    for (c = 0; c < 7; c++)
        b->col_height[c] = 0;
}

// Drop a piece in column, returns row placed or -1 if full
int drop_piece(Board *b, int col, int player) {
    if (col < 0 || col >= 7) return -1;
    if (b->col_height[col] >= 6) return -1;
    int row = b->col_height[col];
    b->cells[row][col] = player;
    b->col_height[col]++;
    return row;
}

// Check for 4 in a row for given player
int check_four(Board *b, int player) {
    int r, c;
    // Horizontal
    for (r = 0; r < 6; r++) {
        for (c = 0; c <= 3; c++) {
            if (b->cells[r][c] == player &&
                b->cells[r][c+1] == player &&
                b->cells[r][c+2] == player &&
                b->cells[r][c+3] == player)
                return 1;
        }
    }
    // Vertical
    for (r = 0; r <= 2; r++) {
        for (c = 0; c < 7; c++) {
            if (b->cells[r][c] == player &&
                b->cells[r+1][c] == player &&
                b->cells[r+2][c] == player &&
                b->cells[r+3][c] == player)
                return 1;
        }
    }
    // Diagonal up-right
    for (r = 0; r <= 2; r++) {
        for (c = 0; c <= 3; c++) {
            if (b->cells[r][c] == player &&
                b->cells[r+1][c+1] == player &&
                b->cells[r+2][c+2] == player &&
                b->cells[r+3][c+3] == player)
                return 1;
        }
    }
    // Diagonal down-right
    for (r = 3; r < 6; r++) {
        for (c = 0; c <= 3; c++) {
            if (b->cells[r][c] == player &&
                b->cells[r-1][c+1] == player &&
                b->cells[r-2][c+2] == player &&
                b->cells[r-3][c+3] == player)
                return 1;
        }
    }
    return 0;
}

// Count number of 3-in-a-row opportunities (with one empty)
int count_threats(Board *b, int player) {
    int threats = 0;
    int r, c;
    // Horizontal threats
    for (r = 0; r < 6; r++) {
        for (c = 0; c <= 3; c++) {
            int count = 0;
            int empty = 0;
            int i;
            for (i = 0; i < 4; i++) {
                if (b->cells[r][c+i] == player) count++;
                else if (b->cells[r][c+i] == 0) empty++;
            }
            if (count == 3 && empty == 1) threats++;
        }
    }
    // Vertical threats
    for (r = 0; r <= 2; r++) {
        for (c = 0; c < 7; c++) {
            int count = 0;
            int empty = 0;
            int i;
            for (i = 0; i < 4; i++) {
                if (b->cells[r+i][c] == player) count++;
                else if (b->cells[r+i][c] == 0) empty++;
            }
            if (count == 3 && empty == 1) threats++;
        }
    }
    return threats;
}

int total_pieces(Board *b) {
    int count = 0;
    int c;
    for (c = 0; c < 7; c++) count += b->col_height[c];
    return count;
}

int main() {
    Board b;
    board_init(&b);

    // Test basic drops
    int r0 = drop_piece(&b, 3, 1);
    printf("drop1: %d\n", r0);
    // EXPECT: drop1: 0
    int r1 = drop_piece(&b, 3, 2);
    printf("drop2: %d\n", r1);
    // EXPECT: drop2: 1
    int r2 = drop_piece(&b, 3, 1);
    printf("drop3: %d\n", r2);
    // EXPECT: drop3: 2

    printf("pieces: %d\n", total_pieces(&b));
    // EXPECT: pieces: 3

    // No winner yet
    printf("win1: %d\n", check_four(&b, 1));
    // EXPECT: win1: 0
    printf("win2: %d\n", check_four(&b, 2));
    // EXPECT: win2: 0

    // Test horizontal win
    Board b2;
    board_init(&b2);
    drop_piece(&b2, 0, 1);
    drop_piece(&b2, 0, 2);
    drop_piece(&b2, 1, 1);
    drop_piece(&b2, 1, 2);
    drop_piece(&b2, 2, 1);
    drop_piece(&b2, 2, 2);
    drop_piece(&b2, 3, 1);
    printf("horiz_win: %d\n", check_four(&b2, 1));
    // EXPECT: horiz_win: 1

    // Test vertical win
    Board b3;
    board_init(&b3);
    drop_piece(&b3, 0, 2);
    drop_piece(&b3, 1, 2);
    drop_piece(&b3, 0, 2);
    drop_piece(&b3, 1, 2);
    drop_piece(&b3, 0, 2);
    drop_piece(&b3, 1, 1);
    drop_piece(&b3, 0, 2); // col 0: four 2's
    printf("vert_win: %d\n", check_four(&b3, 2));
    // EXPECT: vert_win: 1

    // Test diagonal win (up-right)
    Board b4;
    board_init(&b4);
    drop_piece(&b4, 0, 1); // (0,0)=1
    drop_piece(&b4, 1, 2); // (0,1)=2
    drop_piece(&b4, 1, 1); // (1,1)=1
    drop_piece(&b4, 2, 2); // (0,2)=2
    drop_piece(&b4, 2, 2); // (1,2)=2
    drop_piece(&b4, 3, 1); // (0,3)=1
    drop_piece(&b4, 2, 1); // (2,2)=1
    drop_piece(&b4, 3, 2); // (1,3)=2
    drop_piece(&b4, 3, 2); // (2,3)=2
    drop_piece(&b4, 4, 2); // (0,4)=2
    drop_piece(&b4, 3, 1); // (3,3)=1
    // Diagonal: (0,0)=1, (1,1)=1, (2,2)=1, (3,3)=1
    printf("diag_win: %d\n", check_four(&b4, 1));
    // EXPECT: diag_win: 1

    // Test column full
    Board b5;
    board_init(&b5);
    int i;
    for (i = 0; i < 6; i++) {
        drop_piece(&b5, 0, (i % 2) + 1);
    }
    int overflow = drop_piece(&b5, 0, 1);
    printf("overflow: %d\n", overflow);
    // EXPECT: overflow: -1

    // Test threats
    Board b6;
    board_init(&b6);
    drop_piece(&b6, 0, 1);
    drop_piece(&b6, 1, 1);
    drop_piece(&b6, 2, 1);
    // Three in a row with empty at col 3 - should be a threat
    int t = count_threats(&b6, 1);
    printf("threats: %d\n", t);
    // EXPECT: threats: 1

    printf("done\n");
    // EXPECT: done
    return 0;
}
