int printf(const char *fmt, ...);
// EXPECT: === Basic Function Tests ===
// EXPECT: add: 3+5=8
// EXPECT: sub: 10-4=6
// EXPECT: mul: 7*8=56
// EXPECT: div: 100/3=33
// EXPECT: max2: max(3,7)=7
// EXPECT: min2: min(3,7)=3
// EXPECT: abs_pos: abs(5)=5
// EXPECT: abs_neg: abs(-5)=5
// EXPECT: abs_zero: abs(0)=0
// EXPECT: clamp_low: clamp(-5,0,10)=0
// EXPECT: clamp_high: clamp(15,0,10)=10
// EXPECT: clamp_mid: clamp(5,0,10)=5
// EXPECT: multi_param: f(1,2,3,4,5)=15
// EXPECT: nested_call: add(mul(3,4),5)=17
// EXPECT: void_func: hello from void
// EXPECT: void_modify: x=42
// EXPECT: factorial_0: 1
// EXPECT: factorial_1: 1
// EXPECT: factorial_5: 120
// EXPECT: factorial_10: 3628800
// EXPECT: fib_0: 0
// EXPECT: fib_1: 1
// EXPECT: fib_5: 5
// EXPECT: fib_10: 55
// EXPECT: fib_15: 610
// EXPECT: pow_2_0: 1
// EXPECT: pow_2_10: 1024
// EXPECT: pow_3_5: 243
// EXPECT: gcd_12_8: 4
// EXPECT: gcd_100_75: 25
// EXPECT: gcd_17_13: 1
// EXPECT: is_even_4: yes
// EXPECT: is_even_7: no
// EXPECT: sum_to_n_10: 55
// EXPECT: sum_to_n_100: 5050
// EXPECT: forward_decl: 42
// EXPECT: chain: g(f(2))=10
// EXPECT: === All Basic Function Tests Passed ===

// Forward declaration
int forward_func(int x);

int add(int a, int b) {
    return a + b;
}

int sub(int a, int b) {
    return a - b;
}

int mul(int a, int b) {
    return a * b;
}

int my_div(int a, int b) {
    return a / b;
}

int max2(int a, int b) {
    if (a > b) return a;
    return b;
}

int min2(int a, int b) {
    if (a < b) return a;
    return b;
}

int my_abs(int x) {
    if (x < 0) return -x;
    return x;
}

int clamp(int x, int lo, int hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

int sum5(int a, int b, int c, int d, int e) {
    return a + b + c + d + e;
}

void print_hello(void) {
    printf("void_func: hello from void\n");
}

void set_value(int *p, int v) {
    *p = v;
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

int my_pow(int base, int exp) {
    if (exp == 0) return 1;
    return base * my_pow(base, exp - 1);
}

int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int is_even(int n) {
    if (n < 0) n = -n;
    if (n == 0) return 1;
    if (n == 1) return 0;
    return is_even(n - 2);
}

int sum_to_n(int n) {
    if (n <= 0) return 0;
    return n + sum_to_n(n - 1);
}

// Definition of forward-declared function
int forward_func(int x) {
    return x * 2 + 10;
}

int f_chain(int x) {
    return x * 3;
}

int g_chain(int x) {
    return x + 4;
}

int main() {
    printf("=== Basic Function Tests ===\n");

    // basic arithmetic functions
    printf("add: 3+5=%d\n", add(3, 5));
    printf("sub: 10-4=%d\n", sub(10, 4));
    printf("mul: 7*8=%d\n", mul(7, 8));
    printf("div: 100/3=%d\n", my_div(100, 3));

    // max and min
    printf("max2: max(3,7)=%d\n", max2(3, 7));
    printf("min2: min(3,7)=%d\n", min2(3, 7));

    // absolute value
    printf("abs_pos: abs(5)=%d\n", my_abs(5));
    printf("abs_neg: abs(-5)=%d\n", my_abs(-5));
    printf("abs_zero: abs(0)=%d\n", my_abs(0));

    // clamp
    printf("clamp_low: clamp(-5,0,10)=%d\n", clamp(-5, 0, 10));
    printf("clamp_high: clamp(15,0,10)=%d\n", clamp(15, 0, 10));
    printf("clamp_mid: clamp(5,0,10)=%d\n", clamp(5, 0, 10));

    // multiple parameters
    printf("multi_param: f(1,2,3,4,5)=%d\n", sum5(1, 2, 3, 4, 5));

    // nested calls
    printf("nested_call: add(mul(3,4),5)=%d\n", add(mul(3, 4), 5));

    // void function
    print_hello();

    // void function that modifies via pointer
    int x = 0;
    set_value(&x, 42);
    printf("void_modify: x=%d\n", x);

    // factorial
    printf("factorial_0: %d\n", factorial(0));
    printf("factorial_1: %d\n", factorial(1));
    printf("factorial_5: %d\n", factorial(5));
    printf("factorial_10: %d\n", factorial(10));

    // fibonacci
    printf("fib_0: %d\n", fibonacci(0));
    printf("fib_1: %d\n", fibonacci(1));
    printf("fib_5: %d\n", fibonacci(5));
    printf("fib_10: %d\n", fibonacci(10));
    printf("fib_15: %d\n", fibonacci(15));

    // power
    printf("pow_2_0: %d\n", my_pow(2, 0));
    printf("pow_2_10: %d\n", my_pow(2, 10));
    printf("pow_3_5: %d\n", my_pow(3, 5));

    // GCD
    printf("gcd_12_8: %d\n", gcd(12, 8));
    printf("gcd_100_75: %d\n", gcd(100, 75));
    printf("gcd_17_13: %d\n", gcd(17, 13));

    // is_even via recursion
    printf("is_even_4: %s\n", is_even(4) ? "yes" : "no");
    printf("is_even_7: %s\n", is_even(7) ? "yes" : "no");

    // sum to n
    printf("sum_to_n_10: %d\n", sum_to_n(10));
    printf("sum_to_n_100: %d\n", sum_to_n(100));

    // forward declaration
    printf("forward_decl: %d\n", forward_func(16));

    // function chaining
    printf("chain: g(f(2))=%d\n", g_chain(f_chain(2)));

    printf("=== All Basic Function Tests Passed ===\n");
    return 0;
}
