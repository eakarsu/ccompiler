int printf(const char *fmt, ...);

int tail_factorial(int n, int acc) {
    if (n <= 1) return acc;
    return tail_factorial(n - 1, n * acc);
}

int tail_gcd(int a, int b) {
    if (b == 0) return a;
    return tail_gcd(b, a % b);
}

int tail_sum(int n, int acc) {
    if (n <= 0) return acc;
    return tail_sum(n - 1, acc + n);
}

int tail_fib(int n, int a, int b) {
    if (n == 0) return a;
    if (n == 1) return b;
    return tail_fib(n - 1, b, a + b);
}

int tail_power(int base, int exp, int acc) {
    if (exp == 0) return acc;
    return tail_power(base, exp - 1, acc * base);
}

int tail_length(int arr[], int n, int acc) {
    if (n <= 0) return acc;
    return tail_length(arr, n - 1, acc + 1);
}

int tail_count_digits(int n, int acc) {
    if (n < 10) return acc + 1;
    return tail_count_digits(n / 10, acc + 1);
}

int tail_sum_digits(int n, int acc) {
    if (n == 0) return acc;
    return tail_sum_digits(n / 2, acc + (n % 2));
}

int tail_reverse_num(int n, int acc) {
    if (n == 0) return acc;
    return tail_reverse_num(n / 10, acc * 10 + n % 10);
}

int tail_multiply(int a, int b, int acc) {
    if (b == 0) return acc;
    return tail_multiply(a, b - 1, acc + a);
}

int tail_modulo(int a, int b, int acc) {
    if (a < b) return a;
    return tail_modulo(a - b, b, acc + 1);
}

int tail_div(int a, int b, int acc) {
    if (a < b) return acc;
    return tail_div(a - b, b, acc + 1);
}

int tail_max_arr(int arr[], int n, int best) {
    if (n <= 0) return best;
    if (arr[n - 1] > best) return tail_max_arr(arr, n - 1, arr[n - 1]);
    return tail_max_arr(arr, n - 1, best);
}

int tail_min_arr(int arr[], int n, int best) {
    if (n <= 0) return best;
    if (arr[n - 1] < best) return tail_min_arr(arr, n - 1, arr[n - 1]);
    return tail_min_arr(arr, n - 1, best);
}

int tail_contains(int arr[], int n, int val) {
    if (n <= 0) return 0;
    if (arr[n - 1] == val) return 1;
    return tail_contains(arr, n - 1, val);
}

int tail_sum_arr(int arr[], int n, int acc) {
    if (n <= 0) return acc;
    return tail_sum_arr(arr, n - 1, acc + arr[n - 1]);
}

int tail_tribonacci(int n, int a, int b, int c) {
    if (n == 0) return a;
    if (n == 1) return b;
    if (n == 2) return c;
    return tail_tribonacci(n - 1, b, c, a + b + c);
}

int tail_lucas(int n, int a, int b) {
    if (n == 0) return a;
    if (n == 1) return b;
    return tail_lucas(n - 1, b, a + b);
}

int main(void) {
    // EXPECT: fact(1)=1
    printf("fact(1)=%d\n", tail_factorial(1, 1));
    // EXPECT: fact(5)=120
    printf("fact(5)=%d\n", tail_factorial(5, 1));
    // EXPECT: fact(10)=3628800
    printf("fact(10)=%d\n", tail_factorial(10, 1));

    // EXPECT: gcd(48,18)=6
    printf("gcd(48,18)=%d\n", tail_gcd(48, 18));
    // EXPECT: gcd(100,75)=25
    printf("gcd(100,75)=%d\n", tail_gcd(100, 75));
    // EXPECT: gcd(17,13)=1
    printf("gcd(17,13)=%d\n", tail_gcd(17, 13));

    // EXPECT: sum(100)=5050
    printf("sum(100)=%d\n", tail_sum(100, 0));
    // EXPECT: sum(1000)=500500
    printf("sum(1000)=%d\n", tail_sum(1000, 0));

    // EXPECT: fib(0)=0
    printf("fib(0)=%d\n", tail_fib(0, 0, 1));
    // EXPECT: fib(10)=55
    printf("fib(10)=%d\n", tail_fib(10, 0, 1));
    // EXPECT: fib(20)=6765
    printf("fib(20)=%d\n", tail_fib(20, 0, 1));

    // EXPECT: pow(2,10)=1024
    printf("pow(2,10)=%d\n", tail_power(2, 10, 1));
    // EXPECT: pow(3,5)=243
    printf("pow(3,5)=%d\n", tail_power(3, 5, 1));

    // EXPECT: cntdig(12345)=5
    printf("cntdig(12345)=%d\n", tail_count_digits(12345, 0));
    // EXPECT: cntdig(1)=1
    printf("cntdig(1)=%d\n", tail_count_digits(1, 0));

    // EXPECT: bits(255)=8
    printf("bits(255)=%d\n", tail_sum_digits(255, 0));
    // EXPECT: bits(1024)=1
    printf("bits(1024)=%d\n", tail_sum_digits(1024, 0));

    // EXPECT: rev(12345)=54321
    printf("rev(12345)=%d\n", tail_reverse_num(12345, 0));
    // EXPECT: rev(1000)=1
    printf("rev(1000)=%d\n", tail_reverse_num(1000, 0));

    // EXPECT: mul(7,6)=42
    printf("mul(7,6)=%d\n", tail_multiply(7, 6, 0));
    // EXPECT: div(100,7)=14
    printf("div(100,7)=%d\n", tail_div(100, 7, 0));
    // EXPECT: mod(100,7)=2
    printf("mod(100,7)=%d\n", tail_modulo(100, 7, 0));

    int arr[8];
    arr[0]=3; arr[1]=7; arr[2]=1; arr[3]=9; arr[4]=2; arr[5]=8; arr[6]=4; arr[7]=6;
    // EXPECT: max=9
    printf("max=%d\n", tail_max_arr(arr, 8, arr[0]));
    // EXPECT: min=1
    printf("min=%d\n", tail_min_arr(arr, 8, arr[0]));
    // EXPECT: has7=1
    printf("has7=%d\n", tail_contains(arr, 8, 7));
    // EXPECT: has5=0
    printf("has5=%d\n", tail_contains(arr, 8, 5));
    // EXPECT: arrsum=40
    printf("arrsum=%d\n", tail_sum_arr(arr, 8, 0));

    // EXPECT: trib(7)=13
    printf("trib(7)=%d\n", tail_tribonacci(7, 0, 0, 1));

    // EXPECT: lucas(0)=2
    printf("lucas(0)=%d\n", tail_lucas(0, 2, 1));
    // EXPECT: lucas(5)=11
    printf("lucas(5)=%d\n", tail_lucas(5, 2, 1));

    return 0;
}
