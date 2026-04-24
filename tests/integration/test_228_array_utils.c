int printf(const char *fmt, ...);
// EXPECT: arr: 3 1 4 1 5 9 2 6\nsum=31\nproduct=6480\nmax=9\nmin=1\nfind(5)=4\nfind(7)=-1\ncount(1)=2\nreversed: 6 2 9 5 1 4 1 3\nrot_left_3: 1 5 9 2 6 3 1 4\nfilled: 42 42 42 42 42\nequal(d,e)=1\nequal(d,f)=0
// Test 228: Helper function library - array utilities

void print_arr(int *arr, int n) {
    int i = 0;
    while (i < n) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
        i = i + 1;
    }
    printf("\n");
}

int arr_sum(int *arr, int n) {
    int s = 0;
    int i = 0;
    while (i < n) { s = s + arr[i]; i = i + 1; }
    return s;
}

int arr_product(int *arr, int n) {
    int p = 1;
    int i = 0;
    while (i < n) { p = p * arr[i]; i = i + 1; }
    return p;
}

int arr_max(int *arr, int n) {
    int m = arr[0];
    int i = 1;
    while (i < n) { if (arr[i] > m) m = arr[i]; i = i + 1; }
    return m;
}

int arr_min(int *arr, int n) {
    int m = arr[0];
    int i = 1;
    while (i < n) { if (arr[i] < m) m = arr[i]; i = i + 1; }
    return m;
}

int arr_find(int *arr, int n, int target) {
    int i = 0;
    while (i < n) {
        if (arr[i] == target) return i;
        i = i + 1;
    }
    return -1;
}

int arr_count(int *arr, int n, int target) {
    int c = 0;
    int i = 0;
    while (i < n) {
        if (arr[i] == target) c = c + 1;
        i = i + 1;
    }
    return c;
}

void arr_reverse(int *arr, int n) {
    int i = 0;
    int j = n - 1;
    while (i < j) {
        int t = arr[i]; arr[i] = arr[j]; arr[j] = t;
        i = i + 1; j = j - 1;
    }
}

void arr_rotate_left(int *arr, int n, int k) {
    int kk = k;
    while (kk < 0) kk = kk + n;
    kk = kk - (kk / n) * n;
    arr_reverse(arr, kk);
    arr_reverse(arr + kk, n - kk);
    arr_reverse(arr, n);
}

void arr_fill(int *arr, int n, int val) {
    int i = 0;
    while (i < n) { arr[i] = val; i = i + 1; }
}

void arr_copy(int *dst, int *src, int n) {
    int i = 0;
    while (i < n) { dst[i] = src[i]; i = i + 1; }
}

int arr_equal(int *a, int *b, int n) {
    int i = 0;
    while (i < n) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return 1;
}

int main(void) {
    int a[8];
    a[0] = 3; a[1] = 1; a[2] = 4; a[3] = 1;
    a[4] = 5; a[5] = 9; a[6] = 2; a[7] = 6;

    printf("arr: "); print_arr(a, 8);
    printf("sum=%d\n", arr_sum(a, 8));
    printf("product=%d\n", arr_product(a, 8));
    printf("max=%d\n", arr_max(a, 8));
    printf("min=%d\n", arr_min(a, 8));
    printf("find(5)=%d\n", arr_find(a, 8, 5));
    printf("find(7)=%d\n", arr_find(a, 8, 7));
    printf("count(1)=%d\n", arr_count(a, 8, 1));

    int b[8];
    arr_copy(b, a, 8);
    arr_reverse(b, 8);
    printf("reversed: "); print_arr(b, 8);

    arr_copy(b, a, 8);
    arr_rotate_left(b, 8, 3);
    printf("rot_left_3: "); print_arr(b, 8);

    int c[5];
    arr_fill(c, 5, 42);
    printf("filled: "); print_arr(c, 5);

    int d[3];
    d[0] = 1; d[1] = 2; d[2] = 3;
    int e[3];
    e[0] = 1; e[1] = 2; e[2] = 3;
    int f[3];
    f[0] = 1; f[1] = 2; f[2] = 4;
    printf("equal(d,e)=%d\n", arr_equal(d, e, 3));
    printf("equal(d,f)=%d\n", arr_equal(d, f, 3));

    return 0;
}
