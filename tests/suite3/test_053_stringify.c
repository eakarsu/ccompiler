int printf(const char *fmt, ...);

/* Stringify operator # in macros */

/* Basic stringify macro */
#define STR(x)          #x

/* Stringify and print with label */
#define PRINT_STR(x)    printf("%s\n", #x)

/* Stringify a number token */
#define STR_NUM(n)      #n

/* Combine stringify with other text (using printf directly) */
#define SHOW_VAR(name, val) printf(#name " = %d\n", (val))

/* Stringify identifiers */
#define NAMEOF(x)       #x

/* Stringify expressions as tokens (not evaluated) */
#define TOKEN_STR(a, b) #a "+" #b

/* Stringify in a helper that builds a message */
#define LABEL(x)        "label:" #x

/* Macro that stringifies its argument and prints it */
#define PRINT_NAME(x)   printf("%s\n", #x)

/* Stringify a macro value: use two levels */
#define XSTR(x)         STR(x)
#define VERSION         42
#define VERSION_STR     XSTR(VERSION)

/* Stringify combined with integer printing */
#define PRINT_BOTH(name, val) \
    printf("%s=%d\n", #name, (val))

/* Simple token identifiers */
#define ALPHA   1
#define BETA    2
#define GAMMA   3

int main(void) {
    /* Basic stringify of number tokens */
    printf("%s\n", STR(hello));          // EXPECT: hello
    printf("%s\n", STR(world));          // EXPECT: world
    printf("%s\n", STR(42));             // EXPECT: 42
    printf("%s\n", STR(100));            // EXPECT: 100
    printf("%s\n", STR(0));              // EXPECT: 0

    /* Stringify identifier tokens */
    printf("%s\n", NAMEOF(alpha));       // EXPECT: alpha
    printf("%s\n", NAMEOF(beta));        // EXPECT: beta
    printf("%s\n", NAMEOF(main));        // EXPECT: main
    printf("%s\n", NAMEOF(printf));      // EXPECT: printf

    /* PRINT_STR macro */
    PRINT_STR(foo);                      // EXPECT: foo
    PRINT_STR(bar);                      // EXPECT: bar
    PRINT_STR(baz);                      // EXPECT: baz

    /* STR_NUM: stringify number tokens */
    printf("%s\n", STR_NUM(1));          // EXPECT: 1
    printf("%s\n", STR_NUM(999));        // EXPECT: 999
    printf("%s\n", STR_NUM(2025));       // EXPECT: 2025

    /* SHOW_VAR: stringify name and print value */
    int x = 10;
    int y = 20;
    SHOW_VAR(x, x);                      // EXPECT: x = 10
    SHOW_VAR(y, y);                      // EXPECT: y = 20
    SHOW_VAR(x, x + y);                  // EXPECT: x = 30

    /* TOKEN_STR: two stringifications joined */
    printf("%s\n", TOKEN_STR(a, b));     // EXPECT: a+b
    printf("%s\n", TOKEN_STR(foo, bar)); // EXPECT: foo+bar

    /* LABEL macro */
    printf("%s\n", LABEL(start));        // EXPECT: label:start
    printf("%s\n", LABEL(end));          // EXPECT: label:end
    printf("%s\n", LABEL(loop));         // EXPECT: label:loop

    /* PRINT_NAME */
    PRINT_NAME(count);                   // EXPECT: count
    PRINT_NAME(total);                   // EXPECT: total
    PRINT_NAME(index);                   // EXPECT: index

    /* Two-level stringify: XSTR expands macro before stringifying */
    /* VERSION_STR = XSTR(42) = STR(42) = "42" */
    printf("%s\n", VERSION_STR);         // EXPECT: 42

    /* Direct STR does NOT expand macros, just stringifies token */
    printf("%s\n", STR(VERSION));        // EXPECT: VERSION

    /* PRINT_BOTH */
    int count = 7;
    PRINT_BOTH(count, count);            // EXPECT: count=7
    int total = 99;
    PRINT_BOTH(total, total);            // EXPECT: total=99

    /* Stringify with numeric macros */
    printf("%s\n", NAMEOF(ALPHA));       // EXPECT: ALPHA
    printf("%s\n", NAMEOF(BETA));        // EXPECT: BETA
    printf("%s\n", NAMEOF(GAMMA));       // EXPECT: GAMMA

    /* Stringify various tokens */
    printf("%s\n", STR(if));             // EXPECT: if
    printf("%s\n", STR(while));          // EXPECT: while
    printf("%s\n", STR(return));         // EXPECT: return
    printf("%s\n", STR(int));            // EXPECT: int
    printf("%s\n", STR(void));           // EXPECT: void

    /* Stringify punctuation-free identifiers */
    printf("%s\n", STR(my_variable));    // EXPECT: my_variable
    printf("%s\n", STR(MyStruct));       // EXPECT: MyStruct
    printf("%s\n", STR(FLAG_ENABLED));   // EXPECT: FLAG_ENABLED

    /* Values are independent of stringify */
    printf("%d\n", ALPHA);               // EXPECT: 1
    printf("%d\n", BETA);                // EXPECT: 2
    printf("%d\n", GAMMA);               // EXPECT: 3

    /* Multi-use: print name and value */
    SHOW_VAR(ALPHA, ALPHA);              // EXPECT: ALPHA = 1
    SHOW_VAR(BETA, BETA);                // EXPECT: BETA = 2
    SHOW_VAR(GAMMA, GAMMA);              // EXPECT: GAMMA = 3

    return 0;
}
