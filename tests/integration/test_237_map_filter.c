int printf(const char *fmt, ...);
// EXPECT: source: 1 2 3 4 5 6 7 8 9 10\ndoubled: 2 4 6 8 10 12 14 16 18 20\nsquared: 1 4 9 16 25 36 49 64 81 100\nevens: 2 4 6 8 10\nodds: 1 3 5 7 9\nprimes: 2 3 5 7\ngt5: 6 7 8 9 10\nmap_double_then_even=10\nmap_add1_then_even=5\nfilter_even_map_sq=220\nfilter_odd_map_dbl=50\nfilter_prime_map_sq=87\npositives: 7 4 2 5\nnegated: 3 -7 1 -4 0 8 -2 -5
// Test 237: Map and filter patterns using arrays

typedef int (*mapper)(int);
typedef int (*predicate)(int);

int is_even(int x) { return (x / 2) * 2 == x; }
int is_odd(int x) { return !is_even(x); }
int is_positive(int x) { return x > 0; }
int is_gt_5(int x) { return x > 5; }
int is_prime(int x) {
    if (x < 2) return 0;
    if (x == 2) return 1;
    if (is_even(x)) return 0;
    int i = 3;
    while (i * i <= x) {
        if ((x / i) * i == x) return 0;
        i = i + 2;
    }
    return 1;
}

int double_val(int x) { return x * 2; }
int square(int x) { return x * x; }
int add_one(int x) { return x + 1; }
int negate(int x) { return -x; }

void map(int *src, int *dst, int n, mapper fn) {
    int i = 0;
    while (i < n) {
        dst[i] = fn(src[i]);
        i = i + 1;
    }
}

int filter(int *src, int *dst, int n, predicate pred) {
    int count = 0;
    int i = 0;
    while (i < n) {
        if (pred(src[i])) {
            dst[count] = src[i];
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

int map_filter_count(int *arr, int n, mapper fn, predicate pred) {
    int count = 0;
    int i = 0;
    while (i < n) {
        if (pred(fn(arr[i]))) count = count + 1;
        i = i + 1;
    }
    return count;
}

int filter_map_sum(int *arr, int n, predicate pred, mapper fn) {
    int sum = 0;
    int i = 0;
    while (i < n) {
        if (pred(arr[i])) {
            sum = sum + fn(arr[i]);
        }
        i = i + 1;
    }
    return sum;
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
    int src[10];
    src[0] = 1; src[1] = 2; src[2] = 3; src[3] = 4; src[4] = 5;
    src[5] = 6; src[6] = 7; src[7] = 8; src[8] = 9; src[9] = 10;
    int dst[10];

    printf("source: "); print_arr(src, 10);

    map(src, dst, 10, double_val);
    printf("doubled: "); print_arr(dst, 10);

    map(src, dst, 10, square);
    printf("squared: "); print_arr(dst, 10);

    int cnt = filter(src, dst, 10, is_even);
    printf("evens: "); print_arr(dst, cnt);

    cnt = filter(src, dst, 10, is_odd);
    printf("odds: "); print_arr(dst, cnt);

    cnt = filter(src, dst, 10, is_prime);
    printf("primes: "); print_arr(dst, cnt);

    cnt = filter(src, dst, 10, is_gt_5);
    printf("gt5: "); print_arr(dst, cnt);

    printf("map_double_then_even=%d\n", map_filter_count(src, 10, double_val, is_even));
    printf("map_add1_then_even=%d\n", map_filter_count(src, 10, add_one, is_even));

    printf("filter_even_map_sq=%d\n", filter_map_sum(src, 10, is_even, square));
    printf("filter_odd_map_dbl=%d\n", filter_map_sum(src, 10, is_odd, double_val));
    printf("filter_prime_map_sq=%d\n", filter_map_sum(src, 10, is_prime, square));

    int mixed[8];
    mixed[0] = -3; mixed[1] = 7; mixed[2] = -1; mixed[3] = 4;
    mixed[4] = 0; mixed[5] = -8; mixed[6] = 2; mixed[7] = 5;
    cnt = filter(mixed, dst, 8, is_positive);
    printf("positives: "); print_arr(dst, cnt);

    map(mixed, dst, 8, negate);
    printf("negated: "); print_arr(dst, 8);

    return 0;
}
