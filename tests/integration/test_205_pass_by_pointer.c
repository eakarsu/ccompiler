int printf(const char *fmt, ...);
// EXPECT: before swap: a=5 b=10\nafter swap: a=10 b=5\nincremented: 43\nadded 8: 51\nmultiplied by 3: 153\nclamped 150: 100\nclamped -5: 0\nsorted: 2 5 7\nsum=30 max=9 min=1\nreversed: 6 4 9 1 7 3
// Test 205: Pass-by-pointer (simulating pass-by-reference)

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void increment(int *val) {
    *val = *val + 1;
}

void add_to(int *val, int amount) {
    *val = *val + amount;
}

void multiply_by(int *val, int factor) {
    *val = *val * factor;
}

void clamp_ptr(int *val, int lo, int hi) {
    if (*val < lo) *val = lo;
    if (*val > hi) *val = hi;
}

void sort3(int *a, int *b, int *c) {
    if (*a > *b) swap(a, b);
    if (*b > *c) swap(b, c);
    if (*a > *b) swap(a, b);
}

void compute_stats(int *arr, int n, int *sum_out, int *max_out, int *min_out) {
    *sum_out = 0;
    *max_out = arr[0];
    *min_out = arr[0];
    int i = 0;
    while (i < n) {
        *sum_out = *sum_out + arr[i];
        if (arr[i] > *max_out) *max_out = arr[i];
        if (arr[i] < *min_out) *min_out = arr[i];
        i = i + 1;
    }
}

void reverse_array(int *arr, int n) {
    int i = 0;
    int j = n - 1;
    while (i < j) {
        swap(&arr[i], &arr[j]);
        i = i + 1;
        j = j - 1;
    }
}

int main(void) {
    int a = 5;
    int b = 10;
    printf("before swap: a=%d b=%d\n", a, b);
    swap(&a, &b);
    printf("after swap: a=%d b=%d\n", a, b);

    int x = 42;
    increment(&x);
    printf("incremented: %d\n", x);
    add_to(&x, 8);
    printf("added 8: %d\n", x);
    multiply_by(&x, 3);
    printf("multiplied by 3: %d\n", x);

    int v = 150;
    clamp_ptr(&v, 0, 100);
    printf("clamped 150: %d\n", v);
    v = -5;
    clamp_ptr(&v, 0, 100);
    printf("clamped -5: %d\n", v);

    int p = 7;
    int q = 2;
    int r = 5;
    sort3(&p, &q, &r);
    printf("sorted: %d %d %d\n", p, q, r);

    int arr[6];
    arr[0] = 3; arr[1] = 7; arr[2] = 1; arr[3] = 9; arr[4] = 4; arr[5] = 6;
    int sum_val = 0;
    int max_val = 0;
    int min_val = 0;
    compute_stats(arr, 6, &sum_val, &max_val, &min_val);
    printf("sum=%d max=%d min=%d\n", sum_val, max_val, min_val);

    reverse_array(arr, 6);
    printf("reversed:");
    int i = 0;
    while (i < 6) {
        printf(" %d", arr[i]);
        i = i + 1;
    }
    printf("\n");

    return 0;
}
