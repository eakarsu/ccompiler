int printf(const char *fmt, ...);

int power(int base, int exp) {
    if (exp == 0) return 1;
    return base * power(base, exp - 1);
}

int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int fibonacci(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int sum_of_digits(int n) {
    if (n < 0) n = -n;
    if (n < 10) return n;
    return (n % 10) + sum_of_digits(n / 10);
}

int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int hanoi_count(int n) {
    if (n == 1) return 1;
    return 2 * hanoi_count(n - 1) + 1;
}

int lcm(int a, int b) {
    return (a / gcd(a, b)) * b;
}

int recursive_multiply(int a, int b) {
    if (b == 0) return 0;
    if (b < 0) return -recursive_multiply(a, -b);
    return a + recursive_multiply(a, b - 1);
}

int recursive_divide(int a, int b) {
    if (a < b) return 0;
    return 1 + recursive_divide(a - b, b);
}

int digital_root(int n) {
    if (n < 10) return n;
    return digital_root(sum_of_digits(n));
}

int ackermann(int m, int n) {
    if (m == 0) return n + 1;
    if (n == 0) return ackermann(m - 1, 1);
    return ackermann(m - 1, ackermann(m, n - 1));
}

int sum_range(int a, int b) {
    if (a > b) return 0;
    if (a == b) return a;
    return a + sum_range(a + 1, b);
}

int count_digits(int n) {
    if (n < 0) n = -n;
    if (n < 10) return 1;
    return 1 + count_digits(n / 10);
}

int main(void) {
    // EXPECT: power(2,10)=1024
    printf("power(2,10)=%d\n", power(2, 10));
    // EXPECT: power(3,5)=243
    printf("power(3,5)=%d\n", power(3, 5));
    // EXPECT: power(5,0)=1
    printf("power(5,0)=%d\n", power(5, 0));
    // EXPECT: fact(0)=1
    printf("fact(0)=%d\n", factorial(0));
    // EXPECT: fact(5)=120
    printf("fact(5)=%d\n", factorial(5));
    // EXPECT: fact(10)=3628800
    printf("fact(10)=%d\n", factorial(10));
    // EXPECT: fib(0)=0
    printf("fib(0)=%d\n", fibonacci(0));
    // EXPECT: fib(10)=55
    printf("fib(10)=%d\n", fibonacci(10));
    // EXPECT: fib(15)=610
    printf("fib(15)=%d\n", fibonacci(15));
    // EXPECT: digits(12345)=15
    printf("digits(12345)=%d\n", sum_of_digits(12345));
    // EXPECT: digits(999)=27
    printf("digits(999)=%d\n", sum_of_digits(999));
    // EXPECT: gcd(48,18)=6
    printf("gcd(48,18)=%d\n", gcd(48, 18));
    // EXPECT: gcd(100,75)=25
    printf("gcd(100,75)=%d\n", gcd(100, 75));
    // EXPECT: hanoi(1)=1
    printf("hanoi(1)=%d\n", hanoi_count(1));
    // EXPECT: hanoi(4)=15
    printf("hanoi(4)=%d\n", hanoi_count(4));
    // EXPECT: hanoi(10)=1023
    printf("hanoi(10)=%d\n", hanoi_count(10));
    // EXPECT: lcm(12,18)=36
    printf("lcm(12,18)=%d\n", lcm(12, 18));
    // EXPECT: mul(7,6)=42
    printf("mul(7,6)=%d\n", recursive_multiply(7, 6));
    // EXPECT: mul(3,-4)=-12
    printf("mul(3,-4)=%d\n", recursive_multiply(3, -4));
    // EXPECT: div(100,7)=14
    printf("div(100,7)=%d\n", recursive_divide(100, 7));
    // EXPECT: droot(9875)=2
    printf("droot(9875)=%d\n", digital_root(9875));
    // EXPECT: droot(493)=7
    printf("droot(493)=%d\n", digital_root(493));
    // EXPECT: ack(2,3)=9
    printf("ack(2,3)=%d\n", ackermann(2, 3));
    // EXPECT: ack(3,2)=29
    printf("ack(3,2)=%d\n", ackermann(3, 2));
    // EXPECT: sum(1,100)=5050
    printf("sum(1,100)=%d\n", sum_range(1, 100));
    // EXPECT: cntdig(123456)=6
    printf("cntdig(123456)=%d\n", count_digits(123456));
    return 0;
}
