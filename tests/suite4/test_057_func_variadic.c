int printf(const char *fmt, ...);

// Helper functions to produce values for printf
int get_value(int x) {
    return x * 10;
}

int compute(int a, int b) {
    return a + b;
}

char *get_greeting(void) {
    return "hello";
}

char *get_farewell(void) {
    return "goodbye";
}

int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int fibonacci(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int abs_val(int x) {
    return x < 0 ? -x : x;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a < b ? a : b;
}

int main(void) {
    // Single integer argument
    // EXPECT: 42
    printf("%d\n", 42);

    // Two integer arguments
    // EXPECT: 10 20
    printf("%d %d\n", 10, 20);

    // Three integer arguments
    // EXPECT: 1 2 3
    printf("%d %d %d\n", 1, 2, 3);

    // Four integer arguments
    // EXPECT: 10 20 30 40
    printf("%d %d %d %d\n", 10, 20, 30, 40);

    // Five integer arguments
    // EXPECT: 1 2 3 4 5
    printf("%d %d %d %d %d\n", 1, 2, 3, 4, 5);

    // Six integer arguments
    // EXPECT: 10 20 30 40 50 60
    printf("%d %d %d %d %d %d\n", 10, 20, 30, 40, 50, 60);

    // String argument
    // EXPECT: hello world
    printf("%s %s\n", "hello", "world");

    // Mixed string and int
    // EXPECT: value is 99
    printf("value is %d\n", 99);

    // EXPECT: name is Alice age 30
    printf("name is %s age %d\n", "Alice", 30);

    // Computed arguments
    // EXPECT: 30
    printf("%d\n", get_value(3));
    // EXPECT: 15
    printf("%d\n", compute(7, 8));

    // String function results
    // EXPECT: hello
    printf("%s\n", get_greeting());
    // EXPECT: goodbye
    printf("%s\n", get_farewell());

    // Multiple computed results
    // EXPECT: 50 70
    printf("%d %d\n", get_value(5), get_value(7));

    // Factorial results
    // EXPECT: 1 1 2 6 24 120
    printf("%d %d %d %d %d %d\n",
        factorial(0), factorial(1), factorial(2),
        factorial(3), factorial(4), factorial(5));

    // Fibonacci results
    // EXPECT: 0 1 1 2 3 5
    printf("%d %d %d %d %d %d\n",
        fibonacci(0), fibonacci(1), fibonacci(2),
        fibonacci(3), fibonacci(4), fibonacci(5));

    // Complex expressions as arguments
    int a = 10;
    int b = 3;
    // EXPECT: 13 7 30 3 1
    printf("%d %d %d %d %d\n", a + b, a - b, a * b, a / b, a % b);

    // Nested function calls as arguments
    // EXPECT: 10 3
    printf("%d %d\n", max(10, 3), min(10, 3));

    // Abs values
    // EXPECT: 5 5 0
    printf("%d %d %d\n", abs_val(5), abs_val(-5), abs_val(0));

    // Ternary results as arguments
    int x = 7;
    // EXPECT: positive odd
    printf("%s %s\n",
        x > 0 ? "positive" : "non-positive",
        x % 2 != 0 ? "odd" : "even");

    // Printf with no extra arguments (just a string)
    // EXPECT: just a string
    printf("just a string\n");

    // Array element as arguments
    int arr[] = {100, 200, 300, 400, 500};
    // EXPECT: 100 300 500
    printf("%d %d %d\n", arr[0], arr[2], arr[4]);

    // Pointer dereference as arguments
    int val = 77;
    int *p = &val;
    // EXPECT: 77
    printf("%d\n", *p);

    // Struct member as argument
    struct Pair { int first; int second; };
    struct Pair pair;
    pair.first = 11;
    pair.second = 22;
    // EXPECT: 11 22
    printf("%d %d\n", pair.first, pair.second);

    // Complex formatting
    // EXPECT: result: 42 status: ok
    printf("result: %d status: %s\n", 42, "ok");

    // EXPECT: a=1 b=2 c=3 d=4
    printf("a=%d b=%d c=%d d=%d\n", 1, 2, 3, 4);

    // Large values
    // EXPECT: 1000000 -1000000
    printf("%d %d\n", 1000000, -1000000);

    // Expressions involving multiple operations
    int m = 5;
    int n = 8;
    // (5+8)*(5-8) = 13*(-3) = -39
    // EXPECT: -39
    printf("%d\n", (m + n) * (m - n));

    // Multiple strings
    // EXPECT: the quick brown fox
    printf("%s %s %s %s\n", "the", "quick", "brown", "fox");

    return 0;
}
