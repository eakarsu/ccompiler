int printf(const char *fmt, ...);
// EXPECT: Spiral matrix:\n 1  2  3  4  5 \n16 17 18 19  6 \n15 24 25 20  7 \n14 23 22 21  8 \n13 12 11 10  9 \nSpiral order readback: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 \nRing sums:\nring 0: 136\nring 1: 164\nring 2: 25\nCenter value: 25
int main(void) {
    int n = 5;
    int grid[5][5];
    int i;
    int j;

    int val = 1;
    int top = 0;
    int bottom = n - 1;
    int leftt = 0;
    int rightt = n - 1;

    while (top <= bottom && leftt <= rightt) {
        for (i = leftt; i <= rightt; i++) {
            grid[top][i] = val;
            val++;
        }
        top++;
        for (i = top; i <= bottom; i++) {
            grid[i][rightt] = val;
            val++;
        }
        rightt--;
        if (top <= bottom) {
            for (i = rightt; i >= leftt; i--) {
                grid[bottom][i] = val;
                val++;
            }
            bottom--;
        }
        if (leftt <= rightt) {
            for (i = bottom; i >= top; i--) {
                grid[i][leftt] = val;
                val++;
            }
            leftt++;
        }
    }

    printf("Spiral matrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%2d ", grid[i][j]);
        }
        printf("\n");
    }

    printf("Spiral order readback: ");
    top = 0;
    bottom = n - 1;
    leftt = 0;
    rightt = n - 1;
    while (top <= bottom && leftt <= rightt) {
        for (i = leftt; i <= rightt; i++) printf("%d ", grid[top][i]);
        top++;
        for (i = top; i <= bottom; i++) printf("%d ", grid[i][rightt]);
        rightt--;
        if (top <= bottom) {
            for (i = rightt; i >= leftt; i--) printf("%d ", grid[bottom][i]);
            bottom--;
        }
        if (leftt <= rightt) {
            for (i = bottom; i >= top; i--) printf("%d ", grid[i][leftt]);
            leftt++;
        }
    }
    printf("\n");

    printf("Ring sums:\n");
    int ring;
    for (ring = 0; ring < (n + 1) / 2; ring++) {
        int rsum = 0;
        for (i = ring; i < n - ring; i++) rsum = rsum + grid[ring][i];
        for (i = ring + 1; i < n - ring; i++) rsum = rsum + grid[i][n - 1 - ring];
        if (ring < n - 1 - ring) {
            for (i = n - 2 - ring; i >= ring; i--) rsum = rsum + grid[n - 1 - ring][i];
        }
        if (ring < n - 1 - ring) {
            for (i = n - 2 - ring; i > ring; i--) rsum = rsum + grid[i][ring];
        }
        printf("ring %d: %d\n", ring, rsum);
    }

    printf("Center value: %d\n", grid[n / 2][n / 2]);

    return 0;
}
