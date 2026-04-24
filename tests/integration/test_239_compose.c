int printf(const char *fmt, ...);
// EXPECT: apply inc 5 times to 0: 5\napply double 4 times to 1: 16\napply dec 3 times to 10: 7\ncompose(double,inc,5)=12\ncompose(inc,double,5)=11\ncompose3(inc,double,square,3)=19\ncompose3(square,inc,double,3)=49\nbefore chain: 1 2 3 4 5\nafter chain: 5 7 9 11 13\niterate inc from 0 to 10: 10 steps\niterate double from 1 to 16: 4 steps\n=== collatz ===\nc(1)=0\nc(2)=1\nc(3)=7\nc(4)=2\nc(5)=5\nc(6)=8\nc(7)=16\nc(8)=3\nc(9)=19\nc(10)=6\nc(11)=14\nc(12)=9\nc(13)=9\nc(14)=17\nc(15)=17\nc(16)=4\nc(17)=12\nc(18)=20\nc(19)=20\nc(20)=7
// Test 239: Function composition and chaining

typedef int (*unary)(int);
typedef int (*binary)(int, int);

int identity(int x) { return x; }
int inc(int x) { return x + 1; }
int dec(int x) { return x - 1; }
int double_val(int x) { return x * 2; }
int triple_val(int x) { return x * 3; }
int square(int x) { return x * x; }
int negate(int x) { return -x; }
int abs_val(int x) { if (x < 0) return -x; return x; }
int mod10(int x) { int r = x - (x / 10) * 10; if (r < 0) r = r + 10; return r; }

int apply_n_times(unary fn, int x, int n) {
    int result = x;
    int i = 0;
    while (i < n) {
        result = fn(result);
        i = i + 1;
    }
    return result;
}

int compose2(unary f, unary g, int x) {
    return f(g(x));
}

int compose3(unary f, unary g, unary h, int x) {
    return f(g(h(x)));
}

void apply_chain(int *arr, int n, unary *fns, int n_fns) {
    int i = 0;
    while (i < n) {
        int j = 0;
        while (j < n_fns) {
            arr[i] = fns[j](arr[i]);
            j = j + 1;
        }
        i = i + 1;
    }
}

int iterate_until(unary fn, int x, int target, int max_iter) {
    int val = x;
    int count = 0;
    while (val != target && count < max_iter) {
        val = fn(val);
        count = count + 1;
    }
    return count;
}

int collatz_step(int n) {
    if ((n / 2) * 2 == n) return n / 2;
    return 3 * n + 1;
}

int collatz_length(int n) {
    int count = 0;
    int val = n;
    while (val != 1) {
        val = collatz_step(val);
        count = count + 1;
    }
    return count;
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
    printf("apply inc 5 times to 0: %d\n", apply_n_times(inc, 0, 5));
    printf("apply double 4 times to 1: %d\n", apply_n_times(double_val, 1, 4));
    printf("apply dec 3 times to 10: %d\n", apply_n_times(dec, 10, 3));

    printf("compose(double,inc,5)=%d\n", compose2(double_val, inc, 5));
    printf("compose(inc,double,5)=%d\n", compose2(inc, double_val, 5));
    printf("compose3(inc,double,square,3)=%d\n", compose3(inc, double_val, square, 3));
    printf("compose3(square,inc,double,3)=%d\n", compose3(square, inc, double_val, 3));

    int arr[5];
    arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5;
    unary chain[3];
    chain[0] = inc;
    chain[1] = double_val;
    chain[2] = inc;
    printf("before chain: "); print_arr(arr, 5);
    apply_chain(arr, 5, chain, 3);
    printf("after chain: "); print_arr(arr, 5);

    printf("iterate inc from 0 to 10: %d steps\n", iterate_until(inc, 0, 10, 100));
    printf("iterate double from 1 to 16: %d steps\n", iterate_until(double_val, 1, 16, 100));

    printf("=== collatz ===\n");
    int i = 1;
    while (i <= 20) {
        printf("c(%d)=%d\n", i, collatz_length(i));
        i = i + 1;
    }

    return 0;
}
