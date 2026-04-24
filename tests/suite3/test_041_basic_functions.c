int printf(const char *fmt, ...);

/* Functions with 0 parameters */
int get_zero(void) {
    return 0;
}

int get_forty_two(void) {
    return 42;
}

int get_negative(void) {
    return -7;
}

/* Void function */
void do_nothing(void) {
    /* intentionally empty */
}

/* Functions with 1 parameter */
int double_it(int x) {
    return x * 2;
}

int negate(int x) {
    return -x;
}

int square(int x) {
    return x * x;
}

/* Functions with 2 parameters */
int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

int max2(int a, int b) {
    if (a > b) return a;
    return b;
}

int min2(int a, int b) {
    if (a < b) return a;
    return b;
}

/* Functions with 3 parameters */
int add3(int a, int b, int c) {
    return a + b + c;
}

int clamp(int val, int lo, int hi) {
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

int median3(int a, int b, int c) {
    /* return the middle value */
    if ((a <= b && b <= c) || (c <= b && b <= a)) return b;
    if ((b <= a && a <= c) || (c <= a && a <= b)) return a;
    return c;
}

/* Functions with 4 parameters */
int sum4(int a, int b, int c, int d) {
    return a + b + c + d;
}

int dot2(int ax, int ay, int bx, int by) {
    return ax * bx + ay * by;
}

/* Functions with 5 parameters */
int sum5(int a, int b, int c, int d, int e) {
    return a + b + c + d + e;
}

/* Functions with 6 parameters */
int sum6(int a, int b, int c, int d, int e, int f) {
    return a + b + c + d + e + f;
}

/* Calling functions from functions */
int sum_of_squares(int a, int b) {
    return add(square(a), square(b));
}

int range(int a, int b) {
    return subtract(max2(a, b), min2(a, b));
}

int abs_val(int x) {
    if (x < 0) return negate(x);
    return x;
}

int distance1d(int a, int b) {
    return abs_val(subtract(a, b));
}

int main(void) {
    /* 0-param functions */
    printf("%d\n", get_zero());         // EXPECT: 0
    printf("%d\n", get_forty_two());    // EXPECT: 42
    printf("%d\n", get_negative());     // EXPECT: -7

    /* 1-param functions */
    printf("%d\n", double_it(5));       // EXPECT: 10
    printf("%d\n", double_it(-3));      // EXPECT: -6
    printf("%d\n", negate(8));          // EXPECT: -8
    printf("%d\n", negate(-4));         // EXPECT: 4
    printf("%d\n", square(7));          // EXPECT: 49
    printf("%d\n", square(-6));         // EXPECT: 36

    /* 2-param functions */
    printf("%d\n", add(3, 4));          // EXPECT: 7
    printf("%d\n", subtract(10, 3));    // EXPECT: 7
    printf("%d\n", multiply(6, 7));     // EXPECT: 42
    printf("%d\n", max2(9, 4));         // EXPECT: 9
    printf("%d\n", max2(3, 11));        // EXPECT: 11
    printf("%d\n", min2(9, 4));         // EXPECT: 4
    printf("%d\n", min2(3, 11));        // EXPECT: 3

    /* 3-param functions */
    printf("%d\n", add3(1, 2, 3));      // EXPECT: 6
    printf("%d\n", clamp(5, 0, 10));    // EXPECT: 5
    printf("%d\n", clamp(-3, 0, 10));   // EXPECT: 0
    printf("%d\n", clamp(15, 0, 10));   // EXPECT: 10
    printf("%d\n", median3(3, 1, 2));   // EXPECT: 2
    printf("%d\n", median3(1, 3, 2));   // EXPECT: 2
    printf("%d\n", median3(2, 2, 2));   // EXPECT: 2

    /* 4-param functions */
    printf("%d\n", sum4(1, 2, 3, 4));   // EXPECT: 10
    printf("%d\n", dot2(1, 2, 3, 4));   // EXPECT: 11

    /* 5-param functions */
    printf("%d\n", sum5(1, 2, 3, 4, 5)); // EXPECT: 15

    /* 6-param functions */
    printf("%d\n", sum6(1, 2, 3, 4, 5, 6)); // EXPECT: 21

    /* Chained calls */
    printf("%d\n", sum_of_squares(3, 4));   // EXPECT: 25
    printf("%d\n", range(8, 3));             // EXPECT: 5
    printf("%d\n", range(3, 8));             // EXPECT: 5
    printf("%d\n", abs_val(-11));            // EXPECT: 11
    printf("%d\n", abs_val(11));             // EXPECT: 11
    printf("%d\n", distance1d(3, 7));        // EXPECT: 4
    printf("%d\n", distance1d(7, 3));        // EXPECT: 4

    return 0;
}
