int printf(const char *fmt, ...);
// EXPECT: fib: 0 1 1 2 3 5 8 13 21 34 55 89 144 \nmatch: 1 1 1 1 1 1 1 1 1 1 1 \nfib sum(0..10) = 143\ntrib: 0 0 1 1 2 4 7 13 24 44 81 \nparity: 0110110110110\nratios: 200 150 166 160 162 161 
// Test: fibonacci variants

int fib_iterative(int n) {
    int a, b, temp, i;
    if (n <= 0) return 0;
    if (n == 1) return 1;
    a = 0;
    b = 1;
    for (i = 2; i <= n; i++) {
        temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

int fib_recursive(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fib_recursive(n - 1) + fib_recursive(n - 2);
}

int tribonacci(int n) {
    int a, b, c, temp, i;
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 1;
    a = 0; b = 0; c = 1;
    for (i = 3; i <= n; i++) {
        temp = a + b + c;
        a = b;
        b = c;
        c = temp;
    }
    return c;
}

int main(void) {
    int i;
    int sum;

    /* Standard fibonacci */
    printf("fib: ");
    for (i = 0; i <= 12; i++) {
        printf("%d ", fib_iterative(i));
    }
    printf("\n");

    /* Verify iterative matches recursive */
    printf("match: ");
    for (i = 0; i <= 10; i++) {
        if (fib_iterative(i) == fib_recursive(i)) {
            printf("1 ");
        } else {
            printf("0 ");
        }
    }
    printf("\n");

    /* Sum of first N fibonacci numbers */
    sum = 0;
    for (i = 0; i <= 10; i++) {
        sum += fib_iterative(i);
    }
    printf("fib sum(0..10) = %d\n", sum);

    /* Tribonacci sequence */
    printf("trib: ");
    for (i = 0; i <= 10; i++) {
        printf("%d ", tribonacci(i));
    }
    printf("\n");

    /* Check if fib(n) is even or odd */
    printf("parity: ");
    for (i = 0; i <= 12; i++) {
        printf("%d", fib_iterative(i) % 2);
    }
    printf("\n");

    /* Fibonacci ratio approximation (integer) */
    printf("ratios: ");
    for (i = 3; i <= 8; i++) {
        printf("%d ", (fib_iterative(i) * 100) / fib_iterative(i - 1));
    }
    printf("\n");

    return 0;
}
