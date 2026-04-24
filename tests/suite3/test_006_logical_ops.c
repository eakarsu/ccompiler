int printf(const char *fmt, ...);

/* Helper: side effect functions to test short-circuit evaluation */
int side_a = 0;
int side_b = 0;

int set_a(int val) {
    side_a = val;
    return val;
}

int set_b(int val) {
    side_b = val;
    return val;
}

int main(void) {
    /* Basic logical AND */
    printf("%d\n", 1 && 1);         // EXPECT: 1
    printf("%d\n", 1 && 0);         // EXPECT: 0
    printf("%d\n", 0 && 1);         // EXPECT: 0
    printf("%d\n", 0 && 0);         // EXPECT: 0

    /* Basic logical OR */
    printf("%d\n", 1 || 0);         // EXPECT: 1
    printf("%d\n", 0 || 1);         // EXPECT: 1
    printf("%d\n", 0 || 0);         // EXPECT: 0
    printf("%d\n", 1 || 1);         // EXPECT: 1

    /* Logical NOT */
    printf("%d\n", !0);             // EXPECT: 1
    printf("%d\n", !1);             // EXPECT: 0
    printf("%d\n", !42);            // EXPECT: 0
    printf("%d\n", !(-5));          // EXPECT: 0
    printf("%d\n", !!1);            // EXPECT: 1
    printf("%d\n", !!0);            // EXPECT: 0
    printf("%d\n", !!!1);           // EXPECT: 0

    /* Non-zero values treated as true */
    printf("%d\n", 5 && 3);         // EXPECT: 1
    printf("%d\n", (-1) && 100);    // EXPECT: 1
    printf("%d\n", 0 && 100);       // EXPECT: 0
    printf("%d\n", 5 || 0);         // EXPECT: 1
    printf("%d\n", 0 || (-7));      // EXPECT: 1

    /* Short-circuit: && does NOT evaluate right if left is false */
    side_a = 0;
    side_b = 0;
    int r1 = set_a(0) && set_b(1);
    /* set_a(0) returns 0, so set_b(1) is NOT called */
    printf("%d\n", r1);             // EXPECT: 0
    printf("%d\n", side_a);         // EXPECT: 0
    printf("%d\n", side_b);         // EXPECT: 0
    /* side_b remains 0 because right operand not evaluated */

    /* Short-circuit: && DOES evaluate right if left is true */
    side_a = 0;
    side_b = 0;
    int r2 = set_a(1) && set_b(2);
    /* set_a(1) returns 1 (true), so set_b(2) IS called */
    printf("%d\n", r2);             // EXPECT: 1
    printf("%d\n", side_a);         // EXPECT: 1
    printf("%d\n", side_b);         // EXPECT: 2

    /* Short-circuit: || does NOT evaluate right if left is true */
    side_a = 0;
    side_b = 0;
    int r3 = set_a(5) || set_b(7);
    /* set_a(5) returns 5 (true), so set_b(7) is NOT called */
    printf("%d\n", r3);             // EXPECT: 1
    printf("%d\n", side_a);         // EXPECT: 5
    printf("%d\n", side_b);         // EXPECT: 0

    /* Short-circuit: || DOES evaluate right if left is false */
    side_a = 0;
    side_b = 0;
    int r4 = set_a(0) || set_b(3);
    /* set_a(0) returns 0 (false), so set_b(3) IS called */
    printf("%d\n", r4);             // EXPECT: 1
    printf("%d\n", side_a);         // EXPECT: 0
    printf("%d\n", side_b);         // EXPECT: 3

    /* Ternary operator */
    int t1 = 1 ? 10 : 20;
    printf("%d\n", t1);             // EXPECT: 10
    int t2 = 0 ? 10 : 20;
    printf("%d\n", t2);             // EXPECT: 20
    int t3 = (-1) ? 99 : 0;
    printf("%d\n", t3);             // EXPECT: 99

    /* Chained ternary */
    int val = 5;
    int result = (val < 0) ? -1 : (val == 0) ? 0 : 1;
    printf("%d\n", result);         // EXPECT: 1

    int val2 = -3;
    int result2 = (val2 < 0) ? -1 : (val2 == 0) ? 0 : 1;
    printf("%d\n", result2);        // EXPECT: -1

    int val3 = 0;
    int result3 = (val3 < 0) ? -1 : (val3 == 0) ? 0 : 1;
    printf("%d\n", result3);        // EXPECT: 0

    /* Complex boolean expressions */
    int a = 3, b = 7, c = 5;
    printf("%d\n", a < b && b > c);     // EXPECT: 1
    /* 3<7=1, 7>5=1, 1&&1=1 */
    printf("%d\n", a > b || b > c);     // EXPECT: 1
    /* 3>7=0, 7>5=1, 0||1=1 */
    printf("%d\n", !(a < b) || c < a);  // EXPECT: 0
    /* !(1)=0, 5<3=0, 0||0=0 */
    printf("%d\n", a < b && b > c && c > a); // EXPECT: 1
    /* 1 && 1 && 1 = 1 */

    /* Logical ops in conditional */
    int x = 10;
    int clamped = (x > 100) ? 100 : ((x < 0) ? 0 : x);
    printf("%d\n", clamped);        // EXPECT: 10

    int y = 150;
    int clamped2 = (y > 100) ? 100 : ((y < 0) ? 0 : y);
    printf("%d\n", clamped2);       // EXPECT: 100

    /* De Morgan: !(a && b) == !a || !b */
    int p = 1, q = 0;
    printf("%d\n", !(p && q));      // EXPECT: 1
    printf("%d\n", !p || !q);       // EXPECT: 1
    printf("%d\n", !(p && q) == (!p || !q)); // EXPECT: 1

    /* Logical result is always 0 or 1 */
    printf("%d\n", 5 && 3);         // EXPECT: 1
    printf("%d\n", 100 || 200);     // EXPECT: 1
    printf("%d\n", !100);           // EXPECT: 0

    return 0;
}
