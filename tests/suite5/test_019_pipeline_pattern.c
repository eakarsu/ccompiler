int printf(const char *fmt, ...);

typedef int (*MapFn)(int);
typedef int (*FilterFn)(int);
typedef int (*ReduceFn)(int, int);

int double_val(int x) { return x * 2; }
int square_val(int x) { return x * x; }
int add_ten(int x) { return x + 10; }
int negate_val(int x) { return -x; }

int is_positive(int x) { return x > 0; }
int is_even(int x) { return (x % 2) == 0; }
int is_less_20(int x) { return x < 20; }

int reduce_add(int acc, int x) { return acc + x; }
int reduce_mul(int acc, int x) { return acc * x; }
int reduce_max(int acc, int x) {
    if (x > acc) return x;
    return acc;
}
int reduce_min(int acc, int x) {
    if (x < acc) return x;
    return acc;
}

void map_arr(int dst[], int src[], int n, MapFn fn) {
    int i = 0;
    while (i < n) {
        dst[i] = fn(src[i]);
        i = i + 1;
    }
}

int filter_arr(int dst[], int src[], int n, FilterFn fn) {
    int count = 0;
    int i = 0;
    while (i < n) {
        if (fn(src[i])) {
            dst[count] = src[i];
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

int reduce_arr(int arr[], int n, int init, ReduceFn fn) {
    int acc = init;
    int i = 0;
    while (i < n) {
        acc = fn(acc, arr[i]);
        i = i + 1;
    }
    return acc;
}

int pipeline_map_filter_reduce(int src[], int n,
                               MapFn m, FilterFn f,
                               ReduceFn r, int init) {
    int mapped[20];
    int filtered[20];
    map_arr(mapped, src, n, m);
    int fcount = filter_arr(filtered, mapped, n, f);
    return reduce_arr(filtered, fcount, init, r);
}

void multi_map(int arr[], int n,
               MapFn fns[], int nfns) {
    int f = 0;
    while (f < nfns) {
        int i = 0;
        while (i < n) {
            arr[i] = fns[f](arr[i]);
            i = i + 1;
        }
        f = f + 1;
    }
}

int any_match(int arr[], int n, FilterFn pred) {
    int i = 0;
    while (i < n) {
        if (pred(arr[i])) return 1;
        i = i + 1;
    }
    return 0;
}

int all_match(int arr[], int n, FilterFn pred) {
    int i = 0;
    while (i < n) {
        if (pred(arr[i]) == 0) return 0;
        i = i + 1;
    }
    return 1;
}

int count_match(int arr[], int n, FilterFn pred) {
    int cnt = 0;
    int i = 0;
    while (i < n) {
        if (pred(arr[i])) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

int main(void) {
    int a[5] = {1, 2, 3, 4, 5};
    int b[5];

    map_arr(b, a, 5, double_val);
    // EXPECT: map_d[0]=2
    printf("map_d[0]=%d\n", b[0]);
    // EXPECT: map_d[4]=10
    printf("map_d[4]=%d\n", b[4]);

    map_arr(b, a, 5, square_val);
    // EXPECT: map_s[2]=9
    printf("map_s[2]=%d\n", b[2]);

    int c[6] = {-3, 5, -1, 8, 0, 2};
    int d[6];
    int fcount = filter_arr(d, c, 6, is_positive);
    // EXPECT: filt_cnt=3
    printf("filt_cnt=%d\n", fcount);
    // EXPECT: filt[0]=5
    printf("filt[0]=%d\n", d[0]);
    // EXPECT: filt[2]=2
    printf("filt[2]=%d\n", d[2]);

    int e[5] = {1, 2, 3, 4, 5};
    // EXPECT: sum=15
    printf("sum=%d\n", reduce_arr(e, 5, 0, reduce_add));
    // EXPECT: prod=120
    printf("prod=%d\n", reduce_arr(e, 5, 1, reduce_mul));
    // EXPECT: max=5
    printf("max=%d\n", reduce_arr(e, 5, e[0], reduce_max));
    // EXPECT: min=1
    printf("min=%d\n", reduce_arr(e, 5, e[0], reduce_min));

    int f[6] = {1, 2, 3, 4, 5, 6};
    int result = pipeline_map_filter_reduce(
        f, 6, double_val, is_less_20, reduce_add, 0);
    // EXPECT: pipeline=42
    printf("pipeline=%d\n", result);

    int g[4] = {1, 2, 3, 4};
    MapFn chain[2];
    chain[0] = double_val;
    chain[1] = add_ten;
    multi_map(g, 4, chain, 2);
    // EXPECT: multi[0]=12
    printf("multi[0]=%d\n", g[0]);
    // EXPECT: multi[3]=18
    printf("multi[3]=%d\n", g[3]);

    int h[4] = {2, 4, 6, 8};
    // EXPECT: any_pos=1
    printf("any_pos=%d\n", any_match(h, 4, is_positive));
    // EXPECT: all_even=1
    printf("all_even=%d\n", all_match(h, 4, is_even));
    // EXPECT: cnt_lt20=4
    printf("cnt_lt20=%d\n", count_match(h, 4, is_less_20));

    return 0;
}
