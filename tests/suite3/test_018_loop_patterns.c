int printf(const char *fmt, ...);

/* test_018_loop_patterns.c
   Tests: Sum 1..N, factorial, Fibonacci, digit counting, reverse number,
   GCD computation, power computation using loops.
*/

int sum_1_to_n(int n) {
    int s = 0;
    for (int i = 1; i <= n; i++) s += i;
    return s;
}
/* sum_1_to_n(10) = 55 */
/* sum_1_to_n(100) = 5050 */

int factorial(int n) {
    int f = 1;
    for (int i = 2; i <= n; i++) f *= i;
    return f;
}
/* factorial(0) = 1, factorial(1) = 1, factorial(5) = 120, factorial(10) = 3628800 */

int fibonacci(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    int a = 0, b = 1;
    for (int i = 2; i <= n; i++) {
        int tmp = a + b;
        a = b;
        b = tmp;
    }
    return b;
}
/* fib(0)=0, fib(1)=1, fib(2)=1, fib(3)=2, fib(7)=13, fib(10)=55 */

int count_digits(int n) {
    if (n == 0) return 1;
    if (n < 0) n = -n;
    int count = 0;
    while (n > 0) { count++; n /= 10; }
    return count;
}

int reverse_number(int n) {
    int rev = 0;
    while (n > 0) {
        rev = rev * 10 + n % 10;
        n /= 10;
    }
    return rev;
}
/* reverse_number(1234) = 4321 */
/* reverse_number(100) = 1 */

int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}
/* gcd(48, 18) = 6 */
/* gcd(100, 75) = 25 */
/* gcd(7, 13) = 1 */

int power(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) result *= base;
    return result;
}
/* power(2, 10) = 1024 */
/* power(3, 5) = 243 */

/* Sum of squares 1..N */
int sum_squares(int n) {
    int s = 0;
    for (int i = 1; i <= n; i++) s += i * i;
    return s;
}
/* sum_squares(5) = 1+4+9+16+25 = 55 */
/* sum_squares(10) = 385 */

/* Count digits equal to d in number n */
int count_digit_d(int n, int d) {
    int count = 0;
    if (n == 0) return d == 0 ? 1 : 0;
    if (n < 0) n = -n;
    while (n > 0) {
        if (n % 10 == d) count++;
        n /= 10;
    }
    return count;
}
/* count_digit_d(112233, 1) = 2 */
/* count_digit_d(112233, 2) = 2 */
/* count_digit_d(112233, 3) = 2 */

/* LCM using GCD */
int lcm(int a, int b) {
    return a / gcd(a, b) * b;
}
/* lcm(4, 6) = 12 */
/* lcm(7, 5) = 35 */

/* Integer square root (floor) */
int isqrt(int n) {
    int r = 0;
    while ((r + 1) * (r + 1) <= n) r++;
    return r;
}
/* isqrt(16) = 4 */
/* isqrt(17) = 4 */
/* isqrt(25) = 5 */

/* Sum of all divisors of n */
int sum_divisors(int n) {
    int s = 0;
    for (int i = 1; i <= n; i++) {
        if (n % i == 0) s += i;
    }
    return s;
}
/* sum_divisors(12) = 1+2+3+4+6+12 = 28 */
/* sum_divisors(6) = 1+2+3+6 = 12 */

/* Number of perfect squares <= n */
int count_squares(int n) {
    int count = 0;
    for (int i = 1; i * i <= n; i++) count++;
    return count;
}
/* count_squares(25) = 5 (1,4,9,16,25) */
/* count_squares(10) = 3 (1,4,9) */

int main(void) {
    printf("%d\n", sum_1_to_n(10));  // EXPECT: 55
    printf("%d\n", sum_1_to_n(100)); // EXPECT: 5050

    printf("%d\n", factorial(0));  // EXPECT: 1
    printf("%d\n", factorial(1));  // EXPECT: 1
    printf("%d\n", factorial(5));  // EXPECT: 120
    printf("%d\n", factorial(10)); // EXPECT: 3628800

    printf("%d\n", fibonacci(0));  // EXPECT: 0
    printf("%d\n", fibonacci(1));  // EXPECT: 1
    printf("%d\n", fibonacci(2));  // EXPECT: 1
    printf("%d\n", fibonacci(3));  // EXPECT: 2
    printf("%d\n", fibonacci(7));  // EXPECT: 13
    printf("%d\n", fibonacci(10)); // EXPECT: 55

    printf("%d\n", count_digits(0));      // EXPECT: 1
    printf("%d\n", count_digits(7));      // EXPECT: 1
    printf("%d\n", count_digits(42));     // EXPECT: 2
    printf("%d\n", count_digits(999));    // EXPECT: 3
    printf("%d\n", count_digits(10000));  // EXPECT: 5

    printf("%d\n", reverse_number(1234)); // EXPECT: 4321
    printf("%d\n", reverse_number(100));  // EXPECT: 1
    printf("%d\n", reverse_number(9));    // EXPECT: 9

    printf("%d\n", gcd(48, 18));  // EXPECT: 6
    printf("%d\n", gcd(100, 75)); // EXPECT: 25
    printf("%d\n", gcd(7, 13));   // EXPECT: 1

    printf("%d\n", power(2, 10)); // EXPECT: 1024
    printf("%d\n", power(3, 5));  // EXPECT: 243
    printf("%d\n", power(5, 0));  // EXPECT: 1

    printf("%d\n", sum_squares(5));  // EXPECT: 55
    printf("%d\n", sum_squares(10)); // EXPECT: 385

    printf("%d\n", count_digit_d(112233, 1)); // EXPECT: 2
    printf("%d\n", count_digit_d(112233, 3)); // EXPECT: 2

    printf("%d\n", lcm(4, 6)); // EXPECT: 12
    printf("%d\n", lcm(7, 5)); // EXPECT: 35

    printf("%d\n", isqrt(16)); // EXPECT: 4
    printf("%d\n", isqrt(17)); // EXPECT: 4
    printf("%d\n", isqrt(25)); // EXPECT: 5

    printf("%d\n", sum_divisors(12)); // EXPECT: 28
    printf("%d\n", sum_divisors(6));  // EXPECT: 12

    printf("%d\n", count_squares(25)); // EXPECT: 5
    printf("%d\n", count_squares(10)); // EXPECT: 3

    return 0;
}
