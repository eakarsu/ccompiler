int printf(const char *fmt, ...);
// EXPECT: Zigzag reorder:\n1 20 2 19 3 18 4 17 5 16 6 15 7 14 8 13 9 12 10 11 \nZigzag rows (3 rows):\nRow 0: 1 5 9 \nRow 1: 2 4 6 8 10 12 \nRow 2: 3 7 11 \nZigzag scan 4x4:\n1 2 5 9 6 3 4 7 10 13 14 11 8 12 15 16 \nBounce pattern: 0 1 2 3 4 5 6 5 4 3 2 1 0 1 2 3 4 5 6 5 
int main(void) {
    int arr[20];
    int i;
    for (i = 0; i < 20; i++) {
        arr[i] = i + 1;
    }

    printf("Zigzag reorder:\n");
    int zigzag[20];
    int left = 0;
    int right = 19;
    int idx = 0;
    while (left <= right) {
        zigzag[idx] = arr[left];
        idx++;
        left++;
        if (left <= right) {
            zigzag[idx] = arr[right];
            idx++;
            right--;
        }
    }
    for (i = 0; i < 20; i++) {
        printf("%d ", zigzag[i]);
    }
    printf("\n");

    printf("Zigzag rows (3 rows):\n");
    int rows = 3;
    int result[20];
    int row_idx[3];
    row_idx[0] = 0;
    row_idx[1] = 0;
    row_idx[2] = 0;
    int n = 12;
    int row = 0;
    int going_down = 1;
    int row_data[3][12];
    for (i = 0; i < n; i++) {
        row_data[row][row_idx[row]] = i + 1;
        row_idx[row]++;
        if (row == 0) {
            going_down = 1;
        } else if (row == rows - 1) {
            going_down = 0;
        }
        if (going_down) {
            row++;
        } else {
            row--;
        }
    }
    for (i = 0; i < rows; i++) {
        printf("Row %d: ", i);
        int j;
        for (j = 0; j < row_idx[i]; j++) {
            printf("%d ", row_data[i][j]);
        }
        printf("\n");
    }

    printf("Zigzag scan 4x4:\n");
    int grid[4][4];
    int val = 1;
    for (i = 0; i < 4; i++) {
        int j;
        for (j = 0; j < 4; j++) {
            grid[i][j] = val;
            val++;
        }
    }
    int d;
    for (d = 0; d < 7; d++) {
        int r;
        int c;
        if (d < 4) { r = 0; c = d; }
        else { r = d - 3; c = 3; }
        int items[4];
        int count = 0;
        while (r < 4 && c >= 0) {
            items[count] = grid[r][c];
            count++;
            r++;
            c--;
        }
        if (d % 2 == 0) {
            for (i = count - 1; i >= 0; i--) printf("%d ", items[i]);
        } else {
            for (i = 0; i < count; i++) printf("%d ", items[i]);
        }
    }
    printf("\n");

    printf("Bounce pattern: ");
    int pos = 0;
    int dir = 1;
    int width = 7;
    for (i = 0; i < 20; i++) {
        printf("%d ", pos);
        pos = pos + dir;
        if (pos >= width - 1 || pos <= 0) {
            dir = -dir;
        }
    }
    printf("\n");

    return 0;
}
