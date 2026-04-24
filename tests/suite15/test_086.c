int printf(const char *fmt, ...);

/* A* pathfinding on a grid
 * Grid is 8x8, 0 = open, 1 = wall
 * Movement: 4-directional (up, down, left, right)
 * Cost: 1 per step
 * Heuristic: Manhattan distance
 */

int grid[64];  /* 8x8 grid stored flat */

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

/* Manhattan distance heuristic */
int heuristic(int r1, int c1, int r2, int c2) {
    return abs_val(r1 - r2) + abs_val(c1 - c2);
}

/* A* search: returns path length or -1 if no path
 * start_r, start_c: start position
 * end_r, end_c: goal position
 * path_r, path_c: arrays to store the path (max 64)
 * Returns number of steps in path (including start and end)
 */
int astar(int start_r, int start_c, int end_r, int end_c,
          int *path_r, int *path_c) {
    int g[64];       /* cost from start */
    int f[64];       /* g + heuristic */
    int parent[64];  /* parent index for path reconstruction */
    int closed[64];  /* visited */
    int open[64];    /* in open set */
    int i;
    int current;
    int best_f;
    int best_idx;
    int cr;
    int cc;
    int nr;
    int nc;
    int ni;
    int ng;
    int dr[4];
    int dc[4];
    int d;
    int found;
    int path_len;
    int path_stack[64];
    int sp;

    /* Directions: up, down, left, right */
    dr[0] = -1; dc[0] = 0;
    dr[1] = 1;  dc[1] = 0;
    dr[2] = 0;  dc[2] = -1;
    dr[3] = 0;  dc[3] = 1;

    /* Initialize */
    i = 0;
    while (i < 64) {
        g[i] = 9999;
        f[i] = 9999;
        parent[i] = -1;
        closed[i] = 0;
        open[i] = 0;
        i = i + 1;
    }

    current = start_r * 8 + start_c;
    g[current] = 0;
    f[current] = heuristic(start_r, start_c, end_r, end_c);
    open[current] = 1;

    found = 0;

    while (found == 0) {
        /* Find node in open set with lowest f */
        best_f = 99999;
        best_idx = -1;
        i = 0;
        while (i < 64) {
            if (open[i] && f[i] < best_f) {
                best_f = f[i];
                best_idx = i;
            }
            i = i + 1;
        }

        if (best_idx == -1) return -1;  /* No path */

        current = best_idx;
        cr = current / 8;
        cc = current % 8;

        if (cr == end_r && cc == end_c) {
            found = 1;
        } else {
            open[current] = 0;
            closed[current] = 1;

            /* Expand neighbors */
            d = 0;
            while (d < 4) {
                nr = cr + dr[d];
                nc = cc + dc[d];

                if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
                    ni = nr * 8 + nc;
                    if (closed[ni] == 0 && grid[ni] == 0) {
                        ng = g[current] + 1;
                        if (ng < g[ni]) {
                            g[ni] = ng;
                            f[ni] = ng + heuristic(nr, nc, end_r, end_c);
                            parent[ni] = current;
                            open[ni] = 1;
                        }
                    }
                }
                d = d + 1;
            }
        }
    }

    /* Reconstruct path */
    sp = 0;
    current = end_r * 8 + end_c;
    while (current != -1) {
        path_stack[sp] = current;
        sp = sp + 1;
        current = parent[current];
    }

    /* Reverse into path arrays */
    path_len = sp;
    i = 0;
    while (i < sp) {
        path_r[i] = path_stack[sp - 1 - i] / 8;
        path_c[i] = path_stack[sp - 1 - i] % 8;
        i = i + 1;
    }

    return path_len;
}

void clear_grid(void) {
    int i;
    i = 0;
    while (i < 64) {
        grid[i] = 0;
        i = i + 1;
    }
}

int main(void) {
    int path_r[64];
    int path_c[64];
    int len;
    int i;

    printf("A* Pathfinding Tests\n");
    // EXPECT: A* Pathfinding Tests

    /* Test 1: Open grid, corner to corner */
    clear_grid();
    len = astar(0, 0, 7, 7, path_r, path_c);
    printf("T1 len: %d\n", len);
    // EXPECT: T1 len: 15
    printf("T1 start: %d %d\n", path_r[0], path_c[0]);
    // EXPECT: T1 start: 0 0
    printf("T1 end: %d %d\n", path_r[len - 1], path_c[len - 1]);
    // EXPECT: T1 end: 7 7

    /* Test 2: Wall blocking direct path */
    clear_grid();
    /* Vertical wall at column 3, rows 0-5 */
    i = 0;
    while (i < 6) {
        grid[i * 8 + 3] = 1;
        i = i + 1;
    }
    len = astar(0, 0, 0, 7, path_r, path_c);
    printf("T2 len: %d\n", len);
    // EXPECT: T2 len: 20
    printf("T2 start: %d %d\n", path_r[0], path_c[0]);
    // EXPECT: T2 start: 0 0
    printf("T2 end: %d %d\n", path_r[len - 1], path_c[len - 1]);
    // EXPECT: T2 end: 0 7

    /* Test 3: Maze with narrow passage */
    clear_grid();
    /* Horizontal wall at row 3, cols 0-6 (gap at col 7) */
    i = 0;
    while (i < 7) {
        grid[3 * 8 + i] = 1;
        i = i + 1;
    }
    len = astar(0, 0, 7, 0, path_r, path_c);
    printf("T3 len: %d\n", len);
    // EXPECT: T3 len: 22

    /* Test 4: Adjacent cells */
    clear_grid();
    len = astar(3, 3, 3, 4, path_r, path_c);
    printf("T4 len: %d\n", len);
    // EXPECT: T4 len: 2

    /* Test 5: Same start and end */
    clear_grid();
    len = astar(4, 4, 4, 4, path_r, path_c);
    printf("T5 len: %d\n", len);
    // EXPECT: T5 len: 1

    /* Test 6: L-shaped wall */
    clear_grid();
    grid[2 * 8 + 2] = 1;
    grid[2 * 8 + 3] = 1;
    grid[2 * 8 + 4] = 1;
    grid[3 * 8 + 2] = 1;
    grid[4 * 8 + 2] = 1;

    len = astar(3, 3, 1, 1, path_r, path_c);
    printf("T6 len: %d\n", len);
    // EXPECT: T6 len: 9

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
