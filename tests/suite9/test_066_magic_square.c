int printf(const char *fmt, ...);

// Magic Square Validator and Generator
// A magic square has equal sums for all rows, columns, and diagonals

int square[25]; // up to 5x5

void set_sq(int n, int r, int c, int val) {
    square[r * n + c] = val;
}

int get_sq(int n, int r, int c) {
    return square[r * n + c];
}

int row_sum(int n, int r) {
    int sum = 0;
    int c;
    for (c = 0; c < n; c++) {
        sum += get_sq(n, r, c);
    }
    return sum;
}

int col_sum(int n, int c) {
    int sum = 0;
    int r;
    for (r = 0; r < n; r++) {
        sum += get_sq(n, r, c);
    }
    return sum;
}

int diag_sum(int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        sum += get_sq(n, i, i);
    }
    return sum;
}

int anti_diag_sum(int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        sum += get_sq(n, i, n - 1 - i);
    }
    return sum;
}

int is_magic(int n) {
    int target = row_sum(n, 0);
    int i;
    // Check all rows
    for (i = 0; i < n; i++) {
        if (row_sum(n, i) != target) return 0;
    }
    // Check all columns
    for (i = 0; i < n; i++) {
        if (col_sum(n, i) != target) return 0;
    }
    // Check diagonals
    if (diag_sum(n) != target) return 0;
    if (anti_diag_sum(n) != target) return 0;
    return 1;
}

int magic_constant(int n) {
    // Magic constant = n * (n^2 + 1) / 2
    return n * (n * n + 1) / 2;
}

// Generate odd-order magic square using Siamese method
void generate_odd_magic(int n) {
    int i;
    for (i = 0; i < n * n; i++) square[i] = 0;

    int r = 0;
    int c = n / 2;
    int num;
    for (num = 1; num <= n * n; num++) {
        set_sq(n, r, c, num);
        int nr = r - 1;
        int nc = c + 1;
        if (nr < 0) nr = n - 1;
        if (nc >= n) nc = 0;
        if (get_sq(n, nr, nc) != 0) {
            nr = r + 1;
            nc = c;
            if (nr >= n) nr = 0;
        }
        r = nr;
        c = nc;
    }
}

int has_all_numbers(int n) {
    int total = n * n;
    int seen[25];
    int i;
    for (i = 0; i < total; i++) seen[i] = 0;
    for (i = 0; i < total; i++) {
        int v = square[i];
        if (v < 1 || v > total) return 0;
        if (seen[v - 1]) return 0;
        seen[v - 1] = 1;
    }
    return 1;
}

void print_square(int n) {
    int r;
    int c;
    for (r = 0; r < n; r++) {
        for (c = 0; c < n; c++) {
            if (c > 0) printf(" ");
            printf("%d", get_sq(n, r, c));
        }
        printf("%s", "\n");
    }
}

int main(void) {
    // Test 1: Magic constant for 3x3
    printf("Magic constant 3x3: %d\n", magic_constant(3));
    // EXPECT: Magic constant 3x3: 15

    // Test 2: Magic constant for 5x5
    printf("Magic constant 5x5: %d\n", magic_constant(5));
    // EXPECT: Magic constant 5x5: 65

    // Test 3: Generate 3x3 magic square
    generate_odd_magic(3);
    int valid = is_magic(3);
    printf("3x3 is magic: %d\n", valid);
    // EXPECT: 3x3 is magic: 1

    // Test 4: Print 3x3 magic square
    printf("3x3 square:\n");
    // EXPECT: 3x3 square:
    print_square(3);
    // EXPECT: 8 1 6
    // EXPECT: 3 5 7
    // EXPECT: 4 9 2

    // Test 5: Verify sums
    printf("Row 0: %d Row 1: %d Row 2: %d\n", row_sum(3, 0), row_sum(3, 1), row_sum(3, 2));
    // EXPECT: Row 0: 15 Row 1: 15 Row 2: 15

    // Test 6: Column sums
    printf("Col 0: %d Col 1: %d Col 2: %d\n", col_sum(3, 0), col_sum(3, 1), col_sum(3, 2));
    // EXPECT: Col 0: 15 Col 1: 15 Col 2: 15

    // Test 7: Diagonal sums
    printf("Diag: %d Anti-diag: %d\n", diag_sum(3), anti_diag_sum(3));
    // EXPECT: Diag: 15 Anti-diag: 15

    // Test 8: Has all numbers 1-9
    int all = has_all_numbers(3);
    printf("Has all numbers: %d\n", all);
    // EXPECT: Has all numbers: 1

    // Test 9: Non-magic square detection
    set_sq(3, 0, 0, 1); set_sq(3, 0, 1, 2); set_sq(3, 0, 2, 3);
    set_sq(3, 1, 0, 4); set_sq(3, 1, 1, 5); set_sq(3, 1, 2, 6);
    set_sq(3, 2, 0, 7); set_sq(3, 2, 1, 8); set_sq(3, 2, 2, 9);
    valid = is_magic(3);
    printf("Sequential is magic: %d\n", valid);
    // EXPECT: Sequential is magic: 0

    // Test 10: Generate 5x5 magic square
    generate_odd_magic(5);
    valid = is_magic(5);
    printf("5x5 is magic: %d\n", valid);
    // EXPECT: 5x5 is magic: 1

    // Test 11: 5x5 row sums
    printf("5x5 row sums: %d %d %d %d %d\n",
           row_sum(5, 0), row_sum(5, 1), row_sum(5, 2), row_sum(5, 3), row_sum(5, 4));
    // EXPECT: 5x5 row sums: 65 65 65 65 65

    // Test 12: 5x5 has all numbers
    all = has_all_numbers(5);
    printf("5x5 has all numbers: %d\n", all);
    // EXPECT: 5x5 has all numbers: 1

    // Test 13: Print 5x5
    printf("5x5 square:\n");
    // EXPECT: 5x5 square:
    print_square(5);
    // EXPECT: 17 24 1 8 15
    // EXPECT: 23 5 7 14 16
    // EXPECT: 4 6 13 20 22
    // EXPECT: 10 12 19 21 3
    // EXPECT: 11 18 25 2 9

    printf("Magic square tests passed\n");
    // EXPECT: Magic square tests passed
    return 0;
}
