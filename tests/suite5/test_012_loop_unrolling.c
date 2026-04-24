int printf(const char *fmt, ...);

int sum_unrolled_by2(int arr[], int n) {
    int sum = 0;
    int i = 0;
    while (i + 1 < n) {
        sum = sum + arr[i] + arr[i + 1];
        i = i + 2;
    }
    while (i < n) {
        sum = sum + arr[i];
        i = i + 1;
    }
    return sum;
}

int sum_unrolled_by4(int arr[], int n) {
    int sum = 0;
    int i = 0;
    while (i + 3 < n) {
        sum = sum + arr[i] + arr[i+1] + arr[i+2] + arr[i+3];
        i = i + 4;
    }
    while (i < n) {
        sum = sum + arr[i];
        i = i + 1;
    }
    return sum;
}

int stride_loop(int arr[], int n, int stride) {
    int sum = 0;
    int i = 0;
    while (i < n) {
        sum = sum + arr[i];
        i = i + stride;
    }
    return sum;
}

void duff_copy(int dst[], int src[], int count) {
    int i = 0;
    int rem = count % 4;
    while (rem > 0) {
        dst[i] = src[i];
        i = i + 1;
        rem = rem - 1;
    }
    while (i < count) {
        dst[i]   = src[i];
        dst[i+1] = src[i+1];
        dst[i+2] = src[i+2];
        dst[i+3] = src[i+3];
        i = i + 4;
    }
}

int dot_product_unrolled(int a[], int b[], int n) {
    int dot = 0;
    int i = 0;
    while (i + 1 < n) {
        dot = dot + a[i]*b[i] + a[i+1]*b[i+1];
        i = i + 2;
    }
    while (i < n) {
        dot = dot + a[i] * b[i];
        i = i + 1;
    }
    return dot;
}

int find_max_unrolled(int arr[], int n) {
    int mx = arr[0];
    int i = 1;
    while (i + 1 < n) {
        if (arr[i] > mx) mx = arr[i];
        if (arr[i+1] > mx) mx = arr[i+1];
        i = i + 2;
    }
    while (i < n) {
        if (arr[i] > mx) mx = arr[i];
        i = i + 1;
    }
    return mx;
}

int reverse_unrolled(int arr[], int n) {
    int lo = 0;
    int hi = n - 1;
    while (lo + 1 < hi) {
        int t1 = arr[lo];
        arr[lo] = arr[hi];
        arr[hi] = t1;
        int t2 = arr[lo+1];
        arr[lo+1] = arr[hi-1];
        arr[hi-1] = t2;
        lo = lo + 2;
        hi = hi - 2;
    }
    if (lo < hi) {
        int t = arr[lo];
        arr[lo] = arr[hi];
        arr[hi] = t;
    }
    return arr[0];
}

int main(void) {
    int a[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    int b[7] = {10, 20, 30, 40, 50, 60, 70};

    // EXPECT: sum2(8)=36
    printf("sum2(8)=%d\n", sum_unrolled_by2(a, 8));
    // EXPECT: sum2(7)=28
    printf("sum2(7)=%d\n", sum_unrolled_by2(a, 7));
    // EXPECT: sum4(8)=36
    printf("sum4(8)=%d\n", sum_unrolled_by4(a, 8));
    // EXPECT: sum4(7)=28
    printf("sum4(7)=%d\n", sum_unrolled_by4(a, 7));
    // EXPECT: stride(8,2)=16
    printf("stride(8,2)=%d\n", stride_loop(a, 8, 2));
    // EXPECT: stride(8,3)=12
    printf("stride(8,3)=%d\n", stride_loop(a, 8, 3));

    int dst[8] = {0,0,0,0,0,0,0,0};
    duff_copy(dst, a, 7);
    // EXPECT: duff[0]=1
    printf("duff[0]=%d\n", dst[0]);
    // EXPECT: duff[3]=4
    printf("duff[3]=%d\n", dst[3]);
    // EXPECT: duff[6]=7
    printf("duff[6]=%d\n", dst[6]);

    int c[5] = {2, 3, 4, 5, 6};
    int d[5] = {1, 2, 3, 4, 5};
    // EXPECT: dot(5)=70
    printf("dot(5)=%d\n", dot_product_unrolled(c, d, 5));

    int e[6] = {3, 7, 1, 9, 2, 5};
    // EXPECT: max=9
    printf("max=%d\n", find_max_unrolled(e, 6));

    int f[5] = {3, 7, 1, 9, 2};
    // EXPECT: max_odd=9
    printf("max_odd=%d\n", find_max_unrolled(f, 5));

    int g[6] = {10, 20, 30, 40, 50, 60};
    reverse_unrolled(g, 6);
    // EXPECT: rev[0]=60
    printf("rev[0]=%d\n", g[0]);
    // EXPECT: rev[5]=10
    printf("rev[5]=%d\n", g[5]);

    int h[5] = {5, 4, 3, 2, 1};
    reverse_unrolled(h, 5);
    // EXPECT: rev5[0]=1
    printf("rev5[0]=%d\n", h[0]);
    // EXPECT: rev5[4]=5
    printf("rev5[4]=%d\n", h[4]);

    return 0;
}
