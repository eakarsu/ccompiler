int printf(const char *fmt, ...);

/* Board: 0=empty, 1=X, 2=O */

int check_winner(int *board) {
    /* Check rows */
    int i;
    for (i = 0; i < 3; i = i + 1) {
        if (board[i * 3] != 0 &&
            board[i * 3] == board[i * 3 + 1] &&
            board[i * 3 + 1] == board[i * 3 + 2]) {
            return board[i * 3];
        }
    }
    /* Check cols */
    for (i = 0; i < 3; i = i + 1) {
        if (board[i] != 0 &&
            board[i] == board[i + 3] &&
            board[i + 3] == board[i + 6]) {
            return board[i];
        }
    }
    /* Check diags */
    if (board[0] != 0 && board[0] == board[4] && board[4] == board[8]) {
        return board[0];
    }
    if (board[2] != 0 && board[2] == board[4] && board[4] == board[6]) {
        return board[2];
    }
    return 0;
}

int is_full(int *board) {
    int i;
    for (i = 0; i < 9; i = i + 1) {
        if (board[i] == 0) return 0;
    }
    return 1;
}

int count_moves(int *board) {
    int c = 0;
    int i;
    for (i = 0; i < 9; i = i + 1) {
        if (board[i] != 0) c = c + 1;
    }
    return c;
}

/* Minimax: player 1=X (maximizer), 2=O (minimizer) */
int minimax(int *board, int is_max) {
    int w = check_winner(board);
    if (w == 1) return 10;
    if (w == 2) return -10;
    if (is_full(board)) return 0;

    int i;
    if (is_max) {
        int best = -100;
        for (i = 0; i < 9; i = i + 1) {
            if (board[i] == 0) {
                board[i] = 1;
                int val = minimax(board, 0);
                if (val > best) best = val;
                board[i] = 0;
            }
        }
        return best;
    } else {
        int best = 100;
        for (i = 0; i < 9; i = i + 1) {
            if (board[i] == 0) {
                board[i] = 2;
                int val = minimax(board, 1);
                if (val < best) best = val;
                board[i] = 0;
            }
        }
        return best;
    }
}

int find_best_move(int *board, int player) {
    int best_move = -1;
    int i;
    if (player == 1) {
        int best_val = -100;
        for (i = 0; i < 9; i = i + 1) {
            if (board[i] == 0) {
                board[i] = 1;
                int val = minimax(board, 0);
                if (val > best_val) {
                    best_val = val;
                    best_move = i;
                }
                board[i] = 0;
            }
        }
    } else {
        int best_val = 100;
        for (i = 0; i < 9; i = i + 1) {
            if (board[i] == 0) {
                board[i] = 2;
                int val = minimax(board, 1);
                if (val < best_val) {
                    best_val = val;
                    best_move = i;
                }
                board[i] = 0;
            }
        }
    }
    return best_move;
}

void clear_board(int *board) {
    int i;
    for (i = 0; i < 9; i = i + 1) {
        board[i] = 0;
    }
}

int main(void) {
    int board[9];
    clear_board(board);

    /* Empty board tests */
    // EXPECT: Empty winner: 0
    printf("Empty winner: %d\n", check_winner(board));
    // EXPECT: Empty full: 0
    printf("Empty full: %d\n", is_full(board));
    // EXPECT: Empty moves: 0
    printf("Empty moves: %d\n", count_moves(board));

    /* X wins row */
    board[0] = 1; board[1] = 1; board[2] = 1;
    // EXPECT: X row win: 1
    printf("X row win: %d\n", check_winner(board));
    clear_board(board);

    /* O wins column */
    board[1] = 2; board[4] = 2; board[7] = 2;
    // EXPECT: O col win: 2
    printf("O col win: %d\n", check_winner(board));
    clear_board(board);

    /* X wins diagonal */
    board[0] = 1; board[4] = 1; board[8] = 1;
    // EXPECT: X diag win: 1
    printf("X diag win: %d\n", check_winner(board));
    clear_board(board);

    /* O wins anti-diagonal */
    board[2] = 2; board[4] = 2; board[6] = 2;
    // EXPECT: O anti-diag win: 2
    printf("O anti-diag win: %d\n", check_winner(board));
    clear_board(board);

    /* Draw position */
    board[0] = 1; board[1] = 2; board[2] = 1;
    board[3] = 1; board[4] = 1; board[5] = 2;
    board[6] = 2; board[7] = 1; board[8] = 2;
    // EXPECT: Draw winner: 0
    printf("Draw winner: %d\n", check_winner(board));
    // EXPECT: Draw full: 1
    printf("Draw full: %d\n", is_full(board));
    clear_board(board);

    /* Minimax: empty board eval for X */
    int eval = minimax(board, 1);
    // EXPECT: Empty eval: 0
    printf("Empty eval: %d\n", eval);

    /* Minimax: X about to win */
    board[0] = 1; board[1] = 1; /* X needs pos 2 */
    board[3] = 2; board[4] = 2;
    int move = find_best_move(board, 1);
    // EXPECT: X best move: 2
    printf("X best move: %d\n", move);
    clear_board(board);

    /* O should block X */
    board[0] = 1; board[4] = 1;
    /* Minimax finds optimal O response */
    int o_move = find_best_move(board, 2);
    // EXPECT: O blocks at: 1
    printf("O blocks at: %d\n", o_move);
    clear_board(board);

    /* Center is best first move for X */
    int first = find_best_move(board, 1);
    // EXPECT: First move: 0
    printf("First move: %d\n", first);

    /* Win in one for O */
    board[6] = 2; board[7] = 2;
    board[0] = 1; board[4] = 1;
    int o_win = find_best_move(board, 2);
    // EXPECT: O wins at: 8
    printf("O wins at: %d\n", o_win);
    clear_board(board);

    /* Move count test */
    board[0] = 1; board[4] = 2; board[8] = 1;
    // EXPECT: Moves played: 3
    printf("Moves played: %d\n", count_moves(board));

    return 0;
}
