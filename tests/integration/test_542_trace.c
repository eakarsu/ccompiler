int printf(const char *fmt, ...);
// EXPECT: Matrix A:\n|    1    2    3 |\n|    4    5    6 |\n|    7    8    9 |\ntr(A) = 15\n\nMatrix B:\n|    2    0    1 |\n|    1    3    0 |\n|    0    1    4 |\ntr(B) = 9\n\nA + B:\n|    3    2    4 |\n|    5    8    6 |\n|    7    9   13 |\ntr(A+B) = 24, tr(A)+tr(B) = 24\n\nAB:\n|    4    9   13 |\n|   13   21   28 |\n|   22   33   43 |\ntr(AB) = 68\n\nBA:\n|    9   12   15 |\n|   13   17   21 |\n|   32   37   42 |\ntr(BA) = 68\n\ntr(AB) == tr(BA)? 1\n\nA^T:\n|    1    4    7 |\n|    2    5    8 |\n|    3    6    9 |\ntr(A^T) = 15, tr(A) = 15\n\ntr(3A) = 45, 3*tr(A) = 45\n\nA^2:\n|   30   36   42 |\n|   66   81   96 |\n|  102  126  150 |\ntr(A^2) = 261
// Test: Matrix trace and properties

int trace3(int m[3][3]) {
    return m[0][0] + m[1][1] + m[2][2];
}

void mat_add(int a[3][3], int b[3][3], int c[3][3]) {
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            c[i][j] = a[i][j] + b[i][j];
}

void mat_mult(int a[3][3], int b[3][3], int c[3][3]) {
    int i, j, k;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++) {
            c[i][j] = 0;
            for (k = 0; k < 3; k++)
                c[i][j] = c[i][j] + a[i][k] * b[k][j];
        }
}

void mat_transpose(int m[3][3], int t[3][3]) {
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            t[i][j] = m[j][i];
}

void print_mat(int m[3][3]) {
    int i, j;
    for (i = 0; i < 3; i++) {
        printf("| ");
        for (j = 0; j < 3; j++) {
            printf("%4d ", m[i][j]);
        }
        printf("|\n");
    }
}

int main(void) {
    int i, j;

    int A[3][3], B[3][3];
    A[0][0] = 1; A[0][1] = 2; A[0][2] = 3;
    A[1][0] = 4; A[1][1] = 5; A[1][2] = 6;
    A[2][0] = 7; A[2][1] = 8; A[2][2] = 9;

    B[0][0] = 2; B[0][1] = 0; B[0][2] = 1;
    B[1][0] = 1; B[1][1] = 3; B[1][2] = 0;
    B[2][0] = 0; B[2][1] = 1; B[2][2] = 4;

    printf("Matrix A:\n");
    print_mat(A);
    printf("tr(A) = %d\n\n", trace3(A));

    printf("Matrix B:\n");
    print_mat(B);
    printf("tr(B) = %d\n\n", trace3(B));

    // tr(A+B) = tr(A) + tr(B)
    int C[3][3];
    mat_add(A, B, C);
    printf("A + B:\n");
    print_mat(C);
    printf("tr(A+B) = %d, tr(A)+tr(B) = %d\n\n", trace3(C), trace3(A) + trace3(B));

    // tr(AB) = tr(BA)
    int AB[3][3], BA[3][3];
    mat_mult(A, B, AB);
    mat_mult(B, A, BA);
    printf("AB:\n");
    print_mat(AB);
    printf("tr(AB) = %d\n\n", trace3(AB));

    printf("BA:\n");
    print_mat(BA);
    printf("tr(BA) = %d\n\n", trace3(BA));

    printf("tr(AB) == tr(BA)? %d\n", trace3(AB) == trace3(BA));

    // tr(A^T) = tr(A)
    int AT[3][3];
    mat_transpose(A, AT);
    printf("\nA^T:\n");
    print_mat(AT);
    printf("tr(A^T) = %d, tr(A) = %d\n", trace3(AT), trace3(A));

    // tr(kA) = k*tr(A)
    int kA[3][3];
    int k = 3;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            kA[i][j] = k * A[i][j];
    printf("\ntr(3A) = %d, 3*tr(A) = %d\n", trace3(kA), k * trace3(A));

    // A^2
    int A2[3][3];
    mat_mult(A, A, A2);
    printf("\nA^2:\n");
    print_mat(A2);
    printf("tr(A^2) = %d\n", trace3(A2));

    return 0;
}
