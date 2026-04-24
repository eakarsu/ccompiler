int printf(const char *fmt, ...);
// EXPECT: Stirling numbers of the second kind S(n,k):\nn=0: 1 \nn=1: 0 1 \nn=2: 0 1 1 \nn=3: 0 1 3 1 \nn=4: 0 1 7 6 1 \nn=5: 0 1 15 25 10 1 \nn=6: 0 1 31 90 65 15 1 \nn=7: 0 1 63 301 350 140 21 1 \nn=8: 0 1 127 966 1701 1050 266 28 1 \n\nRow sums (Bell numbers):\nB(0) = 1\nB(1) = 1\nB(2) = 2\nB(3) = 5\nB(4) = 15\nB(5) = 52\nB(6) = 203\nB(7) = 877\nB(8) = 4140\n\nStirling numbers of the first kind |s(n,k)|:\nn=0: 1 \nn=1: 0 1 \nn=2: 0 1 1 \nn=3: 0 2 3 1 \nn=4: 0 6 11 6 1 \nn=5: 0 24 50 35 10 1 \nn=6: 0 120 274 225 85 15 1 \nn=7: 0 720 1764 1624 735 175 21 1 \nn=8: 0 5040 13068 13132 6769 1960 322 28 1 \n\nRow sums of first kind (should be n!):\nsum |s(0,k)| = 1, 0! = 1\nsum |s(1,k)| = 1, 1! = 1\nsum |s(2,k)| = 2, 2! = 2\nsum |s(3,k)| = 6, 3! = 6\nsum |s(4,k)| = 24, 4! = 24\nsum |s(5,k)| = 120, 5! = 120\nsum |s(6,k)| = 720, 6! = 720\nsum |s(7,k)| = 5040, 7! = 5040\nsum |s(8,k)| = 40320, 8! = 40320
// Test: Stirling numbers (second kind)

int main(void) {
    // S(n,k) = Stirling numbers of the second kind
    // S(n,k) = k*S(n-1,k) + S(n-1,k-1)
    int S[10][10];
    int i, j;

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            S[i][j] = 0;
        }
    }
    S[0][0] = 1;

    for (i = 1; i < 10; i++) {
        for (j = 1; j <= i; j++) {
            S[i][j] = j * S[i - 1][j] + S[i - 1][j - 1];
        }
    }

    printf("Stirling numbers of the second kind S(n,k):\n");
    for (i = 0; i < 9; i++) {
        printf("n=%d: ", i);
        for (j = 0; j <= i; j++) {
            printf("%d ", S[i][j]);
        }
        printf("\n");
    }

    // Row sums = Bell numbers
    printf("\nRow sums (Bell numbers):\n");
    for (i = 0; i < 9; i++) {
        int sum = 0;
        for (j = 0; j <= i; j++) {
            sum = sum + S[i][j];
        }
        printf("B(%d) = %d\n", i, sum);
    }

    // Stirling numbers of the first kind (unsigned)
    // |s(n,k)| = |s(n-1,k-1)| + (n-1)*|s(n-1,k)|
    int s[10][10];
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            s[i][j] = 0;
        }
    }
    s[0][0] = 1;

    for (i = 1; i < 10; i++) {
        for (j = 1; j <= i; j++) {
            s[i][j] = s[i - 1][j - 1] + (i - 1) * s[i - 1][j];
        }
    }

    printf("\nStirling numbers of the first kind |s(n,k)|:\n");
    for (i = 0; i < 9; i++) {
        printf("n=%d: ", i);
        for (j = 0; j <= i; j++) {
            printf("%d ", s[i][j]);
        }
        printf("\n");
    }

    // Row sums of first kind = n!
    printf("\nRow sums of first kind (should be n!):\n");
    for (i = 0; i < 9; i++) {
        int sum = 0;
        for (j = 0; j <= i; j++) {
            sum = sum + s[i][j];
        }
        int fact = 1;
        for (j = 1; j <= i; j++) fact = fact * j;
        printf("sum |s(%d,k)| = %d, %d! = %d\n", i, sum, i, fact);
    }

    return 0;
}
