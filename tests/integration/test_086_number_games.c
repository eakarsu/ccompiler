int printf(const char *fmt, ...);
// EXPECT: === Number Guessing Tests ===\nGuess 50 in [1,100]: 1\nGuess 1 in [1,100]: 6\nGuess 100 in [1,100]: 7\nGuess 42 in [1,100]: 7\nGuess 73 in [1,100]: 6\nMax guesses [1,16]: 5\nMax guesses [1,32]: 6\nMax guesses [1,64]: 7\nTotal guesses [1,8]: 21\nTotal guesses [1,16]: 54\n=== Magic Square Tests ===\nMagic3x3_1: 1\nConstant_1: 15\nUses1to9_1: 1\nMagic3x3_2: 1\nConstant_2: 15\nMagic3x3_3: 0\nMagic4x4: 1\nConstant4: 34\n=== Sudoku 4x4 Tests ===\nSudoku1 valid: 1\nSudoku1 row0: 1\nSudoku1 row1: 1\nSudoku1 col0: 1\nSudoku1 col1: 1\nSudoku2 valid: 1\nSudoku3 valid: 0\nSudoku3 row0: 0\nSudoku4 valid: 0\n=== Latin Square Tests ===\nLatin3x3_1: 1\nLatin3x3_2: 0\nLatin4x4: 1\nOrthogonal3: 1\nNotOrthogonal: 0\nLatin5x5: 1\n=== Number Property Tests ===\nPerfect 6: 1\nPerfect 28: 1\nPerfect 12: 0\nPerfect 496: 1\nAbundant 12: 1\nAbundant 18: 1\nAbundant 7: 0\nDeficient 8: 1\nDeficient 15: 1\nDigitSum 123: 6\nDigitSum 9999: 36\nDigitSum 0: 0\nDigRoot 493: 7\nDigRoot 9999: 9\nDigRoot 1: 1\nHappy 7: 1\nHappy 10: 1\nHappy 4: 0\nHappy 19: 1\nHappy 2: 0\nArmstrong 153: 1\nArmstrong 370: 1\nArmstrong 371: 1\nArmstrong 407: 1\nArmstrong 123: 0\nArmstrong count [100,999]: 4
// ============================================================
// test_086_number_games.c
// Number-based puzzles and games:
//   1. Deterministic number guessing (binary search)
//   2. Magic squares verification and properties
//   3. Sudoku validator (4x4)
//   4. Latin square check
//   5. Number properties and sequences
// ============================================================

// ============================================================
// 1. Deterministic Number Guessing (Binary Search)
// ============================================================

// Simulate a number guessing game using binary search
// The secret number is known; we simulate the search
int guess_number(int secret, int low, int high) {
    int guesses = 0;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        guesses++;
        if (mid == secret) {
            return guesses;
        } else if (mid < secret) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return guesses; // Should not reach here for valid input
}

// Count maximum guesses needed for range [1, n]
int max_guesses_for_range(int n) {
    int max = 0;
    int i;
    for (i = 1; i <= n; i++) {
        int g = guess_number(i, 1, n);
        if (g > max) max = g;
    }
    return max;
}

// Average guesses for range [1, n] (returns integer: total guesses)
int total_guesses_for_range(int n) {
    int total = 0;
    int i;
    for (i = 1; i <= n; i++) {
        total = total + guess_number(i, 1, n);
    }
    return total;
}

// ============================================================
// 2. Magic Square Verification
// ============================================================

// Check if a 3x3 grid is a magic square
// A magic square has all rows, columns, and diagonals sum to the same value
int is_magic_square_3(int sq[3][3]) {
    // Calculate target sum (first row)
    int target = sq[0][0] + sq[0][1] + sq[0][2];

    // Check all rows
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        int row_sum = 0;
        for (j = 0; j < 3; j++) {
            row_sum = row_sum + sq[i][j];
        }
        if (row_sum != target) return 0;
    }

    // Check all columns
    for (j = 0; j < 3; j++) {
        int col_sum = 0;
        for (i = 0; i < 3; i++) {
            col_sum = col_sum + sq[i][j];
        }
        if (col_sum != target) return 0;
    }

    // Check main diagonal
    int diag1 = sq[0][0] + sq[1][1] + sq[2][2];
    if (diag1 != target) return 0;

    // Check anti-diagonal
    int diag2 = sq[0][2] + sq[1][1] + sq[2][0];
    if (diag2 != target) return 0;

    return 1;
}

// Get the magic constant of a 3x3 square
int magic_constant_3(int sq[3][3]) {
    return sq[0][0] + sq[0][1] + sq[0][2];
}

// Check if a 3x3 magic square uses numbers 1-9
int uses_1_to_9(int sq[3][3]) {
    int used[10];
    int i;
    int j;
    for (i = 0; i < 10; i++) used[i] = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            int val = sq[i][j];
            if (val < 1 || val > 9) return 0;
            if (used[val]) return 0; // duplicate
            used[val] = 1;
        }
    }
    return 1;
}

// Check a 4x4 magic square
int is_magic_square_4(int sq[4][4]) {
    int target = sq[0][0] + sq[0][1] + sq[0][2] + sq[0][3];

    int i;
    int j;

    // Check rows
    for (i = 0; i < 4; i++) {
        int row_sum = 0;
        for (j = 0; j < 4; j++) {
            row_sum = row_sum + sq[i][j];
        }
        if (row_sum != target) return 0;
    }

    // Check columns
    for (j = 0; j < 4; j++) {
        int col_sum = 0;
        for (i = 0; i < 4; i++) {
            col_sum = col_sum + sq[i][j];
        }
        if (col_sum != target) return 0;
    }

    // Check diagonals
    int diag1 = 0;
    int diag2 = 0;
    for (i = 0; i < 4; i++) {
        diag1 = diag1 + sq[i][i];
        diag2 = diag2 + sq[i][3 - i];
    }
    if (diag1 != target) return 0;
    if (diag2 != target) return 0;

    return 1;
}

// ============================================================
// 3. Sudoku Validator (4x4)
// ============================================================

// Validate a completed 4x4 Sudoku
// Rules: Each row, column, and 2x2 box contains 1-4 exactly once
int validate_sudoku_4(int grid[4][4]) {
    int i;
    int j;
    int k;

    // Check each row
    for (i = 0; i < 4; i++) {
        int seen[5];
        for (k = 0; k < 5; k++) seen[k] = 0;
        for (j = 0; j < 4; j++) {
            int val = grid[i][j];
            if (val < 1 || val > 4) return 0;
            if (seen[val]) return 0;
            seen[val] = 1;
        }
    }

    // Check each column
    for (j = 0; j < 4; j++) {
        int seen[5];
        for (k = 0; k < 5; k++) seen[k] = 0;
        for (i = 0; i < 4; i++) {
            int val = grid[i][j];
            if (seen[val]) return 0;
            seen[val] = 1;
        }
    }

    // Check each 2x2 box
    int br;
    int bc;
    for (br = 0; br < 2; br++) {
        for (bc = 0; bc < 2; bc++) {
            int seen[5];
            for (k = 0; k < 5; k++) seen[k] = 0;
            for (i = br * 2; i < br * 2 + 2; i++) {
                for (j = bc * 2; j < bc * 2 + 2; j++) {
                    int val = grid[i][j];
                    if (seen[val]) return 0;
                    seen[val] = 1;
                }
            }
        }
    }

    return 1;
}

// Check if a row is valid
int sudoku_4_row_valid(int grid[4][4], int row) {
    int seen[5];
    int k;
    int j;
    for (k = 0; k < 5; k++) seen[k] = 0;
    for (j = 0; j < 4; j++) {
        int val = grid[row][j];
        if (val < 1 || val > 4 || seen[val]) return 0;
        seen[val] = 1;
    }
    return 1;
}

// Check if a column is valid
int sudoku_4_col_valid(int grid[4][4], int col) {
    int seen[5];
    int k;
    int i;
    for (k = 0; k < 5; k++) seen[k] = 0;
    for (i = 0; i < 4; i++) {
        int val = grid[i][col];
        if (val < 1 || val > 4 || seen[val]) return 0;
        seen[val] = 1;
    }
    return 1;
}

// ============================================================
// 4. Latin Square Check
// ============================================================

// A Latin square is an n x n array filled with n different symbols,
// each occurring exactly once in each row and column
int is_latin_square(int sq[5][5], int n) {
    int i;
    int j;
    int k;

    // Check rows: each row contains 1..n exactly once
    for (i = 0; i < n; i++) {
        int seen[10];
        for (k = 0; k < 10; k++) seen[k] = 0;
        for (j = 0; j < n; j++) {
            int val = sq[i][j];
            if (val < 1 || val > n) return 0;
            if (seen[val]) return 0;
            seen[val] = 1;
        }
    }

    // Check columns: each column contains 1..n exactly once
    for (j = 0; j < n; j++) {
        int seen[10];
        for (k = 0; k < 10; k++) seen[k] = 0;
        for (i = 0; i < n; i++) {
            int val = sq[i][j];
            if (seen[val]) return 0;
            seen[val] = 1;
        }
    }

    return 1;
}

// Check if two Latin squares are orthogonal
// Two Latin squares are orthogonal if every pair (a,b) appears exactly once
// when overlaid
int are_orthogonal_latin(int sq1[5][5], int sq2[5][5], int n) {
    // Check that each pair (sq1[i][j], sq2[i][j]) is unique
    int seen[10][10];
    int a;
    int b;
    for (a = 0; a < 10; a++) {
        for (b = 0; b < 10; b++) {
            seen[a][b] = 0;
        }
    }

    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            int v1 = sq1[i][j];
            int v2 = sq2[i][j];
            if (seen[v1][v2]) return 0;
            seen[v1][v2] = 1;
        }
    }
    return 1;
}

// ============================================================
// 5. Number Properties and Sequences
// ============================================================

// Check if a number is a perfect number (sum of proper divisors = number)
int is_perfect(int n) {
    if (n <= 1) return 0;
    int sum = 1; // 1 is always a divisor
    int i;
    for (i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            sum = sum + i;
            if (i != n / i) {
                sum = sum + n / i;
            }
        }
    }
    return sum == n;
}

// Check if a number is abundant (sum of proper divisors > number)
int is_abundant(int n) {
    if (n <= 1) return 0;
    int sum = 1;
    int i;
    for (i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            sum = sum + i;
            if (i != n / i) {
                sum = sum + n / i;
            }
        }
    }
    return sum > n;
}

// Check if a number is deficient (sum of proper divisors < number)
int is_deficient(int n) {
    if (n <= 1) return 0;
    int sum = 1;
    int i;
    for (i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            sum = sum + i;
            if (i != n / i) {
                sum = sum + n / i;
            }
        }
    }
    return sum < n;
}

// Sum of digits
int digit_sum(int n) {
    if (n < 0) n = -n;
    int sum = 0;
    while (n > 0) {
        sum = sum + n % 10;
        n = n / 10;
    }
    return sum;
}

// Digital root (repeated digit sum until single digit)
int digital_root(int n) {
    while (n >= 10) {
        n = digit_sum(n);
    }
    return n;
}

// Check if number is a happy number
// A happy number reaches 1 when repeatedly replacing with sum of squares of digits
int sum_of_digit_squares(int n) {
    int sum = 0;
    while (n > 0) {
        int d = n % 10;
        sum = sum + d * d;
        n = n / 10;
    }
    return sum;
}

int is_happy(int n) {
    // Use cycle detection with limited iterations
    int slow = n;
    int fast = n;
    int i;
    for (i = 0; i < 100; i++) {
        slow = sum_of_digit_squares(slow);
        fast = sum_of_digit_squares(sum_of_digit_squares(fast));
        if (slow == 1 || fast == 1) return 1;
        if (slow == fast) return 0; // cycle detected, not happy
    }
    return 0;
}

// Check if number is an Armstrong/narcissistic number
// For a 3-digit number: abc where a^3 + b^3 + c^3 = abc
int is_armstrong_3(int n) {
    if (n < 100 || n > 999) return 0;
    int a = n / 100;
    int b = (n / 10) % 10;
    int c = n % 10;
    return a * a * a + b * b * b + c * c * c == n;
}

// Count Armstrong numbers in a range
int count_armstrong_3(int low, int high) {
    int count = 0;
    int i;
    for (i = low; i <= high; i++) {
        if (is_armstrong_3(i)) count++;
    }
    return count;
}

// ============================================================
// Test functions
// ============================================================

void test_guessing() {
    printf("=== Number Guessing Tests ===\n");

    // Binary search in range [1, 100]
    printf("Guess 50 in [1,100]: %d\n", guess_number(50, 1, 100));
    printf("Guess 1 in [1,100]: %d\n", guess_number(1, 1, 100));
    printf("Guess 100 in [1,100]: %d\n", guess_number(100, 1, 100));
    printf("Guess 42 in [1,100]: %d\n", guess_number(42, 1, 100));
    printf("Guess 73 in [1,100]: %d\n", guess_number(73, 1, 100));

    // Max guesses for range
    printf("Max guesses [1,16]: %d\n", max_guesses_for_range(16));
    printf("Max guesses [1,32]: %d\n", max_guesses_for_range(32));
    printf("Max guesses [1,64]: %d\n", max_guesses_for_range(64));

    // Total guesses
    printf("Total guesses [1,8]: %d\n", total_guesses_for_range(8));
    printf("Total guesses [1,16]: %d\n", total_guesses_for_range(16));
}

void test_magic_square() {
    printf("=== Magic Square Tests ===\n");

    // Classic 3x3 magic square
    int sq1[3][3];
    sq1[0][0] = 2; sq1[0][1] = 7; sq1[0][2] = 6;
    sq1[1][0] = 9; sq1[1][1] = 5; sq1[1][2] = 1;
    sq1[2][0] = 4; sq1[2][1] = 3; sq1[2][2] = 8;

    printf("Magic3x3_1: %d\n", is_magic_square_3(sq1));
    printf("Constant_1: %d\n", magic_constant_3(sq1));
    printf("Uses1to9_1: %d\n", uses_1_to_9(sq1));

    // Another valid 3x3 magic square
    int sq2[3][3];
    sq2[0][0] = 8; sq2[0][1] = 1; sq2[0][2] = 6;
    sq2[1][0] = 3; sq2[1][1] = 5; sq2[1][2] = 7;
    sq2[2][0] = 4; sq2[2][1] = 9; sq2[2][2] = 2;

    printf("Magic3x3_2: %d\n", is_magic_square_3(sq2));
    printf("Constant_2: %d\n", magic_constant_3(sq2));

    // Invalid magic square
    int sq3[3][3];
    sq3[0][0] = 1; sq3[0][1] = 2; sq3[0][2] = 3;
    sq3[1][0] = 4; sq3[1][1] = 5; sq3[1][2] = 6;
    sq3[2][0] = 7; sq3[2][1] = 8; sq3[2][2] = 9;

    printf("Magic3x3_3: %d\n", is_magic_square_3(sq3));

    // 4x4 magic square (Durer's)
    int sq4[4][4];
    sq4[0][0] = 16; sq4[0][1] = 3; sq4[0][2] = 2; sq4[0][3] = 13;
    sq4[1][0] = 5; sq4[1][1] = 10; sq4[1][2] = 11; sq4[1][3] = 8;
    sq4[2][0] = 9; sq4[2][1] = 6; sq4[2][2] = 7; sq4[2][3] = 12;
    sq4[3][0] = 4; sq4[3][1] = 15; sq4[3][2] = 14; sq4[3][3] = 1;

    printf("Magic4x4: %d\n", is_magic_square_4(sq4));
    int constant4 = sq4[0][0] + sq4[0][1] + sq4[0][2] + sq4[0][3];
    printf("Constant4: %d\n", constant4);
}

void test_sudoku() {
    printf("=== Sudoku 4x4 Tests ===\n");

    // Valid 4x4 Sudoku
    int grid1[4][4];
    grid1[0][0] = 1; grid1[0][1] = 2; grid1[0][2] = 3; grid1[0][3] = 4;
    grid1[1][0] = 3; grid1[1][1] = 4; grid1[1][2] = 1; grid1[1][3] = 2;
    grid1[2][0] = 2; grid1[2][1] = 1; grid1[2][2] = 4; grid1[2][3] = 3;
    grid1[3][0] = 4; grid1[3][1] = 3; grid1[3][2] = 2; grid1[3][3] = 1;

    printf("Sudoku1 valid: %d\n", validate_sudoku_4(grid1));
    printf("Sudoku1 row0: %d\n", sudoku_4_row_valid(grid1, 0));
    printf("Sudoku1 row1: %d\n", sudoku_4_row_valid(grid1, 1));
    printf("Sudoku1 col0: %d\n", sudoku_4_col_valid(grid1, 0));
    printf("Sudoku1 col1: %d\n", sudoku_4_col_valid(grid1, 1));

    // Another valid 4x4 Sudoku
    int grid2[4][4];
    grid2[0][0] = 4; grid2[0][1] = 3; grid2[0][2] = 2; grid2[0][3] = 1;
    grid2[1][0] = 2; grid2[1][1] = 1; grid2[1][2] = 4; grid2[1][3] = 3;
    grid2[2][0] = 3; grid2[2][1] = 4; grid2[2][2] = 1; grid2[2][3] = 2;
    grid2[3][0] = 1; grid2[3][1] = 2; grid2[3][2] = 3; grid2[3][3] = 4;

    printf("Sudoku2 valid: %d\n", validate_sudoku_4(grid2));

    // Invalid 4x4 Sudoku (duplicate in row)
    int grid3[4][4];
    grid3[0][0] = 1; grid3[0][1] = 1; grid3[0][2] = 3; grid3[0][3] = 4;
    grid3[1][0] = 3; grid3[1][1] = 4; grid3[1][2] = 1; grid3[1][3] = 2;
    grid3[2][0] = 2; grid3[2][1] = 3; grid3[2][2] = 4; grid3[2][3] = 1;
    grid3[3][0] = 4; grid3[3][1] = 2; grid3[3][2] = 2; grid3[3][3] = 3;

    printf("Sudoku3 valid: %d\n", validate_sudoku_4(grid3));
    printf("Sudoku3 row0: %d\n", sudoku_4_row_valid(grid3, 0));

    // Invalid: valid rows/cols but invalid box
    int grid4[4][4];
    grid4[0][0] = 1; grid4[0][1] = 2; grid4[0][2] = 3; grid4[0][3] = 4;
    grid4[1][0] = 2; grid4[1][1] = 1; grid4[1][2] = 4; grid4[1][3] = 3;
    grid4[2][0] = 3; grid4[2][1] = 4; grid4[2][2] = 1; grid4[2][3] = 2;
    grid4[3][0] = 4; grid4[3][1] = 3; grid4[3][2] = 2; grid4[3][3] = 1;

    printf("Sudoku4 valid: %d\n", validate_sudoku_4(grid4));
}

void test_latin_square() {
    printf("=== Latin Square Tests ===\n");

    // Valid 3x3 Latin square
    int ls1[5][5];
    ls1[0][0] = 1; ls1[0][1] = 2; ls1[0][2] = 3;
    ls1[1][0] = 2; ls1[1][1] = 3; ls1[1][2] = 1;
    ls1[2][0] = 3; ls1[2][1] = 1; ls1[2][2] = 2;

    printf("Latin3x3_1: %d\n", is_latin_square(ls1, 3));

    // Invalid 3x3 Latin square
    int ls2[5][5];
    ls2[0][0] = 1; ls2[0][1] = 2; ls2[0][2] = 3;
    ls2[1][0] = 2; ls2[1][1] = 3; ls2[1][2] = 1;
    ls2[2][0] = 1; ls2[2][1] = 2; ls2[2][2] = 3; // duplicate column

    printf("Latin3x3_2: %d\n", is_latin_square(ls2, 3));

    // Valid 4x4 Latin square
    int ls3[5][5];
    ls3[0][0] = 1; ls3[0][1] = 2; ls3[0][2] = 3; ls3[0][3] = 4;
    ls3[1][0] = 2; ls3[1][1] = 1; ls3[1][2] = 4; ls3[1][3] = 3;
    ls3[2][0] = 3; ls3[2][1] = 4; ls3[2][2] = 1; ls3[2][3] = 2;
    ls3[3][0] = 4; ls3[3][1] = 3; ls3[3][2] = 2; ls3[3][3] = 1;

    printf("Latin4x4: %d\n", is_latin_square(ls3, 4));

    // Test orthogonal Latin squares
    int ols1[5][5];
    ols1[0][0] = 1; ols1[0][1] = 2; ols1[0][2] = 3;
    ols1[1][0] = 2; ols1[1][1] = 3; ols1[1][2] = 1;
    ols1[2][0] = 3; ols1[2][1] = 1; ols1[2][2] = 2;

    int ols2[5][5];
    ols2[0][0] = 1; ols2[0][1] = 2; ols2[0][2] = 3;
    ols2[1][0] = 3; ols2[1][1] = 1; ols2[1][2] = 2;
    ols2[2][0] = 2; ols2[2][1] = 3; ols2[2][2] = 1;

    printf("Orthogonal3: %d\n", are_orthogonal_latin(ols1, ols2, 3));

    // Non-orthogonal pair
    int ols3[5][5];
    ols3[0][0] = 1; ols3[0][1] = 2; ols3[0][2] = 3;
    ols3[1][0] = 2; ols3[1][1] = 3; ols3[1][2] = 1;
    ols3[2][0] = 3; ols3[2][1] = 1; ols3[2][2] = 2;

    // Same as ols1, so overlaying gives pairs (1,1),(2,2),(3,3),(2,2)... not all unique
    printf("NotOrthogonal: %d\n", are_orthogonal_latin(ols1, ols3, 3));

    // Valid 5x5 Latin square
    int ls5[5][5];
    ls5[0][0]=1; ls5[0][1]=2; ls5[0][2]=3; ls5[0][3]=4; ls5[0][4]=5;
    ls5[1][0]=2; ls5[1][1]=3; ls5[1][2]=4; ls5[1][3]=5; ls5[1][4]=1;
    ls5[2][0]=3; ls5[2][1]=4; ls5[2][2]=5; ls5[2][3]=1; ls5[2][4]=2;
    ls5[3][0]=4; ls5[3][1]=5; ls5[3][2]=1; ls5[3][3]=2; ls5[3][4]=3;
    ls5[4][0]=5; ls5[4][1]=1; ls5[4][2]=2; ls5[4][3]=3; ls5[4][4]=4;

    printf("Latin5x5: %d\n", is_latin_square(ls5, 5));
}

void test_number_properties() {
    printf("=== Number Property Tests ===\n");

    // Perfect numbers
    printf("Perfect 6: %d\n", is_perfect(6));
    printf("Perfect 28: %d\n", is_perfect(28));
    printf("Perfect 12: %d\n", is_perfect(12));
    printf("Perfect 496: %d\n", is_perfect(496));

    // Abundant numbers
    printf("Abundant 12: %d\n", is_abundant(12));
    printf("Abundant 18: %d\n", is_abundant(18));
    printf("Abundant 7: %d\n", is_abundant(7));

    // Deficient numbers
    printf("Deficient 8: %d\n", is_deficient(8));
    printf("Deficient 15: %d\n", is_deficient(15));

    // Digit sum
    printf("DigitSum 123: %d\n", digit_sum(123));
    printf("DigitSum 9999: %d\n", digit_sum(9999));
    printf("DigitSum 0: %d\n", digit_sum(0));

    // Digital root
    printf("DigRoot 493: %d\n", digital_root(493));
    printf("DigRoot 9999: %d\n", digital_root(9999));
    printf("DigRoot 1: %d\n", digital_root(1));

    // Happy numbers
    printf("Happy 7: %d\n", is_happy(7));
    printf("Happy 10: %d\n", is_happy(10));
    printf("Happy 4: %d\n", is_happy(4));
    printf("Happy 19: %d\n", is_happy(19));
    printf("Happy 2: %d\n", is_happy(2));

    // Armstrong numbers
    printf("Armstrong 153: %d\n", is_armstrong_3(153));
    printf("Armstrong 370: %d\n", is_armstrong_3(370));
    printf("Armstrong 371: %d\n", is_armstrong_3(371));
    printf("Armstrong 407: %d\n", is_armstrong_3(407));
    printf("Armstrong 123: %d\n", is_armstrong_3(123));
    printf("Armstrong count [100,999]: %d\n", count_armstrong_3(100, 999));
}


int main() {
    test_guessing();
    test_magic_square();
    test_sudoku();
    test_latin_square();
    test_number_properties();
    return 0;
}
