// EXPECT: === Maze Solver ===
// EXPECT: Maze 1 (5x5):
// EXPECT: S . # # #
// EXPECT: # . # . .
// EXPECT: # . . . #
// EXPECT: # # # . #
// EXPECT: # # # . E
// EXPECT: Maze 1 solved!
// EXPECT: Path length: 9
// EXPECT: S . # # #
// EXPECT: # . # . .
// EXPECT: # . . . #
// EXPECT: # # # . #
// EXPECT: # # # . E
// EXPECT: Maze 2 (6x6):
// EXPECT: S . . # # #
// EXPECT: # # . # . .
// EXPECT: # # . . . #
// EXPECT: # # # # . .
// EXPECT: . . . . . #
// EXPECT: # # # # . E
// EXPECT: Maze 2 solved!
// EXPECT: Path length: 11
// EXPECT: Maze 3 (no solution):
// EXPECT: S # E
// EXPECT: # # #
// EXPECT: # # #
// EXPECT: Maze 3 has no solution (expected)
// EXPECT: All maze solver tests passed

int printf(const char *fmt, ...);

int maze[10][10];
int visited[10][10];
int path_r[100];
int path_c[100];
int path_len;
int maze_rows;
int maze_cols;
int start_r;
int start_c;
int end_r;
int end_c;

void init_maze(int rows, int cols) {
    maze_rows = rows;
    maze_cols = cols;
    path_len = 0;
    int r;
    int c;
    for (r = 0; r < 10; r++)
        for (c = 0; c < 10; c++) {
            maze[r][c] = 1;
            visited[r][c] = 0;
        }
}

void print_maze(void) {
    int r;
    int c;
    for (r = 0; r < maze_rows; r++) {
        for (c = 0; c < maze_cols; c++) {
            if (c > 0) printf(" ");
            if (r == start_r && c == start_c) {
                printf("S");
            } else if (r == end_r && c == end_c) {
                printf("E");
            } else if (maze[r][c] == 1) {
                printf("#");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}

int solve(int r, int c) {
    if (r < 0 || r >= maze_rows || c < 0 || c >= maze_cols) return 0;
    if (maze[r][c] == 1) return 0;
    if (visited[r][c]) return 0;

    visited[r][c] = 1;
    path_r[path_len] = r;
    path_c[path_len] = c;
    path_len++;

    if (r == end_r && c == end_c) return 1;

    if (solve(r + 1, c)) return 1;
    if (solve(r, c + 1)) return 1;
    if (solve(r - 1, c)) return 1;
    if (solve(r, c - 1)) return 1;

    path_len--;
    return 0;
}

void test_maze1(void) {
    init_maze(5, 5);

    maze[0][0] = 0; maze[0][1] = 0;
    maze[1][1] = 0;
    maze[2][1] = 0; maze[2][2] = 0; maze[2][3] = 0;
    maze[1][3] = 0; maze[1][4] = 0;
    maze[3][3] = 0;
    maze[4][3] = 0; maze[4][4] = 0;

    start_r = 0; start_c = 0;
    end_r = 4; end_c = 4;

    printf("Maze 1 (5x5):\n");
    print_maze();

    if (solve(start_r, start_c)) {
        printf("Maze 1 solved!\n");
        printf("Path length: %d\n", path_len);
        print_maze();
    } else {
        printf("Maze 1 has no solution\n");
    }
}

void test_maze2(void) {
    init_maze(6, 6);

    maze[0][0] = 0; maze[0][1] = 0; maze[0][2] = 0;
    maze[1][2] = 0;
    maze[2][2] = 0; maze[2][3] = 0; maze[2][4] = 0;
    maze[1][4] = 0; maze[1][5] = 0;
    maze[3][4] = 0; maze[3][5] = 0;
    maze[4][0] = 0; maze[4][1] = 0; maze[4][2] = 0; maze[4][3] = 0; maze[4][4] = 0;
    maze[5][4] = 0; maze[5][5] = 0;

    start_r = 0; start_c = 0;
    end_r = 5; end_c = 5;

    printf("Maze 2 (6x6):\n");
    print_maze();

    if (solve(start_r, start_c)) {
        printf("Maze 2 solved!\n");
        printf("Path length: %d\n", path_len);
    } else {
        printf("Maze 2 has no solution\n");
    }
}

void test_maze3(void) {
    init_maze(3, 3);

    maze[0][0] = 0;
    maze[0][2] = 0;

    start_r = 0; start_c = 0;
    end_r = 0; end_c = 2;

    printf("Maze 3 (no solution):\n");
    print_maze();

    if (solve(start_r, start_c)) {
        printf("Maze 3 solved (unexpected)\n");
    } else {
        printf("Maze 3 has no solution (expected)\n");
    }
}

int main(void) {
    printf("=== Maze Solver ===\n");
    test_maze1();
    test_maze2();
    test_maze3();
    printf("All maze solver tests passed\n");
    return 0;
}
