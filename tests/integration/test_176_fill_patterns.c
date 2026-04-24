int printf(const char *fmt, ...);
// EXPECT: Alternating fill: 1 -1 1 -1 1 -1 1 -1 1 -1 1 -1 1 -1 1 -1 1 -1 1 -1 \nStaircase fill: 0 0 0 0 1 1 1 1 2 2 2 2 3 3 3 3 4 4 4 4 \nTriangular wave: 0 1 2 3 4 3 2 1 0 1 2 3 4 3 2 1 0 1 2 3 \nCheckerboard 4x5:\n0 1 0 1 0 \n1 0 1 0 1 \n0 1 0 1 0 \n1 0 1 0 1 \nBorder fill:\n1 1 1 1 1 \n1 0 0 0 1 \n1 0 0 0 1 \n1 0 0 0 1 \n1 1 1 1 1 \nDiamond fill:\n0 0 1 0 0 \n0 1 1 1 0 \n1 1 1 1 1 \n0 1 1 1 0 \n0 0 1 0 0 
int main(void) {
    int arr[20];
    int i;
    int j;

    printf("Alternating fill: ");
    for (i = 0; i < 20; i++) {
        if (i % 2 == 0) {
            arr[i] = 1;
        } else {
            arr[i] = -1;
        }
    }
    for (i = 0; i < 20; i++) printf("%d ", arr[i]);
    printf("\n");

    printf("Staircase fill: ");
    for (i = 0; i < 20; i++) {
        arr[i] = i / 4;
    }
    for (i = 0; i < 20; i++) printf("%d ", arr[i]);
    printf("\n");

    printf("Triangular wave: ");
    int period = 8;
    for (i = 0; i < 20; i++) {
        int pos = i % period;
        if (pos < period / 2) {
            arr[i] = pos;
        } else {
            arr[i] = period - pos;
        }
    }
    for (i = 0; i < 20; i++) printf("%d ", arr[i]);
    printf("\n");

    printf("Checkerboard 4x5:\n");
    int board[4][5];
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 5; j++) {
            board[i][j] = (i + j) % 2;
        }
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 5; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }

    printf("Border fill:\n");
    int grid[5][5];
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            if (i == 0 || i == 4 || j == 0 || j == 4) {
                grid[i][j] = 1;
            } else {
                grid[i][j] = 0;
            }
        }
    }
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            printf("%d ", grid[i][j]);
        }
        printf("\n");
    }

    printf("Diamond fill:\n");
    int size = 5;
    int center = size / 2;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            int dist = (i - center);
            if (dist < 0) dist = -dist;
            int dist2 = (j - center);
            if (dist2 < 0) dist2 = -dist2;
            if (dist + dist2 <= center) {
                grid[i][j] = 1;
            } else {
                grid[i][j] = 0;
            }
        }
    }
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            printf("%d ", grid[i][j]);
        }
        printf("\n");
    }

    return 0;
}
