int printf(const char *fmt, ...);
// EXPECT: init:0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0|valid_row0:1|valid_col0:1|valid_box00:1|place11:1|place12:0|solve_empty:1|s1:1,2,3,4,3,4,1,2,2,1,4,3,4,3,2,1|s1_valid:1|s2:1,2,3,4,3,4,1,2,2,1,4,3,4,3,2,1|s2_valid:1|puzzle1:_,2,3,4,3,4,1,2,2,1,4,3,4,3,2,1|solve_p1:1|p1_sol:1,2,3,4,3,4,1,2,2,1,4,3,4,3,2,1|puzzle2:1,_,_,4,_,4,1,_,2,1,_,3,4,_,2,1|solve_p2:1|p2_sol:1,2,3,4,3,4,1,2,2,1,4,3,4,3,2,1|count_empty:288|count_p1:1|count_p2:1|gen1_blanks:4|gen1_solvable:1|row_clash:0|col_clash:0|box_clash:0|full_valid:1|diag1:1,4,4,1|diag2:4,1,1,4|corners:1,4,4,1|sum_row0:10|sum_row1:10|sum_row2:10|sum_row3:10|sum_col0:10|sum_col1:10|sum_col2:10|sum_col3:10|prod_diag:16|min_all:1|max_all:4|unique_rows:1|unique_cols:1|total_cells:16|filled:16|empty:0|done
int grid[16];
int temp_grid[16];
int solution_count;

int get(int r, int c) {
    return grid[r * 4 + c];
}

void set(int r, int c, int val) {
    grid[r * 4 + c] = val;
}

int get_temp(int r, int c) {
    return temp_grid[r * 4 + c];
}

void set_temp(int r, int c, int val) {
    temp_grid[r * 4 + c] = val;
}

void clear_grid(void) {
    int i;
    i = 0;
    while (i < 16) {
        grid[i] = 0;
        i = i + 1;
    }
}

void copy_to_temp(void) {
    int i;
    i = 0;
    while (i < 16) {
        temp_grid[i] = grid[i];
        i = i + 1;
    }
}

void copy_from_temp(void) {
    int i;
    i = 0;
    while (i < 16) {
        grid[i] = temp_grid[i];
        i = i + 1;
    }
}

void print_grid(void) {
    int i;
    i = 0;
    while (i < 16) {
        if (i > 0) {
            printf("%c", ',');
        }
        printf("%d", grid[i]);
        i = i + 1;
    }
}

void print_grid_with_blanks(void) {
    int i;
    i = 0;
    while (i < 16) {
        if (i > 0) {
            printf("%c", ',');
        }
        if (grid[i] == 0) {
            printf("%c", '_');
        } else {
            printf("%d", grid[i]);
        }
        i = i + 1;
    }
}

int valid_row(int r) {
    int seen[5];
    int c;
    int v;
    seen[0] = 0;
    seen[1] = 0;
    seen[2] = 0;
    seen[3] = 0;
    seen[4] = 0;
    c = 0;
    while (c < 4) {
        v = get(r, c);
        if (v < 1) { c = c + 1; continue; }
        if (v > 4) { c = c + 1; continue; }
        if (seen[v]) { return 0; }
        seen[v] = 1;
        c = c + 1;
    }
    return 1;
}

int valid_col(int c) {
    int seen[5];
    int r;
    int v;
    seen[0] = 0;
    seen[1] = 0;
    seen[2] = 0;
    seen[3] = 0;
    seen[4] = 0;
    r = 0;
    while (r < 4) {
        v = get(r, c);
        if (v < 1) { r = r + 1; continue; }
        if (v > 4) { r = r + 1; continue; }
        if (seen[v]) { return 0; }
        seen[v] = 1;
        r = r + 1;
    }
    return 1;
}

int valid_box(int br, int bc) {
    int seen[5];
    int r;
    int c;
    int v;
    seen[0] = 0;
    seen[1] = 0;
    seen[2] = 0;
    seen[3] = 0;
    seen[4] = 0;
    r = br;
    while (r < br + 2) {
        c = bc;
        while (c < bc + 2) {
            v = get(r, c);
            if (v >= 1) {
                if (v <= 4) {
                    if (seen[v]) { return 0; }
                    seen[v] = 1;
                }
            }
            c = c + 1;
        }
        r = r + 1;
    }
    return 1;
}

int is_valid_placement(int r, int c, int val) {
    int old;
    int result;
    old = get(r, c);
    set(r, c, val);
    result = 1;
    if (!valid_row(r)) { result = 0; }
    if (!valid_col(c)) { result = 0; }
    if (!valid_box((r / 2) * 2, (c / 2) * 2)) { result = 0; }
    set(r, c, old);
    return result;
}

int is_full_valid(void) {
    int r;
    int c;
    r = 0;
    while (r < 4) {
        if (!valid_row(r)) { return 0; }
        r = r + 1;
    }
    c = 0;
    while (c < 4) {
        if (!valid_col(c)) { return 0; }
        c = c + 1;
    }
    if (!valid_box(0, 0)) { return 0; }
    if (!valid_box(0, 2)) { return 0; }
    if (!valid_box(2, 0)) { return 0; }
    if (!valid_box(2, 2)) { return 0; }
    return 1;
}

int solve(void) {
    int i;
    int r;
    int c;
    int val;
    i = 0;
    while (i < 16) {
        if (grid[i] == 0) {
            r = i / 4;
            c = i - r * 4;
            val = 1;
            while (val <= 4) {
                if (is_valid_placement(r, c, val)) {
                    set(r, c, val);
                    if (solve()) {
                        return 1;
                    }
                    set(r, c, 0);
                }
                val = val + 1;
            }
            return 0;
        }
        i = i + 1;
    }
    return 1;
}

void count_solutions_helper(void) {
    int i;
    int r;
    int c;
    int val;
    i = 0;
    while (i < 16) {
        if (grid[i] == 0) {
            r = i / 4;
            c = i - r * 4;
            val = 1;
            while (val <= 4) {
                if (is_valid_placement(r, c, val)) {
                    set(r, c, val);
                    count_solutions_helper();
                    set(r, c, 0);
                }
                val = val + 1;
            }
            return;
        }
        i = i + 1;
    }
    solution_count = solution_count + 1;
}

int count_solutions(void) {
    solution_count = 0;
    count_solutions_helper();
    return solution_count;
}

int count_empty_cells(void) {
    int i;
    int cnt;
    cnt = 0;
    i = 0;
    while (i < 16) {
        if (grid[i] == 0) {
            cnt = cnt + 1;
        }
        i = i + 1;
    }
    return cnt;
}

int count_filled_cells(void) {
    return 16 - count_empty_cells();
}

int sum_row(int r) {
    int s;
    int c;
    s = 0;
    c = 0;
    while (c < 4) {
        s = s + get(r, c);
        c = c + 1;
    }
    return s;
}

int sum_col(int c) {
    int s;
    int r;
    s = 0;
    r = 0;
    while (r < 4) {
        s = s + get(r, c);
        r = r + 1;
    }
    return s;
}

int min_all(void) {
    int m;
    int i;
    m = grid[0];
    i = 1;
    while (i < 16) {
        if (grid[i] < m) {
            m = grid[i];
        }
        i = i + 1;
    }
    return m;
}

int max_all(void) {
    int m;
    int i;
    m = grid[0];
    i = 1;
    while (i < 16) {
        if (grid[i] > m) {
            m = grid[i];
        }
        i = i + 1;
    }
    return m;
}

int check_unique_rows(void) {
    int r;
    int c;
    int seen[5];
    int v;
    r = 0;
    while (r < 4) {
        seen[1] = 0; seen[2] = 0; seen[3] = 0; seen[4] = 0;
        c = 0;
        while (c < 4) {
            v = get(r, c);
            if (v >= 1 && v <= 4) {
                if (seen[v]) { return 0; }
                seen[v] = 1;
            }
            c = c + 1;
        }
        r = r + 1;
    }
    return 1;
}

int check_unique_cols(void) {
    int r;
    int c;
    int seen[5];
    int v;
    c = 0;
    while (c < 4) {
        seen[1] = 0; seen[2] = 0; seen[3] = 0; seen[4] = 0;
        r = 0;
        while (r < 4) {
            v = get(r, c);
            if (v >= 1 && v <= 4) {
                if (seen[v]) { return 0; }
                seen[v] = 1;
            }
            r = r + 1;
        }
        c = c + 1;
    }
    return 1;
}

void generate_puzzle(int blanks) {
    int i;
    int step;
    /* Start from a solved grid already in grid[] */
    /* Remove 'blanks' cells deterministically */
    step = 16 / blanks;
    if (step < 1) { step = 1; }
    i = 0;
    while (i < 16 && blanks > 0) {
        grid[i] = 0;
        blanks = blanks - 1;
        i = i + step;
    }
}

int main(void) {
    int i;
    int cnt;
    int s;

    /* Test 1: Initialize empty grid */
    clear_grid();
    printf("%c", 'i');
    printf("%c", 'n');
    printf("%c", 'i');
    printf("%c", 't');
    printf("%c", ':');
    print_grid();

    /* Test 2: Validate empty row */
    printf("%c", '|');
    printf("%c", 'v');
    printf("%c", 'a');
    printf("%c", 'l');
    printf("%c", 'i');
    printf("%c", 'd');
    printf("%c", '_');
    printf("%c", 'r');
    printf("%c", 'o');
    printf("%c", 'w');
    printf("%c", '0');
    printf("%c", ':');
    printf("%d", valid_row(0));

    /* Test 3: Validate empty col */
    printf("%c", '|');
    printf("%c", 'v');
    printf("%c", 'a');
    printf("%c", 'l');
    printf("%c", 'i');
    printf("%c", 'd');
    printf("%c", '_');
    printf("%c", 'c');
    printf("%c", 'o');
    printf("%c", 'l');
    printf("%c", '0');
    printf("%c", ':');
    printf("%d", valid_col(0));

    /* Test 4: Validate empty box */
    printf("%c", '|');
    printf("%c", 'v');
    printf("%c", 'a');
    printf("%c", 'l');
    printf("%c", 'i');
    printf("%c", 'd');
    printf("%c", '_');
    printf("%c", 'b');
    printf("%c", 'o');
    printf("%c", 'x');
    printf("%c", '0');
    printf("%c", '0');
    printf("%c", ':');
    printf("%d", valid_box(0, 0));

    /* Test 5: Valid placement */
    printf("%c", '|');
    printf("%c", 'p');
    printf("%c", 'l');
    printf("%c", 'a');
    printf("%c", 'c');
    printf("%c", 'e');
    printf("%c", '1');
    printf("%c", '1');
    printf("%c", ':');
    printf("%d", is_valid_placement(0, 0, 1));

    /* Test 6: Invalid placement (duplicate in row after placing) */
    set(0, 0, 1);
    printf("%c", '|');
    printf("%c", 'p');
    printf("%c", 'l');
    printf("%c", 'a');
    printf("%c", 'c');
    printf("%c", 'e');
    printf("%c", '1');
    printf("%c", '2');
    printf("%c", ':');
    printf("%d", is_valid_placement(0, 1, 1));
    set(0, 0, 0);

    /* Test 7: Solve empty grid */
    clear_grid();
    printf("%c", '|');
    printf("%c", 's');
    printf("%c", 'o');
    printf("%c", 'l');
    printf("%c", 'v');
    printf("%c", 'e');
    printf("%c", '_');
    printf("%c", 'e');
    printf("%c", 'm');
    printf("%c", 'p');
    printf("%c", 't');
    printf("%c", 'y');
    printf("%c", ':');
    printf("%d", solve());

    /* Test 8: Print solution 1 */
    printf("%c", '|');
    printf("%c", 's');
    printf("%c", '1');
    printf("%c", ':');
    print_grid();

    /* Test 9: Validate solution 1 */
    printf("%c", '|');
    printf("%c", 's');
    printf("%c", '1');
    printf("%c", '_');
    printf("%c", 'v');
    printf("%c", 'a');
    printf("%c", 'l');
    printf("%c", 'i');
    printf("%c", 'd');
    printf("%c", ':');
    printf("%d", is_full_valid());

    /* Test 10: Second solution - use a different starting point */
    clear_grid();
    /* Force row 0 to be 1,2,3,4 and row 1 to be 3,4,1,2 */
    set(0, 0, 1); set(0, 1, 2); set(0, 2, 3); set(0, 3, 4);
    set(1, 0, 3); set(1, 1, 4); set(1, 2, 1); set(1, 3, 2);
    /* Solve rest */
    solve();
    printf("%c", '|');
    printf("%c", 's');
    printf("%c", '2');
    printf("%c", ':');
    print_grid();

    /* Test 11: Validate solution 2 */
    printf("%c", '|');
    printf("%c", 's');
    printf("%c", '2');
    printf("%c", '_');
    printf("%c", 'v');
    printf("%c", 'a');
    printf("%c", 'l');
    printf("%c", 'i');
    printf("%c", 'd');
    printf("%c", ':');
    printf("%d", is_full_valid());

    /* Test 12: Create puzzle from solution 1 */
    clear_grid();
    solve(); /* get solution 1 again: 1,2,3,4,3,4,1,2,2,1,4,3,4,3,2,1 */
    grid[0] = 0; /* remove cell 0 */
    printf("%c", '|');
    printf("%c", 'p');
    printf("%c", 'u');
    printf("%c", 'z');
    printf("%c", 'z');
    printf("%c", 'l');
    printf("%c", 'e');
    printf("%c", '1');
    printf("%c", ':');
    print_grid_with_blanks();

    /* Test 13: Solve puzzle 1 */
    printf("%c", '|');
    printf("%c", 's');
    printf("%c", 'o');
    printf("%c", 'l');
    printf("%c", 'v');
    printf("%c", 'e');
    printf("%c", '_');
    printf("%c", 'p');
    printf("%c", '1');
    printf("%c", ':');
    printf("%d", solve());

    /* Test 14: Print puzzle 1 solution */
    printf("%c", '|');
    printf("%c", 'p');
    printf("%c", '1');
    printf("%c", '_');
    printf("%c", 's');
    printf("%c", 'o');
    printf("%c", 'l');
    printf("%c", ':');
    print_grid();

    /* Test 15: Harder puzzle */
    clear_grid();
    set(0, 0, 1); set(0, 3, 4);
    set(1, 1, 4); set(1, 2, 1);
    set(2, 0, 2); set(2, 1, 1); set(2, 3, 3);
    set(3, 0, 4); set(3, 2, 2); set(3, 3, 1);
    printf("%c", '|');
    printf("%c", 'p');
    printf("%c", 'u');
    printf("%c", 'z');
    printf("%c", 'z');
    printf("%c", 'l');
    printf("%c", 'e');
    printf("%c", '2');
    printf("%c", ':');
    print_grid_with_blanks();

    /* Test 16: Solve puzzle 2 */
    printf("%c", '|');
    printf("%c", 's');
    printf("%c", 'o');
    printf("%c", 'l');
    printf("%c", 'v');
    printf("%c", 'e');
    printf("%c", '_');
    printf("%c", 'p');
    printf("%c", '2');
    printf("%c", ':');
    printf("%d", solve());

    /* Test 17: Print puzzle 2 solution */
    printf("%c", '|');
    printf("%c", 'p');
    printf("%c", '2');
    printf("%c", '_');
    printf("%c", 's');
    printf("%c", 'o');
    printf("%c", 'l');
    printf("%c", ':');
    print_grid();

    /* Test 18: Count solutions of empty grid */
    clear_grid();
    printf("%c", '|');
    printf("%c", 'c');
    printf("%c", 'o');
    printf("%c", 'u');
    printf("%c", 'n');
    printf("%c", 't');
    printf("%c", '_');
    printf("%c", 'e');
    printf("%c", 'm');
    printf("%c", 'p');
    printf("%c", 't');
    printf("%c", 'y');
    printf("%c", ':');
    printf("%d", count_solutions());

    /* Test 19: Count solutions of puzzle 1 (should be 1) */
    clear_grid();
    solve();
    grid[0] = 0;
    printf("%c", '|');
    printf("%c", 'c');
    printf("%c", 'o');
    printf("%c", 'u');
    printf("%c", 'n');
    printf("%c", 't');
    printf("%c", '_');
    printf("%c", 'p');
    printf("%c", '1');
    printf("%c", ':');
    printf("%d", count_solutions());

    /* Test 20: Count solutions of puzzle 2 */
    clear_grid();
    set(0, 0, 1); set(0, 3, 4);
    set(1, 1, 4); set(1, 2, 1);
    set(2, 0, 2); set(2, 1, 1); set(2, 3, 3);
    set(3, 0, 4); set(3, 2, 2); set(3, 3, 1);
    printf("%c", '|');
    printf("%c", 'c');
    printf("%c", 'o');
    printf("%c", 'u');
    printf("%c", 'n');
    printf("%c", 't');
    printf("%c", '_');
    printf("%c", 'p');
    printf("%c", '2');
    printf("%c", ':');
    printf("%d", count_solutions());

    /* Test 21: Generate puzzle with 4 blanks */
    clear_grid();
    solve(); /* 1,2,3,4,3,4,1,2,2,1,4,3,4,3,2,1 */
    copy_to_temp();
    generate_puzzle(4); /* removes cells 0, 4, 8, 12 */
    cnt = count_empty_cells();
    printf("%c", '|');
    printf("%c", 'g');
    printf("%c", 'e');
    printf("%c", 'n');
    printf("%c", '1');
    printf("%c", '_');
    printf("%c", 'b');
    printf("%c", 'l');
    printf("%c", 'a');
    printf("%c", 'n');
    printf("%c", 'k');
    printf("%c", 's');
    printf("%c", ':');
    printf("%d", cnt);

    /* Test 22: Check generated puzzle is solvable */
    printf("%c", '|');
    printf("%c", 'g');
    printf("%c", 'e');
    printf("%c", 'n');
    printf("%c", '1');
    printf("%c", '_');
    printf("%c", 's');
    printf("%c", 'o');
    printf("%c", 'l');
    printf("%c", 'v');
    printf("%c", 'a');
    printf("%c", 'b');
    printf("%c", 'l');
    printf("%c", 'e');
    printf("%c", ':');
    copy_from_temp();
    generate_puzzle(4);
    printf("%d", solve());

    /* Test 23: Row clash detection */
    clear_grid();
    set(0, 0, 1); set(0, 1, 1);
    printf("%c", '|');
    printf("%c", 'r');
    printf("%c", 'o');
    printf("%c", 'w');
    printf("%c", '_');
    printf("%c", 'c');
    printf("%c", 'l');
    printf("%c", 'a');
    printf("%c", 's');
    printf("%c", 'h');
    printf("%c", ':');
    printf("%d", valid_row(0));

    /* Test 24: Col clash detection */
    clear_grid();
    set(0, 0, 2); set(1, 0, 2);
    printf("%c", '|');
    printf("%c", 'c');
    printf("%c", 'o');
    printf("%c", 'l');
    printf("%c", '_');
    printf("%c", 'c');
    printf("%c", 'l');
    printf("%c", 'a');
    printf("%c", 's');
    printf("%c", 'h');
    printf("%c", ':');
    printf("%d", valid_col(0));

    /* Test 25: Box clash detection */
    clear_grid();
    set(0, 0, 3); set(1, 1, 3);
    printf("%c", '|');
    printf("%c", 'b');
    printf("%c", 'o');
    printf("%c", 'x');
    printf("%c", '_');
    printf("%c", 'c');
    printf("%c", 'l');
    printf("%c", 'a');
    printf("%c", 's');
    printf("%c", 'h');
    printf("%c", ':');
    printf("%d", valid_box(0, 0));

    /* Test 26: Full valid check on solved grid */
    clear_grid();
    solve();
    printf("%c", '|');
    printf("%c", 'f');
    printf("%c", 'u');
    printf("%c", 'l');
    printf("%c", 'l');
    printf("%c", '_');
    printf("%c", 'v');
    printf("%c", 'a');
    printf("%c", 'l');
    printf("%c", 'i');
    printf("%c", 'd');
    printf("%c", ':');
    printf("%d", is_full_valid());

    /* Grid is 1,2,3,4,3,4,1,2,2,1,4,3,4,3,2,1 */
    /* Test 27: Diagonal values */
    printf("%c", '|');
    printf("%c", 'd');
    printf("%c", 'i');
    printf("%c", 'a');
    printf("%c", 'g');
    printf("%c", '1');
    printf("%c", ':');
    printf("%d", get(0, 0));
    printf("%c", ',');
    printf("%d", get(1, 1));
    printf("%c", ',');
    printf("%d", get(2, 2));
    printf("%c", ',');
    printf("%d", get(3, 3));

    /* Test 28: Anti-diagonal */
    printf("%c", '|');
    printf("%c", 'd');
    printf("%c", 'i');
    printf("%c", 'a');
    printf("%c", 'g');
    printf("%c", '2');
    printf("%c", ':');
    printf("%d", get(0, 3));
    printf("%c", ',');
    printf("%d", get(1, 2));
    printf("%c", ',');
    printf("%d", get(2, 1));
    printf("%c", ',');
    printf("%d", get(3, 0));

    /* Test 29: Corners */
    printf("%c", '|');
    printf("%c", 'c');
    printf("%c", 'o');
    printf("%c", 'r');
    printf("%c", 'n');
    printf("%c", 'e');
    printf("%c", 'r');
    printf("%c", 's');
    printf("%c", ':');
    printf("%d", get(0, 0));
    printf("%c", ',');
    printf("%d", get(0, 3));
    printf("%c", ',');
    printf("%d", get(3, 0));
    printf("%c", ',');
    printf("%d", get(3, 3));

    /* Test 30-33: Row sums */
    printf("%c", '|');
    printf("%c", 's');
    printf("%c", 'u');
    printf("%c", 'm');
    printf("%c", '_');
    printf("%c", 'r');
    printf("%c", 'o');
    printf("%c", 'w');
    printf("%c", '0');
    printf("%c", ':');
    printf("%d", sum_row(0));

    printf("%c", '|');
    printf("%c", 's');
    printf("%c", 'u');
    printf("%c", 'm');
    printf("%c", '_');
    printf("%c", 'r');
    printf("%c", 'o');
    printf("%c", 'w');
    printf("%c", '1');
    printf("%c", ':');
    printf("%d", sum_row(1));

    printf("%c", '|');
    printf("%c", 's');
    printf("%c", 'u');
    printf("%c", 'm');
    printf("%c", '_');
    printf("%c", 'r');
    printf("%c", 'o');
    printf("%c", 'w');
    printf("%c", '2');
    printf("%c", ':');
    printf("%d", sum_row(2));

    printf("%c", '|');
    printf("%c", 's');
    printf("%c", 'u');
    printf("%c", 'm');
    printf("%c", '_');
    printf("%c", 'r');
    printf("%c", 'o');
    printf("%c", 'w');
    printf("%c", '3');
    printf("%c", ':');
    printf("%d", sum_row(3));

    /* Test 34-37: Col sums */
    printf("%c", '|');
    printf("%c", 's');
    printf("%c", 'u');
    printf("%c", 'm');
    printf("%c", '_');
    printf("%c", 'c');
    printf("%c", 'o');
    printf("%c", 'l');
    printf("%c", '0');
    printf("%c", ':');
    printf("%d", sum_col(0));

    printf("%c", '|');
    printf("%c", 's');
    printf("%c", 'u');
    printf("%c", 'm');
    printf("%c", '_');
    printf("%c", 'c');
    printf("%c", 'o');
    printf("%c", 'l');
    printf("%c", '1');
    printf("%c", ':');
    printf("%d", sum_col(1));

    printf("%c", '|');
    printf("%c", 's');
    printf("%c", 'u');
    printf("%c", 'm');
    printf("%c", '_');
    printf("%c", 'c');
    printf("%c", 'o');
    printf("%c", 'l');
    printf("%c", '2');
    printf("%c", ':');
    printf("%d", sum_col(2));

    printf("%c", '|');
    printf("%c", 's');
    printf("%c", 'u');
    printf("%c", 'm');
    printf("%c", '_');
    printf("%c", 'c');
    printf("%c", 'o');
    printf("%c", 'l');
    printf("%c", '3');
    printf("%c", ':');
    printf("%d", sum_col(3));

    /* Test 38: Product of diagonal */
    printf("%c", '|');
    printf("%c", 'p');
    printf("%c", 'r');
    printf("%c", 'o');
    printf("%c", 'd');
    printf("%c", '_');
    printf("%c", 'd');
    printf("%c", 'i');
    printf("%c", 'a');
    printf("%c", 'g');
    printf("%c", ':');
    printf("%d", get(0,0) * get(1,1) * get(2,2) * get(3,3));

    /* Test 39: Min value */
    printf("%c", '|');
    printf("%c", 'm');
    printf("%c", 'i');
    printf("%c", 'n');
    printf("%c", '_');
    printf("%c", 'a');
    printf("%c", 'l');
    printf("%c", 'l');
    printf("%c", ':');
    printf("%d", min_all());

    /* Test 40: Max value */
    printf("%c", '|');
    printf("%c", 'm');
    printf("%c", 'a');
    printf("%c", 'x');
    printf("%c", '_');
    printf("%c", 'a');
    printf("%c", 'l');
    printf("%c", 'l');
    printf("%c", ':');
    printf("%d", max_all());

    /* Test 41: Unique rows */
    printf("%c", '|');
    printf("%c", 'u');
    printf("%c", 'n');
    printf("%c", 'i');
    printf("%c", 'q');
    printf("%c", 'u');
    printf("%c", 'e');
    printf("%c", '_');
    printf("%c", 'r');
    printf("%c", 'o');
    printf("%c", 'w');
    printf("%c", 's');
    printf("%c", ':');
    printf("%d", check_unique_rows());

    /* Test 42: Unique cols */
    printf("%c", '|');
    printf("%c", 'u');
    printf("%c", 'n');
    printf("%c", 'i');
    printf("%c", 'q');
    printf("%c", 'u');
    printf("%c", 'e');
    printf("%c", '_');
    printf("%c", 'c');
    printf("%c", 'o');
    printf("%c", 'l');
    printf("%c", 's');
    printf("%c", ':');
    printf("%d", check_unique_cols());

    /* Test 43: Total cells */
    printf("%c", '|');
    printf("%c", 't');
    printf("%c", 'o');
    printf("%c", 't');
    printf("%c", 'a');
    printf("%c", 'l');
    printf("%c", '_');
    printf("%c", 'c');
    printf("%c", 'e');
    printf("%c", 'l');
    printf("%c", 'l');
    printf("%c", 's');
    printf("%c", ':');
    printf("%d", 16);

    /* Test 44: Filled cells */
    printf("%c", '|');
    printf("%c", 'f');
    printf("%c", 'i');
    printf("%c", 'l');
    printf("%c", 'l');
    printf("%c", 'e');
    printf("%c", 'd');
    printf("%c", ':');
    printf("%d", count_filled_cells());

    /* Test 45: Empty cells in solved */
    printf("%c", '|');
    printf("%c", 'e');
    printf("%c", 'm');
    printf("%c", 'p');
    printf("%c", 't');
    printf("%c", 'y');
    printf("%c", ':');
    printf("%d", count_empty_cells());

    /* Done */
    printf("%c", '|');
    printf("%c", 'd');
    printf("%c", 'o');
    printf("%c", 'n');
    printf("%c", 'e');
    printf("%c", '\n');

    return 0;
}
