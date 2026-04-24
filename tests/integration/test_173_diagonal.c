int printf(const char *fmt, ...);
// EXPECT: Grid:\n 1  2  3  4  5 \n 6  7  8  9 10 \n11 12 13 14 15 \n16 17 18 19 20 \n21 22 23 24 25 \nMain diagonal: 1 7 13 19 25 \nAnti-diagonal: 5 9 13 17 21 \nAll diagonals (top-left to bottom-right):\nd0: 1 \nd1: 2 6 \nd2: 3 7 11 \nd3: 4 8 12 16 \nd4: 5 9 13 17 21 \nd5: 10 14 18 22 \nd6: 15 19 23 \nd7: 20 24 \nd8: 25 \nDiagonal sums:\nd0=1 d1=8 d2=21 d3=40 d4=65 d5=64 d6=57 d7=44 d8=25 \nZigzag diagonal: 1 6 2 3 7 11 16 12 8 4 5 9 13 17 21 22 18 14 10 15 19 23 24 20 25 
int main(void) {
    int grid[5][5];
    int i;
    int j;
    int val = 1;

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            grid[i][j] = val;
            val++;
        }
    }

    printf("Grid:\n");
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            printf("%2d ", grid[i][j]);
        }
        printf("\n");
    }

    printf("Main diagonal: ");
    for (i = 0; i < 5; i++) {
        printf("%d ", grid[i][i]);
    }
    printf("\n");

    printf("Anti-diagonal: ");
    for (i = 0; i < 5; i++) {
        printf("%d ", grid[i][4 - i]);
    }
    printf("\n");

    printf("All diagonals (top-left to bottom-right):\n");
    int d;
    for (d = 0; d < 9; d++) {
        int r;
        int c;
        if (d < 5) {
            r = 0;
            c = d;
        } else {
            r = d - 4;
            c = 4;
        }
        printf("d%d: ", d);
        while (r < 5 && c >= 0) {
            printf("%d ", grid[r][c]);
            r++;
            c--;
        }
        printf("\n");
    }

    printf("Diagonal sums:\n");
    int diag_sum;
    for (d = 0; d < 9; d++) {
        int r;
        int c;
        diag_sum = 0;
        if (d < 5) {
            r = 0;
            c = d;
        } else {
            r = d - 4;
            c = 4;
        }
        while (r < 5 && c >= 0) {
            diag_sum = diag_sum + grid[r][c];
            r++;
            c--;
        }
        printf("d%d=%d ", d, diag_sum);
    }
    printf("\n");

    printf("Zigzag diagonal: ");
    for (d = 0; d < 9; d++) {
        int r;
        int c;
        int items[5];
        int count = 0;
        if (d < 5) {
            r = 0;
            c = d;
        } else {
            r = d - 4;
            c = 4;
        }
        while (r < 5 && c >= 0) {
            items[count] = grid[r][c];
            count++;
            r++;
            c--;
        }
        if (d % 2 == 0) {
            for (i = 0; i < count; i++) {
                printf("%d ", items[i]);
            }
        } else {
            for (i = count - 1; i >= 0; i--) {
                printf("%d ", items[i]);
            }
        }
    }
    printf("\n");

    return 0;
}
