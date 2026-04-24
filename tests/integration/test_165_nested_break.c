int printf(const char *fmt, ...);
// EXPECT: Break from inner loop:\nrow 0: 0 1 2 (broke at j=3)\nrow 1: 0 1 2 3 (broke at j=4)\nrow 2: 0 1 2 3 4 (broke at j=5)\nrow 3: 0 1 2 3 4 5 (broke at j=6)\nrow 4: 0 1 2 3 4 5 6 (broke at j=7)\nFind in 2D:\nfound 22 at (1,3)\nFirst row with sum > 30:\nrow 1 has sum 70\nNested 3-level break:\ntotal combinations=72\nBreak on condition change:\n3 10 6 2 9 5 1 8 4 0 \nfirst decrease at index 1
int main(void) {
    int i;
    int j;
    int k;

    printf("Break from inner loop:\n");
    for (i = 0; i < 5; i++) {
        printf("row %d: ", i);
        for (j = 0; j < 10; j++) {
            if (j > i + 2) {
                break;
            }
            printf("%d ", j);
        }
        printf("(broke at j=%d)\n", j);
    }

    printf("Find in 2D:\n");
    int grid[4][4];
    int val = 1;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            grid[i][j] = val;
            val = val + 3;
        }
    }
    int target = 22;
    int found_i = -1;
    int found_j = -1;
    for (i = 0; i < 4; i++) {
        int found = 0;
        for (j = 0; j < 4; j++) {
            if (grid[i][j] == target) {
                found_i = i;
                found_j = j;
                found = 1;
                break;
            }
        }
        if (found) break;
    }
    printf("found %d at (%d,%d)\n", target, found_i, found_j);

    printf("First row with sum > 30:\n");
    int row_sum;
    for (i = 0; i < 4; i++) {
        row_sum = 0;
        for (j = 0; j < 4; j++) {
            row_sum = row_sum + grid[i][j];
        }
        if (row_sum > 30) {
            break;
        }
    }
    printf("row %d has sum %d\n", i, row_sum);

    printf("Nested 3-level break:\n");
    int total = 0;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            for (k = 0; k < 5; k++) {
                if (i + j + k > 6) {
                    break;
                }
                total++;
            }
        }
    }
    printf("total combinations=%d\n", total);

    printf("Break on condition change:\n");
    int arr[10];
    for (i = 0; i < 10; i++) {
        arr[i] = (i * 7 + 3) % 11;
    }
    for (i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    for (i = 0; i < 9; i++) {
        if (arr[i] > arr[i + 1]) {
            break;
        }
    }
    printf("first decrease at index %d\n", i);

    return 0;
}
