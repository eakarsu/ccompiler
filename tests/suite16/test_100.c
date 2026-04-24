int printf(const char *fmt, ...);

// Test 100: Comprehensive functional pipeline: map/filter/reduce with
//           virtual dispatch, option type, and pipeline combinator

// Option type (Maybe monad)
struct Option {
    int value;
    int has_value;
};

struct Option option_some(int val) {
    struct Option o;
    o.value = val;
    o.has_value = 1;
    return o;
}

struct Option option_none(void) {
    struct Option o;
    o.value = 0;
    o.has_value = 0;
    return o;
}

int option_get_or(struct Option o, int fallback) {
    if (o.has_value) return o.value;
    return fallback;
}

typedef struct Option (*OptionFn)(int);

struct Option option_bind(struct Option o, OptionFn fn) {
    if (!o.has_value) return o;
    return fn(o.value);
}

struct Option option_map_int(struct Option o, int (*fn)(int)) {
    if (!o.has_value) return o;
    return option_some(fn(o.value));
}

// Virtual dispatch table for transformations
typedef int (*Transform)(int);
typedef int (*Predicate)(int);

struct Pipeline {
    int kind[16];      // 0=map, 1=filter
    Transform maps[16];
    Predicate filters[16];
    int steps;
};

void pipeline_init(struct Pipeline *p) {
    p->steps = 0;
}

void pipeline_add_map(struct Pipeline *p, Transform fn) {
    p->kind[p->steps] = 0;
    p->maps[p->steps] = fn;
    p->steps = p->steps + 1;
}

void pipeline_add_filter(struct Pipeline *p, Predicate fn) {
    p->kind[p->steps] = 1;
    p->filters[p->steps] = fn;
    p->steps = p->steps + 1;
}

// Run pipeline: apply all map/filter steps, collect results
int pipeline_run(struct Pipeline *p, int *src, int len, int *dst) {
    int buf1[32];
    int buf2[32];
    int *cur;
    int *nxt;
    int cur_len;
    int nxt_len;
    int i;
    int s;

    cur = buf1;
    nxt = buf2;
    for (i = 0; i < len; i++) {
        cur[i] = src[i];
    }
    cur_len = len;

    for (s = 0; s < p->steps; s++) {
        nxt_len = 0;
        if (p->kind[s] == 0) {
            // map
            for (i = 0; i < cur_len; i++) {
                nxt[nxt_len] = p->maps[s](cur[i]);
                nxt_len = nxt_len + 1;
            }
        } else {
            // filter
            for (i = 0; i < cur_len; i++) {
                if (p->filters[s](cur[i])) {
                    nxt[nxt_len] = cur[i];
                    nxt_len = nxt_len + 1;
                }
            }
        }
        // swap buffers
        if (nxt == buf2) {
            cur = buf2;
            nxt = buf1;
        } else {
            cur = buf1;
            nxt = buf2;
        }
        cur_len = nxt_len;
    }

    for (i = 0; i < cur_len; i++) {
        dst[i] = cur[i];
    }
    return cur_len;
}

// Reduce helper
int reduce(int *arr, int len, int init, int (*fn)(int, int)) {
    int i;
    int acc;
    acc = init;
    for (i = 0; i < len; i++) {
        acc = fn(acc, arr[i]);
    }
    return acc;
}

// Helper functions
int dbl(int x) { return x * 2; }
int sqr(int x) { return x * x; }
int inc(int x) { return x + 1; }
int neg(int x) { return -x; }
int add3(int x) { return x + 3; }

int is_even(int x) { return x % 2 == 0; }
int is_positive(int x) { return x > 0; }
int is_lt_50(int x) { return x < 50; }
int is_gt_5(int x) { return x > 5; }

int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }

// Safe operations returning Option
struct Option safe_div_opt(int x) {
    if (x == 0) return option_none();
    return option_some(100 / x);
}

struct Option check_range(int x) {
    if (x < 0 || x > 1000) return option_none();
    return option_some(x);
}

void print_array(const char *label, int *arr, int len) {
    int i;
    printf("%s:", label);
    for (i = 0; i < len; i++) {
        printf(" %d", arr[i]);
    }
    printf("\n");
}

int main(void) {
    struct Pipeline pipe;
    struct Option opt;
    int src[10];
    int dst[32];
    int n;
    int i;

    src[0] = 1; src[1] = 2; src[2] = 3; src[3] = 4; src[4] = 5;
    src[5] = 6; src[6] = 7; src[7] = 8; src[8] = 9; src[9] = 10;

    // Pipeline 1: double all
    pipeline_init(&pipe);
    pipeline_add_map(&pipe, dbl);
    n = pipeline_run(&pipe, src, 10, dst);
    print_array("p_dbl", dst, n); // EXPECT: p_dbl: 2 4 6 8 10 12 14 16 18 20

    // Pipeline 2: filter evens
    pipeline_init(&pipe);
    pipeline_add_filter(&pipe, is_even);
    n = pipeline_run(&pipe, src, 10, dst);
    print_array("p_even", dst, n); // EXPECT: p_even: 2 4 6 8 10

    // Pipeline 3: filter evens, then double
    pipeline_init(&pipe);
    pipeline_add_filter(&pipe, is_even);
    pipeline_add_map(&pipe, dbl);
    n = pipeline_run(&pipe, src, 10, dst);
    print_array("p_ev_dbl", dst, n); // EXPECT: p_ev_dbl: 4 8 12 16 20

    // Pipeline 4: square, filter < 50
    pipeline_init(&pipe);
    pipeline_add_map(&pipe, sqr);
    pipeline_add_filter(&pipe, is_lt_50);
    n = pipeline_run(&pipe, src, 10, dst);
    print_array("p_sq_lt50", dst, n); // EXPECT: p_sq_lt50: 1 4 9 16 25 36 49

    // Pipeline 5: inc, filter even, double, filter > 5
    pipeline_init(&pipe);
    pipeline_add_map(&pipe, inc);
    pipeline_add_filter(&pipe, is_even);
    pipeline_add_map(&pipe, dbl);
    pipeline_add_filter(&pipe, is_gt_5);
    n = pipeline_run(&pipe, src, 10, dst);
    print_array("p_complex", dst, n); // EXPECT: p_complex: 8 12 16 20

    // Pipeline + reduce: sum of (double of evens)
    pipeline_init(&pipe);
    pipeline_add_filter(&pipe, is_even);
    pipeline_add_map(&pipe, dbl);
    n = pipeline_run(&pipe, src, 10, dst);
    printf("sum_dbl_ev: %d\n", reduce(dst, n, 0, add)); // EXPECT: sum_dbl_ev: 60

    // Pipeline + reduce: product of first 4
    pipeline_init(&pipe);
    pipeline_add_map(&pipe, inc);
    n = pipeline_run(&pipe, src, 4, dst);
    printf("prod_inc4: %d\n", reduce(dst, n, 1, mul)); // EXPECT: prod_inc4: 120

    // Option type tests
    opt = option_some(42);
    printf("some42: %d\n", option_get_or(opt, -1)); // EXPECT: some42: 42

    opt = option_none();
    printf("none: %d\n", option_get_or(opt, -1)); // EXPECT: none: -1

    opt = option_bind(option_some(5), safe_div_opt);
    printf("div5: %d\n", option_get_or(opt, -1)); // EXPECT: div5: 20

    opt = option_bind(option_some(0), safe_div_opt);
    printf("div0: %d\n", option_get_or(opt, -1)); // EXPECT: div0: -1

    opt = option_bind(option_none(), safe_div_opt);
    printf("none_div: %d\n", option_get_or(opt, -1)); // EXPECT: none_div: -1

    // Option chained: check_range then safe_div_opt
    opt = option_bind(option_some(50), check_range);
    opt = option_bind(opt, safe_div_opt);
    printf("chain50: %d\n", option_get_or(opt, -1)); // EXPECT: chain50: 2

    opt = option_bind(option_some(-5), check_range);
    opt = option_bind(opt, safe_div_opt);
    printf("chain_neg: %d\n", option_get_or(opt, -1)); // EXPECT: chain_neg: -1

    // Option map
    opt = option_map_int(option_some(7), sqr);
    printf("map_sq7: %d\n", option_get_or(opt, -1)); // EXPECT: map_sq7: 49

    opt = option_map_int(option_none(), sqr);
    printf("map_none: %d\n", option_get_or(opt, -1)); // EXPECT: map_none: -1

    // Combined: pipeline on negative numbers
    src[0] = -3; src[1] = -2; src[2] = -1; src[3] = 0;
    src[4] = 1; src[5] = 2; src[6] = 3;
    pipeline_init(&pipe);
    pipeline_add_filter(&pipe, is_positive);
    pipeline_add_map(&pipe, sqr);
    n = pipeline_run(&pipe, src, 7, dst);
    print_array("pos_sq", dst, n); // EXPECT: pos_sq: 1 4 9
    printf("sum_pos_sq: %d\n", reduce(dst, n, 0, add)); // EXPECT: sum_pos_sq: 14

    printf("done\n"); // EXPECT: done
    return 0;
}
