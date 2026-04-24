int printf(const char *fmt, ...);
// EXPECT: 3x3 spiral: 1 2 3 6 9 8 7 4 5\n4x4 spiral: 1 2 3 4 8 12 16 15 14 13 9 5 6 7 11 10\n3x4 spiral: 1 2 3 4 8 12 11 10 9 5 6 7\n1x5 spiral: 1 2 3 4 5\n5x1 spiral: 10 20 30 40 50\n2x2 spiral: 1 2 4 3\n1x1 spiral: 42\n2x3 spiral: 1 2 3 6 5 4
// Test: spiral print of 2D array

void spiral_print(int *m, int rows, int cols) {
    int top = 0;
    int bottom = rows - 1;
    int left = 0;
    int right = cols - 1;
    int first = 1;
    int i;

    while (top <= bottom && left <= right) {
        // Left to right
        for (i = left; i <= right; i++) {
            if (!first) printf(" ");
            printf("%d", *(m + top * cols + i));
            first = 0;
        }
        top = top + 1;

        // Top to bottom
        for (i = top; i <= bottom; i++) {
            printf(" %d", *(m + i * cols + right));
        }
        right = right - 1;

        // Right to left
        if (top <= bottom) {
            for (i = right; i >= left; i--) {
                printf(" %d", *(m + bottom * cols + i));
            }
            bottom = bottom - 1;
        }

        // Bottom to top
        if (left <= right) {
            for (i = bottom; i >= top; i--) {
                printf(" %d", *(m + i * cols + left));
            }
            left = left + 1;
        }
    }
    printf("\n");
}

int main(void) {
    // 3x3 matrix
    int a[9];
    int i;
    for (i = 0; i < 9; i++) a[i] = i + 1;
    printf("3x3 spiral: ");
    spiral_print(a, 3, 3);

    // 4x4 matrix
    int b[16];
    for (i = 0; i < 16; i++) b[i] = i + 1;
    printf("4x4 spiral: ");
    spiral_print(b, 4, 4);

    // 3x4 matrix
    int c[12];
    for (i = 0; i < 12; i++) c[i] = i + 1;
    printf("3x4 spiral: ");
    spiral_print(c, 3, 4);

    // 1x5 (single row)
    int d[5];
    for (i = 0; i < 5; i++) d[i] = i + 1;
    printf("1x5 spiral: ");
    spiral_print(d, 1, 5);

    // 5x1 (single column)
    int e[5];
    for (i = 0; i < 5; i++) e[i] = (i + 1) * 10;
    printf("5x1 spiral: ");
    spiral_print(e, 5, 1);

    // 2x2
    int f[4];
    f[0] = 1; f[1] = 2; f[2] = 3; f[3] = 4;
    printf("2x2 spiral: ");
    spiral_print(f, 2, 2);

    // 1x1
    int g[1];
    g[0] = 42;
    printf("1x1 spiral: ");
    spiral_print(g, 1, 1);

    // 2x3
    int h[6];
    for (i = 0; i < 6; i++) h[i] = i + 1;
    printf("2x3 spiral: ");
    spiral_print(h, 2, 3);

    return 0;
}
