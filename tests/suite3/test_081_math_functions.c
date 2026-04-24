int printf(const char *fmt, ...);

/* Integer absolute value */
int abs_val(int x) {
    return x < 0 ? -x : x;
}

/* Minimum of two integers */
int min_val(int a, int b) {
    return a < b ? a : b;
}

/* Maximum of two integers */
int max_val(int a, int b) {
    return a > b ? a : b;
}

/* Clamp x to [lo, hi] */
int clamp(int x, int lo, int hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

/* Greatest common divisor (Euclidean) */
int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Least common multiple */
int lcm(int a, int b) {
    return a / gcd(a, b) * b;
}

/* Integer power: base^exp (exp >= 0) */
int power(int base, int exp) {
    int result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Integer square root via Newton's method */
int isqrt(int n) {
    if (n < 0) return -1;
    if (n == 0) return 0;
    int x = n;
    int x1 = (x + 1) / 2;
    while (x1 < x) {
        x = x1;
        x1 = (x + n / x) / 2;
    }
    return x;
}

/* Sign function: -1, 0, or 1 */
int sign(int x) {
    if (x < 0) return -1;
    if (x > 0) return 1;
    return 0;
}

/* Ceiling integer division (positive divisor) */
int ceil_div(int a, int b) {
    return (a + b - 1) / b;
}

/* Count digits in a positive integer */
int count_digits(int n) {
    if (n == 0) return 1;
    int count = 0;
    if (n < 0) { n = -n; }
    while (n > 0) {
        count++;
        n /= 10;
    }
    return count;
}

/* Sum of digits */
int sum_digits(int n) {
    if (n < 0) n = -n;
    int s = 0;
    while (n > 0) {
        s += n % 10;
        n /= 10;
    }
    return s;
}

/* Reverse digits of a number */
int reverse_digits(int n) {
    int r = 0;
    while (n > 0) {
        r = r * 10 + n % 10;
        n /= 10;
    }
    return r;
}

int main(void) {
    /* abs_val tests */
    printf("%d\n", abs_val(-7));   // EXPECT: 7
    printf("%d\n", abs_val(5));    // EXPECT: 5
    printf("%d\n", abs_val(0));    // EXPECT: 0

    /* min / max tests */
    printf("%d\n", min_val(3, 8));  // EXPECT: 3
    printf("%d\n", min_val(8, 3));  // EXPECT: 3
    printf("%d\n", max_val(3, 8));  // EXPECT: 8
    printf("%d\n", max_val(8, 3));  // EXPECT: 8

    /* clamp tests */
    printf("%d\n", clamp(5, 1, 10));   // EXPECT: 5
    printf("%d\n", clamp(-3, 1, 10));  // EXPECT: 1
    printf("%d\n", clamp(15, 1, 10));  // EXPECT: 10

    /* gcd tests */
    printf("%d\n", gcd(12, 8));    // EXPECT: 4
    printf("%d\n", gcd(100, 75));  // EXPECT: 25
    printf("%d\n", gcd(7, 13));    // EXPECT: 1

    /* lcm tests */
    printf("%d\n", lcm(4, 6));     // EXPECT: 12
    printf("%d\n", lcm(3, 7));     // EXPECT: 21

    /* power tests */
    printf("%d\n", power(2, 10));  // EXPECT: 1024
    printf("%d\n", power(3, 5));   // EXPECT: 243
    printf("%d\n", power(7, 0));   // EXPECT: 1

    /* isqrt tests */
    printf("%d\n", isqrt(0));      // EXPECT: 0
    printf("%d\n", isqrt(1));      // EXPECT: 1
    printf("%d\n", isqrt(4));      // EXPECT: 2
    printf("%d\n", isqrt(9));      // EXPECT: 3
    printf("%d\n", isqrt(16));     // EXPECT: 4
    printf("%d\n", isqrt(144));    // EXPECT: 12
    printf("%d\n", isqrt(200));    // EXPECT: 14

    /* sign tests */
    printf("%d\n", sign(-99));  // EXPECT: -1
    printf("%d\n", sign(0));    // EXPECT: 0
    printf("%d\n", sign(42));   // EXPECT: 1

    /* ceil_div tests */
    printf("%d\n", ceil_div(10, 3));  // EXPECT: 4
    printf("%d\n", ceil_div(9, 3));   // EXPECT: 3
    printf("%d\n", ceil_div(7, 2));   // EXPECT: 4

    /* count_digits tests */
    printf("%d\n", count_digits(0));      // EXPECT: 1
    printf("%d\n", count_digits(9));      // EXPECT: 1
    printf("%d\n", count_digits(100));    // EXPECT: 3
    printf("%d\n", count_digits(12345));  // EXPECT: 5

    /* sum_digits tests */
    printf("%d\n", sum_digits(123));   // EXPECT: 6
    printf("%d\n", sum_digits(9999));  // EXPECT: 36
    printf("%d\n", sum_digits(100));   // EXPECT: 1

    /* reverse_digits tests */
    printf("%d\n", reverse_digits(12345));  // EXPECT: 54321
    printf("%d\n", reverse_digits(100));    // EXPECT: 1
    printf("%d\n", reverse_digits(9));      // EXPECT: 9

    return 0;
}
