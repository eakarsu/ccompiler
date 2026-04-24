int printf(const char *fmt, ...);

/* Conditional compilation: #if, #ifdef, #ifndef, #elif, #else, #endif */
/* defined() operator, nested conditionals, arithmetic in #if */

#define FEATURE_A   1
#define FEATURE_B   0
#define VERSION     3
#define MAX_SIZE    64

/* Undefined intentionally: FEATURE_C is not defined */

#define PLATFORM    2
/* PLATFORM: 1=linux, 2=macos, 3=windows */

#define DEBUG       1
#define RELEASE     0

/* Arithmetic constants for #if */
#define MAJOR       2
#define MINOR       5
#define PATCH       1

int main(void) {
    /* #ifdef: defined macro */
#ifdef FEATURE_A
    printf("%d\n", 1);                   // EXPECT: 1
#else
    printf("%d\n", 0);
#endif

    /* #ifdef: undefined macro */
#ifdef FEATURE_C
    printf("%d\n", 99);
#else
    printf("%d\n", 2);                   // EXPECT: 2
#endif

    /* #ifndef: undefined macro (branch taken) */
#ifndef FEATURE_C
    printf("%d\n", 3);                   // EXPECT: 3
#else
    printf("%d\n", 99);
#endif

    /* #ifndef: defined macro (branch not taken) */
#ifndef FEATURE_A
    printf("%d\n", 99);
#else
    printf("%d\n", 4);                   // EXPECT: 4
#endif

    /* #if with value */
#if FEATURE_A
    printf("%d\n", 5);                   // EXPECT: 5
#endif

#if FEATURE_B
    printf("%d\n", 99);
#else
    printf("%d\n", 6);                   // EXPECT: 6
#endif

    /* #if with comparison */
#if VERSION == 3
    printf("%d\n", 7);                   // EXPECT: 7
#else
    printf("%d\n", 99);
#endif

#if VERSION > 5
    printf("%d\n", 99);
#elif VERSION > 2
    printf("%d\n", 8);                   // EXPECT: 8
#else
    printf("%d\n", 99);
#endif

    /* #if with arithmetic */
#if MAJOR * 10 + MINOR == 25
    printf("%d\n", 9);                   // EXPECT: 9
#else
    printf("%d\n", 99);
#endif
    /* MAJOR*10 + MINOR = 20+5 = 25 */

#if PATCH + MINOR == 6
    printf("%d\n", 10);                  // EXPECT: 10
#else
    printf("%d\n", 99);
#endif
    /* 1 + 5 = 6 */

    /* defined() operator */
#if defined(FEATURE_A)
    printf("%d\n", 11);                  // EXPECT: 11
#else
    printf("%d\n", 99);
#endif

#if defined(FEATURE_C)
    printf("%d\n", 99);
#else
    printf("%d\n", 12);                  // EXPECT: 12
#endif

    /* defined() with logical operators */
#if defined(FEATURE_A) && defined(FEATURE_B)
    printf("%d\n", 13);                  // EXPECT: 13
#else
    printf("%d\n", 99);
#endif

#if defined(FEATURE_A) && !defined(FEATURE_C)
    printf("%d\n", 14);                  // EXPECT: 14
#else
    printf("%d\n", 99);
#endif

#if defined(FEATURE_C) || defined(FEATURE_A)
    printf("%d\n", 15);                  // EXPECT: 15
#else
    printf("%d\n", 99);
#endif

    /* Nested conditionals */
#if PLATFORM == 2
#  if DEBUG
    printf("%d\n", 16);                  // EXPECT: 16
#  else
    printf("%d\n", 99);
#  endif
#else
    printf("%d\n", 99);
#endif

#if PLATFORM == 1
    printf("%d\n", 99);
#elif PLATFORM == 2
    printf("%d\n", 17);                  // EXPECT: 17
#elif PLATFORM == 3
    printf("%d\n", 99);
#else
    printf("%d\n", 99);
#endif

    /* Three-way #elif chain */
#if VERSION < 2
    printf("%d\n", 99);
#elif VERSION < 3
    printf("%d\n", 99);
#elif VERSION == 3
    printf("%d\n", 18);                  // EXPECT: 18
#else
    printf("%d\n", 99);
#endif

    /* #if with MAX_SIZE */
#if MAX_SIZE >= 32
    printf("%d\n", 19);                  // EXPECT: 19
#else
    printf("%d\n", 99);
#endif

#if MAX_SIZE / 8 == 8
    printf("%d\n", 20);                  // EXPECT: 20
#else
    printf("%d\n", 99);
#endif
    /* 64 / 8 = 8 */

    /* DEBUG/RELEASE macros */
#if DEBUG && !RELEASE
    printf("%d\n", 21);                  // EXPECT: 21
#else
    printf("%d\n", 99);
#endif

#if !DEBUG && RELEASE
    printf("%d\n", 99);
#else
    printf("%d\n", 22);                  // EXPECT: 22
#endif

    /* Nested #ifdef inside #if */
#if VERSION >= 3
#  ifdef FEATURE_A
    printf("%d\n", 23);                  // EXPECT: 23
#  endif
#endif

    /* Code block conditional */
    int val = 0;
#if FEATURE_A
    val = val + 100;
#endif
#if FEATURE_B
    val = val + 200;
#endif
#if !defined(FEATURE_C)
    val = val + 50;
#endif
    printf("%d\n", val);                 // EXPECT: 150
    /* 100 + 0 + 50 = 150 */

    return 0;
}
