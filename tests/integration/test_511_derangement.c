int printf(const char *fmt, ...);
// EXPECT: Derangements D(n):\nD(0) = 1\nD(1) = 0\nD(2) = 1\nD(3) = 2\nD(4) = 9\nD(5) = 44\nD(6) = 265\nD(7) = 1854\nD(8) = 14833\nD(9) = 133496\nD(10) = 1334961\nD(11) = 14684570\nD(12) = 176214841\n\nD(n)/n! ratios (x10000):\nD(1)/1! = 0/10000\nD(2)/2! = 5000/10000\nD(3)/3! = 3333/10000\nD(4)/4! = 3750/10000\nD(5)/5! = 3666/10000\nD(6)/6! = 3680/10000\nD(7)/7! = 3678/10000\nD(8)/8! = 3678/10000\nD(9)/9! = 3678/10000\nD(10)/10! = 128/10000\nD(11)/11! = 20/10000\n\nVerification via inclusion-exclusion:\nD(2) = 1 (recurrence: 1) match: 1\nD(3) = 2 (recurrence: 2) match: 1\nD(4) = 9 (recurrence: 9) match: 1\nD(5) = 44 (recurrence: 44) match: 1\nD(6) = 265 (recurrence: 265) match: 1\nD(7) = 1854 (recurrence: 1854) match: 1\nD(8) = 14833 (recurrence: 14833) match: 1\n\nPermutations of 6 with k fixed points:\nk=0: C(6,0)*D(6) = 1*265 = 265\nk=1: C(6,1)*D(5) = 6*44 = 264\nk=2: C(6,2)*D(4) = 15*9 = 135\nk=3: C(6,3)*D(3) = 20*2 = 40\nk=4: C(6,4)*D(2) = 15*1 = 15\nk=5: C(6,5)*D(1) = 6*0 = 0\nk=6: C(6,6)*D(0) = 1*1 = 1\nSum = 720 (should be 720)
// Test: Derangement counting

int main(void) {
    int D[15];
    int fact[15];
    int i, j;

    // Compute factorials
    fact[0] = 1;
    for (i = 1; i < 15; i++) {
        fact[i] = fact[i - 1] * i;
    }

    // Derangements: D(n) = (n-1) * (D(n-1) + D(n-2))
    // D(0) = 1, D(1) = 0
    D[0] = 1;
    D[1] = 0;
    for (i = 2; i < 13; i++) {
        D[i] = (i - 1) * (D[i - 1] + D[i - 2]);
    }

    printf("Derangements D(n):\n");
    for (i = 0; i < 13; i++) {
        printf("D(%d) = %d\n", i, D[i]);
    }

    // Ratio D(n)/n! approaches 1/e ~ 0.3679
    printf("\nD(n)/n! ratios (x10000):\n");
    for (i = 1; i < 12; i++) {
        int ratio = D[i] * 10000 / fact[i];
        printf("D(%d)/%d! = %d/10000\n", i, i, ratio);
    }

    // Verify using inclusion-exclusion:
    // D(n) = n! * sum_{k=0}^{n} (-1)^k / k!
    printf("\nVerification via inclusion-exclusion:\n");
    for (i = 2; i <= 8; i++) {
        int result = 0;
        int sign = 1;
        for (j = 0; j <= i; j++) {
            result = result + sign * fact[i] / fact[j];
            sign = -sign;
        }
        printf("D(%d) = %d (recurrence: %d) match: %d\n",
               i, result, D[i], result == D[i]);
    }

    // Partial derangements: exactly k fixed points
    // Number of permutations of n with exactly k fixed points:
    // C(n,k) * D(n-k)
    printf("\nPermutations of 6 with k fixed points:\n");
    for (i = 0; i <= 6; i++) {
        // C(6,i) * D(6-i)
        int binom = fact[6] / (fact[i] * fact[6 - i]);
        int count = binom * D[6 - i];
        printf("k=%d: C(6,%d)*D(%d) = %d*%d = %d\n",
               i, i, 6 - i, binom, D[6 - i], count);
    }

    // Sum should equal 6!
    int sum = 0;
    for (i = 0; i <= 6; i++) {
        int binom = fact[6] / (fact[i] * fact[6 - i]);
        sum = sum + binom * D[6 - i];
    }
    printf("Sum = %d (should be %d)\n", sum, fact[6]);

    return 0;
}
