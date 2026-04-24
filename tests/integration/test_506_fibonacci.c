int printf(const char *fmt, ...);
// EXPECT: Fibonacci sequence (first 20):\n0 1 1 2 3 5 8 13 21 34 55 89 144 233 377 610 987 1597 2584 4181 \nSum of first 20: 10945\nLucas sequence (first 15):\n2 1 3 4 7 11 18 29 47 76 123 199 322 521 843 \nFib ratios (x1000):\nF(5)/F(4) = 1666/1000\nF(6)/F(5) = 1600/1000\nF(7)/F(6) = 1625/1000\nF(8)/F(7) = 1615/1000\nF(9)/F(8) = 1619/1000\nF(10)/F(9) = 1617/1000\nF(11)/F(10) = 1618/1000\nF(12)/F(11) = 1617/1000\nF(13)/F(12) = 1618/1000\nF(14)/F(13) = 1618/1000\nF(15)/F(14) = 1618/1000\nCassini identity check:\nn=2: F(n)^2 - F(n+1)*F(n-1) = -1\nn=3: F(n)^2 - F(n+1)*F(n-1) = 1\nn=4: F(n)^2 - F(n+1)*F(n-1) = -1\nn=5: F(n)^2 - F(n+1)*F(n-1) = 1\nn=6: F(n)^2 - F(n+1)*F(n-1) = -1\nn=7: F(n)^2 - F(n+1)*F(n-1) = 1\nn=8: F(n)^2 - F(n+1)*F(n-1) = -1\nn=9: F(n)^2 - F(n+1)*F(n-1) = 1\nn=10: F(n)^2 - F(n+1)*F(n-1) = -1\nn=11: F(n)^2 - F(n+1)*F(n-1) = 1\nn=12: F(n)^2 - F(n+1)*F(n-1) = -1\nn=13: F(n)^2 - F(n+1)*F(n-1) = 1\nn=14: F(n)^2 - F(n+1)*F(n-1) = -1\nEven Fibonacci (first 20): 0 2 8 34 144 610 2584 
// Test: Fibonacci number variants

int main(void) {
    int fib[30];
    int i;

    // Standard Fibonacci
    fib[0] = 0;
    fib[1] = 1;
    for (i = 2; i < 30; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    printf("Fibonacci sequence (first 20):\n");
    for (i = 0; i < 20; i++) {
        printf("%d ", fib[i]);
    }
    printf("\n");

    // Sum of first N Fibonacci numbers
    int sum = 0;
    for (i = 0; i < 20; i++) {
        sum = sum + fib[i];
    }
    printf("Sum of first 20: %d\n", sum);

    // Lucas numbers: L(0)=2, L(1)=1, L(n)=L(n-1)+L(n-2)
    int lucas[20];
    lucas[0] = 2;
    lucas[1] = 1;
    for (i = 2; i < 20; i++) {
        lucas[i] = lucas[i - 1] + lucas[i - 2];
    }
    printf("Lucas sequence (first 15):\n");
    for (i = 0; i < 15; i++) {
        printf("%d ", lucas[i]);
    }
    printf("\n");

    // Ratio of consecutive Fibonacci (approximate golden ratio)
    // Using integer arithmetic: fib[n]*1000/fib[n-1]
    printf("Fib ratios (x1000):\n");
    for (i = 5; i <= 15; i++) {
        int ratio = fib[i] * 1000 / fib[i - 1];
        printf("F(%d)/F(%d) = %d/1000\n", i, i - 1, ratio);
    }

    // Fibonacci identity: F(n)^2 - F(n+1)*F(n-1) = (-1)^(n+1)
    printf("Cassini identity check:\n");
    for (i = 2; i < 15; i++) {
        int val = fib[i] * fib[i] - fib[i + 1] * fib[i - 1];
        printf("n=%d: F(n)^2 - F(n+1)*F(n-1) = %d\n", i, val);
    }

    // Check which Fibonacci numbers are even
    printf("Even Fibonacci (first 20): ");
    for (i = 0; i < 20; i++) {
        if (fib[i] % 2 == 0) {
            printf("%d ", fib[i]);
        }
    }
    printf("\n");

    return 0;
}
