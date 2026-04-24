int printf(const char *fmt, ...);
// EXPECT: 1 \n1 1 \n1 2 1 \n1 3 3 1 \n1 4 6 4 1 \n1 5 10 10 5 1 \n1 6 15 20 15 6 1 \n1 7 21 35 35 21 7 1 \nrow sums: 1 2 4 8 16 32 64 128 \nalt sums: 1 0 0 0 0 0 0 0 \ndiag sums: 1 1 2 3 5 8 13 21 \nsymmetric: 1 1 1 1 1 1 1 1 \nhockey stick: 5 = 5
// Test: Pascal's triangle computation

int main(void) {
    int tri[10][10];
    int i, j;
    int sum;

    /* Build Pascal's triangle */
    for (i = 0; i < 10; i++) {
        tri[i][0] = 1;
        tri[i][i] = 1;
        for (j = 1; j < i; j++) {
            tri[i][j] = tri[i - 1][j - 1] + tri[i - 1][j];
        }
    }

    /* Print first 8 rows */
    for (i = 0; i < 8; i++) {
        for (j = 0; j <= i; j++) {
            printf("%d ", tri[i][j]);
        }
        printf("\n");
    }

    /* Row sums should be powers of 2 */
    printf("row sums: ");
    for (i = 0; i < 8; i++) {
        sum = 0;
        for (j = 0; j <= i; j++) {
            sum += tri[i][j];
        }
        printf("%d ", sum);
    }
    printf("\n");

    /* Alternating row sums should be 0 (except row 0) */
    printf("alt sums: ");
    for (i = 0; i < 8; i++) {
        sum = 0;
        for (j = 0; j <= i; j++) {
            if (j % 2 == 0) {
                sum += tri[i][j];
            } else {
                sum -= tri[i][j];
            }
        }
        printf("%d ", sum);
    }
    printf("\n");

    /* Fibonacci from diagonal sums */
    printf("diag sums: ");
    {
        int d;
        for (d = 0; d < 8; d++) {
            sum = 0;
            for (i = 0; i <= d; i++) {
                j = d - i;
                if (j <= i && j >= 0) {
                    sum += tri[i][j];
                }
            }
            printf("%d ", sum);
        }
    }
    printf("\n");

    /* Check symmetry */
    printf("symmetric: ");
    for (i = 0; i < 8; i++) {
        int sym;
        sym = 1;
        for (j = 0; j <= i; j++) {
            if (tri[i][j] != tri[i][i - j]) {
                sym = 0;
            }
        }
        printf("%d ", sym);
    }
    printf("\n");

    /* Hockey stick property: sum of C(r,r) + C(r+1,r) + ... + C(n,r) = C(n+1,r+1) */
    /* sum of tri[0][0] + tri[1][1] + tri[2][2] + tri[3][3] + tri[4][4] should equal tri[5][4]? */
    /* Actually: sum of col 0: tri[0][0]+tri[1][0]+tri[2][0]+tri[3][0]+tri[4][0] = tri[5][1] */
    sum = 0;
    for (i = 0; i < 5; i++) {
        sum += tri[i][0];
    }
    printf("hockey stick: %d = %d\n", sum, tri[5][1]);

    return 0;
}
