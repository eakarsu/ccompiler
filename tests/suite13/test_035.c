int printf(const char *fmt, ...);

// Spiral Order Traversal of Matrix
// Traverses a matrix in spiral order (clockwise from outside in)

int spiral_order(int mat[5][5], int rows, int cols, int *out) {
    int top = 0;
    int bottom = rows - 1;
    int left = 0;
    int right = cols - 1;
    int idx = 0;
    int i;

    while (top <= bottom && left <= right) {
        // Traverse right along top row
        for (i = left; i <= right; i++) {
            out[idx] = mat[top][i];
            idx = idx + 1;
        }
        top = top + 1;

        // Traverse down along right column
        for (i = top; i <= bottom; i++) {
            out[idx] = mat[i][right];
            idx = idx + 1;
        }
        right = right - 1;

        // Traverse left along bottom row
        if (top <= bottom) {
            for (i = right; i >= left; i--) {
                out[idx] = mat[bottom][i];
                idx = idx + 1;
            }
            bottom = bottom - 1;
        }

        // Traverse up along left column
        if (left <= right) {
            for (i = bottom; i >= top; i--) {
                out[idx] = mat[i][left];
                idx = idx + 1;
            }
            left = left + 1;
        }
    }
    return idx;
}

void print_array(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

int main() {
    int mat[5][5];
    int result[25];
    int count;
    int i;
    int j;

    // Test 1: 3x3 matrix
    // 1 2 3
    // 4 5 6
    // 7 8 9
    mat[0][0] = 1; mat[0][1] = 2; mat[0][2] = 3;
    mat[1][0] = 4; mat[1][1] = 5; mat[1][2] = 6;
    mat[2][0] = 7; mat[2][1] = 8; mat[2][2] = 9;

    count = spiral_order(mat, 3, 3, result);
    // Spiral: right(1,2,3), down(6,9), left(8,7), up(4), right(5)
    printf("3x3 count: %d\n", count);
    // EXPECT: 3x3 count: 9
    print_array(result, count);
    // EXPECT: 1 2 3 6 9 8 7 4 5

    // Test 2: 4x4 matrix
    // 1  2  3  4
    // 5  6  7  8
    // 9  10 11 12
    // 13 14 15 16
    int val = 1;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            mat[i][j] = val;
            val = val + 1;
        }
    }
    count = spiral_order(mat, 4, 4, result);
    printf("4x4 count: %d\n", count);
    // EXPECT: 4x4 count: 16
    // Spiral: right(1,2,3,4), down(8,12,16), left(15,14,13), up(9,5), right(6,7), down(11), left(10)
    print_array(result, count);
    // EXPECT: 1 2 3 4 8 12 16 15 14 13 9 5 6 7 11 10

    // Test 3: 1x1 matrix
    mat[0][0] = 42;
    count = spiral_order(mat, 1, 1, result);
    printf("1x1 count: %d\n", count);
    // EXPECT: 1x1 count: 1
    printf("1x1 val: %d\n", result[0]);
    // EXPECT: 1x1 val: 42

    // Test 4: 1x5 single row
    mat[0][0] = 10; mat[0][1] = 20; mat[0][2] = 30; mat[0][3] = 40; mat[0][4] = 50;
    count = spiral_order(mat, 1, 5, result);
    printf("1x5 count: %d\n", count);
    // EXPECT: 1x5 count: 5
    print_array(result, count);
    // EXPECT: 10 20 30 40 50

    // Test 5: 5x1 single column
    mat[0][0] = 2; mat[1][0] = 4; mat[2][0] = 6; mat[3][0] = 8; mat[4][0] = 10;
    count = spiral_order(mat, 5, 1, result);
    printf("5x1 count: %d\n", count);
    // EXPECT: 5x1 count: 5
    print_array(result, count);
    // EXPECT: 2 4 6 8 10

    // Test 6: 2x2 matrix
    mat[0][0] = 1; mat[0][1] = 2;
    mat[1][0] = 3; mat[1][1] = 4;
    count = spiral_order(mat, 2, 2, result);
    printf("2x2 count: %d\n", count);
    // EXPECT: 2x2 count: 4
    // Spiral: right(1,2), down(4), left(3)
    print_array(result, count);
    // EXPECT: 1 2 4 3

    // Test 7: 3x4 rectangular
    // 1  2  3  4
    // 5  6  7  8
    // 9 10 11 12
    val = 1;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            mat[i][j] = val;
            val = val + 1;
        }
    }
    count = spiral_order(mat, 3, 4, result);
    printf("3x4 count: %d\n", count);
    // EXPECT: 3x4 count: 12
    // Spiral: right(1,2,3,4), down(8,12), left(11,10,9), up(5), right(6,7)
    print_array(result, count);
    // EXPECT: 1 2 3 4 8 12 11 10 9 5 6 7

    // Test 8: Verify first and last elements
    printf("first: %d last: %d\n", result[0], result[count - 1]);
    // EXPECT: first: 1 last: 7

    // Test 9: 2x3 rectangular
    mat[0][0] = 10; mat[0][1] = 20; mat[0][2] = 30;
    mat[1][0] = 40; mat[1][1] = 50; mat[1][2] = 60;
    count = spiral_order(mat, 2, 3, result);
    printf("2x3 count: %d\n", count);
    // EXPECT: 2x3 count: 6
    // Spiral: right(10,20,30), down(60), left(50,40)
    print_array(result, count);
    // EXPECT: 10 20 30 60 50 40

    return 0;
}
