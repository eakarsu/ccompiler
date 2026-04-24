// EXPECT: === Sizeof Basic ===
// EXPECT: sizeof(char)=1
// EXPECT: sizeof(int)=4
// EXPECT: sizeof(int*)=8
// EXPECT: === Struct Sizeof ===
// EXPECT: sizeof TwoInts=8
// EXPECT: sizeof CharInt >= 5
// EXPECT: === Offsetof Simulation ===
// EXPECT: offset of x in Point: 0
// EXPECT: offset of y in Point: 4
// EXPECT: === Nested Struct Sizeof ===
// EXPECT: sizeof Outer >= 12
// EXPECT: === Array of Structs ===
// EXPECT: stride equals sizeof: yes
// EXPECT: === Struct with Array ===
// EXPECT: sizeof WithArray >= 24
// EXPECT: data[0]=10 data[4]=50
// EXPECT: === Struct Packing Observation ===
// EXPECT: Mixed fields accessible: c1=A i=42 c2=B
// EXPECT: === Pointer Arithmetic on Struct Array ===
// EXPECT: via ptr arith: id=2 val=200
// EXPECT: === Sizeof Expressions ===
// EXPECT: sizeof array of 10 ints: 40
// EXPECT: sizeof struct array[3] >= 24
// EXPECT: === Byte-Level Access ===
// EXPECT: first byte of 0x01020304: 4

int printf(const char *fmt, ...);

struct TwoInts {
    int a;
    int b;
};

struct CharInt {
    char c;
    int i;
};

struct Point {
    int x;
    int y;
};

struct Inner {
    int a;
    int b;
};

struct Outer {
    struct Inner inner;
    int c;
};

struct WithArray {
    int data[5];
    int len;
};

struct Mixed {
    char c1;
    int i;
    char c2;
};

struct Record {
    int id;
    int val;
};

void test_sizeof_basic(void) {
    printf("=== Sizeof Basic ===\n");
    printf("sizeof(char)=%d\n", (int)sizeof(char));
    printf("sizeof(int)=%d\n", (int)sizeof(int));
    printf("sizeof(int*)=%d\n", (int)sizeof(int*));
}

void test_struct_sizeof(void) {
    printf("=== Struct Sizeof ===\n");
    printf("sizeof TwoInts=%d\n", (int)sizeof(struct TwoInts));
    int s = (int)sizeof(struct CharInt);
    if (s >= 5) {
        printf("sizeof CharInt >= 5\n");
    } else {
        printf("sizeof CharInt < 5 (got %d)\n", s);
    }
}

void test_offsetof(void) {
    printf("=== Offsetof Simulation ===\n");
    struct Point p;
    char *base = (char *)&p;
    char *xptr = (char *)&p.x;
    char *yptr = (char *)&p.y;
    int off_x = (int)(xptr - base);
    int off_y = (int)(yptr - base);
    printf("offset of x in Point: %d\n", off_x);
    printf("offset of y in Point: %d\n", off_y);
}

void test_nested_sizeof(void) {
    printf("=== Nested Struct Sizeof ===\n");
    int s = (int)sizeof(struct Outer);
    if (s >= 12) {
        printf("sizeof Outer >= 12\n");
    } else {
        printf("sizeof Outer < 12 (got %d)\n", s);
    }
}

void test_array_stride(void) {
    printf("=== Array of Structs ===\n");
    struct Point arr[3];
    char *p0 = (char *)&arr[0];
    char *p1 = (char *)&arr[1];
    int stride = (int)(p1 - p0);
    int sz = (int)sizeof(struct Point);
    if (stride == sz) {
        printf("stride equals sizeof: yes\n");
    } else {
        printf("stride equals sizeof: no (stride=%d sizeof=%d)\n", stride, sz);
    }
}

void test_struct_with_array(void) {
    printf("=== Struct with Array ===\n");
    int s = (int)sizeof(struct WithArray);
    if (s >= 24) {
        printf("sizeof WithArray >= 24\n");
    } else {
        printf("sizeof WithArray < 24 (got %d)\n", s);
    }
    struct WithArray wa;
    wa.data[0] = 10;
    wa.data[1] = 20;
    wa.data[2] = 30;
    wa.data[3] = 40;
    wa.data[4] = 50;
    wa.len = 5;
    printf("data[0]=%d data[4]=%d\n", wa.data[0], wa.data[4]);
}

void test_packing(void) {
    printf("=== Struct Packing Observation ===\n");
    struct Mixed m;
    m.c1 = 'A';
    m.i = 42;
    m.c2 = 'B';
    printf("Mixed fields accessible: c1=%c i=%d c2=%c\n", m.c1, m.i, m.c2);
}

void test_ptr_arith(void) {
    printf("=== Pointer Arithmetic on Struct Array ===\n");
    struct Record recs[3];
    recs[0].id = 0;
    recs[0].val = 0;
    recs[1].id = 1;
    recs[1].val = 100;
    recs[2].id = 2;
    recs[2].val = 200;
    struct Record *p = recs;
    p = p + 2;
    printf("via ptr arith: id=%d val=%d\n", p->id, p->val);
}

void test_sizeof_expr(void) {
    printf("=== Sizeof Expressions ===\n");
    int arr[10];
    printf("sizeof array of 10 ints: %d\n", (int)sizeof(arr));
    struct Point pts[3];
    int s = (int)sizeof(pts);
    if (s >= 24) {
        printf("sizeof struct array[3] >= 24\n");
    } else {
        printf("sizeof struct array[3] < 24 (got %d)\n", s);
    }
}

void test_byte_access(void) {
    printf("=== Byte-Level Access ===\n");
    int val = 0x01020304;
    char *bytes = (char *)&val;
    // ARM64 is little-endian, so bytes[0] is least significant
    printf("first byte of 0x01020304: %d\n", (int)(bytes[0] & 0xFF));
}

int main(void) {
    test_sizeof_basic();
    test_struct_sizeof();
    test_offsetof();
    test_nested_sizeof();
    test_array_stride();
    test_struct_with_array();
    test_packing();
    test_ptr_arith();
    test_sizeof_expr();
    test_byte_access();
    return 0;
}
