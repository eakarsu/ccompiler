// EXPECT: platform=arm64
// EXPECT: debug_off=1
// EXPECT: opt_level=2
// EXPECT: feat_network=1
// EXPECT: feat_audio=0
// EXPECT: api_version=5
// EXPECT: combo_result=11
// EXPECT: triple_elif=third
// EXPECT: deep_nest=42
// EXPECT: arith_cond=1
// EXPECT: logic_cond=1
// EXPECT: paren_cond=1
// EXPECT: ternary_sim=100
// EXPECT: undef_guard=999
// EXPECT: version_ge=1
// EXPECT: multi_feat=both
// EXPECT: bit_check=1
// EXPECT: fallthrough=5
// EXPECT: final_config=168
// EXPECT: all_passed=1

int printf(const char *fmt, ...);

// Platform selection
#define TARGET_ARM64 1

#ifdef TARGET_ARM64
  #define PLATFORM_STR "arm64"
#elif defined(TARGET_X86)
  #define PLATFORM_STR "x86"
#else
  #define PLATFORM_STR "unknown"
#endif

// Debug flag (not defined = release)
#ifdef DEBUG
  #define DEBUG_OFF 0
#else
  #define DEBUG_OFF 1
#endif

// Optimization level selection
#define OPT_LEVEL 2

#if OPT_LEVEL == 0
  #define OPT_STR "none"
  #define OPT_VAL 0
#elif OPT_LEVEL == 1
  #define OPT_STR "basic"
  #define OPT_VAL 1
#elif OPT_LEVEL == 2
  #define OPT_STR "full"
  #define OPT_VAL 2
#else
  #define OPT_STR "aggressive"
  #define OPT_VAL 3
#endif

// Feature flags
#define HAS_NETWORK 1
#define HAS_GRAPHICS 1

#ifdef HAS_NETWORK
  #define FEAT_NETWORK 1
#else
  #define FEAT_NETWORK 0
#endif

#ifndef HAS_AUDIO
  #define FEAT_AUDIO 0
#else
  #define FEAT_AUDIO 1
#endif

// API version with #if arithmetic
#define API_MAJOR 1
#define API_MINOR 3
#define API_VERSION (API_MAJOR * 2 + API_MINOR)

// Combination of conditions
#if defined(HAS_NETWORK) && OPT_LEVEL >= 2
  #define COMBO_RESULT 11
#else
  #define COMBO_RESULT 0
#endif

// Triple #elif
#define MODE 3
#if MODE == 1
  #define TRIPLE_STR "first"
#elif MODE == 2
  #define TRIPLE_STR "second"
#elif MODE == 3
  #define TRIPLE_STR "third"
#else
  #define TRIPLE_STR "other"
#endif

// Deep nesting
#define LAYER1 1
#define LAYER2 1
#define LAYER3 1
#if LAYER1
  #if LAYER2
    #if LAYER3
      #define DEEP_NEST_VAL 42
    #else
      #define DEEP_NEST_VAL 0
    #endif
  #else
    #define DEEP_NEST_VAL 0
  #endif
#else
  #define DEEP_NEST_VAL 0
#endif

// Arithmetic in #if
#if (10 + 20) == 30
  #define ARITH_COND 1
#else
  #define ARITH_COND 0
#endif

// Logical operators in #if
#if 1 && !0
  #define LOGIC_COND 1
#else
  #define LOGIC_COND 0
#endif

// Parenthesized conditions
#if (defined(TARGET_ARM64) || defined(TARGET_X86)) && OPT_LEVEL > 0
  #define PAREN_COND 1
#else
  #define PAREN_COND 0
#endif

// Simulated ternary via #if
#if OPT_LEVEL >= 2
  #define TERNARY_SIM 100
#else
  #define TERNARY_SIM 50
#endif

// Undef then guard
#define TEMP_GUARD 888
#undef TEMP_GUARD
#ifndef TEMP_GUARD
  #define UNDEF_GUARD 999
#else
  #define UNDEF_GUARD 0
#endif

// Version comparison
#define MIN_VERSION 2
#if API_VERSION >= MIN_VERSION
  #define VERSION_GE 1
#else
  #define VERSION_GE 0
#endif

// Multi-feature string
#if defined(HAS_NETWORK) && defined(HAS_GRAPHICS)
  #define MULTI_FEAT_STR "both"
#elif defined(HAS_NETWORK)
  #define MULTI_FEAT_STR "net_only"
#else
  #define MULTI_FEAT_STR "none"
#endif

// Bitwise in #if
#if (0xFF & 0x01) == 1
  #define BIT_CHECK 1
#else
  #define BIT_CHECK 0
#endif

// Fallthrough: define in else, use later
#if 0
  #define FALL_VAL 10
#else
  #define FALL_VAL 5
#endif

// Final config: combine everything
#define FINAL_CONFIG (DEEP_NEST_VAL + COMBO_RESULT + API_VERSION + OPT_VAL + FEAT_NETWORK + FEAT_AUDIO + FALL_VAL + BIT_CHECK + ARITH_COND + LOGIC_COND + PAREN_COND + TERNARY_SIM - VERSION_GE - DEBUG_OFF)

int main(void) {
    int passed = 1;

    printf("platform=%s\n", PLATFORM_STR);
    printf("debug_off=%d\n", DEBUG_OFF);
    if (DEBUG_OFF != 1) passed = 0;

    printf("opt_level=%d\n", OPT_VAL);
    if (OPT_VAL != 2) passed = 0;

    printf("feat_network=%d\n", FEAT_NETWORK);
    if (FEAT_NETWORK != 1) passed = 0;

    printf("feat_audio=%d\n", FEAT_AUDIO);
    if (FEAT_AUDIO != 0) passed = 0;

    printf("api_version=%d\n", API_VERSION);
    if (API_VERSION != 5) passed = 0;

    printf("combo_result=%d\n", COMBO_RESULT);
    if (COMBO_RESULT != 11) passed = 0;

    printf("triple_elif=%s\n", TRIPLE_STR);

    printf("deep_nest=%d\n", DEEP_NEST_VAL);
    if (DEEP_NEST_VAL != 42) passed = 0;

    printf("arith_cond=%d\n", ARITH_COND);
    if (ARITH_COND != 1) passed = 0;

    printf("logic_cond=%d\n", LOGIC_COND);
    if (LOGIC_COND != 1) passed = 0;

    printf("paren_cond=%d\n", PAREN_COND);
    if (PAREN_COND != 1) passed = 0;

    printf("ternary_sim=%d\n", TERNARY_SIM);
    if (TERNARY_SIM != 100) passed = 0;

    printf("undef_guard=%d\n", UNDEF_GUARD);
    if (UNDEF_GUARD != 999) passed = 0;

    printf("version_ge=%d\n", VERSION_GE);
    if (VERSION_GE != 1) passed = 0;

    printf("multi_feat=%s\n", MULTI_FEAT_STR);

    printf("bit_check=%d\n", BIT_CHECK);
    if (BIT_CHECK != 1) passed = 0;

    printf("fallthrough=%d\n", FALL_VAL);
    if (FALL_VAL != 5) passed = 0;

    // 42 + 11 + 3 + 2 + 1 + 0 + 5 + 1 + 1 + 1 + 1 + 100 - 1 - 1 = 166... let me recalculate
    // Actually just print it
    printf("final_config=%d\n", FINAL_CONFIG);

    printf("all_passed=%d\n", passed);
    return passed ? 0 : 1;
}
