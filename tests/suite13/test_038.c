int printf(const char *fmt, ...);

// Flood Fill Algorithm on 2D Grid
// Implements iterative flood fill using a stack (no recursion to avoid deep stacks)

struct Point {
    int r;
    int c;
};

int grid[8][8];

void clear_grid() {
    int i;
    int j;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            grid[i][j] = 0;
        }
    }
}

int count_val(int val, int rows, int cols) {
    int count = 0;
    int i;
    int j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (grid[i][j] == val) count = count + 1;
        }
    }
    return count;
}

// Flood fill using explicit stack (iterative)
void flood_fill(int rows, int cols, int sr, int sc, int new_color) {
    int old_color = grid[sr][sc];
    if (old_color == new_color) return;

    struct Point stack[64];
    int top = 0;

    stack[top].r = sr;
    stack[top].c = sc;
    top = top + 1;

    while (top > 0) {
        top = top - 1;
        int r = stack[top].r;
        int c = stack[top].c;

        if (r < 0 || r >= rows || c < 0 || c >= cols) continue;
        if (grid[r][c] != old_color) continue;

        grid[r][c] = new_color;

        // Push 4-directional neighbors
        if (top + 4 <= 64) {
            stack[top].r = r - 1; stack[top].c = c; top = top + 1;
            stack[top].r = r + 1; stack[top].c = c; top = top + 1;
            stack[top].r = r; stack[top].c = c - 1; top = top + 1;
            stack[top].r = r; stack[top].c = c + 1; top = top + 1;
        }
    }
}

void print_grid(int rows, int cols) {
    int i;
    int j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (j > 0) printf(" ");
            printf("%d", grid[i][j]);
        }
        printf("\n");
    }
}

int main() {
    // Test 1: Fill a 4x4 empty grid from corner
    clear_grid();
    flood_fill(4, 4, 0, 0, 5);
    printf("fill empty count 5: %d\n", count_val(5, 4, 4));
    // EXPECT: fill empty count 5: 16

    // Test 2: Fill with barrier
    // Grid 5x5:
    // 0 0 0 0 0
    // 0 0 1 0 0
    // 0 1 1 1 0
    // 0 0 1 0 0
    // 0 0 0 0 0
    // Barrier (1) forms a diamond. Fill from (0,0) with 2. Should fill outer region.
    clear_grid();
    grid[1][2] = 1;
    grid[2][1] = 1; grid[2][2] = 1; grid[2][3] = 1;
    grid[3][2] = 1;

    flood_fill(5, 5, 0, 0, 2);
    printf("barrier fill 2s: %d\n", count_val(2, 5, 5));
    // EXPECT: barrier fill 2s: 20
    // 0s remain inside diamond: (1,1) has barrier neighbors... let me trace
    // Barrier cells: (1,2),(2,1),(2,2),(2,3),(3,2) = 5 cells of value 1
    // The 0-connected region from (0,0):
    // Row 0: all 5 -> 2
    // (1,0): connected to (0,0) -> 2
    // (1,1): connected to (1,0) -> 2 (not blocked, (1,1) is 0)
    // (1,3): (0,3)->(1,3) -> 2
    // (1,4): (0,4)->(1,4) -> 2
    // (2,0): (1,0)->(2,0) -> 2
    // (2,4): (1,4)->(2,4) -> 2
    // (3,0): (2,0)->(3,0) -> 2
    // (3,1): (3,0)->(3,1) -> 2
    // (3,3): (2,4)->(3,4)->(3,3) -> 2  Wait, (3,4) connected?
    // (3,4): (2,4)->(3,4) -> 2
    // (3,3): (3,4)->(3,3) -> 2
    // (4,0..4): row 4 all connected via (3,0)->(4,0), etc -> 2
    // Total 0s originally: 25 - 5 = 20. All 0s are connected? No inner region?
    // Actually the diamond doesn't enclose anything - (1,1) connects to (0,1), etc.
    // So all 20 zeros become 2.
    // Recount: count_val(2, 5, 5) = 20
    // I need to fix the EXPECT above.

    // Actually let me recount. 5 barrier cells with value 1.
    // Remaining 20 cells are 0. All connected because the diamond has gaps at corners.
    // So all 20 become 2.
    printf("barrier 1s remain: %d\n", count_val(1, 5, 5));
    // EXPECT: barrier 1s remain: 5
    printf("barrier 0s left: %d\n", count_val(0, 5, 5));
    // EXPECT: barrier 0s left: 0

    // Test 3: Enclosed region - closed box
    // 1 1 1 1 1
    // 1 0 0 0 1
    // 1 0 0 0 1
    // 1 0 0 0 1
    // 1 1 1 1 1
    clear_grid();
    int i;
    int j;
    for (i = 0; i < 5; i++) {
        grid[0][i] = 1;
        grid[4][i] = 1;
        grid[i][0] = 1;
        grid[i][4] = 1;
    }
    // Fill inside from (2,2) with 3
    flood_fill(5, 5, 2, 2, 3);
    printf("box inner 3s: %d\n", count_val(3, 5, 5));
    // EXPECT: box inner 3s: 9
    printf("box walls 1s: %d\n", count_val(1, 5, 5));
    // EXPECT: box walls 1s: 16

    // Test 4: Fill wall itself doesn't leak inside
    clear_grid();
    for (i = 0; i < 5; i++) {
        grid[0][i] = 1;
        grid[4][i] = 1;
        grid[i][0] = 1;
        grid[i][4] = 1;
    }
    // Fill border from (0,0) with 7
    flood_fill(5, 5, 0, 0, 7);
    printf("wall fill 7s: %d\n", count_val(7, 5, 5));
    // EXPECT: wall fill 7s: 16
    printf("inside still 0: %d\n", count_val(0, 5, 5));
    // EXPECT: inside still 0: 9

    // Test 5: Already same color, no change
    clear_grid();
    grid[2][2] = 5;
    flood_fill(4, 4, 0, 0, 0);
    printf("same color no-op: %d\n", count_val(0, 4, 4));
    // EXPECT: same color no-op: 15

    // Test 6: Single pixel fill
    clear_grid();
    flood_fill(1, 1, 0, 0, 9);
    printf("single pixel: %d\n", grid[0][0]);
    // EXPECT: single pixel: 9

    // Test 7: L-shaped region
    // 0 1 0
    // 0 1 0
    // 0 0 0
    clear_grid();
    grid[0][1] = 1;
    grid[1][1] = 1;
    // Fill from (0,0) with 4. Should only fill (0,0),(1,0),(2,0),(2,1),(2,2)
    // Wait: (0,0) connects to (1,0), (1,0) connects to (2,0)
    // (2,0) connects to (2,1), (2,1) connects to (2,2)
    // (2,2) connects to (1,2), (1,2) connects to (0,2)
    // So left side + bottom + right side all connected = 7 cells
    flood_fill(3, 3, 0, 0, 4);
    printf("L region 4s: %d\n", count_val(4, 3, 3));
    // EXPECT: L region 4s: 7
    printf("L walls 1s: %d\n", count_val(1, 3, 3));
    // EXPECT: L walls 1s: 2

    // Test 8: Horizontal wall splits grid
    // 0 0 0 0
    // 1 1 1 1
    // 0 0 0 0
    clear_grid();
    for (j = 0; j < 4; j++) {
        grid[1][j] = 1;
    }
    flood_fill(3, 4, 0, 0, 6);
    printf("top half 6s: %d\n", count_val(6, 3, 4));
    // EXPECT: top half 6s: 4
    printf("bottom still 0: %d\n", count_val(0, 3, 4));
    // EXPECT: bottom still 0: 4

    return 0;
}
