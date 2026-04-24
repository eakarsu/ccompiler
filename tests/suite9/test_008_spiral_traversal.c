int printf(const char *fmt, ...);

void spiral_order(int *m, int rows, int cols, int *out, int *out_len) {
    int top;
    int bottom;
    int left;
    int right;
    int idx;

    top = 0;
    bottom = rows - 1;
    left = 0;
    right = cols - 1;
    idx = 0;

    while (top <= bottom && left <= right) {
        int i;
        /* Traverse right */
        for (i = left; i <= right; i = i + 1) {
            out[idx] = *(m + top * cols + i);
            idx = idx + 1;
        }
        top = top + 1;

        /* Traverse down */
        for (i = top; i <= bottom; i = i + 1) {
            out[idx] = *(m + i * cols + right);
            idx = idx + 1;
        }
        right = right - 1;

        /* Traverse left */
        if (top <= bottom) {
            for (i = right; i >= left; i = i - 1) {
                out[idx] = *(m + bottom * cols + i);
                idx = idx + 1;
            }
            bottom = bottom - 1;
        }

        /* Traverse up */
        if (left <= right) {
            for (i = bottom; i >= top; i = i - 1) {
                out[idx] = *(m + i * cols + left);
                idx = idx + 1;
            }
            left = left + 1;
        }
    }
    *out_len = idx;
}

void print_array(int *arr, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

void print_matrix(int *m, int rows, int cols) {
    int i;
    int j;
    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            if (j > 0) printf(" ");
            printf("%d", *(m + i * cols + j));
        }
        printf("\n");
    }
}

int main() {
    /* 3x3 spiral */
    int a[3][3];
    a[0][0] = 1; a[0][1] = 2; a[0][2] = 3;
    a[1][0] = 4; a[1][1] = 5; a[1][2] = 6;
    a[2][0] = 7; a[2][1] = 8; a[2][2] = 9;

    int out[16];
    int len;

    // EXPECT: 3x3 matrix:
    printf("3x3 matrix:\n");
    // EXPECT: 1 2 3
    // EXPECT: 4 5 6
    // EXPECT: 7 8 9
    print_matrix((int *)a, 3, 3);

    spiral_order((int *)a, 3, 3, out, &len);

    // EXPECT: spiral 3x3:
    printf("spiral 3x3:\n");
    // EXPECT: 1 2 3 6 9 8 7 4 5
    print_array(out, len);

    // EXPECT: len: 9
    printf("len: %d\n", len);

    /* 4x4 spiral */
    int b[4][4];
    int v;
    int i;
    int j;
    v = 1;
    for (i = 0; i < 4; i = i + 1)
        for (j = 0; j < 4; j = j + 1) {
            b[i][j] = v;
            v = v + 1;
        }

    // EXPECT: 4x4 matrix:
    printf("4x4 matrix:\n");
    // EXPECT: 1 2 3 4
    // EXPECT: 5 6 7 8
    // EXPECT: 9 10 11 12
    // EXPECT: 13 14 15 16
    print_matrix((int *)b, 4, 4);

    spiral_order((int *)b, 4, 4, out, &len);

    // EXPECT: spiral 4x4:
    printf("spiral 4x4:\n");
    // EXPECT: 1 2 3 4 8 12 16 15 14 13 9 5 6 7 11 10
    print_array(out, len);

    // EXPECT: len: 16
    printf("len: %d\n", len);

    /* 3x4 rectangular */
    int c[3][4];
    v = 1;
    for (i = 0; i < 3; i = i + 1)
        for (j = 0; j < 4; j = j + 1) {
            c[i][j] = v;
            v = v + 1;
        }

    spiral_order((int *)c, 3, 4, out, &len);

    // EXPECT: spiral 3x4:
    printf("spiral 3x4:\n");
    // EXPECT: 1 2 3 4 8 12 11 10 9 5 6 7
    print_array(out, len);

    // EXPECT: len: 12
    printf("len: %d\n", len);

    /* 1x4 single row */
    int d[1][4];
    d[0][0] = 10; d[0][1] = 20; d[0][2] = 30; d[0][3] = 40;

    spiral_order((int *)d, 1, 4, out, &len);

    // EXPECT: spiral 1x4:
    printf("spiral 1x4:\n");
    // EXPECT: 10 20 30 40
    print_array(out, len);

    /* 2x2 */
    int e[2][2];
    e[0][0] = 1; e[0][1] = 2;
    e[1][0] = 3; e[1][1] = 4;

    spiral_order((int *)e, 2, 2, out, &len);

    // EXPECT: spiral 2x2:
    printf("spiral 2x2:\n");
    // EXPECT: 1 2 4 3
    print_array(out, len);

    // EXPECT: Done
    printf("Done\n");

    return 0;
}
