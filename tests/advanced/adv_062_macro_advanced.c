// EXPECT: str_hello=hello
// EXPECT: str_42=42
// EXPECT: concat_ab=12
// EXPECT: var_x3=30
// EXPECT: ifdef_yes=1
// EXPECT: ifndef_yes=1
// EXPECT: elif_branch=2
// EXPECT: nested_if=10
// EXPECT: complex_cond=1
// EXPECT: defined_check=1
// EXPECT: guard_once=7
// EXPECT: multi_paste=100
// EXPECT: chain_str=world
// EXPECT: zero_or_one=1
// EXPECT: flag_combo=3
// EXPECT: undef_check=0
// EXPECT: redef_str=new
// EXPECT: all_passed=1

int printf(const char *fmt, ...);

// --- Stringification ---
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

// --- Token pasting ---
#define PASTE(a, b) a ## b
#define MAKE_VAR(prefix, num) prefix ## num

// --- Conditional compilation ---
#define FEATURE_A 1
#define FEATURE_B 0
#define PLATFORM_ARM 1

// --- Guard-like pattern ---
#ifndef GUARD_HEADER_H
#define GUARD_HEADER_H 7
#endif

// nested #ifdef
#ifdef FEATURE_A
  #define FEAT_A_VAL 1
#else
  #define FEAT_A_VAL 0
#endif

#ifndef FEATURE_C
  #define FEAT_C_MISSING 1
#else
  #define FEAT_C_MISSING 0
#endif

// #elif chain
#if FEATURE_B == 1
  #define ELIF_RESULT 1
#elif FEATURE_A == 1
  #define ELIF_RESULT 2
#else
  #define ELIF_RESULT 3
#endif

// Nested #if
#if PLATFORM_ARM
  #if FEATURE_A
    #define NESTED_IF_VAL 10
  #else
    #define NESTED_IF_VAL 20
  #endif
#else
  #define NESTED_IF_VAL 30
#endif

// Complex condition with defined()
#if defined(FEATURE_A) && !defined(FEATURE_C)
  #define COMPLEX_COND_VAL 1
#else
  #define COMPLEX_COND_VAL 0
#endif

// defined() as expression
#if defined(PLATFORM_ARM)
  #define DEFINED_CHECK_VAL 1
#else
  #define DEFINED_CHECK_VAL 0
#endif

// Multi-level pasting
#define HUNDRED_VAL 100
#define MULTI_PASTE PASTE(HUNDRED, _VAL)

// Chained stringify
#define WORLD world
#define CHAIN_STR STRINGIFY(world)

// Zero or one via conditional
#if FEATURE_A + PLATFORM_ARM > 1
  #define ZERO_OR_ONE 1
#else
  #define ZERO_OR_ONE 0
#endif

// Flag combos
#if FEATURE_A && PLATFORM_ARM
  #define FLAG_COMBO 3
#elif FEATURE_A
  #define FLAG_COMBO 1
#else
  #define FLAG_COMBO 0
#endif

// Undef and re-check
#define TEMP_DEF 999
#undef TEMP_DEF
#ifdef TEMP_DEF
  #define UNDEF_CHECK 1
#else
  #define UNDEF_CHECK 0
#endif

// Redefine stringify target
#define LABEL old
#undef LABEL
#define LABEL new
#define REDEF_STR STRINGIFY(new)

int main(void) {
    int passed = 1;

    // Stringification tests
    printf("str_hello=%s\n", STRINGIFY(hello));
    printf("str_42=%s\n", STRINGIFY(42));

    // Token pasting tests
    int ab = 12;
    printf("concat_ab=%d\n", PASTE(a, b));
    if (PASTE(a, b) != 12) passed = 0;

    int x3 = 30;
    printf("var_x3=%d\n", MAKE_VAR(x, 3));
    if (MAKE_VAR(x, 3) != 30) passed = 0;

    // Conditional compilation results
    printf("ifdef_yes=%d\n", FEAT_A_VAL);
    if (FEAT_A_VAL != 1) passed = 0;

    printf("ifndef_yes=%d\n", FEAT_C_MISSING);
    if (FEAT_C_MISSING != 1) passed = 0;

    printf("elif_branch=%d\n", ELIF_RESULT);
    if (ELIF_RESULT != 2) passed = 0;

    printf("nested_if=%d\n", NESTED_IF_VAL);
    if (NESTED_IF_VAL != 10) passed = 0;

    printf("complex_cond=%d\n", COMPLEX_COND_VAL);
    if (COMPLEX_COND_VAL != 1) passed = 0;

    printf("defined_check=%d\n", DEFINED_CHECK_VAL);
    if (DEFINED_CHECK_VAL != 1) passed = 0;

    printf("guard_once=%d\n", GUARD_HEADER_H);
    if (GUARD_HEADER_H != 7) passed = 0;

    printf("multi_paste=%d\n", MULTI_PASTE);
    if (MULTI_PASTE != 100) passed = 0;

    printf("chain_str=%s\n", CHAIN_STR);

    printf("zero_or_one=%d\n", ZERO_OR_ONE);
    if (ZERO_OR_ONE != 1) passed = 0;

    printf("flag_combo=%d\n", FLAG_COMBO);
    if (FLAG_COMBO != 3) passed = 0;

    printf("undef_check=%d\n", UNDEF_CHECK);
    if (UNDEF_CHECK != 0) passed = 0;

    printf("redef_str=%s\n", REDEF_STR);

    printf("all_passed=%d\n", passed);
    return passed ? 0 : 1;
}
