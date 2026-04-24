int printf(const char *fmt, ...);

struct Point {
    int x;
    int y;
};

union StructOrInt {
    struct Point pt;
    int raw;
};

union CharArray {
    int value;
    char bytes[4];
};

union MultiType {
    int i;
    char c;
    int *p;
};

union TwoStructs {
    struct Point a;
    struct Point b;
};

int test_struct_in_union(void) {
    union StructOrInt u;
    u.pt.x = 10;
    u.pt.y = 20;
    return u.pt.x + u.pt.y;
}

int test_int_overwrites_struct(void) {
    union StructOrInt u;
    u.pt.x = 10;
    u.pt.y = 20;
    u.raw = 42;
    // raw overlaps with pt.x (first member)
    return u.raw;
}

int test_struct_overwrites_int(void) {
    union StructOrInt u;
    u.raw = 99;
    u.pt.x = 5;
    u.pt.y = 15;
    return u.pt.x + u.pt.y;
}

int test_char_array_union(void) {
    union CharArray u;
    u.value = 0;
    u.bytes[0] = 1;
    u.bytes[1] = 0;
    u.bytes[2] = 0;
    u.bytes[3] = 0;
    // little-endian: bytes[0] is low byte
    // value = 1
    return u.value;
}

int test_char_array_write2(void) {
    union CharArray u;
    u.value = 0;
    u.bytes[0] = 2;
    u.bytes[1] = 1;
    u.bytes[2] = 0;
    u.bytes[3] = 0;
    // little-endian: 2 + 1*256 = 258
    return u.value;
}

int test_int_to_bytes(void) {
    union CharArray u;
    u.value = 513; // 513 = 1 + 2*256
    // bytes[0] = 1, bytes[1] = 2
    int b0 = u.bytes[0];
    int b1 = u.bytes[1];
    return b0 * 10 + b1;
}

int test_pointer_in_union(void) {
    union MultiType u;
    int x = 42;
    u.p = &x;
    int result = *(u.p);
    return result;
}

int test_two_structs_union(void) {
    union TwoStructs u;
    u.a.x = 100;
    u.a.y = 200;
    // a and b share the same memory
    return u.b.x + u.b.y;
}

int test_union_sizeof_struct(void) {
    // struct Point is 8 bytes (2 ints)
    // union StructOrInt has Point (8 bytes) and int (4 bytes)
    // sizeof should be 8
    return sizeof(union StructOrInt);
}

int test_sequential_writes(void) {
    union StructOrInt u;
    int sum = 0;
    u.raw = 1;
    sum = sum + u.raw;
    u.pt.x = 10;
    u.pt.y = 20;
    sum = sum + u.pt.x + u.pt.y;
    u.raw = 5;
    sum = sum + u.raw;
    return sum; // 1 + 10 + 20 + 5 = 36
}

int test_union_array_of_structs(void) {
    union StructOrInt arr[3];
    arr[0].pt.x = 1;
    arr[0].pt.y = 2;
    arr[1].pt.x = 3;
    arr[1].pt.y = 4;
    arr[2].pt.x = 5;
    arr[2].pt.y = 6;
    int sum = 0;
    int i;
    for (i = 0; i < 3; i = i + 1) {
        sum = sum + arr[i].pt.x + arr[i].pt.y;
    }
    return sum; // 1+2+3+4+5+6 = 21
}

int test_char_punning(void) {
    union CharArray u;
    u.value = 65; // 'A' is 65
    return u.bytes[0];
}

int main(void) {
    // EXPECT: struct in union: 30
    printf("struct in union: %d\n", test_struct_in_union());

    // EXPECT: int overwrites struct: 42
    printf("int overwrites struct: %d\n", test_int_overwrites_struct());

    // EXPECT: struct overwrites int: 20
    printf("struct overwrites int: %d\n", test_struct_overwrites_int());

    // EXPECT: char array to int: 1
    printf("char array to int: %d\n", test_char_array_union());

    // EXPECT: char array write2: 258
    printf("char array write2: %d\n", test_char_array_write2());

    // EXPECT: int to bytes: 12
    printf("int to bytes: %d\n", test_int_to_bytes());

    // EXPECT: pointer in union: 42
    printf("pointer in union: %d\n", test_pointer_in_union());

    // EXPECT: two structs share: 300
    printf("two structs share: %d\n", test_two_structs_union());

    // EXPECT: sizeof StructOrInt: 8
    printf("sizeof StructOrInt: %d\n", test_union_sizeof_struct());

    // EXPECT: sequential writes: 36
    printf("sequential writes: %d\n", test_sequential_writes());

    // EXPECT: union array of structs: 21
    printf("union array of structs: %d\n", test_union_array_of_structs());

    // EXPECT: char punning 65: 65
    printf("char punning 65: %d\n", test_char_punning());

    union CharArray cu;
    cu.value = 770; // 770 = 2 + 3*256
    // EXPECT: byte0 of 770: 2
    printf("byte0 of 770: %d\n", cu.bytes[0]);

    // EXPECT: byte1 of 770: 3
    printf("byte1 of 770: %d\n", cu.bytes[1]);

    // EXPECT: sizeof CharArray: 4
    printf("sizeof CharArray: %d\n", (int)sizeof(union CharArray));

    // EXPECT: sizeof MultiType: 8
    printf("sizeof MultiType: %d\n", (int)sizeof(union MultiType));

    return 0;
}
