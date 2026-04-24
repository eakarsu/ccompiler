int printf(const char *fmt, ...);
// EXPECT: === Chess Board Operations ===\nInitial board:\n8 r n b q k b n r \n7 p p p p p p p p \n6 . . . . . . . . \n5 . . . . . . . . \n4 . . . . . . . . \n3 . . . . . . . . \n2 P P P P P P P P \n1 R N B Q K B N R \n  a b c d e f g h\nPieces: W=16 B=16\nAfter e2-e4:\nAfter e7-e5:\nAfter Bf1-c4:\nAfter Nb8-c6:\n8 r . b q k b n r \n7 p p p p . p p p \n6 . . n . . . . . \n5 . . . . p . . . \n4 . . B . P . . . \n3 . . . . . . . . \n2 P P P P . P P P \n1 R N B Q K . N R \n  a b c d e f g h\nPieces: W=16 B=16\nTotal adjacent threats: 2

enum Piece { EMPTY, PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };
enum Color { NONE, WHITE, BLACK };

struct Square {
    enum Piece piece;
    enum Color color;
};

struct Board {
    struct Square grid[8][8];
    int white_pieces;
    int black_pieces;
};

char piece_char(enum Piece p, enum Color c) {
    if (p == EMPTY) return '.';
    char ch = '.';
    if (p == PAWN) ch = 'P';
    else if (p == ROOK) ch = 'R';
    else if (p == KNIGHT) ch = 'N';
    else if (p == BISHOP) ch = 'B';
    else if (p == QUEEN) ch = 'Q';
    else if (p == KING) ch = 'K';
    if (c == BLACK) ch = ch + 32;
    return ch;
}

void init_board(struct Board *b) {
    int r, c;
    for (r = 0; r < 8; r++) {
        for (c = 0; c < 8; c++) {
            b->grid[r][c].piece = EMPTY;
            b->grid[r][c].color = NONE;
        }
    }

    enum Piece back_row[8];
    back_row[0] = ROOK; back_row[1] = KNIGHT; back_row[2] = BISHOP;
    back_row[3] = QUEEN; back_row[4] = KING; back_row[5] = BISHOP;
    back_row[6] = KNIGHT; back_row[7] = ROOK;

    for (c = 0; c < 8; c++) {
        b->grid[0][c].piece = back_row[c];
        b->grid[0][c].color = WHITE;
        b->grid[1][c].piece = PAWN;
        b->grid[1][c].color = WHITE;
        b->grid[6][c].piece = PAWN;
        b->grid[6][c].color = BLACK;
        b->grid[7][c].piece = back_row[c];
        b->grid[7][c].color = BLACK;
    }
    b->white_pieces = 16;
    b->black_pieces = 16;
}

void print_board(struct Board *b) {
    int r, c;
    for (r = 7; r >= 0; r--) {
        printf("%d ", r + 1);
        for (c = 0; c < 8; c++) {
            printf("%c ", piece_char(b->grid[r][c].piece, b->grid[r][c].color));
        }
        printf("\n");
    }
    printf("  a b c d e f g h\n");
}

int move_piece(struct Board *b, int fr, int fc, int tr, int tc) {
    if (fr < 0 || fr >= 8 || fc < 0 || fc >= 8) return 0;
    if (tr < 0 || tr >= 8 || tc < 0 || tc >= 8) return 0;
    if (b->grid[fr][fc].piece == EMPTY) return 0;

    if (b->grid[tr][tc].piece != EMPTY) {
        if (b->grid[tr][tc].color == WHITE) b->white_pieces--;
        else if (b->grid[tr][tc].color == BLACK) b->black_pieces--;
    }

    b->grid[tr][tc] = b->grid[fr][fc];
    b->grid[fr][fc].piece = EMPTY;
    b->grid[fr][fc].color = NONE;
    return 1;
}

int count_attacks(struct Board *b, int row, int col) {
    int attacks = 0;
    enum Color my_color = b->grid[row][col].color;
    int dr[8];
    int dc[8];
    dr[0]=-1; dr[1]=-1; dr[2]=-1; dr[3]=0; dr[4]=0; dr[5]=1; dr[6]=1; dr[7]=1;
    dc[0]=-1; dc[1]=0;  dc[2]=1;  dc[3]=-1;dc[4]=1; dc[5]=-1;dc[6]=0; dc[7]=1;

    int i;
    for (i = 0; i < 8; i++) {
        int nr = row + dr[i];
        int nc = col + dc[i];
        if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
            if (b->grid[nr][nc].piece != EMPTY && b->grid[nr][nc].color != my_color) {
                attacks++;
            }
        }
    }
    return attacks;
}

int main(void) {
    struct Board board;
    init_board(&board);

    printf("=== Chess Board Operations ===\n");
    printf("Initial board:\n");
    print_board(&board);

    printf("Pieces: W=%d B=%d\n", board.white_pieces, board.black_pieces);

    move_piece(&board, 1, 4, 3, 4);
    printf("After e2-e4:\n");
    move_piece(&board, 6, 4, 4, 4);
    printf("After e7-e5:\n");

    move_piece(&board, 0, 5, 3, 2);
    printf("After Bf1-c4:\n");
    move_piece(&board, 7, 1, 5, 2);
    printf("After Nb8-c6:\n");

    print_board(&board);
    printf("Pieces: W=%d B=%d\n", board.white_pieces, board.black_pieces);

    int total_attacks = 0;
    int r, c;
    for (r = 0; r < 8; r++) {
        for (c = 0; c < 8; c++) {
            if (board.grid[r][c].piece != EMPTY) {
                total_attacks = total_attacks + count_attacks(&board, r, c);
            }
        }
    }
    printf("Total adjacent threats: %d\n", total_attacks);

    return 0;
}
