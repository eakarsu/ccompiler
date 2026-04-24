int printf(const char *fmt, ...);

// Preprocessor torture: nested macros, ## token paste, # stringify,
// #if arithmetic, nested #ifdef, __LINE__ in macros.

// ---- Basic nested macros ----
#define A1 10
#define A2 (A1 + 5)
#define A3 (A2 * 2)
#define A4 (A3 + A1)
#define A5 (A4 - A2)

// A1=10, A2=15, A3=30, A4=40, A5=25

// ---- Deep macro nesting ----
#define INNER(x) ((x) * (x))
#define MID(x) INNER((x) + 1)
#define OUTER(x) MID(INNER(x))
#define DEEP(x) OUTER((x) + 2)
#define DEEPER(x) (DEEP(x) + DEEP((x) + 1))

// INNER(x) = x*x
// MID(x) = INNER(x+1) = (x+1)*(x+1)
// OUTER(x) = MID(INNER(x)) = MID(x*x) = (x*x+1)*(x*x+1)
// DEEP(x) = OUTER(x+2) = ((x+2)*(x+2)+1)*((x+2)*(x+2)+1)
// DEEP(3) = (5*5+1)*(5*5+1) = 26*26 = 676
// DEEP(4) = (6*6+1)*(6*6+1) = 37*37 = 1369
// DEEPER(3) = 676 + 1369 = 2045

// ---- Token pasting with ## ----
#define PASTE(a, b) a ## b
#define PASTE3(a, b, c) a ## b ## c

int func_1(void) { return 100; }
int func_2(void) { return 200; }
int func_3(void) { return 300; }

#define CALL_FUNC(n) PASTE(func_, n)()

// ---- Stringify with # ----
#define STRINGIFY(x) #x
#define XSTRINGIFY(x) STRINGIFY(x)

#define MY_CONST 42
// STRINGIFY(MY_CONST) => "MY_CONST"
// XSTRINGIFY(MY_CONST) => STRINGIFY(42) => "42"

// ---- Conditional compilation ----
#define FEATURE_A 1
#define FEATURE_B 0
#define VERSION 3

#if FEATURE_A
int feature_a_val = 100;
#else
int feature_a_val = 0;
#endif

#if FEATURE_B
int feature_b_val = 100;
#else
int feature_b_val = 0;
#endif

#if VERSION >= 3
int version_check = 1;
#elif VERSION >= 2
int version_check = 0;
#else
int version_check = -1;
#endif

// ---- Nested #ifdef ----
#define PLATFORM_ARM 1
#define HAS_NEON 1

#ifdef PLATFORM_ARM
  #ifdef HAS_NEON
    int platform_val = 30;
  #else
    int platform_val = 20;
  #endif
#else
  int platform_val = 10;
#endif

// ---- #if with arithmetic ----
#define SIZE_A 16
#define SIZE_B 32
#define TOTAL (SIZE_A + SIZE_B)

#if (SIZE_A * 2 == SIZE_B)
int size_check = 1;
#else
int size_check = 0;
#endif

#if TOTAL > 40
int total_check = 1;
#else
int total_check = 0;
#endif

// ---- Macro generating macro-like patterns ----
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define CLAMP(x, lo, hi) MIN(MAX((x), (lo)), (hi))
#define ABS(x) ((x) >= 0 ? (x) : -(x))

// ---- Macro with multiple statements via comma operator ----
#define SWAP_INT(a, b, tmp) ((tmp) = (a), (a) = (b), (b) = (tmp))

// ---- Recursive-like expansion (not actually recursive, but looks like it) ----
#define ADD1(x) ((x) + 1)
#define ADD2(x) ADD1(ADD1(x))
#define ADD4(x) ADD2(ADD2(x))
#define ADD8(x) ADD4(ADD4(x))
#define ADD16(x) ADD8(ADD8(x))

// ---- Variadic-like macros (C99) ----
#define LOG(fmt, ...) printf(fmt, __VA_ARGS__)

// ---- Guard-like pattern ----
#ifndef GUARD_INCLUDED
#define GUARD_INCLUDED 1
int guard_val = 999;
#endif

#ifndef GUARD_INCLUDED
int guard_val = 0;  // should NOT be included
#endif

// ---- Line number tracking ----
int get_approx_line(void) {
    return __LINE__;
}

// ---- Chained token paste for array indexing ----
int data_0 = 10;
int data_1 = 20;
int data_2 = 30;
int data_3 = 40;
int data_4 = 50;

#define DATA(n) PASTE(data_, n)

// ---- Conditional function selection ----
#define USE_FAST 1

int slow_compute(int x) { return x * 2; }
int fast_compute(int x) { return x * 2; }

#if USE_FAST
#define COMPUTE fast_compute
#else
#define COMPUTE slow_compute
#endif

// ---- Stringify of expression ----
#define SHOW_EXPR(expr) printf("%s = %d\n", #expr, (expr))

int main(void) {
    // Nested macro values
    printf("%d\n", A1);
    // EXPECT: 10
    printf("%d\n", A2);
    // EXPECT: 15
    printf("%d\n", A3);
    // EXPECT: 30
    printf("%d\n", A4);
    // EXPECT: 40
    printf("%d\n", A5);
    // EXPECT: 25

    // Deep nesting
    printf("%d\n", DEEPER(3));
    // EXPECT: 2045

    // Token pasting
    printf("%d\n", CALL_FUNC(1));
    // EXPECT: 100
    printf("%d\n", CALL_FUNC(2));
    // EXPECT: 200
    printf("%d\n", CALL_FUNC(3));
    // EXPECT: 300

    // Stringify
    printf("%s\n", STRINGIFY(MY_CONST));
    // EXPECT: MY_CONST
    printf("%s\n", XSTRINGIFY(MY_CONST));
    // EXPECT: 42

    // Conditional compilation
    printf("%d\n", feature_a_val);
    // EXPECT: 100
    printf("%d\n", feature_b_val);
    // EXPECT: 0
    printf("%d\n", version_check);
    // EXPECT: 1

    // Nested ifdef
    printf("%d\n", platform_val);
    // EXPECT: 30

    // #if arithmetic
    printf("%d\n", size_check);
    // EXPECT: 1
    printf("%d\n", total_check);
    // EXPECT: 1

    // MAX, MIN, CLAMP, ABS
    printf("%d\n", MAX(10, 20));
    // EXPECT: 20
    printf("%d\n", MIN(10, 20));
    // EXPECT: 10
    printf("%d\n", CLAMP(50, 0, 30));
    // EXPECT: 30
    printf("%d\n", CLAMP(-5, 0, 30));
    // EXPECT: 0
    printf("%d\n", ABS(-42));
    // EXPECT: 42

    // SWAP via macro
    int sa = 111, sb = 222, stmp;
    SWAP_INT(sa, sb, stmp);
    printf("%d %d\n", sa, sb);
    // EXPECT: 222 111

    // ADD16
    printf("%d\n", ADD16(0));
    // EXPECT: 16
    printf("%d\n", ADD16(100));
    // EXPECT: 116

    // Variadic LOG
    LOG("%d + %d = %d\n", 3, 4, 7);
    // EXPECT: 3 + 4 = 7

    // Guard value
    printf("%d\n", guard_val);
    // EXPECT: 999

    // __LINE__ should return some positive number
    int ln = get_approx_line();
    if (ln > 0) {
        printf("line ok\n");
    } else {
        printf("line fail\n");
    }
    // EXPECT: line ok

    // Data via token paste
    printf("%d\n", DATA(0) + DATA(1) + DATA(2) + DATA(3) + DATA(4));
    // EXPECT: 150

    // Conditional function
    printf("%d\n", COMPUTE(25));
    // EXPECT: 50

    // SHOW_EXPR
    SHOW_EXPR(10 + 20);
    // EXPECT: 10 + 20 = 30

    printf("ALL PASSED\n");
    // EXPECT: ALL PASSED
    return 0;
}
