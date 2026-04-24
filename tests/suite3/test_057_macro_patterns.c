int printf(const char *fmt, ...);

/* Common macro patterns: MIN/MAX, SWAP, ARRAY_SIZE, STRINGIFY, container_of-like */

/* MIN/MAX with type safety via ternary */
#define MAX(a, b)           ((a) > (b) ? (a) : (b))
#define MIN(a, b)           ((a) < (b) ? (a) : (b))

/* SWAP using XOR for integers */
#define SWAP_INT(a, b)      do { (a) ^= (b); (b) ^= (a); (a) ^= (b); } while(0)

/* SWAP using temp variable approach (via comma expr) */
#define SWAP_TMP(type, a, b) do { type _tmp = (a); (a) = (b); (b) = _tmp; } while(0)

/* ARRAY_SIZE: number of elements in a static array */
#define ARRAY_SIZE(arr)     (sizeof(arr) / sizeof((arr)[0]))

/* STRINGIFY */
#define STR(x)              #x
#define XSTR(x)             STR(x)

/* Clamp */
#define CLAMP(x, lo, hi)    ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))

/* Generic dispatch macro: call function based on type code */
#define DISPATCH(code, a, b) \
    ((code) == 0 ? (a) + (b) : \
     (code) == 1 ? (a) - (b) : \
     (code) == 2 ? (a) * (b) : \
                   (a) / (b))

/* BIT operations */
#define SET_BIT(val, n)     ((val) | (1 << (n)))
#define CLR_BIT(val, n)     ((val) & ~(1 << (n)))
#define TST_BIT(val, n)     (((val) >> (n)) & 1)

/* ALIGN_UP: round x up to multiple of align (power of 2) */
#define ALIGN_UP(x, align)  (((x) + (align) - 1) & ~((align) - 1))

/* IS_POWER_OF_2 */
#define IS_POW2(x)          ((x) > 0 && ((x) & ((x) - 1)) == 0)

/* DIV_ROUND_UP */
#define DIV_CEIL(a, b)      (((a) + (b) - 1) / (b))

/* container_of-like: offsetof simulation */
#define OFFSETOF(type, member) ((int)&((type*)0)->member)

struct Point { int x; int y; };
struct Rect  { int left; int top; int right; int bottom; };

int main(void) {
    /* MIN / MAX */
    printf("%d\n", MAX(10, 20));         // EXPECT: 20
    printf("%d\n", MIN(10, 20));         // EXPECT: 10
    printf("%d\n", MAX(-5, -3));         // EXPECT: -3
    printf("%d\n", MIN(-5, -3));         // EXPECT: -5
    printf("%d\n", MAX(7, 7));           // EXPECT: 7

    /* SWAP_INT using XOR */
    int a = 15, b = 25;
    SWAP_INT(a, b);
    printf("%d\n", a);                   // EXPECT: 25
    printf("%d\n", b);                   // EXPECT: 15

    /* SWAP_TMP using temp var */
    int p = 100, q = 200;
    SWAP_TMP(int, p, q);
    printf("%d\n", p);                   // EXPECT: 200
    printf("%d\n", q);                   // EXPECT: 100

    /* ARRAY_SIZE */
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[] = {10, 20, 30};
    char str[] = "hello";
    printf("%d\n", (int)ARRAY_SIZE(arr1)); // EXPECT: 5
    printf("%d\n", (int)ARRAY_SIZE(arr2)); // EXPECT: 3
    /* str includes null terminator: "hello" = 5 chars + '\0' = 6 */
    printf("%d\n", (int)ARRAY_SIZE(str));  // EXPECT: 6

    /* CLAMP */
    printf("%d\n", CLAMP(5, 0, 10));     // EXPECT: 5
    printf("%d\n", CLAMP(-1, 0, 10));    // EXPECT: 0
    printf("%d\n", CLAMP(11, 0, 10));    // EXPECT: 10

    /* DISPATCH generic macro */
    printf("%d\n", DISPATCH(0, 10, 3));  // EXPECT: 13
    printf("%d\n", DISPATCH(1, 10, 3));  // EXPECT: 7
    printf("%d\n", DISPATCH(2, 10, 3));  // EXPECT: 30
    printf("%d\n", DISPATCH(3, 10, 2));  // EXPECT: 5

    /* SET_BIT / CLR_BIT / TST_BIT */
    int flags = 0;
    flags = SET_BIT(flags, 0);
    flags = SET_BIT(flags, 3);
    printf("%d\n", flags);               // EXPECT: 9
    /* bit 0 = 1, bit 3 = 8, total = 9 */
    printf("%d\n", TST_BIT(flags, 0));   // EXPECT: 1
    printf("%d\n", TST_BIT(flags, 1));   // EXPECT: 0
    printf("%d\n", TST_BIT(flags, 3));   // EXPECT: 1
    flags = CLR_BIT(flags, 0);
    printf("%d\n", flags);               // EXPECT: 8
    printf("%d\n", TST_BIT(flags, 0));   // EXPECT: 0

    /* ALIGN_UP */
    printf("%d\n", ALIGN_UP(5, 4));      // EXPECT: 8
    printf("%d\n", ALIGN_UP(8, 4));      // EXPECT: 8
    printf("%d\n", ALIGN_UP(9, 8));      // EXPECT: 16
    printf("%d\n", ALIGN_UP(1, 16));     // EXPECT: 16
    printf("%d\n", ALIGN_UP(16, 16));    // EXPECT: 16

    /* IS_POW2 */
    printf("%d\n", IS_POW2(1));          // EXPECT: 1
    printf("%d\n", IS_POW2(2));          // EXPECT: 1
    printf("%d\n", IS_POW2(4));          // EXPECT: 1
    printf("%d\n", IS_POW2(8));          // EXPECT: 1
    printf("%d\n", IS_POW2(3));          // EXPECT: 0
    printf("%d\n", IS_POW2(6));          // EXPECT: 0
    printf("%d\n", IS_POW2(0));          // EXPECT: 0

    /* DIV_CEIL */
    printf("%d\n", DIV_CEIL(10, 3));     // EXPECT: 4
    /* (10+3-1)/3 = 12/3 = 4 */
    printf("%d\n", DIV_CEIL(9, 3));      // EXPECT: 3
    printf("%d\n", DIV_CEIL(1, 4));      // EXPECT: 1
    printf("%d\n", DIV_CEIL(7, 4));      // EXPECT: 2

    /* OFFSETOF: layout check for Point */
    printf("%d\n", OFFSETOF(struct Point, x));  // EXPECT: 0
    printf("%d\n", OFFSETOF(struct Point, y));  // EXPECT: 4

    /* OFFSETOF for Rect */
    printf("%d\n", OFFSETOF(struct Rect, left));   // EXPECT: 0
    printf("%d\n", OFFSETOF(struct Rect, top));    // EXPECT: 4
    printf("%d\n", OFFSETOF(struct Rect, right));  // EXPECT: 8
    printf("%d\n", OFFSETOF(struct Rect, bottom)); // EXPECT: 12

    /* XSTR two-level stringify */
    #define MYVAL 123
    printf("%s\n", XSTR(MYVAL));         // EXPECT: 123

    return 0;
}
