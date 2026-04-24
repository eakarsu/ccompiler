int printf(const char *fmt, ...);
// EXPECT: Mobius function mu(n):\nmu(1) = 1\nmu(2) = -1\nmu(3) = -1\nmu(4) = 0\nmu(5) = -1\nmu(6) = 1\nmu(7) = -1\nmu(8) = 0\nmu(9) = 0\nmu(10) = 1\nmu(11) = -1\nmu(12) = 0\nmu(13) = -1\nmu(14) = 1\nmu(15) = 1\nmu(16) = 0\nmu(17) = -1\nmu(18) = 0\nmu(19) = -1\nmu(20) = 0\nmu(21) = 1\nmu(22) = 1\nmu(23) = -1\nmu(24) = 0\nmu(25) = 0\nmu(26) = 1\nmu(27) = 0\nmu(28) = 0\nmu(29) = -1\nmu(30) = -1\n\nMertens function M(n) = sum mu(k) for k=1..n:\nM(1) = 1\nM(2) = 0\nM(3) = -1\nM(4) = -1\nM(5) = -2\nM(6) = -1\nM(7) = -2\nM(8) = -2\nM(9) = -2\nM(10) = -1\nM(11) = -2\nM(12) = -2\nM(13) = -3\nM(14) = -2\nM(15) = -1\nM(16) = -1\nM(17) = -2\nM(18) = -2\nM(19) = -3\nM(20) = -3\nM(21) = -2\nM(22) = -1\nM(23) = -2\nM(24) = -2\nM(25) = -2\nM(26) = -1\nM(27) = -1\nM(28) = -1\nM(29) = -2\nM(30) = -3\n\nDivisor sum property: sum mu(d) for d|n:\nn=1: sum=1 (expected 1)\nn=2: sum=0 (expected 0)\nn=3: sum=0 (expected 0)\nn=4: sum=0 (expected 0)\nn=5: sum=0 (expected 0)\nn=6: sum=0 (expected 0)\nn=7: sum=0 (expected 0)\nn=8: sum=0 (expected 0)\nn=9: sum=0 (expected 0)\nn=10: sum=0 (expected 0)\nn=11: sum=0 (expected 0)\nn=12: sum=0 (expected 0)\nn=13: sum=0 (expected 0)\nn=14: sum=0 (expected 0)\nn=15: sum=0 (expected 0)\n\nSquarefree numbers up to 30: 1 2 3 5 6 7 10 11 13 14 15 17 19 21 22 23 26 29 30 \nCount: 19\n\nVerification mu(n)!=0 iff squarefree:\nAll match: 1
// Test: Mobius function

int mobius(int n) {
    if (n == 1) return 1;
    int count = 0;
    int d = 2;
    int temp = n;
    while (d * d <= temp) {
        if (temp % d == 0) {
            count++;
            temp = temp / d;
            if (temp % d == 0) {
                return 0; // has squared factor
            }
        }
        d++;
    }
    if (temp > 1) count++;
    if (count % 2 == 0) return 1;
    return -1;
}

int main(void) {
    int i, j;

    printf("Mobius function mu(n):\n");
    for (i = 1; i <= 30; i++) {
        printf("mu(%d) = %d\n", i, mobius(i));
    }

    // Property: sum of mu(d) for d|n equals 0 for n>1, 1 for n=1
    printf("\nMertens function M(n) = sum mu(k) for k=1..n:\n");
    int mertens = 0;
    for (i = 1; i <= 30; i++) {
        mertens = mertens + mobius(i);
        printf("M(%d) = %d\n", i, mertens);
    }

    // Verify: sum mu(d) for d|n = [n==1]
    printf("\nDivisor sum property: sum mu(d) for d|n:\n");
    for (i = 1; i <= 15; i++) {
        int sum = 0;
        for (j = 1; j <= i; j++) {
            if (i % j == 0) {
                sum = sum + mobius(j);
            }
        }
        printf("n=%d: sum=%d (expected %d)\n", i, sum, i == 1 ? 1 : 0);
    }

    // Count squarefree numbers up to N
    printf("\nSquarefree numbers up to 30: ");
    int sf_count = 0;
    for (i = 1; i <= 30; i++) {
        int is_squarefree = 1;
        for (j = 2; j * j <= i; j++) {
            if (i % (j * j) == 0) {
                is_squarefree = 0;
                break;
            }
        }
        if (is_squarefree) {
            printf("%d ", i);
            sf_count++;
        }
    }
    printf("\nCount: %d\n", sf_count);

    // Verify squarefree <=> mu(n) != 0
    printf("\nVerification mu(n)!=0 iff squarefree:\n");
    int match = 1;
    for (i = 1; i <= 30; i++) {
        int is_sf = 1;
        for (j = 2; j * j <= i; j++) {
            if (i % (j * j) == 0) { is_sf = 0; break; }
        }
        int mu_nonzero = (mobius(i) != 0) ? 1 : 0;
        if (is_sf != mu_nonzero) match = 0;
    }
    printf("All match: %d\n", match);

    return 0;
}
