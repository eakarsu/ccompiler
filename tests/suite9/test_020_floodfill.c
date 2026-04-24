int printf(const char *fmt, ...);

// Flood fill algorithm using recursion

int grid[8][8];
int rows;
int cols;

void init_grid(void) {
    int i;
    int j;
    rows = 6;
    cols = 6;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            grid[i][j] = 0;
        }
    }
}

void set_border(void) {
    // Create a border/shape to fill around
    // Shape: a rectangle border from (1,1) to (4,4)
    int i;
    for (i = 1; i <= 4; i++) {
        grid[1][i] = 1;
        grid[4][i] = 1;
    }
    for (i = 1; i <= 4; i++) {
        grid[i][1] = 1;
        grid[i][4] = 1;
    }
}

void flood_fill(int r, int c, int old_color, int new_color) {
    if (r < 0 || r >= rows || c < 0 || c >= cols) return;
    if (grid[r][c] != old_color) return;
    grid[r][c] = new_color;
    flood_fill(r - 1, c, old_color, new_color);
    flood_fill(r + 1, c, old_color, new_color);
    flood_fill(r, c - 1, old_color, new_color);
    flood_fill(r, c + 1, old_color, new_color);
}

void print_grid(void) {
    int i;
    int j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%d", grid[i][j]);
        }
        printf("\n");
    }
}

int count_color(int color) {
    int i;
    int j;
    int cnt;
    cnt = 0;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (grid[i][j] == color) cnt = cnt + 1;
        }
    }
    return cnt;
}

int count_regions_visited[8][8];

void mark_region(int r, int c, int color) {
    if (r < 0 || r >= rows || c < 0 || c >= cols) return;
    if (grid[r][c] != color) return;
    if (count_regions_visited[r][c]) return;
    count_regions_visited[r][c] = 1;
    mark_region(r - 1, c, color);
    mark_region(r + 1, c, color);
    mark_region(r, c - 1, color);
    mark_region(r, c + 1, color);
}

int count_regions(int color) {
    int i;
    int j;
    int regions;
    regions = 0;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            count_regions_visited[i][j] = 0;
        }
    }
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (grid[i][j] == color && count_regions_visited[i][j] == 0) {
                mark_region(i, j, color);
                regions = regions + 1;
            }
        }
    }
    return regions;
}

int region_size_count;

void measure_region(int r, int c, int color) {
    if (r < 0 || r >= rows || c < 0 || c >= cols) return;
    if (grid[r][c] != color) return;
    if (count_regions_visited[r][c]) return;
    count_regions_visited[r][c] = 1;
    region_size_count = region_size_count + 1;
    measure_region(r - 1, c, color);
    measure_region(r + 1, c, color);
    measure_region(r, c - 1, color);
    measure_region(r, c + 1, color);
}

int main() {
    int cnt;
    int regions;
    int i;
    int j;

    // Test 1: basic flood fill
    init_grid();
    set_border();

    // EXPECT: Initial grid:
    printf("Initial grid:\n");
    // EXPECT: 000000
    // EXPECT: 011110
    // EXPECT: 010010
    // EXPECT: 010010
    // EXPECT: 011110
    // EXPECT: 000000
    print_grid();

    // Fill inside the rectangle with color 2
    flood_fill(2, 2, 0, 2);
    // EXPECT: After filling inside:
    printf("After filling inside:\n");
    // EXPECT: 000000
    // EXPECT: 011110
    // EXPECT: 012210
    // EXPECT: 012210
    // EXPECT: 011110
    // EXPECT: 000000
    print_grid();

    cnt = count_color(2);
    // EXPECT: Filled cells: 4
    printf("Filled cells: %d\n", cnt);

    // Fill outside with color 3
    flood_fill(0, 0, 0, 3);
    // EXPECT: After filling outside:
    printf("After filling outside:\n");
    // EXPECT: 333333
    // EXPECT: 311113
    // EXPECT: 312213
    // EXPECT: 312213
    // EXPECT: 311113
    // EXPECT: 333333
    print_grid();

    cnt = count_color(3);
    // EXPECT: Outside cells: 20
    printf("Outside cells: %d\n", cnt);

    // Test 2: count regions
    init_grid();
    // Create some separate islands
    grid[0][0] = 1; grid[0][1] = 1;
    grid[0][4] = 1; grid[0][5] = 1;
    grid[2][2] = 1; grid[2][3] = 1;
    grid[4][0] = 1;
    grid[5][5] = 1;

    regions = count_regions(1);
    // EXPECT: Regions of color 1: 5
    printf("Regions of color 1: %d\n", regions);

    regions = count_regions(0);
    // EXPECT: Regions of color 0: 1
    printf("Regions of color 0: %d\n", regions);

    // Test 3: measure region size
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            count_regions_visited[i][j] = 0;
        }
    }
    region_size_count = 0;
    measure_region(0, 0, 1);
    // EXPECT: Region at (0,0) size: 2
    printf("Region at (0,0) size: %d\n", region_size_count);

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            count_regions_visited[i][j] = 0;
        }
    }
    region_size_count = 0;
    measure_region(0, 2, 0);
    // EXPECT: Background region size: 28
    printf("Background region size: %d\n", region_size_count);

    // Test 4: fill all color-1 regions with color 5
    init_grid();
    grid[0][0] = 1; grid[0][1] = 1;
    grid[3][3] = 1; grid[3][4] = 1; grid[4][3] = 1;
    flood_fill(0, 0, 1, 5);
    flood_fill(3, 3, 1, 5);
    cnt = count_color(5);
    // EXPECT: Total color 5 after fills: 5
    printf("Total color 5 after fills: %d\n", cnt);

    // EXPECT: Flood fill done
    printf("Flood fill done\n");

    return 0;
}
