int printf(const char *fmt, ...);

/* Include guard simulation, #ifndef checks, feature flags, platform detection */

/* Simulate include guard pattern */
#ifndef MY_HEADER_INCLUDED
#define MY_HEADER_INCLUDED
#define HEADER_VERSION  1
#endif

/* Feature flags */
#define FEATURE_LOG         1
#define FEATURE_ASSERT      1
/* FEATURE_NETWORK is intentionally not defined */
/* FEATURE_GPU is intentionally not defined */

/* Simulated platform detection */
#define PLATFORM_MACOS      1
/* PLATFORM_LINUX not defined */
/* PLATFORM_WINDOWS not defined */

/* Build type */
#define BUILD_DEBUG         1
/* BUILD_RELEASE not defined */

/* Capability macros derived from features */
#ifdef FEATURE_LOG
#  define LOG_ENABLED       1
#else
#  define LOG_ENABLED       0
#endif

#ifdef FEATURE_NETWORK
#  define NET_ENABLED       1
#else
#  define NET_ENABLED       0
#endif

/* Nested feature: assert depends on debug */
#if defined(FEATURE_ASSERT) && defined(BUILD_DEBUG)
#  define FULL_ASSERT       1
#else
#  define FULL_ASSERT       0
#endif

/* Platform string */
#ifdef PLATFORM_MACOS
#  define PLATFORM_NAME "macos"
#elif defined(PLATFORM_LINUX)
#  define PLATFORM_NAME "linux"
#elif defined(PLATFORM_WINDOWS)
#  define PLATFORM_NAME "windows"
#else
#  define PLATFORM_NAME "unknown"
#endif

/* Build-type string */
#ifdef BUILD_DEBUG
#  define BUILD_NAME "debug"
#else
#  define BUILD_NAME "release"
#endif

/* Conditional function-like macros */
#ifdef FEATURE_LOG
#  define DO_LOG(x) printf("LOG: %d\n", (x))
#else
#  define DO_LOG(x) ((void)0)
#endif

#ifdef FEATURE_NETWORK
#  define DO_NET(x) printf("NET: %d\n", (x))
#else
#  define DO_NET(x) printf("NET_OFF\n")
#endif

/* Version guard: define extras only if version is high enough */
#if HEADER_VERSION >= 1
#  define EXTRA_A   10
#  define EXTRA_B   20
#endif

#if HEADER_VERSION >= 2
#  define EXTRA_C   30
#endif

/* Compile-time constant selection */
#ifdef BUILD_DEBUG
#  define BUFFER_SIZE   64
#else
#  define BUFFER_SIZE   4096
#endif

#ifdef PLATFORM_MACOS
#  define PAGE_SIZE     4096
#else
#  define PAGE_SIZE     65536
#endif

int main(void) {
    /* Include guard: HEADER_VERSION should be 1 */
    printf("%d\n", HEADER_VERSION);      // EXPECT: 1

    /* Feature flag values */
    printf("%d\n", LOG_ENABLED);         // EXPECT: 1
    printf("%d\n", NET_ENABLED);         // EXPECT: 0

    /* FULL_ASSERT: both FEATURE_ASSERT and BUILD_DEBUG defined */
    printf("%d\n", FULL_ASSERT);         // EXPECT: 1

    /* Platform detection */
    printf("%s\n", PLATFORM_NAME);       // EXPECT: macos
    printf("%s\n", BUILD_NAME);          // EXPECT: debug

    /* Conditional log macro */
    DO_LOG(42);                          // EXPECT: LOG: 42
    DO_LOG(100);                         // EXPECT: LOG: 100

    /* Conditional net macro (feature off, prints NET_OFF) */
    DO_NET(0);                           // EXPECT: NET_OFF

    /* Version-gated extras */
    printf("%d\n", EXTRA_A);             // EXPECT: 10
    printf("%d\n", EXTRA_B);             // EXPECT: 20

    /* EXTRA_C not defined (version < 2) */
#ifdef EXTRA_C
    printf("%d\n", EXTRA_C);
#else
    printf("%d\n", 0);                   // EXPECT: 0
#endif

    /* Buffer/page sizes from platform+build */
    printf("%d\n", BUFFER_SIZE);         // EXPECT: 64
    printf("%d\n", PAGE_SIZE);           // EXPECT: 4096

    /* Simulated double-include guard: redefine attempt does nothing */
#ifndef MY_HEADER_INCLUDED
#  define HEADER_VERSION  99
#endif
    printf("%d\n", HEADER_VERSION);      // EXPECT: 1

    /* Nested #ifdef chain */
#ifdef PLATFORM_MACOS
#  ifdef BUILD_DEBUG
    printf("%d\n", 1);                   // EXPECT: 1
#  else
    printf("%d\n", 0);
#  endif
#else
    printf("%d\n", 0);
#endif

    /* #if with defined() and &&  */
#if defined(FEATURE_LOG) && defined(BUILD_DEBUG)
    printf("%d\n", 2);                   // EXPECT: 2
#else
    printf("%d\n", 0);
#endif

    /* #if with defined() and || */
#if defined(FEATURE_NETWORK) || defined(PLATFORM_MACOS)
    printf("%d\n", 3);                   // EXPECT: 3
#else
    printf("%d\n", 0);
#endif

    /* #if with ! and defined() */
#if !defined(FEATURE_GPU)
    printf("%d\n", 4);                   // EXPECT: 4
#else
    printf("%d\n", 0);
#endif

    /* Conditional computation */
    int val = 0;
#ifdef FEATURE_LOG
    val = val + 1;
#endif
#ifdef FEATURE_ASSERT
    val = val + 2;
#endif
#ifdef FEATURE_NETWORK
    val = val + 4;
#endif
#ifdef FEATURE_GPU
    val = val + 8;
#endif
    printf("%d\n", val);                 // EXPECT: 3
    /* LOG(1) + ASSERT(2) = 3 */

    /* Page-aligned computation */
    printf("%d\n", PAGE_SIZE / BUFFER_SIZE); // EXPECT: 64
    /* 4096 / 64 = 64 */

    return 0;
}
