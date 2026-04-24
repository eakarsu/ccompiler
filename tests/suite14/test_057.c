int printf(const char *fmt, ...);

// Minesweeper number generation
// Generate mine counts for each cell based on adjacent mines

typedef struct {
    int cells[10][10]; // 0=empty, -1=mine, positive=adjacent mine count
    int rows;
    int cols;
} MineField;

void field_init(MineField *f, int rows, int cols) {
    f->rows = rows;
    f->cols = cols;
    int r, c;
    for (r = 0; r < 10; r++)
        for (c = 0; c < 10; c++)
            f->cells[r][c] = 0;
}

void place_mine(MineField *f, int r, int c) {
    f->cells[r][c] = -1;
}

int is_mine(MineField *f, int r, int c) {
    if (r < 0 || r >= f->rows || c < 0 || c >= f->cols) return 0;
    return f->cells[r][c] == -1;
}

int count_adjacent_mines(MineField *f, int r, int c) {
    int count = 0;
    int dr, dc;
    for (dr = -1; dr <= 1; dr++) {
        for (dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            if (is_mine(f, r + dr, c + dc)) count++;
        }
    }
    return count;
}

void compute_numbers(MineField *f) {
    int r, c;
    for (r = 0; r < f->rows; r++) {
        for (c = 0; c < f->cols; c++) {
            if (f->cells[r][c] != -1) {
                f->cells[r][c] = count_adjacent_mines(f, r, c);
            }
        }
    }
}

int count_mines(MineField *f) {
    int count = 0;
    int r, c;
    for (r = 0; r < f->rows; r++)
        for (c = 0; c < f->cols; c++)
            if (f->cells[r][c] == -1) count++;
    return count;
}

int max_number(MineField *f) {
    int mx = 0;
    int r, c;
    for (r = 0; r < f->rows; r++) {
        for (c = 0; c < f->cols; c++) {
            if (f->cells[r][c] > mx) mx = f->cells[r][c];
        }
    }
    return mx;
}

int count_zeros(MineField *f) {
    int count = 0;
    int r, c;
    for (r = 0; r < f->rows; r++) {
        for (c = 0; c < f->cols; c++) {
            if (f->cells[r][c] == 0) count++;
        }
    }
    return count;
}

// Flood fill to count reachable non-mine cells from a zero cell
void flood_fill(MineField *f, int visited[10][10], int r, int c, int *count) {
    if (r < 0 || r >= f->rows || c < 0 || c >= f->cols) return;
    if (visited[r][c]) return;
    if (f->cells[r][c] == -1) return;
    visited[r][c] = 1;
    *count = *count + 1;
    if (f->cells[r][c] != 0) return; // stop expanding at numbered cells but count them
    int dr, dc;
    for (dr = -1; dr <= 1; dr++) {
        for (dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            flood_fill(f, visited, r + dr, c + dc, count);
        }
    }
}

int main() {
    // Test 1: Simple 5x5 field with mines in corners
    MineField f1;
    field_init(&f1, 5, 5);
    place_mine(&f1, 0, 0);
    place_mine(&f1, 0, 4);
    place_mine(&f1, 4, 0);
    place_mine(&f1, 4, 4);
    compute_numbers(&f1);

    printf("mines1: %d\n", count_mines(&f1));
    // EXPECT: mines1: 4
    // center (2,2): no adjacent mines
    printf("center: %d\n", f1.cells[2][2]);
    // EXPECT: center: 0
    // (1,1): adjacent to (0,0)=mine only => 1
    printf("near_corner: %d\n", f1.cells[1][1]);
    // EXPECT: near_corner: 1
    // (0,2): neighbors are (0,1),(0,3),(1,1),(1,2),(1,3) - no mines => 0
    printf("edge_top: %d\n", f1.cells[0][2]);
    // EXPECT: edge_top: 0
    // (0,1): adjacent to (0,0)=mine => 1
    printf("c01: %d\n", f1.cells[0][1]);
    // EXPECT: c01: 1
    // (1,0): adjacent to (0,0)=mine => 1
    printf("c10: %d\n", f1.cells[1][0]);
    // EXPECT: c10: 1

    // Test 2: Dense mine field 4x4
    MineField f2;
    field_init(&f2, 4, 4);
    place_mine(&f2, 0, 0);
    place_mine(&f2, 0, 1);
    place_mine(&f2, 0, 2);
    place_mine(&f2, 1, 0);
    compute_numbers(&f2);

    printf("mines2: %d\n", count_mines(&f2));
    // EXPECT: mines2: 4
    // (1,1) neighbors: (0,0)=M,(0,1)=M,(0,2)=M,(1,0)=M,(1,2),(2,0),(2,1),(2,2) => 4 mines
    printf("dense_11: %d\n", f2.cells[1][1]);
    // EXPECT: dense_11: 4
    // (1,2) neighbors: (0,1)=M,(0,2)=M,(0,3),(1,1),(1,3),(2,1),(2,2),(2,3) => 2 mines
    printf("dense_12: %d\n", f2.cells[1][2]);
    // EXPECT: dense_12: 2
    // (2,0) neighbors: (1,0)=M,(1,1),(2,1),(3,0),(3,1) => 1 mine
    printf("dense_20: %d\n", f2.cells[2][0]);
    // EXPECT: dense_20: 1
    // (3,3): neighbors (2,2),(2,3),(3,2) => 0 mines
    printf("dense_33: %d\n", f2.cells[3][3]);
    // EXPECT: dense_33: 0
    // (0,3): neighbors (0,2)=M,(1,2),(1,3) => 1 mine
    printf("dense_03: %d\n", f2.cells[0][3]);
    // EXPECT: dense_03: 1

    // Test 3: Full row of mines
    MineField f3;
    field_init(&f3, 3, 8);
    int i;
    for (i = 0; i < 8; i++) place_mine(&f3, 1, i);
    compute_numbers(&f3);
    printf("mines3: %d\n", count_mines(&f3));
    // EXPECT: mines3: 8
    // (0,0) neighbors: (0,1),(1,0)=M,(1,1)=M => 2
    printf("above_corner: %d\n", f3.cells[0][0]);
    // EXPECT: above_corner: 2
    // (0,3) neighbors: (0,2),(0,4),(1,2)=M,(1,3)=M,(1,4)=M => 3
    printf("above_mid: %d\n", f3.cells[0][3]);
    // EXPECT: above_mid: 3
    // (2,5) neighbors: (1,4)=M,(1,5)=M,(1,6)=M,(2,4),(2,6) => 3
    printf("below_mid: %d\n", f3.cells[2][5]);
    // EXPECT: below_mid: 3

    // Test 4: Single mine in center of 5x5
    MineField f4;
    field_init(&f4, 5, 5);
    place_mine(&f4, 2, 2);
    compute_numbers(&f4);

    printf("max_num: %d\n", max_number(&f4));
    // EXPECT: max_num: 1
    printf("zeros4: %d\n", count_zeros(&f4));
    // EXPECT: zeros4: 16

    // Flood fill from (0,0) which is 0
    // Zero cells are connected and numbered cells (=1) around the mine are boundaries
    // The 8 cells around (2,2) have value 1. All other non-mine cells are 0.
    // From (0,0)=0, flood fill expands through all zeros and stops at numbered cells (counting them too)
    // 25 total - 1 mine = 24 non-mine cells. All reachable from (0,0).
    int visited[10][10];
    int r, c;
    for (r = 0; r < 10; r++)
        for (c = 0; c < 10; c++)
            visited[r][c] = 0;
    int fill_count = 0;
    flood_fill(&f4, visited, 0, 0, &fill_count);
    printf("flood_count: %d\n", fill_count);
    // EXPECT: flood_count: 24

    // Test 5: Two mines side by side
    MineField f5;
    field_init(&f5, 3, 3);
    place_mine(&f5, 1, 0);
    place_mine(&f5, 1, 1);
    compute_numbers(&f5);
    // (0,0) neighbors: (0,1),(1,0)=M,(1,1)=M => 2
    printf("twom_00: %d\n", f5.cells[0][0]);
    // EXPECT: twom_00: 2
    // (1,2) neighbors: (0,1),(0,2),(1,1)=M,(2,1),(2,2) => 1
    printf("twom_12: %d\n", f5.cells[1][2]);
    // EXPECT: twom_12: 1
    // (2,0) neighbors: (1,0)=M,(1,1)=M,(2,1) => 2
    printf("twom_20: %d\n", f5.cells[2][0]);
    // EXPECT: twom_20: 2

    printf("done\n");
    // EXPECT: done
    return 0;
}
