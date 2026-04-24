// EXPECT: === Union Basic ===
// EXPECT: int val: 42
// EXPECT: char val: A
// EXPECT: === Union Size ===
// EXPECT: size of union >= 4: yes
// EXPECT: === Type Punning ===
// EXPECT: wrote int 0, read char 0: yes
// EXPECT: === Tagged Union ===
// EXPECT: type INT: 42
// EXPECT: type CHAR: X
// EXPECT: type FLOAT_AS_INT: 100
// EXPECT: === Union Assignment ===
// EXPECT: copied val: 999
// EXPECT: === Union in Struct ===
// EXPECT: cmd type 1, value 10
// EXPECT: cmd type 2, name: Hi
// EXPECT: === Union Array ===
// EXPECT: u[0].i = 10
// EXPECT: u[1].i = 20
// EXPECT: u[2].i = 30
// EXPECT: === Union Pointer ===
// EXPECT: via pointer: 55

int printf(const char *fmt, ...);

union Data {
    int i;
    char c;
};

enum TypeTag {
    TYPE_INT,
    TYPE_CHAR,
    TYPE_FLOAT_AS_INT
};

struct Tagged {
    enum TypeTag tag;
    union {
        int i;
        char c;
    } value;
};

struct Command {
    int type;
    union {
        int intval;
        char strval[8];
    } payload;
};

void test_basic(void) {
    printf("=== Union Basic ===\n");
    union Data d;
    d.i = 42;
    printf("int val: %d\n", d.i);
    d.c = 'A';
    printf("char val: %c\n", d.c);
}

void test_size(void) {
    printf("=== Union Size ===\n");
    int s = sizeof(union Data);
    if (s >= 4) {
        printf("size of union >= 4: yes\n");
    } else {
        printf("size of union >= 4: no (got %d)\n", s);
    }
}

void test_type_punning(void) {
    printf("=== Type Punning ===\n");
    union Data d;
    d.i = 0;
    if (d.c == 0) {
        printf("wrote int 0, read char 0: yes\n");
    } else {
        printf("wrote int 0, read char 0: no\n");
    }
}

void print_tagged(struct Tagged *t) {
    if (t->tag == TYPE_INT) {
        printf("type INT: %d\n", t->value.i);
    } else if (t->tag == TYPE_CHAR) {
        printf("type CHAR: %c\n", t->value.c);
    } else if (t->tag == TYPE_FLOAT_AS_INT) {
        printf("type FLOAT_AS_INT: %d\n", t->value.i);
    }
}

void test_tagged_union(void) {
    printf("=== Tagged Union ===\n");
    struct Tagged t1;
    t1.tag = TYPE_INT;
    t1.value.i = 42;
    print_tagged(&t1);

    struct Tagged t2;
    t2.tag = TYPE_CHAR;
    t2.value.c = 'X';
    print_tagged(&t2);

    struct Tagged t3;
    t3.tag = TYPE_FLOAT_AS_INT;
    t3.value.i = 100;
    print_tagged(&t3);
}

void test_assignment(void) {
    printf("=== Union Assignment ===\n");
    union Data d1;
    d1.i = 999;
    union Data d2;
    d2 = d1;
    printf("copied val: %d\n", d2.i);
}

void test_union_in_struct(void) {
    printf("=== Union in Struct ===\n");
    struct Command cmd1;
    cmd1.type = 1;
    cmd1.payload.intval = 10;
    printf("cmd type %d, value %d\n", cmd1.type, cmd1.payload.intval);

    struct Command cmd2;
    cmd2.type = 2;
    cmd2.payload.strval[0] = 'H';
    cmd2.payload.strval[1] = 'i';
    cmd2.payload.strval[2] = 0;
    printf("cmd type %d, name: %s\n", cmd2.type, cmd2.payload.strval);
}

void test_union_array(void) {
    printf("=== Union Array ===\n");
    union Data arr[3];
    arr[0].i = 10;
    arr[1].i = 20;
    arr[2].i = 30;
    int i;
    for (i = 0; i < 3; i++) {
        printf("u[%d].i = %d\n", i, arr[i].i);
    }
}

void test_union_pointer(void) {
    printf("=== Union Pointer ===\n");
    union Data d;
    d.i = 55;
    union Data *p = &d;
    printf("via pointer: %d\n", p->i);
}

int main(void) {
    test_basic();
    test_size();
    test_type_punning();
    test_tagged_union();
    test_assignment();
    test_union_in_struct();
    test_union_array();
    test_union_pointer();
    return 0;
}
