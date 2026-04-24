int printf(const char *fmt, ...);
// EXPECT: sum 1..50 = 1275\nformula = 1275\nsum sq 1..10 = 385\nformula = 385\nsum cube 1..10 = 3025\nformula = 3025\ngeo sum (10 terms) = 1023\nformula = 1023\nH(10)*10000 ~ 29288\ntriangular: 1 3 6 10 15 21 28 36 45 55 \npentagonal: 1 5 12 22 35 51 70 92 \ntetrahedral: 1 4 10 20 35 56 84 120 \ncatalan: 1 1 2 5 14 42 132 429 
// Test: series sums and mathematical sequences

int main(void) {
    int i;
    int sum;
    int n;

    /* Arithmetic series: 1 + 2 + ... + n */
    n = 50;
    sum = 0;
    for (i = 1; i <= n; i++) {
        sum += i;
    }
    printf("sum 1..50 = %d\n", sum);
    printf("formula = %d\n", n * (n + 1) / 2);

    /* Sum of squares: 1^2 + 2^2 + ... + n^2 */
    n = 10;
    sum = 0;
    for (i = 1; i <= n; i++) {
        sum += i * i;
    }
    printf("sum sq 1..10 = %d\n", sum);
    printf("formula = %d\n", n * (n + 1) * (2 * n + 1) / 6);

    /* Sum of cubes: 1^3 + 2^3 + ... + n^3 */
    n = 10;
    sum = 0;
    for (i = 1; i <= n; i++) {
        sum += i * i * i;
    }
    printf("sum cube 1..10 = %d\n", sum);
    {
        int s;
        s = n * (n + 1) / 2;
        printf("formula = %d\n", s * s);
    }

    /* Geometric series: 1 + 2 + 4 + 8 + ... (powers of 2) */
    sum = 0;
    {
        int term;
        term = 1;
        for (i = 0; i < 10; i++) {
            sum += term;
            term *= 2;
        }
    }
    printf("geo sum (10 terms) = %d\n", sum);
    printf("formula = %d\n", (1 << 10) - 1);

    /* Harmonic series approximation: scaled by 10000 */
    sum = 0;
    for (i = 1; i <= 10; i++) {
        sum += 10000 / i;
    }
    printf("H(10)*10000 ~ %d\n", sum);

    /* Triangular numbers */
    printf("triangular: ");
    for (i = 1; i <= 10; i++) {
        printf("%d ", i * (i + 1) / 2);
    }
    printf("\n");

    /* Pentagonal numbers */
    printf("pentagonal: ");
    for (i = 1; i <= 8; i++) {
        printf("%d ", i * (3 * i - 1) / 2);
    }
    printf("\n");

    /* Tetrahedral numbers: T(n) = n*(n+1)*(n+2)/6 */
    printf("tetrahedral: ");
    for (i = 1; i <= 8; i++) {
        printf("%d ", i * (i + 1) * (i + 2) / 6);
    }
    printf("\n");

    /* Catalan numbers (first few) */
    {
        int cat[10];
        int j;
        cat[0] = 1;
        for (i = 1; i < 8; i++) {
            cat[i] = 0;
            for (j = 0; j < i; j++) {
                cat[i] += cat[j] * cat[i - 1 - j];
            }
        }
        printf("catalan: ");
        for (i = 0; i < 8; i++) {
            printf("%d ", cat[i]);
        }
        printf("\n");
    }

    return 0;
}
