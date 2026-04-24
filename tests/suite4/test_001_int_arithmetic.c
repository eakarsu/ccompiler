int printf(const char *fmt, ...);

// Test basic integer arithmetic: add, sub, mul, div, mod
// with positive, negative, zero, and boundary values.

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int div_int(int a, int b) { return a / b; }
int mod_int(int a, int b) { return a % b; }

int square(int x) { return x * x; }

int sum_range(int lo, int hi) {
    int total = 0;
    int i;
    for (i = lo; i <= hi; i = i + 1) {
        total = total + i;
    }
    return total;
}

int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int abs_val(int x) {
    return x < 0 ? -x : x;
}

int sign(int x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a < 0 && a < b ? a : (b < a ? b : a);
}

int gcd(int a, int b) {
    a = abs_val(a);
    b = abs_val(b);
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int power(int base, int exp) {
    int result = 1;
    int i;
    for (i = 0; i < exp; i = i + 1) {
        result = result * base;
    }
    return result;
}

int main(void) {
    // Basic addition
    // EXPECT: 3 + 4 = 7
    printf("%d + %d = %d\n", 3, 4, add(3, 4));

    // Addition with negatives
    // EXPECT: -5 + 3 = -2
    printf("%d + %d = %d\n", -5, 3, add(-5, 3));

    // Addition with zero
    // EXPECT: 0 + 0 = 0
    printf("%d + %d = %d\n", 0, 0, add(0, 0));

    // Subtraction
    // EXPECT: 10 - 7 = 3
    printf("%d - %d = %d\n", 10, 7, sub(10, 7));

    // Subtraction giving negative
    // EXPECT: 3 - 10 = -7
    printf("%d - %d = %d\n", 3, 10, sub(3, 10));

    // Multiplication
    // EXPECT: 6 * 7 = 42
    printf("%d * %d = %d\n", 6, 7, mul(6, 7));

    // Multiplication with negative
    // EXPECT: -3 * 5 = -15
    printf("%d * %d = %d\n", -3, 5, mul(-3, 5));

    // Multiplication of two negatives
    // EXPECT: -4 * -6 = 24
    printf("%d * %d = %d\n", -4, -6, mul(-4, -6));

    // Multiplication by zero
    // EXPECT: 999 * 0 = 0
    printf("%d * %d = %d\n", 999, 0, mul(999, 0));

    // Division
    // EXPECT: 20 / 4 = 5
    printf("%d / %d = %d\n", 20, 4, div_int(20, 4));

    // Division with truncation
    // EXPECT: 7 / 2 = 3
    printf("%d / %d = %d\n", 7, 2, div_int(7, 2));

    // Division negative dividend
    // EXPECT: -7 / 2 = -3
    printf("%d / %d = %d\n", -7, 2, div_int(-7, 2));

    // Modulo
    // EXPECT: 17 % 5 = 2
    printf("%d %% %d = %d\n", 17, 5, mod_int(17, 5));

    // Modulo with negative
    // EXPECT: -17 % 5 = -2
    printf("%d %% %d = %d\n", -17, 5, mod_int(-17, 5));

    // Square
    // EXPECT: square(9) = 81
    printf("square(%d) = %d\n", 9, square(9));

    // Sum of range 1..10
    // EXPECT: sum(1..10) = 55
    printf("sum(1..10) = %d\n", sum_range(1, 10));

    // Factorial
    // EXPECT: 6! = 720
    printf("%d! = %d\n", 6, factorial(6));

    // Absolute value
    // EXPECT: abs(-42) = 42
    printf("abs(%d) = %d\n", -42, abs_val(-42));

    // Sign function
    // EXPECT: sign(-100) = -1
    printf("sign(%d) = %d\n", -100, sign(-100));

    // EXPECT: sign(0) = 0
    printf("sign(%d) = %d\n", 0, sign(0));

    // EXPECT: sign(50) = 1
    printf("sign(%d) = %d\n", 50, sign(50));

    // GCD
    // EXPECT: gcd(48, 18) = 6
    printf("gcd(%d, %d) = %d\n", 48, 18, gcd(48, 18));

    // Power
    // EXPECT: 2^10 = 1024
    printf("%d^%d = %d\n", 2, 10, power(2, 10));

    // Complex expression: (3 + 4) * (10 - 2) / 2
    int result = (3 + 4) * (10 - 2) / 2;
    // EXPECT: (3+4)*(10-2)/2 = 28
    printf("(3+4)*(10-2)/2 = %d\n", result);

    // Nested calls
    // EXPECT: max(min(5,3), min(7,2)) = 3
    printf("max(min(5,3), min(7,2)) = %d\n", max(min(5, 3), min(7, 2)));

    return 0;
}
