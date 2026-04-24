int printf(const char *fmt, ...);
// EXPECT: === Maze Generation & Solving ===\nMaze (# = wall, space = path):\n# ######\n#      #\n# ###  #\n#    # #\n#  ### #\n# #    #\n#      #\n###### #\nOpen cells: 30\nSolved: yes\nPath length: 17\nSolution (* = path):\n#*######\n#*     #\n#*###  #\n#*   # #\n#* ### #\n#*#****#\n#******#\n######*#

struct Maze {
    int walls[8][8];
    int visited[8][8];
    int rows;
    int cols;
};

void init_maze(struct Maze *m) {
    m->rows = 8;
    m->cols = 8;
    int r, c;
    for (r = 0; r < 8; r++) {
        for (c = 0; c < 8; c++) {
            m->walls[r][c] = 1;
            m->visited[r][c] = 0;
        }
    }
}

void carve_path(struct Maze *m, int r, int c) {
    if (r < 0 || r >= m->rows || c < 0 || c >= m->cols) return;
    m->walls[r][c] = 0;
}

void generate_simple_maze(struct Maze *m) {
    int r, c;
    for (r = 0; r < 8; r++) {
        for (c = 0; c < 8; c++) {
            if (r == 0 || r == 7 || c == 0 || c == 7) {
                m->walls[r][c] = 1;
            } else {
                m->walls[r][c] = 0;
            }
        }
    }
    m->walls[2][2] = 1; m->walls[2][3] = 1; m->walls[2][4] = 1;
    m->walls[4][3] = 1; m->walls[4][4] = 1; m->walls[4][5] = 1;
    m->walls[3][5] = 1;
    m->walls[5][2] = 1;

    carve_path(m, 0, 1);
    carve_path(m, 7, 6);
}

void print_maze(struct Maze *m) {
    int r, c;
    for (r = 0; r < 8; r++) {
        for (c = 0; c < 8; c++) {
            if (m->visited[r][c] && !m->walls[r][c]) {
                printf("*");
            } else if (m->walls[r][c]) {
                printf("#");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

int solve_maze(struct Maze *m, int r, int c, int tr, int tc) {
    if (r < 0 || r >= m->rows || c < 0 || c >= m->cols) return 0;
    if (m->walls[r][c]) return 0;
    if (m->visited[r][c]) return 0;

    m->visited[r][c] = 1;

    if (r == tr && c == tc) return 1;

    if (solve_maze(m, r - 1, c, tr, tc)) return 1;
    if (solve_maze(m, r + 1, c, tr, tc)) return 1;
    if (solve_maze(m, r, c - 1, tr, tc)) return 1;
    if (solve_maze(m, r, c + 1, tr, tc)) return 1;

    m->visited[r][c] = 0;
    return 0;
}

int count_open(struct Maze *m) {
    int count = 0;
    int r, c;
    for (r = 0; r < m->rows; r++) {
        for (c = 0; c < m->cols; c++) {
            if (!m->walls[r][c]) count++;
        }
    }
    return count;
}

int count_path(struct Maze *m) {
    int count = 0;
    int r, c;
    for (r = 0; r < m->rows; r++) {
        for (c = 0; c < m->cols; c++) {
            if (m->visited[r][c]) count++;
        }
    }
    return count;
}

int main(void) {
    struct Maze maze;
    init_maze(&maze);
    generate_simple_maze(&maze);

    printf("=== Maze Generation & Solving ===\n");
    printf("Maze (# = wall, space = path):\n");
    print_maze(&maze);

    printf("Open cells: %d\n", count_open(&maze));

    int solved = solve_maze(&maze, 0, 1, 7, 6);
    printf("Solved: %s\n", solved ? "yes" : "no");

    if (solved) {
        printf("Path length: %d\n", count_path(&maze));
        printf("Solution (* = path):\n");
        print_maze(&maze);
    }

    return 0;
}
