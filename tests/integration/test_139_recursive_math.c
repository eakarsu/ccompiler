int printf(const char *fmt, ...);
// EXPECT: sum(10) = 55\nsum(100) = 5050\n2^10 = 1024\n3^5 = 243\nack(0,0) = 1\nack(1,1) = 3\nack(2,2) = 7\nack(3,2) = 29\nack(3,3) = 61\nC(n,k) row 5: 1 5 10 10 5 1 \nC(n,k) row 7: 1 7 21 35 35 21 7 1 \ncatalan: 1 1 2 5 14 42 132 \npaths(3,3) = 6\npaths(4,4) = 20\npaths(5,5) = 70\nsymmetry: 1 1 1 1 1 1 1 
// Test: recursive mathematical functions

int sum_rec(int n) {
    if (n <= 0) return 0;
    return n + sum_rec(n - 1);
}

int power_rec(int base, int exp) {
    if (exp == 0) return 1;
    return base * power_rec(base, exp - 1);
}

int ackermann(int m, int n) {
    if (m == 0) return n + 1;
    if (n == 0) return ackermann(m - 1, 1);
    return ackermann(m - 1, ackermann(m, n - 1));
}

int binomial(int n, int k) {
    if (k == 0 || k == n) return 1;
    return binomial(n - 1, k - 1) + binomial(n - 1, k);
}

int catalan_rec(int n) {
    int sum, i;
    if (n <= 1) return 1;
    sum = 0;
    for (i = 0; i < n; i++) {
        sum += catalan_rec(i) * catalan_rec(n - 1 - i);
    }
    return sum;
}

int count_paths(int m, int n) {
    if (m == 1 || n == 1) return 1;
    return count_paths(m - 1, n) + count_paths(m, n - 1);
}

int main(void) {
    int i;

    /* Recursive sum */
    printf("sum(10) = %d\n", sum_rec(10));
    printf("sum(100) = %d\n", sum_rec(100));

    /* Recursive power */
    printf("2^10 = %d\n", power_rec(2, 10));
    printf("3^5 = %d\n", power_rec(3, 5));

    /* Ackermann (small values only!) */
    printf("ack(0,0) = %d\n", ackermann(0, 0));
    printf("ack(1,1) = %d\n", ackermann(1, 1));
    printf("ack(2,2) = %d\n", ackermann(2, 2));
    printf("ack(3,2) = %d\n", ackermann(3, 2));
    printf("ack(3,3) = %d\n", ackermann(3, 3));

    /* Binomial coefficients */
    printf("C(n,k) row 5: ");
    for (i = 0; i <= 5; i++) {
        printf("%d ", binomial(5, i));
    }
    printf("\n");

    printf("C(n,k) row 7: ");
    for (i = 0; i <= 7; i++) {
        printf("%d ", binomial(7, i));
    }
    printf("\n");

    /* Catalan numbers */
    printf("catalan: ");
    for (i = 0; i <= 6; i++) {
        printf("%d ", catalan_rec(i));
    }
    printf("\n");

    /* Grid paths */
    printf("paths(3,3) = %d\n", count_paths(3, 3));
    printf("paths(4,4) = %d\n", count_paths(4, 4));
    printf("paths(5,5) = %d\n", count_paths(5, 5));

    /* Verify binomial identity: C(n,k) = C(n,n-k) */
    printf("symmetry: ");
    for (i = 0; i <= 6; i++) {
        printf("%d ", binomial(6, i) == binomial(6, 6 - i));
    }
    printf("\n");

    return 0;
}
