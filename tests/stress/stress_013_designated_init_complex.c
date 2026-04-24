// Designated initializer edge cases: out-of-order, sparse, nested, overwrite
int printf(const char *fmt, ...);

struct inner {
    int a;
    int b;
};

struct outer {
    int x;
    struct inner in;
    int y;
};

struct deep {
    struct outer o;
    int z;
};

struct with_arr {
    int id;
    int data[5];
    int tag;
};

void test_out_of_order_struct(void) {
    struct outer s = {.y = 99, .x = 11, .in = {.b = 55, .a = 33}};
    printf("%d %d %d %d\n", s.x, s.in.a, s.in.b, s.y);
    // EXPECT: 11 33 55 99
}

void test_sparse_array(void) {
    int arr[10] = {[0] = 10, [5] = 50, [9] = 90};
    printf("%d %d %d %d %d\n", arr[0], arr[1], arr[5], arr[8], arr[9]);
    // EXPECT: 10 0 50 0 90
}

void test_nested_designated(void) {
    struct deep d = {.o = {.x = 1, .in = {.a = 2, .b = 3}, .y = 4}, .z = 5};
    printf("%d %d %d %d %d\n", d.o.x, d.o.in.a, d.o.in.b, d.o.y, d.z);
    // EXPECT: 1 2 3 4 5
}

void test_nested_deep_designator(void) {
    struct deep d = {.o.x = 100, .o.in.a = 200, .o.in.b = 300, .o.y = 400, .z = 500};
    printf("%d %d %d %d %d\n", d.o.x, d.o.in.a, d.o.in.b, d.o.y, d.z);
    // EXPECT: 100 200 300 400 500
}

void test_mix_designated_positional(void) {
    int arr[6] = {1, 2, [4] = 40, 50};
    printf("%d %d %d %d %d %d\n", arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);
    // EXPECT: 1 2 0 0 40 50
}

void test_overwrite_designation(void) {
    int arr[5] = {[2] = 20, [2] = 99, [0] = 1, [0] = 7};
    printf("%d %d %d %d %d\n", arr[0], arr[1], arr[2], arr[3], arr[4]);
    // EXPECT: 7 0 99 0 0
}

void test_struct_array_designated(void) {
    struct inner arr[4] = {
        [0] = {.a = 10, .b = 11},
        [2] = {.a = 30, .b = 31},
        [3] = {.a = 40, .b = 41}
    };
    int i;
    for (i = 0; i < 4; i++) {
        printf("[%d] a=%d b=%d\n", i, arr[i].a, arr[i].b);
    }
    // EXPECT: [0] a=10 b=11
    // EXPECT: [1] a=0 b=0
    // EXPECT: [2] a=30 b=31
    // EXPECT: [3] a=40 b=41
}

void test_with_arr_struct(void) {
    struct with_arr w = {.tag = 99, .id = 1, .data = {[0] = 10, [2] = 30, [4] = 50}};
    printf("id=%d tag=%d\n", w.id, w.tag);
    printf("data: %d %d %d %d %d\n", w.data[0], w.data[1], w.data[2], w.data[3], w.data[4]);
    // EXPECT: id=1 tag=99
    // EXPECT: data: 10 0 30 0 50
}

void test_all_zeros_except_last(void) {
    int arr[8] = {[7] = 777};
    printf("%d %d %d %d %d %d %d %d\n",
           arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6], arr[7]);
    // EXPECT: 0 0 0 0 0 0 0 777
}

void test_designated_then_continue(void) {
    int arr[6] = {[3] = 30, 40, 50};
    printf("%d %d %d %d %d %d\n", arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);
    // EXPECT: 0 0 0 30 40 50
}

void test_struct_partial_designated(void) {
    struct outer s = {.x = 42};
    printf("%d %d %d %d\n", s.x, s.in.a, s.in.b, s.y);
    // EXPECT: 42 0 0 0
}

void test_array_of_struct_sparse(void) {
    struct inner arr[5] = {[1].a = 11, [1].b = 12, [3].a = 31, [3].b = 32};
    int i;
    for (i = 0; i < 5; i++) {
        printf("%d: %d %d\n", i, arr[i].a, arr[i].b);
    }
    // EXPECT: 0: 0 0
    // EXPECT: 1: 11 12
    // EXPECT: 2: 0 0
    // EXPECT: 3: 31 32
    // EXPECT: 4: 0 0
}

void test_consecutive_designated(void) {
    int arr[5] = {[0] = 1, [1] = 2, [2] = 3, [3] = 4, [4] = 5};
    int sum = 0;
    int i;
    for (i = 0; i < 5; i++) sum = sum + arr[i];
    printf("sum=%d\n", sum);
    // EXPECT: sum=15
}

int main(void) {
    test_out_of_order_struct();
    test_sparse_array();
    test_nested_designated();
    test_nested_deep_designator();
    test_mix_designated_positional();
    test_overwrite_designation();
    test_struct_array_designated();
    test_with_arr_struct();
    test_all_zeros_except_last();
    test_designated_then_continue();
    test_struct_partial_designated();
    test_array_of_struct_sparse();
    test_consecutive_designated();
    return 0;
}
