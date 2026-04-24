int printf(const char *fmt, ...);
// EXPECT: Pentagonal numbers:\nP(1) = 1\nP(2) = 5\nP(3) = 12\nP(4) = 22\nP(5) = 35\nP(6) = 51\nP(7) = 70\nP(8) = 92\nP(9) = 117\nP(10) = 145\nP(11) = 176\nP(12) = 210\nP(13) = 247\nP(14) = 287\nP(15) = 330\nP(16) = 376\nP(17) = 425\nP(18) = 477\nP(19) = 532\nP(20) = 590\n\nGeneralized pentagonal numbers:\nk=1: GP = 1\nk=-1: GP = 2\nk=2: GP = 5\nk=-2: GP = 7\nk=3: GP = 12\nk=-3: GP = 15\nk=4: GP = 22\nk=-4: GP = 26\nk=5: GP = 35\nk=-5: GP = 40\n\nPentagonal check for 1-50:\n1 5 12 22 35 \n\nPartition numbers via pentagonal theorem:\np(0) = 1\np(1) = 1\np(2) = 2\np(3) = 3\np(4) = 5\np(5) = 7\np(6) = 11\np(7) = 15\np(8) = 22\np(9) = 30\np(10) = 42\np(11) = 56\np(12) = 77\np(13) = 101\np(14) = 135\np(15) = 176\np(16) = 231\np(17) = 297\np(18) = 385\np(19) = 490\np(20) = 627\n\nDifferences P(n+1) - P(n):\nP(2)-P(1) = 4\nP(3)-P(2) = 7\nP(4)-P(3) = 10\nP(5)-P(4) = 13\nP(6)-P(5) = 16\nP(7)-P(6) = 19\nP(8)-P(7) = 22\nP(9)-P(8) = 25\nP(10)-P(9) = 28\nP(11)-P(10) = 31\nP(12)-P(11) = 34\nP(13)-P(12) = 37\nP(14)-P(13) = 40\nP(15)-P(14) = 43
// Test: Pentagonal numbers

int main(void) {
    int pent[30];
    int i, j;

    // Pentagonal numbers: P(n) = n*(3n-1)/2
    printf("Pentagonal numbers:\n");
    for (i = 1; i <= 20; i++) {
        pent[i] = i * (3 * i - 1) / 2;
        printf("P(%d) = %d\n", i, pent[i]);
    }

    // Generalized pentagonal numbers (include negative indices)
    printf("\nGeneralized pentagonal numbers:\n");
    int gen_pent[20];
    int idx = 0;
    for (i = 0; i < 10; i++) {
        int k;
        if (i % 2 == 0) {
            k = i / 2 + 1;
        } else {
            k = -(i / 2 + 1);
        }
        gen_pent[idx] = k * (3 * k - 1) / 2;
        printf("k=%d: GP = %d\n", k, gen_pent[idx]);
        idx++;
    }

    // Check if a number is pentagonal
    printf("\nPentagonal check for 1-50:\n");
    for (i = 1; i <= 50; i++) {
        int is_pent = 0;
        for (j = 1; j <= 20; j++) {
            if (pent[j] == i) { is_pent = 1; break; }
        }
        if (is_pent) printf("%d ", i);
    }
    printf("\n");

    // Pentagonal number theorem connection to partitions
    // p(n) - p(n-1) - p(n-2) + p(n-5) + p(n-7) - ... = 0
    // Compute partition function using pentagonal theorem
    int p[50];
    p[0] = 1;
    for (i = 1; i < 40; i++) {
        p[i] = 0;
        int k = 1;
        int sign = 1;
        while (1) {
            int g1 = k * (3 * k - 1) / 2;
            int g2 = k * (3 * k + 1) / 2;
            if (g1 > i) break;
            p[i] = p[i] + sign * p[i - g1];
            if (g2 <= i) {
                p[i] = p[i] + sign * p[i - g2];
            }
            k++;
            sign = -sign;
        }
    }

    printf("\nPartition numbers via pentagonal theorem:\n");
    for (i = 0; i <= 20; i++) {
        printf("p(%d) = %d\n", i, p[i]);
    }

    // Differences between consecutive pentagonal numbers
    printf("\nDifferences P(n+1) - P(n):\n");
    for (i = 1; i < 15; i++) {
        printf("P(%d)-P(%d) = %d\n", i + 1, i, pent[i + 1] - pent[i]);
    }

    return 0;
}
