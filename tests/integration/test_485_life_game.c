int printf(const char *fmt, ...);
// EXPECT: === Game of Life ===\nInitial (glider):\n........\n..#.....\n...#....\n.###....\n........\n........\n........\n........\nAlive: 5\nGen 1: alive=5\nGen 2: alive=5\nGen 3: alive=5\nGen 4: alive=5\nAfter 4 generations:\n........\n........\n...#....\n....#...\n..###...\n........\n........\n........\nFinal alive: 5\nBlinker test:\n  Gen 1: alive=3\n  Gen 2: alive=3\n  Gen 3: alive=3

struct Grid {
    int cells[10][10];
    int rows;
    int cols;
};

void clear_grid(struct Grid *g) {
    int r, c;
    g->rows = 10;
    g->cols = 10;
    for (r = 0; r < 10; r++) {
        for (c = 0; c < 10; c++) {
            g->cells[r][c] = 0;
        }
    }
}

void set_cell(struct Grid *g, int r, int c) {
    if (r >= 0 && r < g->rows && c >= 0 && c < g->cols) {
        g->cells[r][c] = 1;
    }
}

int count_neighbors(struct Grid *g, int row, int col) {
    int count = 0;
    int dr, dc;
    for (dr = -1; dr <= 1; dr++) {
        for (dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            int nr = row + dr;
            int nc = col + dc;
            if (nr >= 0 && nr < g->rows && nc >= 0 && nc < g->cols) {
                count = count + g->cells[nr][nc];
            }
        }
    }
    return count;
}

void step(struct Grid *current, struct Grid *next) {
    int r, c;
    next->rows = current->rows;
    next->cols = current->cols;
    for (r = 0; r < current->rows; r++) {
        for (c = 0; c < current->cols; c++) {
            int n = count_neighbors(current, r, c);
            if (current->cells[r][c]) {
                next->cells[r][c] = (n == 2 || n == 3) ? 1 : 0;
            } else {
                next->cells[r][c] = (n == 3) ? 1 : 0;
            }
        }
    }
}

int count_alive(struct Grid *g) {
    int count = 0;
    int r, c;
    for (r = 0; r < g->rows; r++) {
        for (c = 0; c < g->cols; c++) {
            count = count + g->cells[r][c];
        }
    }
    return count;
}

void print_grid(struct Grid *g) {
    int r, c;
    for (r = 0; r < 8; r++) {
        for (c = 0; c < 8; c++) {
            printf("%c", g->cells[r][c] ? '#' : '.');
        }
        printf("\n");
    }
}

int main(void) {
    struct Grid grid_a;
    struct Grid grid_b;

    clear_grid(&grid_a);

    set_cell(&grid_a, 1, 2);
    set_cell(&grid_a, 2, 3);
    set_cell(&grid_a, 3, 1);
    set_cell(&grid_a, 3, 2);
    set_cell(&grid_a, 3, 3);

    printf("=== Game of Life ===\n");
    printf("Initial (glider):\n");
    print_grid(&grid_a);
    printf("Alive: %d\n", count_alive(&grid_a));

    int gen;
    struct Grid *current = &grid_a;
    struct Grid *next = &grid_b;

    for (gen = 1; gen <= 4; gen++) {
        step(current, next);
        printf("Gen %d: alive=%d\n", gen, count_alive(next));
        struct Grid *tmp = current;
        current = next;
        next = tmp;
    }

    printf("After 4 generations:\n");
    print_grid(current);
    printf("Final alive: %d\n", count_alive(current));

    clear_grid(&grid_a);
    set_cell(&grid_a, 3, 3);
    set_cell(&grid_a, 3, 4);
    set_cell(&grid_a, 3, 5);
    printf("Blinker test:\n");
    current = &grid_a;
    next = &grid_b;
    for (gen = 1; gen <= 3; gen++) {
        step(current, next);
        printf("  Gen %d: alive=%d\n", gen, count_alive(next));
        struct Grid *tmp2 = current;
        current = next;
        next = tmp2;
    }

    return 0;
}
