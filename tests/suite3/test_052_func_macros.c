int printf(const char *fmt, ...);

/* Function-like macros */

#define MAX(a, b)       ((a) > (b) ? (a) : (b))
#define MIN(a, b)       ((a) < (b) ? (a) : (b))
#define ABS(x)          ((x) < 0 ? -(x) : (x))
#define SQUARE(x)       ((x) * (x))
#define CUBE(x)         ((x) * (x) * (x))

/* Macro using parameter twice */
#define DOUBLE(x)       ((x) + (x))

/* Macro calling another macro */
#define MAX3(a, b, c)   (MAX(MAX(a, b), c))
#define MIN3(a, b, c)   (MIN(MIN(a, b), c))

/* Multiline macro using backslash continuation */
#define CLAMP(val, lo, hi) \
    ((val) < (lo) ? (lo) : ((val) > (hi) ? (hi) : (val)))

/* Macro that returns sum of squares */
#define SUM_SQ(a, b)    (SQUARE(a) + SQUARE(b))

/* Increment-by-step macro */
#define ADD(a, b)       ((a) + (b))
#define MUL(a, b)       ((a) * (b))

/* Nested macro: uses ADD and MUL */
#define FMADD(a, b, c)  (ADD(MUL(a, b), c))

/* Sign macro */
#define SIGN(x)         ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))

/* Power-of-2 check */
#define IS_EVEN(x)      ((x) % 2 == 0)

/* Divide safely: returns 0 if divisor is 0 */
#define SAFE_DIV(a, b)  ((b) != 0 ? (a) / (b) : 0)

/* Range macro: val in [lo, hi] */
#define IN_RANGE(val, lo, hi) ((val) >= (lo) && (val) <= (hi))

int main(void) {
    /* MAX tests */
    printf("%d\n", MAX(3, 7));           // EXPECT: 7
    printf("%d\n", MAX(7, 3));           // EXPECT: 7
    printf("%d\n", MAX(-1, -5));         // EXPECT: -1
    printf("%d\n", MAX(4, 4));           // EXPECT: 4

    /* MIN tests */
    printf("%d\n", MIN(3, 7));           // EXPECT: 3
    printf("%d\n", MIN(7, 3));           // EXPECT: 3
    printf("%d\n", MIN(-1, -5));         // EXPECT: -5
    printf("%d\n", MIN(4, 4));           // EXPECT: 4

    /* ABS tests */
    printf("%d\n", ABS(5));              // EXPECT: 5
    printf("%d\n", ABS(-5));             // EXPECT: 5
    printf("%d\n", ABS(0));              // EXPECT: 0

    /* SQUARE and CUBE */
    printf("%d\n", SQUARE(4));           // EXPECT: 16
    printf("%d\n", SQUARE(0));           // EXPECT: 0
    printf("%d\n", SQUARE(-3));          // EXPECT: 9
    printf("%d\n", CUBE(3));             // EXPECT: 27
    printf("%d\n", CUBE(-2));            // EXPECT: -8

    /* DOUBLE: parameter used twice */
    printf("%d\n", DOUBLE(6));           // EXPECT: 12
    printf("%d\n", DOUBLE(0));           // EXPECT: 0
    printf("%d\n", DOUBLE(-4));          // EXPECT: -8

    /* Macro calling macro */
    printf("%d\n", MAX3(1, 5, 3));       // EXPECT: 5
    printf("%d\n", MAX3(9, 2, 7));       // EXPECT: 9
    printf("%d\n", MAX3(4, 4, 4));       // EXPECT: 4
    printf("%d\n", MIN3(1, 5, 3));       // EXPECT: 1
    printf("%d\n", MIN3(9, 2, 7));       // EXPECT: 2

    /* CLAMP multiline macro */
    printf("%d\n", CLAMP(5, 0, 10));     // EXPECT: 5
    printf("%d\n", CLAMP(-3, 0, 10));    // EXPECT: 0
    printf("%d\n", CLAMP(15, 0, 10));    // EXPECT: 10
    printf("%d\n", CLAMP(0, 0, 10));     // EXPECT: 0
    printf("%d\n", CLAMP(10, 0, 10));    // EXPECT: 10

    /* SUM_SQ: macro calling macro */
    printf("%d\n", SUM_SQ(3, 4));        // EXPECT: 25
    /* 9 + 16 = 25 */
    printf("%d\n", SUM_SQ(0, 5));        // EXPECT: 25
    printf("%d\n", SUM_SQ(1, 1));        // EXPECT: 2

    /* FMADD: nested macros */
    printf("%d\n", FMADD(2, 3, 4));      // EXPECT: 10
    /* 2*3 + 4 = 10 */
    printf("%d\n", FMADD(5, 5, 0));      // EXPECT: 25
    printf("%d\n", FMADD(1, 1, 1));      // EXPECT: 2

    /* SIGN macro */
    printf("%d\n", SIGN(10));            // EXPECT: 1
    printf("%d\n", SIGN(-10));           // EXPECT: -1
    printf("%d\n", SIGN(0));             // EXPECT: 0

    /* IS_EVEN */
    printf("%d\n", IS_EVEN(4));          // EXPECT: 1
    printf("%d\n", IS_EVEN(7));          // EXPECT: 0
    printf("%d\n", IS_EVEN(0));          // EXPECT: 1

    /* SAFE_DIV */
    printf("%d\n", SAFE_DIV(10, 2));     // EXPECT: 5
    printf("%d\n", SAFE_DIV(10, 0));     // EXPECT: 0
    printf("%d\n", SAFE_DIV(0, 5));      // EXPECT: 0

    /* IN_RANGE */
    printf("%d\n", IN_RANGE(5, 1, 10));  // EXPECT: 1
    printf("%d\n", IN_RANGE(0, 1, 10));  // EXPECT: 0
    printf("%d\n", IN_RANGE(10, 1, 10)); // EXPECT: 1
    printf("%d\n", IN_RANGE(11, 1, 10)); // EXPECT: 0

    /* Macros with variable arguments */
    int a = 8, b = 3;
    printf("%d\n", MAX(a, b));           // EXPECT: 8
    printf("%d\n", MIN(a, b));           // EXPECT: 3
    printf("%d\n", ABS(a - b - 10));     // EXPECT: 5
    /* a-b-10 = 8-3-10 = -5, ABS(-5) = 5 */

    /* Combine macros */
    printf("%d\n", MAX(SQUARE(2), CUBE(2))); // EXPECT: 8
    /* SQUARE(2)=4, CUBE(2)=8, MAX(4,8)=8 */

    printf("%d\n", CLAMP(SQUARE(4), 0, 10)); // EXPECT: 10
    /* SQUARE(4)=16, CLAMP(16,0,10)=10 */

    return 0;
}
