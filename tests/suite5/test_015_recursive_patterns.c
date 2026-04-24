int printf(const char *fmt, ...);

int is_even(int n);
int is_odd(int n);

int is_even(int n) {
    if (n < 0) n = -n;
    if (n == 0) return 1;
    return is_odd(n - 1);
}

int is_odd(int n) {
    if (n < 0) n = -n;
    if (n == 0) return 0;
    return is_even(n - 1);
}

int ack(int m, int n) {
    if (m == 0) return n + 1;
    if (n == 0) return ack(m - 1, 1);
    return ack(m - 1, ack(m, n - 1));
}

int fib_memo_arr[30];
int fib_memo_init;

void init_fib_memo(void) {
    int i = 0;
    while (i < 30) {
        fib_memo_arr[i] = -1;
        i = i + 1;
    }
    fib_memo_init = 1;
}

int fib_memo(int n) {
    if (n <= 1) return n;
    if (fib_memo_arr[n] != -1) return fib_memo_arr[n];
    fib_memo_arr[n] = fib_memo(n - 1) + fib_memo(n - 2);
    return fib_memo_arr[n];
}

int power(int base, int exp) {
    if (exp == 0) return 1;
    if (exp % 2 == 0) {
        int half = power(base, exp / 2);
        return half * half;
    }
    return base * power(base, exp - 1);
}

int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int flatten_sum(int arr[], int n) {
    if (n == 0) return 0;
    if (n == 1) return arr[0];
    int mid = n / 2;
    return flatten_sum(arr, mid) + flatten_sum(arr + mid, n - mid);
}

int binary_search_rec(int arr[], int lo, int hi, int target) {
    if (lo > hi) return -1;
    int mid = lo + (hi - lo) / 2;
    if (arr[mid] == target) return mid;
    if (arr[mid] < target) return binary_search_rec(arr, mid + 1, hi, target);
    return binary_search_rec(arr, lo, mid - 1, target);
}

int max_depth(int tree[], int idx, int n) {
    if (idx >= n) return 0;
    if (tree[idx] == 0) return 0;
    int left = max_depth(tree, 2 * idx + 1, n);
    int right = max_depth(tree, 2 * idx + 2, n);
    if (left > right) return left + 1;
    return right + 1;
}

int count_paths(int grid[], int rows, int cols,
                int r, int c) {
    if (r == rows - 1 && c == cols - 1) return 1;
    int paths = 0;
    if (r + 1 < rows && grid[(r+1)*cols + c] == 0)
        paths = paths + count_paths(grid, rows, cols, r+1, c);
    if (c + 1 < cols && grid[r*cols + (c+1)] == 0)
        paths = paths + count_paths(grid, rows, cols, r, c+1);
    return paths;
}

int main(void) {
    // EXPECT: even(4)=1
    printf("even(4)=%d\n", is_even(4));
    // EXPECT: even(7)=0
    printf("even(7)=%d\n", is_even(7));
    // EXPECT: odd(3)=1
    printf("odd(3)=%d\n", is_odd(3));
    // EXPECT: odd(10)=0
    printf("odd(10)=%d\n", is_odd(10));

    // EXPECT: ack(2,3)=9
    printf("ack(2,3)=%d\n", ack(2, 3));
    // EXPECT: ack(3,2)=29
    printf("ack(3,2)=%d\n", ack(3, 2));

    init_fib_memo();
    // EXPECT: fib(10)=55
    printf("fib(10)=%d\n", fib_memo(10));
    // EXPECT: fib(15)=610
    printf("fib(15)=%d\n", fib_memo(15));

    // EXPECT: pow(2,10)=1024
    printf("pow(2,10)=%d\n", power(2, 10));
    // EXPECT: pow(3,5)=243
    printf("pow(3,5)=%d\n", power(3, 5));

    // EXPECT: gcd(48,18)=6
    printf("gcd(48,18)=%d\n", gcd(48, 18));

    int arr[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    // EXPECT: flatsum=36
    printf("flatsum=%d\n", flatten_sum(arr, 8));

    int sorted[7] = {2, 5, 8, 12, 16, 23, 38};
    // EXPECT: bsearch(12)=3
    printf("bsearch(12)=%d\n", binary_search_rec(sorted, 0, 6, 12));
    // EXPECT: bsearch(99)=-1
    printf("bsearch(99)=%d\n", binary_search_rec(sorted, 0, 6, 99));

    int tree[7] = {1, 1, 1, 1, 0, 0, 1};
    // EXPECT: depth=3
    printf("depth=%d\n", max_depth(tree, 0, 7));

    int grid[9] = {0,0,0, 0,0,0, 0,0,0};
    // EXPECT: paths(3x3)=6
    printf("paths(3x3)=%d\n", count_paths(grid, 3, 3, 0, 0));

    return 0;
}
