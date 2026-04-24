int printf(const char *fmt, ...);

struct Grid {
    int cells[8][8];
    int width;
    int height;
    int generation;
};

void clear_grid(struct Grid *g) {
    int r;
    int c;
    g->width = 8;
    g->height = 8;
    g->generation = 0;
    for (r = 0; r < 8; r = r + 1) {
        for (c = 0; c < 8; c = c + 1) {
            g->cells[r][c] = 0;
        }
    }
}

void set_cell(struct Grid *g, int r, int c, int val) {
    if (r >= 0 && r < g->height && c >= 0 && c < g->width) {
        g->cells[r][c] = val;
    }
}

int get_cell(struct Grid *g, int r, int c) {
    if (r < 0 || r >= g->height || c < 0 || c >= g->width) return 0;
    return g->cells[r][c];
}

int count_neighbors(struct Grid *g, int r, int c) {
    int count = 0;
    int dr;
    int dc;
    for (dr = -1; dr <= 1; dr = dr + 1) {
        for (dc = -1; dc <= 1; dc = dc + 1) {
            if (dr == 0 && dc == 0) continue;
            count = count + get_cell(g, r + dr, c + dc);
        }
    }
    return count;
}

int count_alive(struct Grid *g) {
    int total = 0;
    int r;
    int c;
    for (r = 0; r < g->height; r = r + 1) {
        for (c = 0; c < g->width; c = c + 1) {
            total = total + g->cells[r][c];
        }
    }
    return total;
}

void step_generation(struct Grid *g) {
    int next[8][8];
    int r;
    int c;
    for (r = 0; r < g->height; r = r + 1) {
        for (c = 0; c < g->width; c = c + 1) {
            int n = count_neighbors(g, r, c);
            int alive = g->cells[r][c];
            if (alive) {
                if (n == 2 || n == 3) next[r][c] = 1;
                else next[r][c] = 0;
            } else {
                if (n == 3) next[r][c] = 1;
                else next[r][c] = 0;
            }
        }
    }
    for (r = 0; r < g->height; r = r + 1) {
        for (c = 0; c < g->width; c = c + 1) {
            g->cells[r][c] = next[r][c];
        }
    }
    g->generation = g->generation + 1;
}

void print_row_status(struct Grid *g, int r) {
    int c;
    int alive = 0;
    for (c = 0; c < g->width; c = c + 1) {
        alive = alive + g->cells[r][c];
    }
    printf("Row %d: %d alive\n", r, alive);
}

void setup_blinker(struct Grid *g, int r, int c) {
    set_cell(g, r, c - 1, 1);
    set_cell(g, r, c, 1);
    set_cell(g, r, c + 1, 1);
}

void setup_glider(struct Grid *g, int r, int c) {
    set_cell(g, r, c + 1, 1);
    set_cell(g, r + 1, c + 2, 1);
    set_cell(g, r + 2, c, 1);
    set_cell(g, r + 2, c + 1, 1);
    set_cell(g, r + 2, c + 2, 1);
}

void setup_block(struct Grid *g, int r, int c) {
    set_cell(g, r, c, 1);
    set_cell(g, r, c + 1, 1);
    set_cell(g, r + 1, c, 1);
    set_cell(g, r + 1, c + 1, 1);
}

int main() {
    struct Grid grid;

    printf("=== Blinker Test ===\n");
    // EXPECT: === Blinker Test ===
    clear_grid(&grid);
    setup_blinker(&grid, 3, 3);
    printf("Gen %d: alive=%d\n", grid.generation, count_alive(&grid));
    // EXPECT: Gen 0: alive=3
    step_generation(&grid);
    printf("Gen %d: alive=%d\n", grid.generation, count_alive(&grid));
    // EXPECT: Gen 1: alive=3
    printf("Cell(2,3)=%d Cell(3,3)=%d Cell(4,3)=%d\n",
           get_cell(&grid, 2, 3), get_cell(&grid, 3, 3), get_cell(&grid, 4, 3));
    // EXPECT: Cell(2,3)=1 Cell(3,3)=1 Cell(4,3)=1
    printf("Cell(3,2)=%d Cell(3,4)=%d\n",
           get_cell(&grid, 3, 2), get_cell(&grid, 3, 4));
    // EXPECT: Cell(3,2)=0 Cell(3,4)=0
    step_generation(&grid);
    printf("Gen %d: alive=%d\n", grid.generation, count_alive(&grid));
    // EXPECT: Gen 2: alive=3
    printf("Cell(3,2)=%d Cell(3,3)=%d Cell(3,4)=%d\n",
           get_cell(&grid, 3, 2), get_cell(&grid, 3, 3), get_cell(&grid, 3, 4));
    // EXPECT: Cell(3,2)=1 Cell(3,3)=1 Cell(3,4)=1

    printf("=== Block Test (Still Life) ===\n");
    // EXPECT: === Block Test (Still Life) ===
    clear_grid(&grid);
    setup_block(&grid, 2, 2);
    printf("Gen %d: alive=%d\n", grid.generation, count_alive(&grid));
    // EXPECT: Gen 0: alive=4
    step_generation(&grid);
    printf("Gen %d: alive=%d\n", grid.generation, count_alive(&grid));
    // EXPECT: Gen 1: alive=4
    step_generation(&grid);
    printf("Gen %d: alive=%d (stable)\n", grid.generation, count_alive(&grid));
    // EXPECT: Gen 2: alive=4 (stable)

    printf("=== Glider Test ===\n");
    // EXPECT: === Glider Test ===
    clear_grid(&grid);
    setup_glider(&grid, 0, 0);
    printf("Gen %d: alive=%d\n", grid.generation, count_alive(&grid));
    // EXPECT: Gen 0: alive=5
    int gen;
    for (gen = 0; gen < 4; gen = gen + 1) {
        step_generation(&grid);
        printf("Gen %d: alive=%d\n", grid.generation, count_alive(&grid));
    }
    // EXPECT: Gen 1: alive=5
    // EXPECT: Gen 2: alive=5
    // EXPECT: Gen 3: alive=5
    // EXPECT: Gen 4: alive=5
    printf("Glider moved: cell(1,1)=%d cell(3,2)=%d\n",
           get_cell(&grid, 1, 1), get_cell(&grid, 3, 2));
    // EXPECT: Glider moved: cell(1,1)=0 cell(3,2)=1

    return 0;
}
