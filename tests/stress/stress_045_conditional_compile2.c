// stress_045_conditional_compile2.c - Conditional compilation stress test
// EXPECT: feature_a=100
// EXPECT: feature_b=200
// EXPECT: nested_level=5
// EXPECT: toggled=999
// EXPECT: guard_result=42
// EXPECT: elif_chain=4
// EXPECT: multi_guard=77
// EXPECT: combo=300
// EXPECT: token_paste=55
// EXPECT: stringize=11
// EXPECT: recursive_macro=120
// EXPECT: variadic_count=3

int printf(const char *fmt, ...);

// --- Feature flags ---
#define FEATURE_A 1
#define FEATURE_B 1
#define FEATURE_C 0

// --- Nested #if/#elif/#else 5 levels ---
#define LEVEL1 1
#define LEVEL2 1
#define LEVEL3 1
#define LEVEL4 1
#define LEVEL5 1

int get_nested_level(void) {
#if LEVEL1
    #if LEVEL2
        #if LEVEL3
            #if LEVEL4
                #if LEVEL5
                    return 5;
                #else
                    return 4;
                #endif
            #else
                return 3;
            #endif
        #else
            return 2;
        #endif
    #else
        return 1;
    #endif
#else
    return 0;
#endif
}

// --- #ifdef guarding different implementations ---
#define USE_FAST_PATH

#ifdef USE_FAST_PATH
int compute(int x) {
    return x * x;
}
#else
int compute(int x) {
    int result = 0;
    int i;
    for (i = 0; i < x; i++)
        result = result + x;
    return result;
}
#endif

// --- #if 0 to disable large block ---
int skipped_check(void) {
#if 0
    // This entire block should be skipped by the preprocessor
    int x = 9999;
    int y = x * x * x;
    return y + x + 1;
    int z = y / 0;
    int w = undeclared_variable_name;
    return -1;
#endif
    return 42;
}

// --- #define toggling behavior ---
#define MODE 3

int get_mode_value(void) {
#if MODE == 1
    return 111;
#elif MODE == 2
    return 222;
#elif MODE == 3
    return 999;
#elif MODE == 4
    return 444;
#else
    return 0;
#endif
}

// --- #elif chain selecting specific value ---
#define SELECTOR 4

int elif_chain(void) {
#if SELECTOR == 1
    return 1;
#elif SELECTOR == 2
    return 2;
#elif SELECTOR == 3
    return 3;
#elif SELECTOR == 4
    return 4;
#elif SELECTOR == 5
    return 5;
#else
    return 0;
#endif
}

// --- Multiple guard macros ---
#define GUARD_X
#define GUARD_Y

int multi_guard(void) {
#ifdef GUARD_X
    #ifdef GUARD_Y
        return 77;
    #else
        return 11;
    #endif
#else
    #ifdef GUARD_Y
        return 22;
    #else
        return 33;
    #endif
#endif
}

// --- Combination of features ---
int combo(void) {
    int sum = 0;
#if FEATURE_A
    sum = sum + 100;
#endif
#if FEATURE_B
    sum = sum + 200;
#endif
#if FEATURE_C
    sum = sum + 400;
#endif
    return sum;
}

// --- Token pasting with ## ---
#define PASTE(a, b) a##b
#define MAKE_VAR(n) var_##n

int MAKE_VAR(alpha) = 55;

// --- Stringize with # ---
#define STRINGIFY(x) #x
#define XSTRINGIFY(x) STRINGIFY(x)

int count_chars(const char *s) {
    int n = 0;
    while (s[n] != 0) n++;
    return n;
}

// --- Recursive-like macro expansion ---
#define MUL2(x) ((x) * 2)
#define MUL4(x) MUL2(MUL2(x))
#define MUL8(x) MUL2(MUL4(x))

// --- Macro that conditionally expands ---
#define SAFE_DIV(a, b) ((b) != 0 ? (a) / (b) : 0)

// --- Variadic-like counting macro ---
// Count arguments by shifting: supports 1-5 args
#define COUNT_ARGS_IMPL(_1, _2, _3, _4, _5, N, ...) N
#define COUNT_ARGS(...) COUNT_ARGS_IMPL(__VA_ARGS__, 5, 4, 3, 2, 1)

// --- #undef and redefine ---
#define FLIPFLOP 10
#undef FLIPFLOP
#define FLIPFLOP 20

// --- Nested conditional with arithmetic-like expressions ---
#define VAL_A 10
#define VAL_B 20

#if VAL_A + VAL_B == 30
#define COMBINED 1
#else
#define COMBINED 0
#endif

#if VAL_A * 2 == VAL_B
#define DOUBLED 1
#else
#define DOUBLED 0
#endif

int main(void) {
    // Feature flags
#if FEATURE_A
    printf("feature_a=%d\n", 100);
#endif

#if FEATURE_B
    printf("feature_b=%d\n", 200);
#endif

    // Nested levels
    printf("nested_level=%d\n", get_nested_level());

    // Toggled mode
    printf("toggled=%d\n", get_mode_value());

    // Guard result
    printf("guard_result=%d\n", skipped_check());

    // Elif chain
    printf("elif_chain=%d\n", elif_chain());

    // Multi guard
    printf("multi_guard=%d\n", multi_guard());

    // Combo features
    printf("combo=%d\n", combo());

    // Token pasting
    printf("token_paste=%d\n", PASTE(var_, alpha));

    // Stringize: count chars of "hello world" = 11
    printf("stringize=%d\n", count_chars(STRINGIFY(hello world)));

    // Recursive macro: MUL8(15) = 15*8 = 120
    printf("recursive_macro=%d\n", MUL8(15));

    // Variadic count
    printf("variadic_count=%d\n", COUNT_ARGS(a, b, c));

    return 0;
}
