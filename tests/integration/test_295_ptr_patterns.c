int printf(const char *fmt, ...);
// EXPECT: min=-5 max=8\nbefore partition: 3 -1 7 -5 2 -3 8 -2\nafter partition: -2 -1 -3 -5 2 7 8 3\nnegatives: 4\ninterleaved: 1 2 3 4 5 6\nunequal interleave: 10 20 30 40 50 60\nevery 2nd: 10 30 50 70\nevery 3rd: 10 40 70
// Test: advanced pointer manipulation patterns

void print_arr(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

// Find min and max in one pass using pointers
void min_max(int *arr, int n, int *min_out, int *max_out) {
    *min_out = arr[0];
    *max_out = arr[0];
    int *p = arr + 1;
    int *end = arr + n;
    while (p < end) {
        if (*p < *min_out) *min_out = *p;
        if (*p > *max_out) *max_out = *p;
        p = p + 1;
    }
}

// Partition into negatives and positives
int partition_sign(int *arr, int n) {
    int *lo = arr;
    int *hi = arr + n - 1;
    while (lo < hi) {
        while (lo < hi && *lo < 0) lo = lo + 1;
        while (lo < hi && *hi >= 0) hi = hi - 1;
        if (lo < hi) {
            int tmp = *lo;
            *lo = *hi;
            *hi = tmp;
            lo = lo + 1;
            hi = hi - 1;
        }
    }
    // Count negatives
    int cnt = 0;
    int *p = arr;
    while (p < arr + n && *p < 0) {
        cnt = cnt + 1;
        p = p + 1;
    }
    return cnt;
}

// Interleave two arrays using pointers
void interleave(int *a, int na, int *b, int nb, int *out) {
    int *pa = a; int *pb = b;
    int *po = out;
    int *ea = a + na; int *eb = b + nb;
    while (pa < ea && pb < eb) {
        *po = *pa; po = po + 1; pa = pa + 1;
        *po = *pb; po = po + 1; pb = pb + 1;
    }
    while (pa < ea) { *po = *pa; po = po + 1; pa = pa + 1; }
    while (pb < eb) { *po = *pb; po = po + 1; pb = pb + 1; }
}

// Copy every nth element
int copy_nth(int *src, int n, int step, int *dst) {
    int cnt = 0;
    int *p = src;
    int *end = src + n;
    while (p < end) {
        dst[cnt] = *p;
        cnt = cnt + 1;
        p = p + step;
    }
    return cnt;
}

int main(void) {
    int arr[8];
    arr[0]=3; arr[1]=-1; arr[2]=7; arr[3]=-5;
    arr[4]=2; arr[5]=-3; arr[6]=8; arr[7]=-2;

    // Min/max
    int mn; int mx;
    min_max(arr, 8, &mn, &mx);
    printf("min=%d max=%d\n", mn, mx);

    // Partition by sign
    printf("before partition: ");
    print_arr(arr, 8);
    int neg_count = partition_sign(arr, 8);
    printf("after partition: ");
    print_arr(arr, 8);
    printf("negatives: %d\n", neg_count);

    // Interleave
    int a[3]; a[0]=1; a[1]=3; a[2]=5;
    int b[3]; b[0]=2; b[1]=4; b[2]=6;
    int inter[6];
    interleave(a, 3, b, 3, inter);
    printf("interleaved: ");
    print_arr(inter, 6);

    // Unequal interleave
    int c[2]; c[0]=10; c[1]=30;
    int d[4]; d[0]=20; d[1]=40; d[2]=50; d[3]=60;
    int inter2[6];
    interleave(c, 2, d, 4, inter2);
    printf("unequal interleave: ");
    print_arr(inter2, 6);

    // Copy every 2nd
    int src[8];
    int i;
    for (i = 0; i < 8; i++) src[i] = (i + 1) * 10;
    int dst[4];
    int cnt = copy_nth(src, 8, 2, dst);
    printf("every 2nd: ");
    print_arr(dst, cnt);

    // Copy every 3rd
    int dst2[3];
    cnt = copy_nth(src, 8, 3, dst2);
    printf("every 3rd: ");
    print_arr(dst2, cnt);

    return 0;
}
