int printf(const char *fmt, ...);
// EXPECT: Triangular numbers:\nT(1) = 1\nT(2) = 3\nT(3) = 6\nT(4) = 10\nT(5) = 15\nT(6) = 21\nT(7) = 28\nT(8) = 36\nT(9) = 45\nT(10) = 55\nT(11) = 66\nT(12) = 78\nT(13) = 91\nT(14) = 105\nT(15) = 120\nT(16) = 136\nT(17) = 153\nT(18) = 171\nT(19) = 190\nT(20) = 210\n\nVerification T(n) = sum 1..n:\nT(1) = 1, sum = 1, match = 1\nT(2) = 3, sum = 3, match = 1\nT(3) = 6, sum = 6, match = 1\nT(4) = 10, sum = 10, match = 1\nT(5) = 15, sum = 15, match = 1\nT(6) = 21, sum = 21, match = 1\nT(7) = 28, sum = 28, match = 1\nT(8) = 36, sum = 36, match = 1\nT(9) = 45, sum = 45, match = 1\nT(10) = 55, sum = 55, match = 1\n\nT(n) + T(n-1) = n^2:\nT(1)+T(0) = 1+0 = 1, 1^2 = 1\nT(2)+T(1) = 3+1 = 4, 2^2 = 4\nT(3)+T(2) = 6+3 = 9, 3^2 = 9\nT(4)+T(3) = 10+6 = 16, 4^2 = 16\nT(5)+T(4) = 15+10 = 25, 5^2 = 25\nT(6)+T(5) = 21+15 = 36, 6^2 = 36\nT(7)+T(6) = 28+21 = 49, 7^2 = 49\nT(8)+T(7) = 36+28 = 64, 8^2 = 64\nT(9)+T(8) = 45+36 = 81, 9^2 = 81\nT(10)+T(9) = 55+45 = 100, 10^2 = 100\n\nTriangular numbers up to 100:\n1 3 6 10 15 21 28 36 45 55 66 78 91 \nCount: 13\n\nSum of first n triangular numbers:\nSum T(1)..T(1) = 1 (formula: 1)\nSum T(1)..T(2) = 4 (formula: 4)\nSum T(1)..T(3) = 10 (formula: 10)\nSum T(1)..T(4) = 20 (formula: 20)\nSum T(1)..T(5) = 35 (formula: 35)\nSum T(1)..T(6) = 56 (formula: 56)\nSum T(1)..T(7) = 84 (formula: 84)\nSum T(1)..T(8) = 120 (formula: 120)\nSum T(1)..T(9) = 165 (formula: 165)\nSum T(1)..T(10) = 220 (formula: 220)\n\nSquare triangular numbers up to 10000:\n1 36 1225 
// Test: Triangular numbers

int is_triangular(int n) {
    // n is triangular if 8n+1 is a perfect square
    int val = 8 * n + 1;
    int sq = 1;
    while (sq * sq < val) sq++;
    return sq * sq == val;
}

int main(void) {
    int tri[30];
    int i, j;

    // Generate triangular numbers T(n) = n*(n+1)/2
    printf("Triangular numbers:\n");
    for (i = 1; i <= 20; i++) {
        tri[i] = i * (i + 1) / 2;
        printf("T(%d) = %d\n", i, tri[i]);
    }

    // Properties: T(n) = sum of 1..n
    printf("\nVerification T(n) = sum 1..n:\n");
    for (i = 1; i <= 10; i++) {
        int sum = 0;
        for (j = 1; j <= i; j++) sum = sum + j;
        printf("T(%d) = %d, sum = %d, match = %d\n", i, tri[i], sum, tri[i] == sum);
    }

    // T(n) + T(n-1) = n^2
    printf("\nT(n) + T(n-1) = n^2:\n");
    tri[0] = 0;
    for (i = 1; i <= 10; i++) {
        printf("T(%d)+T(%d) = %d+%d = %d, %d^2 = %d\n",
               i, i - 1, tri[i], tri[i - 1], tri[i] + tri[i - 1], i, i * i);
    }

    // Check which numbers are triangular
    printf("\nTriangular numbers up to 100:\n");
    int tcount = 0;
    for (i = 1; i <= 100; i++) {
        if (is_triangular(i)) {
            printf("%d ", i);
            tcount++;
        }
    }
    printf("\nCount: %d\n", tcount);

    // Sum of consecutive triangular numbers
    printf("\nSum of first n triangular numbers:\n");
    int cumsum = 0;
    for (i = 1; i <= 10; i++) {
        cumsum = cumsum + tri[i];
        int expected = i * (i + 1) * (i + 2) / 6;
        printf("Sum T(1)..T(%d) = %d (formula: %d)\n", i, cumsum, expected);
    }

    // Triangular numbers that are also perfect squares
    printf("\nSquare triangular numbers up to 10000:\n");
    for (i = 1; i <= 10000; i++) {
        if (is_triangular(i)) {
            int sq = 1;
            while (sq * sq < i) sq++;
            if (sq * sq == i) {
                printf("%d ", i);
            }
        }
    }
    printf("\n");

    return 0;
}
