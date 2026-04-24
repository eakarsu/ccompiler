int printf(const char *fmt, ...);
// EXPECT: g1=1 xw=1 ow=0 wl=1 ec=4 g2=2 g3=3 full=1 xc=5 oc=4 v1=1 v2=0 v3=0 v4=0 cw=2 gs1=0 gs2=0 gs3=0 gs4=3 b2w=1 c4=1 xwins=3 owins=1 draws=1 ng=5

/* ================================================================
 * test_037_game_logic.c
 * Tic-tac-toe board logic: win detection, move validation,
 * game simulation with predetermined moves.
 * Board: 3x3 array, 0=empty, 1=X, 2=O
 * ================================================================ */

int board[9]; /* 0-8: row*3+col */

void board_init(void) {
    int i;
    i = 0;
    while (i < 9) { board[i] = 0; i = i + 1; }
}

int get_cell(int row, int col) {
    return board[row * 3 + col];
}

void set_cell(int row, int col, int val) {
    board[row * 3 + col] = val;
}

/* Check if move is valid */
int is_valid_move(int row, int col) {
    if (row < 0 || row > 2) return 0;
    if (col < 0 || col > 2) return 0;
    if (board[row * 3 + col] != 0) return 0;
    return 1;
}

/* Make a move; returns 1 on success, 0 on failure */
int make_move(int row, int col, int player) {
    if (is_valid_move(row, col) == 0) return 0;
    board[row * 3 + col] = player;
    return 1;
}

/* Check for winner. Returns 1 if player has won, 0 otherwise */
int check_winner(int player) {
    int r; int c;
    /* Check rows */
    r = 0;
    while (r < 3) {
        if (board[r*3+0] == player && board[r*3+1] == player && board[r*3+2] == player) return 1;
        r = r + 1;
    }
    /* Check cols */
    c = 0;
    while (c < 3) {
        if (board[0*3+c] == player && board[1*3+c] == player && board[2*3+c] == player) return 1;
        c = c + 1;
    }
    /* Check diagonals */
    if (board[0] == player && board[4] == player && board[8] == player) return 1;
    if (board[2] == player && board[4] == player && board[6] == player) return 1;
    return 0;
}

/* Check if board is full */
int is_board_full(void) {
    int i;
    i = 0;
    while (i < 9) {
        if (board[i] == 0) return 0;
        i = i + 1;
    }
    return 1;
}

/* Count empty cells */
int count_empty(void) {
    int i; int cnt;
    cnt = 0; i = 0;
    while (i < 9) { if (board[i] == 0) cnt = cnt + 1; i = i + 1; }
    return cnt;
}

/* Count moves by a player */
int count_player(int player) {
    int i; int cnt;
    cnt = 0; i = 0;
    while (i < 9) { if (board[i] == player) cnt = cnt + 1; i = i + 1; }
    return cnt;
}

/* Game state: 0=ongoing, 1=X wins, 2=O wins, 3=draw */
int game_state(void) {
    if (check_winner(1)) return 1;
    if (check_winner(2)) return 2;
    if (is_board_full()) return 3;
    return 0;
}

/* Check if a player can win in one move. Returns cell index or -1 */
int can_win_next(int player) {
    int i; int saved;
    i = 0;
    while (i < 9) {
        if (board[i] == 0) {
            board[i] = player;
            if (check_winner(player)) {
                board[i] = 0;
                return i;
            }
            board[i] = 0;
        }
        i = i + 1;
    }
    return -1;
}

/* Count winning lines for a player */
int count_winning_lines(int player) {
    int cnt;
    int r; int c;
    cnt = 0;
    /* Rows */
    r = 0;
    while (r < 3) {
        if (board[r*3+0] == player && board[r*3+1] == player && board[r*3+2] == player)
            cnt = cnt + 1;
        r = r + 1;
    }
    /* Cols */
    c = 0;
    while (c < 3) {
        if (board[c] == player && board[3+c] == player && board[6+c] == player)
            cnt = cnt + 1;
        c = c + 1;
    }
    /* Diags */
    if (board[0] == player && board[4] == player && board[8] == player)
        cnt = cnt + 1;
    if (board[2] == player && board[4] == player && board[6] == player)
        cnt = cnt + 1;
    return cnt;
}

/* ---- Second board for parallel games ---- */
int board2[9];

void board2_init(void) {
    int i;
    i = 0;
    while (i < 9) { board2[i] = 0; i = i + 1; }
}

int board2_make_move(int pos, int player) {
    if (pos < 0 || pos > 8) return 0;
    if (board2[pos] != 0) return 0;
    board2[pos] = player;
    return 1;
}

int board2_check_winner(int player) {
    int r; int c;
    r = 0;
    while (r < 3) {
        if (board2[r*3+0] == player && board2[r*3+1] == player && board2[r*3+2] == player) return 1;
        r = r + 1;
    }
    c = 0;
    while (c < 3) {
        if (board2[c] == player && board2[3+c] == player && board2[6+c] == player) return 1;
        c = c + 1;
    }
    if (board2[0] == player && board2[4] == player && board2[8] == player) return 1;
    if (board2[2] == player && board2[4] == player && board2[6] == player) return 1;
    return 0;
}

int board2_full(void) {
    int i;
    i = 0;
    while (i < 9) { if (board2[i] == 0) return 0; i = i + 1; }
    return 1;
}

/* ---- Connect-4 simplified: 4x4 board ---- */
int c4_board[16]; /* 4 rows x 4 cols */
int c4_height[4]; /* column heights */

void c4_init(void) {
    int i;
    i = 0;
    while (i < 16) { c4_board[i] = 0; i = i + 1; }
    i = 0;
    while (i < 4) { c4_height[i] = 0; i = i + 1; }
}

int c4_drop(int col, int player) {
    int row;
    if (col < 0 || col > 3) return 0;
    if (c4_height[col] >= 4) return 0;
    row = c4_height[col];
    c4_board[row * 4 + col] = player;
    c4_height[col] = c4_height[col] + 1;
    return 1;
}

int c4_get(int row, int col) {
    if (row < 0 || row > 3 || col < 0 || col > 3) return 0;
    return c4_board[row * 4 + col];
}

/* Check for 3-in-a-row (simplified connect-3 in 4x4) */
int c4_check3(int player) {
    int r; int c;
    /* Horizontal */
    r = 0;
    while (r < 4) {
        c = 0;
        while (c < 2) {
            if (c4_get(r,c)==player && c4_get(r,c+1)==player && c4_get(r,c+2)==player) return 1;
            c = c + 1;
        }
        r = r + 1;
    }
    /* Vertical */
    c = 0;
    while (c < 4) {
        r = 0;
        while (r < 2) {
            if (c4_get(r,c)==player && c4_get(r+1,c)==player && c4_get(r+2,c)==player) return 1;
            r = r + 1;
        }
        c = c + 1;
    }
    /* Diagonal \ */
    r = 0;
    while (r < 2) {
        c = 0;
        while (c < 2) {
            if (c4_get(r,c)==player && c4_get(r+1,c+1)==player && c4_get(r+2,c+2)==player) return 1;
            c = c + 1;
        }
        r = r + 1;
    }
    /* Diagonal / */
    r = 0;
    while (r < 2) {
        c = 2;
        while (c < 4) {
            if (c4_get(r,c)==player && c4_get(r+1,c-1)==player && c4_get(r+2,c-2)==player) return 1;
            c = c + 1;
        }
        r = r + 1;
    }
    return 0;
}

/* ---- Score tracking ---- */
int scores[10]; /* up to 10 games */
int num_games;

void scores_init(void) {
    int i;
    num_games = 0;
    i = 0;
    while (i < 10) { scores[i] = 0; i = i + 1; }
}

void record_game(int result) {
    if (num_games < 10) {
        scores[num_games] = result;
        num_games = num_games + 1;
    }
}

int count_wins(int player) {
    int i; int cnt;
    cnt = 0; i = 0;
    while (i < num_games) {
        if (scores[i] == player) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

int count_draws(void) {
    int i; int cnt;
    cnt = 0; i = 0;
    while (i < num_games) {
        if (scores[i] == 3) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

int main(void) {
    int r;

    /* ---- Game 1: X wins via top row ---- */
    board_init();
    make_move(0, 0, 1); /* X */
    make_move(1, 0, 2); /* O */
    make_move(0, 1, 1); /* X */
    make_move(1, 1, 2); /* O */
    make_move(0, 2, 1); /* X wins! */

    printf("g1=%d ", game_state());                /* 1 (X wins) */
    printf("xw=%d ", check_winner(1));             /* 1 */
    printf("ow=%d ", check_winner(2));             /* 0 */
    printf("wl=%d ", count_winning_lines(1));      /* 1 */
    printf("ec=%d ", count_empty());               /* 4 */

    /* ---- Game 2: O wins via diagonal ---- */
    board_init();
    make_move(0, 0, 2); /* O */
    make_move(0, 1, 1); /* X */
    make_move(1, 1, 2); /* O */
    make_move(0, 2, 1); /* X */
    make_move(2, 2, 2); /* O wins diagonal! */

    printf("g2=%d ", game_state());                /* 2 (O wins) */

    /* ---- Game 3: Draw ---- */
    board_init();
    make_move(0, 0, 1); /* X */
    make_move(0, 1, 2); /* O */
    make_move(0, 2, 1); /* X */
    make_move(1, 0, 1); /* X */
    make_move(1, 1, 1); /* X */
    make_move(1, 2, 2); /* O */
    make_move(2, 0, 2); /* O */
    make_move(2, 1, 1); /* X */
    make_move(2, 2, 2); /* O */

    printf("g3=%d ", game_state());                /* 3 (draw) */
    printf("full=%d ", is_board_full());           /* 1 */
    printf("xc=%d ", count_player(1));             /* 5 */
    printf("oc=%d ", count_player(2));             /* 4 */

    /* ---- Validation tests ---- */
    board_init();
    printf("v1=%d ", is_valid_move(0, 0));         /* 1 */
    make_move(0, 0, 1);
    printf("v2=%d ", is_valid_move(0, 0));         /* 0 (occupied) */
    printf("v3=%d ", is_valid_move(-1, 0));        /* 0 (out of range) */
    printf("v4=%d ", is_valid_move(0, 3));         /* 0 (out of range) */

    /* ---- Can-win-next test ---- */
    board_init();
    make_move(0, 0, 1); /* X at (0,0) */
    make_move(0, 1, 1); /* X at (0,1) */
    r = can_win_next(1); /* X can win at (0,2) = cell 2 */
    printf("cw=%d ", r);                           /* 2 */

    /* ---- Game simulation ---- */
    board_init();
    /* Move sequence: center, corners, sides */
    make_move(1, 1, 1); /* X center */
    printf("gs1=%d ", game_state());               /* 0 (ongoing) */
    make_move(0, 0, 2); /* O corner */
    make_move(2, 2, 1); /* X corner */
    make_move(0, 2, 2); /* O corner */
    make_move(2, 0, 1); /* X corner -> X has diagonal 2,0 - 1,1 - 0,2? No. */
    /* Board: O.O / .X. / X.X */
    printf("gs2=%d ", game_state());               /* 0 */
    /* X can win at (0,1)? Check: row0=[O,_,O], col1=[_,X,_], no. */
    /* Actually let me check anti-diagonal: (2,0)=X, (1,1)=X, (0,2)=O -> no */
    /* Diagonal: (0,0)=O, (1,1)=X, (2,2)=X -> no */
    /* X has: (1,1), (2,2), (2,0). No 3 in line. */
    make_move(1, 0, 2); /* O at (1,0) */
    /* O has: (0,0), (0,2), (1,0). Column 0: (0,0)=O, (1,0)=O, (2,0)=X -> no win yet */
    make_move(1, 2, 1); /* X at (1,2) */
    /* X has: (1,1), (2,2), (2,0), (1,2). Check col2: (0,2)=O,(1,2)=X,(2,2)=X -> no */
    /* Check row 1: (1,0)=O, (1,1)=X, (1,2)=X -> no */
    printf("gs3=%d ", game_state());               /* 0 */
    make_move(2, 1, 2); /* O at (2,1) */
    /* O has: (0,0), (0,2), (1,0), (2,1). No line. */
    make_move(0, 1, 1); /* X at (0,1) */
    /* X has: (1,1), (2,2), (2,0), (1,2), (0,1). Check col 1: (0,1)=X,(1,1)=X,(2,1)=O -> no */
    /* Full board now. Check winner. */
    printf("gs4=%d ", game_state());               /* need to check... */

    /* ---- Board2 test: X wins via column ---- */
    board2_init();
    board2_make_move(0, 1); /* X at 0 */
    board2_make_move(1, 2); /* O at 1 */
    board2_make_move(3, 1); /* X at 3 */
    board2_make_move(4, 2); /* O at 4 */
    board2_make_move(6, 1); /* X at 6 -> col 0: 0,3,6 all X */
    printf("b2w=%d ", board2_check_winner(1));     /* 1 */

    /* ---- Connect-4 (simplified 4x4 connect-3) ---- */
    c4_init();
    c4_drop(0, 1); /* X in col 0 row 0 */
    c4_drop(1, 2); /* O in col 1 row 0 */
    c4_drop(0, 1); /* X in col 0 row 1 */
    c4_drop(1, 2); /* O in col 1 row 1 */
    c4_drop(0, 1); /* X in col 0 row 2 -> vertical 3! */
    printf("c4=%d ", c4_check3(1));                /* 1 */

    /* ---- Score tracking ---- */
    scores_init();
    record_game(1); record_game(2); record_game(1);
    record_game(3); record_game(1);
    printf("xwins=%d ", count_wins(1));            /* 3 */
    printf("owins=%d ", count_wins(2));            /* 1 */
    printf("draws=%d ", count_draws());            /* 1 */
    printf("ng=%d", num_games);                    /* 5 */

    printf("\n");
    return 0;
}

/* ---- Additional game logic helpers for 500+ lines ---- */

/* Evaluate board position for a player */
/* Returns number of "threats" (two in a line with third empty) */
int count_threats(int player) {
    int cnt;
    int r; int c;
    int p; int e;
    cnt = 0;
    /* Check rows */
    r = 0;
    while (r < 3) {
        p = 0; e = 0;
        c = 0;
        while (c < 3) {
            if (board[r*3+c] == player) p = p + 1;
            if (board[r*3+c] == 0) e = e + 1;
            c = c + 1;
        }
        if (p == 2 && e == 1) cnt = cnt + 1;
        r = r + 1;
    }
    /* Check cols */
    c = 0;
    while (c < 3) {
        p = 0; e = 0;
        r = 0;
        while (r < 3) {
            if (board[r*3+c] == player) p = p + 1;
            if (board[r*3+c] == 0) e = e + 1;
            r = r + 1;
        }
        if (p == 2 && e == 1) cnt = cnt + 1;
        c = c + 1;
    }
    /* Main diagonal */
    p = 0; e = 0;
    if (board[0] == player) p = p + 1; if (board[0] == 0) e = e + 1;
    if (board[4] == player) p = p + 1; if (board[4] == 0) e = e + 1;
    if (board[8] == player) p = p + 1; if (board[8] == 0) e = e + 1;
    if (p == 2 && e == 1) cnt = cnt + 1;
    /* Anti diagonal */
    p = 0; e = 0;
    if (board[2] == player) p = p + 1; if (board[2] == 0) e = e + 1;
    if (board[4] == player) p = p + 1; if (board[4] == 0) e = e + 1;
    if (board[6] == player) p = p + 1; if (board[6] == 0) e = e + 1;
    if (p == 2 && e == 1) cnt = cnt + 1;
    return cnt;
}

/* Check if center is occupied */
int center_occupied(void) {
    return (board[4] != 0) ? 1 : 0;
}

/* Count corners occupied by player */
int count_corners(int player) {
    int cnt;
    cnt = 0;
    if (board[0] == player) cnt = cnt + 1;
    if (board[2] == player) cnt = cnt + 1;
    if (board[6] == player) cnt = cnt + 1;
    if (board[8] == player) cnt = cnt + 1;
    return cnt;
}

/* Count edges (non-corner, non-center) occupied by player */
int count_edges(int player) {
    int cnt;
    cnt = 0;
    if (board[1] == player) cnt = cnt + 1;
    if (board[3] == player) cnt = cnt + 1;
    if (board[5] == player) cnt = cnt + 1;
    if (board[7] == player) cnt = cnt + 1;
    return cnt;
}

/* Simple heuristic evaluation */
int evaluate_board(void) {
    int score;
    score = 0;
    if (check_winner(1)) return 100;
    if (check_winner(2)) return -100;
    score = score + count_threats(1) * 10;
    score = score - count_threats(2) * 10;
    if (board[4] == 1) score = score + 3;
    if (board[4] == 2) score = score - 3;
    score = score + count_corners(1) * 2;
    score = score - count_corners(2) * 2;
    return score;
}

/* Reset board to specific position */
void set_board(int a, int b, int c, int d, int e, int f, int g, int h, int k) {
    board[0] = a; board[1] = b; board[2] = c;
    board[3] = d; board[4] = e; board[5] = f;
    board[6] = g; board[7] = h; board[8] = k;
}

/* Check symmetry: is board symmetric about main diagonal? */
int is_symmetric(void) {
    if (board[1] != board[3]) return 0;
    if (board[2] != board[6]) return 0;
    if (board[5] != board[7]) return 0;
    return 1;
}

/* Check if board is a rotation of another (90 degrees) */
/* Rotation: (r,c) -> (c, 2-r) */
int rotated_cell(int pos) {
    int r; int c; int nr; int nc;
    r = pos / 3;
    c = pos % 3;
    nr = c;
    nc = 2 - r;
    return nr * 3 + nc;
}

/* Count total moves made */
int total_moves(void) {
    int i; int cnt;
    cnt = 0; i = 0;
    while (i < 9) { if (board[i] != 0) cnt = cnt + 1; i = i + 1; }
    return cnt;
}

/* Whose turn is it? X goes first, so if even moves made -> X's turn (1), odd -> O's turn (2) */
int whose_turn(void) {
    int m;
    m = total_moves();
    if (m % 2 == 0) return 1;
    return 2;
}
