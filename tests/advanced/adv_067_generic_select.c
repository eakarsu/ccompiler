// EXPECT: type_int=int
// EXPECT: type_char=char
// EXPECT: type_ptr=pointer
// EXPECT: type_long=long
// EXPECT: double_int=20
// EXPECT: double_char=cc
// EXPECT: size_int=4
// EXPECT: size_char=1
// EXPECT: size_long=8
// EXPECT: classify_int=1
// EXPECT: classify_char=2
// EXPECT: classify_ptr=3
// EXPECT: combo=1
// EXPECT: var_type=int
// EXPECT: expr_type=unknown
// EXPECT: arr_type=pointer
// EXPECT: nested_type=1
// EXPECT: all_passed=1

int printf(const char *fmt, ...);

/* Type name dispatch - multi-line macro with backslash continuations */
#define type_name(x) _Generic((x), \
    int: "int",                     \
    char: "char",                   \
    long: "long",                   \
    int *: "pointer",              \
    default: "unknown"             \
)

/* Type-safe doubling functions */
int double_int(int x) { return x * 2; }
char double_char(char c) { return c; }
long double_long(long x) { return x * 2; }

#define my_double(x) _Generic((x), \
    int: double_int,                \
    char: double_char,              \
    long: double_long               \
)(x)

/* Size-of via _Generic */
#define type_size(x) _Generic((x), \
    int: 4,                        \
    char: 1,                       \
    long: 8,                       \
    default: 0                     \
)

/* Classification: returns category number */
#define classify(x) _Generic((x), \
    int: 1,                        \
    char: 2,                       \
    int *: 3,                      \
    long: 5,                       \
    default: 0                     \
)

/* Is-integer check */
#define is_integer(x) _Generic((x), \
    int: 1,                         \
    long: 1,                        \
    char: 1,                        \
    default: 0                      \
)

int main(void) {
    int passed = 1;

    /* Type name dispatch */
    int iv = 42;
    char cv = 'a';
    int *pv = &iv;
    long lv = 100;

    printf("type_int=%s\n", type_name(iv));
    printf("type_char=%s\n", type_name(cv));
    printf("type_ptr=%s\n", type_name(pv));
    printf("type_long=%s\n", type_name(lv));

    /* Type-safe double */
    int di = my_double(10);
    printf("double_int=%d\n", di);
    if (di != 20) passed = 0;

    char dc = my_double((char)'c');
    printf("double_char=%c%c\n", dc, dc);
    if (dc != 'c') passed = 0;

    /* Size via _Generic */
    printf("size_int=%d\n", type_size(iv));
    if (type_size(iv) != 4) passed = 0;

    printf("size_char=%d\n", type_size(cv));
    if (type_size(cv) != 1) passed = 0;

    printf("size_long=%d\n", type_size(lv));
    if (type_size(lv) != 8) passed = 0;

    /* Classification */
    int ci = classify(iv);
    printf("classify_int=%d\n", ci);
    if (ci != 1) passed = 0;

    int cc2 = classify(cv);
    printf("classify_char=%d\n", cc2);
    if (cc2 != 2) passed = 0;

    int cp = classify(pv);
    printf("classify_ptr=%d\n", cp);
    if (cp != 3) passed = 0;

    /* is_integer check */
    int combo = is_integer(iv);
    printf("combo=%d\n", combo);
    if (combo != 1) passed = 0;

    /* _Generic with variable */
    int test_var = 5;
    printf("var_type=%s\n", type_name(test_var));

    /* _Generic with expression */
    printf("expr_type=%s\n", type_name(iv + 1));

    /* _Generic with array decayed to pointer */
    int arr[3] = {1, 2, 3};
    printf("arr_type=%s\n", type_name(arr));

    /* Nested _Generic usage */
    int nx = 10;
    int nresult = _Generic(nx, int: 1, default: 0);
    printf("nested_type=%d\n", nresult);
    if (nresult != 1) passed = 0;

    printf("all_passed=%d\n", passed);
    return passed ? 0 : 1;
}
