int printf(const char *fmt, ...);

// Hill Climbing: optimize a function over a 2D integer grid
// Function: -(x-5)^2 - (y-3)^2 + 50  (maximum at (5,3) = 50)
// Also: multi-start hill climbing and steepest ascent

int eval_func(int x, int y) {
    int dx = x - 5;
    int dy = y - 3;
    return -(dx * dx) - (dy * dy) + 50;
}

// Simple hill climbing: move to first improving neighbor
int hill_climb_simple(int sx, int sy, int *rx, int *ry) {
    int x = sx;
    int y = sy;
    int steps = 0;
    int improved = 1;

    while (improved) {
        improved = 0;
        int cur_val = eval_func(x, y);
        // Try 4 neighbors
        int dx[4];
        int dy[4];
        dx[0] = 1; dy[0] = 0;
        dx[1] = -1; dy[1] = 0;
        dx[2] = 0; dy[2] = 1;
        dx[3] = 0; dy[3] = -1;

        int d = 0;
        while (d < 4) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            if (nx >= 0 && nx <= 10 && ny >= 0 && ny <= 10) {
                int nval = eval_func(nx, ny);
                if (nval > cur_val) {
                    x = nx;
                    y = ny;
                    steps = steps + 1;
                    improved = 1;
                    break;
                }
            }
            d = d + 1;
        }
    }

    *rx = x;
    *ry = y;
    return steps;
}

// Steepest ascent: move to best neighbor
int hill_climb_steepest(int sx, int sy, int *rx, int *ry) {
    int x = sx;
    int y = sy;
    int steps = 0;

    while (1) {
        int cur_val = eval_func(x, y);
        int best_x = x;
        int best_y = y;
        int best_val = cur_val;

        int dx[4];
        int dy[4];
        dx[0] = 1; dy[0] = 0;
        dx[1] = -1; dy[1] = 0;
        dx[2] = 0; dy[2] = 1;
        dx[3] = 0; dy[3] = -1;

        int d = 0;
        while (d < 4) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            if (nx >= 0 && nx <= 10 && ny >= 0 && ny <= 10) {
                int nval = eval_func(nx, ny);
                if (nval > best_val) {
                    best_x = nx;
                    best_y = ny;
                    best_val = nval;
                }
            }
            d = d + 1;
        }

        if (best_x == x && best_y == y) {
            break;
        }
        x = best_x;
        y = best_y;
        steps = steps + 1;
    }

    *rx = x;
    *ry = y;
    return steps;
}

// 8-directional steepest ascent
int hill_climb_8dir(int sx, int sy, int *rx, int *ry) {
    int x = sx;
    int y = sy;
    int steps = 0;

    int dx[8];
    int dy[8];
    dx[0] = 1; dy[0] = 0;
    dx[1] = -1; dy[1] = 0;
    dx[2] = 0; dy[2] = 1;
    dx[3] = 0; dy[3] = -1;
    dx[4] = 1; dy[4] = 1;
    dx[5] = 1; dy[5] = -1;
    dx[6] = -1; dy[6] = 1;
    dx[7] = -1; dy[7] = -1;

    while (1) {
        int cur_val = eval_func(x, y);
        int best_x = x;
        int best_y = y;
        int best_val = cur_val;

        int d = 0;
        while (d < 8) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            if (nx >= 0 && nx <= 10 && ny >= 0 && ny <= 10) {
                int nval = eval_func(nx, ny);
                if (nval > best_val) {
                    best_x = nx;
                    best_y = ny;
                    best_val = nval;
                }
            }
            d = d + 1;
        }

        if (best_x == x && best_y == y) break;
        x = best_x;
        y = best_y;
        steps = steps + 1;
    }

    *rx = x;
    *ry = y;
    return steps;
}

int main() {
    // Test function values
    printf("f(5,3) = %d\n", eval_func(5, 3));
    // EXPECT: f(5,3) = 50
    printf("f(0,0) = %d\n", eval_func(0, 0));
    // EXPECT: f(0,0) = 16
    printf("f(10,10) = %d\n", eval_func(10, 10));
    // EXPECT: f(10,10) = -24

    // Simple hill climbing from (0,0)
    int rx;
    int ry;
    int steps = hill_climb_simple(0, 0, &rx, &ry);
    printf("Simple from (0,0): (%d,%d) val=%d steps=%d\n", rx, ry, eval_func(rx, ry), steps);
    // EXPECT: Simple from (0,0): (5,3) val=50 steps=8

    // Steepest ascent from (0,0)
    steps = hill_climb_steepest(0, 0, &rx, &ry);
    printf("Steepest from (0,0): (%d,%d) val=%d steps=%d\n", rx, ry, eval_func(rx, ry), steps);
    // EXPECT: Steepest from (0,0): (5,3) val=50 steps=8

    // From corner (10,10)
    steps = hill_climb_steepest(10, 10, &rx, &ry);
    printf("Steepest from (10,10): (%d,%d) val=%d steps=%d\n", rx, ry, eval_func(rx, ry), steps);
    // EXPECT: Steepest from (10,10): (5,3) val=50 steps=12

    // 8-directional from (10,10)
    steps = hill_climb_8dir(10, 10, &rx, &ry);
    printf("8-dir from (10,10): (%d,%d) val=%d steps=%d\n", rx, ry, eval_func(rx, ry), steps);
    // EXPECT: 8-dir from (10,10): (5,3) val=50 steps=7

    // Already at optimum
    steps = hill_climb_steepest(5, 3, &rx, &ry);
    printf("From optimum: (%d,%d) steps=%d\n", rx, ry, steps);
    // EXPECT: From optimum: (5,3) steps=0

    // Multi-start: try all corners
    int best_val = -9999;
    int best_x = 0;
    int best_y = 0;
    int corners_x[4];
    int corners_y[4];
    corners_x[0] = 0; corners_y[0] = 0;
    corners_x[1] = 10; corners_y[1] = 0;
    corners_x[2] = 0; corners_y[2] = 10;
    corners_x[3] = 10; corners_y[3] = 10;

    int c = 0;
    while (c < 4) {
        hill_climb_steepest(corners_x[c], corners_y[c], &rx, &ry);
        int val = eval_func(rx, ry);
        if (val > best_val) {
            best_val = val;
            best_x = rx;
            best_y = ry;
        }
        c = c + 1;
    }
    printf("Multi-start best: (%d,%d) val=%d\n", best_x, best_y, best_val);
    // EXPECT: Multi-start best: (5,3) val=50

    printf("Hill climbing done\n");
    // EXPECT: Hill climbing done

    return 0;
}
