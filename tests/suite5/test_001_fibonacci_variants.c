int printf(const char *fmt, ...);

int fib_memo(int n, int memo[]) {
    if (n <= 1) return n;
    if (memo[n] != 0) return memo[n];
    memo[n] = fib_memo(n - 1, memo) + fib_memo(n - 2, memo);
    return memo[n];
}

int tribonacci(int n) {
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 1;
    int a = 0;
    int b = 0;
    int c = 1;
    int i = 3;
    int tmp;
    while (i <= n) {
        tmp = a + b + c;
        a = b;
        b = c;
        c = tmp;
        i = i + 1;
    }
    return c;
}

int lucas(int n) {
    if (n == 0) return 2;
    if (n == 1) return 1;
    int a = 2;
    int b = 1;
    int i = 2;
    int tmp;
    while (i <= n) {
        tmp = a + b;
        a = b;
        b = tmp;
        i = i + 1;
    }
    return b;
}

int fib_iterative(int n) {
    if (n <= 1) return n;
    int a = 0;
    int b = 1;
    int i = 2;
    int tmp;
    while (i <= n) {
        tmp = a + b;
        a = b;
        b = tmp;
        i = i + 1;
    }
    return b;
}

int fib_sum(int n) {
    int sum = 0;
    int i = 0;
    while (i <= n) {
        sum = sum + fib_iterative(i);
        i = i + 1;
    }
    return sum;
}

int main(void) {
    int memo[30];
    int i = 0;
    while (i < 30) {
        memo[i] = 0;
        i = i + 1;
    }

    // EXPECT: fib(0) = 0
    printf("fib(0) = %d\n", fib_memo(0, memo));
    // EXPECT: fib(1) = 1
    printf("fib(1) = %d\n", fib_memo(1, memo));
    // EXPECT: fib(10) = 55
    printf("fib(10) = %d\n", fib_memo(10, memo));
    // EXPECT: fib(15) = 610
    printf("fib(15) = %d\n", fib_memo(15, memo));
    // EXPECT: fib(20) = 6765
    printf("fib(20) = %d\n", fib_memo(20, memo));

    // EXPECT: trib(0) = 0
    printf("trib(0) = %d\n", tribonacci(0));
    // EXPECT: trib(2) = 1
    printf("trib(2) = %d\n", tribonacci(2));
    // EXPECT: trib(5) = 4
    printf("trib(5) = %d\n", tribonacci(5));
    // EXPECT: trib(8) = 24
    printf("trib(8) = %d\n", tribonacci(8));
    // EXPECT: trib(10) = 81
    printf("trib(10) = %d\n", tribonacci(10));

    // EXPECT: lucas(0) = 2
    printf("lucas(0) = %d\n", lucas(0));
    // EXPECT: lucas(1) = 1
    printf("lucas(1) = %d\n", lucas(1));
    // EXPECT: lucas(5) = 11
    printf("lucas(5) = %d\n", lucas(5));
    // EXPECT: lucas(10) = 123
    printf("lucas(10) = %d\n", lucas(10));

    // EXPECT: fib_iter(12) = 144
    printf("fib_iter(12) = %d\n", fib_iterative(12));

    // EXPECT: fib_sum(6) = 20
    printf("fib_sum(6) = %d\n", fib_sum(6));
    // EXPECT: fib_sum(10) = 143
    printf("fib_sum(10) = %d\n", fib_sum(10));

    int fib_sq = fib_iterative(5) * fib_iterative(5);
    // EXPECT: fib(5)^2 = 25
    printf("fib(5)^2 = %d\n", fib_sq);

    int lf = lucas(6) + fib_iterative(6);
    // EXPECT: lucas(6)+fib(6) = 26
    printf("lucas(6)+fib(6) = %d\n", lf);

    return 0;
}
