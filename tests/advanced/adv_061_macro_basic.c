
// EXPECT: PI_INT=3
// EXPECT: HUNDRED=100
// EXPECT: SQ(5)=25
// EXPECT: SQ(3+2)=25
// EXPECT: MAX(10,20)=20
// EXPECT: MAX(99,1)=99
// EXPECT: CUBE(3)=27
// EXPECT: ADD(10,20)=30
// EXPECT: DOUBLE(7)=14
// EXPECT: AREA(4,5)=20
// EXPECT: MULTI=600
// EXPECT: nested=50
// EXPECT: chained=200
// EXPECT: redef=42
// EXPECT: line_check=1
// EXPECT: guard_ok=1
// EXPECT: combo=35
// EXPECT: expr_macro=17
// EXPECT: shift_macro=32
// EXPECT: all_passed=1
int printf(const char *fmt, ...);

// Simple constant macros
#define PI_INT 3
#define HUNDRED 100
#define MAGIC 42

// Parameterized macros with parenthesization
#define SQ(x) ((x) * (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define CUBE(x) ((x) * (x) * (x))
#define ADD(a, b) ((a) + (b))
#define DOUBLE(x) ((x) + (x))
#define AREA(w, h) ((w) * (h))

// Multi-token / multi-expression macro
#define MULTI (10 * 20 * 3)

// Nested macro usage
#define HALF(x) ((x) / 2)
#define NESTED_VAL HALF(HUNDRED)

// Chained macros
#define BASE 50
#define SCALE 4
#define CHAINED (BASE * SCALE)

// Redefine macro (later definition wins)
#define REDEF_VAL 99
#undef REDEF_VAL
#define REDEF_VAL MAGIC

// __LINE__ usage
#define CAPTURE_LINE __LINE__

// Guard-like pattern
#define GUARD_FLAG 1

// Combination macro
#define COMBO(a, b, c) ((a) + (b) * (c))

// Expression macros
#define EXPR_MACRO(x) ((x) * 2 + 1)
#define SHIFT_MACRO(x, n) ((x) << (n))

int main(void) {
    int passed = 1;

    // Test simple constants
    printf("PI_INT=%d\n", PI_INT);
    if (PI_INT != 3) passed = 0;

    printf("HUNDRED=%d\n", HUNDRED);
    if (HUNDRED != 100) passed = 0;

    // Test parameterized macros
    printf("SQ(5)=%d\n", SQ(5));
    if (SQ(5) != 25) passed = 0;

    // SQ with expression argument - parenthesization matters
    printf("SQ(3+2)=%d\n", SQ(3 + 2));
    if (SQ(3 + 2) != 25) passed = 0;

    printf("MAX(10,20)=%d\n", MAX(10, 20));
    if (MAX(10, 20) != 20) passed = 0;

    printf("MAX(99,1)=%d\n", MAX(99, 1));
    if (MAX(99, 1) != 99) passed = 0;

    printf("CUBE(3)=%d\n", CUBE(3));
    if (CUBE(3) != 27) passed = 0;

    printf("ADD(10,20)=%d\n", ADD(10, 20));
    if (ADD(10, 20) != 30) passed = 0;

    printf("DOUBLE(7)=%d\n", DOUBLE(7));
    if (DOUBLE(7) != 14) passed = 0;

    printf("AREA(4,5)=%d\n", AREA(4, 5));
    if (AREA(4, 5) != 20) passed = 0;

    // Multi-token macro
    printf("MULTI=%d\n", MULTI);
    if (MULTI != 600) passed = 0;

    // Nested macros
    printf("nested=%d\n", NESTED_VAL);
    if (NESTED_VAL != 50) passed = 0;

    // Chained macros
    printf("chained=%d\n", CHAINED);
    if (CHAINED != 200) passed = 0;

    // Redefined macro
    printf("redef=%d\n", REDEF_VAL);
    if (REDEF_VAL != 42) passed = 0;

    // __LINE__ - just check it is positive
    int ln = CAPTURE_LINE;
    printf("line_check=%d\n", ln > 0 ? 1 : 0);
    if (ln <= 0) passed = 0;

    // Guard flag
    printf("guard_ok=%d\n", GUARD_FLAG);
    if (GUARD_FLAG != 1) passed = 0;

    // Combination macro
    printf("combo=%d\n", COMBO(5, 6, 5));
    if (COMBO(5, 6, 5) != 35) passed = 0;

    // Expression macros
    printf("expr_macro=%d\n", EXPR_MACRO(8));
    if (EXPR_MACRO(8) != 17) passed = 0;

    printf("shift_macro=%d\n", SHIFT_MACRO(1, 5));
    if (SHIFT_MACRO(1, 5) != 32) passed = 0;

    printf("all_passed=%d\n", passed);
    return passed ? 0 : 1;
}
