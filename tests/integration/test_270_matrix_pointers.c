int printf(const char *fmt, ...);
// EXPECT: A:\n1 2 3\n4 5 6\n7 8 9\ntrace(A) = 15\n2*A:\n2 4 6\n8 10 12\n14 16 18\nA + 2A:\n3 6 9\n12 15 18\n21 24 27\nA^T:\n1 4 7\n2 5 8\n3 6 9\nD (2x3):\n10 20 30\n40 50 60\nD^T (3x2):\n10 40\n20 50\n30 60
// Test: matrix operations with pointers

void mat_print(int *m, int rows, int cols) {
    int i; int j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (j > 0) printf(" ");
            printf("%d", *(m + i * cols + j));
        }
        printf("\n");
    }
}

void mat_add(int *a, int *b, int *c, int rows, int cols) {
    int i;
    for (i = 0; i < rows * cols; i++) {
        c[i] = a[i] + b[i];
    }
}

void mat_scale(int *m, int rows, int cols, int s) {
    int i;
    for (i = 0; i < rows * cols; i++) {
        m[i] = m[i] * s;
    }
}

int mat_trace(int *m, int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        sum = sum + *(m + i * n + i);
    }
    return sum;
}

void mat_transpose(int *src, int *dst, int rows, int cols) {
    int i; int j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            *(dst + j * rows + i) = *(src + i * cols + j);
        }
    }
}

int main(void) {
    // 3x3 matrix
    int a[9];
    int i;
    for (i = 0; i < 9; i++) a[i] = i + 1;
    printf("A:\n");
    mat_print(a, 3, 3);

    // Trace
    printf("trace(A) = %d\n", mat_trace(a, 3));

    // Scale
    int b[9];
    for (i = 0; i < 9; i++) b[i] = a[i];
    mat_scale(b, 3, 3, 2);
    printf("2*A:\n");
    mat_print(b, 3, 3);

    // Add
    int c[9];
    mat_add(a, b, c, 3, 3);
    printf("A + 2A:\n");
    mat_print(c, 3, 3);

    // Transpose
    int t[9];
    mat_transpose(a, t, 3, 3);
    printf("A^T:\n");
    mat_print(t, 3, 3);

    // Non-square transpose (2x3 -> 3x2)
    int d[6];
    for (i = 0; i < 6; i++) d[i] = (i + 1) * 10;
    printf("D (2x3):\n");
    mat_print(d, 2, 3);
    int dt[6];
    mat_transpose(d, dt, 2, 3);
    printf("D^T (3x2):\n");
    mat_print(dt, 3, 2);

    return 0;
}
