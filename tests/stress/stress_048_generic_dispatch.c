// stress_048_generic_dispatch.c - _Generic stress test
// EXPECT: int_type=1
// EXPECT: char_type=2
// EXPECT: ptr_type=3
// EXPECT: long_type=4
// EXPECT: unsigned_type=5
// EXPECT: int_size=4
// EXPECT: char_size=1
// EXPECT: ptr_size=8
// EXPECT: abs_int=42
// EXPECT: abs_long=100
// EXPECT: classify_arr=3
// EXPECT: double_dispatch=10
// EXPECT: typedef_generic=1
// EXPECT: generic_expr=99

int printf(const char *fmt, ...);

// --- Basic type classification ---
#define CLASSIFY(x) _Generic((x), \
    int: 1,                        \
    char: 2,                       \
    int *: 3,                      \
    long: 4,                       \
    unsigned int: 5,               \
    default: 0                     \
)

// --- Size via _Generic ---
#define TYPE_SIZE(x) _Generic((x), \
    int: sizeof(int),              \
    char: sizeof(char),            \
    int *: sizeof(int *),          \
    long: sizeof(long),            \
    default: 0                     \
)

// --- Absolute value via _Generic selecting function ---
int abs_int(int x) { return x < 0 ? -x : x; }
long abs_long(long x) { return x < 0 ? -x : x; }

#define MY_ABS(x) _Generic((x), \
    int: abs_int,                \
    long: abs_long               \
)(x)

// --- _Generic with pointer types ---
int classify_ptr(int *p) { return 1; }
int classify_ptr_char(char *p) { return 2; }
int classify_ptr_void(void *p) { return 3; }

#define CLASSIFY_PTR(x) _Generic((x), \
    int *: classify_ptr,               \
    char *: classify_ptr_char,         \
    void *: classify_ptr_void,         \
    default: classify_ptr_void         \
)(x)

// --- _Generic returning values for double dispatch ---
int dispatch_int_int(int a, int b) { return a + b; }
int dispatch_int_char(int a, char b) { return a - b; }

#define DISPATCH(a, b) _Generic((a), \
    int: _Generic((b),               \
        int: dispatch_int_int,       \
        char: dispatch_int_char      \
    )                                \
)(a, b)

// --- typedef types with _Generic ---
typedef int myint;
typedef char mychar;

#define IS_MYINT(x) _Generic((x), \
    int: 1,                        \
    char: 0,                       \
    default: -1                    \
)

// --- _Generic in expression context ---
#define MAX_OR_MIN(x, choose_max) _Generic((x), \
    int: ((choose_max) ? ((x) > 0 ? (x) : 0) : ((x) < 0 ? (x) : 0)), \
    default: 0                                                          \
)

int main(void) {
    // Basic type classification
    int iv = 10;
    char cv = 'A';
    int *pv = &iv;
    long lv = 100;
    unsigned int uv = 50;

    printf("int_type=%d\n", CLASSIFY(iv));
    printf("char_type=%d\n", CLASSIFY(cv));
    printf("ptr_type=%d\n", CLASSIFY(pv));
    printf("long_type=%d\n", CLASSIFY(lv));
    printf("unsigned_type=%d\n", CLASSIFY(uv));

    // Type size via _Generic
    printf("int_size=%d\n", (int)TYPE_SIZE(iv));
    printf("char_size=%d\n", (int)TYPE_SIZE(cv));
    printf("ptr_size=%d\n", (int)TYPE_SIZE(pv));

    // Absolute value via _Generic
    int neg_i = -42;
    long neg_l = -100;
    printf("abs_int=%d\n", MY_ABS(neg_i));
    printf("abs_long=%ld\n", MY_ABS(neg_l));

    // Pointer classification: void* should give 3
    void *vp = 0;
    printf("classify_arr=%d\n", CLASSIFY_PTR(vp));

    // Double dispatch via nested _Generic
    int da = 7;
    int db = 3;
    printf("double_dispatch=%d\n", DISPATCH(da, db));  // 7+3=10

    // typedef type (myint is int, so should match int)
    myint mi = 42;
    printf("typedef_generic=%d\n", IS_MYINT(mi));

    // _Generic in expression
    int val = 99;
    printf("generic_expr=%d\n", MAX_OR_MIN(val, 1));  // max: 99>0 so 99

    return 0;
}
