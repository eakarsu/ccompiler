int printf(const char *fmt, ...);
// EXPECT: perfect: 6 28 496 \nsigma: 0 1 1 3 1 6 1 7 4 8 1 16 1 10 9 \nabundant count <= 50: 9\nabundant: 12 18 20 24 30 36 40 42 48 54 \ndeficient count <= 20: 16\nharshad: 1 2 3 4 5 6 7 8 9 10 12 18 20 21 24 27 30 36 40 42 45 48 50 \nsigma(220) = 284\nsigma(284) = 220\namicable: 1\nclassify: D D D D D P D D D D D A 
// Test: number properties (perfect, abundant, deficient)

int sum_divisors(int n) {
    int sum, i;
    sum = 0;
    for (i = 1; i < n; i++) {
        if (n % i == 0) {
            sum += i;
        }
    }
    return sum;
}

int is_perfect(int n) {
    return n > 1 && sum_divisors(n) == n;
}

int is_abundant(int n) {
    return sum_divisors(n) > n;
}

int is_deficient(int n) {
    return n > 0 && sum_divisors(n) < n;
}

int is_harshad(int n) {
    int digit_sum, tmp;
    if (n <= 0) return 0;
    digit_sum = 0;
    tmp = n;
    while (tmp > 0) {
        digit_sum += tmp % 10;
        tmp /= 10;
    }
    return n % digit_sum == 0;
}

int main(void) {
    int i;
    int count;

    /* Perfect numbers up to 500 */
    printf("perfect: ");
    for (i = 2; i <= 500; i++) {
        if (is_perfect(i)) {
            printf("%d ", i);
        }
    }
    printf("\n");

    /* Sum of divisors for small numbers */
    printf("sigma: ");
    for (i = 1; i <= 15; i++) {
        printf("%d ", sum_divisors(i));
    }
    printf("\n");

    /* Count abundant numbers up to 50 */
    count = 0;
    for (i = 1; i <= 50; i++) {
        if (is_abundant(i)) count++;
    }
    printf("abundant count <= 50: %d\n", count);

    /* First 10 abundant numbers */
    printf("abundant: ");
    count = 0;
    for (i = 1; count < 10; i++) {
        if (is_abundant(i)) {
            printf("%d ", i);
            count++;
        }
    }
    printf("\n");

    /* Count deficient up to 20 */
    count = 0;
    for (i = 1; i <= 20; i++) {
        if (is_deficient(i)) count++;
    }
    printf("deficient count <= 20: %d\n", count);

    /* Harshad numbers up to 50 */
    printf("harshad: ");
    for (i = 1; i <= 50; i++) {
        if (is_harshad(i)) {
            printf("%d ", i);
        }
    }
    printf("\n");

    /* Amicable numbers check: 220 and 284 */
    printf("sigma(220) = %d\n", sum_divisors(220));
    printf("sigma(284) = %d\n", sum_divisors(284));
    printf("amicable: %d\n", sum_divisors(220) == 284 && sum_divisors(284) == 220);

    /* Number classification */
    printf("classify: ");
    for (i = 1; i <= 12; i++) {
        int s;
        s = sum_divisors(i);
        if (s == i) printf("P");
        else if (s > i) printf("A");
        else printf("D");
        printf(" ");
    }
    printf("\n");

    return 0;
}
