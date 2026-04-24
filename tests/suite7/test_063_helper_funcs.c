int printf(const char *fmt, ...);

int my_abs(int x) {
    if (x < 0) return -x;
    return x;
}

int my_min(int a, int b) {
    if (a < b) return a;
    return b;
}

int my_max(int a, int b) {
    if (a > b) return a;
    return b;
}

int my_clamp(int val, int lo, int hi) {
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

int my_sign(int x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

void my_swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int my_diff(int a, int b) {
    return my_abs(a - b);
}

int my_clamp_abs(int val, int limit) {
    int a = my_abs(val);
    return my_clamp(a, 0, limit);
}

int my_median3(int a, int b, int c) {
    if ((a <= b && b <= c) || (c <= b && b <= a)) return b;
    if ((b <= a && a <= c) || (c <= a && a <= b)) return a;
    return c;
}

int my_in_range(int val, int lo, int hi) {
    return val >= lo && val <= hi;
}

int my_wrap(int val, int lo, int hi) {
    int range = hi - lo + 1;
    while (val < lo) val = val + range;
    while (val > hi) val = val - range;
    return val;
}

int my_lerp_int(int a, int b, int t_percent) {
    return a + (b - a) * t_percent / 100;
}

int min3(int a, int b, int c) {
    return my_min(my_min(a, b), c);
}

int max3(int a, int b, int c) {
    return my_max(my_max(a, b), c);
}

int clamp_and_sign(int val, int lo, int hi) {
    int clamped = my_clamp(val, lo, hi);
    return my_sign(clamped);
}

int sort3(int *a, int *b, int *c) {
    if (*a > *b) my_swap(a, b);
    if (*b > *c) my_swap(b, c);
    if (*a > *b) my_swap(a, b);
    return *a + *b + *c;
}

int sum_abs(int arr[], int n) {
    int total = 0;
    int i = 0;
    while (i < n) {
        total = total + my_abs(arr[i]);
        i = i + 1;
    }
    return total;
}

int clamp_all(int arr[], int n, int lo, int hi) {
    int changed = 0;
    int i = 0;
    while (i < n) {
        int before = arr[i];
        arr[i] = my_clamp(arr[i], lo, hi);
        if (arr[i] != before) changed = changed + 1;
        i = i + 1;
    }
    return changed;
}

int main(void) {
    // EXPECT: abs5: 5
    printf("abs5: %d\n", my_abs(-5));

    // EXPECT: abs0: 0
    printf("abs0: %d\n", my_abs(0));

    // EXPECT: min: 3
    printf("min: %d\n", my_min(3, 7));

    // EXPECT: max: 7
    printf("max: %d\n", my_max(3, 7));

    // EXPECT: clamp_lo: 0
    printf("clamp_lo: %d\n", my_clamp(-5, 0, 10));

    // EXPECT: clamp_hi: 10
    printf("clamp_hi: %d\n", my_clamp(15, 0, 10));

    // EXPECT: clamp_mid: 5
    printf("clamp_mid: %d\n", my_clamp(5, 0, 10));

    // EXPECT: sign_pos: 1
    printf("sign_pos: %d\n", my_sign(42));

    // EXPECT: sign_neg: -1
    printf("sign_neg: %d\n", my_sign(-3));

    // EXPECT: sign_zero: 0
    printf("sign_zero: %d\n", my_sign(0));

    int x = 10;
    int y = 20;
    my_swap(&x, &y);
    // EXPECT: swap: 20 10
    printf("swap: %d %d\n", x, y);

    // EXPECT: diff: 7
    printf("diff: %d\n", my_diff(3, 10));

    // EXPECT: clamp_abs: 5
    printf("clamp_abs: %d\n", my_clamp_abs(-8, 5));

    // EXPECT: median: 5
    printf("median: %d\n", my_median3(3, 5, 9));

    // EXPECT: median2: 5
    printf("median2: %d\n", my_median3(9, 5, 3));

    // EXPECT: in_range1: 1
    printf("in_range1: %d\n", my_in_range(5, 0, 10));

    // EXPECT: in_range0: 0
    printf("in_range0: %d\n", my_in_range(15, 0, 10));

    // EXPECT: wrap: 3
    printf("wrap: %d\n", my_wrap(13, 0, 9));

    // EXPECT: lerp: 15
    printf("lerp: %d\n", my_lerp_int(10, 20, 50));

    // EXPECT: min3: 1
    printf("min3: %d\n", min3(5, 1, 9));

    // EXPECT: max3: 9
    printf("max3: %d\n", max3(5, 1, 9));

    int a = 9;
    int b = 3;
    int c = 6;
    int total = sort3(&a, &b, &c);
    // EXPECT: sort3: 3 6 9 sum=18
    printf("sort3: %d %d %d sum=%d\n", a, b, c, total);

    int arr1[] = {-3, 5, -7, 2, -1};
    // EXPECT: sum_abs: 18
    printf("sum_abs: %d\n", sum_abs(arr1, 5));

    int arr2[] = {-5, 3, 15, 7, -2};
    int changed = clamp_all(arr2, 5, 0, 10);
    // EXPECT: clamped: 3 changed=3
    printf("clamped: %d changed=%d\n", arr2[1], changed);

    return 0;
}
