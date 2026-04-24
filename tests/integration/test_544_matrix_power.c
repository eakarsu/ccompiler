int printf(const char *fmt, ...);
// EXPECT: Fibonacci via matrix power:\nF(1) = 1\nF(2) = 1\nF(3) = 2\nF(4) = 3\nF(5) = 5\nF(6) = 8\nF(7) = 13\nF(8) = 21\nF(9) = 34\nF(10) = 55\nF(11) = 89\nF(12) = 144\nF(13) = 233\nF(14) = 377\nF(15) = 610\n\nUpper triangular matrix power:\nA^1:\n|      1      1 |\n|      0      1 |\nA^2:\n|      1      2 |\n|      0      1 |\nA^3:\n|      1      3 |\n|      0      1 |\nA^4:\n|      1      4 |\n|      0      1 |\nA^5:\n|      1      5 |\n|      0      1 |\nA^6:\n|      1      6 |\n|      0      1 |\nA^7:\n|      1      7 |\n|      0      1 |\nA^8:\n|      1      8 |\n|      0      1 |\n\nFast matrix power: [[2,1],[1,1]]^n\nB^1:\n|      2      1 |\n|      1      1 |\ndet = 1\n\nB^2:\n|      5      3 |\n|      3      2 |\ndet = 1\n\nB^4:\n|     34     21 |\n|     21     13 |\ndet = 1\n\nB^8:\n|   1597    987 |\n|    987    610 |\ndet = 1\n\nB^3:\n|     13      8 |\n|      8      5 |\ndet = 1\n\nB^5:\n|     89     55 |\n|     55     34 |\ndet = 1
// Test: Matrix power computation

void mat_mult2(int a[2][2], int b[2][2], int c[2][2]) {
    int i, j, k;
    int temp[2][2];
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++) {
            temp[i][j] = 0;
            for (k = 0; k < 2; k++)
                temp[i][j] = temp[i][j] + a[i][k] * b[k][j];
        }
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++)
            c[i][j] = temp[i][j];
}

void mat_copy2(int src[2][2], int dst[2][2]) {
    int i, j;
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++)
            dst[i][j] = src[i][j];
}

void print_mat2(int m[2][2]) {
    printf("| %6d %6d |\n", m[0][0], m[0][1]);
    printf("| %6d %6d |\n", m[1][0], m[1][1]);
}

int main(void) {
    int i;

    // Fibonacci via matrix exponentiation
    // [[1,1],[1,0]]^n = [[F(n+1), F(n)], [F(n), F(n-1)]]
    int F[2][2];
    F[0][0] = 1; F[0][1] = 1;
    F[1][0] = 1; F[1][1] = 0;

    printf("Fibonacci via matrix power:\n");
    int result[2][2];
    result[0][0] = 1; result[0][1] = 0;
    result[1][0] = 0; result[1][1] = 1;

    for (i = 1; i <= 15; i++) {
        mat_mult2(result, F, result);
        printf("F(%d) = %d\n", i, result[0][1]);
    }

    // Matrix power: A^n using repeated squaring
    int A[2][2];
    A[0][0] = 1; A[0][1] = 1;
    A[1][0] = 0; A[1][1] = 1;

    // Upper triangular: A^n = [[1, n], [0, 1]]
    printf("\nUpper triangular matrix power:\n");
    int R[2][2];
    R[0][0] = 1; R[0][1] = 0;
    R[1][0] = 0; R[1][1] = 1;
    for (i = 1; i <= 8; i++) {
        mat_mult2(R, A, R);
        printf("A^%d:\n", i);
        print_mat2(R);
    }

    // Fast matrix power (binary exponentiation)
    printf("\nFast matrix power: [[2,1],[1,1]]^n\n");
    int B[2][2];
    B[0][0] = 2; B[0][1] = 1;
    B[1][0] = 1; B[1][1] = 1;

    int powers[6];
    powers[0] = 1; powers[1] = 2; powers[2] = 4;
    powers[3] = 8; powers[4] = 3; powers[5] = 5;

    for (i = 0; i < 6; i++) {
        int n = powers[i];
        // Compute B^n
        int res[2][2], base[2][2];
        res[0][0] = 1; res[0][1] = 0;
        res[1][0] = 0; res[1][1] = 1;
        mat_copy2(B, base);
        int exp = n;
        while (exp > 0) {
            if (exp % 2 == 1) {
                mat_mult2(res, base, res);
            }
            mat_mult2(base, base, base);
            exp = exp / 2;
        }
        printf("B^%d:\n", n);
        print_mat2(res);
        printf("det = %d\n\n", res[0][0] * res[1][1] - res[0][1] * res[1][0]);
    }

    return 0;
}
