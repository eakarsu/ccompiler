int printf(const char *fmt, ...);
// EXPECT: Matrix:\n1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16\nmain diagonal sum = 34\nanti diagonal sum = 34\n3x3 diagonal sums:\ndiag[-2]: 3\ndiag[-1]: 8\ndiag[0]: 15\ndiag[1]: 12\ndiag[2]: 7\ndiagonal matrix:\n5 0 0\n0 3 0\n0 0 7\nis diagonal: 1\nb is diagonal: 0\nborder sum = 102
// Test: diagonal sums and operations on 2D arrays

void print_mat(int *m, int n) {
    int i; int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (j > 0) printf(" ");
            printf("%d", *(m + i * n + j));
        }
        printf("\n");
    }
}

int main_diagonal_sum(int *m, int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        sum = sum + *(m + i * n + i);
    }
    return sum;
}

int anti_diagonal_sum(int *m, int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        sum = sum + *(m + i * n + (n - 1 - i));
    }
    return sum;
}

// Sum of all diagonals (upper-left to lower-right)
void all_diagonal_sums(int *m, int n) {
    int d;
    // Diagonals indexed from -(n-1) to (n-1)
    for (d = -(n - 1); d <= n - 1; d++) {
        int sum = 0;
        int count = 0;
        int i; int j;
        for (i = 0; i < n; i++) {
            j = i - d;
            if (j >= 0 && j < n) {
                sum = sum + *(m + i * n + j);
                count = count + 1;
            }
        }
        if (count > 0) {
            printf("diag[%d]: %d\n", d, sum);
        }
    }
}

// Check if matrix is diagonal
int is_diagonal(int *m, int n) {
    int i; int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i != j && *(m + i * n + j) != 0) return 0;
        }
    }
    return 1;
}

int main(void) {
    // 4x4 matrix
    int a[16];
    int i;
    for (i = 0; i < 16; i++) a[i] = i + 1;
    printf("Matrix:\n");
    print_mat(a, 4);

    printf("main diagonal sum = %d\n", main_diagonal_sum(a, 4));
    printf("anti diagonal sum = %d\n", anti_diagonal_sum(a, 4));

    // All diagonal sums for 3x3
    int b[9];
    for (i = 0; i < 9; i++) b[i] = i + 1;
    printf("3x3 diagonal sums:\n");
    all_diagonal_sums(b, 3);

    // Diagonal matrix check
    int diag[9];
    for (i = 0; i < 9; i++) diag[i] = 0;
    diag[0] = 5; diag[4] = 3; diag[8] = 7;
    printf("diagonal matrix:\n");
    print_mat(diag, 3);
    printf("is diagonal: %d\n", is_diagonal(diag, 3));
    printf("b is diagonal: %d\n", is_diagonal(b, 3));

    // Border sum (top row + bottom row + left col + right col - corners)
    int border = 0;
    int n = 4;
    int j;
    for (j = 0; j < n; j++) border = border + a[j];          // top
    for (j = 0; j < n; j++) border = border + a[(n-1)*n+j];  // bottom
    for (i = 1; i < n-1; i++) border = border + a[i*n];       // left
    for (i = 1; i < n-1; i++) border = border + a[i*n+n-1];  // right
    printf("border sum = %d\n", border);

    return 0;
}
