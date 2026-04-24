int printf(const char *fmt, ...);
// EXPECT: Before fill:\n0 0 0 0 0\n0 1 1 1 0\n0 1 0 1 0\n0 1 1 1 0\n0 0 0 0 0\ncomponent size at (1,1): 8\nAfter fill (1,1) -> 2:\n0 0 0 0 0\n0 2 2 2 0\n0 2 0 2 0\n0 2 2 2 0\n0 0 0 0 0\nzero region size from (0,0): 16\nAfter fill (2,2) -> 5:\n0 0 0 0 0\n0 2 2 2 0\n0 2 0 2 0\n0 2 2 2 0\n0 0 0 0 0
// Test: flood fill on 2D array (iterative with stack)

int grid[5][5];
int visited[5][5];

void print_grid(int rows, int cols) {
    int i; int j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (j > 0) printf(" ");
            printf("%d", grid[i][j]);
        }
        printf("\n");
    }
}

// Stack for iterative flood fill
int stack_r[50];
int stack_c[50];
int sp = -1;

void push(int r, int c) {
    sp = sp + 1;
    stack_r[sp] = r;
    stack_c[sp] = c;
}

void pop(int *r, int *c) {
    *r = stack_r[sp];
    *c = stack_c[sp];
    sp = sp - 1;
}

void flood_fill(int sr, int sc, int old_val, int new_val, int rows, int cols) {
    if (old_val == new_val) return;
    sp = -1;
    push(sr, sc);

    while (sp >= 0) {
        int r; int c;
        pop(&r, &c);
        if (r < 0 || r >= rows || c < 0 || c >= cols) continue;
        if (grid[r][c] != old_val) continue;

        grid[r][c] = new_val;
        push(r - 1, c);
        push(r + 1, c);
        push(r, c - 1);
        push(r, c + 1);
    }
}

// Count connected component size
int count_component(int sr, int sc, int val, int rows, int cols) {
    int i; int j;
    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
            visited[i][j] = 0;

    sp = -1;
    push(sr, sc);
    int count = 0;

    while (sp >= 0) {
        int r; int c;
        pop(&r, &c);
        if (r < 0 || r >= rows || c < 0 || c >= cols) continue;
        if (visited[r][c]) continue;
        if (grid[r][c] != val) continue;

        visited[r][c] = 1;
        count = count + 1;
        push(r - 1, c);
        push(r + 1, c);
        push(r, c - 1);
        push(r, c + 1);
    }
    return count;
}

int main(void) {
    // Set up grid
    int i; int j;
    for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++)
            grid[i][j] = 0;

    // Create a region of 1s
    grid[1][1] = 1; grid[1][2] = 1; grid[1][3] = 1;
    grid[2][1] = 1; grid[2][3] = 1;
    grid[3][1] = 1; grid[3][2] = 1; grid[3][3] = 1;

    printf("Before fill:\n");
    print_grid(5, 5);

    // Count component
    int size = count_component(1, 1, 1, 5, 5);
    printf("component size at (1,1): %d\n", size);

    // Flood fill from (1,1) with value 2
    flood_fill(1, 1, 1, 2, 5, 5);
    printf("After fill (1,1) -> 2:\n");
    print_grid(5, 5);

    // Fill the 0-region from (0,0)
    int zero_size = count_component(0, 0, 0, 5, 5);
    printf("zero region size from (0,0): %d\n", zero_size);

    // Fill center (2,2) region
    flood_fill(2, 2, 2, 5, 5, 5);
    printf("After fill (2,2) -> 5:\n");
    print_grid(5, 5);

    return 0;
}
