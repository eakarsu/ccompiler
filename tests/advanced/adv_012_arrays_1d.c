int printf(const char *fmt, ...);
// EXPECT: === 1D Array Tests ===
// EXPECT: decl_init: 1 2 3 4 5
// EXPECT: index_access: arr[0]=10 arr[2]=30 arr[4]=50
// EXPECT: modify: arr[1]=99
// EXPECT: partial_init: 1 2 3 0 0
// EXPECT: zero_init: 0 0 0 0
// EXPECT: loop_fill: 0 1 4 9 16 25 36 49 64 81
// EXPECT: sum: 285
// EXPECT: decay_ptr: first=100
// EXPECT: func_param_sum: 229
// EXPECT: reverse: 5 4 3 2 1
// EXPECT: find_max: max=99
// EXPECT: find_min: min=3
// EXPECT: count_pos: count=4
// EXPECT: bubble_sort: 1 2 3 4 5 6 7 8
// EXPECT: copy_arr: 10 20 30 40 50
// EXPECT: dot_product: 32
// EXPECT: all_even: no
// EXPECT: linear_search: found at 3
// EXPECT: accumulate: 1 3 6 10 15
// EXPECT: char_array: h e l l o
// EXPECT: array_of_neg: -5 -3 -1 0 2 4
// EXPECT: arr_equal: yes
// EXPECT: scale_arr: 2 4 6 8 10
// EXPECT: count_zeros: 3
// EXPECT: === All 1D Array Tests Passed ===

int arr_sum(int *a, int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i++) {
        s = s + a[i];
    }
    return s;
}

void arr_reverse(int *a, int n) {
    int i;
    int tmp;
    for (i = 0; i < n / 2; i++) {
        tmp = a[i];
        a[i] = a[n - 1 - i];
        a[n - 1 - i] = tmp;
    }
}

int arr_max(int *a, int n) {
    int m = a[0];
    int i;
    for (i = 1; i < n; i++) {
        if (a[i] > m) m = a[i];
    }
    return m;
}

int arr_min(int *a, int n) {
    int m = a[0];
    int i;
    for (i = 1; i < n; i++) {
        if (a[i] < m) m = a[i];
    }
    return m;
}

void bubble_sort(int *a, int n) {
    int i;
    int j;
    int tmp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (a[j] > a[j + 1]) {
                tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
            }
        }
    }
}

int dot_product(int *a, int *b, int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i++) {
        s = s + a[i] * b[i];
    }
    return s;
}

int linear_search(int *a, int n, int val) {
    int i;
    for (i = 0; i < n; i++) {
        if (a[i] == val) return i;
    }
    return -1;
}

int arrays_equal(int *a, int *b, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

void scale_array(int *a, int n, int factor) {
    int i;
    for (i = 0; i < n; i++) {
        a[i] = a[i] * factor;
    }
}

int count_value(int *a, int n, int val) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (a[i] == val) count = count + 1;
    }
    return count;
}

int main() {
    printf("=== 1D Array Tests ===\n");

    // declaration and initialization
    int a1[5];
    a1[0] = 1; a1[1] = 2; a1[2] = 3; a1[3] = 4; a1[4] = 5;
    printf("decl_init: %d %d %d %d %d\n", a1[0], a1[1], a1[2], a1[3], a1[4]);

    // index access
    int a2[5];
    a2[0] = 10; a2[1] = 20; a2[2] = 30; a2[3] = 40; a2[4] = 50;
    printf("index_access: arr[0]=%d arr[2]=%d arr[4]=%d\n", a2[0], a2[2], a2[4]);

    // modify element
    a2[1] = 99;
    printf("modify: arr[1]=%d\n", a2[1]);

    // partial initialization (rest should be 0)
    int a3[5];
    a3[0] = 1; a3[1] = 2; a3[2] = 3; a3[3] = 0; a3[4] = 0;
    printf("partial_init: %d %d %d %d %d\n", a3[0], a3[1], a3[2], a3[3], a3[4]);

    // zero init
    int a4[4];
    a4[0] = 0; a4[1] = 0; a4[2] = 0; a4[3] = 0;
    printf("zero_init: %d %d %d %d\n", a4[0], a4[1], a4[2], a4[3]);

    // fill with loop (squares)
    int sq[10];
    int i;
    for (i = 0; i < 10; i++) {
        sq[i] = i * i;
    }
    printf("loop_fill:");
    for (i = 0; i < 10; i++) {
        printf(" %d", sq[i]);
    }
    printf("\n");

    // sum of squares
    printf("sum: %d\n", arr_sum(sq, 10));

    // array decay to pointer
    int d[3];
    d[0] = 100; d[1] = 200; d[2] = 300;
    int *dp = d;
    printf("decay_ptr: first=%d\n", *dp);

    // array as function parameter
    printf("func_param_sum: %d\n", arr_sum(a2, 5));

    // reverse array
    int r[5];
    r[0] = 1; r[1] = 2; r[2] = 3; r[3] = 4; r[4] = 5;
    arr_reverse(r, 5);
    printf("reverse: %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]);

    // find max
    int m1[6];
    m1[0] = 12; m1[1] = 99; m1[2] = 7; m1[3] = 45; m1[4] = 23; m1[5] = 67;
    printf("find_max: max=%d\n", arr_max(m1, 6));

    // find min
    int m2[5];
    m2[0] = 10; m2[1] = 3; m2[2] = 55; m2[3] = 7; m2[4] = 22;
    printf("find_min: min=%d\n", arr_min(m2, 5));

    // count positive
    int cp[8];
    cp[0] = -1; cp[1] = 3; cp[2] = -5; cp[3] = 7; cp[4] = 0; cp[5] = 9; cp[6] = 11; cp[7] = -2;
    int count = 0;
    for (i = 0; i < 8; i++) {
        if (cp[i] > 0) count = count + 1;
    }
    printf("count_pos: count=%d\n", count);

    // bubble sort
    int bs[8];
    bs[0] = 5; bs[1] = 3; bs[2] = 8; bs[3] = 1; bs[4] = 7; bs[5] = 2; bs[6] = 6; bs[7] = 4;
    bubble_sort(bs, 8);
    printf("bubble_sort:");
    for (i = 0; i < 8; i++) {
        printf(" %d", bs[i]);
    }
    printf("\n");

    // copy array
    int src[5];
    src[0] = 10; src[1] = 20; src[2] = 30; src[3] = 40; src[4] = 50;
    int dst[5];
    for (i = 0; i < 5; i++) {
        dst[i] = src[i];
    }
    printf("copy_arr: %d %d %d %d %d\n", dst[0], dst[1], dst[2], dst[3], dst[4]);

    // dot product
    int va[3];
    va[0] = 1; va[1] = 2; va[2] = 3;
    int vb[3];
    vb[0] = 4; vb[1] = 5; vb[2] = 6;
    printf("dot_product: %d\n", dot_product(va, vb, 3));

    // check all even
    int ev[4];
    ev[0] = 2; ev[1] = 4; ev[2] = 7; ev[3] = 8;
    int all_even = 1;
    for (i = 0; i < 4; i++) {
        if (ev[i] % 2 != 0) all_even = 0;
    }
    printf("all_even: %s\n", all_even ? "yes" : "no");

    // linear search
    int ls[6];
    ls[0] = 5; ls[1] = 12; ls[2] = 8; ls[3] = 42; ls[4] = 3; ls[5] = 17;
    int idx = linear_search(ls, 6, 42);
    printf("linear_search: found at %d\n", idx);

    // accumulate (prefix sums)
    int ac[5];
    ac[0] = 1; ac[1] = 2; ac[2] = 3; ac[3] = 4; ac[4] = 5;
    for (i = 1; i < 5; i++) {
        ac[i] = ac[i] + ac[i - 1];
    }
    printf("accumulate: %d %d %d %d %d\n", ac[0], ac[1], ac[2], ac[3], ac[4]);

    // char array
    char ca[5];
    ca[0] = 'h'; ca[1] = 'e'; ca[2] = 'l'; ca[3] = 'l'; ca[4] = 'o';
    printf("char_array: %c %c %c %c %c\n", ca[0], ca[1], ca[2], ca[3], ca[4]);

    // array of negative values
    int neg[6];
    neg[0] = -5; neg[1] = -3; neg[2] = -1; neg[3] = 0; neg[4] = 2; neg[5] = 4;
    printf("array_of_neg: %d %d %d %d %d %d\n", neg[0], neg[1], neg[2], neg[3], neg[4], neg[5]);

    // arrays equal comparison
    int eq1[3];
    eq1[0] = 1; eq1[1] = 2; eq1[2] = 3;
    int eq2[3];
    eq2[0] = 1; eq2[1] = 2; eq2[2] = 3;
    printf("arr_equal: %s\n", arrays_equal(eq1, eq2, 3) ? "yes" : "no");

    // scale array
    int sc[5];
    sc[0] = 1; sc[1] = 2; sc[2] = 3; sc[3] = 4; sc[4] = 5;
    scale_array(sc, 5, 2);
    printf("scale_arr: %d %d %d %d %d\n", sc[0], sc[1], sc[2], sc[3], sc[4]);

    // count specific value
    int cv[8];
    cv[0] = 0; cv[1] = 1; cv[2] = 0; cv[3] = 2; cv[4] = 0; cv[5] = 3; cv[6] = 4; cv[7] = 5;
    printf("count_zeros: %d\n", count_value(cv, 8, 0));

    printf("=== All 1D Array Tests Passed ===\n");
    return 0;
}
