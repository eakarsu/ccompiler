int printf(const char *fmt, ...);

// Test 091: Map over arrays using function pointers

typedef int (*MapFn)(int);

int double_it(int x) {
    return x * 2;
}

int square_it(int x) {
    return x * x;
}

int negate_it(int x) {
    return -x;
}

int add_ten(int x) {
    return x + 10;
}

int triple_it(int x) {
    return x * 3;
}

void map_array(int *src, int *dst, int len, MapFn fn) {
    int i;
    for (i = 0; i < len; i++) {
        dst[i] = fn(src[i]);
    }
}

void print_array(const char *label, int *arr, int len) {
    int i;
    printf("%s:", label);
    for (i = 0; i < len; i++) {
        printf(" %d", arr[i]);
    }
    printf("\n");
}

void map_in_place(int *arr, int len, MapFn fn) {
    int i;
    for (i = 0; i < len; i++) {
        arr[i] = fn(arr[i]);
    }
}

void map_chain(int *src, int *dst, int len, MapFn f1, MapFn f2) {
    int i;
    for (i = 0; i < len; i++) {
        dst[i] = f2(f1(src[i]));
    }
}

int apply_twice(int x, MapFn fn) {
    return fn(fn(x));
}

int apply_n_times(int x, MapFn fn, int n) {
    int i;
    int result;
    result = x;
    for (i = 0; i < n; i++) {
        result = fn(result);
    }
    return result;
}

void map_selected(int *src, int *dst, int len, MapFn fn, int threshold) {
    int i;
    for (i = 0; i < len; i++) {
        if (src[i] > threshold) {
            dst[i] = fn(src[i]);
        } else {
            dst[i] = src[i];
        }
    }
}

int main(void) {
    int src[5];
    int dst[5];
    int i;

    src[0] = 1; src[1] = 2; src[2] = 3; src[3] = 4; src[4] = 5;

    map_array(src, dst, 5, double_it);
    print_array("doubled", dst, 5); // EXPECT: doubled: 2 4 6 8 10

    map_array(src, dst, 5, square_it);
    print_array("squared", dst, 5); // EXPECT: squared: 1 4 9 16 25

    map_array(src, dst, 5, negate_it);
    print_array("negated", dst, 5); // EXPECT: negated: -1 -2 -3 -4 -5

    map_array(src, dst, 5, add_ten);
    print_array("plus10", dst, 5); // EXPECT: plus10: 11 12 13 14 15

    map_chain(src, dst, 5, double_it, square_it);
    print_array("dbl_sq", dst, 5); // EXPECT: dbl_sq: 4 16 36 64 100

    map_chain(src, dst, 5, add_ten, triple_it);
    print_array("add_tri", dst, 5); // EXPECT: add_tri: 33 36 39 42 45

    printf("twice_dbl_3: %d\n", apply_twice(3, double_it)); // EXPECT: twice_dbl_3: 12
    printf("twice_sq_2: %d\n", apply_twice(2, square_it)); // EXPECT: twice_sq_2: 16

    printf("dbl_x4: %d\n", apply_n_times(1, double_it, 4)); // EXPECT: dbl_x4: 16
    printf("add10_x3: %d\n", apply_n_times(0, add_ten, 3)); // EXPECT: add10_x3: 30

    for (i = 0; i < 5; i++) dst[i] = src[i];
    map_in_place(dst, 5, triple_it);
    print_array("tripled", dst, 5); // EXPECT: tripled: 3 6 9 12 15

    map_selected(src, dst, 5, square_it, 3);
    print_array("sel_sq", dst, 5); // EXPECT: sel_sq: 1 2 3 16 25

    printf("done\n"); // EXPECT: done
    return 0;
}
