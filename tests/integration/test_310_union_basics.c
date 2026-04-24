int printf(const char *fmt, ...);
// EXPECT: int value: 42\nchar value: A\nnumber: 12345\nparts: 100, 200\nint: 99\nchar: Z\nextract: 99\nextract char as int: -1\nint: 10\nchar: X\nint: 30
// Test: union basics

union Value {
    int i;
    char c;
};

union Number {
    int integer;
    int parts[2];
};

struct TaggedValue {
    int tag;
    union Value val;
};

void print_tagged(struct TaggedValue *tv) {
    if (tv->tag == 0) {
        printf("int: %d\n", tv->val.i);
    } else if (tv->tag == 1) {
        printf("char: %c\n", tv->val.c);
    } else {
        printf("unknown tag\n");
    }
}

struct TaggedValue make_int_val(int v) {
    struct TaggedValue tv;
    tv.tag = 0;
    tv.val.i = v;
    return tv;
}

struct TaggedValue make_char_val(char c) {
    struct TaggedValue tv;
    tv.tag = 1;
    tv.val.c = c;
    return tv;
}

int extract_int(struct TaggedValue *tv) {
    if (tv->tag == 0) return tv->val.i;
    return -1;
}

int main(void) {
    union Value v;
    v.i = 42;
    printf("int value: %d\n", v.i);

    v.c = 'A';
    printf("char value: %c\n", v.c);

    union Number n;
    n.integer = 12345;
    printf("number: %d\n", n.integer);
    n.parts[0] = 100;
    n.parts[1] = 200;
    printf("parts: %d, %d\n", n.parts[0], n.parts[1]);

    struct TaggedValue tv1 = make_int_val(99);
    struct TaggedValue tv2 = make_char_val('Z');

    print_tagged(&tv1);
    print_tagged(&tv2);

    printf("extract: %d\n", extract_int(&tv1));
    printf("extract char as int: %d\n", extract_int(&tv2));

    struct TaggedValue arr[3];
    arr[0] = make_int_val(10);
    arr[1] = make_char_val('X');
    arr[2] = make_int_val(30);
    int i;
    for (i = 0; i < 3; i++) {
        print_tagged(&arr[i]);
    }

    return 0;
}
