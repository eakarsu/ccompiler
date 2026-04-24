int printf(const char *fmt, ...);
// EXPECT: factorials: 1 1 2 6 24 120 720 5040 40320 362880 3628800 \nmatch: 1 1 1 1 1 1 1 1 1 1 1 \nP(5,2) = 20\nP(5,3) = 60\nP(6,2) = 30\nC(5,2) = 10\nC(5,3) = 10\nC(6,3) = 20\nC(10,5) = 252\npascal row 6: 1 6 15 20 15 6 1 \ne*10000 ~ 27179\nD(4) = 9
// Test: factorials and permutations

int factorial(int n) {
    int result, i;
    result = 1;
    for (i = 2; i <= n; i++) {
        result = result * i;
    }
    return result;
}

int factorial_rec(int n) {
    if (n <= 1) return 1;
    return n * factorial_rec(n - 1);
}

int permutations(int n, int r) {
    /* P(n,r) = n! / (n-r)! */
    return factorial(n) / factorial(n - r);
}

int combinations(int n, int r) {
    /* C(n,r) = n! / (r! * (n-r)!) */
    return factorial(n) / (factorial(r) * factorial(n - r));
}

int main(void) {
    int i;
    int sum;

    /* Factorials 0-10 */
    printf("factorials: ");
    for (i = 0; i <= 10; i++) {
        printf("%d ", factorial(i));
    }
    printf("\n");

    /* Verify recursive matches iterative */
    printf("match: ");
    for (i = 0; i <= 10; i++) {
        if (factorial(i) == factorial_rec(i)) {
            printf("1 ");
        } else {
            printf("0 ");
        }
    }
    printf("\n");

    /* Permutations */
    printf("P(5,2) = %d\n", permutations(5, 2));
    printf("P(5,3) = %d\n", permutations(5, 3));
    printf("P(6,2) = %d\n", permutations(6, 2));

    /* Combinations */
    printf("C(5,2) = %d\n", combinations(5, 2));
    printf("C(5,3) = %d\n", combinations(5, 3));
    printf("C(6,3) = %d\n", combinations(6, 3));
    printf("C(10,5) = %d\n", combinations(10, 5));

    /* Pascal's triangle row 6 */
    printf("pascal row 6: ");
    for (i = 0; i <= 6; i++) {
        printf("%d ", combinations(6, i));
    }
    printf("\n");

    /* e approximation: sum of 1/n! (scaled by 10000) */
    sum = 0;
    for (i = 0; i <= 8; i++) {
        sum += 10000 / factorial(i);
    }
    printf("e*10000 ~ %d\n", sum);

    /* Derangements D(n) = n! * sum(-1^k/k!, k=0..n) */
    /* D(4) = 9, D(5) = 44 */
    {
        int n, k, d;
        int sign;
        n = 4;
        d = 0;
        sign = 1;
        for (k = 0; k <= n; k++) {
            d += sign * factorial(n) / factorial(k);
            sign = -sign;
        }
        printf("D(4) = %d\n", d);
    }

    return 0;
}
