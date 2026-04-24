int printf(const char *fmt, ...);
// EXPECT: === Advanced Pointer Arithmetic Tests ===
// EXPECT: inc_walk: 10 20 30 40 50
// EXPECT: dec_walk: 50 40 30 20 10
// EXPECT: ptr_sub: distance=4
// EXPECT: ptr_sub_neg: distance=-3
// EXPECT: sizeof_int_ptr: step=4
// EXPECT: sizeof_char_ptr: step=1
// EXPECT: cast_int_to_char: bytes=4
// EXPECT: array_of_ptrs: 10 20 30
// EXPECT: ptr_index: p[0]=10 p[1]=20 p[2]=30
// EXPECT: post_inc_ptr: first=1 second=2
// EXPECT: pre_inc_ptr: second=2
// EXPECT: stride_by_2: 0 2 4 6 8
// EXPECT: ptr_cmp_loop: count=5
// EXPECT: swap_ptrs: a=20 b=10
// EXPECT: ptr_offset_neg: val=10
// EXPECT: multi_array_ptrs: 100 200 300
// EXPECT: char_ptr_walk:h-e-l-l-o
// EXPECT: ptr_to_middle: mid=30
// EXPECT: sum_range: 90
// EXPECT: find_ptr: found 30 at index 2
// EXPECT: ptr_diff_bytes: 20
// EXPECT: reverse_via_ptrs: 5 4 3 2 1
// EXPECT: max_via_ptrs: 50
// EXPECT: copy_via_ptrs: 1 2 3 4 5
// EXPECT: scatter_gather: 10 30 50
// EXPECT: === All Advanced Pointer Arithmetic Tests Passed ===

void swap_ints(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int sum_range_ptr(int *start, int *end) {
    int s = 0;
    int *p = start;
    while (p < end) {
        s = s + *p;
        p = p + 1;
    }
    return s;
}

int *find_value(int *arr, int n, int val) {
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] == val) return arr + i;
    }
    return 0;
}

void reverse_array(int *arr, int n) {
    int *left = arr;
    int *right = arr + n - 1;
    while (left < right) {
        int tmp = *left;
        *left = *right;
        *right = tmp;
        left = left + 1;
        right = right - 1;
    }
}

int max_via_ptrs(int *arr, int n) {
    int *p = arr;
    int *end = arr + n;
    int m = *p;
    p = p + 1;
    while (p < end) {
        if (*p > m) m = *p;
        p = p + 1;
    }
    return m;
}

void copy_array(int *dst, int *src, int n) {
    int *sp = src;
    int *dp = dst;
    int *end = src + n;
    while (sp < end) {
        *dp = *sp;
        sp = sp + 1;
        dp = dp + 1;
    }
}

int main() {
    printf("=== Advanced Pointer Arithmetic Tests ===\n");

    // walk forward through array with pointer increment
    int arr[5];
    arr[0] = 10; arr[1] = 20; arr[2] = 30; arr[3] = 40; arr[4] = 50;
    printf("inc_walk:");
    int *p = arr;
    int i;
    for (i = 0; i < 5; i++) {
        printf(" %d", *p);
        p = p + 1;
    }
    printf("\n");

    // walk backward through array
    printf("dec_walk:");
    p = arr + 4;
    for (i = 0; i < 5; i++) {
        printf(" %d", *p);
        p = p - 1;
    }
    printf("\n");

    // pointer subtraction
    int *pstart = &arr[0];
    int *pend = &arr[4];
    long dist = pend - pstart;
    printf("ptr_sub: distance=%ld\n", dist);

    // negative pointer subtraction
    long neg_dist = pstart - (pstart + 3);
    printf("ptr_sub_neg: distance=%ld\n", neg_dist);

    // sizeof with pointers - step size for int
    char *c1 = (char *)&arr[0];
    char *c2 = (char *)&arr[1];
    long step = c2 - c1;
    printf("sizeof_int_ptr: step=%ld\n", step);

    // sizeof with char pointers
    char buf[5];
    buf[0] = 'a'; buf[1] = 'b'; buf[2] = 'c'; buf[3] = 'd'; buf[4] = 'e';
    char *cp1 = &buf[0];
    char *cp2 = &buf[1];
    long cstep = cp2 - cp1;
    printf("sizeof_char_ptr: step=%ld\n", cstep);

    // cast int pointer to char pointer - examine bytes
    int val = 0x04030201;
    char *bp = (char *)&val;
    // little-endian: first byte = 0x01, count non-zero bytes
    int byte_count = 0;
    for (i = 0; i < 4; i++) {
        if (bp[i] != 0) byte_count = byte_count + 1;
    }
    printf("cast_int_to_char: bytes=%d\n", byte_count);

    // array of pointers
    int x = 10;
    int y = 20;
    int z = 30;
    int *ptrs[3];
    ptrs[0] = &x;
    ptrs[1] = &y;
    ptrs[2] = &z;
    printf("array_of_ptrs: %d %d %d\n", *ptrs[0], *ptrs[1], *ptrs[2]);

    // pointer indexing (p[i] is *(p+i))
    int *pi = arr;
    printf("ptr_index: p[0]=%d p[1]=%d p[2]=%d\n", pi[0], pi[1], pi[2]);

    // post-increment pointer
    int seq[5];
    seq[0] = 1; seq[1] = 2; seq[2] = 3; seq[3] = 4; seq[4] = 5;
    int *sp = seq;
    int v1 = *sp;
    sp = sp + 1;
    int v2 = *sp;
    printf("post_inc_ptr: first=%d second=%d\n", v1, v2);

    // pre-increment pointer
    int *sp2 = seq;
    sp2 = sp2 + 1;
    printf("pre_inc_ptr: second=%d\n", *sp2);

    // stride by 2 (skip every other element)
    int data[10];
    for (i = 0; i < 10; i++) {
        data[i] = i;
    }
    printf("stride_by_2:");
    p = data;
    for (i = 0; i < 5; i++) {
        printf(" %d", *p);
        p = p + 2;
    }
    printf("\n");

    // pointer comparison in loop
    int count = 0;
    int *pp = arr;
    int *pe = arr + 5;
    while (pp < pe) {
        count = count + 1;
        pp = pp + 1;
    }
    printf("ptr_cmp_loop: count=%d\n", count);

    // swap values via pointers
    int sa = 10;
    int sb = 20;
    swap_ints(&sa, &sb);
    printf("swap_ptrs: a=%d b=%d\n", sa, sb);

    // negative offset from pointer
    int *pmid = &arr[3];
    int *pneg = pmid - 3;
    printf("ptr_offset_neg: val=%d\n", *pneg);

    // array of pointers to different arrays
    int a1[1];
    a1[0] = 100;
    int a2[1];
    a2[0] = 200;
    int a3[1];
    a3[0] = 300;
    int *multi[3];
    multi[0] = a1;
    multi[1] = a2;
    multi[2] = a3;
    printf("multi_array_ptrs: %d %d %d\n", *multi[0], *multi[1], *multi[2]);

    // char pointer walk
    char hello[6];
    hello[0] = 'h'; hello[1] = 'e'; hello[2] = 'l'; hello[3] = 'l'; hello[4] = 'o'; hello[5] = 0;
    printf("char_ptr_walk:");
    char *cw = hello;
    while (*cw != 0) {
        if (cw != hello) printf("-");
        printf("%c", *cw);
        cw = cw + 1;
    }
    printf("\n");

    // pointer to middle of array
    int mid5[5];
    mid5[0] = 10; mid5[1] = 20; mid5[2] = 30; mid5[3] = 40; mid5[4] = 50;
    int *mid_ptr = mid5 + 2;
    printf("ptr_to_middle: mid=%d\n", *mid_ptr);

    // sum of range via pointer
    int range[5];
    range[0] = 10; range[1] = 20; range[2] = 30; range[3] = 40; range[4] = 50;
    // sum elements [1..4) => 20+30+40 = 90
    printf("sum_range: %d\n", sum_range_ptr(range + 1, range + 4));

    // find value and report index
    int *found = find_value(arr, 5, 30);
    if (found != 0) {
        long fidx = found - arr;
        printf("find_ptr: found %d at index %ld\n", *found, fidx);
    }

    // pointer difference in bytes (using char cast)
    int big[6];
    big[0] = 0; big[1] = 0; big[2] = 0; big[3] = 0; big[4] = 0; big[5] = 0;
    char *b1 = (char *)&big[0];
    char *b2 = (char *)&big[5];
    long byte_diff = b2 - b1;
    printf("ptr_diff_bytes: %ld\n", byte_diff);

    // reverse array via pointer swaps
    int rev[5];
    rev[0] = 1; rev[1] = 2; rev[2] = 3; rev[3] = 4; rev[4] = 5;
    reverse_array(rev, 5);
    printf("reverse_via_ptrs: %d %d %d %d %d\n", rev[0], rev[1], rev[2], rev[3], rev[4]);

    // find max via pointer walk
    printf("max_via_ptrs: %d\n", max_via_ptrs(arr, 5));

    // copy array via pointer walk
    int csrc[5];
    csrc[0] = 1; csrc[1] = 2; csrc[2] = 3; csrc[3] = 4; csrc[4] = 5;
    int cdst[5];
    copy_array(cdst, csrc, 5);
    printf("copy_via_ptrs: %d %d %d %d %d\n", cdst[0], cdst[1], cdst[2], cdst[3], cdst[4]);

    // scatter/gather - use index array to pick elements
    int values[5];
    values[0] = 10; values[1] = 20; values[2] = 30; values[3] = 40; values[4] = 50;
    int indices[3];
    indices[0] = 0; indices[1] = 2; indices[2] = 4;
    printf("scatter_gather:");
    for (i = 0; i < 3; i++) {
        printf(" %d", values[indices[i]]);
    }
    printf("\n");

    printf("=== All Advanced Pointer Arithmetic Tests Passed ===\n");
    return 0;
}
