int printf(const char *fmt, ...);

/* Basic math functions for use via pointers */
int add(int a, int b)      { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }
int divide(int a, int b)   { return a / b; }
int modulo(int a, int b)   { return a % b; }
int maximum(int a, int b)  { return a > b ? a : b; }
int minimum(int a, int b)  { return a < b ? a : b; }

/* Unary functions */
int square(int x)  { return x * x; }
int cube(int x)    { return x * x * x; }
int negate(int x)  { return -x; }
int identity(int x){ return x; }
int increment(int x){ return x + 1; }
int decrement(int x){ return x - 1; }

/* Predicate functions (return 0 or 1) */
int is_positive(int x) { return x > 0 ? 1 : 0; }
int is_negative(int x) { return x < 0 ? 1 : 0; }
int is_zero(int x)     { return x == 0 ? 1 : 0; }
int is_even(int x)     { return x % 2 == 0 ? 1 : 0; }
int is_odd(int x)      { return x % 2 != 0 ? 1 : 0; }

/* Function pointer typedefs */
typedef int (*BinaryOp)(int, int);
typedef int (*UnaryOp)(int);
typedef int (*Predicate)(int);

/* Apply a binary op to two values */
int apply_binary(BinaryOp op, int a, int b) {
    return op(a, b);
}

/* Apply a unary op to a value */
int apply_unary(UnaryOp op, int x) {
    return op(x);
}

/* Apply unary op to each element of array, return sum of results */
int map_sum(UnaryOp op, int *arr, int n) {
    int total = 0;
    int i;
    for (i = 0; i < n; i++) {
        total += op(arr[i]);
    }
    return total;
}

/* Count elements satisfying predicate */
int count_if(Predicate pred, int *arr, int n) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (pred(arr[i])) count++;
    }
    return count;
}

/* Compose two unary functions: compose(f, g)(x) = f(g(x)) */
/* We implement this by calling directly */
int compose_apply(UnaryOp f, UnaryOp g, int x) {
    return f(g(x));
}

/* Function that returns result of calling op with both args swapped */
int apply_swapped(BinaryOp op, int a, int b) {
    return op(b, a);
}

int main(void) {
    /* Direct function pointer assignment and call */
    BinaryOp op;
    op = add;
    printf("%d\n", op(3, 4));       // EXPECT: 7

    op = subtract;
    printf("%d\n", op(10, 3));      // EXPECT: 7

    op = multiply;
    printf("%d\n", op(6, 7));       // EXPECT: 42

    op = divide;
    printf("%d\n", op(20, 4));      // EXPECT: 5

    op = modulo;
    printf("%d\n", op(17, 5));      // EXPECT: 2

    op = maximum;
    printf("%d\n", op(9, 3));       // EXPECT: 9

    op = minimum;
    printf("%d\n", op(9, 3));       // EXPECT: 3

    /* Unary function pointers */
    UnaryOp uop;
    uop = square;
    printf("%d\n", uop(5));         // EXPECT: 25

    uop = cube;
    printf("%d\n", uop(3));         // EXPECT: 27

    uop = negate;
    printf("%d\n", uop(-8));        // EXPECT: 8

    uop = increment;
    printf("%d\n", uop(41));        // EXPECT: 42

    /* Array of function pointers */
    BinaryOp ops[5];
    ops[0] = add;
    ops[1] = subtract;
    ops[2] = multiply;
    ops[3] = divide;
    ops[4] = modulo;

    printf("%d\n", ops[0](10, 5));  // EXPECT: 15
    printf("%d\n", ops[1](10, 5));  // EXPECT: 5
    printf("%d\n", ops[2](10, 5));  // EXPECT: 50
    printf("%d\n", ops[3](10, 5));  // EXPECT: 2
    printf("%d\n", ops[4](10, 5));  // EXPECT: 0

    /* Array of unary function pointers */
    UnaryOp unary_ops[4];
    unary_ops[0] = identity;
    unary_ops[1] = square;
    unary_ops[2] = cube;
    unary_ops[3] = negate;

    int i;
    for (i = 0; i < 4; i++) {
        printf("%d\n", unary_ops[i](3)); // EXPECT: 3
                                          // EXPECT: 9
                                          // EXPECT: 27
                                          // EXPECT: -3
    }

    /* Function pointer as parameter */
    printf("%d\n", apply_binary(add, 100, 23));       // EXPECT: 123
    printf("%d\n", apply_binary(multiply, 7, 8));     // EXPECT: 56
    printf("%d\n", apply_unary(square, 9));            // EXPECT: 81
    printf("%d\n", apply_unary(negate, -15));          // EXPECT: 15

    /* Map and count */
    int arr[6];
    arr[0] = -3; arr[1] = -1; arr[2] = 0; arr[3] = 2; arr[4] = 4; arr[5] = 6;
    printf("%d\n", map_sum(square, arr, 6));           // EXPECT: 66
    /* -3^2=9, -1^2=1, 0^2=0, 2^2=4, 4^2=16, 6^2=36  => 9+1+0+4+16+36 = 66 */

    printf("%d\n", count_if(is_positive, arr, 6));     // EXPECT: 3
    printf("%d\n", count_if(is_negative, arr, 6));     // EXPECT: 2
    printf("%d\n", count_if(is_zero, arr, 6));         // EXPECT: 1
    printf("%d\n", count_if(is_even, arr, 6));         // EXPECT: 4
    printf("%d\n", count_if(is_odd, arr, 6));          // EXPECT: 2

    /* Compose */
    printf("%d\n", compose_apply(negate, square, 4));  // EXPECT: -16
    printf("%d\n", compose_apply(square, increment, 4)); // EXPECT: 25

    /* Swapped arguments */
    printf("%d\n", apply_swapped(subtract, 10, 3));    // EXPECT: -7
    /* subtract(b=3, a=10) = 3 - 10 = -7 */
    printf("%d\n", apply_swapped(divide, 2, 10));      // EXPECT: 5
    /* divide(b=10, a=2) = 10 / 2 = 5 */

    return 0;
}
