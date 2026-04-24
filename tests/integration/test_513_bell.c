int printf(const char *fmt, ...);
// EXPECT: Bell triangle:\n1 \n1 2 \n2 3 5 \n5 7 10 15 \n15 20 27 37 52 \n52 67 87 114 151 203 \n203 255 322 409 523 674 877 \n877 1080 1335 1657 2066 2589 3263 4140 \n4140 5017 6097 7432 9089 11155 13744 17007 21147 \n21147 25287 30304 36401 43833 52922 64077 77821 94828 115975 \n\nBell numbers B(n) = first element of each row:\nB(0) = 1\nB(1) = 1\nB(2) = 2\nB(3) = 5\nB(4) = 15\nB(5) = 52\nB(6) = 203\nB(7) = 877\nB(8) = 4140\nB(9) = 21147\nB(10) = 115975\nB(11) = 678570\n\nVerification via Stirling numbers:\nB(0) = 1 (bell triangle: 1) match: 1\nB(1) = 1 (bell triangle: 1) match: 1\nB(2) = 2 (bell triangle: 2) match: 1\nB(3) = 5 (bell triangle: 5) match: 1\nB(4) = 15 (bell triangle: 15) match: 1\nB(5) = 52 (bell triangle: 52) match: 1\nB(6) = 203 (bell triangle: 203) match: 1\nB(7) = 877 (bell triangle: 877) match: 1\nB(8) = 4140 (bell triangle: 4140) match: 1\nB(9) = 21147 (bell triangle: 21147) match: 1\n\nBell number ratios B(n+1)/B(n) (x100):\nB(2)/B(1) = 200/100\nB(3)/B(2) = 250/100\nB(4)/B(3) = 300/100\nB(5)/B(4) = 346/100\nB(6)/B(5) = 390/100\nB(7)/B(6) = 432/100\nB(8)/B(7) = 472/100\nB(9)/B(8) = 510/100\nB(10)/B(9) = 548/100\n\nPartitions of {1,2,3}: B(3) = 5\nPartitions of {1,2,3,4}: B(4) = 15
// Test: Bell numbers

int main(void) {
    // Bell triangle method
    int bell[12][12];
    int i, j;

    for (i = 0; i < 12; i++) {
        for (j = 0; j < 12; j++) {
            bell[i][j] = 0;
        }
    }

    bell[0][0] = 1;
    for (i = 1; i < 12; i++) {
        bell[i][0] = bell[i - 1][i - 1];
        for (j = 1; j <= i; j++) {
            bell[i][j] = bell[i][j - 1] + bell[i - 1][j - 1];
        }
    }

    printf("Bell triangle:\n");
    for (i = 0; i < 10; i++) {
        for (j = 0; j <= i; j++) {
            printf("%d ", bell[i][j]);
        }
        printf("\n");
    }

    printf("\nBell numbers B(n) = first element of each row:\n");
    for (i = 0; i < 12; i++) {
        printf("B(%d) = %d\n", i, bell[i][0]);
    }

    // Verify using Stirling numbers: B(n) = sum S(n,k)
    int S[12][12];
    for (i = 0; i < 12; i++)
        for (j = 0; j < 12; j++)
            S[i][j] = 0;
    S[0][0] = 1;
    for (i = 1; i < 12; i++) {
        for (j = 1; j <= i; j++) {
            S[i][j] = j * S[i - 1][j] + S[i - 1][j - 1];
        }
    }

    printf("\nVerification via Stirling numbers:\n");
    for (i = 0; i < 10; i++) {
        int sum = 0;
        for (j = 0; j <= i; j++) {
            sum = sum + S[i][j];
        }
        printf("B(%d) = %d (bell triangle: %d) match: %d\n",
               i, sum, bell[i][0], sum == bell[i][0]);
    }

    // Dobinski's formula approximation: B(n) ~ (1/e) * sum k^n/k!
    // Use integer approximation for small n
    printf("\nBell number ratios B(n+1)/B(n) (x100):\n");
    for (i = 1; i < 10; i++) {
        int ratio = bell[i + 1][0] * 100 / bell[i][0];
        printf("B(%d)/B(%d) = %d/100\n", i + 1, i, ratio);
    }

    // Count set partitions explicitly for n=3
    // {1,2,3}: {{1,2,3}}, {{1},{2,3}}, {{1,2},{3}}, {{1,3},{2}}, {{1},{2},{3}}
    printf("\nPartitions of {1,2,3}: B(3) = %d\n", bell[3][0]);
    printf("Partitions of {1,2,3,4}: B(4) = %d\n", bell[4][0]);

    return 0;
}
