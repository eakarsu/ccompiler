int printf(const char *fmt, ...);

// Matrix Rotation (90, 180, 270 degrees clockwise)
// Rotates a 4x4 matrix in-place and verifies results

void print_matrix(int grid[4][4], int n) {
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (j > 0) printf(" ");
            printf("%d", grid[i][j]);
        }
        printf("\n");
    }
}

void copy_matrix(int src[4][4], int dst[4][4], int n) {
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            dst[i][j] = src[i][j];
        }
    }
}

// Rotate 90 degrees clockwise: new[j][n-1-i] = old[i][j]
// Equivalently: new[i][j] = old[n-1-j][i]
void rotate90(int grid[4][4], int n) {
    int temp[4][4];
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            temp[i][j] = grid[n - 1 - j][i];
        }
    }
    copy_matrix(temp, grid, n);
}

// Rotate 180 degrees: new[i][j] = old[n-1-i][n-1-j]
void rotate180(int grid[4][4], int n) {
    int temp[4][4];
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            temp[i][j] = grid[n - 1 - i][n - 1 - j];
        }
    }
    copy_matrix(temp, grid, n);
}

// Rotate 270 degrees clockwise (= 90 counter-clockwise)
// new[i][j] = old[j][n-1-i]
void rotate270(int grid[4][4], int n) {
    int temp[4][4];
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            temp[i][j] = grid[j][n - 1 - i];
        }
    }
    copy_matrix(temp, grid, n);
}

int matrices_equal(int a[4][4], int b[4][4], int n) {
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (a[i][j] != b[i][j]) return 0;
        }
    }
    return 1;
}

int main() {
    int grid[4][4];
    int orig[4][4];

    // Initialize 3x3 grid:
    // 1 2 3
    // 4 5 6
    // 7 8 9
    grid[0][0] = 1; grid[0][1] = 2; grid[0][2] = 3; grid[0][3] = 0;
    grid[1][0] = 4; grid[1][1] = 5; grid[1][2] = 6; grid[1][3] = 0;
    grid[2][0] = 7; grid[2][1] = 8; grid[2][2] = 9; grid[2][3] = 0;
    grid[3][0] = 0; grid[3][1] = 0; grid[3][2] = 0; grid[3][3] = 0;
    copy_matrix(grid, orig, 3);

    // Test 1: Rotate 90 clockwise
    // old[n-1-j][i]: new[0][0]=old[2][0]=7, new[0][1]=old[1][0]=4, new[0][2]=old[0][0]=1
    //                new[1][0]=old[2][1]=8, new[1][1]=old[1][1]=5, new[1][2]=old[0][1]=2
    //                new[2][0]=old[2][2]=9, new[2][1]=old[1][2]=6, new[2][2]=old[0][2]=3
    rotate90(grid, 3);
    printf("90cw row0: %d %d %d\n", grid[0][0], grid[0][1], grid[0][2]);
    // EXPECT: 90cw row0: 7 4 1
    printf("90cw row1: %d %d %d\n", grid[1][0], grid[1][1], grid[1][2]);
    // EXPECT: 90cw row1: 8 5 2
    printf("90cw row2: %d %d %d\n", grid[2][0], grid[2][1], grid[2][2]);
    // EXPECT: 90cw row2: 9 6 3

    // Test 2: Rotate another 90 (total 180)
    rotate90(grid, 3);
    printf("180 row0: %d %d %d\n", grid[0][0], grid[0][1], grid[0][2]);
    // EXPECT: 180 row0: 9 8 7
    printf("180 row1: %d %d %d\n", grid[1][0], grid[1][1], grid[1][2]);
    // EXPECT: 180 row1: 6 5 4
    printf("180 row2: %d %d %d\n", grid[2][0], grid[2][1], grid[2][2]);
    // EXPECT: 180 row2: 3 2 1

    // Test 3: Rotate another 90 (total 270)
    rotate90(grid, 3);
    printf("270 row0: %d %d %d\n", grid[0][0], grid[0][1], grid[0][2]);
    // EXPECT: 270 row0: 3 6 9
    printf("270 row1: %d %d %d\n", grid[1][0], grid[1][1], grid[1][2]);
    // EXPECT: 270 row1: 2 5 8
    printf("270 row2: %d %d %d\n", grid[2][0], grid[2][1], grid[2][2]);
    // EXPECT: 270 row2: 1 4 7

    // Test 4: One more 90 = back to original (360)
    rotate90(grid, 3);
    printf("360 == orig: %d\n", matrices_equal(grid, orig, 3));
    // EXPECT: 360 == orig: 1

    // Test 5: Direct rotate180
    copy_matrix(orig, grid, 3);
    rotate180(grid, 3);
    printf("direct 180 row0: %d %d %d\n", grid[0][0], grid[0][1], grid[0][2]);
    // EXPECT: direct 180 row0: 9 8 7
    printf("direct 180 row1: %d %d %d\n", grid[1][0], grid[1][1], grid[1][2]);
    // EXPECT: direct 180 row1: 6 5 4

    // Test 6: Direct rotate270
    copy_matrix(orig, grid, 3);
    rotate270(grid, 3);
    printf("direct 270 row0: %d %d %d\n", grid[0][0], grid[0][1], grid[0][2]);
    // EXPECT: direct 270 row0: 3 6 9
    printf("direct 270 row2: %d %d %d\n", grid[2][0], grid[2][1], grid[2][2]);
    // EXPECT: direct 270 row2: 1 4 7

    // Test 7: Rotate90 + Rotate270 = identity
    copy_matrix(orig, grid, 3);
    rotate90(grid, 3);
    rotate270(grid, 3);
    printf("90+270 == orig: %d\n", matrices_equal(grid, orig, 3));
    // EXPECT: 90+270 == orig: 1

    // Test 8: 4x4 rotation
    int g4[4][4];
    int o4[4][4];
    int val = 1;
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            g4[i][j] = val;
            val = val + 1;
        }
    }
    copy_matrix(g4, o4, 4);

    // Original 4x4:
    //  1  2  3  4
    //  5  6  7  8
    //  9 10 11 12
    // 13 14 15 16
    rotate90(g4, 4);
    // After 90cw: new[i][j] = old[n-1-j][i]
    // row0: old[3][0]=13, old[2][0]=9, old[1][0]=5, old[0][0]=1
    printf("4x4 90cw row0: %d %d %d %d\n", g4[0][0], g4[0][1], g4[0][2], g4[0][3]);
    // EXPECT: 4x4 90cw row0: 13 9 5 1
    // row3: old[3][3]=16, old[2][3]=12, old[1][3]=8, old[0][3]=4
    printf("4x4 90cw row3: %d %d %d %d\n", g4[3][0], g4[3][1], g4[3][2], g4[3][3]);
    // EXPECT: 4x4 90cw row3: 16 12 8 4

    // Four 90-degree rotations should restore original
    rotate90(g4, 4);
    rotate90(g4, 4);
    rotate90(g4, 4);
    printf("4x4 360 == orig: %d\n", matrices_equal(g4, o4, 4));
    // EXPECT: 4x4 360 == orig: 1

    return 0;
}
