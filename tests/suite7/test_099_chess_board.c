int printf(const char *fmt, ...);

// Chess board: piece placement, move validation, attack detection

enum {
    EMPTY = 0,
    PAWN = 1,
    KNIGHT = 2,
    BISHOP = 3,
    ROOK = 4,
    QUEEN = 5,
    KING = 6
};

enum {
    WHITE = 1,
    BLACK = -1
};

struct Piece {
    int type;
    int color;
};

struct Board {
    struct Piece squares[64];
    int white_king_pos;
    int black_king_pos;
    int move_count;
};

int pos(int row, int col) {
    return row * 8 + col;
}

int get_row(int p) {
    return p / 8;
}

int get_col(int p) {
    return p % 8;
}

int in_bounds(int r, int c) {
    return (r >= 0 && r < 8 && c >= 0 && c < 8) ? 1 : 0;
}

int abs_val(int x) {
    return (x < 0) ? -x : x;
}

void board_init(struct Board *b) {
    int i;
    for (i = 0; i < 64; i++) {
        b->squares[i].type = EMPTY;
        b->squares[i].color = 0;
    }
    b->white_king_pos = -1;
    b->black_king_pos = -1;
    b->move_count = 0;
}

void place_piece(struct Board *b, int row, int col, int type, int color) {
    int p = pos(row, col);
    b->squares[p].type = type;
    b->squares[p].color = color;
    if (type == KING) {
        if (color == WHITE) b->white_king_pos = p;
        else b->black_king_pos = p;
    }
}

int get_piece_type(struct Board *b, int row, int col) {
    return b->squares[pos(row, col)].type;
}

int get_piece_color(struct Board *b, int row, int col) {
    return b->squares[pos(row, col)].color;
}

// Check if a knight at (sr, sc) can attack (tr, tc)
int knight_attacks(int sr, int sc, int tr, int tc) {
    int dr = abs_val(sr - tr);
    int dc = abs_val(sc - tc);
    return ((dr == 2 && dc == 1) || (dr == 1 && dc == 2)) ? 1 : 0;
}

// Check if a rook at (sr, sc) can attack (tr, tc) (ignoring blocking for simplicity)
int rook_can_reach(int sr, int sc, int tr, int tc) {
    return (sr == tr || sc == tc) ? 1 : 0;
}

// Check if a bishop at (sr, sc) can attack (tr, tc) (ignoring blocking)
int bishop_can_reach(int sr, int sc, int tr, int tc) {
    return (abs_val(sr - tr) == abs_val(sc - tc) && sr != tr) ? 1 : 0;
}

// Check if path is clear for rook move (horizontal/vertical)
int rook_path_clear(struct Board *b, int sr, int sc, int tr, int tc) {
    int dr;
    int dc;
    int r;
    int c;
    if (sr == tr) {
        dc = (tc > sc) ? 1 : -1;
        c = sc + dc;
        while (c != tc) {
            if (b->squares[pos(sr, c)].type != EMPTY) return 0;
            c = c + dc;
        }
        return 1;
    }
    if (sc == tc) {
        dr = (tr > sr) ? 1 : -1;
        r = sr + dr;
        while (r != tr) {
            if (b->squares[pos(r, sc)].type != EMPTY) return 0;
            r = r + dr;
        }
        return 1;
    }
    return 0;
}

// Check if path is clear for bishop move (diagonal)
int bishop_path_clear(struct Board *b, int sr, int sc, int tr, int tc) {
    int dr = (tr > sr) ? 1 : -1;
    int dc = (tc > sc) ? 1 : -1;
    int r = sr + dr;
    int c = sc + dc;
    while (r != tr && c != tc) {
        if (b->squares[pos(r, c)].type != EMPTY) return 0;
        r = r + dr;
        c = c + dc;
    }
    return 1;
}

// Validate a move (basic rules, no check validation)
int is_valid_move(struct Board *b, int sr, int sc, int tr, int tc) {
    if (in_bounds(sr, sc) == 0 || in_bounds(tr, tc) == 0) return 0;
    if (sr == tr && sc == tc) return 0;

    struct Piece src = b->squares[pos(sr, sc)];
    struct Piece dst = b->squares[pos(tr, tc)];

    if (src.type == EMPTY) return 0;
    if (dst.type != EMPTY && dst.color == src.color) return 0;

    int dr = tr - sr;
    int dc = tc - sc;

    if (src.type == PAWN) {
        int forward = src.color; // WHITE=1 moves up (increasing row), BLACK=-1 moves down
        if (dc == 0 && dr == forward && dst.type == EMPTY) return 1;
        if (abs_val(dc) == 1 && dr == forward && dst.type != EMPTY) return 1;
        return 0;
    }
    if (src.type == KNIGHT) {
        return knight_attacks(sr, sc, tr, tc);
    }
    if (src.type == BISHOP) {
        if (bishop_can_reach(sr, sc, tr, tc) == 0) return 0;
        return bishop_path_clear(b, sr, sc, tr, tc);
    }
    if (src.type == ROOK) {
        if (rook_can_reach(sr, sc, tr, tc) == 0) return 0;
        return rook_path_clear(b, sr, sc, tr, tc);
    }
    if (src.type == QUEEN) {
        if (rook_can_reach(sr, sc, tr, tc)) return rook_path_clear(b, sr, sc, tr, tc);
        if (bishop_can_reach(sr, sc, tr, tc)) return bishop_path_clear(b, sr, sc, tr, tc);
        return 0;
    }
    if (src.type == KING) {
        return (abs_val(dr) <= 1 && abs_val(dc) <= 1) ? 1 : 0;
    }
    return 0;
}

// Count how many squares a piece at (r,c) attacks
int count_attacks(struct Board *b, int r, int c) {
    int count = 0;
    int tr;
    int tc;
    for (tr = 0; tr < 8; tr++) {
        for (tc = 0; tc < 8; tc++) {
            if (tr == r && tc == c) continue;
            struct Piece src = b->squares[pos(r, c)];
            struct Piece dst = b->squares[pos(tr, tc)];
            // Check if this piece could move there (attack square)
            if (dst.type == EMPTY || dst.color != src.color) {
                if (is_valid_move(b, r, c, tr, tc)) count = count + 1;
            }
        }
    }
    return count;
}

int main(void) {
    struct Board b;
    int valid;

    board_init(&b);
    // EXPECT: Board init: sq0=0
    printf("Board init: sq0=%d\n", b.squares[0].type);

    // Place some pieces
    place_piece(&b, 0, 4, KING, WHITE);
    place_piece(&b, 7, 4, KING, BLACK);
    place_piece(&b, 1, 4, PAWN, WHITE);
    place_piece(&b, 6, 4, PAWN, BLACK);

    // EXPECT: White king at 4
    printf("White king at %d\n", b.white_king_pos);

    // EXPECT: Black king at 60
    printf("Black king at %d\n", b.black_king_pos);

    // EXPECT: Piece at 1,4: type=1 color=1
    printf("Piece at 1,4: type=%d color=%d\n", get_piece_type(&b, 1, 4), get_piece_color(&b, 1, 4));

    // Pawn move validation
    valid = is_valid_move(&b, 1, 4, 2, 4);
    // EXPECT: Pawn forward: valid=1
    printf("Pawn forward: valid=%d\n", valid);

    valid = is_valid_move(&b, 1, 4, 0, 4);
    // EXPECT: Pawn backward: valid=0
    printf("Pawn backward: valid=%d\n", valid);

    valid = is_valid_move(&b, 1, 4, 2, 5);
    // EXPECT: Pawn diag empty: valid=0
    printf("Pawn diag empty: valid=%d\n", valid);

    // Place enemy piece for capture
    place_piece(&b, 2, 5, PAWN, BLACK);
    valid = is_valid_move(&b, 1, 4, 2, 5);
    // EXPECT: Pawn capture: valid=1
    printf("Pawn capture: valid=%d\n", valid);

    // Knight moves
    place_piece(&b, 3, 3, KNIGHT, WHITE);
    valid = is_valid_move(&b, 3, 3, 5, 4);
    // EXPECT: Knight L-shape: valid=1
    printf("Knight L-shape: valid=%d\n", valid);

    valid = is_valid_move(&b, 3, 3, 4, 4);
    // EXPECT: Knight diagonal: valid=0
    printf("Knight diagonal: valid=%d\n", valid);

    {
        int attacks = count_attacks(&b, 3, 3);
        // EXPECT: Knight attacks: 7
        printf("Knight attacks: %d\n", attacks);
    }

    // Rook moves
    board_init(&b);
    place_piece(&b, 3, 3, ROOK, WHITE);
    valid = is_valid_move(&b, 3, 3, 3, 7);
    // EXPECT: Rook horizontal: valid=1
    printf("Rook horizontal: valid=%d\n", valid);

    valid = is_valid_move(&b, 3, 3, 7, 3);
    // EXPECT: Rook vertical: valid=1
    printf("Rook vertical: valid=%d\n", valid);

    valid = is_valid_move(&b, 3, 3, 5, 5);
    // EXPECT: Rook diagonal: valid=0
    printf("Rook diagonal: valid=%d\n", valid);

    // Rook blocked
    place_piece(&b, 3, 5, PAWN, WHITE);
    valid = is_valid_move(&b, 3, 3, 3, 7);
    // EXPECT: Rook blocked: valid=0
    printf("Rook blocked: valid=%d\n", valid);

    // Bishop moves
    board_init(&b);
    place_piece(&b, 3, 3, BISHOP, WHITE);
    valid = is_valid_move(&b, 3, 3, 5, 5);
    // EXPECT: Bishop diag: valid=1
    printf("Bishop diag: valid=%d\n", valid);

    valid = is_valid_move(&b, 3, 3, 3, 5);
    // EXPECT: Bishop horiz: valid=0
    printf("Bishop horiz: valid=%d\n", valid);

    // King moves
    board_init(&b);
    place_piece(&b, 4, 4, KING, WHITE);
    valid = is_valid_move(&b, 4, 4, 5, 5);
    // EXPECT: King one step: valid=1
    printf("King one step: valid=%d\n", valid);

    valid = is_valid_move(&b, 4, 4, 6, 6);
    // EXPECT: King two step: valid=0
    printf("King two step: valid=%d\n", valid);

    // Queen combines rook and bishop
    board_init(&b);
    place_piece(&b, 4, 4, QUEEN, WHITE);
    valid = is_valid_move(&b, 4, 4, 4, 7);
    // EXPECT: Queen horiz: valid=1
    printf("Queen horiz: valid=%d\n", valid);

    valid = is_valid_move(&b, 4, 4, 7, 7);
    // EXPECT: Queen diag: valid=1
    printf("Queen diag: valid=%d\n", valid);

    valid = is_valid_move(&b, 4, 4, 6, 5);
    // EXPECT: Queen knight-like: valid=0
    printf("Queen knight-like: valid=%d\n", valid);

    // EXPECT: Chess sim complete
    printf("Chess sim complete\n");

    return 0;
}
