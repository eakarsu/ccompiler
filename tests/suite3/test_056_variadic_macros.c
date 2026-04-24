int printf(const char *fmt, ...);

/* Variadic macros: __VA_ARGS__ */

/* Basic variadic macro forwarding to printf */
#define LOG(fmt, ...)       printf(fmt, __VA_ARGS__)

/* Variadic with a named leading param */
#define LOG2(label, fmt, ...) printf("[" label "] " fmt, __VA_ARGS__)

/* Count-style: wrap with prefix and newline */
#define PRINT(fmt, ...)     printf(fmt "\n", __VA_ARGS__)

/* Variadic macro that adds a tag */
#define INFO(fmt, ...)      printf("INFO: " fmt "\n", __VA_ARGS__)
#define WARN(fmt, ...)      printf("WARN: " fmt "\n", __VA_ARGS__)

/* Compute and print */
#define SHOW(name, expr)    printf(name "=%d\n", (expr))

/* Two fixed args, then variadic */
#define DBG(file, line, fmt, ...) \
    printf(file ":%d: " fmt "\n", (line), __VA_ARGS__)

/* Variadic with three or more args */
#define SUM3_PRINT(a, b, c) printf("%d\n", (a) + (b) + (c))

/* Variadic forwarding for formatted output */
#define FMT_PRINT(fmt, ...) printf(fmt, __VA_ARGS__)

/* Named-param variadic */
#define ADD_PRINT(x, ...)   printf("%d\n", (x) + __VA_ARGS__ + 0)

int helper(int a, int b, int c) {
    return a + b + c;
}

int main(void) {
    /* Basic LOG variadic */
    LOG("%d\n", 42);                     // EXPECT: 42
    LOG("%d\n", 100);                    // EXPECT: 100
    LOG("%d %d\n", 3, 4);               // EXPECT: 3 4
    LOG("%d %d %d\n", 1, 2, 3);        // EXPECT: 1 2 3

    /* PRINT macro (appends newline) */
    PRINT("%d", 10);                     // EXPECT: 10
    PRINT("%d", 20);                     // EXPECT: 20
    PRINT("%d %d", 5, 6);               // EXPECT: 5 6

    /* INFO and WARN with variadic */
    INFO("%d", 7);                       // EXPECT: INFO: 7
    INFO("%d %d", 8, 9);                // EXPECT: INFO: 8 9
    WARN("%d", 0);                       // EXPECT: WARN: 0
    WARN("%d %d", 11, 22);              // EXPECT: WARN: 11 22

    /* LOG2: named label + variadic */
    LOG2("test", "%d\n", 55);           // EXPECT: [test] 55
    LOG2("math", "%d\n", 99);           // EXPECT: [math] 99
    LOG2("sys", "%d %d\n", 1, 2);      // EXPECT: [sys] 1 2

    /* SHOW macro */
    int x = 5, y = 7;
    SHOW("x", x);                        // EXPECT: x=5
    SHOW("y", y);                        // EXPECT: y=7
    SHOW("x+y", x + y);                  // EXPECT: x+y=12
    SHOW("x*y", x * y);                  // EXPECT: x*y=35

    /* DBG macro: file, line, variadic */
    DBG("main.c", 1, "%d", 42);         // EXPECT: main.c:1: 42
    DBG("main.c", 2, "%d %d", 3, 4);   // EXPECT: main.c:2: 3 4

    /* SUM3_PRINT */
    SUM3_PRINT(1, 2, 3);                 // EXPECT: 6
    SUM3_PRINT(10, 20, 30);              // EXPECT: 60
    SUM3_PRINT(0, 0, 0);                 // EXPECT: 0

    /* FMT_PRINT: direct forwarding */
    FMT_PRINT("%d\n", 77);              // EXPECT: 77
    FMT_PRINT("%d %d\n", 3, 5);        // EXPECT: 3 5
    FMT_PRINT("%d %d %d\n", 4, 5, 6);  // EXPECT: 4 5 6

    /* Multiple variadic arguments */
    LOG("%d %d %d %d\n", 10, 20, 30, 40); // EXPECT: 10 20 30 40

    /* Using variadic macro inside a loop */
    int i;
    for (i = 1; i <= 5; i++) {
        PRINT("%d", i * i);
    }
    // EXPECT: 1
    // EXPECT: 4
    // EXPECT: 9
    // EXPECT: 16
    // EXPECT: 25

    /* Variadic macro with computed arguments */
    int a = 3, b = 4;
    LOG("%d\n", a * a + b * b);          // EXPECT: 25
    /* 9 + 16 = 25 */

    LOG("%d %d\n", a + b, a * b);       // EXPECT: 7 12

    /* Variadic macro calling a function */
    LOG("%d\n", helper(1, 2, 3));       // EXPECT: 6
    LOG("%d\n", helper(10, 20, 30));    // EXPECT: 60

    return 0;
}
