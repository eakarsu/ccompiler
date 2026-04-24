// stress_068_preprocessor_compute.c - Preprocessor computation stress test
// EXPECT: arith_add=15
// EXPECT: arith_sub=5
// EXPECT: arith_mul=50
// EXPECT: arith_div=2
// EXPECT: arith_mod=1
// EXPECT: arith_complex=42
// EXPECT: cmp_lt=1
// EXPECT: cmp_gt=0
// EXPECT: cmp_le=1
// EXPECT: cmp_ge=0
// EXPECT: cmp_eq=1
// EXPECT: cmp_ne=1
// EXPECT: logic_and=1
// EXPECT: logic_or=1
// EXPECT: logic_not=0
// EXPECT: logic_complex=1
// EXPECT: ternary_true=100
// EXPECT: ternary_false=200
// EXPECT: nested_ternary=3
// EXPECT: bitwise_and=3
// EXPECT: bitwise_or=7
// EXPECT: bitwise_xor=6
// EXPECT: bitwise_not=-6
// EXPECT: shift_left=32
// EXPECT: shift_right=2
// EXPECT: defined_yes=1
// EXPECT: defined_no=0
// EXPECT: defined_complex=1
// EXPECT: concat_var=42
// EXPECT: stringify=hello
// EXPECT: multi_level=999
// EXPECT: recursive_depth=10
// EXPECT: platform_ok=1
// EXPECT: size_check_int=1
// EXPECT: size_check_ptr=1
// EXPECT: ifdef_chain=correct
// EXPECT: token_paste_func=25
// EXPECT: variadic_count=3

int printf(const char *fmt, ...);

// --- Arithmetic in preprocessor ---
#define PP_A 10
#define PP_B 5

#if (PP_A + PP_B) == 15
#define ARITH_ADD 15
#else
#define ARITH_ADD 0
#endif

#if (PP_A - PP_B) == 5
#define ARITH_SUB 5
#else
#define ARITH_SUB 0
#endif

#if (PP_A * PP_B) == 50
#define ARITH_MUL 50
#else
#define ARITH_MUL 0
#endif

#if (PP_A / PP_B) == 2
#define ARITH_DIV 2
#else
#define ARITH_DIV 0
#endif

#if (PP_A % 3) == 1
#define ARITH_MOD 1
#else
#define ARITH_MOD 0
#endif

// Complex arithmetic
#if ((PP_A * 4) + (PP_B - 3)) == 42
#define ARITH_COMPLEX 42
#else
#define ARITH_COMPLEX 0
#endif

// --- Comparison operators ---
#if PP_B < PP_A
#define CMP_LT 1
#else
#define CMP_LT 0
#endif

#if PP_B > PP_A
#define CMP_GT 1
#else
#define CMP_GT 0
#endif

#if PP_B <= PP_B
#define CMP_LE 1
#else
#define CMP_LE 0
#endif

#if PP_A >= (PP_A + 1)
#define CMP_GE 1
#else
#define CMP_GE 0
#endif

#if PP_A == 10
#define CMP_EQ 1
#else
#define CMP_EQ 0
#endif

#if PP_A != PP_B
#define CMP_NE 1
#else
#define CMP_NE 0
#endif

// --- Logical operators ---
#if (PP_A > 0) && (PP_B > 0)
#define LOGIC_AND 1
#else
#define LOGIC_AND 0
#endif

#if (PP_A > 100) || (PP_B > 0)
#define LOGIC_OR 1
#else
#define LOGIC_OR 0
#endif

#if !(PP_A < 0)
#define LOGIC_NOT_VAL 0
#else
#define LOGIC_NOT_VAL 1
#endif

// Complex logical
#if ((PP_A > 5) && (PP_B < 10)) || (PP_A == 0)
#define LOGIC_COMPLEX 1
#else
#define LOGIC_COMPLEX 0
#endif

// --- Ternary in #if is not standard, use nested #if instead ---
#if PP_A > PP_B
#define TERNARY_TRUE 100
#define TERNARY_FALSE 200
#else
#define TERNARY_TRUE 200
#define TERNARY_FALSE 100
#endif

// Nested conditional
#if PP_A > 20
#define NESTED_TERNARY 1
#elif PP_A > 8
#define NESTED_TERNARY 3
#elif PP_A > 0
#define NESTED_TERNARY 2
#else
#define NESTED_TERNARY 0
#endif

// --- Bitwise operators in #if ---
#if (7 & 3) == 3
#define BW_AND 3
#else
#define BW_AND 0
#endif

#if (5 | 3) == 7
#define BW_OR 7
#else
#define BW_OR 0
#endif

#if (5 ^ 3) == 6
#define BW_XOR 6
#else
#define BW_XOR 0
#endif

// Bitwise NOT: ~5 in preprocessor (using two's complement)
#define BW_NOT_VAL (~5)

// --- Shift operators ---
#if (1 << 5) == 32
#define SHIFT_LEFT 32
#else
#define SHIFT_LEFT 0
#endif

#if (8 >> 2) == 2
#define SHIFT_RIGHT 2
#else
#define SHIFT_RIGHT 0
#endif

// --- defined() operator ---
#define FEATURE_X 1
// FEATURE_Y intentionally not defined

#if defined(FEATURE_X)
#define DEFINED_YES 1
#else
#define DEFINED_YES 0
#endif

#if defined(FEATURE_Y)
#define DEFINED_NO 1
#else
#define DEFINED_NO 0
#endif

// Complex defined expressions
#if defined(FEATURE_X) && !defined(FEATURE_Y)
#define DEFINED_COMPLEX 1
#else
#define DEFINED_COMPLEX 0
#endif

// --- Token pasting (##) ---
#define PASTE(a, b) a##b
#define MAKE_VAR(prefix, num) prefix##num

int PASTE(concat_, var) = 42;

// --- Stringification (#) ---
#define STRINGIFY(x) #x

// --- Multi-level macro expansion ---
#define LEVEL1 LEVEL2
#define LEVEL2 LEVEL3
#define LEVEL3 999

// --- Recursive-like depth via chaining ---
#define DEPTH_0 0
#define DEPTH_1 (DEPTH_0 + 1)
#define DEPTH_2 (DEPTH_1 + 1)
#define DEPTH_3 (DEPTH_2 + 1)
#define DEPTH_4 (DEPTH_3 + 1)
#define DEPTH_5 (DEPTH_4 + 1)
#define DEPTH_6 (DEPTH_5 + 1)
#define DEPTH_7 (DEPTH_6 + 1)
#define DEPTH_8 (DEPTH_7 + 1)
#define DEPTH_9 (DEPTH_8 + 1)
#define DEPTH_10 (DEPTH_9 + 1)

// --- Platform check via sizeof ---
// Can't use sizeof in #if, but can check at runtime and set via preprocessor guards
#define CHECK_PLATFORM 1

// --- #ifdef chain ---
#define OPT_A
#define OPT_B
// OPT_C not defined

#ifdef OPT_A
#ifdef OPT_B
#ifndef OPT_C
#define IFDEF_CHAIN_RESULT "correct"
#else
#define IFDEF_CHAIN_RESULT "wrong_c"
#endif
#else
#define IFDEF_CHAIN_RESULT "wrong_b"
#endif
#else
#define IFDEF_CHAIN_RESULT "wrong_a"
#endif

// --- Token paste for function names ---
#define MAKE_FUNC(name) func_##name
int MAKE_FUNC(square)(int x) { return x * x; }

// --- Variadic macro ---
#define COUNT_ARGS_IMPL(_1, _2, _3, _4, _5, N, ...) N
#define COUNT_ARGS(...) COUNT_ARGS_IMPL(__VA_ARGS__, 5, 4, 3, 2, 1)

// --- Comma in macro args ---
#define PAIR(a, b) ((a) + (b))

// --- Guard macro pattern ---
#ifndef GUARD_MACRO_H
#define GUARD_MACRO_H 1
#endif

// --- Conditional compilation nesting ---
#if 1
#if 1
#if 0
#define DEEP_NEST "wrong"
#else
#define DEEP_NEST "right"
#endif
#endif
#endif

// --- X-macro pattern ---
#define OPERATIONS \
    X(add, +) \
    X(sub, -) \
    X(mul, *)

int main(void) {
    printf("arith_add=%d\n", ARITH_ADD);
    printf("arith_sub=%d\n", ARITH_SUB);
    printf("arith_mul=%d\n", ARITH_MUL);
    printf("arith_div=%d\n", ARITH_DIV);
    printf("arith_mod=%d\n", ARITH_MOD);
    printf("arith_complex=%d\n", ARITH_COMPLEX);

    printf("cmp_lt=%d\n", CMP_LT);
    printf("cmp_gt=%d\n", CMP_GT);
    printf("cmp_le=%d\n", CMP_LE);
    printf("cmp_ge=%d\n", CMP_GE);
    printf("cmp_eq=%d\n", CMP_EQ);
    printf("cmp_ne=%d\n", CMP_NE);

    printf("logic_and=%d\n", LOGIC_AND);
    printf("logic_or=%d\n", LOGIC_OR);
    printf("logic_not=%d\n", LOGIC_NOT_VAL);
    printf("logic_complex=%d\n", LOGIC_COMPLEX);

    printf("ternary_true=%d\n", TERNARY_TRUE);
    printf("ternary_false=%d\n", TERNARY_FALSE);
    printf("nested_ternary=%d\n", NESTED_TERNARY);

    printf("bitwise_and=%d\n", BW_AND);
    printf("bitwise_or=%d\n", BW_OR);
    printf("bitwise_xor=%d\n", BW_XOR);
    printf("bitwise_not=%d\n", BW_NOT_VAL);
    printf("shift_left=%d\n", SHIFT_LEFT);
    printf("shift_right=%d\n", SHIFT_RIGHT);

    printf("defined_yes=%d\n", DEFINED_YES);
    printf("defined_no=%d\n", DEFINED_NO);
    printf("defined_complex=%d\n", DEFINED_COMPLEX);

    printf("concat_var=%d\n", concat_var);
    printf("stringify=%s\n", STRINGIFY(hello));
    printf("multi_level=%d\n", LEVEL1);
    printf("recursive_depth=%d\n", DEPTH_10);

    // Runtime sizeof checks (can't do in preprocessor)
    printf("platform_ok=%d\n", CHECK_PLATFORM);
    printf("size_check_int=%d\n", (int)(sizeof(int) >= 4));
    printf("size_check_ptr=%d\n", (int)(sizeof(void*) >= 4));

    printf("ifdef_chain=%s\n", IFDEF_CHAIN_RESULT);
    printf("token_paste_func=%d\n", MAKE_FUNC(square)(5));
    printf("variadic_count=%d\n", COUNT_ARGS(a, b, c));

    return 0;
}
