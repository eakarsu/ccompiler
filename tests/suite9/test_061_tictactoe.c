int printf(const char *fmt, ...);

// Tic-Tac-Toe with Minimax AI
// Board: 0=empty, 1=X, 2=O

int board[9];

int check_winner(void) {
    int wins[8][3];
    wins[0][0] = 0; wins[0][1] = 1; wins[0][2] = 2;
    wins[1][0] = 3; wins[1][1] = 4; wins[1][2] = 5;
    wins[2][0] = 6; wins[2][1] = 7; wins[2][2] = 8;
    wins[3][0] = 0; wins[3][1] = 3; wins[3][2] = 6;
    wins[4][0] = 1; wins[4][1] = 4; wins[4][2] = 7;
    wins[5][0] = 2; wins[5][1] = 5; wins[5][2] = 8;
    wins[6][0] = 0; wins[6][1] = 4; wins[6][2] = 8;
    wins[7][0] = 2; wins[7][1] = 4; wins[7][2] = 6;
    int i;
    for (i = 0; i < 8; i++) {
        int a = wins[i][0];
        int b = wins[i][1];
        int c = wins[i][2];
        if (board[a] != 0 && board[a] == board[b] && board[b] == board[c]) {
            return board[a];
        }
    }
    return 0;
}

int is_full(void) {
    int i;
    for (i = 0; i < 9; i++) {
        if (board[i] == 0) return 0;
    }
    return 1;
}

int minimax(int is_maximizing) {
    int winner = check_winner();
    if (winner == 1) return 10;
    if (winner == 2) return -10;
    if (is_full()) return 0;

    int i;
    if (is_maximizing) {
        int best = -100;
        for (i = 0; i < 9; i++) {
            if (board[i] == 0) {
                board[i] = 1;
                int score = minimax(0);
                board[i] = 0;
                if (score > best) best = score;
            }
        }
        return best;
    } else {
        int best = 100;
        for (i = 0; i < 9; i++) {
            if (board[i] == 0) {
                board[i] = 2;
                int score = minimax(1);
                board[i] = 0;
                if (score < best) best = score;
            }
        }
        return best;
    }
}

int find_best_move(int player) {
    int best_score;
    int best_move = -1;
    int i;
    if (player == 1) {
        best_score = -100;
        for (i = 0; i < 9; i++) {
            if (board[i] == 0) {
                board[i] = 1;
                int score = minimax(0);
                board[i] = 0;
                if (score > best_score) {
                    best_score = score;
                    best_move = i;
                }
            }
        }
    } else {
        best_score = 100;
        for (i = 0; i < 9; i++) {
            if (board[i] == 0) {
                board[i] = 2;
                int score = minimax(1);
                board[i] = 0;
                if (score < best_score) {
                    best_score = score;
                    best_move = i;
                }
            }
        }
    }
    return best_move;
}

void print_board(void) {
    int i;
    for (i = 0; i < 9; i++) {
        if (board[i] == 0) printf(".");
        else if (board[i] == 1) printf("%s", "X");
        else printf("%s", "O");
        if (i % 3 == 2) printf("%s", "\n");
    }
}

void clear_board(void) {
    int i;
    for (i = 0; i < 9; i++) board[i] = 0;
}

int main(void) {
    int i;
    clear_board();

    // Test 1: X plays optimally from empty board, first move should be corner or center
    int move = find_best_move(1);
    printf("X best first move: %d\n", move);
    // EXPECT: X best first move: 0

    // Test 2: Set up a board where X can win immediately
    clear_board();
    board[0] = 1; board[1] = 1; // X X .
    move = find_best_move(1);
    printf("X winning move: %d\n", move);
    // EXPECT: X winning move: 2

    // Test 3: O must block X from winning
    clear_board();
    board[0] = 1; board[4] = 1; // X at corners, O must block
    board[2] = 2;
    move = find_best_move(1);
    printf("X next move: %d\n", move);
    // EXPECT: X next move: 1

    // Test 4: Check winner detection
    clear_board();
    board[0] = 1; board[1] = 1; board[2] = 1;
    int w = check_winner();
    printf("Winner (row): %d\n", w);
    // EXPECT: Winner (row): 1

    // Test 5: Diagonal win
    clear_board();
    board[0] = 2; board[4] = 2; board[8] = 2;
    w = check_winner();
    printf("Winner (diag): %d\n", w);
    // EXPECT: Winner (diag): 2

    // Test 6: No winner yet
    clear_board();
    board[0] = 1; board[4] = 2;
    w = check_winner();
    printf("No winner: %d\n", w);
    // EXPECT: No winner: 0

    // Test 7: Full board draw scenario detection
    clear_board();
    board[0] = 1; board[1] = 2; board[2] = 1;
    board[3] = 1; board[4] = 1; board[5] = 2;
    board[6] = 2; board[7] = 1; board[8] = 2;
    w = check_winner();
    int full = is_full();
    printf("Draw check: winner=%d full=%d\n", w, full);
    // EXPECT: Draw check: winner=0 full=1

    // Test 8: Print a board state
    clear_board();
    board[0] = 1; board[4] = 2; board[8] = 1;
    printf("Board state:\n");
    // EXPECT: Board state:
    print_board();
    // EXPECT: X..
    // EXPECT: .O.
    // EXPECT: ..X

    // Test 9: O must block X's winning line
    clear_board();
    board[3] = 1; board[4] = 1; // . . . | X X . | . . .
    move = find_best_move(2);
    printf("O blocks at: %d\n", move);
    // EXPECT: O blocks at: 0

    // Test 10: Column win
    clear_board();
    board[1] = 2; board[4] = 2; board[7] = 2;
    w = check_winner();
    printf("Column winner: %d\n", w);
    // EXPECT: Column winner: 2

    printf("Tic-tac-toe tests passed\n");
    // EXPECT: Tic-tac-toe tests passed
    return 0;
}
