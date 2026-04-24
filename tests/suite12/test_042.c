int printf(const char *fmt, ...);

// A* Pathfinding with Manhattan heuristic on a grid
// Grid: 0 = open, 1 = wall
// Find shortest path from top-left to bottom-right

int grid[8][8];
int g_cost[64];   // cost from start
int f_cost[64];   // g + heuristic
int came_from[64];
int closed[64];
int open_list[64];
int open_count;

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

int manhattan(int r1, int c1, int r2, int c2) {
    return abs_val(r1 - r2) + abs_val(c1 - c2);
}

void init_astar() {
    int i = 0;
    while (i < 64) {
        g_cost[i] = 9999;
        f_cost[i] = 9999;
        came_from[i] = -1;
        closed[i] = 0;
        open_list[i] = 0;
        i = i + 1;
    }
    open_count = 0;
}

void add_to_open(int idx) {
    open_list[open_count] = idx;
    open_count = open_count + 1;
}

int pop_best_open() {
    int best = -1;
    int best_f = 99999;
    int best_pos = -1;
    int i = 0;
    while (i < open_count) {
        int idx = open_list[i];
        if (f_cost[idx] < best_f) {
            best_f = f_cost[idx];
            best = idx;
            best_pos = i;
        }
        i = i + 1;
    }
    if (best_pos >= 0) {
        open_list[best_pos] = open_list[open_count - 1];
        open_count = open_count - 1;
    }
    return best;
}

int is_in_open(int idx) {
    int i = 0;
    while (i < open_count) {
        if (open_list[i] == idx) return 1;
        i = i + 1;
    }
    return 0;
}

// Neighbor offsets: up, down, left, right
int dr[4];
int dc[4];

int astar(int sr, int sc, int er, int ec) {
    dr[0] = -1; dr[1] = 1; dr[2] = 0; dr[3] = 0;
    dc[0] = 0; dc[1] = 0; dc[2] = -1; dc[3] = 1;

    init_astar();
    int start = sr * 8 + sc;
    int goal = er * 8 + ec;
    g_cost[start] = 0;
    f_cost[start] = manhattan(sr, sc, er, ec);
    add_to_open(start);

    while (open_count > 0) {
        int current = pop_best_open();
        if (current == goal) {
            return g_cost[goal];
        }
        closed[current] = 1;
        int cr = current / 8;
        int cc = current - cr * 8;

        int d = 0;
        while (d < 4) {
            int nr = cr + dr[d];
            int nc = cc + dc[d];
            if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
                int nidx = nr * 8 + nc;
                if (closed[nidx] == 0 && grid[nr][nc] == 0) {
                    int tentative_g = g_cost[current] + 1;
                    if (tentative_g < g_cost[nidx]) {
                        came_from[nidx] = current;
                        g_cost[nidx] = tentative_g;
                        f_cost[nidx] = tentative_g + manhattan(nr, nc, er, ec);
                        if (is_in_open(nidx) == 0) {
                            add_to_open(nidx);
                        }
                    }
                }
            }
            d = d + 1;
        }
    }
    return -1;
}

int path[64];

int reconstruct_path(int goal) {
    int len = 0;
    int cur = goal;
    while (cur != -1) {
        path[len] = cur;
        len = len + 1;
        cur = came_from[cur];
    }
    return len;
}

int main() {
    // Initialize grid to all open
    int r = 0;
    while (r < 8) {
        int c = 0;
        while (c < 8) {
            grid[r][c] = 0;
            c = c + 1;
        }
        r = r + 1;
    }

    // No obstacles: shortest path from (0,0) to (7,7) = 14
    int dist = astar(0, 0, 7, 7);
    printf("Open grid distance: %d\n", dist);
    // EXPECT: Open grid distance: 14

    int plen = reconstruct_path(7 * 8 + 7);
    printf("Path length: %d\n", plen);
    // EXPECT: Path length: 15

    // Add a wall blocking direct path
    grid[0][1] = 1;
    grid[1][1] = 1;
    grid[2][1] = 1;
    grid[3][1] = 1;

    dist = astar(0, 0, 0, 3);
    printf("Wall detour distance: %d\n", dist);
    // EXPECT: Wall detour distance: 11

    // Clear walls and test adjacent cells
    grid[0][1] = 0;
    grid[1][1] = 0;
    grid[2][1] = 0;
    grid[3][1] = 0;

    dist = astar(0, 0, 0, 1);
    printf("Adjacent distance: %d\n", dist);
    // EXPECT: Adjacent distance: 1

    dist = astar(0, 0, 0, 0);
    printf("Same cell distance: %d\n", dist);
    // EXPECT: Same cell distance: 0

    // Test unreachable
    grid[0][1] = 1;
    grid[1][0] = 1;
    dist = astar(0, 0, 7, 7);
    printf("Blocked start: %d\n", dist);
    // EXPECT: Blocked start: -1

    printf("A* done\n");
    // EXPECT: A* done

    return 0;
}
