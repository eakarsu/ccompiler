int printf(const char *fmt, ...);

// Minesweeper grid generation and logic
// Grid: 8x8, mines=1, numbers=0-8, hidden/revealed state

int mines[64];     // 1=mine, 0=safe
int numbers[64];   // adjacent mine count
int revealed[64];  // 0=hidden, 1=revealed
int flagged[64];   // 0=not flagged, 1=flagged
int grid_size;
int seed;

int next_rand(void) {
    seed = seed * 1103515245 + 12345;
    int val = seed;
    if (val < 0) val = -val;
    return val;
}

void init_grid(int size) {
    grid_size = size;
    int i;
    for (i = 0; i < size * size; i++) {
        mines[i] = 0;
        numbers[i] = 0;
        revealed[i] = 0;
        flagged[i] = 0;
    }
}

void place_mines(int count) {
    int placed = 0;
    while (placed < count) {
        int pos = next_rand() % (grid_size * grid_size);
        if (mines[pos] == 0) {
            mines[pos] = 1;
            placed++;
        }
    }
}

void calc_numbers(void) {
    int r;
    int c;
    int n = grid_size;
    for (r = 0; r < n; r++) {
        for (c = 0; c < n; c++) {
            if (mines[r * n + c]) {
                numbers[r * n + c] = -1; // mine marker
                continue;
            }
            int count = 0;
            int dr;
            int dc;
            for (dr = -1; dr <= 1; dr++) {
                for (dc = -1; dc <= 1; dc++) {
                    if (dr == 0 && dc == 0) continue;
                    int nr = r + dr;
                    int nc = c + dc;
                    if (nr >= 0 && nr < n && nc >= 0 && nc < n) {
                        if (mines[nr * n + nc]) count++;
                    }
                }
            }
            numbers[r * n + c] = count;
        }
    }
}

int count_mines(void) {
    int count = 0;
    int i;
    for (i = 0; i < grid_size * grid_size; i++) {
        if (mines[i]) count++;
    }
    return count;
}

// Flood-fill reveal (reveal blank cells and their neighbors)
void reveal_cell(int r, int c) {
    int n = grid_size;
    if (r < 0 || r >= n || c < 0 || c >= n) return;
    int idx = r * n + c;
    if (revealed[idx]) return;
    if (flagged[idx]) return;
    revealed[idx] = 1;
    if (numbers[idx] == 0 && mines[idx] == 0) {
        int dr;
        int dc;
        for (dr = -1; dr <= 1; dr++) {
            for (dc = -1; dc <= 1; dc++) {
                if (dr == 0 && dc == 0) continue;
                reveal_cell(r + dr, c + dc);
            }
        }
    }
}

int count_revealed(void) {
    int count = 0;
    int i;
    for (i = 0; i < grid_size * grid_size; i++) {
        if (revealed[i]) count++;
    }
    return count;
}

int is_mine(int r, int c) {
    return mines[r * grid_size + c];
}

int get_number(int r, int c) {
    return numbers[r * grid_size + c];
}

void toggle_flag(int r, int c) {
    int idx = r * grid_size + c;
    if (revealed[idx]) return;
    flagged[idx] = 1 - flagged[idx];
}

int count_flags(void) {
    int count = 0;
    int i;
    for (i = 0; i < grid_size * grid_size; i++) {
        if (flagged[i]) count++;
    }
    return count;
}

// Check win: all non-mine cells revealed
int check_win(void) {
    int i;
    int n = grid_size * grid_size;
    for (i = 0; i < n; i++) {
        if (mines[i] == 0 && revealed[i] == 0) return 0;
    }
    return 1;
}

void print_grid(void) {
    int r;
    int c;
    int n = grid_size;
    for (r = 0; r < n; r++) {
        for (c = 0; c < n; c++) {
            int idx = r * n + c;
            if (mines[idx]) printf("%s", "*");
            else printf("%d", numbers[idx]);
        }
        printf("%s", "\n");
    }
}

// Count cells with a specific number
int count_number_cells(int num) {
    int count = 0;
    int i;
    for (i = 0; i < grid_size * grid_size; i++) {
        if (numbers[i] == num && mines[i] == 0) count++;
    }
    return count;
}

int main(void) {
    // Test 1: Initialize 4x4 grid
    init_grid(4);
    printf("Grid initialized: size=%d\n", grid_size);
    // EXPECT: Grid initialized: size=4

    // Test 2: Place mines deterministically
    seed = 42;
    place_mines(3);
    int mc = count_mines();
    printf("Mines placed: %d\n", mc);
    // EXPECT: Mines placed: 3

    // Test 3: Calculate numbers
    calc_numbers();
    // Print the grid to see layout
    printf("4x4 grid:\n");
    // EXPECT: 4x4 grid:
    print_grid();
    // EXPECT: 1110
    // EXPECT: 2*10
    // EXPECT: *221
    // EXPECT: 111*

    // Test 4: Check specific numbers
    printf("(0,0)=%d (0,2)=%d (1,1)=%d\n", get_number(0, 0), get_number(0, 2), get_number(1, 1));
    // EXPECT: (0,0)=1 (0,2)=1 (1,1)=-1

    // Test 5: Mine check
    printf("Is mine (0,1): %d\n", is_mine(0, 1));
    // EXPECT: Is mine (0,1): 0
    printf("Is mine (0,0): %d\n", is_mine(0, 0));
    // EXPECT: Is mine (0,0): 0

    // Test 6: Reveal a safe cell
    reveal_cell(0, 0);
    int rev = count_revealed();
    printf("After reveal (0,0): %d cells\n", rev);
    // EXPECT: After reveal (0,0): 1 cells

    // Test 7: Flood fill from blank area
    reveal_cell(0, 3);
    rev = count_revealed();
    printf("After reveal (0,3): %d cells\n", rev);
    // EXPECT: After reveal (0,3): 7 cells

    // Test 8: Flag a mine
    toggle_flag(0, 1);
    printf("Flags: %d\n", count_flags());
    // EXPECT: Flags: 1

    // Test 9: Unflag
    toggle_flag(0, 1);
    printf("Flags after unflag: %d\n", count_flags());
    // EXPECT: Flags after unflag: 0

    // Test 10: Win check (not won yet)
    printf("Won: %d\n", check_win());
    // EXPECT: Won: 0

    // Test 11: Reveal all remaining safe cells
    reveal_cell(0, 0);
    reveal_cell(0, 1);
    reveal_cell(0, 2);
    reveal_cell(1, 0);
    reveal_cell(1, 2);
    reveal_cell(1, 3);
    reveal_cell(2, 1);
    reveal_cell(2, 2);
    reveal_cell(2, 3);
    reveal_cell(3, 0);
    reveal_cell(3, 1);
    reveal_cell(3, 2);
    rev = count_revealed();
    printf("Total revealed: %d\n", rev);
    // EXPECT: Total revealed: 13
    printf("Won now: %d\n", check_win());
    // EXPECT: Won now: 1

    // Test 12: New 6x6 grid
    init_grid(6);
    seed = 100;
    place_mines(5);
    calc_numbers();
    mc = count_mines();
    printf("6x6 mines: %d\n", mc);
    // EXPECT: 6x6 mines: 5

    // Test 13: Count blank cells (number 0) in 6x6
    int blanks = count_number_cells(0);
    printf("Blank cells: %d\n", blanks);
    // EXPECT: Blank cells: 15

    // Test 14: Print 6x6
    printf("6x6 grid:\n");
    // EXPECT: 6x6 grid:
    print_grid();
    // EXPECT: 01*100
    // EXPECT: 011100
    // EXPECT: 110000
    // EXPECT: *31000
    // EXPECT: **1011
    // EXPECT: 22101*

    printf("Minesweeper tests passed\n");
    // EXPECT: Minesweeper tests passed
    return 0;
}
