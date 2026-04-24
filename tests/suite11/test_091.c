int printf(const char *fmt, ...);

// Test: Comma operator chains
// The comma operator evaluates left-to-right and returns the last value.

int global_counter;

int bump(int val) {
    global_counter = global_counter + val;
    return global_counter;
}

int main() {
    int a;
    int b;
    int c;
    int d;
    int e;
    int result;

    // Basic comma operator: evaluates all, returns last
    a = (1, 2, 3, 4, 5);
    printf("%d\n", a); // EXPECT: 5

    // Comma with side effects
    b = 0;
    c = (b = 10, b + 5);
    printf("%d\n", b); // EXPECT: 10
    printf("%d\n", c); // EXPECT: 15

    // Nested comma operators
    d = 0;
    e = 0;
    result = (d = 1, e = 2, d + e);
    printf("%d\n", d); // EXPECT: 1
    printf("%d\n", e); // EXPECT: 2
    printf("%d\n", result); // EXPECT: 3

    // Comma in for loop initialization
    a = 0;
    b = 0;
    for (a = 0, b = 10; a < 3; a = a + 1, b = b - 1) {
        // loop body
    }
    printf("%d\n", a); // EXPECT: 3
    printf("%d\n", b); // EXPECT: 7

    // Comma operator with function calls
    global_counter = 0;
    result = (bump(1), bump(2), bump(3));
    printf("%d\n", result); // EXPECT: 6
    printf("%d\n", global_counter); // EXPECT: 6

    // Comma in while condition (unusual but valid)
    a = 0;
    b = 5;
    while (a = a + 1, a < 4) {
        b = b + a;
    }
    printf("%d\n", a); // EXPECT: 4
    printf("%d\n", b); // EXPECT: 11

    // Chained comma with multiple assignments
    a = 0;
    b = 0;
    c = 0;
    d = 0;
    e = (a = 100, b = 200, c = 300, d = 400, a + b + c + d);
    printf("%d\n", a); // EXPECT: 100
    printf("%d\n", b); // EXPECT: 200
    printf("%d\n", c); // EXPECT: 300
    printf("%d\n", d); // EXPECT: 400
    printf("%d\n", e); // EXPECT: 1000

    // Comma operator precedence (lower than assignment)
    a = 0;
    b = 0;
    a = 5;
    b = (a = 10, a * 2);
    printf("%d\n", a); // EXPECT: 10
    printf("%d\n", b); // EXPECT: 20

    // Comma in return-like expression
    result = (1 + 2, 3 + 4, 5 + 6);
    printf("%d\n", result); // EXPECT: 11

    // Multiple commas with increments
    a = 0;
    b = 0;
    c = 0;
    result = (a = a + 1, b = b + 2, c = b + a, c * 3);
    printf("%d\n", a); // EXPECT: 1
    printf("%d\n", b); // EXPECT: 2
    printf("%d\n", c); // EXPECT: 3
    printf("%d\n", result); // EXPECT: 9

    // Comma operator discarding results
    a = 0;
    result = (a = 999, 42);
    printf("%d\n", a); // EXPECT: 999
    printf("%d\n", result); // EXPECT: 42

    // Comma in complex for loop
    a = 0;
    b = 1;
    c = 0;
    for (a = 0, b = 1; a < 5; a = a + 1, b = b * 2) {
        c = c + b;
    }
    printf("%d\n", c); // EXPECT: 31

    // Deeply chained comma
    result = (1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    printf("%d\n", result); // EXPECT: 10

    // Comma with conditional side effects
    a = 0;
    b = 0;
    if (a = 5, b = 10, a + b > 10) {
        printf("%d\n", 1); // EXPECT: 1
    } else {
        printf("%d\n", 0);
    }
    printf("%d\n", a); // EXPECT: 5
    printf("%d\n", b); // EXPECT: 10

    return 0;
}
