int printf(const char *fmt, ...);

/* Token pasting ## operator in macros */

/* Paste two tokens together to form a new identifier */
#define PASTE(a, b)         a##b
#define PASTE3(a, b, c)     a##b##c

/* Paste to form a variable name */
#define MAKE_VAR(prefix, n) prefix##n

/* Paste to form a function name */
#define CALL_FN(name, suffix) name##suffix

/* Paste a prefix onto a number */
#define VAR(n)              var_##n

/* Integer token pasting to form a larger number */
#define COMBINE_DIGITS(a, b) a##b

/* Struct field access with pasted name */
#define FIELD(base, num)    base##num

/* Named counter macro using paste */
#define COUNTER(x)          counter_##x

/* Two-level paste: expand before pasting */
#define XPASTE(a, b)        PASTE(a, b)
#define PREFIX              my
#define SUFFIX              func

int add10(int x) { return x + 10; }
int add20(int x) { return x + 20; }
int add30(int x) { return x + 30; }

int mul2(int x) { return x * 2; }
int mul3(int x) { return x * 3; }
int mul4(int x) { return x * 4; }

#define CALL_ADD(n, x)      add##n(x)
#define CALL_MUL(n, x)      mul##n(x)

int main(void) {
    /* Basic token pasting */
    int PASTE(foo, bar) = 42;
    printf("%d\n", foobar);              // EXPECT: 42

    int PASTE(x, 1) = 10;
    int PASTE(x, 2) = 20;
    int PASTE(x, 3) = 30;
    printf("%d\n", x1);                  // EXPECT: 10
    printf("%d\n", x2);                  // EXPECT: 20
    printf("%d\n", x3);                  // EXPECT: 30
    printf("%d\n", x1 + x2 + x3);       // EXPECT: 60

    /* Three-token paste */
    int PASTE3(a, b, c) = 99;
    printf("%d\n", abc);                 // EXPECT: 99

    /* MAKE_VAR: prefix + number */
    int MAKE_VAR(val, 1) = 100;
    int MAKE_VAR(val, 2) = 200;
    int MAKE_VAR(val, 3) = 300;
    printf("%d\n", val1);                // EXPECT: 100
    printf("%d\n", val2);                // EXPECT: 200
    printf("%d\n", val3);                // EXPECT: 300

    /* VAR macro: prefix with underscore */
    int VAR(0) = 5;
    int VAR(1) = 6;
    int VAR(2) = 7;
    printf("%d\n", var_0);               // EXPECT: 5
    printf("%d\n", var_1);               // EXPECT: 6
    printf("%d\n", var_2);               // EXPECT: 7
    printf("%d\n", var_0 + var_1 + var_2); // EXPECT: 18

    /* COUNTER macro */
    int COUNTER(a) = 11;
    int COUNTER(b) = 22;
    int COUNTER(c) = 33;
    printf("%d\n", counter_a);           // EXPECT: 11
    printf("%d\n", counter_b);           // EXPECT: 22
    printf("%d\n", counter_c);           // EXPECT: 33

    /* Pasting to call functions */
    printf("%d\n", CALL_ADD(10, 5));     // EXPECT: 15
    printf("%d\n", CALL_ADD(20, 5));     // EXPECT: 25
    printf("%d\n", CALL_ADD(30, 5));     // EXPECT: 35

    printf("%d\n", CALL_MUL(2, 7));      // EXPECT: 14
    printf("%d\n", CALL_MUL(3, 7));      // EXPECT: 21
    printf("%d\n", CALL_MUL(4, 7));      // EXPECT: 28

    /* COMBINE_DIGITS: paste digit tokens */
    int n = COMBINE_DIGITS(1, 2);
    printf("%d\n", n);                   // EXPECT: 12

    int m = COMBINE_DIGITS(9, 9);
    printf("%d\n", m);                   // EXPECT: 99

    /* PASTE3 for longer names */
    int PASTE3(my, _val, ue) = 55;
    printf("%d\n", my_value);            // EXPECT: 55

    /* FIELD macro for struct-like naming */
    int FIELD(item, 0) = 1000;
    int FIELD(item, 1) = 2000;
    printf("%d\n", item0);               // EXPECT: 1000
    printf("%d\n", item1);               // EXPECT: 2000
    printf("%d\n", item0 + item1);       // EXPECT: 3000

    /* Use paste result in expression */
    int PASTE(sum_, val) = MAKE_VAR(val, 1) + MAKE_VAR(val, 2) + MAKE_VAR(val, 3);
    printf("%d\n", sum_val);             // EXPECT: 600
    /* 100 + 200 + 300 = 600 */

    /* Paste with arithmetic on result */
    int PASTE(p, q) = 8;
    printf("%d\n", pq * 3);             // EXPECT: 24
    printf("%d\n", pq + PASTE(x, 1));   // EXPECT: 18
    /* 8 + 10 = 18 */

    return 0;
}
