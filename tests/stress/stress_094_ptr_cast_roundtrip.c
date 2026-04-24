// stress_094_ptr_cast_roundtrip.c - Pointer casting round-trip torture test
// Exercises: int*->void*->int*, char*->int*->char*, struct*->void*->struct*,
// function pointer casts, pointer-to-long-and-back, aliasing through casts.
// EXPECT: int_void_int=42
// EXPECT: int_void_int=99
// EXPECT: char_read=0x44434241
// EXPECT: char_roundtrip=ABCD
// EXPECT: struct_void_struct x=10 y=20 z=30
// EXPECT: struct_cast_back=60
// EXPECT: funcptr_cast=35
// EXPECT: funcptr_cast2=60
// EXPECT: ptr_long_ptr=777
// EXPECT: ptr_arith_long=888
// EXPECT: array_alias=300
// EXPECT: union_cast=1078523331
// EXPECT: multi_hop=12345
// EXPECT: all_pass=1

int printf(const char *fmt, ...);

struct Point {
    int x;
    int y;
    int z;
};

// int* -> void* -> int*
int test_int_void_int(int val) {
    int x = val;
    int *ip = &x;
    void *vp = (void *)ip;
    int *ip2 = (int *)vp;
    return *ip2;
}

// Write bytes via char*, read via int*
unsigned int test_char_to_int(void) {
    char buf[4];
    buf[0] = 0x41;  // 'A'
    buf[1] = 0x42;  // 'B'
    buf[2] = 0x43;  // 'C'
    buf[3] = 0x44;  // 'D'
    // Read as int (little-endian: 0x44434241)
    unsigned int *ip = (unsigned int *)(void *)buf;
    return *ip;
}

// Write via int*, read back as char*
void test_int_to_char_roundtrip(char *out) {
    int val = 0x44434241;  // "ABCD" in little-endian
    char *cp = (char *)&val;
    out[0] = cp[0];
    out[1] = cp[1];
    out[2] = cp[2];
    out[3] = cp[3];
}

// struct* -> void* -> struct*
int test_struct_void_struct(int x, int y, int z) {
    struct Point p;
    p.x = x;
    p.y = y;
    p.z = z;
    struct Point *sp = &p;
    void *vp = (void *)sp;
    struct Point *sp2 = (struct Point *)vp;
    return sp2->x + sp2->y + sp2->z;
}

// Function pointer cast
int add5(int x) { return x + 5; }
int mul2(int x) { return x * 2; }

int test_funcptr_cast(int x, int which) {
    int (*f)(int);
    if (which == 0) {
        f = add5;
    } else {
        f = mul2;
    }
    // Cast to void* and back
    void *vp = (void *)f;
    int (*f2)(int) = (int (*)(int))vp;
    return f2(x);
}

// Pointer -> long -> pointer
int test_ptr_long_ptr(int val) {
    int x = val;
    int *ip = &x;
    long lv = (long)ip;
    int *ip2 = (int *)lv;
    return *ip2;
}

// Pointer arithmetic through long
int test_ptr_arith_long(void) {
    int arr[5];
    arr[0] = 111;
    arr[1] = 222;
    arr[2] = 333;
    arr[3] = 444;
    arr[4] = 888;
    long base = (long)arr;
    long offset = base + 4 * 4;  // arr[4]
    int *p = (int *)offset;
    return *p;
}

// Array aliasing through different pointer types
int test_array_alias(void) {
    int arr[4];
    arr[0] = 100;
    arr[1] = 200;
    arr[2] = 300;
    arr[3] = 400;
    // Access arr[2] through void* -> char* -> back to int*
    void *vp = (void *)arr;
    char *cp = (char *)vp;
    cp = cp + 8;  // skip 8 bytes = 2 ints
    int *ip = (int *)(void *)cp;
    return *ip;  // should be 300
}

// Union-like reinterpretation via pointer cast
unsigned int test_union_cast(void) {
    // Store a float pattern and read as int
    // Use 3.14f which is 0x4048F5C3 in IEEE 754
    // Actually, let's use a known bit pattern
    float f = 3.14f;
    unsigned int *ip = (unsigned int *)(void *)&f;
    return *ip;
}

// Multi-hop cast chain: int* -> void* -> char* -> void* -> long -> void* -> int*
int test_multi_hop(int val) {
    int x = val;
    int *ip = &x;
    void *v1 = (void *)ip;
    char *cp = (char *)v1;
    void *v2 = (void *)cp;
    long lv = (long)v2;
    void *v3 = (void *)lv;
    int *ip2 = (int *)v3;
    return *ip2;
}

int main(void) {
    int pass = 1;

    // int* -> void* -> int*
    printf("int_void_int=%d\n", test_int_void_int(42));
    if (test_int_void_int(42) != 42) pass = 0;
    printf("int_void_int=%d\n", test_int_void_int(99));
    if (test_int_void_int(99) != 99) pass = 0;

    // char* -> int*
    unsigned int cv = test_char_to_int();
    printf("char_read=0x%X\n", cv);
    if (cv != 0x44434241) pass = 0;

    // int -> char roundtrip
    char buf[5];
    buf[4] = 0;
    test_int_to_char_roundtrip(buf);
    printf("char_roundtrip=%c%c%c%c\n", buf[0], buf[1], buf[2], buf[3]);
    if (buf[0] != 'A' || buf[1] != 'B' || buf[2] != 'C' || buf[3] != 'D') pass = 0;

    // struct* -> void* -> struct*
    struct Point p;
    p.x = 10; p.y = 20; p.z = 30;
    void *vp = (void *)&p;
    struct Point *sp = (struct Point *)vp;
    printf("struct_void_struct x=%d y=%d z=%d\n", sp->x, sp->y, sp->z);
    if (sp->x != 10 || sp->y != 20 || sp->z != 30) pass = 0;
    printf("struct_cast_back=%d\n", test_struct_void_struct(10, 20, 30));
    if (test_struct_void_struct(10, 20, 30) != 60) pass = 0;

    // Function pointer cast
    printf("funcptr_cast=%d\n", test_funcptr_cast(30, 0));
    if (test_funcptr_cast(30, 0) != 35) pass = 0;
    printf("funcptr_cast2=%d\n", test_funcptr_cast(30, 1));
    // mul2(30) = 60, but we named it funcptr_cast2
    // Actually let me test with different input for variety
    // test_funcptr_cast(30, 1) = mul2(30) = 60
    // Hmm, the EXPECT says 15, let me adjust
    // Let me use test_funcptr_cast(15, 0) = add5(15) = 20 -- nah
    // Let me just use what the function returns
    if (test_funcptr_cast(30, 1) != 60) pass = 0;

    // ptr -> long -> ptr
    printf("ptr_long_ptr=%d\n", test_ptr_long_ptr(777));
    if (test_ptr_long_ptr(777) != 777) pass = 0;

    // ptr arithmetic through long
    printf("ptr_arith_long=%d\n", test_ptr_arith_long());
    if (test_ptr_arith_long() != 888) pass = 0;

    // array aliasing
    printf("array_alias=%d\n", test_array_alias());
    if (test_array_alias() != 300) pass = 0;

    // union-like cast
    printf("union_cast=%u\n", test_union_cast());
    if (test_union_cast() != 1078523331) pass = 0;

    // multi-hop
    printf("multi_hop=%d\n", test_multi_hop(12345));
    if (test_multi_hop(12345) != 12345) pass = 0;

    printf("all_pass=%d\n", pass);
    return 0;
}
