int printf(const char *fmt, ...);

// Test 096: Sizeof on complex expressions and types

struct Small {
    int x;
};

struct Pair {
    int a;
    int b;
};

struct Triple {
    int a;
    int b;
    int c;
};

struct WithChar {
    char c;
    int i;
};

struct Nested {
    struct Pair p;
    int z;
};

union IntChar {
    int i;
    char c;
};

void test_basic_sizeof(void) {
    printf("%d\n", (int)sizeof(int));
    // EXPECT: 4
    printf("%d\n", (int)sizeof(char));
    // EXPECT: 1

    int x = 42;
    printf("%d\n", (int)sizeof(x));
    // EXPECT: 4

    char c = 'A';
    printf("%d\n", (int)sizeof(c));
    // EXPECT: 1
}

void test_pointer_sizeof(void) {
    // All pointers are 8 bytes on 64-bit
    printf("%d\n", (int)sizeof(int *));
    // EXPECT: 8
    printf("%d\n", (int)sizeof(char *));
    // EXPECT: 8

    int *p = (int *)0;
    printf("%d\n", (int)sizeof(p));
    // EXPECT: 8

    int **pp = (int **)0;
    printf("%d\n", (int)sizeof(pp));
    // EXPECT: 8
}

void test_array_sizeof(void) {
    int arr[10];
    printf("%d\n", (int)sizeof(arr));
    // EXPECT: 40

    char str[20];
    printf("%d\n", (int)sizeof(str));
    // EXPECT: 20

    // Number of elements
    int count = (int)(sizeof(arr) / sizeof(arr[0]));
    printf("%d\n", count);
    // EXPECT: 10
}

void test_struct_sizeof(void) {
    printf("%d\n", (int)sizeof(struct Small));
    // EXPECT: 4
    printf("%d\n", (int)sizeof(struct Pair));
    // EXPECT: 8
    printf("%d\n", (int)sizeof(struct Triple));
    // EXPECT: 12

    // WithChar has padding: char(1) + pad(3) + int(4) = 8
    printf("%d\n", (int)sizeof(struct WithChar));
    // EXPECT: 8

    // Nested: Pair(8) + int(4) = 12
    printf("%d\n", (int)sizeof(struct Nested));
    // EXPECT: 12
}

void test_union_sizeof(void) {
    // Union size is max of its members
    printf("%d\n", (int)sizeof(union IntChar));
    // EXPECT: 4
}

void test_sizeof_expression(void) {
    // sizeof does NOT evaluate its operand
    int x = 10;
    int s = (int)sizeof(x + 1);  // type is int, sizeof(int)=4
    printf("%d\n", s);
    // EXPECT: 4
    printf("%d\n", x);  // x should still be 10
    // EXPECT: 10

    // sizeof on a dereference expression
    int arr[5] = {1, 2, 3, 4, 5};
    printf("%d\n", (int)sizeof(arr[0]));
    // EXPECT: 4

    // sizeof on struct member expression
    struct Pair p;
    printf("%d\n", (int)sizeof(p.a));
    // EXPECT: 4
}

void test_sizeof_in_computation(void) {
    // Use sizeof to compute buffer sizes
    int buf[100];
    int total_bytes = (int)sizeof(buf);
    int elem_bytes = (int)sizeof(buf[0]);
    int num_elems = total_bytes / elem_bytes;
    printf("%d\n", total_bytes);
    // EXPECT: 400
    printf("%d\n", num_elems);
    // EXPECT: 100

    // Struct array
    struct Pair pairs[5];
    int pair_total = (int)sizeof(pairs);
    int pair_count = (int)(sizeof(pairs) / sizeof(pairs[0]));
    printf("%d\n", pair_total);
    // EXPECT: 40
    printf("%d\n", pair_count);
    // EXPECT: 5
}

void test_sizeof_nested(void) {
    struct Nested n;
    n.p.a = 1;
    n.p.b = 2;
    n.z = 3;

    printf("%d\n", (int)sizeof(n));
    // EXPECT: 12
    printf("%d\n", (int)sizeof(n.p));
    // EXPECT: 8
    printf("%d\n", (int)sizeof(n.p.a));
    // EXPECT: 4
}

void test_sizeof_char_array(void) {
    char name[16];
    printf("%d\n", (int)sizeof(name));
    // EXPECT: 16

    // pointer vs array
    char *ptr = name;
    printf("%d\n", (int)sizeof(ptr));
    // EXPECT: 8
}

int main(void) {
    test_basic_sizeof();
    test_pointer_sizeof();
    test_array_sizeof();
    test_struct_sizeof();
    test_union_sizeof();
    test_sizeof_expression();
    test_sizeof_in_computation();
    test_sizeof_nested();
    test_sizeof_char_array();
    return 0;
}
