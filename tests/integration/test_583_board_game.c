int printf(const char *fmt, ...);
// EXPECT: Initial board:\nP . . . . \n. # . T . \n. . G # . \n. # T . . \n. . . . G \nScore: 0 Moves: 0\n\nMove right:\n. P . . . \n. # . T . \n. . G # . \n. # T . . \n. . . . G \nScore: 0 Moves: 1\n\nMove down:\n. P . . . \n. # . T . \n. . G # . \n. # T . . \n. . . . G \nScore: 0 Moves: 1\n\nMove right:\n. . P . . \n. # . T . \n. . G # . \n. # T . . \n. . . . G \nScore: 0 Moves: 2\n\nMove down:\n. . . . . \n. # P T . \n. . G # . \n. # T . . \n. . . . G \nScore: 0 Moves: 3\n\nTry move into wall:\nResult: -1 (blocked)\n\nMove right to goal:\n  Trap! -5\n. . . . . \n. # . P . \n. . G # . \n. # T . . \n. . . . G \nScore: -5 Moves: 4\nFinal score: -5 in 4 moves
#define BOARD_SIZE 5

enum CellType {
    CELL_EMPTY = 0,
    CELL_PLAYER = 1,
    CELL_WALL = 2,
    CELL_GOAL = 3,
    CELL_TRAP = 4
};

struct Board {
    int grid[BOARD_SIZE][BOARD_SIZE];
    int player_x;
    int player_y;
    int score;
    int moves;
    int game_over;
};

void board_init(struct Board *b) {
    int i;
    int j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            b->grid[i][j] = CELL_EMPTY;
        }
    }
    b->player_x = 0;
    b->player_y = 0;
    b->grid[0][0] = CELL_PLAYER;
    b->score = 0;
    b->moves = 0;
    b->game_over = 0;

    b->grid[1][1] = CELL_WALL;
    b->grid[2][3] = CELL_WALL;
    b->grid[3][1] = CELL_WALL;
    b->grid[1][3] = CELL_TRAP;
    b->grid[3][2] = CELL_TRAP;
    b->grid[2][2] = CELL_GOAL;
    b->grid[4][4] = CELL_GOAL;
}

char cell_char(int type) {
    if (type == CELL_EMPTY) return '.';
    if (type == CELL_PLAYER) return 'P';
    if (type == CELL_WALL) return '#';
    if (type == CELL_GOAL) return 'G';
    if (type == CELL_TRAP) return 'T';
    return '?';
}

void board_print(struct Board *b) {
    int i;
    int j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", cell_char(b->grid[i][j]));
        }
        printf("\n");
    }
    printf("Score: %d Moves: %d\n", b->score, b->moves);
}

int board_move(struct Board *b, int dx, int dy) {
    if (b->game_over) return -1;
    int nx = b->player_x + dx;
    int ny = b->player_y + dy;
    if (nx < 0 || nx >= BOARD_SIZE || ny < 0 || ny >= BOARD_SIZE) return -1;
    if (b->grid[ny][nx] == CELL_WALL) return -1;

    int target = b->grid[ny][nx];
    b->grid[b->player_y][b->player_x] = CELL_EMPTY;
    b->player_x = nx;
    b->player_y = ny;
    b->grid[ny][nx] = CELL_PLAYER;
    b->moves++;

    if (target == CELL_GOAL) {
        b->score = b->score + 10;
        printf("  Goal reached! +10\n");
    } else if (target == CELL_TRAP) {
        b->score = b->score - 5;
        printf("  Trap! -5\n");
    }
    return 0;
}

int main(void) {
    struct Board board;
    board_init(&board);
    printf("Initial board:\n");
    board_print(&board);

    printf("\nMove right:\n");
    board_move(&board, 1, 0);
    board_print(&board);

    printf("\nMove down:\n");
    board_move(&board, 0, 1);
    board_print(&board);

    printf("\nMove right:\n");
    board_move(&board, 1, 0);
    board_print(&board);

    printf("\nMove down:\n");
    board_move(&board, 0, 1);
    board_print(&board);

    printf("\nTry move into wall:\n");
    int result = board_move(&board, -1, 0);
    printf("Result: %d (blocked)\n", result);

    printf("\nMove right to goal:\n");
    board_move(&board, 1, 0);
    board_print(&board);

    printf("Final score: %d in %d moves\n", board.score, board.moves);

    return 0;
}
