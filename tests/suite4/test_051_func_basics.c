int printf(const char *fmt, ...);

// Simple function returning a constant
int get_zero(void) {
    return 0;
}

int get_forty_two(void) {
    return 42;
}

// Function with one parameter
int identity(int x) {
    return x;
}

int double_val(int x) {
    return x * 2;
}

int negate(int x) {
    return -x;
}

// Function with two parameters
int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

// Function with three parameters
int sum3(int a, int b, int c) {
    return a + b + c;
}

int clamp(int val, int lo, int hi) {
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

// Function with four parameters
int sum4(int a, int b, int c, int d) {
    return a + b + c + d;
}

// Function with five parameters
int sum5(int a, int b, int c, int d, int e) {
    return a + b + c + d + e;
}

// Function with six parameters
int sum6(int a, int b, int c, int d, int e, int f) {
    return a + b + c + d + e + f;
}

// Function that calls other functions
int compute(int x, int y) {
    int s = add(x, y);
    int d = subtract(x, y);
    return multiply(s, d);
}

// Function using local variables
int quadratic(int a, int b, int c, int x) {
    int x2 = x * x;
    int ax2 = a * x2;
    int bx = b * x;
    return ax2 + bx + c;
}

// Void-returning function that modifies via pointer
void set_value(int *p, int val) {
    *p = val;
}

// Forward declaration test
int forward_func(int x);

int uses_forward(int x) {
    return forward_func(x) + 1;
}

int forward_func(int x) {
    return x * 10;
}

// Function returning result of another function
int double_add(int a, int b) {
    return double_val(add(a, b));
}

int main(void) {
    // Test no-arg functions
    // EXPECT: 0
    printf("%d\n", get_zero());
    // EXPECT: 42
    printf("%d\n", get_forty_two());

    // Test single parameter
    // EXPECT: 7
    printf("%d\n", identity(7));
    // EXPECT: 20
    printf("%d\n", double_val(10));
    // EXPECT: -5
    printf("%d\n", negate(5));

    // Test two parameters
    // EXPECT: 15
    printf("%d\n", add(10, 5));
    // EXPECT: 5
    printf("%d\n", subtract(10, 5));
    // EXPECT: 50
    printf("%d\n", multiply(10, 5));

    // Test three parameters
    // EXPECT: 60
    printf("%d\n", sum3(10, 20, 30));
    // EXPECT: 5
    printf("%d\n", clamp(5, 0, 10));
    // EXPECT: 0
    printf("%d\n", clamp(-3, 0, 10));
    // EXPECT: 10
    printf("%d\n", clamp(15, 0, 10));

    // Test four, five, six parameters
    // EXPECT: 100
    printf("%d\n", sum4(10, 20, 30, 40));
    // EXPECT: 150
    printf("%d\n", sum5(10, 20, 30, 40, 50));
    // EXPECT: 210
    printf("%d\n", sum6(10, 20, 30, 40, 50, 60));

    // Test function calling other functions
    // compute(7, 3) = (7+3)*(7-3) = 10*4 = 40
    // EXPECT: 40
    printf("%d\n", compute(7, 3));

    // Test quadratic: 2*3^2 + 5*3 + 1 = 18 + 15 + 1 = 34
    // EXPECT: 34
    printf("%d\n", quadratic(2, 5, 1, 3));

    // Test void function with pointer
    int val = 0;
    set_value(&val, 99);
    // EXPECT: 99
    printf("%d\n", val);

    // Test forward declaration
    // EXPECT: 51
    printf("%d\n", uses_forward(5));

    // Test nested function calls
    // double_add(3,4) = double_val(add(3,4)) = double_val(7) = 14
    // EXPECT: 14
    printf("%d\n", double_add(3, 4));

    // Test chained calls
    // add(multiply(2,3), subtract(10,4)) = add(6, 6) = 12
    // EXPECT: 12
    printf("%d\n", add(multiply(2, 3), subtract(10, 4)));

    // Test function results as arguments
    int a = add(1, 2);
    int b = multiply(a, 3);
    int c = subtract(b, 1);
    // EXPECT: 8
    printf("%d\n", c);

    return 0;
}
