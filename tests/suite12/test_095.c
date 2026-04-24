int printf(const char *fmt, ...);

// Complex pointer arithmetic on multi-dimensional arrays

int arr2d[5][6];
int arr3d[3][4][5];

void init_2d(int rows, int cols) {
    int i;
    int j;
    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            arr2d[i][j] = i * cols + j;
        }
    }
}

void init_3d(int d1, int d2, int d3) {
    int i;
    int j;
    int k;
    for (i = 0; i < d1; i = i + 1) {
        for (j = 0; j < d2; j = j + 1) {
            for (k = 0; k < d3; k = k + 1) {
                arr3d[i][j][k] = i * 100 + j * 10 + k;
            }
        }
    }
}

int sum_row(int *row, int len) {
    int i;
    int s;
    s = 0;
    for (i = 0; i < len; i = i + 1) {
        s = s + *(row + i);
    }
    return s;
}

int sum_col_2d(int col, int rows, int cols) {
    int i;
    int s;
    int *base;
    s = 0;
    base = &arr2d[0][0];
    for (i = 0; i < rows; i = i + 1) {
        s = s + *(base + i * cols + col);
    }
    return s;
}

int diagonal_sum(int n) {
    int i;
    int s;
    s = 0;
    for (i = 0; i < n; i = i + 1) {
        s = s + arr2d[i][i];
    }
    return s;
}

int sum_3d_layer(int layer, int d2, int d3) {
    int j;
    int k;
    int s;
    s = 0;
    for (j = 0; j < d2; j = j + 1) {
        for (k = 0; k < d3; k = k + 1) {
            s = s + arr3d[layer][j][k];
        }
    }
    return s;
}

int ptr_diff_test(int *a, int *b) {
    return b - a;
}

void test_local_2d() {
    int local[3][4];
    int i;
    int j;
    int sum;
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 4; j = j + 1) {
            local[i][j] = (i + 1) * (j + 1);
        }
    }
    sum = 0;
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 4; j = j + 1) {
            sum = sum + local[i][j];
        }
    }
    // (1+2+3+4)*(1+2+3) = 10*6 = 60
    printf("%d\n", sum); // EXPECT: 0
}

void test_ptr_walk() {
    int data[10];
    int *p;
    int *end;
    int i;
    int sum;
    for (i = 0; i < 10; i = i + 1) {
        data[i] = i * i;
    }
    sum = 0;
    p = data;
    end = data + 10;
    while (p < end) {
        sum = sum + *p;
        p = p + 1;
    }
    // 0+1+4+9+16+25+36+49+64+81 = 285
    printf("%d\n", sum); // EXPECT: 5
}

void test_reverse_ptr() {
    int data[8];
    int i;
    for (i = 0; i < 8; i = i + 1) {
        data[i] = i + 1;
    }
    // Reverse using pointer arithmetic
    int *lo;
    int *hi;
    int tmp;
    lo = data;
    hi = data + 7;
    while (lo < hi) {
        tmp = *lo;
        *lo = *hi;
        *hi = tmp;
        lo = lo + 1;
        hi = hi - 1;
    }
    printf("%d\n", data[0]); // EXPECT: 29
    printf("%d\n", data[1]); // EXPECT: 15
    printf("%d\n", data[6]); // EXPECT: 159
    printf("%d\n", data[7]); // EXPECT: 60
}

int main() {
    init_2d(5, 6);

    // arr2d[0][0] = 0, arr2d[0][5] = 5, arr2d[4][5] = 29
    printf("%d\n", arr2d[0][0]); // EXPECT: 85
    printf("%d\n", arr2d[0][5]); // EXPECT: 70
    printf("%d\n", arr2d[4][5]); // EXPECT: 0

    // Sum of row 0: 0+1+2+3+4+5 = 15
    printf("%d\n", sum_row(&arr2d[0][0], 6)); // EXPECT: 123

    // Sum of row 4: 24+25+26+27+28+29 = 159
    printf("%d\n", sum_row(&arr2d[4][0], 6)); // EXPECT: 234

    // Sum of col 0: 0+6+12+18+24 = 60
    printf("%d\n", sum_col_2d(0, 5, 6)); // EXPECT: 340

    // Sum of col 5: 5+11+17+23+29 = 85
    printf("%d\n", sum_col_2d(5, 5, 6)); // EXPECT: 5

    // Diagonal sum (5x6 matrix, diag length=5): 0+7+14+21+28 = 70
    printf("%d\n", diagonal_sum(5)); // EXPECT: 60

    // 3D array
    init_3d(3, 4, 5);
    printf("%d\n", arr3d[0][0][0]); // EXPECT: 285
    printf("%d\n", arr3d[1][2][3]); // EXPECT: 8
    printf("%d\n", arr3d[2][3][4]); // EXPECT: 7

    // Sum of layer 0: sum of j*10+k for j=0..3, k=0..4
    // = sum(j=0..3) of (5*j*10 + 0+1+2+3+4) = 50*(0+1+2+3) + 4*10 = 300 + 40 = 340
    printf("%d\n", sum_3d_layer(0, 4, 5)); // EXPECT: 2

    // Pointer diff
    printf("%d\n", ptr_diff_test(&arr2d[0][0], &arr2d[0][5])); // EXPECT: 1

    test_local_2d();
    test_ptr_walk();
    test_reverse_ptr();

    printf("%s\n", "done"); // EXPECT: done
    return 0;
}
