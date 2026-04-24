int printf(const char *fmt, ...);

// Test 092: Union type punning and reinterpretation

union IntBytes {
    int val;
    char bytes[4];
};

union TwoInts {
    int as_int;
    char as_char;
};

union Mixed {
    int i;
    char c;
    int arr[2];
};

union Nested {
    struct {
        int x;
        int y;
    } point;
    int raw[2];
};

void test_basic_union(void) {
    union TwoInts u;
    u.as_int = 65;
    // 65 is 'A', low byte should be 65
    printf("%d\n", u.as_int);
    // EXPECT: 65
    printf("%d\n", u.as_char);
    // EXPECT: 65

    u.as_char = 0;
    // only the low byte changes; on little-endian this zeros the low byte
    printf("%d\n", u.as_char);
    // EXPECT: 0
}

void test_union_size(void) {
    union Mixed m;
    m.arr[0] = 100;
    m.arr[1] = 200;

    // reading i should give arr[0] since they share storage
    printf("%d\n", m.i);
    // EXPECT: 100

    m.i = 42;
    printf("%d\n", m.arr[0]);
    // EXPECT: 42
    // arr[1] is still 200, untouched by writing to i
    printf("%d\n", m.arr[1]);
    // EXPECT: 200
}

void test_nested_union(void) {
    union Nested n;
    n.point.x = 11;
    n.point.y = 22;

    // raw[0] overlaps with point.x, raw[1] overlaps with point.y
    printf("%d\n", n.raw[0]);
    // EXPECT: 11
    printf("%d\n", n.raw[1]);
    // EXPECT: 22

    n.raw[0] = 33;
    printf("%d\n", n.point.x);
    // EXPECT: 33
    printf("%d\n", n.point.y);
    // EXPECT: 22
}

void test_union_pointer(void) {
    union IntBytes u;
    u.val = 0;
    u.bytes[0] = 1;  // set low byte to 1 (little-endian)
    u.bytes[1] = 0;
    u.bytes[2] = 0;
    u.bytes[3] = 0;
    printf("%d\n", u.val);
    // EXPECT: 1

    u.val = 0;
    u.bytes[1] = 1;  // set second byte: value = 256 on little-endian
    printf("%d\n", u.val);
    // EXPECT: 256
}

void test_union_array(void) {
    union TwoInts arr[3];
    arr[0].as_int = 10;
    arr[1].as_int = 20;
    arr[2].as_int = 30;

    int sum = 0;
    int i;
    for (i = 0; i < 3; i++) {
        sum = sum + arr[i].as_int;
    }
    printf("%d\n", sum);
    // EXPECT: 60
}

void test_union_in_struct(void) {
    struct Container {
        int tag;
        union {
            int ival;
            char cval;
        } data;
    };

    struct Container c;
    c.tag = 1;
    c.data.ival = 999;
    printf("%d\n", c.tag);
    // EXPECT: 1
    printf("%d\n", c.data.ival);
    // EXPECT: 999

    c.tag = 2;
    c.data.cval = 88;
    printf("%d\n", c.tag);
    // EXPECT: 2
    printf("%d\n", c.data.cval);
    // EXPECT: 88
}

void test_union_swap(void) {
    // Use union to swap interpretation
    union Mixed m;
    m.i = 12345;
    int saved = m.i;
    m.arr[0] = 0;
    m.arr[1] = 0;
    m.i = saved;
    printf("%d\n", m.i);
    // EXPECT: 12345
}

int main(void) {
    test_basic_union();
    test_union_size();
    test_nested_union();
    test_union_pointer();
    test_union_array();
    test_union_in_struct();
    test_union_swap();
    return 0;
}
