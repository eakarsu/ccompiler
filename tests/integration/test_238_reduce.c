int printf(const char *fmt, ...);
// EXPECT: sum=31\nproduct=6480\nmax=9\nmin=1\ngcd=1\nsum_right=31\nprefix_sum: 3 4 8 9 14 23 25 31\nprefix_max: 3 3 4 4 5 9 9 9\nall_pos(pos)=1\nall_pos(mix)=0\nany_pos(mix)=1\nall_even(pos)=0\nany_even(pos)=1\ndot_product=70
// Test 238: Reduce/fold patterns using arrays

typedef int (*reducer)(int, int);

int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }
int max_fn(int a, int b) { if (a > b) return a; return b; }
int min_fn(int a, int b) { if (a < b) return a; return b; }
int gcd(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        int t = b;
        b = a - (a / b) * b;
        a = t;
    }
    return a;
}

int reduce(int *arr, int n, reducer fn, int init) {
    int result = init;
    int i = 0;
    while (i < n) {
        result = fn(result, arr[i]);
        i = i + 1;
    }
    return result;
}

int reduce_right(int *arr, int n, reducer fn, int init) {
    int result = init;
    int i = n - 1;
    while (i >= 0) {
        result = fn(arr[i], result);
        i = i - 1;
    }
    return result;
}

int scan(int *arr, int *out, int n, reducer fn, int init) {
    int acc = init;
    int i = 0;
    while (i < n) {
        acc = fn(acc, arr[i]);
        out[i] = acc;
        i = i + 1;
    }
    return acc;
}

int all(int *arr, int n, int (*pred)(int)) {
    int i = 0;
    while (i < n) {
        if (!pred(arr[i])) return 0;
        i = i + 1;
    }
    return 1;
}

int any(int *arr, int n, int (*pred)(int)) {
    int i = 0;
    while (i < n) {
        if (pred(arr[i])) return 1;
        i = i + 1;
    }
    return 0;
}

int is_positive(int x) { return x > 0; }
int is_even(int x) { return (x / 2) * 2 == x; }

void print_arr(int *arr, int n) {
    int i = 0;
    while (i < n) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
        i = i + 1;
    }
    printf("\n");
}

int zip_reduce(int *a, int *b, int n, reducer combine, reducer fold, int init) {
    int result = init;
    int i = 0;
    while (i < n) {
        result = fold(result, combine(a[i], b[i]));
        i = i + 1;
    }
    return result;
}

int main(void) {
    int arr[8];
    arr[0] = 3; arr[1] = 1; arr[2] = 4; arr[3] = 1;
    arr[4] = 5; arr[5] = 9; arr[6] = 2; arr[7] = 6;

    printf("sum=%d\n", reduce(arr, 8, add, 0));
    printf("product=%d\n", reduce(arr, 8, mul, 1));
    printf("max=%d\n", reduce(arr, 8, max_fn, arr[0]));
    printf("min=%d\n", reduce(arr, 8, min_fn, arr[0]));
    printf("gcd=%d\n", reduce(arr, 8, gcd, arr[0]));

    printf("sum_right=%d\n", reduce_right(arr, 8, add, 0));

    int scanned[8];
    scan(arr, scanned, 8, add, 0);
    printf("prefix_sum: "); print_arr(scanned, 8);

    scan(arr, scanned, 8, max_fn, 0);
    printf("prefix_max: "); print_arr(scanned, 8);

    int pos[4]; pos[0] = 1; pos[1] = 2; pos[2] = 3; pos[3] = 4;
    int mix[4]; mix[0] = -1; mix[1] = 2; mix[2] = -3; mix[3] = 4;

    printf("all_pos(pos)=%d\n", all(pos, 4, is_positive));
    printf("all_pos(mix)=%d\n", all(mix, 4, is_positive));
    printf("any_pos(mix)=%d\n", any(mix, 4, is_positive));
    printf("all_even(pos)=%d\n", all(pos, 4, is_even));
    printf("any_even(pos)=%d\n", any(pos, 4, is_even));

    int a[4]; a[0] = 1; a[1] = 2; a[2] = 3; a[3] = 4;
    int b[4]; b[0] = 5; b[1] = 6; b[2] = 7; b[3] = 8;
    printf("dot_product=%d\n", zip_reduce(a, b, 4, mul, add, 0));

    return 0;
}
