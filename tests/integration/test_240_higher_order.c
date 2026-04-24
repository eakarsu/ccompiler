int printf(const char *fmt, ...);
// EXPECT: sum=36\nproduct=40320\nmax=8\nmin=1\ncount_even=4\nfirst_even=2\nlast_even=7\nevens: 8 4 2 6\nodds: 5 3 1 7\ndoubled: 10 6 16 2 8 14 4 12\nsquared: 25 9 64 1 16 49 4 36\nzip_add: 11 22 33 44\nzip_mul: 10 40 90 160\nzip_max: 10 20 30 40
// Test 240: Higher-order function patterns

typedef int (*unary)(int);
typedef int (*binary)(int, int);
typedef int (*predicate)(int);

int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }
int sub(int a, int b) { return a - b; }
int max_fn(int a, int b) { return (a > b) ? a : b; }
int min_fn(int a, int b) { return (a < b) ? a : b; }

int double_val(int x) { return x * 2; }
int square(int x) { return x * x; }
int inc(int x) { return x + 1; }
int is_even(int x) { return (x / 2) * 2 == x; }
int is_positive(int x) { return x > 0; }

int fold_left(int *arr, int n, binary fn, int init) {
    int acc = init;
    int i = 0;
    while (i < n) { acc = fn(acc, arr[i]); i = i + 1; }
    return acc;
}

void map_inplace(int *arr, int n, unary fn) {
    int i = 0;
    while (i < n) { arr[i] = fn(arr[i]); i = i + 1; }
}

int count_where(int *arr, int n, predicate pred) {
    int c = 0;
    int i = 0;
    while (i < n) { if (pred(arr[i])) c = c + 1; i = i + 1; }
    return c;
}

int find_where(int *arr, int n, predicate pred) {
    int i = 0;
    while (i < n) { if (pred(arr[i])) return i; i = i + 1; }
    return -1;
}

int find_last_where(int *arr, int n, predicate pred) {
    int i = n - 1;
    while (i >= 0) { if (pred(arr[i])) return i; i = i - 1; }
    return -1;
}

void partition_arr(int *arr, int n, predicate pred, int *yes, int *yes_n, int *no, int *no_n) {
    *yes_n = 0;
    *no_n = 0;
    int i = 0;
    while (i < n) {
        if (pred(arr[i])) {
            yes[*yes_n] = arr[i];
            *yes_n = *yes_n + 1;
        } else {
            no[*no_n] = arr[i];
            *no_n = *no_n + 1;
        }
        i = i + 1;
    }
}

int zip_with(int *a, int *b, int *out, int n, binary fn) {
    int i = 0;
    while (i < n) {
        out[i] = fn(a[i], b[i]);
        i = i + 1;
    }
    return n;
}

void print_arr(int *arr, int n) {
    int i = 0;
    while (i < n) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
        i = i + 1;
    }
    printf("\n");
}

int main(void) {
    int arr[8];
    arr[0] = 5; arr[1] = 3; arr[2] = 8; arr[3] = 1;
    arr[4] = 4; arr[5] = 7; arr[6] = 2; arr[7] = 6;

    printf("sum=%d\n", fold_left(arr, 8, add, 0));
    printf("product=%d\n", fold_left(arr, 8, mul, 1));
    printf("max=%d\n", fold_left(arr, 8, max_fn, arr[0]));
    printf("min=%d\n", fold_left(arr, 8, min_fn, arr[0]));

    printf("count_even=%d\n", count_where(arr, 8, is_even));
    printf("first_even=%d\n", find_where(arr, 8, is_even));
    printf("last_even=%d\n", find_last_where(arr, 8, is_even));

    int yes[8];
    int no[8];
    int yn = 0;
    int nn = 0;
    partition_arr(arr, 8, is_even, yes, &yn, no, &nn);
    printf("evens: "); print_arr(yes, yn);
    printf("odds: "); print_arr(no, nn);

    int work[8];
    int i = 0;
    while (i < 8) { work[i] = arr[i]; i = i + 1; }
    map_inplace(work, 8, double_val);
    printf("doubled: "); print_arr(work, 8);

    i = 0;
    while (i < 8) { work[i] = arr[i]; i = i + 1; }
    map_inplace(work, 8, square);
    printf("squared: "); print_arr(work, 8);

    int a[4]; a[0] = 1; a[1] = 2; a[2] = 3; a[3] = 4;
    int b[4]; b[0] = 10; b[1] = 20; b[2] = 30; b[3] = 40;
    int out[4];
    zip_with(a, b, out, 4, add);
    printf("zip_add: "); print_arr(out, 4);
    zip_with(a, b, out, 4, mul);
    printf("zip_mul: "); print_arr(out, 4);
    zip_with(a, b, out, 4, max_fn);
    printf("zip_max: "); print_arr(out, 4);

    return 0;
}
