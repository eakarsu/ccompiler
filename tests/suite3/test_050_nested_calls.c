int printf(const char *fmt, ...);

/* Building blocks */
int add(int a, int b)   { return a + b; }
int sub(int a, int b)   { return a - b; }
int mul(int a, int b)   { return a * b; }
int div2(int a, int b)  { return a / b; }
int mod(int a, int b)   { return a % b; }

int square(int x)       { return x * x; }
int cube(int x)         { return x * x * x; }
int double_it(int x)    { return x * 2; }
int triple(int x)       { return x * 3; }
int increment(int x)    { return x + 1; }
int decrement(int x)    { return x - 1; }
int negate(int x)       { return -x; }
int abs_val(int x)      { return x < 0 ? -x : x; }

int min2(int a, int b)  { return a < b ? a : b; }
int max2(int a, int b)  { return a > b ? a : b; }
int clamp(int v, int lo, int hi) { return min2(max2(v, lo), hi); }

/* Functions that produce results to be used as args */
int sum3(int a, int b, int c) { return a + b + c; }
int product3(int a, int b, int c) { return a * b * c; }
int avg2(int a, int b) { return (a + b) / 2; }

/* Deeper nesting helpers */
int f(int x) { return x + 1; }
int g(int x) { return x * 2; }
int h(int x) { return x - 3; }
int p(int x) { return x * x; }
int q(int x) { return x + 10; }

/* Multi-arg with nested calls */
int weighted_sum(int a, int wa, int b, int wb) {
    return add(mul(a, wa), mul(b, wb));
}

/* Functions returning structs used as context */
int poly2(int a, int b, int c, int x) {
    /* a*x^2 + b*x + c */
    return add(add(mul(a, square(x)), mul(b, x)), c);
}

int poly3(int a, int b, int c, int d, int x) {
    /* a*x^3 + b*x^2 + c*x + d */
    return add(add(add(mul(a, cube(x)), mul(b, square(x))), mul(c, x)), d);
}

/* Horner's method: a*x^2 + b*x + c = ((a)*x + b)*x + c */
int horner2(int a, int b, int c, int x) {
    return add(mul(add(mul(a, x), b), x), c);
}

/* GCD via repeated mod */
int gcd(int a, int b) {
    while (b) { int t = mod(a, b); a = b; b = t; }
    return a;
}

int lcm(int a, int b) {
    return mul(div2(a, gcd(a, b)), b);
}

int main(void) {
    /* f(g(h(x))) single nesting */
    /* h(5) = 5-3 = 2, g(2) = 4, f(4) = 5 */
    printf("%d\n", f(g(h(5))));             // EXPECT: 5

    /* h(g(f(x))) */
    /* f(3) = 4, g(4) = 8, h(8) = 5 */
    printf("%d\n", h(g(f(3))));             // EXPECT: 5

    /* g(g(g(x))) */
    /* g(1) = 2, g(2) = 4, g(4) = 8 */
    printf("%d\n", g(g(g(1))));             // EXPECT: 8

    /* p(q(x)) */
    /* q(3) = 13, p(13) = 169 */
    printf("%d\n", p(q(3)));                // EXPECT: 169

    /* q(p(x)) */
    /* p(3) = 9, q(9) = 19 */
    printf("%d\n", q(p(3)));                // EXPECT: 19

    /* 4-level nesting */
    /* h(3)=0, g(0)=0, f(0)=1, p(1)=1 */
    printf("%d\n", p(f(g(h(3)))));          // EXPECT: 1

    /* 5-level nesting */
    /* h(7)=4, g(4)=8, f(8)=9, p(9)=81, q(81)=91 */
    printf("%d\n", q(p(f(g(h(7))))));       // EXPECT: 91

    /* Function result as argument */
    printf("%d\n", add(square(3), square(4)));    // EXPECT: 25
    printf("%d\n", add(cube(2), cube(3)));         // EXPECT: 35
    /* 8 + 27 = 35 */
    printf("%d\n", mul(add(2, 3), sub(10, 4)));   // EXPECT: 30
    /* 5 * 6 = 30 */
    printf("%d\n", sub(mul(5, 4), mul(3, 2)));    // EXPECT: 14
    /* 20 - 6 = 14 */

    /* Multiple function calls in one expression */
    printf("%d\n", add(add(1, 2), add(3, 4)));    // EXPECT: 10
    printf("%d\n", mul(mul(2, 3), mul(4, 5)));     // EXPECT: 120
    printf("%d\n", max2(min2(8, 5), min2(3, 6)));  // EXPECT: 5
    /* min2(8,5)=5, min2(3,6)=3, max2(5,3)=5 */

    /* sum3 with nested args */
    printf("%d\n", sum3(square(2), square(3), square(4)));  // EXPECT: 29
    /* 4 + 9 + 16 = 29 */
    printf("%d\n", product3(add(1,1), add(2,1), add(3,1))); // EXPECT: 24
    /* 2 * 3 * 4 = 24 */

    /* avg2 nested */
    printf("%d\n", avg2(double_it(5), triple(4)));  // EXPECT: 11
    /* double_it(5)=10, triple(4)=12, avg2(10,12)=11 */

    /* clamp with nested */
    printf("%d\n", clamp(square(5), 0, 50));         // EXPECT: 25
    printf("%d\n", clamp(negate(5), -3, 10));         // EXPECT: -3
    printf("%d\n", clamp(add(3, 4), 0, 100));         // EXPECT: 7

    /* weighted_sum */
    printf("%d\n", weighted_sum(3, 2, 4, 3));   // EXPECT: 18
    /* 3*2 + 4*3 = 6+12 = 18 */
    printf("%d\n", weighted_sum(square(2), 3, cube(2), 1)); // EXPECT: 20
    /* square(2)=4, 4*3=12; cube(2)=8, 8*1=8; 12+8=20 */

    /* Polynomial evaluation */
    printf("%d\n", poly2(1, 2, 3, 4));  // EXPECT: 27
    /* 1*16 + 2*4 + 3 = 16 + 8 + 3 = 27 */
    printf("%d\n", poly2(2, -3, 1, 5)); // EXPECT: 36
    /* 2*25 + (-3)*5 + 1 = 50 - 15 + 1 = 36 */
    printf("%d\n", poly3(1, 0, 0, 0, 3)); // EXPECT: 27
    /* 1*27 + 0 + 0 + 0 = 27 */
    printf("%d\n", poly3(1, -2, 1, 0, 3)); // EXPECT: 12
    /* poly3(1,-2,1,0,3): a=1,b=-2,c=1,d=0,x=3 */
    /* 1*27 + (-2)*9 + 1*3 + 0 = 27 - 18 + 3 + 0 = 12 */

    /* Horner method matches direct */
    printf("%d\n", horner2(1, 2, 3, 4));  // EXPECT: 27
    printf("%d\n", horner2(2, -3, 1, 5)); // EXPECT: 36

    /* gcd/lcm with nested calls */
    printf("%d\n", gcd(add(6, 6), mul(3, 4)));  // EXPECT: 12
    /* gcd(12, 12) = 12 */
    printf("%d\n", gcd(mul(2, 6), mul(3, 4)));   // EXPECT: 12
    /* gcd(12, 12) = 12 */
    printf("%d\n", lcm(add(2, 2), add(3, 3)));   // EXPECT: 12
    /* lcm(4, 6) = 12 */

    /* decrement/increment chains */
    printf("%d\n", increment(increment(increment(0)))); // EXPECT: 3
    printf("%d\n", decrement(decrement(decrement(10)))); // EXPECT: 7

    /* abs_val of nested */
    printf("%d\n", abs_val(sub(3, 10)));   // EXPECT: 7
    printf("%d\n", abs_val(negate(42)));   // EXPECT: 42

    /* max2 of three values via nesting */
    printf("%d\n", max2(max2(3, 7), 5));   // EXPECT: 7
    printf("%d\n", max2(max2(9, 2), 6));   // EXPECT: 9
    printf("%d\n", min2(min2(4, 8), 2));   // EXPECT: 2

    return 0;
}
