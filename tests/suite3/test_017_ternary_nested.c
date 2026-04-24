int printf(const char *fmt, ...);

/* test_017_ternary_nested.c
   Tests: ternary operator, nested ternary, ternary as function arg,
   ternary with side effects, ternary in loop condition.
*/

int abs_val(int x) {
    return x < 0 ? -x : x;
}

int clamp(int x, int lo, int hi) {
    return x < lo ? lo : (x > hi ? hi : x);
}

/* nested ternary: classify sign with magnitude */
int sign_class(int x) {
    return x > 100 ? 3 : (x > 0 ? 2 : (x == 0 ? 1 : 0));
}
/* sign_class(200) = 3 */
/* sign_class(50) = 2 */
/* sign_class(0) = 1 */
/* sign_class(-5) = 0 */

/* ternary as function argument */
int add(int a, int b) { return a + b; }

int ternary_arg(int flag, int a, int b) {
    return add(flag ? a : b, flag ? b : a);
}
/* always returns a+b regardless of flag */

/* ternary with side effects: only one branch evaluated */
int increment(int *p) {
    (*p)++;
    return *p;
}

int ternary_side_effect(int cond, int *p1, int *p2) {
    /* only one of p1 or p2 should be incremented */
    return cond ? increment(p1) : increment(p2);
}

/* deeply nested ternary: 4 levels */
int classify4(int n) {
    return n < 0 ? -1
         : n == 0 ? 0
         : n < 10 ? 1
         : n < 100 ? 2
         : 3;
}

/* ternary in loop condition */
int ternary_loop(int start, int step) {
    int s = 0;
    for (int i = start; (step > 0 ? i <= 10 : i >= 1); i += step) {
        s += i;
    }
    return s;
}
/* ternary_loop(1, 1): sum 1..10 = 55 */
/* ternary_loop(10, -1): sum 10..1 = 55 */

/* ternary assigning to variable */
int ternary_assign(int x) {
    int result;
    result = (x % 2 == 0) ? x / 2 : x * 3 + 1;
    return result;
}
/* ternary_assign(4) = 2 */
/* ternary_assign(5) = 16 */

/* nested ternary selecting among 3 arrays */
int table_a[5] = {10, 20, 30, 40, 50};
int table_b[5] = {100, 200, 300, 400, 500};
int table_c[5] = {1000, 2000, 3000, 4000, 5000};

int table_select(int which, int idx) {
    return which == 0 ? table_a[idx]
         : which == 1 ? table_b[idx]
         : table_c[idx];
}

/* ternary used to avoid division by zero */
int safe_div(int a, int b) {
    return b != 0 ? a / b : -1;
}

/* ternary chain mimicking abs difference */
int abs_diff(int a, int b) {
    return a > b ? a - b : b - a;
}

/* ternary with complex condition */
int ternary_complex(int a, int b, int c) {
    return (a > 0 && b > 0) ? (c > 0 ? a + b + c : a + b) : 0;
}
/* ternary_complex(1, 2, 3) = 6 */
/* ternary_complex(1, 2, -1) = 3 */
/* ternary_complex(-1, 2, 3) = 0 */

int main(void) {
    printf("%d\n", abs_val(-7));  // EXPECT: 7
    printf("%d\n", abs_val(0));   // EXPECT: 0
    printf("%d\n", abs_val(5));   // EXPECT: 5

    printf("%d\n", clamp(-5, 0, 10));  // EXPECT: 0
    printf("%d\n", clamp(5, 0, 10));   // EXPECT: 5
    printf("%d\n", clamp(15, 0, 10));  // EXPECT: 10

    printf("%d\n", sign_class(200)); // EXPECT: 3
    printf("%d\n", sign_class(50));  // EXPECT: 2
    printf("%d\n", sign_class(0));   // EXPECT: 1
    printf("%d\n", sign_class(-5));  // EXPECT: 0

    printf("%d\n", ternary_arg(1, 3, 7)); // EXPECT: 10
    printf("%d\n", ternary_arg(0, 3, 7)); // EXPECT: 10

    int p1 = 0, p2 = 0;
    ternary_side_effect(1, &p1, &p2);
    printf("%d %d\n", p1, p2); // EXPECT: 1 0
    ternary_side_effect(0, &p1, &p2);
    printf("%d %d\n", p1, p2); // EXPECT: 1 1

    printf("%d\n", classify4(-5));  // EXPECT: -1
    printf("%d\n", classify4(0));   // EXPECT: 0
    printf("%d\n", classify4(7));   // EXPECT: 1
    printf("%d\n", classify4(50));  // EXPECT: 2
    printf("%d\n", classify4(200)); // EXPECT: 3

    printf("%d\n", ternary_loop(1, 1));   // EXPECT: 55
    printf("%d\n", ternary_loop(10, -1)); // EXPECT: 55

    printf("%d\n", ternary_assign(4)); // EXPECT: 2
    printf("%d\n", ternary_assign(5)); // EXPECT: 16

    printf("%d\n", table_select(0, 2)); // EXPECT: 30
    printf("%d\n", table_select(1, 3)); // EXPECT: 400
    printf("%d\n", table_select(2, 4)); // EXPECT: 5000

    printf("%d\n", safe_div(10, 3));  // EXPECT: 3
    printf("%d\n", safe_div(10, 0));  // EXPECT: -1

    printf("%d\n", abs_diff(7, 3)); // EXPECT: 4
    printf("%d\n", abs_diff(3, 7)); // EXPECT: 4

    printf("%d\n", ternary_complex(1, 2, 3));  // EXPECT: 6
    printf("%d\n", ternary_complex(1, 2, -1)); // EXPECT: 3
    printf("%d\n", ternary_complex(-1, 2, 3)); // EXPECT: 0

    /* inline ternary in expression */
    int x = 3, y = 5;
    int m = x > y ? x : y;
    printf("%d\n", m); // EXPECT: 5

    /* ternary used as lhs is not valid, but as rhs in chain */
    int a = 10, b = 20;
    int result = (a < b ? a : b) + (a > b ? a : b);
    printf("%d\n", result); // EXPECT: 30

    return 0;
}
