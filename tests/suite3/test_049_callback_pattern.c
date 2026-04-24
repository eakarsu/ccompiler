int printf(const char *fmt, ...);

typedef int (*IntUnary)(int);
typedef int (*IntBinary)(int, int);
typedef int (*IntPred)(int);
typedef int (*IntCmp)(int, int);

/* --- Basic functions for use as callbacks --- */
int double_it(int x)  { return x * 2; }
int triple_it(int x)  { return x * 3; }
int square(int x)     { return x * x; }
int negate(int x)     { return -x; }
int increment(int x)  { return x + 1; }
int decrement(int x)  { return x - 1; }
int abs_val(int x)    { return x < 0 ? -x : x; }

int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }
int max2(int a, int b){ return a > b ? a : b; }
int min2(int a, int b){ return a < b ? a : b; }

int is_even(int x)     { return x % 2 == 0; }
int is_positive(int x) { return x > 0; }
int is_negative(int x) { return x < 0; }
int is_gt10(int x)     { return x > 10; }
int is_lt0(int x)      { return x < 0; }

int cmp_asc(int a, int b)  { return a - b; }
int cmp_desc(int a, int b) { return b - a; }

/* --- Higher-order functions --- */

/* map: apply f to each element, store in out */
void map(IntUnary f, int *in, int *out, int n) {
    int i;
    for (i = 0; i < n; i++) out[i] = f(in[i]);
}

/* filter: keep elements where pred is true, return count */
int filter(IntPred pred, int *in, int *out, int n) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (pred(in[i])) out[i < n ? count : 0] = in[i], count++;
    }
    return count;
}

/* reduce/fold_left: combine with binary op, starting from init */
int reduce(IntBinary op, int *arr, int n, int init) {
    int acc = init;
    int i;
    for (i = 0; i < n; i++) acc = op(acc, arr[i]);
    return acc;
}

/* foreach: apply f to each element (for side effects via printf) */
void foreach_print(IntUnary f, int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) printf("%d\n", f(arr[i]));
}

/* any: return 1 if any element satisfies pred */
int any(IntPred pred, int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) if (pred(arr[i])) return 1;
    return 0;
}

/* all: return 1 if all elements satisfy pred */
int all(IntPred pred, int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) if (!pred(arr[i])) return 0;
    return 1;
}

/* count_if: count elements satisfying pred */
int count_if(IntPred pred, int *arr, int n) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) if (pred(arr[i])) count++;
    return count;
}

/* find_first: return index of first element satisfying pred, -1 if none */
int find_first(IntPred pred, int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) if (pred(arr[i])) return i;
    return -1;
}

/* sort using comparator (insertion sort) */
void sort_with_cmp(int *arr, int n, IntCmp cmp) {
    int i, j;
    for (i = 1; i < n; i++) {
        int key = arr[i];
        j = i - 1;
        while (j >= 0 && cmp(arr[j], key) > 0) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

/* apply_n_times: apply f to x n times */
int apply_n_times(IntUnary f, int x, int n) {
    int result = x;
    int i;
    for (i = 0; i < n; i++) result = f(result);
    return result;
}

/* compose two unary functions */
int compose(IntUnary f, IntUnary g, int x) {
    return f(g(x));
}

int main(void) {
    int arr[8];
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4;
    arr[4]=5; arr[5]=6; arr[6]=7; arr[7]=8;

    /* map with double */
    int out[8];
    map(double_it, arr, out, 8);
    printf("%d\n", out[0]);   // EXPECT: 2
    printf("%d\n", out[4]);   // EXPECT: 10
    printf("%d\n", out[7]);   // EXPECT: 16

    /* map with square */
    map(square, arr, out, 8);
    printf("%d\n", out[0]);   // EXPECT: 1
    printf("%d\n", out[2]);   // EXPECT: 9
    printf("%d\n", out[7]);   // EXPECT: 64

    /* reduce sum */
    printf("%d\n", reduce(add, arr, 8, 0));   // EXPECT: 36
    /* 1+2+3+4+5+6+7+8 = 36 */

    /* reduce product */
    printf("%d\n", reduce(mul, arr, 8, 1));   // EXPECT: 40320
    /* 8! = 40320 */

    /* reduce max */
    printf("%d\n", reduce(max2, arr, 8, arr[0])); // EXPECT: 8

    /* reduce min */
    printf("%d\n", reduce(min2, arr, 8, arr[0])); // EXPECT: 1

    /* count_if */
    printf("%d\n", count_if(is_even, arr, 8));     // EXPECT: 4
    printf("%d\n", count_if(is_positive, arr, 8)); // EXPECT: 8

    /* any / all */
    printf("%d\n", any(is_gt10, arr, 8));   // EXPECT: 0
    printf("%d\n", any(is_even, arr, 8));   // EXPECT: 1
    printf("%d\n", all(is_positive, arr, 8)); // EXPECT: 1
    printf("%d\n", all(is_even, arr, 8));     // EXPECT: 0

    /* find_first */
    printf("%d\n", find_first(is_even, arr, 8));  // EXPECT: 1
    printf("%d\n", find_first(is_gt10, arr, 8));  // EXPECT: -1

    /* Mixed array with negatives */
    int arr2[6];
    arr2[0]=-5; arr2[1]=3; arr2[2]=-1; arr2[3]=0; arr2[4]=7; arr2[5]=-2;
    printf("%d\n", count_if(is_negative, arr2, 6)); // EXPECT: 3
    printf("%d\n", count_if(is_positive, arr2, 6)); // EXPECT: 2
    printf("%d\n", any(is_positive, arr2, 6));       // EXPECT: 1
    printf("%d\n", all(is_negative, arr2, 6));       // EXPECT: 0
    printf("%d\n", find_first(is_negative, arr2, 6)); // EXPECT: 0

    /* sort ascending */
    int s1[6];
    s1[0]=5; s1[1]=2; s1[2]=8; s1[3]=1; s1[4]=9; s1[5]=3;
    sort_with_cmp(s1, 6, cmp_asc);
    printf("%d\n", s1[0]);   // EXPECT: 1
    printf("%d\n", s1[1]);   // EXPECT: 2
    printf("%d\n", s1[2]);   // EXPECT: 3
    printf("%d\n", s1[3]);   // EXPECT: 5
    printf("%d\n", s1[4]);   // EXPECT: 8
    printf("%d\n", s1[5]);   // EXPECT: 9

    /* sort descending */
    int s2[5];
    s2[0]=4; s2[1]=1; s2[2]=7; s2[3]=3; s2[4]=6;
    sort_with_cmp(s2, 5, cmp_desc);
    printf("%d\n", s2[0]);   // EXPECT: 7
    printf("%d\n", s2[1]);   // EXPECT: 6
    printf("%d\n", s2[2]);   // EXPECT: 4
    printf("%d\n", s2[3]);   // EXPECT: 3
    printf("%d\n", s2[4]);   // EXPECT: 1

    /* apply_n_times */
    printf("%d\n", apply_n_times(double_it, 1, 8));    // EXPECT: 256
    printf("%d\n", apply_n_times(increment, 0, 10));   // EXPECT: 10
    printf("%d\n", apply_n_times(square, 2, 3));        // EXPECT: 256
    /* square(2)=4, square(4)=16, square(16)=256 */

    /* compose */
    printf("%d\n", compose(double_it, increment, 5));  // EXPECT: 12
    /* increment(5)=6, double_it(6)=12 */
    printf("%d\n", compose(square, double_it, 3));     // EXPECT: 36
    /* double_it(3)=6, square(6)=36 */
    printf("%d\n", compose(abs_val, negate, 7));       // EXPECT: 7
    /* negate(7)=-7, abs_val(-7)=7 */

    /* foreach_print with transform */
    int small[4];
    small[0]=1; small[1]=2; small[2]=3; small[3]=4;
    foreach_print(triple_it, small, 4); // EXPECT: 3
                                         // EXPECT: 6
                                         // EXPECT: 9
                                         // EXPECT: 12

    return 0;
}
