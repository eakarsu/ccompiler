int printf(const char *fmt, ...);
// EXPECT: R30:p0=1 p1=3 p2=3 p3=6 p4=4 p5=9 p6=5 p7=12 p8=7 p9=12 R110:p0=1 p1=2 p2=3 p3=3 p4=5 p5=3 p6=5 p7=6 R90:p0=1 p1=2 p2=2 p3=4 p4=2 BLK:g0=3 g1=3 g2=3 g3=3 g4=3 per2=1 GLI:g0=5 g1=5 g2=5 g3=5 g4=5 BLCK:g0=4 g1=4 g2=4 g3=4 still=1 BEE:g0=6 g1=6 g2=6 still=1 TOAD:g0=6 g1=6 g2=6 g3=6 per2=1 POP:r30=22 r110=11 r90=16 DIE:g0=1 g1=0 TWO:g0=2 g1=0 PAT:g0:0001000 g1:0011100 g2:0110010 g3:1101111 g4:1001000 R110L:p20=9 CUS:g0=4 g1=4 g2=6 g3=6 g4=6 g5=6 DONE

/* =========================================================================
 * Test 059: Cellular Automata
 * =========================================================================
 * 1D elementary cellular automata (Rule 30, Rule 110, Rule 90),
 * Conway's Game of Life on a small grid, generation counting.
 * ========================================================================= */

/* ---- 1D Cellular Automaton ---- */
int ca_cells[32];
int ca_next[32];
int ca_width;

void ca_init(int width) {
    int i;
    ca_width = width;
    for (i = 0; i < width; i = i + 1) {
        ca_cells[i] = 0;
    }
}

int ca_get_rule_bit(int rule, int pattern) {
    int val;
    int i;
    val = rule;
    for (i = 0; i < pattern; i = i + 1) {
        val = val / 2;
    }
    return val % 2;
}

void ca_step(int rule) {
    int i;
    int left;
    int center;
    int right;
    int pattern;

    for (i = 0; i < ca_width; i = i + 1) {
        if (i == 0) {
            left = 0;
        } else {
            left = ca_cells[i - 1];
        }
        center = ca_cells[i];
        if (i == ca_width - 1) {
            right = 0;
        } else {
            right = ca_cells[i + 1];
        }
        pattern = left * 4 + center * 2 + right;
        ca_next[i] = ca_get_rule_bit(rule, pattern);
    }

    for (i = 0; i < ca_width; i = i + 1) {
        ca_cells[i] = ca_next[i];
    }
}

int ca_population(void) {
    int count;
    int i;
    count = 0;
    for (i = 0; i < ca_width; i = i + 1) {
        count = count + ca_cells[i];
    }
    return count;
}

/* ---- Game of Life ---- */
int gol_grid[12][12];
int gol_next[12][12];
int gol_rows;
int gol_cols;
int gol_saved[12][12];

void gol_init(int rows, int cols) {
    int i;
    int j;
    gol_rows = rows;
    gol_cols = cols;
    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            gol_grid[i][j] = 0;
            gol_saved[i][j] = 0;
        }
    }
}

int gol_count_neighbors(int row, int col) {
    int count;
    int dr;
    int dc;
    int nr;
    int nc;

    count = 0;
    for (dr = -1; dr <= 1; dr = dr + 1) {
        for (dc = -1; dc <= 1; dc = dc + 1) {
            if (dr != 0 || dc != 0) {
                nr = row + dr;
                nc = col + dc;
                if (nr >= 0 && nr < gol_rows && nc >= 0 && nc < gol_cols) {
                    count = count + gol_grid[nr][nc];
                }
            }
        }
    }
    return count;
}

void gol_step(void) {
    int i;
    int j;
    int neighbors;

    for (i = 0; i < gol_rows; i = i + 1) {
        for (j = 0; j < gol_cols; j = j + 1) {
            neighbors = gol_count_neighbors(i, j);
            if (gol_grid[i][j] == 1) {
                if (neighbors == 2 || neighbors == 3) {
                    gol_next[i][j] = 1;
                } else {
                    gol_next[i][j] = 0;
                }
            } else {
                if (neighbors == 3) {
                    gol_next[i][j] = 1;
                } else {
                    gol_next[i][j] = 0;
                }
            }
        }
    }

    for (i = 0; i < gol_rows; i = i + 1) {
        for (j = 0; j < gol_cols; j = j + 1) {
            gol_grid[i][j] = gol_next[i][j];
        }
    }
}

int gol_count_alive(void) {
    int count;
    int i;
    int j;
    count = 0;
    for (i = 0; i < gol_rows; i = i + 1) {
        for (j = 0; j < gol_cols; j = j + 1) {
            count = count + gol_grid[i][j];
        }
    }
    return count;
}

void gol_save(void) {
    int i;
    int j;
    for (i = 0; i < gol_rows; i = i + 1) {
        for (j = 0; j < gol_cols; j = j + 1) {
            gol_saved[i][j] = gol_grid[i][j];
        }
    }
}

int gol_compare_saved(void) {
    int i;
    int j;
    for (i = 0; i < gol_rows; i = i + 1) {
        for (j = 0; j < gol_cols; j = j + 1) {
            if (gol_grid[i][j] != gol_saved[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

/* =========================================================================
 * Test functions
 * ========================================================================= */

void test_rule30(void) {
    int gen;

    printf("R30:");
    ca_init(21);
    ca_cells[10] = 1;

    printf("p0=%d", ca_population());

    for (gen = 1; gen <= 9; gen = gen + 1) {
        ca_step(30);
        printf(" p%d=%d", gen, ca_population());
    }
}

void test_rule110(void) {
    int gen;

    printf(" R110:");
    ca_init(21);
    ca_cells[10] = 1;

    printf("p0=%d", ca_population());

    for (gen = 1; gen <= 7; gen = gen + 1) {
        ca_step(110);
        printf(" p%d=%d", gen, ca_population());
    }
}

void test_rule90(void) {
    int gen;

    printf(" R90:");
    ca_init(9);
    ca_cells[4] = 1;

    printf("p0=%d", ca_population());

    for (gen = 1; gen <= 4; gen = gen + 1) {
        ca_step(90);
        printf(" p%d=%d", gen, ca_population());
    }
}

void test_blinker(void) {
    int gen;
    int is_period2;

    printf(" BLK:");
    gol_init(5, 5);
    gol_grid[1][2] = 1;
    gol_grid[2][2] = 1;
    gol_grid[3][2] = 1;

    printf("g0=%d", gol_count_alive());
    gol_save();

    for (gen = 1; gen <= 4; gen = gen + 1) {
        gol_step();
        printf(" g%d=%d", gen, gol_count_alive());
    }

    /* Check period 2 */
    gol_init(5, 5);
    gol_grid[1][2] = 1;
    gol_grid[2][2] = 1;
    gol_grid[3][2] = 1;
    gol_save();
    gol_step();
    gol_step();
    is_period2 = gol_compare_saved();
    printf(" per2=%d", is_period2);
}

void test_glider(void) {
    int gen;

    printf(" GLI:");
    gol_init(10, 10);
    gol_grid[1][2] = 1;
    gol_grid[2][3] = 1;
    gol_grid[3][1] = 1;
    gol_grid[3][2] = 1;
    gol_grid[3][3] = 1;

    printf("g0=%d", gol_count_alive());

    for (gen = 1; gen <= 4; gen = gen + 1) {
        gol_step();
        printf(" g%d=%d", gen, gol_count_alive());
    }
}

void test_block(void) {
    int gen;
    int is_still;

    printf(" BLCK:");
    gol_init(6, 6);
    gol_grid[2][2] = 1;
    gol_grid[2][3] = 1;
    gol_grid[3][2] = 1;
    gol_grid[3][3] = 1;

    printf("g0=%d", gol_count_alive());
    gol_save();

    for (gen = 1; gen <= 3; gen = gen + 1) {
        gol_step();
        printf(" g%d=%d", gen, gol_count_alive());
    }
    is_still = gol_compare_saved();
    printf(" still=%d", is_still);
}

void test_beehive(void) {
    int is_still;

    printf(" BEE:");
    gol_init(6, 6);
    gol_grid[1][2] = 1;
    gol_grid[1][3] = 1;
    gol_grid[2][1] = 1;
    gol_grid[2][4] = 1;
    gol_grid[3][2] = 1;
    gol_grid[3][3] = 1;

    printf("g0=%d", gol_count_alive());
    gol_save();

    gol_step();
    printf(" g1=%d", gol_count_alive());
    gol_step();
    printf(" g2=%d", gol_count_alive());

    is_still = gol_compare_saved();
    printf(" still=%d", is_still);
}

void test_toad(void) {
    int gen;
    int is_period2;

    printf(" TOAD:");
    gol_init(6, 6);
    gol_grid[2][2] = 1;
    gol_grid[2][3] = 1;
    gol_grid[2][4] = 1;
    gol_grid[3][1] = 1;
    gol_grid[3][2] = 1;
    gol_grid[3][3] = 1;

    printf("g0=%d", gol_count_alive());
    gol_save();

    for (gen = 1; gen <= 3; gen = gen + 1) {
        gol_step();
        printf(" g%d=%d", gen, gol_count_alive());
    }

    gol_init(6, 6);
    gol_grid[2][2] = 1;
    gol_grid[2][3] = 1;
    gol_grid[2][4] = 1;
    gol_grid[3][1] = 1;
    gol_grid[3][2] = 1;
    gol_grid[3][3] = 1;
    gol_save();
    gol_step();
    gol_step();
    is_period2 = gol_compare_saved();
    printf(" per2=%d", is_period2);
}

void test_population_tracking(void) {
    int gen;

    printf(" POP:");

    ca_init(31);
    ca_cells[15] = 1;
    for (gen = 0; gen < 15; gen = gen + 1) {
        ca_step(30);
    }
    printf("r30=%d", ca_population());

    ca_init(31);
    ca_cells[15] = 1;
    for (gen = 0; gen < 15; gen = gen + 1) {
        ca_step(110);
    }
    printf(" r110=%d", ca_population());

    ca_init(31);
    ca_cells[15] = 1;
    for (gen = 0; gen < 15; gen = gen + 1) {
        ca_step(90);
    }
    printf(" r90=%d", ca_population());
}

void test_die_off(void) {
    printf(" DIE:");
    gol_init(5, 5);
    gol_grid[2][2] = 1;

    printf("g0=%d", gol_count_alive());
    gol_step();
    printf(" g1=%d", gol_count_alive());
}

void test_two_cells(void) {
    printf(" TWO:");
    gol_init(5, 5);
    gol_grid[2][2] = 1;
    gol_grid[2][3] = 1;

    printf("g0=%d", gol_count_alive());
    gol_step();
    printf(" g1=%d", gol_count_alive());
}

void test_rule30_pattern(void) {
    int i;
    int gen;

    printf(" PAT:");
    ca_init(11);
    ca_cells[5] = 1;

    /* Print first 5 generations center 7 cells */
    for (gen = 0; gen < 5; gen = gen + 1) {
        if (gen > 0) {
            ca_step(30);
        }
        printf("g%d:", gen);
        for (i = 2; i <= 8; i = i + 1) {
            printf("%d", ca_cells[i]);
        }
        printf(" ");
    }
}

void test_rule110_long(void) {
    int gen;

    printf("R110L:");
    ca_init(31);
    ca_cells[15] = 1;

    for (gen = 0; gen < 20; gen = gen + 1) {
        if (gen > 0) {
            ca_step(110);
        }
    }
    printf("p20=%d", ca_population());
}

void test_gol_custom(void) {
    int gen;

    printf(" CUS:");
    gol_init(8, 8);
    /* L-shaped pattern */
    gol_grid[2][2] = 1;
    gol_grid[3][2] = 1;
    gol_grid[4][2] = 1;
    gol_grid[4][3] = 1;

    printf("g0=%d", gol_count_alive());
    for (gen = 1; gen <= 5; gen = gen + 1) {
        gol_step();
        printf(" g%d=%d", gen, gol_count_alive());
    }
    printf(" DONE\n");
}

int main(void) {
    test_rule30();
    test_rule110();
    test_rule90();
    test_blinker();
    test_glider();
    test_block();
    test_beehive();
    test_toad();
    test_population_tracking();
    test_die_off();
    test_two_cells();
    test_rule30_pattern();
    test_rule110_long();
    test_gol_custom();
    return 0;
}
