int printf(const char *fmt, ...);

union IntOrChar {
    int i;
    char c;
};

union TwoInts {
    int a;
    int b;
};

union BigSmall {
    int arr[4];
    int single;
};

int test_size(void) {
    union IntOrChar u;
    // sizeof union should be size of largest member
    // int is 4 bytes, char is 1 byte, so union is 4
    return sizeof(u);
}

int test_size_array_union(void) {
    union BigSmall u;
    // arr is 4 * 4 = 16 bytes, single is 4 bytes
    // union size = 16
    return sizeof(u);
}

int test_write_int_read_int(void) {
    union IntOrChar u;
    u.i = 42;
    return u.i;
}

int test_write_char(void) {
    union IntOrChar u;
    u.i = 0;
    u.c = 65; // 'A' = 65
    return u.c;
}

int test_shared_memory(void) {
    union TwoInts u;
    u.a = 100;
    // a and b share memory, so b should also be 100
    return u.b;
}

int test_overwrite(void) {
    union TwoInts u;
    u.a = 100;
    u.b = 200;
    // writing b overwrites a
    return u.a;
}

int test_array_in_union(void) {
    union BigSmall u;
    u.arr[0] = 10;
    u.arr[1] = 20;
    u.arr[2] = 30;
    u.arr[3] = 40;
    // single shares memory with arr[0]
    return u.single;
}

int test_union_pointer(void) {
    union IntOrChar u;
    union IntOrChar *p = &u;
    p->i = 77;
    return p->i;
}

int test_union_in_array(void) {
    union IntOrChar arr[3];
    arr[0].i = 10;
    arr[1].i = 20;
    arr[2].i = 30;
    return arr[0].i + arr[1].i + arr[2].i;
}

int test_union_assign(void) {
    union IntOrChar u1;
    union IntOrChar u2;
    u1.i = 55;
    u2 = u1;
    return u2.i;
}

int test_union_param(union IntOrChar u) {
    return u.i;
}

int test_nested_write(void) {
    union BigSmall u;
    u.single = 999;
    // single overlaps arr[0]
    return u.arr[0];
}

int test_multiple_unions(void) {
    union IntOrChar a;
    union IntOrChar b;
    a.i = 11;
    b.i = 22;
    return a.i + b.i;
}

int main(void) {
    // EXPECT: union size IntOrChar: 4
    printf("union size IntOrChar: %d\n", test_size());

    // EXPECT: union size BigSmall: 16
    printf("union size BigSmall: %d\n", test_size_array_union());

    // EXPECT: write int read int: 42
    printf("write int read int: %d\n", test_write_int_read_int());

    // EXPECT: write char: 65
    printf("write char: %d\n", test_write_char());

    // EXPECT: shared memory: 100
    printf("shared memory: %d\n", test_shared_memory());

    // EXPECT: overwrite: 200
    printf("overwrite: %d\n", test_overwrite());

    // EXPECT: array in union single: 10
    printf("array in union single: %d\n", test_array_in_union());

    // EXPECT: union pointer: 77
    printf("union pointer: %d\n", test_union_pointer());

    // EXPECT: union array sum: 60
    printf("union array sum: %d\n", test_union_in_array());

    // EXPECT: union assign: 55
    printf("union assign: %d\n", test_union_assign());

    union IntOrChar arg;
    arg.i = 88;
    // EXPECT: union param: 88
    printf("union param: %d\n", test_union_param(arg));

    // EXPECT: nested write: 999
    printf("nested write: %d\n", test_nested_write());

    // EXPECT: multiple unions: 33
    printf("multiple unions: %d\n", test_multiple_unions());

    union IntOrChar local;
    local.i = 256;
    // low byte of 256 is 0, so char member = 0
    // EXPECT: type punning 256 to char: 0
    printf("type punning 256 to char: %d\n", local.c);

    local.i = 257;
    // low byte of 257 is 1
    // EXPECT: type punning 257 to char: 1
    printf("type punning 257 to char: %d\n", local.c);

    // EXPECT: sizeof union TwoInts: 4
    printf("sizeof union TwoInts: %d\n", (int)sizeof(union TwoInts));

    return 0;
}
