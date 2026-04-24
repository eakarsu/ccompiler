int printf(const char *fmt, ...);
// EXPECT: max_int = 2147483647\nmin_int = -2147483648\nmax + 1 = -2147483648\nmin - 1 = 2147483647\n100000 * 100000 = 1410065408\nmax - 5 = 2147483642\nmax - 2 = 2147483645\nmax + 1 (via add) = -2147483648\nmin + 5 = -2147483643\nmin - 5 (via sub) = 2147483643\n46341^2 = -2147479015\nmax/2 = 1073741823\nmax/2 + max/2 = 2147483646\nnegation test: -2147483647\ndouble neg: 42
// Test: overflow and wraparound with large ints

int main(void) {
    int max_int;
    int min_int;
    int a, b, c, d, e;
    int overflow_add;
    int overflow_mul;
    int underflow;
    int wrap1, wrap2, wrap3;

    max_int = 2147483647;
    min_int = -2147483647 - 1;

    /* Print max and min */
    printf("max_int = %d\n", max_int);
    printf("min_int = %d\n", min_int);

    /* Overflow addition: max + 1 wraps to min */
    overflow_add = max_int + 1;
    printf("max + 1 = %d\n", overflow_add);

    /* Underflow: min - 1 wraps to max */
    underflow = min_int - 1;
    printf("min - 1 = %d\n", underflow);

    /* Large multiplications */
    a = 100000;
    b = 100000;
    overflow_mul = a * b;
    printf("100000 * 100000 = %d\n", overflow_mul);

    /* Successive additions near boundary */
    c = max_int - 5;
    printf("max - 5 = %d\n", c);
    c = c + 3;
    printf("max - 2 = %d\n", c);
    c = c + 3;
    printf("max + 1 (via add) = %d\n", c);

    /* Negative overflow */
    d = min_int + 5;
    printf("min + 5 = %d\n", d);
    d = d - 10;
    printf("min - 5 (via sub) = %d\n", d);

    /* Multiplication overflow patterns */
    e = 46341;
    wrap1 = e * e;
    printf("46341^2 = %d\n", wrap1);

    wrap2 = max_int / 2;
    printf("max/2 = %d\n", wrap2);

    wrap3 = max_int / 2 + max_int / 2;
    printf("max/2 + max/2 = %d\n", wrap3);

    /* Negation of min_int */
    printf("negation test: %d\n", -max_int);

    /* Double negation */
    printf("double neg: %d\n", -(-42));

    return 0;
}
