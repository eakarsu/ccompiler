int printf(const char *fmt, ...);

// Test: Multi-dimensional array access via pointers
// 2D array as array of arrays, accessed via pointer arithmetic

int main() {
    // 2D array: 3 rows x 4 cols
    int mat[3][4];
    int i;
    int j;
    int val = 1;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            mat[i][j] = val;
            val = val + 1;
        }
    }

    // Direct access
    // EXPECT: mat[0][0] = 1
    printf("mat[0][0] = %d\n", mat[0][0]);
    // EXPECT: mat[1][2] = 7
    printf("mat[1][2] = %d\n", mat[1][2]);
    // EXPECT: mat[2][3] = 12
    printf("mat[2][3] = %d\n", mat[2][3]);

    // Access via pointer to first element of flattened array
    int *flat = &mat[0][0];
    // mat[i][j] == flat[i*4 + j]
    // EXPECT: flat[0] = 1
    printf("flat[0] = %d\n", flat[0]);
    // EXPECT: flat[6] = 7
    printf("flat[6] = %d\n", flat[6]);
    // EXPECT: flat[11] = 12
    printf("flat[11] = %d\n", flat[11]);

    // Sum entire 2D array using flat pointer
    int sum = 0;
    for (i = 0; i < 12; i++) {
        sum = sum + flat[i];
    }
    // EXPECT: flat sum: 78
    printf("flat sum: %d\n", sum);

    // Access 2D array rows via pointer to array
    // mat[i] decays to pointer to int
    int *row0 = mat[0];
    int *row1 = mat[1];
    int *row2 = mat[2];
    // EXPECT: row0[1] = 2
    printf("row0[1] = %d\n", row0[1]);
    // EXPECT: row1[0] = 5
    printf("row1[0] = %d\n", row1[0]);
    // EXPECT: row2[2] = 11
    printf("row2[2] = %d\n", row2[2]);

    // Sum each row
    int rsum;
    for (i = 0; i < 3; i++) {
        rsum = 0;
        int *row = mat[i];
        for (j = 0; j < 4; j++) {
            rsum = rsum + row[j];
        }
        // EXPECT: row 0 sum: 10
        // EXPECT: row 1 sum: 26
        // EXPECT: row 2 sum: 42
        printf("row %d sum: %d\n", i, rsum);
    }

    // Simulate 2D access using 1D array + pointer arithmetic
    int grid[16];
    int cols = 4;
    int rows = 4;
    for (i = 0; i < 16; i++) {
        grid[i] = i * 10;
    }
    // Access grid[2][3] = grid[2*4+3] = grid[11] = 110
    int *gp = grid;
    int r = 2;
    int c = 3;
    int gval = *(gp + r * cols + c);
    // EXPECT: grid[2][3] = 110
    printf("grid[2][3] = %d\n", gval);

    // Diagonal of 4x4 grid
    int diag_sum = 0;
    for (i = 0; i < 4; i++) {
        diag_sum = diag_sum + *(gp + i * cols + i);
    }
    // EXPECT: diag sum: 300
    printf("diag sum: %d\n", diag_sum);

    // 3D array simulation: 2x3x2
    int cube[12];
    for (i = 0; i < 12; i++) {
        cube[i] = i + 1;
    }
    // cube[d1][d2][d3] = cube[d1*6 + d2*2 + d3]
    // cube[1][2][0] = cube[1*6 + 2*2 + 0] = cube[10] = 11
    int *cp = cube;
    int cv = *(cp + 1 * 6 + 2 * 2 + 0);
    // EXPECT: cube[1][2][0] = 11
    printf("cube[1][2][0] = %d\n", cv);

    // cube[0][1][1] = cube[0*6 + 1*2 + 1] = cube[3] = 4
    cv = *(cp + 0 * 6 + 1 * 2 + 1);
    // EXPECT: cube[0][1][1] = 4
    printf("cube[0][1][1] = %d\n", cv);

    // Transpose 3x4 into 4x3 using pointer access
    int trans[4][3];
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            trans[j][i] = mat[i][j];
        }
    }
    // EXPECT: trans[0][0] = 1
    printf("trans[0][0] = %d\n", trans[0][0]);
    // EXPECT: trans[2][1] = 7
    printf("trans[2][1] = %d\n", trans[2][1]);
    // EXPECT: trans[3][2] = 12
    printf("trans[3][2] = %d\n", trans[3][2]);

    // EXPECT: multidim done
    printf("multidim done\n");

    return 0;
}
