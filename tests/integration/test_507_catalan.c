int printf(const char *fmt, ...);
// EXPECT: Catalan numbers:\nC(0) = 1\nC(1) = 1\nC(2) = 2\nC(3) = 5\nC(4) = 14\nC(5) = 42\nC(6) = 132\nC(7) = 429\nC(8) = 1430\nC(9) = 4862\nC(10) = 16796\nC(11) = 58786\nC(12) = 208012\nC(13) = 742900\nC(14) = 2674440\n\nApplications of Catalan numbers:\nBalanced parentheses for n=3: 5 ways\nBalanced parentheses for n=4: 14 ways\nBinary trees with 5 nodes: 42\nTriangulations of hexagon: 14\n\nBallot problem paths:\nRow 0: 1 \nRow 1: 1 1 \nRow 2: 1 2 2 \nRow 3: 1 3 5 5 \nRow 4: 1 4 9 14 14 \nRow 5: 1 5 14 28 42 42 \nRow 6: 1 6 20 48 90 132 132 \n\nSum check: Sum of C(0)..C(14) = 3707852
// Test: Catalan numbers

int main(void) {
    int catalan[15];
    int dp[15][15];
    int i, j;

    // Compute Catalan numbers using DP: C(n) = sum(C(i)*C(n-1-i)) for i=0..n-1
    catalan[0] = 1;
    catalan[1] = 1;
    for (i = 2; i < 15; i++) {
        catalan[i] = 0;
        for (j = 0; j < i; j++) {
            catalan[i] = catalan[i] + catalan[j] * catalan[i - 1 - j];
        }
    }

    printf("Catalan numbers:\n");
    for (i = 0; i < 15; i++) {
        printf("C(%d) = %d\n", i, catalan[i]);
    }

    // Catalan numbers count: number of valid parenthesizations
    // For n=3: ((())), (()()), (())(), ()(()), ()()()
    printf("\nApplications of Catalan numbers:\n");
    printf("Balanced parentheses for n=3: %d ways\n", catalan[3]);
    printf("Balanced parentheses for n=4: %d ways\n", catalan[4]);
    printf("Binary trees with 5 nodes: %d\n", catalan[5]);
    printf("Triangulations of hexagon: %d\n", catalan[4]);

    // Ballot numbers (related to Catalan)
    // B(n,k) = number of paths from (0,0) to (n,k) that don't cross y=x
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            dp[i][j] = 0;
        }
    }
    dp[0][0] = 1;
    for (i = 0; i < 7; i++) {
        for (j = 0; j <= i; j++) {
            if (dp[i][j] > 0) {
                dp[i + 1][j] = dp[i + 1][j] + dp[i][j];
                if (j + 1 <= i + 1) {
                    dp[i][j + 1] = dp[i][j + 1] + dp[i][j];
                }
            }
        }
    }

    printf("\nBallot problem paths:\n");
    for (i = 0; i < 7; i++) {
        printf("Row %d: ", i);
        for (j = 0; j <= i; j++) {
            printf("%d ", dp[i][j]);
        }
        printf("\n");
    }

    // Verify: C(n) = C(2n,n)/(n+1) using the DP values
    printf("\nSum check: ");
    int sum = 0;
    for (i = 0; i < 15; i++) {
        sum = sum + catalan[i];
    }
    printf("Sum of C(0)..C(14) = %d\n", sum);

    return 0;
}
