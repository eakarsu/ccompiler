int printf(const char *fmt, ...);

int main(void) {
    /* Basic addition */
    int a = 10, b = 20;
    printf("%d\n", a + b);          // EXPECT: 30
    printf("%d\n", a + (-5));       // EXPECT: 5
    printf("%d\n", (-7) + (-3));    // EXPECT: -10

    /* Basic subtraction */
    printf("%d\n", b - a);          // EXPECT: 10
    printf("%d\n", a - b);          // EXPECT: -10
    printf("%d\n", 0 - 42);         // EXPECT: -42

    /* Basic multiplication */
    int c = 6, d = 7;
    printf("%d\n", c * d);          // EXPECT: 42
    printf("%d\n", (-3) * 4);       // EXPECT: -12
    printf("%d\n", (-3) * (-4));    // EXPECT: 12
    printf("%d\n", 0 * 9999);       // EXPECT: 0

    /* Basic division */
    printf("%d\n", 100 / 10);       // EXPECT: 10
    printf("%d\n", 99 / 10);        // EXPECT: 9
    printf("%d\n", (-99) / 10);     // EXPECT: -9
    printf("%d\n", 99 / (-10));     // EXPECT: -9
    printf("%d\n", (-99) / (-10));  // EXPECT: 9

    /* Basic modulo */
    printf("%d\n", 100 % 7);        // EXPECT: 2
    printf("%d\n", (-100) % 7);     // EXPECT: -2
    printf("%d\n", 100 % (-7));     // EXPECT: 2
    printf("%d\n", 7 % 7);          // EXPECT: 0

    /* Chained operations: a*b + c*d */
    int p = 3, q = 4, r = 5, s = 6;
    printf("%d\n", p * q + r * s);  // EXPECT: 42
    /* 3*4 = 12, 5*6 = 30, 12+30 = 42 */

    int x = 2, y = 3, z = 4;
    printf("%d\n", x * y * z);      // EXPECT: 24
    printf("%d\n", x + y * z);      // EXPECT: 14
    /* 2 + 3*4 = 2 + 12 = 14 */
    printf("%d\n", (x + y) * z);    // EXPECT: 20
    /* (2+3)*4 = 5*4 = 20 */

    /* Negative chained */
    int m = -2, n = 5;
    printf("%d\n", m * n + 3);      // EXPECT: -7
    /* -2*5+3 = -10+3 = -7 */
    printf("%d\n", m * m + n * n);  // EXPECT: 29
    /* 4 + 25 = 29 */

    /* Division and modulo chain */
    printf("%d\n", 100 / 3 * 3);    // EXPECT: 99
    /* 100/3 = 33, 33*3 = 99 */
    printf("%d\n", 100 % 3 + 100 / 3); // EXPECT: 34
    /* 1 + 33 = 34 */

    /* Large values */
    int big = 1000000;
    printf("%d\n", big * 2000);     // EXPECT: 2000000000
    printf("%d\n", big + 999999);   // EXPECT: 1999999

    /* Edge case: INT_MIN = -2147483648 */
    int imin = -2147483648;
    printf("%d\n", imin + 1);       // EXPECT: -2147483647
    printf("%d\n", imin + 2147483647); // EXPECT: -1
    /* -2147483648 + 2147483647 = -1 */

    /* INT_MAX = 2147483647 */
    int imax = 2147483647;
    printf("%d\n", imax - 2147483646); // EXPECT: 1
    printf("%d\n", imax / 2);          // EXPECT: 1073741823
    printf("%d\n", imax % 2);          // EXPECT: 1

    /* Subtraction resulting in INT boundary */
    printf("%d\n", imax - imax);       // EXPECT: 0
    printf("%d\n", imin - imin);       // EXPECT: 0

    /* Mixed positive/negative chains */
    printf("%d\n", 10 - 3 + 2 - 8);   // EXPECT: 1
    /* 10-3=7, 7+2=9, 9-8=1 */
    printf("%d\n", -1 - (-1));         // EXPECT: 0
    printf("%d\n", 5 * 3 - 2 * 7 + 1); // EXPECT: 2
    /* 15 - 14 + 1 = 2 */

    /* Division truncation toward zero */
    printf("%d\n", 7 / 2);             // EXPECT: 3
    printf("%d\n", (-7) / 2);          // EXPECT: -3
    printf("%d\n", 1 / 3);             // EXPECT: 0
    printf("%d\n", (-1) / 3);          // EXPECT: 0

    /* Power-of-two arithmetic */
    int pw = 1;
    pw = pw * 2;
    pw = pw * 2;
    pw = pw * 2;
    pw = pw * 2;
    printf("%d\n", pw);                // EXPECT: 16

    /* Accumulate sum 1..10 */
    int sum = 0;
    sum = sum + 1;
    sum = sum + 2;
    sum = sum + 3;
    sum = sum + 4;
    sum = sum + 5;
    sum = sum + 6;
    sum = sum + 7;
    sum = sum + 8;
    sum = sum + 9;
    sum = sum + 10;
    printf("%d\n", sum);               // EXPECT: 55

    /* Nested expressions */
    printf("%d\n", ((4 + 6) * (3 - 1)) / (2 + 3)); // EXPECT: 4
    /* (10 * 2) / 5 = 20 / 5 = 4 */

    printf("%d\n", 1000 / (10 * 10));  // EXPECT: 10

    return 0;
}
