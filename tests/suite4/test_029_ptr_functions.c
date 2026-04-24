int printf(const char *fmt, ...);

int g_value = 0;

void set_int(int *p, int val) {
    *p = val;
}

void add_to(int *p, int amount) {
    *p = *p + amount;
}

void multiply_by(int *p, int factor) {
    *p = *p * factor;
}

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int *max_ptr(int *a, int *b) {
    if (*a > *b) {
        return a;
    }
    return b;
}

int *min_ptr(int *a, int *b) {
    if (*a < *b) {
        return a;
    }
    return b;
}

void clamp(int *val, int lo, int hi) {
    if (*val < lo) {
        *val = lo;
    } else if (*val > hi) {
        *val = hi;
    }
}

void abs_val(int *p) {
    if (*p < 0) {
        *p = -(*p);
    }
}

void sort3(int *a, int *b, int *c) {
    if (*a > *b) { swap(a, b); }
    if (*b > *c) { swap(b, c); }
    if (*a > *b) { swap(a, b); }
}

void sum_and_product(int *arr, int n, int *out_sum, int *out_prod) {
    *out_sum = 0;
    *out_prod = 1;
    int i;
    for (i = 0; i < n; i = i + 1) {
        *out_sum = *out_sum + arr[i];
        *out_prod = *out_prod * arr[i];
    }
}

void find_min_max(int *arr, int n, int *out_min, int *out_max) {
    *out_min = arr[0];
    *out_max = arr[0];
    int i;
    for (i = 1; i < n; i = i + 1) {
        if (arr[i] < *out_min) {
            *out_min = arr[i];
        }
        if (arr[i] > *out_max) {
            *out_max = arr[i];
        }
    }
}

int *get_global_ptr(void) {
    return &g_value;
}

void apply_to_array(int *arr, int n, void (*fn)(int *)) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        fn(&arr[i]);
    }
}

void double_val(int *p) {
    *p = *p * 2;
}

void negate_val(int *p) {
    *p = -(*p);
}

int main(void) {
    int x = 0;
    set_int(&x, 42);
    // EXPECT: 42
    printf("%d\n", x);

    add_to(&x, 8);
    // EXPECT: 50
    printf("%d\n", x);

    multiply_by(&x, 3);
    // EXPECT: 150
    printf("%d\n", x);

    int a = 10;
    int b = 20;
    swap(&a, &b);
    // EXPECT: 20
    printf("%d\n", a);
    // EXPECT: 10
    printf("%d\n", b);

    int *mp = max_ptr(&a, &b);
    // EXPECT: 20
    printf("%d\n", *mp);

    int *mnp = min_ptr(&a, &b);
    // EXPECT: 10
    printf("%d\n", *mnp);

    /* Modify through returned pointer */
    *mp = 100;
    // EXPECT: 100
    printf("%d\n", a);

    int v = 150;
    clamp(&v, 0, 100);
    // EXPECT: 100
    printf("%d\n", v);

    v = -5;
    clamp(&v, 0, 100);
    // EXPECT: 0
    printf("%d\n", v);

    v = 50;
    clamp(&v, 0, 100);
    // EXPECT: 50
    printf("%d\n", v);

    int neg = -42;
    abs_val(&neg);
    // EXPECT: 42
    printf("%d\n", neg);

    int pos = 77;
    abs_val(&pos);
    // EXPECT: 77
    printf("%d\n", pos);

    int s1 = 3;
    int s2 = 1;
    int s3 = 2;
    sort3(&s1, &s2, &s3);
    // EXPECT: 1
    printf("%d\n", s1);
    // EXPECT: 2
    printf("%d\n", s2);
    // EXPECT: 3
    printf("%d\n", s3);

    int arr[4];
    arr[0] = 2;
    arr[1] = 5;
    arr[2] = 3;
    arr[3] = 10;
    int sum;
    int prod;
    sum_and_product(arr, 4, &sum, &prod);
    // EXPECT: 20
    printf("%d\n", sum);
    // EXPECT: 300
    printf("%d\n", prod);

    int mn;
    int mx;
    find_min_max(arr, 4, &mn, &mx);
    // EXPECT: 2
    printf("%d\n", mn);
    // EXPECT: 10
    printf("%d\n", mx);

    int *gp = get_global_ptr();
    *gp = 999;
    // EXPECT: 999
    printf("%d\n", g_value);

    /* function pointer as parameter */
    int darr[3];
    darr[0] = 1;
    darr[1] = 2;
    darr[2] = 3;
    apply_to_array(darr, 3, double_val);
    // EXPECT: 2
    printf("%d\n", darr[0]);
    // EXPECT: 4
    printf("%d\n", darr[1]);
    // EXPECT: 6
    printf("%d\n", darr[2]);

    apply_to_array(darr, 3, negate_val);
    // EXPECT: -2
    printf("%d\n", darr[0]);
    // EXPECT: -4
    printf("%d\n", darr[1]);
    // EXPECT: -6
    printf("%d\n", darr[2]);

    return 0;
}
