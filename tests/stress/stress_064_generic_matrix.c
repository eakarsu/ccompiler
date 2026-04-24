// stress_064_generic_matrix.c - _Generic type dispatch stress test
// EXPECT: int_min=3
// EXPECT: int_max=10
// EXPECT: long_min=100
// EXPECT: long_max=999
// EXPECT: char_min=A
// EXPECT: char_max=Z
// EXPECT: uint_min=1
// EXPECT: uint_max=42
// EXPECT: type_int=int
// EXPECT: type_long=long
// EXPECT: type_char=char
// EXPECT: type_uint=uint
// EXPECT: type_ptr=ptr
// EXPECT: int_sorted=1 3 5 7 9
// EXPECT: long_sorted=10 20 30 40 50
// EXPECT: char_sorted=A B C D E
// EXPECT: cmp_int_lt=-1
// EXPECT: cmp_int_eq=0
// EXPECT: cmp_int_gt=1
// EXPECT: cmp_long_lt=-1
// EXPECT: cmp_char_lt=-1
// EXPECT: abs_int=42
// EXPECT: abs_int_neg=42
// EXPECT: abs_long=999
// EXPECT: abs_long_neg=999
// EXPECT: clamp_int_lo=0
// EXPECT: clamp_int_mid=50
// EXPECT: clamp_int_hi=100
// EXPECT: generic_sum_int=15
// EXPECT: generic_sum_long=150
// EXPECT: swap_int=20 10
// EXPECT: swap_long=200 100
// EXPECT: swap_char=B A

int printf(const char *fmt, ...);

// --- Generic min/max via _Generic ---

int min_int(int a, int b) { return a < b ? a : b; }
long min_long(long a, long b) { return a < b ? a : b; }
char min_char(char a, char b) { return a < b ? a : b; }
unsigned min_uint(unsigned a, unsigned b) { return a < b ? a : b; }

int max_int(int a, int b) { return a > b ? a : b; }
long max_long(long a, long b) { return a > b ? a : b; }
char max_char(char a, char b) { return a > b ? a : b; }
unsigned max_uint(unsigned a, unsigned b) { return a > b ? a : b; }

#define generic_min(a, b) _Generic((a), \
    int: min_int, \
    long: min_long, \
    char: min_char, \
    unsigned int: min_uint \
)(a, b)

#define generic_max(a, b) _Generic((a), \
    int: max_int, \
    long: max_long, \
    char: max_char, \
    unsigned int: max_uint \
)(a, b)

// --- Generic type name ---

#define type_name(x) _Generic((x), \
    int: "int", \
    long: "long", \
    char: "char", \
    unsigned int: "uint", \
    int*: "ptr", \
    default: "unknown" \
)

// --- Generic compare (-1, 0, 1) ---

int cmp_int(int a, int b) { return a < b ? -1 : (a > b ? 1 : 0); }
long cmp_long(long a, long b) { return a < b ? -1 : (a > b ? 1 : 0); }
int cmp_char(char a, char b) { return a < b ? -1 : (a > b ? 1 : 0); }

#define generic_cmp(a, b) _Generic((a), \
    int: cmp_int, \
    long: cmp_long, \
    char: cmp_char \
)(a, b)

// --- Generic abs ---

int abs_int(int x) { return x < 0 ? -x : x; }
long abs_long(long x) { return x < 0 ? -x : x; }

#define generic_abs(x) _Generic((x), \
    int: abs_int, \
    long: abs_long \
)(x)

// --- Generic clamp ---

int clamp_int(int x, int lo, int hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

long clamp_long(long x, long lo, long hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

#define generic_clamp(x, lo, hi) _Generic((x), \
    int: clamp_int, \
    long: clamp_long \
)(x, lo, hi)

// --- Generic swap ---

void swap_int(int *a, int *b) { int t = *a; *a = *b; *b = t; }
void swap_long(long *a, long *b) { long t = *a; *a = *b; *b = t; }
void swap_char(char *a, char *b) { char t = *a; *a = *b; *b = t; }

#define generic_swap(a, b) _Generic((a), \
    int*: swap_int, \
    long*: swap_long, \
    char*: swap_char \
)(a, b)

// --- Bubble sort using _Generic compare ---

void sort_int_arr(int *arr, int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (generic_cmp(arr[j], arr[j + 1]) > 0) {
                generic_swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

void sort_long_arr(long *arr, int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (cmp_long(arr[j], arr[j + 1]) > 0) {
                swap_long(&arr[j], &arr[j + 1]);
            }
        }
    }
}

void sort_char_arr(char *arr, int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (cmp_char(arr[j], arr[j + 1]) > 0) {
                swap_char(&arr[j], &arr[j + 1]);
            }
        }
    }
}

// --- Generic sum of array ---

int sum_int_arr(int *arr, int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i++) s += arr[i];
    return s;
}

long sum_long_arr(long *arr, int n) {
    long s = 0;
    int i;
    for (i = 0; i < n; i++) s += arr[i];
    return s;
}

#define generic_sum(arr, n) _Generic((arr[0]), \
    int: sum_int_arr, \
    long: sum_long_arr \
)(arr, n)

// --- Tests ---

void test_min_max(void) {
    int ia = 10, ib = 3;
    printf("int_min=%d\n", generic_min(ia, ib));
    printf("int_max=%d\n", generic_max(ia, ib));

    long la = 999L, lb = 100L;
    printf("long_min=%ld\n", generic_min(lb, la));
    printf("long_max=%ld\n", generic_max(lb, la));

    char ca = 'Z', cb = 'A';
    printf("char_min=%c\n", generic_min(ca, cb));
    printf("char_max=%c\n", generic_max(ca, cb));

    unsigned ua = 42U, ub = 1U;
    printf("uint_min=%u\n", generic_min(ub, ua));
    printf("uint_max=%u\n", generic_max(ub, ua));
}

void test_type_name(void) {
    int i = 0;
    long l = 0;
    char c = 0;
    unsigned u = 0;
    int *p = &i;
    printf("type_int=%s\n", type_name(i));
    printf("type_long=%s\n", type_name(l));
    printf("type_char=%s\n", type_name(c));
    printf("type_uint=%s\n", type_name(u));
    printf("type_ptr=%s\n", type_name(p));
}

void test_sort(void) {
    int iarr[5] = {9, 3, 7, 1, 5};
    sort_int_arr(iarr, 5);
    printf("int_sorted=");
    int i;
    for (i = 0; i < 5; i++) {
        if (i > 0) printf(" ");
        printf("%d", iarr[i]);
    }
    printf("\n");

    long larr[5] = {50, 30, 10, 40, 20};
    sort_long_arr(larr, 5);
    printf("long_sorted=");
    for (i = 0; i < 5; i++) {
        if (i > 0) printf(" ");
        printf("%ld", larr[i]);
    }
    printf("\n");

    char carr[5] = {'E', 'C', 'A', 'D', 'B'};
    sort_char_arr(carr, 5);
    printf("char_sorted=");
    for (i = 0; i < 5; i++) {
        if (i > 0) printf(" ");
        printf("%c", carr[i]);
    }
    printf("\n");
}

void test_compare(void) {
    printf("cmp_int_lt=%d\n", generic_cmp(3, 10));
    printf("cmp_int_eq=%d\n", generic_cmp(5, 5));
    printf("cmp_int_gt=%d\n", generic_cmp(10, 3));
    printf("cmp_long_lt=%d\n", (int)cmp_long(10L, 20L));
    printf("cmp_char_lt=%d\n", cmp_char('A', 'Z'));
}

void test_abs(void) {
    int i1 = 42, i2 = -42;
    printf("abs_int=%d\n", generic_abs(i1));
    printf("abs_int_neg=%d\n", generic_abs(i2));

    long l1 = 999L, l2 = -999L;
    printf("abs_long=%ld\n", generic_abs(l1));
    printf("abs_long_neg=%ld\n", generic_abs(l2));
}

void test_clamp(void) {
    printf("clamp_int_lo=%d\n", generic_clamp(-10, 0, 100));
    printf("clamp_int_mid=%d\n", generic_clamp(50, 0, 100));
    printf("clamp_int_hi=%d\n", generic_clamp(200, 0, 100));
}

void test_generic_sum(void) {
    int iarr[5] = {1, 2, 3, 4, 5};
    printf("generic_sum_int=%d\n", generic_sum(iarr, 5));

    long larr[5] = {10, 20, 30, 40, 50};
    printf("generic_sum_long=%ld\n", generic_sum(larr, 5));
}

void test_swap(void) {
    int ia = 10, ib = 20;
    generic_swap(&ia, &ib);
    printf("swap_int=%d %d\n", ia, ib);

    long la = 100L, lb = 200L;
    generic_swap(&la, &lb);
    printf("swap_long=%ld %ld\n", la, lb);

    char ca = 'A', cb = 'B';
    generic_swap(&ca, &cb);
    printf("swap_char=%c %c\n", ca, cb);
}

int main(void) {
    test_min_max();
    test_type_name();
    test_sort();
    test_compare();
    test_abs();
    test_clamp();
    test_generic_sum();
    test_swap();
    return 0;
}
