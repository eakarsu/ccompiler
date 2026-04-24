int printf(const char *fmt, ...);
// EXPECT: fact(0)=1\nfact(1)=1\nfact(5)=120\nfact(7)=5040\nfact(10)=3628800\nfib(0)=0\nfib(1)=1\nfib(5)=5\nfib(10)=55\nfact_acc(5)=120\nfact_acc(7)=5040\nfib_fast(10)=55\nfib_fast(15)=610\nfib_fast(20)=6765\ndfact(7)=105\ndfact(8)=384\ncatalan(0)=1\ncatalan(1)=1\ncatalan(4)=14\ncatalan(5)=42
// Test 202: Recursive factorial and fibonacci

int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int fibonacci(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int factorial_acc(int n, int acc) {
    if (n <= 1) return acc;
    return factorial_acc(n - 1, n * acc);
}

int fib_iter_helper(int n, int a, int b) {
    if (n <= 0) return a;
    return fib_iter_helper(n - 1, b, a + b);
}

int fib_fast(int n) {
    return fib_iter_helper(n, 0, 1);
}

int double_factorial(int n) {
    if (n <= 1) return 1;
    return n * double_factorial(n - 2);
}

int catalan(int n) {
    if (n <= 1) return 1;
    int result = 0;
    int i = 0;
    while (i < n) {
        result = result + catalan(i) * catalan(n - 1 - i);
        i = i + 1;
    }
    return result;
}

int main(void) {
    printf("fact(0)=%d\n", factorial(0));
    printf("fact(1)=%d\n", factorial(1));
    printf("fact(5)=%d\n", factorial(5));
    printf("fact(7)=%d\n", factorial(7));
    printf("fact(10)=%d\n", factorial(10));

    printf("fib(0)=%d\n", fibonacci(0));
    printf("fib(1)=%d\n", fibonacci(1));
    printf("fib(5)=%d\n", fibonacci(5));
    printf("fib(10)=%d\n", fibonacci(10));

    printf("fact_acc(5)=%d\n", factorial_acc(5, 1));
    printf("fact_acc(7)=%d\n", factorial_acc(7, 1));

    printf("fib_fast(10)=%d\n", fib_fast(10));
    printf("fib_fast(15)=%d\n", fib_fast(15));
    printf("fib_fast(20)=%d\n", fib_fast(20));

    printf("dfact(7)=%d\n", double_factorial(7));
    printf("dfact(8)=%d\n", double_factorial(8));

    printf("catalan(0)=%d\n", catalan(0));
    printf("catalan(1)=%d\n", catalan(1));
    printf("catalan(4)=%d\n", catalan(4));
    printf("catalan(5)=%d\n", catalan(5));

    return 0;
}
