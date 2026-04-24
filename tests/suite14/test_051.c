int printf(const char *fmt, ...);

// Tic-tac-toe board evaluation and minimax
// Board: 0=empty, 1=X, 2=O

int check_winner(int *board) {
    // Check rows
    int i;
    for (i = 0; i < 3; i++) {
        if (board[i * 3] != 0 && board[i * 3] == board[i * 3 + 1] && board[i * 3 + 1] == board[i * 3 + 2])
            return board[i * 3];
    }
    // Check cols
    for (i = 0; i < 3; i++) {
        if (board[i] != 0 && board[i] == board[i + 3] && board[i + 3] == board[i + 6])
            return board[i];
    }
    // Check diagonals
    if (board[0] != 0 && board[0] == board[4] && board[4] == board[8])
        return board[0];
    if (board[2] != 0 && board[2] == board[4] && board[4] == board[6])
        return board[2];
    return 0;
}

int is_full(int *board) {
    int i;
    for (i = 0; i < 9; i++) {
        if (board[i] == 0) return 0;
    }
    return 1;
}

int minimax(int *board, int is_maximizing) {
    int winner = check_winner(board);
    if (winner == 1) return 10;
    if (winner == 2) return -10;
    if (is_full(board)) return 0;

    int best;
    int i;
    if (is_maximizing) {
        best = -100;
        for (i = 0; i < 9; i++) {
            if (board[i] == 0) {
                board[i] = 1;
                int score = minimax(board, 0);
                board[i] = 0;
                if (score > best) best = score;
            }
        }
    } else {
        best = 100;
        for (i = 0; i < 9; i++) {
            if (board[i] == 0) {
                board[i] = 2;
                int score = minimax(board, 1);
                board[i] = 0;
                if (score < best) best = score;
            }
        }
    }
    return best;
}

int find_best_move(int *board) {
    int best_score = -100;
    int best_move = -1;
    int i;
    for (i = 0; i < 9; i++) {
        if (board[i] == 0) {
            board[i] = 1;
            int score = minimax(board, 0);
            board[i] = 0;
            if (score > best_score) {
                best_score = score;
                best_move = i;
            }
        }
    }
    return best_move;
}

void print_board(int *board) {
    int i;
    for (i = 0; i < 9; i++) {
        if (board[i] == 0) printf(".");
        else if (board[i] == 1) printf("X");
        else printf("O");
        if (i % 3 == 2) printf("\n");
    }
}

int main() {
    // Test 1: X wins horizontally
    int b1[9];
    b1[0] = 1; b1[1] = 1; b1[2] = 1;
    b1[3] = 2; b1[4] = 2; b1[5] = 0;
    b1[6] = 0; b1[7] = 0; b1[8] = 0;
    printf("winner1: %d\n", check_winner(b1));
    // EXPECT: winner1: 1

    // Test 2: O wins vertically
    int b2[9];
    b2[0] = 1; b2[1] = 2; b2[2] = 0;
    b2[3] = 0; b2[4] = 2; b2[5] = 1;
    b2[6] = 1; b2[7] = 2; b2[8] = 0;
    printf("winner2: %d\n", check_winner(b2));
    // EXPECT: winner2: 2

    // Test 3: X wins diagonally
    int b3[9];
    b3[0] = 1; b3[1] = 2; b3[2] = 0;
    b3[3] = 2; b3[4] = 1; b3[5] = 0;
    b3[6] = 0; b3[7] = 0; b3[8] = 1;
    printf("winner3: %d\n", check_winner(b3));
    // EXPECT: winner3: 1

    // Test 4: No winner
    int b4[9];
    b4[0] = 1; b4[1] = 2; b4[2] = 1;
    b4[3] = 0; b4[4] = 0; b4[5] = 0;
    b4[6] = 0; b4[7] = 0; b4[8] = 0;
    printf("winner4: %d\n", check_winner(b4));
    // EXPECT: winner4: 0

    // Test 5: Draw
    int b5[9];
    b5[0] = 1; b5[1] = 2; b5[2] = 1;
    b5[3] = 1; b5[4] = 1; b5[5] = 2;
    b5[6] = 2; b5[7] = 1; b5[8] = 2;
    printf("winner5: %d\n", check_winner(b5));
    // EXPECT: winner5: 0
    printf("full5: %d\n", is_full(b5));
    // EXPECT: full5: 1

    // Test 6: Best move - X should block O from winning
    int b6[9];
    b6[0] = 1; b6[1] = 0; b6[2] = 0;
    b6[3] = 0; b6[4] = 2; b6[5] = 0;
    b6[6] = 0; b6[7] = 0; b6[8] = 2;
    // X at 0, O at 4,8. X should take a good position.
    int move6 = find_best_move(b6);
    // Minimax should find best move for X
    printf("best_move6: %d\n", move6);
    // EXPECT: best_move6: 2

    // Test 7: X can win immediately
    int b7[9];
    b7[0] = 1; b7[1] = 1; b7[2] = 0;
    b7[3] = 2; b7[4] = 2; b7[5] = 0;
    b7[6] = 0; b7[7] = 0; b7[8] = 0;
    int move7 = find_best_move(b7);
    printf("best_move7: %d\n", move7);
    // EXPECT: best_move7: 2

    // Test 8: Minimax score for empty board (should be 0 for draw with perfect play)
    int b8[9];
    int j;
    for (j = 0; j < 9; j++) b8[j] = 0;
    int score8 = minimax(b8, 1);
    printf("minimax_empty: %d\n", score8);
    // EXPECT: minimax_empty: 0

    // Test 9: O wins anti-diagonal
    int b9[9];
    b9[0] = 1; b9[1] = 1; b9[2] = 2;
    b9[3] = 0; b9[4] = 2; b9[5] = 1;
    b9[6] = 2; b9[7] = 0; b9[8] = 0;
    printf("winner9: %d\n", check_winner(b9));
    // EXPECT: winner9: 2

    // Test 10: Full board detection
    int b10[9];
    b10[0] = 1; b10[1] = 2; b10[2] = 1;
    b10[3] = 2; b10[4] = 1; b10[5] = 2;
    b10[6] = 2; b10[7] = 1; b10[8] = 2;
    printf("full10: %d\n", is_full(b10));
    // EXPECT: full10: 1

    printf("done\n");
    // EXPECT: done
    return 0;
}
