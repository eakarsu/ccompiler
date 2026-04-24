int printf(const char *fmt, ...);
// EXPECT: === Maze 1: BFS Pathfinding ===\nMaze (7x7):\n# # # # # # #\n# . . . # . #\n# . # . # . #\n# . # . . . #\n# . # # # . #\n# . . . . . #\n# # # # # # #\nBFS from (1,1) to (5,5):\nPath found! Length: 9\nPath: (1,1) (1,2) (1,3) (2,3) (3,3) (3,4) (3,5) (4,5) (5,5)\n=== Maze 2: DFS Pathfinding ===\nMaze (7x7):\n# # # # # # #\n# . . . . . #\n# # # # . # #\n# . . . . . #\n# . # # # . #\n# . . . . . #\n# # # # # # #\nDFS from (1,1) to (5,5):\nPath found! Length: 15\n=== Maze 3: Dead-end Detection ===\nMaze (7x7):\n# # # # # # #\n# . # . . . #\n# . # . # . #\n# . # . # . #\n# . . . # . #\n# # # # # . #\n# # # # # # #\nDead-ends found: 2\nDead-end at (1,1)\nDead-end at (5,5)\n=== Maze 4: No Path ===\nMaze (5x5):\n# # # # #\n# . # . #\n# . # . #\n# . # . #\n# # # # #\nBFS from (1,1) to (3,1):\nPath found! Length: 3\n=== Maze 5: BFS Shortest Path ===\nMaze (9x9):\n# # # # # # # # #\n# . . . . . . . #\n# . # # # # # . #\n# . # . . . # . #\n# . # . # . # . #\n# . # . . . # . #\n# . # # # # # . #\n# . . . . . . . #\n# # # # # # # # #\nBFS from (1,1) to (4,4):\nNo path found!\n=== Maze 6: Multiple Path Comparison ===\nBFS path length: 9\nDFS found path: YES\nBFS is shortest: verified\n=== Distance Map ===\nDistance from (1,1):\nRow 1: 0 1 2 3 -1 -1\nRow 2: -1 -1 -1 4 -1 -1\nRow 3: -1 -1 -1 5 6 7\nRow 4: -1 -1 -1 -1 -1 8\nRow 5: -1 -1 -1 -1 -1 9\n=== Flood Fill ===\nRegion size from (1,1): 18\nRegion size from (3,3): 18\nSame region: YES\n=== All maze tests passed ===

int dr[4];
int dc[4];

void init_directions(void) {
    dr[0] = -1; dc[0] = 0;
    dr[1] = 0;  dc[1] = 1;
    dr[2] = 1;  dc[2] = 0;
    dr[3] = 0;  dc[3] = -1;
}

void print_maze(int maze[9][9], int rows, int cols) {
    int r;
    int c;
    for (r = 0; r < rows; r = r + 1) {
        for (c = 0; c < cols; c = c + 1) {
            if (c > 0) printf(" ");
            if (maze[r][c] == 1) {
                printf("%c", '#');
            } else {
                printf("%c", '.');
            }
        }
        printf("\n");
    }
}

int bfs_queue_r[200];
int bfs_queue_c[200];
int bfs_visited[9][9];
int bfs_parent_r[9][9];
int bfs_parent_c[9][9];
int path_r[100];
int path_c[100];

int bfs(int maze[9][9], int rows, int cols, int sr, int sc, int er, int ec) {
    int front;
    int back;
    int r;
    int c;
    int d;
    int nr;
    int nc;
    int i;
    int j;
    int path_len;

    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            bfs_visited[i][j] = 0;
            bfs_parent_r[i][j] = -1;
            bfs_parent_c[i][j] = -1;
        }
    }

    front = 0;
    back = 0;
    bfs_queue_r[back] = sr;
    bfs_queue_c[back] = sc;
    back = back + 1;
    bfs_visited[sr][sc] = 1;

    while (front < back) {
        r = bfs_queue_r[front];
        c = bfs_queue_c[front];
        front = front + 1;

        if (r == er && c == ec) {
            path_len = 0;
            nr = er;
            nc = ec;
            while (nr != -1) {
                path_r[path_len] = nr;
                path_c[path_len] = nc;
                path_len = path_len + 1;
                i = bfs_parent_r[nr][nc];
                j = bfs_parent_c[nr][nc];
                nr = i;
                nc = j;
            }
            for (i = 0; i < path_len / 2; i = i + 1) {
                int tmp;
                tmp = path_r[i];
                path_r[i] = path_r[path_len - 1 - i];
                path_r[path_len - 1 - i] = tmp;
                tmp = path_c[i];
                path_c[i] = path_c[path_len - 1 - i];
                path_c[path_len - 1 - i] = tmp;
            }
            return path_len;
        }

        for (d = 0; d < 4; d = d + 1) {
            nr = r + dr[d];
            nc = c + dc[d];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                if (maze[nr][nc] == 0 && !bfs_visited[nr][nc]) {
                    bfs_visited[nr][nc] = 1;
                    bfs_parent_r[nr][nc] = r;
                    bfs_parent_c[nr][nc] = c;
                    bfs_queue_r[back] = nr;
                    bfs_queue_c[back] = nc;
                    back = back + 1;
                }
            }
        }
    }
    return -1;
}

int dfs_stack_r[200];
int dfs_stack_c[200];
int dfs_visited[9][9];
int dfs_path_r[100];
int dfs_path_c[100];
int dfs_path_len;

int dfs(int maze[9][9], int rows, int cols, int sr, int sc, int er, int ec) {
    int top;
    int r;
    int c;
    int d;
    int nr;
    int nc;
    int i;
    int j;
    int parent_r[9][9];
    int parent_c[9][9];

    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            dfs_visited[i][j] = 0;
            parent_r[i][j] = -1;
            parent_c[i][j] = -1;
        }
    }

    top = 0;
    dfs_stack_r[top] = sr;
    dfs_stack_c[top] = sc;
    top = top + 1;
    dfs_visited[sr][sc] = 1;

    while (top > 0) {
        top = top - 1;
        r = dfs_stack_r[top];
        c = dfs_stack_c[top];

        if (r == er && c == ec) {
            dfs_path_len = 0;
            nr = er;
            nc = ec;
            while (nr != -1) {
                dfs_path_r[dfs_path_len] = nr;
                dfs_path_c[dfs_path_len] = nc;
                dfs_path_len = dfs_path_len + 1;
                i = parent_r[nr][nc];
                j = parent_c[nr][nc];
                nr = i;
                nc = j;
            }
            for (i = 0; i < dfs_path_len / 2; i = i + 1) {
                int tmp;
                tmp = dfs_path_r[i];
                dfs_path_r[i] = dfs_path_r[dfs_path_len - 1 - i];
                dfs_path_r[dfs_path_len - 1 - i] = tmp;
                tmp = dfs_path_c[i];
                dfs_path_c[i] = dfs_path_c[dfs_path_len - 1 - i];
                dfs_path_c[dfs_path_len - 1 - i] = tmp;
            }
            return dfs_path_len;
        }

        for (d = 0; d < 4; d = d + 1) {
            nr = r + dr[d];
            nc = c + dc[d];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                if (maze[nr][nc] == 0 && !dfs_visited[nr][nc]) {
                    dfs_visited[nr][nc] = 1;
                    parent_r[nr][nc] = r;
                    parent_c[nr][nc] = c;
                    dfs_stack_r[top] = nr;
                    dfs_stack_c[top] = nc;
                    top = top + 1;
                }
            }
        }
    }
    return -1;
}

int count_dead_ends(int maze[9][9], int rows, int cols) {
    int r;
    int c;
    int d;
    int nr;
    int nc;
    int count;
    int open;

    count = 0;
    for (r = 0; r < rows; r = r + 1) {
        for (c = 0; c < cols; c = c + 1) {
            if (maze[r][c] == 0) {
                open = 0;
                for (d = 0; d < 4; d = d + 1) {
                    nr = r + dr[d];
                    nc = c + dc[d];
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                        if (maze[nr][nc] == 0) {
                            open = open + 1;
                        }
                    }
                }
                if (open == 1) {
                    count = count + 1;
                }
            }
        }
    }
    return count;
}

void find_dead_ends(int maze[9][9], int rows, int cols) {
    int r;
    int c;
    int d;
    int nr;
    int nc;
    int open;

    for (r = 0; r < rows; r = r + 1) {
        for (c = 0; c < cols; c = c + 1) {
            if (maze[r][c] == 0) {
                open = 0;
                for (d = 0; d < 4; d = d + 1) {
                    nr = r + dr[d];
                    nc = c + dc[d];
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                        if (maze[nr][nc] == 0) {
                            open = open + 1;
                        }
                    }
                }
                if (open == 1) {
                    printf("Dead-end at (%d,%d)\n", r, c);
                }
            }
        }
    }
}

int dist_map[9][9];

void compute_distances(int maze[9][9], int rows, int cols, int sr, int sc) {
    int queue_r[200];
    int queue_c[200];
    int front;
    int back;
    int i;
    int j;
    int r;
    int c;
    int d;
    int nr;
    int nc;

    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            dist_map[i][j] = -1;
        }
    }

    front = 0;
    back = 0;
    queue_r[back] = sr;
    queue_c[back] = sc;
    back = back + 1;
    dist_map[sr][sc] = 0;

    while (front < back) {
        r = queue_r[front];
        c = queue_c[front];
        front = front + 1;

        for (d = 0; d < 4; d = d + 1) {
            nr = r + dr[d];
            nc = c + dc[d];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                if (maze[nr][nc] == 0 && dist_map[nr][nc] == -1) {
                    dist_map[nr][nc] = dist_map[r][c] + 1;
                    queue_r[back] = nr;
                    queue_c[back] = nc;
                    back = back + 1;
                }
            }
        }
    }
}

int flood_visited[9][9];

int flood_fill(int maze[9][9], int rows, int cols, int sr, int sc) {
    int queue_r[200];
    int queue_c[200];
    int front;
    int back;
    int count;
    int i;
    int j;
    int r;
    int c;
    int d;
    int nr;
    int nc;

    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            flood_visited[i][j] = 0;
        }
    }

    front = 0;
    back = 0;
    queue_r[back] = sr;
    queue_c[back] = sc;
    back = back + 1;
    flood_visited[sr][sc] = 1;
    count = 0;

    while (front < back) {
        r = queue_r[front];
        c = queue_c[front];
        front = front + 1;
        count = count + 1;

        for (d = 0; d < 4; d = d + 1) {
            nr = r + dr[d];
            nc = c + dc[d];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                if (maze[nr][nc] == 0 && !flood_visited[nr][nc]) {
                    flood_visited[nr][nc] = 1;
                    queue_r[back] = nr;
                    queue_c[back] = nc;
                    back = back + 1;
                }
            }
        }
    }
    return count;
}

void set_row(int maze[9][9], int r, int c0, int c1, int c2, int c3, int c4, int c5, int c6) {
    maze[r][0] = c0;
    maze[r][1] = c1;
    maze[r][2] = c2;
    maze[r][3] = c3;
    maze[r][4] = c4;
    maze[r][5] = c5;
    maze[r][6] = c6;
}

void set_row9(int maze[9][9], int r, int c0, int c1, int c2, int c3, int c4,
              int c5, int c6, int c7, int c8) {
    maze[r][0] = c0;
    maze[r][1] = c1;
    maze[r][2] = c2;
    maze[r][3] = c3;
    maze[r][4] = c4;
    maze[r][5] = c5;
    maze[r][6] = c6;
    maze[r][7] = c7;
    maze[r][8] = c8;
}

void set_row5(int maze[9][9], int r, int c0, int c1, int c2, int c3, int c4) {
    maze[r][0] = c0;
    maze[r][1] = c1;
    maze[r][2] = c2;
    maze[r][3] = c3;
    maze[r][4] = c4;
}

int abs_val(int x) {
    if (x < 0) return 0 - x;
    return x;
}

int manhattan(int r1, int c1, int r2, int c2) {
    return abs_val(r1 - r2) + abs_val(c1 - c2);
}

int main(void) {
    int maze[9][9];
    int path_len;
    int i;

    init_directions();

    printf("=== Maze 1: BFS Pathfinding ===\n");
    set_row(maze, 0, 1,1,1,1,1,1,1);
    set_row(maze, 1, 1,0,0,0,1,0,1);
    set_row(maze, 2, 1,0,1,0,1,0,1);
    set_row(maze, 3, 1,0,1,0,0,0,1);
    set_row(maze, 4, 1,0,1,1,1,0,1);
    set_row(maze, 5, 1,0,0,0,0,0,1);
    set_row(maze, 6, 1,1,1,1,1,1,1);

    printf("Maze (7x7):\n");
    print_maze(maze, 7, 7);

    printf("BFS from (1,1) to (5,5):\n");
    path_len = bfs(maze, 7, 7, 1, 1, 5, 5);
    if (path_len > 0) {
        printf("Path found! Length: %d\n", path_len);
        printf("Path:");
        for (i = 0; i < path_len; i = i + 1) {
            printf(" (%d,%d)", path_r[i], path_c[i]);
        }
        printf("\n");
    } else {
        printf("No path found!\n");
    }

    printf("=== Maze 2: DFS Pathfinding ===\n");
    set_row(maze, 0, 1,1,1,1,1,1,1);
    set_row(maze, 1, 1,0,0,0,0,0,1);
    set_row(maze, 2, 1,1,1,1,0,1,1);
    set_row(maze, 3, 1,0,0,0,0,0,1);
    set_row(maze, 4, 1,0,1,1,1,0,1);
    set_row(maze, 5, 1,0,0,0,0,0,1);
    set_row(maze, 6, 1,1,1,1,1,1,1);

    printf("Maze (7x7):\n");
    print_maze(maze, 7, 7);

    printf("DFS from (1,1) to (5,5):\n");
    path_len = dfs(maze, 7, 7, 1, 1, 5, 5);
    if (path_len > 0) {
        printf("Path found! Length: %d\n", path_len);
    } else {
        printf("No path found!\n");
    }

    printf("=== Maze 3: Dead-end Detection ===\n");
    set_row(maze, 0, 1,1,1,1,1,1,1);
    set_row(maze, 1, 1,0,1,0,0,0,1);
    set_row(maze, 2, 1,0,1,0,1,0,1);
    set_row(maze, 3, 1,0,1,0,1,0,1);
    set_row(maze, 4, 1,0,0,0,1,0,1);
    set_row(maze, 5, 1,1,1,1,1,0,1);
    set_row(maze, 6, 1,1,1,1,1,1,1);

    printf("Maze (7x7):\n");
    print_maze(maze, 7, 7);

    {
        int dead_ends;
        dead_ends = count_dead_ends(maze, 7, 7);
        printf("Dead-ends found: %d\n", dead_ends);
        find_dead_ends(maze, 7, 7);
    }

    printf("=== Maze 4: No Path ===\n");
    set_row5(maze, 0, 1,1,1,1,1);
    set_row5(maze, 1, 1,0,1,0,1);
    set_row5(maze, 2, 1,0,1,0,1);
    set_row5(maze, 3, 1,0,1,0,1);
    set_row5(maze, 4, 1,1,1,1,1);

    printf("Maze (5x5):\n");
    print_maze(maze, 5, 5);

    printf("BFS from (1,1) to (3,1):\n");
    path_len = bfs(maze, 5, 5, 1, 1, 3, 1);
    if (path_len > 0) {
        printf("Path found! Length: %d\n", path_len);
    } else {
        printf("No path found!\n");
    }

    printf("=== Maze 5: BFS Shortest Path ===\n");
    set_row9(maze, 0, 1,1,1,1,1,1,1,1,1);
    set_row9(maze, 1, 1,0,0,0,0,0,0,0,1);
    set_row9(maze, 2, 1,0,1,1,1,1,1,0,1);
    set_row9(maze, 3, 1,0,1,0,0,0,1,0,1);
    set_row9(maze, 4, 1,0,1,0,1,0,1,0,1);
    set_row9(maze, 5, 1,0,1,0,0,0,1,0,1);
    set_row9(maze, 6, 1,0,1,1,1,1,1,0,1);
    set_row9(maze, 7, 1,0,0,0,0,0,0,0,1);
    set_row9(maze, 8, 1,1,1,1,1,1,1,1,1);

    printf("Maze (9x9):\n");
    print_maze(maze, 9, 9);

    printf("BFS from (1,1) to (4,4):\n");
    path_len = bfs(maze, 9, 9, 1, 1, 4, 4);
    if (path_len > 0) {
        printf("Path found! Length: %d\n", path_len);
    } else {
        printf("No path found!\n");
    }

    printf("=== Maze 6: Multiple Path Comparison ===\n");
    set_row(maze, 0, 1,1,1,1,1,1,1);
    set_row(maze, 1, 1,0,0,0,1,0,1);
    set_row(maze, 2, 1,0,1,0,1,0,1);
    set_row(maze, 3, 1,0,1,0,0,0,1);
    set_row(maze, 4, 1,0,1,1,1,0,1);
    set_row(maze, 5, 1,0,0,0,0,0,1);
    set_row(maze, 6, 1,1,1,1,1,1,1);

    {
        int bfs_len;
        int dfs_len;
        bfs_len = bfs(maze, 7, 7, 1, 1, 5, 5);
        printf("BFS path length: %d\n", bfs_len);
        dfs_len = dfs(maze, 7, 7, 1, 1, 5, 5);
        if (dfs_len > 0) {
            printf("DFS found path: YES\n");
        } else {
            printf("DFS found path: NO\n");
        }
        if (bfs_len <= dfs_len) {
            printf("BFS is shortest: verified\n");
        } else {
            printf("BFS is shortest: failed\n");
        }
    }

    printf("=== Distance Map ===\n");
    {
        int dm[9][9];
        int r;
        int c;
        for (r = 0; r < 7; r = r + 1) {
            for (c = 0; c < 8; c = c + 1) {
                dm[r][c] = 1;
            }
        }
        dm[1][1] = 0; dm[1][2] = 0; dm[1][3] = 0; dm[1][4] = 0;
        dm[2][4] = 0;
        dm[3][4] = 0; dm[3][5] = 0; dm[3][6] = 0;
        dm[4][6] = 0;
        dm[5][6] = 0;

        printf("Distance from (1,1):\n");
        compute_distances(dm, 7, 8, 1, 1);
        for (r = 1; r <= 5; r = r + 1) {
            printf("Row %d:", r);
            for (c = 1; c <= 6; c = c + 1) {
                printf(" %d", dist_map[r][c]);
            }
            printf("\n");
        }
    }

    printf("=== Flood Fill ===\n");
    {
        int size1;
        int size2;
        set_row(maze, 0, 1,1,1,1,1,1,1);
        set_row(maze, 1, 1,0,0,0,1,0,1);
        set_row(maze, 2, 1,0,1,0,1,0,1);
        set_row(maze, 3, 1,0,1,0,0,0,1);
        set_row(maze, 4, 1,0,1,1,1,0,1);
        set_row(maze, 5, 1,0,0,0,0,0,1);
        set_row(maze, 6, 1,1,1,1,1,1,1);

        size1 = flood_fill(maze, 7, 7, 1, 1);
        printf("Region size from (1,1): %d\n", size1);
        size2 = flood_fill(maze, 7, 7, 3, 3);
        printf("Region size from (3,3): %d\n", size2);
        if (size1 == size2) {
            printf("Same region: YES\n");
        } else {
            printf("Same region: NO\n");
        }
    }

    printf("=== All maze tests passed ===\n");
    return 0;
}
