int printf(const char *fmt, ...);

int grid[8][8];
int next[8][8];

void clear_grid(int g[8][8]) {
    int r;
    int c;
    for (r = 0; r < 8; r = r + 1) {
        for (c = 0; c < 8; c = c + 1) {
            g[r][c] = 0;
        }
    }
}

int count_neighbors(int g[8][8], int row, int col) {
    int count = 0;
    int dr;
    int dc;
    for (dr = -1; dr <= 1; dr = dr + 1) {
        for (dc = -1; dc <= 1; dc = dc + 1) {
            if (dr == 0 && dc == 0) continue;
            int nr = row + dr;
            int nc = col + dc;
            if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
                count = count + g[nr][nc];
            }
        }
    }
    return count;
}

void step(void) {
    int r;
    int c;
    for (r = 0; r < 8; r = r + 1) {
        for (c = 0; c < 8; c = c + 1) {
            int n = count_neighbors(grid, r, c);
            if (grid[r][c] == 1) {
                /* Live cell: survives with 2 or 3 neighbors */
                if (n == 2 || n == 3) {
                    next[r][c] = 1;
                } else {
                    next[r][c] = 0;
                }
            } else {
                /* Dead cell: born with exactly 3 neighbors */
                if (n == 3) {
                    next[r][c] = 1;
                } else {
                    next[r][c] = 0;
                }
            }
        }
    }
    /* Copy next to grid */
    for (r = 0; r < 8; r = r + 1) {
        for (c = 0; c < 8; c = c + 1) {
            grid[r][c] = next[r][c];
        }
    }
}

int count_live(void) {
    int count = 0;
    int r;
    int c;
    for (r = 0; r < 8; r = r + 1) {
        for (c = 0; c < 8; c = c + 1) {
            count = count + grid[r][c];
        }
    }
    return count;
}

int cell_at(int r, int c) {
    return grid[r][c];
}

int main(void) {
    clear_grid(grid);
    clear_grid(next);

    // EXPECT: Initial live: 0
    printf("Initial live: %d\n", count_live());

    /* Set up a blinker (period 2 oscillator) at rows 2-4, col 3 */
    /* Vertical: (2,3), (3,3), (4,3) */
    grid[2][3] = 1;
    grid[3][3] = 1;
    grid[4][3] = 1;

    // EXPECT: Blinker live: 3
    printf("Blinker live: %d\n", count_live());

    /* Check neighbors of center cell (3,3) */
    // EXPECT: Center neighbors: 2
    printf("Center neighbors: %d\n", count_neighbors(grid, 3, 3));

    /* Check neighbors of (3,2) - should be 3 (birth cell) */
    // EXPECT: Side neighbors: 3
    printf("Side neighbors: %d\n", count_neighbors(grid, 3, 2));

    /* Step 1: vertical blinker -> horizontal blinker at row 3, cols 2-4 */
    step();
    // EXPECT: After step 1 live: 3
    printf("After step 1 live: %d\n", count_live());

    /* Verify horizontal orientation */
    // EXPECT: (3,2): 1
    printf("(3,2): %d\n", cell_at(3, 2));
    // EXPECT: (3,3): 1
    printf("(3,3): %d\n", cell_at(3, 3));
    // EXPECT: (3,4): 1
    printf("(3,4): %d\n", cell_at(3, 4));
    // EXPECT: (2,3): 0
    printf("(2,3): %d\n", cell_at(2, 3));
    // EXPECT: (4,3): 0
    printf("(4,3): %d\n", cell_at(4, 3));

    /* Step 2: should return to vertical blinker */
    step();
    // EXPECT: After step 2 live: 3
    printf("After step 2 live: %d\n", count_live());
    // EXPECT: (2,3) back: 1
    printf("(2,3) back: %d\n", cell_at(2, 3));
    // EXPECT: (3,3) back: 1
    printf("(3,3) back: %d\n", cell_at(3, 3));
    // EXPECT: (4,3) back: 1
    printf("(4,3) back: %d\n", cell_at(4, 3));

    /* Now test a block (still life - period 1) */
    clear_grid(grid);
    clear_grid(next);
    grid[1][1] = 1;
    grid[1][2] = 1;
    grid[2][1] = 1;
    grid[2][2] = 1;

    // EXPECT: Block live: 4
    printf("Block live: %d\n", count_live());

    step();
    // EXPECT: Block after step: 4
    printf("Block after step: %d\n", count_live());
    // EXPECT: Block (1,1): 1
    printf("Block (1,1): %d\n", cell_at(1, 1));
    // EXPECT: Block (2,2): 1
    printf("Block (2,2): %d\n", cell_at(2, 2));

    /* Test a glider-like pattern: run for a few steps */
    clear_grid(grid);
    clear_grid(next);
    /* Place an L-triomino */
    grid[0][1] = 1;
    grid[1][2] = 1;
    grid[2][0] = 1;
    grid[2][1] = 1;
    grid[2][2] = 1;

    // EXPECT: Glider start: 5
    printf("Glider start: %d\n", count_live());

    step();
    int gen1 = count_live();
    // EXPECT: Glider gen1: 5
    printf("Glider gen1: %d\n", gen1);

    step();
    int gen2 = count_live();
    // EXPECT: Glider gen2: 5
    printf("Glider gen2: %d\n", gen2);

    step();
    int gen3 = count_live();
    // EXPECT: Glider gen3: 5
    printf("Glider gen3: %d\n", gen3);

    /* Test edge cell neighbor counting */
    clear_grid(grid);
    grid[0][0] = 1;
    grid[0][1] = 1;
    grid[1][0] = 1;
    // EXPECT: Corner neighbors: 2
    printf("Corner neighbors: %d\n", count_neighbors(grid, 0, 0));
    // EXPECT: Edge neighbors (0,1): 2
    printf("Edge neighbors (0,1): %d\n", count_neighbors(grid, 0, 1));

    return 0;
}
