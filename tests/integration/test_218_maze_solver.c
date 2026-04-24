int printf(const char *fmt, ...);
// EXPECT: === maze ===\n ##  #  \n # # #  \n   # # #\n ### #  \n     #  \n ###    \n   # ## \n #   #  \nsolved: 1\npath length: 19\npath: (0,0)->(1,0)->(2,0)->(3,0)->(4,0)->(5,0)->(6,0)->(6,1)->(6,2)->(7,2)->(7,3)->(7,4)->(6,4)->(5,4)->(5,5)->(5,6)->(5,7)->(6,7)->(7,7)\n=== visited maze ===\n.##  #  \n.# # #  \n.  # # #\n.### #  \n.    #  \n.###....\n...#.##.\n.#...# .\ncells visited: 20
// Test 218: Recursive maze solver

int maze[8][8];
int visited[8][8];
int path_r[64];
int path_c[64];
int path_len;
int solved;

void init_maze(void) {
    int i = 0;
    while (i < 8) {
        int j = 0;
        while (j < 8) {
            maze[i][j] = 0;
            visited[i][j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }
    maze[0][1] = 1; maze[0][2] = 1; maze[0][5] = 1;
    maze[1][1] = 1; maze[1][3] = 1; maze[1][5] = 1;
    maze[2][3] = 1; maze[2][5] = 1; maze[2][7] = 1;
    maze[3][1] = 1; maze[3][2] = 1; maze[3][3] = 1; maze[3][5] = 1;
    maze[4][5] = 1;
    maze[5][1] = 1; maze[5][2] = 1; maze[5][3] = 1;
    maze[6][3] = 1; maze[6][5] = 1; maze[6][6] = 1;
    maze[7][1] = 1; maze[7][5] = 1;
}

void print_maze(void) {
    int i = 0;
    while (i < 8) {
        int j = 0;
        while (j < 8) {
            if (maze[i][j] == 1) printf("#");
            else if (visited[i][j]) printf(".");
            else printf(" ");
            j = j + 1;
        }
        printf("\n");
        i = i + 1;
    }
}

int solve(int r, int c, int goal_r, int goal_c) {
    if (r < 0 || r >= 8 || c < 0 || c >= 8) return 0;
    if (maze[r][c] == 1) return 0;
    if (visited[r][c]) return 0;

    visited[r][c] = 1;
    path_r[path_len] = r;
    path_c[path_len] = c;
    path_len = path_len + 1;

    if (r == goal_r && c == goal_c) return 1;

    if (solve(r + 1, c, goal_r, goal_c)) return 1;
    if (solve(r, c + 1, goal_r, goal_c)) return 1;
    if (solve(r - 1, c, goal_r, goal_c)) return 1;
    if (solve(r, c - 1, goal_r, goal_c)) return 1;

    path_len = path_len - 1;
    return 0;
}

int main(void) {
    init_maze();
    printf("=== maze ===\n");
    print_maze();

    path_len = 0;
    solved = solve(0, 0, 7, 7);
    printf("solved: %d\n", solved);
    printf("path length: %d\n", path_len);
    printf("path: ");
    int i = 0;
    while (i < path_len) {
        printf("(%d,%d)", path_r[i], path_c[i]);
        if (i < path_len - 1) printf("->");
        i = i + 1;
    }
    printf("\n");

    printf("=== visited maze ===\n");
    print_maze();

    int count = 0;
    i = 0;
    while (i < 8) {
        int j = 0;
        while (j < 8) {
            if (visited[i][j]) count = count + 1;
            j = j + 1;
        }
        i = i + 1;
    }
    printf("cells visited: %d\n", count);

    return 0;
}
