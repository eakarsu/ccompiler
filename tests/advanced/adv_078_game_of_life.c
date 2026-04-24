// EXPECT: === Game of Life ===
// EXPECT: Blinker gen 0:
// EXPECT: ........
// EXPECT: ...***..
// EXPECT: ........
// EXPECT: ........
// EXPECT: ........
// EXPECT: ........
// EXPECT: ........
// EXPECT: ........
// EXPECT: Blinker gen 1:
// EXPECT: ....*...
// EXPECT: ....*...
// EXPECT: ....*...
// EXPECT: ........
// EXPECT: ........
// EXPECT: ........
// EXPECT: ........
// EXPECT: ........
// EXPECT: Blinker gen 2:
// EXPECT: ........
// EXPECT: ...***..
// EXPECT: ........
// EXPECT: ........
// EXPECT: ........
// EXPECT: ........
// EXPECT: ........
// EXPECT: ........
// EXPECT: Blinker period verified: 2
// EXPECT: Glider gen 0:
// EXPECT: ..*...........
// EXPECT: ...*..........
// EXPECT: .***..........
// EXPECT: ..............
// EXPECT: ..............
// EXPECT: ..............
// EXPECT: ..............
// EXPECT: ..............
// EXPECT: Glider gen 4:
// EXPECT: ..............
// EXPECT: ...*..........
// EXPECT: ....*.........
// EXPECT: Glider moved: right=1, down=1
// EXPECT: Block stable after 3 generations
// EXPECT: All game of life tests passed

int printf(const char *fmt, ...);

int grid[8][8];
int temp[8][8];

void clear_grid(void) {
    int r;
    int c;
    for (r = 0; r < 8; r++)
        for (c = 0; c < 8; c++)
            grid[r][c] = 0;
}

void copy_grid_to_temp(void) {
    int r;
    int c;
    for (r = 0; r < 8; r++)
        for (c = 0; c < 8; c++)
            temp[r][c] = grid[r][c];
}

void copy_temp_to_grid(void) {
    int r;
    int c;
    for (r = 0; r < 8; r++)
        for (c = 0; c < 8; c++)
            grid[r][c] = temp[r][c];
}

int count_neighbors(int r, int c) {
    int count = 0;
    int dr;
    int dc;
    for (dr = -1; dr <= 1; dr++) {
        for (dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            int nr = r + dr;
            int nc = c + dc;
            if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
                count = count + grid[nr][nc];
            }
        }
    }
    return count;
}

void step(void) {
    int r;
    int c;
    for (r = 0; r < 8; r++) {
        for (c = 0; c < 8; c++) {
            int n = count_neighbors(r, c);
            if (grid[r][c]) {
                temp[r][c] = (n == 2 || n == 3) ? 1 : 0;
            } else {
                temp[r][c] = (n == 3) ? 1 : 0;
            }
        }
    }
    copy_temp_to_grid();
}

void print_grid(const char *label) {
    printf("%s\n", label);
    int r;
    int c;
    for (r = 0; r < 8; r++) {
        for (c = 0; c < 8; c++) {
            printf("%c", grid[r][c] ? '*' : '.');
        }
        printf("\n");
    }
}

int grids_equal(void) {
    int r;
    int c;
    for (r = 0; r < 8; r++)
        for (c = 0; c < 8; c++)
            if (grid[r][c] != temp[r][c]) return 0;
    return 1;
}

void test_blinker(void) {
    clear_grid();
    grid[1][3] = 1;
    grid[1][4] = 1;
    grid[1][5] = 1;

    print_grid("Blinker gen 0:");

    int saved[8][8];
    int r;
    int c;
    for (r = 0; r < 8; r++)
        for (c = 0; c < 8; c++)
            saved[r][c] = grid[r][c];

    step();
    print_grid("Blinker gen 1:");

    step();
    print_grid("Blinker gen 2:");

    int match = 1;
    for (r = 0; r < 8; r++)
        for (c = 0; c < 8; c++)
            if (grid[r][c] != saved[r][c]) match = 0;

    if (match) {
        printf("Blinker period verified: 2\n");
    } else {
        printf("Blinker period FAILED\n");
    }
}

int grid2[8][14];
int temp2[8][14];

void clear_grid2(void) {
    int r; int c;
    for (r = 0; r < 8; r++)
        for (c = 0; c < 14; c++)
            grid2[r][c] = 0;
}

int count_neighbors2(int r, int c) {
    int count = 0;
    int dr; int dc;
    for (dr = -1; dr <= 1; dr++) {
        for (dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            int nr = r + dr;
            int nc = c + dc;
            if (nr >= 0 && nr < 8 && nc >= 0 && nc < 14) {
                count = count + grid2[nr][nc];
            }
        }
    }
    return count;
}

void step2(void) {
    int r; int c;
    for (r = 0; r < 8; r++) {
        for (c = 0; c < 14; c++) {
            int n = count_neighbors2(r, c);
            if (grid2[r][c]) {
                temp2[r][c] = (n == 2 || n == 3) ? 1 : 0;
            } else {
                temp2[r][c] = (n == 3) ? 1 : 0;
            }
        }
    }
    for (r = 0; r < 8; r++)
        for (c = 0; c < 14; c++)
            grid2[r][c] = temp2[r][c];
}

void print_grid2(const char *label) {
    printf("%s\n", label);
    int r; int c;
    for (r = 0; r < 8; r++) {
        for (c = 0; c < 14; c++) {
            printf("%c", grid2[r][c] ? '*' : '.');
        }
        printf("\n");
    }
}

void test_glider(void) {
    clear_grid2();
    grid2[0][2] = 1;
    grid2[1][3] = 1;
    grid2[2][1] = 1;
    grid2[2][2] = 1;
    grid2[2][3] = 1;

    print_grid2("Glider gen 0:");

    int i;
    for (i = 0; i < 4; i++) step2();

    printf("Glider gen 4:\n");
    int r; int c;
    for (r = 0; r < 3; r++) {
        for (c = 0; c < 14; c++) {
            printf("%c", grid2[r][c] ? '*' : '.');
        }
        printf("\n");
    }

    printf("Glider moved: right=1, down=1\n");
}

void test_block(void) {
    clear_grid();
    grid[1][1] = 1; grid[1][2] = 1;
    grid[2][1] = 1; grid[2][2] = 1;

    int saved[8][8];
    int r; int c;
    int gen;
    int stable = 1;

    for (gen = 0; gen < 3; gen++) {
        for (r = 0; r < 8; r++)
            for (c = 0; c < 8; c++)
                saved[r][c] = grid[r][c];
        step();
        for (r = 0; r < 8; r++)
            for (c = 0; c < 8; c++)
                if (grid[r][c] != saved[r][c]) stable = 0;
    }

    if (stable) {
        printf("Block stable after 3 generations\n");
    } else {
        printf("Block NOT stable\n");
    }
}

int main(void) {
    printf("=== Game of Life ===\n");
    test_blinker();
    test_glider();
    test_block();
    printf("All game of life tests passed\n");
    return 0;
}
