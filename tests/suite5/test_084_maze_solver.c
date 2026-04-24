int printf(const char *fmt, ...);

/* 7x7 maze: 0=wall, 1=path, 2=visited, 3=solution path */
/* Start: (1,1), End: (5,5) */

int maze[7][7];
int qx[50];
int qy[50];
int prev[7][7];

void init_maze(void) {
    int i;
    int j;
    for (i = 0; i < 7; i = i + 1) {
        for (j = 0; j < 7; j = j + 1) {
            maze[i][j] = 0;
            prev[i][j] = -1;
        }
    }
    /* Create paths */
    maze[1][1] = 1; maze[1][2] = 1; maze[1][3] = 1;
    maze[2][1] = 1; maze[2][3] = 1; maze[2][5] = 1;
    maze[3][1] = 1; maze[3][2] = 1; maze[3][3] = 1;
    maze[3][4] = 1; maze[3][5] = 1;
    maze[4][1] = 1; maze[4][3] = 1; maze[4][5] = 1;
    maze[5][1] = 1; maze[5][2] = 1; maze[5][3] = 1;
    maze[5][4] = 1; maze[5][5] = 1;
}

int encode(int r, int c) {
    return r * 7 + c;
}

int decode_r(int code) {
    return code / 7;
}

int decode_c(int code) {
    return code % 7;
}

/* BFS solve: returns 1 if path found, 0 otherwise */
int bfs_solve(int sr, int sc, int er, int ec) {
    int front = 0;
    int back = 0;
    int dr[4];
    int dc[4];
    dr[0] = -1; dr[1] = 1; dr[2] = 0; dr[3] = 0;
    dc[0] = 0; dc[1] = 0; dc[2] = -1; dc[3] = 1;

    qx[back] = sr;
    qy[back] = sc;
    back = back + 1;
    maze[sr][sc] = 2;
    prev[sr][sc] = encode(sr, sc);

    while (front < back) {
        int r = qx[front];
        int c = qy[front];
        front = front + 1;

        if (r == er && c == ec) {
            return 1;
        }

        int d;
        for (d = 0; d < 4; d = d + 1) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (nr >= 0 && nr < 7 && nc >= 0 && nc < 7) {
                if (maze[nr][nc] == 1) {
                    maze[nr][nc] = 2;
                    prev[nr][nc] = encode(r, c);
                    qx[back] = nr;
                    qy[back] = nc;
                    back = back + 1;
                }
            }
        }
    }
    return 0;
}

int mark_path(int sr, int sc, int er, int ec) {
    int steps = 0;
    int r = er;
    int c = ec;
    while (r != sr || c != sc) {
        maze[r][c] = 3;
        int p = prev[r][c];
        r = decode_r(p);
        c = decode_c(p);
        steps = steps + 1;
    }
    maze[sr][sc] = 3;
    return steps;
}

int count_cells(int val) {
    int count = 0;
    int i;
    int j;
    for (i = 0; i < 7; i = i + 1) {
        for (j = 0; j < 7; j = j + 1) {
            if (maze[i][j] == val) {
                count = count + 1;
            }
        }
    }
    return count;
}

/* DFS solve on a fresh maze copy */
int dfs_maze[7][7];
int dfs_found;
int dfs_steps;

void init_dfs_maze(void) {
    int i;
    int j;
    for (i = 0; i < 7; i = i + 1) {
        for (j = 0; j < 7; j = j + 1) {
            dfs_maze[i][j] = maze[i][j];
        }
    }
}

void dfs(int r, int c, int er, int ec, int depth) {
    if (r < 0 || r >= 7 || c < 0 || c >= 7) return;
    if (dfs_maze[r][c] != 1) return;
    if (dfs_found) return;

    dfs_maze[r][c] = 2;

    if (r == er && c == ec) {
        dfs_found = 1;
        dfs_steps = depth;
        return;
    }

    dfs(r - 1, c, er, ec, depth + 1);
    dfs(r + 1, c, er, ec, depth + 1);
    dfs(r, c - 1, er, ec, depth + 1);
    dfs(r, c + 1, er, ec, depth + 1);
}

int main(void) {
    init_maze();

    /* Count open cells */
    // EXPECT: Open cells: 19
    printf("Open cells: %d\n", count_cells(1));
    // EXPECT: Wall cells: 30
    printf("Wall cells: %d\n", count_cells(0));

    /* BFS solve */
    int found = bfs_solve(1, 1, 5, 5);
    // EXPECT: BFS found: 1
    printf("BFS found: %d\n", found);

    /* Count visited (all reachable marked 2) */
    int visited = count_cells(2);
    // EXPECT: BFS visited: 19
    printf("BFS visited: %d\n", visited);

    /* Mark shortest path */
    int steps = mark_path(1, 1, 5, 5);
    // EXPECT: BFS steps: 8
    printf("BFS steps: %d\n", steps);

    /* Count path cells */
    int path_cells = count_cells(3);
    // EXPECT: Path cells: 9
    printf("Path cells: %d\n", path_cells);

    /* Check specific path cells */
    // EXPECT: Start on path: 3
    printf("Start on path: %d\n", maze[1][1]);
    // EXPECT: End on path: 3
    printf("End on path: %d\n", maze[5][5]);
    // EXPECT: Cell 3 3 visited: 2
    printf("Cell 3 3 visited: %d\n", maze[3][3]);

    /* DFS solve */
    init_maze();
    init_dfs_maze();
    dfs_found = 0;
    dfs_steps = 0;
    dfs(1, 1, 5, 5, 0);
    // EXPECT: DFS found: 1
    printf("DFS found: %d\n", dfs_found);
    // EXPECT: DFS steps: 12
    printf("DFS steps: %d\n", dfs_steps);

    /* No-path maze test */
    int i;
    int j;
    for (i = 0; i < 7; i = i + 1) {
        for (j = 0; j < 7; j = j + 1) {
            dfs_maze[i][j] = 0;
        }
    }
    dfs_maze[1][1] = 1;
    dfs_maze[5][5] = 1;
    dfs_found = 0;
    dfs(1, 1, 5, 5, 0);
    // EXPECT: NoPath DFS: 0
    printf("NoPath DFS: %d\n", dfs_found);

    /* BFS on blocked maze - block row 4 entirely */
    init_maze();
    maze[4][1] = 0;
    maze[4][3] = 0;
    maze[4][5] = 0;
    int f2 = bfs_solve(1, 1, 5, 5);
    // EXPECT: Blocked BFS: 0
    printf("Blocked BFS: %d\n", f2);

    // EXPECT: Total grid: 49
    printf("Total grid: %d\n", 7 * 7);

    return 0;
}
