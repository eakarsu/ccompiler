int printf(const char *fmt, ...);

/* _Generic selection expressions: type-based dispatch at compile time */

/* Helper functions to identify types (return a code) */
int classify_int(int x)    { (void)x; return 1; }
int classify_char(char x)  { (void)x; return 2; }
int classify_ptr(void *x)  { (void)x; return 3; }

/* Macros using _Generic */
#define TYPE_CODE(x) _Generic((x), \
    int:    1,                      \
    char:   2,                      \
    int*:   3,                      \
    char*:  4,                      \
    default: 0)

#define ABS(x) _Generic((x), \
    int: ((x) < 0 ? -(x) : (x)), \
    default: (x))

#define IS_INT(x) _Generic((x), int: 1, default: 0)

#define DESCRIBE(x) _Generic((x),   \
    int:    printf("int\n"),        \
    char*:  printf("char*\n"),      \
    int*:   printf("int*\n"),       \
    default: printf("other\n"))

/* _Generic selecting a function to call */
int double_it(int x)   { return x * 2; }
int negate_it(int x)   { return -x; }

#define TRANSFORM(flag, x) _Generic((flag), \
    int: double_it(x),                      \
    default: negate_it(x))

int main(void) {
    /* --- TYPE_CODE for int --- */
    int iv = 42;
    printf("%d\n", TYPE_CODE(iv));            // EXPECT: 1

    /* --- TYPE_CODE for char --- */
    char cv = 'A';
    printf("%d\n", TYPE_CODE(cv));            // EXPECT: 2

    /* --- TYPE_CODE for int* --- */
    int *ip = &iv;
    printf("%d\n", TYPE_CODE(ip));            // EXPECT: 3

    /* --- TYPE_CODE for char* --- */
    char *cp = "hello";
    printf("%d\n", TYPE_CODE(cp));            // EXPECT: 4

    /* --- TYPE_CODE default (long) --- */
    long lv = 100L;
    printf("%d\n", TYPE_CODE(lv));            // EXPECT: 0

    /* --- ABS macro via _Generic --- */
    int neg = -15;
    int pos = 15;
    printf("%d\n", ABS(neg));                 // EXPECT: 15
    printf("%d\n", ABS(pos));                 // EXPECT: 15
    printf("%d\n", ABS(0));                   // EXPECT: 0
    printf("%d\n", ABS(-100));                // EXPECT: 100

    /* --- IS_INT --- */
    printf("%d\n", IS_INT(iv));               // EXPECT: 1
    printf("%d\n", IS_INT(cv));               // EXPECT: 0
    printf("%d\n", IS_INT(ip));               // EXPECT: 0

    /* --- _Generic with literal int --- */
    printf("%d\n", TYPE_CODE(0));             // EXPECT: 1
    printf("%d\n", TYPE_CODE(42));            // EXPECT: 1

    /* --- _Generic DESCRIBE macro (type-based print) --- */
    DESCRIBE(iv);                             // EXPECT: int
    DESCRIBE(cp);                             // EXPECT: char*
    DESCRIBE(ip);                             // EXPECT: int*
    DESCRIBE(lv);                             // EXPECT: other

    /* --- _Generic in expression context with arithmetic --- */
    int a = 10, b = 20;
    int result = TYPE_CODE(a) + TYPE_CODE(b);
    printf("%d\n", result);                   // EXPECT: 2

    /* --- _Generic controlling which function to call (TRANSFORM) --- */
    int flag_int = 0;
    printf("%d\n", TRANSFORM(flag_int, 5));   // EXPECT: 10

    /* --- Nested _Generic: type code used in computation --- */
    int tc = TYPE_CODE(iv);
    printf("%d\n", tc * 100);                 // EXPECT: 100

    /* --- _Generic in loop body --- */
    int codes[5];
    int i;
    int vars[5];
    vars[0] = 1; vars[1] = 2; vars[2] = 3; vars[3] = 4; vars[4] = 5;
    for (i = 0; i < 5; i++) {
        codes[i] = TYPE_CODE(vars[i]);
    }
    /* All are int, so all codes == 1 */
    int csum = 0;
    for (i = 0; i < 5; i++) csum += codes[i];
    printf("%d\n", csum);                     // EXPECT: 5

    /* --- _Generic with address-of expression --- */
    int x = 7;
    printf("%d\n", TYPE_CODE(&x));            // EXPECT: 3

    /* --- _Generic default path explicitly tested --- */
    short sv = 3;
    printf("%d\n", TYPE_CODE(sv));            // EXPECT: 0

    /* --- ABS chain --- */
    printf("%d\n", ABS(ABS(-42)));            // EXPECT: 42

    /* --- _Generic result assigned and used --- */
    int code_res = _Generic(iv, int: 100, default: 0);
    printf("%d\n", code_res);                 // EXPECT: 100

    int default_res = _Generic(lv, int: 100, default: 999);
    printf("%d\n", default_res);              // EXPECT: 999

    /* --- _Generic in ternary --- */
    int is_int_val = IS_INT(iv) ? 1 : -1;
    printf("%d\n", is_int_val);               // EXPECT: 1

    int not_int_val = IS_INT(lv) ? 1 : -1;
    printf("%d\n", not_int_val);              // EXPECT: -1

    /* --- Multiple _Generic in same expression --- */
    int combined = TYPE_CODE(iv) + TYPE_CODE(cp) + TYPE_CODE(lv);
    printf("%d\n", combined);                 // EXPECT: 5

    return 0;
}
