int printf(const char *fmt, ...);

// Saddle Point Detection in Matrix
// A saddle point is an element that is the minimum in its row and maximum in its column
// (or vice versa depending on convention; we use: min in row, max in column)

struct SaddleResult {
    int found;
    int row;
    int col;
    int val;
};

int row_min(int mat[5][5], int row, int cols) {
    int mn = mat[row][0];
    int j;
    for (j = 1; j < cols; j++) {
        if (mat[row][j] < mn) mn = mat[row][j];
    }
    return mn;
}

int col_max(int mat[5][5], int col, int rows) {
    int mx = mat[0][col];
    int i;
    for (i = 1; i < rows; i++) {
        if (mat[i][col] > mx) mx = mat[i][col];
    }
    return mx;
}

int row_max(int mat[5][5], int row, int cols) {
    int mx = mat[row][0];
    int j;
    for (j = 1; j < cols; j++) {
        if (mat[row][j] > mx) mx = mat[row][j];
    }
    return mx;
}

int col_min(int mat[5][5], int col, int rows) {
    int mn = mat[0][col];
    int i;
    for (i = 1; i < rows; i++) {
        if (mat[i][col] < mn) mn = mat[i][col];
    }
    return mn;
}

// Find first saddle point (min in row, max in col)
void find_saddle_min_row_max_col(int mat[5][5], int rows, int cols, struct SaddleResult *res) {
    res->found = 0;
    int i;
    int j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            int rmin = row_min(mat, i, cols);
            int cmax = col_max(mat, j, rows);
            if (mat[i][j] == rmin && mat[i][j] == cmax) {
                res->found = 1;
                res->row = i;
                res->col = j;
                res->val = mat[i][j];
                return;
            }
        }
    }
}

// Find first saddle point (max in row, min in col)
void find_saddle_max_row_min_col(int mat[5][5], int rows, int cols, struct SaddleResult *res) {
    res->found = 0;
    int i;
    int j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            int rmax = row_max(mat, i, cols);
            int cmin = col_min(mat, j, rows);
            if (mat[i][j] == rmax && mat[i][j] == cmin) {
                res->found = 1;
                res->row = i;
                res->col = j;
                res->val = mat[i][j];
                return;
            }
        }
    }
}

// Count all saddle points (min in row, max in col)
int count_saddle_points(int mat[5][5], int rows, int cols) {
    int count = 0;
    int i;
    int j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            int rmin = row_min(mat, i, cols);
            int cmax = col_max(mat, j, rows);
            if (mat[i][j] == rmin && mat[i][j] == cmax) {
                count = count + 1;
            }
        }
    }
    return count;
}

int main() {
    int mat[5][5];
    struct SaddleResult res;

    // Test 1: Classic saddle point example
    // 1 2 3
    // 4 5 6
    // 7 8 9
    // Row mins: 1, 4, 7
    // Col maxes: 7, 8, 9
    // Saddle (min in row, max in col): (2,0) = 7 (min of row 2 is 7, max of col 0 is 7)
    mat[0][0] = 1; mat[0][1] = 2; mat[0][2] = 3;
    mat[1][0] = 4; mat[1][1] = 5; mat[1][2] = 6;
    mat[2][0] = 7; mat[2][1] = 8; mat[2][2] = 9;

    find_saddle_min_row_max_col(mat, 3, 3, &res);
    printf("test1 found: %d\n", res.found);
    // EXPECT: test1 found: 1
    printf("test1 pos: %d %d\n", res.row, res.col);
    // EXPECT: test1 pos: 2 0
    printf("test1 val: %d\n", res.val);
    // EXPECT: test1 val: 7

    // Test 2: No saddle point
    // 1 2
    // 3 0
    // Row mins: 1, 0. Col maxes: 3, 2
    // (0,0)=1: rmin=1, cmax=3 -> 1!=3 no
    // (0,1)=2: rmin=1, cmax=2 -> 2!=1 no
    // (1,0)=3: rmin=0, cmax=3 -> 3!=0 no
    // (1,1)=0: rmin=0, cmax=2 -> 0!=2 no
    mat[0][0] = 1; mat[0][1] = 2;
    mat[1][0] = 3; mat[1][1] = 0;

    find_saddle_min_row_max_col(mat, 2, 2, &res);
    printf("test2 found: %d\n", res.found);
    // EXPECT: test2 found: 0

    // Test 3: Saddle point in middle
    // 10 20 30
    // 5  15 25
    // 1  10 20
    // Row mins: 10, 5, 1. Col maxes: 10, 20, 30
    // (0,0)=10: rmin=10, cmax=10 -> saddle!
    mat[0][0] = 10; mat[0][1] = 20; mat[0][2] = 30;
    mat[1][0] = 5;  mat[1][1] = 15; mat[1][2] = 25;
    mat[2][0] = 1;  mat[2][1] = 10; mat[2][2] = 20;

    find_saddle_min_row_max_col(mat, 3, 3, &res);
    printf("test3 found: %d\n", res.found);
    // EXPECT: test3 found: 1
    printf("test3 pos: %d %d\n", res.row, res.col);
    // EXPECT: test3 pos: 0 0
    printf("test3 val: %d\n", res.val);
    // EXPECT: test3 val: 10

    // Test 4: All same values - every element is a saddle point
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            mat[i][j] = 5;
        }
    }
    int cnt = count_saddle_points(mat, 3, 3);
    printf("test4 all same count: %d\n", cnt);
    // EXPECT: test4 all same count: 9

    // Test 5: Max-in-row, min-in-col saddle
    // 3 1 2
    // 9 7 8
    // 6 4 5
    // Row maxes: 3, 9, 6. Col mins: 3, 1, 2
    // (0,0)=3: rmax=3, cmin=3 -> saddle!
    mat[0][0] = 3; mat[0][1] = 1; mat[0][2] = 2;
    mat[1][0] = 9; mat[1][1] = 7; mat[1][2] = 8;
    mat[2][0] = 6; mat[2][1] = 4; mat[2][2] = 5;

    find_saddle_max_row_min_col(mat, 3, 3, &res);
    printf("test5 found: %d\n", res.found);
    // EXPECT: test5 found: 1
    printf("test5 pos: %d %d\n", res.row, res.col);
    // EXPECT: test5 pos: 0 0
    printf("test5 val: %d\n", res.val);
    // EXPECT: test5 val: 3

    // Test 6: 4x4 matrix saddle point
    //  1  2  3  4
    //  5  6  7  8
    //  9 10 11 12
    // 13 14 15 16
    // Row mins: 1, 5, 9, 13. Col maxes: 13, 14, 15, 16
    // (3,0)=13: rmin=13, cmax=13 -> saddle!
    int val = 1;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            mat[i][j] = val;
            val = val + 1;
        }
    }
    find_saddle_min_row_max_col(mat, 4, 4, &res);
    printf("test6 found: %d\n", res.found);
    // EXPECT: test6 found: 1
    printf("test6 pos: %d %d\n", res.row, res.col);
    // EXPECT: test6 pos: 3 0
    printf("test6 val: %d\n", res.val);
    // EXPECT: test6 val: 13

    // Test 7: Row/column stats on the 4x4
    printf("row0 min: %d\n", row_min(mat, 0, 4));
    // EXPECT: row0 min: 1
    printf("row3 min: %d\n", row_min(mat, 3, 4));
    // EXPECT: row3 min: 13
    printf("col0 max: %d\n", col_max(mat, 0, 4));
    // EXPECT: col0 max: 13
    printf("col3 max: %d\n", col_max(mat, 3, 4));
    // EXPECT: col3 max: 16

    // Test 8: Single element matrix - always a saddle point
    mat[0][0] = 42;
    find_saddle_min_row_max_col(mat, 1, 1, &res);
    printf("test8 1x1 found: %d\n", res.found);
    // EXPECT: test8 1x1 found: 1
    printf("test8 1x1 val: %d\n", res.val);
    // EXPECT: test8 1x1 val: 42

    // Test 9: Multiple saddle points
    // 7 7
    // 7 7
    // All same = 4 saddle points
    mat[0][0] = 7; mat[0][1] = 7;
    mat[1][0] = 7; mat[1][1] = 7;
    printf("test9 2x2 same count: %d\n", count_saddle_points(mat, 2, 2));
    // EXPECT: test9 2x2 same count: 4

    // Test 10: 5x1 column vector - min of row = only element, max of col = max element
    // So saddle is the max element
    mat[0][0] = 3;
    mat[1][0] = 7;
    mat[2][0] = 1;
    mat[3][0] = 9;
    mat[4][0] = 5;
    find_saddle_min_row_max_col(mat, 5, 1, &res);
    printf("test10 col vec found: %d\n", res.found);
    // EXPECT: test10 col vec found: 1
    printf("test10 col vec val: %d\n", res.val);
    // EXPECT: test10 col vec val: 9

    return 0;
}
