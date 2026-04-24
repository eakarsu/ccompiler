int printf(const char *fmt, ...);

#define ROWS 5
#define COLS 5
#define RULE30_W 15

/* ---- Game of Life: one step on 5x5 grid ---- */
void gol_step(int g[ROWS][COLS], int ng[ROWS][COLS]) {
    int r, c, dr, dc, neighbors;
    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < COLS; c++) {
            neighbors = 0;
            for (dr = -1; dr <= 1; dr++) {
                for (dc = -1; dc <= 1; dc++) {
                    if (dr == 0 && dc == 0) continue;
                    int nr = r + dr, nc = c + dc;
                    if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS)
                        neighbors += g[nr][nc];
                }
            }
            if (g[r][c])
                ng[r][c] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
            else
                ng[r][c] = (neighbors == 3) ? 1 : 0;
        }
    }
}

void print_grid(int g[ROWS][COLS]) {
    int r, c;
    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < COLS; c++) printf("%d", g[r][c]);
        printf("\n");
    }
}

/* ---- Cellular automaton Rule 30 ---- */
/* Rule 30: bit pattern 00011110 in binary (pattern index = L*4+C*2+R) */
int rule30_next(int left, int center, int right) {
    int pattern = (left << 2) | (center << 1) | right;
    return (30 >> pattern) & 1;
}

void rule30_step(int cells[], int next[], int w) {
    int i;
    next[0] = rule30_next(0, cells[0], cells[1]);
    for (i = 1; i < w - 1; i++)
        next[i] = rule30_next(cells[i-1], cells[i], cells[i+1]);
    next[w-1] = rule30_next(cells[w-2], cells[w-1], 0);
}

void print_cells(int cells[], int w) {
    int i;
    for (i = 0; i < w; i++) printf("%d", cells[i]);
    printf("\n");
}

/* ---- Linear Congruential Generator ----
   x_{n+1} = (1664525 * x_n + 1013904223) mod 2^31
*/
int lcg_next(int seed) {
    return (seed * 1664525 + 1013904223) & 0x7fffffff;
}

/* ---- Monte Carlo-like: count lattice points inside circle ----
   For radius R, count (i,j) with 0<=i<=R, 0<=j<=R, i^2+j^2 <= R^2.
   This approximates pi via: pi ~ 4 * count / (R+1)^2
*/
int count_circle_points(int R) {
    int count = 0, i, j;
    for (i = 0; i <= R; i++)
        for (j = 0; j <= R; j++)
            if (i*i + j*j <= R*R) count++;
    return count;
}

/* ---- Count live cells in a GoL grid ---- */
int count_live(int g[ROWS][COLS]) {
    int count = 0, r, c;
    for (r = 0; r < ROWS; r++)
        for (c = 0; c < COLS; c++)
            count += g[r][c];
    return count;
}

int main(void) {
    int r, c, i;

    /* ---- Game of Life: blinker (horizontal period-2 oscillator) ---- */
    int g[ROWS][COLS];
    int ng[ROWS][COLS];

    /* Initialize to zeros */
    for (r = 0; r < ROWS; r++)
        for (c = 0; c < COLS; c++) g[r][c] = 0;

    /* Place horizontal blinker at row 2, cols 1-3 */
    g[2][1] = 1; g[2][2] = 1; g[2][3] = 1;

    printf("Initial:\n");
    print_grid(g);
    // EXPECT: Initial:
    // EXPECT: 00000
    // EXPECT: 00000
    // EXPECT: 01110
    // EXPECT: 00000
    // EXPECT: 00000

    printf("%d\n", count_live(g));  // EXPECT: 3

    gol_step(g, ng);
    printf("Step1:\n");
    print_grid(ng);
    // EXPECT: Step1:
    // EXPECT: 00000
    // EXPECT: 00100
    // EXPECT: 00100
    // EXPECT: 00100
    // EXPECT: 00000

    printf("%d\n", count_live(ng));  // EXPECT: 3

    /* Step 2: should return to original */
    for (r = 0; r < ROWS; r++)
        for (c = 0; c < COLS; c++) g[r][c] = ng[r][c];
    gol_step(g, ng);
    printf("Step2:\n");
    print_grid(ng);
    // EXPECT: Step2:
    // EXPECT: 00000
    // EXPECT: 00000
    // EXPECT: 01110
    // EXPECT: 00000
    // EXPECT: 00000

    printf("%d\n", count_live(ng));  // EXPECT: 3

    /* ---- Rule 30: 15-cell world, seed at center ---- */
    int cells[RULE30_W], next_cells[RULE30_W];
    for (i = 0; i < RULE30_W; i++) cells[i] = 0;
    cells[7] = 1;

    printf("R30 gen0: "); print_cells(cells, RULE30_W);
    // EXPECT: R30 gen0: 000000010000000

    rule30_step(cells, next_cells, RULE30_W);
    printf("R30 gen1: "); print_cells(next_cells, RULE30_W);
    // EXPECT: R30 gen1: 000000111000000
    for (i = 0; i < RULE30_W; i++) cells[i] = next_cells[i];

    rule30_step(cells, next_cells, RULE30_W);
    printf("R30 gen2: "); print_cells(next_cells, RULE30_W);
    // EXPECT: R30 gen2: 000001100100000
    for (i = 0; i < RULE30_W; i++) cells[i] = next_cells[i];

    rule30_step(cells, next_cells, RULE30_W);
    printf("R30 gen3: "); print_cells(next_cells, RULE30_W);
    // EXPECT: R30 gen3: 000011011110000
    for (i = 0; i < RULE30_W; i++) cells[i] = next_cells[i];

    rule30_step(cells, next_cells, RULE30_W);
    printf("R30 gen4: "); print_cells(next_cells, RULE30_W);
    // EXPECT: R30 gen4: 000110010001000

    /* ---- LCG: first 5 values from seed=42 ---- */
    int seed = 42;
    for (i = 0; i < 5; i++) {
        seed = lcg_next(seed);
        printf("%d\n", seed);
    }
    // EXPECT: 1083814273
    // EXPECT: 378494188
    // EXPECT: 331920219
    // EXPECT: 955863294
    // EXPECT: 1613448261

    /* LCG dice rolls: seed=12345, 6 rolls (mod 6 + 1) */
    seed = 12345;
    for (i = 0; i < 6; i++) {
        seed = lcg_next(seed);
        printf("%d\n", seed % 6 + 1);
    }
    // EXPECT: 3
    // EXPECT: 2
    // EXPECT: 1
    // EXPECT: 6
    // EXPECT: 5
    // EXPECT: 4

    /* ---- Monte Carlo circle: count lattice points in circle R=10 ---- */
    /* Points (i,j) with 0<=i,j<=10 and i^2+j^2 <= 100 */
    int cp = count_circle_points(10);
    printf("%d\n", cp);              // EXPECT: 90
    /* 4 * 90 = 360, (10+1)^2 = 121, ratio*100 = 297 */
    printf("%d\n", 4 * cp);         // EXPECT: 360
    printf("%d\n", 4 * cp * 100 / 121);  // EXPECT: 297

    /* R=20 */
    cp = count_circle_points(20);
    printf("%d\n", cp);              // EXPECT: 335
    printf("%d\n", 4*cp*100/441);   // EXPECT: 303

    /* Sum of 20 LCG values mod 100, seed=42 */
    seed = 42;
    int sum = 0;
    for (i = 0; i < 20; i++) {
        seed = lcg_next(seed);
        sum += seed % 100;
    }
    printf("%d\n", sum);  // EXPECT: 958

    return 0;
}
