int printf(const char *fmt, ...);
// EXPECT: Hexagonal numbers:\nH(1) = 1\nH(2) = 6\nH(3) = 15\nH(4) = 28\nH(5) = 45\nH(6) = 66\nH(7) = 91\nH(8) = 120\nH(9) = 153\nH(10) = 190\nH(11) = 231\nH(12) = 276\nH(13) = 325\nH(14) = 378\nH(15) = 435\nH(16) = 496\nH(17) = 561\nH(18) = 630\nH(19) = 703\nH(20) = 780\n\nHexagonal = Triangular check:\nH(1) = 1, T(1) = 1, match = 1\nH(2) = 6, T(3) = 6, match = 1\nH(3) = 15, T(5) = 15, match = 1\nH(4) = 28, T(7) = 28, match = 1\nH(5) = 45, T(9) = 45, match = 1\nH(6) = 66, T(11) = 66, match = 1\nH(7) = 91, T(13) = 91, match = 1\nH(8) = 120, T(15) = 120, match = 1\nH(9) = 153, T(17) = 153, match = 1\nH(10) = 190, T(19) = 190, match = 1\n\nCentered hexagonal numbers:\nCH(1) = 1\nCH(2) = 7\nCH(3) = 19\nCH(4) = 37\nCH(5) = 61\nCH(6) = 91\nCH(7) = 127\nCH(8) = 169\nCH(9) = 217\nCH(10) = 271\nCH(11) = 331\nCH(12) = 397\nCH(13) = 469\nCH(14) = 547\nCH(15) = 631\n\nPartial sums of hexagonal numbers:\nSum H(1..1) = 1 (formula: 1)\nSum H(1..2) = 7 (formula: 7)\nSum H(1..3) = 22 (formula: 22)\nSum H(1..4) = 50 (formula: 50)\nSum H(1..5) = 95 (formula: 95)\nSum H(1..6) = 161 (formula: 161)\nSum H(1..7) = 252 (formula: 252)\nSum H(1..8) = 372 (formula: 372)\nSum H(1..9) = 525 (formula: 525)\nSum H(1..10) = 715 (formula: 715)\n\nDifferences H(n+1) - H(n) = 4n+1:\nH(2)-H(1) = 5, 4*1+1 = 5\nH(3)-H(2) = 9, 4*2+1 = 9\nH(4)-H(3) = 13, 4*3+1 = 13\nH(5)-H(4) = 17, 4*4+1 = 17\nH(6)-H(5) = 21, 4*5+1 = 21\nH(7)-H(6) = 25, 4*6+1 = 25\nH(8)-H(7) = 29, 4*7+1 = 29\nH(9)-H(8) = 33, 4*8+1 = 33\nH(10)-H(9) = 37, 4*9+1 = 37\nH(11)-H(10) = 41, 4*10+1 = 41\nH(12)-H(11) = 45, 4*11+1 = 45\n\nHexagonal numbers up to 200:\n1 6 15 28 45 66 91 120 153 190 \nCount: 10
// Test: Hexagonal numbers

int main(void) {
    int hex[25];
    int tri[50];
    int i, j;

    // Hexagonal numbers: H(n) = n*(2n-1)
    printf("Hexagonal numbers:\n");
    for (i = 1; i <= 20; i++) {
        hex[i] = i * (2 * i - 1);
        printf("H(%d) = %d\n", i, hex[i]);
    }

    // Triangular numbers for comparison
    for (i = 1; i <= 40; i++) {
        tri[i] = i * (i + 1) / 2;
    }

    // Every hexagonal number is also triangular: H(n) = T(2n-1)
    printf("\nHexagonal = Triangular check:\n");
    for (i = 1; i <= 10; i++) {
        int t_idx = 2 * i - 1;
        printf("H(%d) = %d, T(%d) = %d, match = %d\n",
               i, hex[i], t_idx, tri[t_idx], hex[i] == tri[t_idx]);
    }

    // Centered hexagonal numbers: 3n^2 - 3n + 1
    printf("\nCentered hexagonal numbers:\n");
    for (i = 1; i <= 15; i++) {
        int ch = 3 * i * i - 3 * i + 1;
        printf("CH(%d) = %d\n", i, ch);
    }

    // Sum of hexagonal numbers
    printf("\nPartial sums of hexagonal numbers:\n");
    int hsum = 0;
    for (i = 1; i <= 10; i++) {
        hsum = hsum + hex[i];
        // Formula: sum H(k) for k=1..n = n^2*(2n+1)/3... actually n*(n+1)*(4n-1)/6
        int formula = i * (i + 1) * (4 * i - 1) / 6;
        printf("Sum H(1..%d) = %d (formula: %d)\n", i, hsum, formula);
    }

    // Differences
    printf("\nDifferences H(n+1) - H(n) = 4n+1:\n");
    for (i = 1; i < 12; i++) {
        int diff = hex[i + 1] - hex[i];
        printf("H(%d)-H(%d) = %d, 4*%d+1 = %d\n",
               i + 1, i, diff, i, 4 * i + 1);
    }

    // Check: which numbers up to 200 are hexagonal
    printf("\nHexagonal numbers up to 200:\n");
    int hcount = 0;
    for (i = 1; i <= 200; i++) {
        int found = 0;
        for (j = 1; j <= 20; j++) {
            if (hex[j] == i) { found = 1; break; }
        }
        if (found) {
            printf("%d ", i);
            hcount++;
        }
    }
    printf("\nCount: %d\n", hcount);

    return 0;
}
