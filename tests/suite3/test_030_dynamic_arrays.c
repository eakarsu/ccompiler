int printf(const char *fmt, ...);

/* test_030_dynamic_arrays.c
   Tests: simulated dynamic arrays using stack-allocated fixed-size buffers,
          variable-amount work with local arrays, array rotation, array reverse,
          stack-based stack/queue simulation, in-place algorithms
*/

/* reverse array in place */
void reverse(int *a, int n) {
    int i;
    for (i = 0; i < n / 2; i++) {
        int tmp = a[i];
        a[i] = a[n - 1 - i];
        a[n - 1 - i] = tmp;
    }
}

/* rotate left by k positions */
void rotate_left(int *a, int n, int k) {
    k = k % n;
    if (k == 0) return;
    reverse(a, k);
    reverse(a + k, n - k);
    reverse(a, n);
}

/* rotate right by k positions */
void rotate_right(int *a, int n, int k) {
    k = k % n;
    rotate_left(a, n, n - k);
}

/* copy src into dst, n elements */
void copy_arr(int *dst, const int *src, int n) {
    int i;
    for (i = 0; i < n; i++) dst[i] = src[i];
}

int sum(const int *a, int n) {
    int s = 0, i;
    for (i = 0; i < n; i++) s += a[i];
    return s;
}

/* bubble sort in place */
void bubble_sort(int *a, int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (a[j] > a[j + 1]) {
                int tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
            }
        }
    }
}

/* merge two sorted halves in-place using a temp buffer */
void merge(int *a, int left, int mid, int right, int *tmp) {
    int i = left, j = mid, k = 0;
    while (i < mid && j < right) {
        if (a[i] <= a[j]) tmp[k++] = a[i++];
        else               tmp[k++] = a[j++];
    }
    while (i < mid)   tmp[k++] = a[i++];
    while (j < right) tmp[k++] = a[j++];
    for (i = 0; i < right - left; i++) a[left + i] = tmp[i];
}

/* stack simulation using array + top index */
void stack_push(int *stack, int *top, int val) {
    stack[*top] = val;
    (*top)++;
}

int stack_pop(int *stack, int *top) {
    (*top)--;
    return stack[*top];
}

/* deduplicate sorted array, return new length */
int dedup(int *a, int n) {
    if (n == 0) return 0;
    int write = 1;
    int i;
    for (i = 1; i < n; i++) {
        if (a[i] != a[write - 1]) {
            a[write++] = a[i];
        }
    }
    return write;
}

int main(void) {
    /* reverse */
    int r[6] = {1, 2, 3, 4, 5, 6};
    reverse(r, 6);
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]);
    // EXPECT: 6 5 4 3 2 1

    int r2[5] = {10, 20, 30, 40, 50};
    reverse(r2, 5);
    printf("%d %d %d %d %d\n", r2[0], r2[1], r2[2], r2[3], r2[4]);
    // EXPECT: 50 40 30 20 10

    int r1[1] = {99};
    reverse(r1, 1);
    printf("%d\n", r1[0]);
    // EXPECT: 99

    /* rotate left */
    int rl[5] = {1, 2, 3, 4, 5};
    rotate_left(rl, 5, 2);
    printf("%d %d %d %d %d\n", rl[0], rl[1], rl[2], rl[3], rl[4]);
    // EXPECT: 3 4 5 1 2

    int rl2[6] = {10, 20, 30, 40, 50, 60};
    rotate_left(rl2, 6, 1);
    printf("%d %d %d %d %d %d\n", rl2[0], rl2[1], rl2[2], rl2[3], rl2[4], rl2[5]);
    // EXPECT: 20 30 40 50 60 10

    /* rotate right */
    int rr[5] = {1, 2, 3, 4, 5};
    rotate_right(rr, 5, 2);
    printf("%d %d %d %d %d\n", rr[0], rr[1], rr[2], rr[3], rr[4]);
    // EXPECT: 4 5 1 2 3

    /* bubble sort */
    int bs[7] = {5, 3, 8, 1, 9, 2, 7};
    bubble_sort(bs, 7);
    printf("%d %d %d %d %d %d %d\n", bs[0], bs[1], bs[2], bs[3], bs[4], bs[5], bs[6]);
    // EXPECT: 1 2 3 5 7 8 9

    /* sort with duplicates */
    int bs2[5] = {3, 1, 2, 1, 3};
    bubble_sort(bs2, 5);
    printf("%d %d %d %d %d\n", bs2[0], bs2[1], bs2[2], bs2[3], bs2[4]);
    // EXPECT: 1 1 2 3 3

    /* merge sorted halves */
    int mg[6] = {1, 3, 5, 2, 4, 6};
    int tmp[6];
    merge(mg, 0, 3, 6, tmp);
    printf("%d %d %d %d %d %d\n", mg[0], mg[1], mg[2], mg[3], mg[4], mg[5]);
    // EXPECT: 1 2 3 4 5 6

    /* stack simulation */
    int stk[16];
    int top = 0;
    stack_push(stk, &top, 10);
    stack_push(stk, &top, 20);
    stack_push(stk, &top, 30);
    printf("%d\n", top);
    // EXPECT: 3
    printf("%d\n", stack_pop(stk, &top));
    // EXPECT: 30
    printf("%d\n", stack_pop(stk, &top));
    // EXPECT: 20
    printf("%d\n", top);
    // EXPECT: 1
    stack_push(stk, &top, 99);
    printf("%d\n", stack_pop(stk, &top));
    // EXPECT: 99
    printf("%d\n", stack_pop(stk, &top));
    // EXPECT: 10
    printf("%d\n", top);
    // EXPECT: 0

    /* dedup */
    int dd[8] = {1, 1, 2, 3, 3, 3, 4, 5};
    int new_len = dedup(dd, 8);
    printf("%d\n", new_len);
    // EXPECT: 5
    printf("%d %d %d %d %d\n", dd[0], dd[1], dd[2], dd[3], dd[4]);
    // EXPECT: 1 2 3 4 5

    int dd2[4] = {7, 7, 7, 7};
    new_len = dedup(dd2, 4);
    printf("%d\n", new_len);
    // EXPECT: 1
    printf("%d\n", dd2[0]);
    // EXPECT: 7

    /* copy and sum */
    int orig[5] = {2, 4, 6, 8, 10};
    int dup[5];
    copy_arr(dup, orig, 5);
    printf("%d\n", sum(dup, 5));
    // EXPECT: 30

    /* rotate left by multiple of n */
    int rmod[4] = {1, 2, 3, 4};
    rotate_left(rmod, 4, 4);
    printf("%d %d %d %d\n", rmod[0], rmod[1], rmod[2], rmod[3]);
    // EXPECT: 1 2 3 4

    /* simulate dynamic growth: fill up to capacity */
    int buf[10];
    int count = 0;
    int i;
    for (i = 0; i < 10; i++) {
        buf[count++] = i * i;
    }
    /* 0,1,4,9,16,25,36,49,64,81 => sum = 285 */
    printf("%d\n", sum(buf, count));
    // EXPECT: 285
    printf("%d\n", count);
    // EXPECT: 10

    return 0;
}
