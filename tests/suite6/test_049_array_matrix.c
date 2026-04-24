int printf(const char *fmt, ...);

void spiral_traverse(int *mat, int rows, int cols) {
    int top = 0;
    int bottom = rows - 1;
    int left = 0;
    int right = cols - 1;
    int first = 1;
    int i;

    while (top <= bottom && left <= right) {
        for (i = left; i <= right; i++) {
            if (!first) printf(" ");
            printf("%d", mat[top * cols + i]);
            first = 0;
        }
        top++;

        for (i = top; i <= bottom; i++) {
            printf(" %d", mat[i * cols + right]);
        }
        right--;

        if (top <= bottom) {
            for (i = right; i >= left; i--) {
                printf(" %d", mat[bottom * cols + i]);
            }
            bottom--;
        }

        if (left <= right) {
            for (i = bottom; i >= top; i--) {
                printf(" %d", mat[i * cols + left]);
            }
            left++;
        }
    }
    printf("\n");
}

void rotate_90(int *mat, int n) {
    int i;
    int j;
    int temp;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            temp = mat[i * n + j];
            mat[i * n + j] = mat[j * n + i];
            mat[j * n + i] = temp;
        }
    }
    for (i = 0; i < n; i++) {
        int left = 0;
        int right = n - 1;
        while (left < right) {
            temp = mat[i * n + left];
            mat[i * n + left] = mat[i * n + right];
            mat[i * n + right] = temp;
            left++;
            right--;
        }
    }
}

void print_matrix(int *mat, int rows, int cols) {
    int i;
    int j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (j > 0) printf(" ");
            printf("%d", mat[i * cols + j]);
        }
        printf("\n");
    }
}

void zigzag_traverse(int *mat, int rows, int cols) {
    int r = 0;
    int c = 0;
    int going_up = 1;
    int first = 1;
    int count = 0;
    int total = rows * cols;

    while (count < total) {
        if (!first) printf(" ");
        printf("%d", mat[r * cols + c]);
        first = 0;
        count++;

        if (going_up) {
            if (c == cols - 1) {
                r++;
                going_up = 0;
            } else if (r == 0) {
                c++;
                going_up = 0;
            } else {
                r--;
                c++;
            }
        } else {
            if (r == rows - 1) {
                c++;
                going_up = 1;
            } else if (c == 0) {
                r++;
                going_up = 1;
            } else {
                r++;
                c--;
            }
        }
    }
    printf("\n");
}

int main(void) {
    int m3[9];
    m3[0] = 1; m3[1] = 2; m3[2] = 3;
    m3[3] = 4; m3[4] = 5; m3[5] = 6;
    m3[6] = 7; m3[7] = 8; m3[8] = 9;

    // EXPECT: 1 2 3 6 9 8 7 4 5
    spiral_traverse(m3, 3, 3);

    int m4[16];
    m4[0] = 1;  m4[1] = 2;  m4[2] = 3;  m4[3] = 4;
    m4[4] = 5;  m4[5] = 6;  m4[6] = 7;  m4[7] = 8;
    m4[8] = 9;  m4[9] = 10; m4[10] = 11; m4[11] = 12;
    m4[12] = 13; m4[13] = 14; m4[14] = 15; m4[15] = 16;

    // EXPECT: 1 2 3 4 8 12 16 15 14 13 9 5 6 7 11 10
    spiral_traverse(m4, 4, 4);

    int mr[6];
    mr[0] = 1; mr[1] = 2; mr[2] = 3;
    mr[3] = 4; mr[4] = 5; mr[5] = 6;
    // EXPECT: 1 2 3 6 5 4
    spiral_traverse(mr, 2, 3);

    int mc[6];
    mc[0] = 1; mc[1] = 2;
    mc[2] = 3; mc[3] = 4;
    mc[4] = 5; mc[5] = 6;
    // EXPECT: 1 2 4 6 5 3
    spiral_traverse(mc, 3, 2);

    m3[0] = 1; m3[1] = 2; m3[2] = 3;
    m3[3] = 4; m3[4] = 5; m3[5] = 6;
    m3[6] = 7; m3[7] = 8; m3[8] = 9;
    rotate_90(m3, 3);
    // EXPECT: 7 4 1
    // EXPECT: 8 5 2
    // EXPECT: 9 6 3
    print_matrix(m3, 3, 3);

    int m2[4];
    m2[0] = 1; m2[1] = 2;
    m2[2] = 3; m2[3] = 4;
    rotate_90(m2, 2);
    // EXPECT: 3 1
    // EXPECT: 4 2
    print_matrix(m2, 2, 2);

    m2[0] = 3; m2[1] = 1;
    m2[2] = 4; m2[3] = 2;
    rotate_90(m2, 2);
    // EXPECT: 4 3
    // EXPECT: 2 1
    print_matrix(m2, 2, 2);

    int z3[9];
    z3[0] = 1; z3[1] = 2; z3[2] = 3;
    z3[3] = 4; z3[4] = 5; z3[5] = 6;
    z3[6] = 7; z3[7] = 8; z3[8] = 9;
    // EXPECT: 1 2 4 7 5 3 6 8 9
    zigzag_traverse(z3, 3, 3);

    int z1[4];
    z1[0] = 1; z1[1] = 2; z1[2] = 3; z1[3] = 4;
    // EXPECT: 1 2 3 4
    zigzag_traverse(z1, 1, 4);

    int z2[4];
    z2[0] = 1; z2[1] = 2; z2[2] = 3; z2[3] = 4;
    // EXPECT: 1 2 3 4
    zigzag_traverse(z2, 4, 1);

    // EXPECT: done
    printf("done\n");

    return 0;
}
