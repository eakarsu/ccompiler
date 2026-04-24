int printf(const char *fmt, ...);

// Maze solving using recursive backtracking

int maze[8][8];
int visited[8][8];
int sol_path[8][8];
int maze_rows;
int maze_cols;

void init_maze(void) {
    int i;
    int j;
    maze_rows = 5;
    maze_cols = 5;
    for (i = 0; i < maze_rows; i++) {
        for (j = 0; j < maze_cols; j++) {
            maze[i][j] = 0;
            visited[i][j] = 0;
            sol_path[i][j] = 0;
        }
    }
    // Set walls (1 = wall)
    maze[0][2] = 1;
    maze[1][2] = 1;
    maze[2][0] = 1;
    maze[3][1] = 1;
    maze[3][3] = 1;
    maze[4][1] = 1;
}

int is_valid(int r, int c) {
    if (r < 0) return 0;
    if (r >= maze_rows) return 0;
    if (c < 0) return 0;
    if (c >= maze_cols) return 0;
    if (maze[r][c] == 1) return 0;
    if (visited[r][c] == 1) return 0;
    return 1;
}

int dr[4];
int dc[4];

int solve_maze(int r, int c, int target_r, int target_c) {
    int i;
    int nr;
    int nc;
    if (r == target_r && c == target_c) {
        sol_path[r][c] = 1;
        return 1;
    }
    visited[r][c] = 1;
    sol_path[r][c] = 1;
    for (i = 0; i < 4; i++) {
        nr = r + dr[i];
        nc = c + dc[i];
        if (is_valid(nr, nc)) {
            if (solve_maze(nr, nc, target_r, target_c)) {
                return 1;
            }
        }
    }
    sol_path[r][c] = 0;
    return 0;
}

void print_maze(void) {
    int i;
    int j;
    for (i = 0; i < maze_rows; i++) {
        for (j = 0; j < maze_cols; j++) {
            if (sol_path[i][j] == 1) {
                printf("*");
            } else if (maze[i][j] == 1) {
                printf("#");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}

int path_length(void) {
    int count;
    int i;
    int j;
    count = 0;
    for (i = 0; i < maze_rows; i++) {
        for (j = 0; j < maze_cols; j++) {
            if (sol_path[i][j] == 1) count = count + 1;
        }
    }
    return count;
}

int count_reachable(int r, int c) {
    int count;
    int i;
    int nr;
    int nc;
    if (r < 0 || r >= maze_rows || c < 0 || c >= maze_cols) return 0;
    if (maze[r][c] == 1 || visited[r][c] == 1) return 0;
    visited[r][c] = 1;
    count = 1;
    for (i = 0; i < 4; i++) {
        nr = r + dr[i];
        nc = c + dc[i];
        count = count + count_reachable(nr, nc);
    }
    return count;
}

int main() {
    int result;
    int len;
    int i;
    int j;
    int reach;

    // Direction arrays: up, down, left, right
    dr[0] = -1; dr[1] = 1; dr[2] = 0; dr[3] = 0;
    dc[0] = 0; dc[1] = 0; dc[2] = -1; dc[3] = 1;

    // Test 1: solve maze from (0,0) to (4,4)
    init_maze();
    result = solve_maze(0, 0, 4, 4);
    // EXPECT: Maze solved: 1
    printf("Maze solved: %d\n", result);

    len = path_length();
    // EXPECT: Path length: 9
    printf("Path length: %d\n", len);

    // EXPECT: Solution:
    printf("Solution:\n");
    // EXPECT: *.#..
    // EXPECT: **#..
    // EXPECT: #**..
    // EXPECT: .#*#.
    // EXPECT: .#***
    print_maze();

    // Test 2: impossible maze
    init_maze();
    maze[1][0] = 1;
    maze[0][1] = 1;
    for (i = 0; i < maze_rows; i++) {
        for (j = 0; j < maze_cols; j++) {
            visited[i][j] = 0;
            sol_path[i][j] = 0;
        }
    }
    result = solve_maze(0, 0, 4, 4);
    // EXPECT: Blocked maze solved: 0
    printf("Blocked maze solved: %d\n", result);

    // Test 3: count reachable cells
    init_maze();
    for (i = 0; i < maze_rows; i++) {
        for (j = 0; j < maze_cols; j++) {
            visited[i][j] = 0;
        }
    }
    reach = count_reachable(0, 0);
    // EXPECT: Reachable from (0,0): 17
    printf("Reachable from (0,0): %d\n", reach);

    // EXPECT: Maze done
    printf("Maze done\n");

    return 0;
}
