int printf(const char *fmt, ...);

/* Object-like macros: constants, expressions, nested definitions */

#define ZERO        0
#define ONE         1
#define TEN         10
#define HUNDRED     100

/* Nested: B defined first, A defined as B */
#define B           10
#define A           B

/* Macros used in arithmetic expressions */
#define BASE        5
#define STEP        3

/* Macro for array size */
#define ARRSIZE     8

/* Macros that build on each other */
#define DOUBLE_TEN  (TEN * 2)
#define TRIPLE_TEN  (TEN * 3)
#define SUM_TENS    (DOUBLE_TEN + TRIPLE_TEN)

/* Large constant macros */
#define KILO        1000
#define MEGA        (KILO * KILO)

/* Boolean-style macros */
#define TRUE_VAL    1
#define FALSE_VAL   0

/* Negative constant macro */
#define NEG_FIVE    (-5)

/* Offset macros */
#define OFFSET_A    7
#define OFFSET_B    13
#define TOTAL_OFF   (OFFSET_A + OFFSET_B)

int main(void) {
    /* Basic constant macros */
    printf("%d\n", ZERO);               // EXPECT: 0
    printf("%d\n", ONE);                // EXPECT: 1
    printf("%d\n", TEN);                // EXPECT: 10
    printf("%d\n", HUNDRED);            // EXPECT: 100

    /* Nested object macros: A -> B -> 10 */
    printf("%d\n", A);                  // EXPECT: 10
    printf("%d\n", B);                  // EXPECT: 10
    printf("%d\n", A + B);              // EXPECT: 20

    /* Macros in expressions */
    printf("%d\n", BASE + STEP);        // EXPECT: 8
    printf("%d\n", BASE * STEP);        // EXPECT: 15
    printf("%d\n", BASE - STEP);        // EXPECT: 2
    printf("%d\n", HUNDRED / TEN);      // EXPECT: 10

    /* Macro for array size */
    int arr[ARRSIZE];
    int i;
    for (i = 0; i < ARRSIZE; i++) {
        arr[i] = i * 2;
    }
    printf("%d\n", arr[0]);             // EXPECT: 0
    printf("%d\n", arr[3]);             // EXPECT: 6
    printf("%d\n", arr[7]);             // EXPECT: 14
    printf("%d\n", ARRSIZE);            // EXPECT: 8

    /* Compound macro expressions */
    printf("%d\n", DOUBLE_TEN);         // EXPECT: 20
    printf("%d\n", TRIPLE_TEN);         // EXPECT: 30
    printf("%d\n", SUM_TENS);           // EXPECT: 50

    /* Large constant macros */
    printf("%d\n", KILO);               // EXPECT: 1000
    printf("%d\n", MEGA);               // EXPECT: 1000000

    /* Boolean-style macros */
    printf("%d\n", TRUE_VAL);           // EXPECT: 1
    printf("%d\n", FALSE_VAL);          // EXPECT: 0
    printf("%d\n", TRUE_VAL + FALSE_VAL); // EXPECT: 1

    /* Negative macro */
    printf("%d\n", NEG_FIVE);           // EXPECT: -5
    printf("%d\n", TEN + NEG_FIVE);     // EXPECT: 5
    printf("%d\n", NEG_FIVE * TEN);     // EXPECT: -50

    /* Offset macros */
    printf("%d\n", OFFSET_A);          // EXPECT: 7
    printf("%d\n", OFFSET_B);          // EXPECT: 13
    printf("%d\n", TOTAL_OFF);         // EXPECT: 20

    /* Macros used in variable initialization */
    int x = TEN * BASE;
    printf("%d\n", x);                  // EXPECT: 50

    int y = HUNDRED - DOUBLE_TEN - TRIPLE_TEN;
    printf("%d\n", y);                  // EXPECT: 50

    /* Macros in conditionals */
    int result = 0;
    if (A == TEN) {
        result = result + 1;
    }
    if (B == TEN) {
        result = result + 1;
    }
    printf("%d\n", result);             // EXPECT: 2

    /* Macros in loop bounds */
    int sum = 0;
    for (i = ONE; i <= TEN; i++) {
        sum = sum + i;
    }
    printf("%d\n", sum);                // EXPECT: 55

    /* Macro in switch-like expression */
    int val = STEP * BASE;
    printf("%d\n", val);                // EXPECT: 15
    printf("%d\n", val + HUNDRED);      // EXPECT: 115

    /* Macro reuse in complex expression */
    printf("%d\n", KILO / HUNDRED);     // EXPECT: 10
    printf("%d\n", MEGA / KILO);        // EXPECT: 1000

    /* Chained macro arithmetic */
    printf("%d\n", A * B + HUNDRED);    // EXPECT: 200
    /* 10 * 10 + 100 = 200 */

    printf("%d\n", SUM_TENS * ONE);     // EXPECT: 50
    printf("%d\n", ZERO * MEGA);        // EXPECT: 0

    return 0;
}
