int printf(const char *fmt, ...);
// EXPECT: Pascal's triangle (rows 0-10):\n1 \n1 1 \n1 2 1 \n1 3 3 1 \n1 4 6 4 1 \n1 5 10 10 5 1 \n1 6 15 20 15 6 1 \n1 7 21 35 35 21 7 1 \n1 8 28 56 70 56 28 8 1 \n1 9 36 84 126 126 84 36 9 1 \n1 10 45 120 210 252 210 120 45 10 1 \nRow sums:\nRow 0: 1\nRow 1: 2\nRow 2: 4\nRow 3: 8\nRow 4: 16\nRow 5: 32\nRow 6: 64\nRow 7: 128\nRow 8: 256\nRow 9: 512\nRow 10: 1024\nRow 11: 2048\nRow 12: 4096\nAlternating row sums:\nRow 0: 1\nRow 1: 0\nRow 2: 0\nRow 3: 0\nRow 4: 0\nRow 5: 0\nRow 6: 0\nRow 7: 0\nRow 8: 0\nRow 9: 0\nRow 10: 0\nDiagonal sums (Fibonacci):\nDiag 0: 1\nDiag 1: 1\nDiag 2: 2\nDiag 3: 3\nDiag 4: 5\nDiag 5: 8\nDiag 6: 13\nDiag 7: 21\nDiag 8: 34\nDiag 9: 55\nHockey stick: C(2,2)+C(3,2)+C(4,2)+C(5,2)+C(6,2) = 35\nC(7,3) = 35
// Test: Pascal's triangle

int main(void) {
    int pascal[15][15];
    int i, j;

    // Build Pascal's triangle
    for (i = 0; i < 15; i++) {
        pascal[i][0] = 1;
        pascal[i][i] = 1;
        for (j = 1; j < i; j++) {
            pascal[i][j] = pascal[i - 1][j - 1] + pascal[i - 1][j];
        }
    }

    // Print triangle
    printf("Pascal's triangle (rows 0-10):\n");
    for (i = 0; i <= 10; i++) {
        for (j = 0; j <= i; j++) {
            printf("%d ", pascal[i][j]);
        }
        printf("\n");
    }

    // Row sums (should be powers of 2)
    printf("Row sums:\n");
    for (i = 0; i <= 12; i++) {
        int sum = 0;
        for (j = 0; j <= i; j++) {
            sum = sum + pascal[i][j];
        }
        printf("Row %d: %d\n", i, sum);
    }

    // Alternating row sums (should be 0 for n>0)
    printf("Alternating row sums:\n");
    for (i = 0; i <= 10; i++) {
        int asum = 0;
        for (j = 0; j <= i; j++) {
            if (j % 2 == 0)
                asum = asum + pascal[i][j];
            else
                asum = asum - pascal[i][j];
        }
        printf("Row %d: %d\n", i, asum);
    }

    // Diagonal sums = Fibonacci numbers
    printf("Diagonal sums (Fibonacci):\n");
    for (i = 0; i < 10; i++) {
        int dsum = 0;
        int r = i;
        int c = 0;
        while (r >= 0 && c <= r) {
            dsum = dsum + pascal[r][c];
            r = r - 1;
            c = c + 1;
            if (r < c) break;
        }
        // Actually use the shallow diagonal
        dsum = 0;
        for (j = 0; j <= i / 2; j++) {
            dsum = dsum + pascal[i - j][j];
        }
        printf("Diag %d: %d\n", i, dsum);
    }

    // Hockey stick identity: C(r,r) + C(r+1,r) + ... + C(n,r) = C(n+1,r+1)
    printf("Hockey stick: C(2,2)+C(3,2)+C(4,2)+C(5,2)+C(6,2) = %d\n",
           pascal[2][2] + pascal[3][2] + pascal[4][2] + pascal[5][2] + pascal[6][2]);
    printf("C(7,3) = %d\n", pascal[7][3]);

    return 0;
}
