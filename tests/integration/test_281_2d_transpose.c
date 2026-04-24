int printf(const char *fmt, ...);
// EXPECT: Original 3x4:\n1 2 3 4\n5 6 7 8\n9 10 11 12\nTransposed 4x3:\n1 5 9\n2 6 10\n3 7 11\n4 8 12\nDouble transposed 3x4:\n1 2 3 4\n5 6 7 8\n9 10 11 12\nSquare 4x4:\n1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16\nTransposed 4x4:\n1 5 9 13\n2 6 10 14\n3 7 11 15\n4 8 12 16\nA + A^T (symmetric):\n2 6 10\n6 10 14\n10 14 18\nsymmetric: 1
// Test: 2D array transpose

void print_mat(int *m, int rows, int cols) {
    int i; int j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (j > 0) printf(" ");
            printf("%d", *(m + i * cols + j));
        }
        printf("\n");
    }
}

void transpose(int *src, int *dst, int rows, int cols) {
    int i; int j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            *(dst + j * rows + i) = *(src + i * cols + j);
        }
    }
}

// In-place transpose for square matrix
void transpose_square(int *m, int n) {
    int i; int j;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            int tmp = *(m + i * n + j);
            *(m + i * n + j) = *(m + j * n + i);
            *(m + j * n + i) = tmp;
        }
    }
}

int main(void) {
    // 3x4 matrix
    int a[12];
    int i;
    for (i = 0; i < 12; i++) a[i] = i + 1;
    printf("Original 3x4:\n");
    print_mat(a, 3, 4);

    int at[12];
    transpose(a, at, 3, 4);
    printf("Transposed 4x3:\n");
    print_mat(at, 4, 3);

    // Double transpose = original
    int att[12];
    transpose(at, att, 4, 3);
    printf("Double transposed 3x4:\n");
    print_mat(att, 3, 4);

    // Square matrix in-place transpose
    int sq[16];
    for (i = 0; i < 16; i++) sq[i] = i + 1;
    printf("Square 4x4:\n");
    print_mat(sq, 4, 4);
    transpose_square(sq, 4);
    printf("Transposed 4x4:\n");
    print_mat(sq, 4, 4);

    // Verify symmetry: A + A^T
    int sym[9];
    int orig[9];
    for (i = 0; i < 9; i++) orig[i] = i + 1;
    int t2[9];
    transpose(orig, t2, 3, 3);
    for (i = 0; i < 9; i++) sym[i] = orig[i] + t2[i];
    printf("A + A^T (symmetric):\n");
    print_mat(sym, 3, 3);

    // Check symmetry
    int is_sym = 1;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (sym[i*3+j] != sym[j*3+i]) is_sym = 0;
        }
    }
    printf("symmetric: %d\n", is_sym);

    return 0;
}
