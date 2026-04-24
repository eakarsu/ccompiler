int printf(const char *fmt, ...);
// EXPECT: === fibonacci ===\nf(0)=0 f(1)=1 f(2)=1 f(3)=2 f(4)=3 f(5)=5 f(6)=8 f(7)=13 f(8)=21 f(9)=34 f(10)=55 f(11)=89 f(12)=144 f(13)=233 f(14)=377 f(15)=610 \n=== gcd ===\ngcd(48,18)=6 gcd(100,75)=25 gcd(13,7)=1\n=== struct pairs ===\np1_sum=30 p2_sum=70\n=== fold ===\nsum=39\nproduct=60480\n=== map ===\ndoubled: 10 6 16 2 18 4 14 8\nsquared: 25 9 64 1 81 4 49 16\n=== count ===\nevens=3\n=== primes 1-30 ===\n2 3 5 7 11 13 17 19 23 29 \n=== sort & search ===\nsorted: 1 2 3 4 5 7 8 9\nfind(5)=4 find(6)=-1\n=== power mod ===\n2^10 mod 1000=24\n3^7 mod 100=87\n7^5 mod 13=11\n=== complex expression ===\nsum=39 n_even=3
// Test 250: Comprehensive stress test combining all patterns

typedef int (*unary_fn)(int);
typedef int (*binary_fn)(int, int);

int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }
int sub(int a, int b) { return a - b; }

int inc(int x) { return x + 1; }
int dbl(int x) { return x * 2; }
int sqr(int x) { return x * x; }
int neg(int x) { return -x; }

struct Pair {
    int first;
    int second;
};

struct Pair make_pair(int a, int b) {
    struct Pair p;
    p.first = a;
    p.second = b;
    return p;
}

int pair_sum(struct Pair p) {
    return p.first + p.second;
}

int gcd(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) { int t = b; b = a - (a / b) * b; a = t; }
    return a;
}

int fib(int n) {
    if (n <= 1) return n;
    int a = 0; int b = 1;
    int i = 2;
    while (i <= n) { int c = a + b; a = b; b = c; i = i + 1; }
    return b;
}

int fold(int *arr, int n, binary_fn fn, int init) {
    int acc = init;
    int i = 0;
    while (i < n) { acc = fn(acc, arr[i]); i = i + 1; }
    return acc;
}

void map_inplace(int *arr, int n, unary_fn fn) {
    int i = 0;
    while (i < n) { arr[i] = fn(arr[i]); i = i + 1; }
}

int count_where(int *arr, int n, int (*pred)(int)) {
    int c = 0;
    int i = 0;
    while (i < n) { if (pred(arr[i])) c = c + 1; i = i + 1; }
    return c;
}

int is_even(int x) { return (x / 2) * 2 == x; }
int is_prime(int x) {
    if (x < 2) return 0;
    if (x == 2) return 1;
    if (is_even(x)) return 0;
    int i = 3;
    while (i * i <= x) { if ((x / i) * i == x) return 0; i = i + 2; }
    return 1;
}

void swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }

void sort_arr(int *arr, int n) {
    int i = 0;
    while (i < n - 1) {
        int min_idx = i;
        int j = i + 1;
        while (j < n) { if (arr[j] < arr[min_idx]) min_idx = j; j = j + 1; }
        swap(&arr[i], &arr[min_idx]);
        i = i + 1;
    }
}

int binary_search(int *arr, int n, int target) {
    int lo = 0; int hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

int power_mod(int base, int exp, int mod) {
    int result = 1;
    base = base - (base / mod) * mod;
    while (exp > 0) {
        if ((exp / 2) * 2 != exp)
            result = (result * base) - ((result * base) / mod) * mod;
        exp = exp / 2;
        base = (base * base) - ((base * base) / mod) * mod;
    }
    return result;
}

void print_arr(int *arr, int n) {
    int i = 0;
    while (i < n) { if (i > 0) printf(" "); printf("%d", arr[i]); i = i + 1; }
    printf("\n");
}

int main(void) {
    printf("=== fibonacci ===\n");
    int i = 0;
    while (i <= 15) { printf("f(%d)=%d ", i, fib(i)); i = i + 1; }
    printf("\n");

    printf("=== gcd ===\n");
    printf("gcd(48,18)=%d gcd(100,75)=%d gcd(13,7)=%d\n", gcd(48,18), gcd(100,75), gcd(13,7));

    printf("=== struct pairs ===\n");
    struct Pair p1 = make_pair(10, 20);
    struct Pair p2 = make_pair(30, 40);
    printf("p1_sum=%d p2_sum=%d\n", pair_sum(p1), pair_sum(p2));

    printf("=== fold ===\n");
    int arr[8];
    arr[0]=5; arr[1]=3; arr[2]=8; arr[3]=1; arr[4]=9; arr[5]=2; arr[6]=7; arr[7]=4;
    printf("sum=%d\n", fold(arr, 8, add, 0));
    printf("product=%d\n", fold(arr, 8, mul, 1));

    printf("=== map ===\n");
    int work[8];
    i = 0; while (i < 8) { work[i] = arr[i]; i = i + 1; }
    map_inplace(work, 8, dbl);
    printf("doubled: "); print_arr(work, 8);

    i = 0; while (i < 8) { work[i] = arr[i]; i = i + 1; }
    map_inplace(work, 8, sqr);
    printf("squared: "); print_arr(work, 8);

    printf("=== count ===\n");
    printf("evens=%d\n", count_where(arr, 8, is_even));

    printf("=== primes 1-30 ===\n");
    i = 1;
    while (i <= 30) { if (is_prime(i)) printf("%d ", i); i = i + 1; }
    printf("\n");

    printf("=== sort & search ===\n");
    i = 0; while (i < 8) { work[i] = arr[i]; i = i + 1; }
    sort_arr(work, 8);
    printf("sorted: "); print_arr(work, 8);
    printf("find(5)=%d find(6)=%d\n", binary_search(work, 8, 5), binary_search(work, 8, 6));

    printf("=== power mod ===\n");
    printf("2^10 mod 1000=%d\n", power_mod(2, 10, 1000));
    printf("3^7 mod 100=%d\n", power_mod(3, 7, 100));
    printf("7^5 mod 13=%d\n", power_mod(7, 5, 13));

    printf("=== complex expression ===\n");
    int result = fold(arr, 8, add, 0);
    int max_val = fold(arr, 8, mul, 1);
    struct Pair stats = make_pair(result, count_where(arr, 8, is_even));
    printf("sum=%d n_even=%d\n", stats.first, stats.second);

    return 0;
}
