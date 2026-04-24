int printf(const char *fmt, ...);
// EXPECT: Partition numbers p(n):\np(0) = 1\np(1) = 1\np(2) = 2\np(3) = 3\np(4) = 5\np(5) = 7\np(6) = 11\np(7) = 15\np(8) = 22\np(9) = 30\np(10) = 42\np(11) = 56\np(12) = 77\np(13) = 101\np(14) = 135\np(15) = 176\np(16) = 231\np(17) = 297\np(18) = 385\np(19) = 490\np(20) = 627\np(21) = 792\np(22) = 1002\np(23) = 1255\np(24) = 1575\np(25) = 1958\np(26) = 2436\np(27) = 3010\np(28) = 3718\np(29) = 4565\np(30) = 5604\n\nPartitions into distinct parts:\npd(0) = 1\npd(1) = 1\npd(2) = 1\npd(3) = 2\npd(4) = 2\npd(5) = 3\npd(6) = 4\npd(7) = 5\npd(8) = 6\npd(9) = 8\npd(10) = 10\npd(11) = 12\npd(12) = 15\npd(13) = 18\npd(14) = 22\npd(15) = 27\n\nEuler's theorem: distinct parts = odd parts:\nn=0: distinct=1, odd=1, match=1\nn=1: distinct=1, odd=1, match=1\nn=2: distinct=1, odd=1, match=1\nn=3: distinct=2, odd=2, match=1\nn=4: distinct=2, odd=2, match=1\nn=5: distinct=3, odd=3, match=1\nn=6: distinct=4, odd=4, match=1\nn=7: distinct=5, odd=5, match=1\nn=8: distinct=6, odd=6, match=1\nn=9: distinct=8, odd=8, match=1\nn=10: distinct=10, odd=10, match=1\nn=11: distinct=12, odd=12, match=1\nn=12: distinct=15, odd=15, match=1\nn=13: distinct=18, odd=18, match=1\nn=14: distinct=22, odd=22, match=1\nn=15: distinct=27, odd=27, match=1\n\nPartitions of 5:\n5\n4+1\n3+2\n3+1+1\n2+2+1\n2+1+1+1\n1+1+1+1+1\nCount: 7
// Test: Integer partition numbers

int main(void) {
    // p(n) = number of ways to write n as sum of positive integers
    // Using DP: p[n] = sum of p[n-k] for each part k
    int p[50];
    int i, j;

    for (i = 0; i < 50; i++) p[i] = 0;
    p[0] = 1;

    // Standard partition function using coin-change style DP
    for (i = 1; i < 50; i++) {
        for (j = i; j < 50; j++) {
            p[j] = p[j] + p[j - i];
        }
    }

    printf("Partition numbers p(n):\n");
    for (i = 0; i <= 30; i++) {
        printf("p(%d) = %d\n", i, p[i]);
    }

    // Partitions into distinct parts
    int pd[50];
    for (i = 0; i < 50; i++) pd[i] = 0;
    pd[0] = 1;
    for (i = 1; i < 50; i++) {
        for (j = 49; j >= i; j--) {
            pd[j] = pd[j] + pd[j - i];
        }
    }

    printf("\nPartitions into distinct parts:\n");
    for (i = 0; i <= 15; i++) {
        printf("pd(%d) = %d\n", i, pd[i]);
    }

    // Partitions into odd parts
    int po[50];
    for (i = 0; i < 50; i++) po[i] = 0;
    po[0] = 1;
    for (i = 1; i < 50; i = i + 2) { // only odd parts
        for (j = i; j < 50; j++) {
            po[j] = po[j] + po[j - i];
        }
    }

    // Euler's theorem: partitions into distinct parts = partitions into odd parts
    printf("\nEuler's theorem: distinct parts = odd parts:\n");
    for (i = 0; i <= 15; i++) {
        printf("n=%d: distinct=%d, odd=%d, match=%d\n",
               i, pd[i], po[i], pd[i] == po[i]);
    }

    // List partitions of 5 explicitly
    printf("\nPartitions of 5:\n");
    printf("5\n4+1\n3+2\n3+1+1\n2+2+1\n2+1+1+1\n1+1+1+1+1\n");
    printf("Count: %d\n", p[5]);

    return 0;
}
