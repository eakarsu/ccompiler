int printf(const char *fmt, ...);
// EXPECT: === before fill ===\n00011000\n00100100\n01000010\n01000010\n01000010\n00100100\n00011000\n00000000\nzeros: 50\nones: 14\n=== fill inside with 2 ===\n00011000\n00122100\n01222210\n01222210\n01222210\n00122100\n00011000\n00000000\ntwos: 16\n=== fill border with 3 ===\n00033000\n00122100\n01222210\n01222210\n01222210\n00122100\n00011000\n00000000\nthrees: 2\n=== fill outside with 4 ===\n44433444\n44122144\n41222214\n41222214\n41222214\n44122144\n44411444\n44444444\nfours: 34
// Test 217: Recursive flood fill on 2D array

int grid[8][8];

void init_grid(void) {
    int i = 0;
    while (i < 8) {
        int j = 0;
        while (j < 8) {
            grid[i][j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }
    grid[0][3] = 1; grid[0][4] = 1;
    grid[1][2] = 1; grid[1][5] = 1;
    grid[2][1] = 1; grid[2][6] = 1;
    grid[3][1] = 1; grid[3][6] = 1;
    grid[4][1] = 1; grid[4][6] = 1;
    grid[5][2] = 1; grid[5][5] = 1;
    grid[6][3] = 1; grid[6][4] = 1;
}

void print_grid(void) {
    int i = 0;
    while (i < 8) {
        int j = 0;
        while (j < 8) {
            printf("%d", grid[i][j]);
            j = j + 1;
        }
        printf("\n");
        i = i + 1;
    }
}

void flood_fill(int r, int c, int old_color, int new_color) {
    if (r < 0 || r >= 8 || c < 0 || c >= 8) return;
    if (grid[r][c] != old_color) return;
    grid[r][c] = new_color;
    flood_fill(r + 1, c, old_color, new_color);
    flood_fill(r - 1, c, old_color, new_color);
    flood_fill(r, c + 1, old_color, new_color);
    flood_fill(r, c - 1, old_color, new_color);
}

int count_color(int color) {
    int count = 0;
    int i = 0;
    while (i < 8) {
        int j = 0;
        while (j < 8) {
            if (grid[i][j] == color) count = count + 1;
            j = j + 1;
        }
        i = i + 1;
    }
    return count;
}

int main(void) {
    init_grid();
    printf("=== before fill ===\n");
    print_grid();
    printf("zeros: %d\n", count_color(0));
    printf("ones: %d\n", count_color(1));

    printf("=== fill inside with 2 ===\n");
    flood_fill(3, 3, 0, 2);
    print_grid();
    printf("twos: %d\n", count_color(2));

    printf("=== fill border with 3 ===\n");
    flood_fill(0, 3, 1, 3);
    print_grid();
    printf("threes: %d\n", count_color(3));

    printf("=== fill outside with 4 ===\n");
    flood_fill(0, 0, 0, 4);
    print_grid();
    printf("fours: %d\n", count_color(4));

    return 0;
}
