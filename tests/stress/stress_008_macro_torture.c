// stress_008_macro_torture.c - Macro stress test
// EXPECT: deep_macro=32
// EXPECT: quint=32
// EXPECT: paste=606
// EXPECT: str=hello
// EXPECT: xstr=42
// EXPECT: fields=100
// EXPECT: clamp=50
// EXPECT: clamp_lo=0
// EXPECT: clamp_hi=100
// EXPECT: apply=30
// EXPECT: swap=222,111
// EXPECT: repeat=56
// EXPECT: poly2=34
// EXPECT: poly3=26
// EXPECT: enum=2
// EXPECT: features=30
// EXPECT: long_macro=15
// EXPECT: abs=42
int printf(const char *fmt, ...);

// --- Deeply nested macro expansion (5+ levels) ---
#define ADD1(x) ((x) + 1)
#define ADD2(x) ADD1(ADD1(x))
#define ADD4(x) ADD2(ADD2(x))
#define ADD8(x) ADD4(ADD4(x))
#define ADD16(x) ADD8(ADD8(x))
#define ADD32(x) ADD16(ADD16(x))

// --- Macros expanding to macros ---
#define SQUARE(x) ((x) * (x))
#define CUBE(x) ((x) * SQUARE(x))
#define QUAD(x) (SQUARE(x) * SQUARE(x))
#define QUINT(x) ((x) * QUAD(x))

// --- Token pasting creating identifiers ---
#define PASTE(a, b) a ## b
#define PASTE3(a, b, c) a ## b ## c
#define VAR(n) PASTE(var_, n)
#define FUNC(n) PASTE(func_, n)

int FUNC(alpha)(int x) { return x + 100; }
int FUNC(beta)(int x) { return x + 200; }
int FUNC(gamma)(int x) { return x + 300; }

// --- Stringification ---
#define STR(x) #x
#define XSTR(x) STR(x)
#define MYVAL 42
#define EXPR(a, b) ((a) + (b))

// --- Macro computing struct field names via ## ---
#define FIELD(s, name) s.field_ ## name
#define SET_FIELD(s, name, val) s.field_ ## name = val
#define GET_FIELD(s, name) s.field_ ## name

struct fields {
    int field_x;
    int field_y;
    int field_z;
    int field_w;
};

// --- Conditional compilation macros ---
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(x) ((x) >= 0 ? (x) : -(x))
#define CLAMP(x, lo, hi) MIN(MAX(x, lo), hi)

// --- Variadic-like macro patterns ---
#define APPLY1(f, a) f(a)
#define APPLY2(f, a, b) f(a) + f(b)
#define APPLY3(f, a, b, c) f(a) + f(b) + f(c)
#define APPLY4(f, a, b, c, d) f(a) + f(b) + f(c) + f(d)

// --- Multi-statement macro ---
#define SWAP_INT(a, b) do { int _t = (a); (a) = (b); (b) = _t; } while(0)

// --- Recursive-like expansion via multiple levels ---
#define REPEAT1(x) x
#define REPEAT2(x) REPEAT1(x), REPEAT1(x)
#define REPEAT4(x) REPEAT2(x), REPEAT2(x)
#define REPEAT8(x) REPEAT4(x), REPEAT4(x)

// --- Macro for array initialization ---
#define INIT_ARRAY(arr, n, val) do { int _i; for(_i = 0; _i < (n); _i++) arr[_i] = (val); } while(0)

// --- Complex nested macro expressions ---
#define POLY2(x, a, b, c) ((a) * SQUARE(x) + (b) * (x) + (c))
#define POLY3(x, a, b, c, d) ((a) * CUBE(x) + (b) * SQUARE(x) + (c) * (x) + (d))

// --- Token pasting for enum ---
#define ENUM_VAL(prefix, name) PASTE3(prefix, _, name)

enum colors {
    ENUM_VAL(COLOR, RED) = 0,
    ENUM_VAL(COLOR, GREEN) = 1,
    ENUM_VAL(COLOR, BLUE) = 2
};

// --- Ifdef chains ---
#define FEATURE_A 1
#define FEATURE_B 1

#ifdef FEATURE_A
#define A_VAL 10
#else
#define A_VAL 0
#endif

#ifdef FEATURE_B
#define B_VAL 20
#else
#define B_VAL 0
#endif

#define FEATURE_SUM (A_VAL + B_VAL)

// --- Line continuation test ---
#define LONG_MACRO(a, b, c, d, e) \
    ((a) + (b) + (c) + (d) + (e))

int main(void) {
    // Deep nesting: ADD32(0) = 32
    printf("deep_macro=%d\n", ADD32(0));

    // Chained macros: QUINT(2) = 2^5 = 32
    printf("quint=%d\n", QUINT(2));

    // Token pasting: call func_alpha, func_beta, func_gamma
    int r = FUNC(alpha)(1) + FUNC(beta)(2) + FUNC(gamma)(3);
    // 101 + 202 + 303 = 606
    printf("paste=%d\n", r);

    // Stringification
    printf("str=%s\n", STR(hello));
    printf("xstr=%s\n", XSTR(MYVAL));

    // Struct field via ##
    struct fields f;
    SET_FIELD(f, x, 10);
    SET_FIELD(f, y, 20);
    SET_FIELD(f, z, 30);
    SET_FIELD(f, w, 40);
    int fsum = GET_FIELD(f, x) + GET_FIELD(f, y) + GET_FIELD(f, z) + GET_FIELD(f, w);
    printf("fields=%d\n", fsum); // 100

    // CLAMP
    printf("clamp=%d\n", CLAMP(50, 0, 100));  // 50
    printf("clamp_lo=%d\n", CLAMP(-10, 0, 100)); // 0
    printf("clamp_hi=%d\n", CLAMP(200, 0, 100)); // 100

    // APPLY
    int asum = APPLY4(SQUARE, 1, 2, 3, 4);
    // 1+4+9+16 = 30
    printf("apply=%d\n", asum);

    // SWAP
    int sa = 111, sb = 222;
    SWAP_INT(sa, sb);
    printf("swap=%d,%d\n", sa, sb); // 222,111

    // REPEAT for array init
    int rarr[] = { REPEAT8(7) };
    int rsum = 0;
    int i;
    for (i = 0; i < 8; i++) rsum += rarr[i];
    printf("repeat=%d\n", rsum); // 7*8 = 56

    // Polynomial: POLY2(3, 2, 5, 1) = 2*9 + 5*3 + 1 = 18+15+1 = 34
    printf("poly2=%d\n", POLY2(3, 2, 5, 1));

    // POLY3(2, 1, 2, 3, 4) = 1*8 + 2*4 + 3*2 + 4 = 8+8+6+4 = 26
    printf("poly3=%d\n", POLY3(2, 1, 2, 3, 4));

    // Enum via token pasting
    printf("enum=%d\n", COLOR_BLUE); // 2

    // Feature sum
    printf("features=%d\n", FEATURE_SUM); // 30

    // Long macro with continuation
    printf("long_macro=%d\n", LONG_MACRO(1, 2, 3, 4, 5)); // 15

    // ABS
    printf("abs=%d\n", ABS(-42)); // 42

    return 0;
}
