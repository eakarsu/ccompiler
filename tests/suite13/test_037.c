int printf(const char *fmt, ...);

// Conway's Game of Life (small grid, few steps)
// Rules:
//   Live cell with 2 or 3 neighbors survives
//   Dead cell with exactly 3 neighbors becomes alive
//   All other cells die or stay dead

int grid[8][8];
int next[8][8];

void clear_grid(int g[8][8]) {
    int i;
    int j;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            g[i][j] = 0;
        }
    }
}

void copy_grid(int src[8][8], int dst[8][8]) {
    int i;
    int j;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            dst[i][j] = src[i][j];
        }
    }
}

int count_neighbors(int g[8][8], int r, int c) {
    int count = 0;
    int dr;
    int dc;
    for (dr = -1; dr <= 1; dr++) {
        for (dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            int nr = r + dr;
            int nc = c + dc;
            if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
                count = count + g[nr][nc];
            }
        }
    }
    return count;
}

void step(int g[8][8], int n[8][8]) {
    int i;
    int j;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            int nb = count_neighbors(g, i, j);
            if (g[i][j] == 1) {
                if (nb == 2 || nb == 3) {
                    n[i][j] = 1;
                } else {
                    n[i][j] = 0;
                }
            } else {
                if (nb == 3) {
                    n[i][j] = 1;
                } else {
                    n[i][j] = 0;
                }
            }
        }
    }
}

int count_alive(int g[8][8]) {
    int count = 0;
    int i;
    int j;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            count = count + g[i][j];
        }
    }
    return count;
}

int grids_equal(int a[8][8], int b[8][8]) {
    int i;
    int j;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (a[i][j] != b[i][j]) return 0;
        }
    }
    return 1;
}

void print_row(int g[8][8], int r, int cols) {
    int j;
    for (j = 0; j < cols; j++) {
        if (j > 0) printf(" ");
        printf("%d", g[r][j]);
    }
    printf("\n");
}

int main() {
    // Test 1: Block (still life) - 2x2 block should not change
    // ..
    // XX
    // XX
    // ..
    clear_grid(grid);
    grid[1][1] = 1; grid[1][2] = 1;
    grid[2][1] = 1; grid[2][2] = 1;
    printf("block alive: %d\n", count_alive(grid));
    // EXPECT: block alive: 4

    step(grid, next);
    copy_grid(next, grid);
    printf("block after 1: %d\n", count_alive(grid));
    // EXPECT: block after 1: 4
    printf("block stable: %d\n", grid[1][1] && grid[1][2] && grid[2][1] && grid[2][2]);
    // EXPECT: block stable: 1

    // Test 2: Blinker (period 2 oscillator)
    // Phase 1 (vertical):   Phase 2 (horizontal):
    //  .X.                   ...
    //  .X.                   XXX
    //  .X.                   ...
    clear_grid(grid);
    grid[1][2] = 1;
    grid[2][2] = 1;
    grid[3][2] = 1;
    printf("blinker start alive: %d\n", count_alive(grid));
    // EXPECT: blinker start alive: 3

    // After 1 step: becomes horizontal
    step(grid, next);
    copy_grid(next, grid);
    printf("blinker step1 row2: %d %d %d\n", grid[2][1], grid[2][2], grid[2][3]);
    // EXPECT: blinker step1 row2: 1 1 1
    printf("blinker step1 col2: %d %d %d\n", grid[1][2], grid[2][2], grid[3][2]);
    // EXPECT: blinker step1 col2: 0 1 0

    // After 2 steps: back to vertical
    step(grid, next);
    copy_grid(next, grid);
    printf("blinker step2 col2: %d %d %d\n", grid[1][2], grid[2][2], grid[3][2]);
    // EXPECT: blinker step2 col2: 1 1 1
    printf("blinker step2 row2: %d %d %d\n", grid[2][1], grid[2][2], grid[2][3]);
    // EXPECT: blinker step2 row2: 0 1 0

    // Test 3: Glider - moves diagonally
    // .X.
    // ..X
    // XXX
    clear_grid(grid);
    grid[0][1] = 1;
    grid[1][2] = 1;
    grid[2][0] = 1; grid[2][1] = 1; grid[2][2] = 1;
    printf("glider start alive: %d\n", count_alive(grid));
    // EXPECT: glider start alive: 5

    // After 4 steps a glider returns to same shape shifted (1,1)
    int s;
    for (s = 0; s < 4; s++) {
        step(grid, next);
        copy_grid(next, grid);
    }
    printf("glider 4 steps alive: %d\n", count_alive(grid));
    // EXPECT: glider 4 steps alive: 5

    // Glider shifted down-right by (1,1):
    // Original: (0,1),(1,2),(2,0),(2,1),(2,2)
    // After 4: (1,2),(2,3),(3,1),(3,2),(3,3)
    printf("glider moved: %d %d %d %d %d\n",
           grid[1][2], grid[2][3], grid[3][1], grid[3][2], grid[3][3]);
    // EXPECT: glider moved: 1 1 1 1 1

    // Test 4: Single cell dies
    clear_grid(grid);
    grid[3][3] = 1;
    step(grid, next);
    copy_grid(next, grid);
    printf("single cell dies: %d\n", count_alive(grid));
    // EXPECT: single cell dies: 0

    // Test 5: Two adjacent cells die
    clear_grid(grid);
    grid[3][3] = 1;
    grid[3][4] = 1;
    step(grid, next);
    copy_grid(next, grid);
    printf("two cells die: %d\n", count_alive(grid));
    // EXPECT: two cells die: 0

    // Test 6: Three cells in L-shape evolve
    // .X
    // XX
    clear_grid(grid);
    grid[2][3] = 1;
    grid[3][2] = 1;
    grid[3][3] = 1;
    step(grid, next);
    copy_grid(next, grid);
    // Each has 2 neighbors; also (2,2) has 3 neighbors -> born
    // (2,3): neighbors = (3,2),(3,3) = 2 -> survive
    // (3,2): neighbors = (2,3),(3,3) = 2 -> survive
    // (3,3): neighbors = (2,3),(3,2) = 2 -> survive
    // (2,2): neighbors = (2,3),(3,2),(3,3) = 3 -> born
    printf("L step1 alive: %d\n", count_alive(grid));
    // EXPECT: L step1 alive: 4
    printf("L became block: %d\n", grid[2][2] && grid[2][3] && grid[3][2] && grid[3][3]);
    // EXPECT: L became block: 1

    return 0;
}
