int printf(const char *fmt, ...);
// EXPECT: 2 + 3*4 = 14\n5*4 - 3 + 2 = 19\n(2+3)*4 = 20\n(2+3)*(4+5) = 45\n2 + 3%4*5 = 17\n(2+3 > 4) = 1\n(2<3 && 4>5) = 0\n(2<3 || 4>5) = 1\n2>3 ? 4 : 5 = 5\n(2&3)==2 = 1\n2+3*4-5/1 = 9\n2*3+4*5 = 26\n(2+3)*(4-5) = -5\n5*8 = 40\n20/4 = 5\n20-5-3 = 12\n20/5/2 = 2
// Test: operator precedence edge cases

int main(void) {
    int a, b, c, d;
    int r1, r2, r3, r4, r5, r6, r7, r8, r9, r10;

    a = 2;
    b = 3;
    c = 4;
    d = 5;

    /* Multiplication before addition */
    r1 = a + b * c;
    printf("2 + 3*4 = %d\n", r1);

    /* Division before subtraction */
    r2 = d * c - b + a;
    printf("5*4 - 3 + 2 = %d\n", r2);

    /* Parentheses override */
    r3 = (a + b) * c;
    printf("(2+3)*4 = %d\n", r3);

    /* Nested parentheses */
    r4 = ((a + b) * (c + d));
    printf("(2+3)*(4+5) = %d\n", r4);

    /* Modulo precedence */
    r5 = a + b % c * d;
    printf("2 + 3%%4*5 = %d\n", r5);

    /* Comparison vs arithmetic */
    r6 = (a + b > c);
    printf("(2+3 > 4) = %d\n", r6);

    /* Logical vs comparison */
    r7 = (a < b && c > d);
    printf("(2<3 && 4>5) = %d\n", r7);

    r8 = (a < b || c > d);
    printf("(2<3 || 4>5) = %d\n", r8);

    /* Ternary precedence */
    r9 = a > b ? c : d;
    printf("2>3 ? 4 : 5 = %d\n", r9);

    /* Bitwise vs comparison */
    r10 = (a & b) == 2;
    printf("(2&3)==2 = %d\n", r10);

    /* Complex mixed precedence */
    printf("2+3*4-5/1 = %d\n", a + b * c - d / 1);
    printf("2*3+4*5 = %d\n", a * b + c * d);
    printf("(2+3)*(4-5) = %d\n", (a + b) * (c - d));

    /* Shift-like behavior with multiply/divide by powers of 2 */
    printf("5*8 = %d\n", d * 8);
    printf("20/4 = %d\n", 20 / c);

    /* Associativity: left-to-right for arithmetic */
    printf("20-5-3 = %d\n", 20 - d - b);
    printf("20/5/2 = %d\n", 20 / d / a);

    return 0;
}
