int printf(const char *fmt, ...);
// EXPECT: Matrix:\n1 2 3 4\n5 6 7 8\n9 10 11 12\nrow 0 sum = 10\nrow 1 sum = 26\nrow 2 sum = 42\ncol 0 sum = 15\ncol 1 sum = 18\ncol 2 sum = 21\ncol 3 sum = 24\nflat: 1 2 3 4 5 6 7 8 9 10 11 12\ndiagonal: 1 1 1 
// Test: 2D arrays

int main(void) {
    int mat[3][4];
    int i;
    int j;

    // Initialize 2D array
    int val = 1;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            mat[i][j] = val;
            val = val + 1;
        }
    }

    // Print 2D array
    printf("Matrix:\n");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            if (j > 0) printf(" ");
            printf("%d", mat[i][j]);
        }
        printf("\n");
    }

    // Row sums
    for (i = 0; i < 3; i++) {
        int rsum = 0;
        for (j = 0; j < 4; j++) {
            rsum = rsum + mat[i][j];
        }
        printf("row %d sum = %d\n", i, rsum);
    }

    // Column sums
    for (j = 0; j < 4; j++) {
        int csum = 0;
        for (i = 0; i < 3; i++) {
            csum = csum + mat[i][j];
        }
        printf("col %d sum = %d\n", j, csum);
    }

    // Flatten 2D to 1D
    int flat[12];
    int k = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            flat[k] = mat[i][j];
            k = k + 1;
        }
    }
    printf("flat: ");
    for (i = 0; i < 12; i++) {
        if (i > 0) printf(" ");
        printf("%d", flat[i]);
    }
    printf("\n");

    // Identity-like check on square portion
    int sq[3][3];
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (i == j) sq[i][j] = 1;
            else sq[i][j] = 0;
        }
    }
    printf("diagonal: ");
    for (i = 0; i < 3; i++) {
        printf("%d ", sq[i][i]);
    }
    printf("\n");

    return 0;
}
