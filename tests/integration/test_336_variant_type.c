int printf(const char *fmt, ...);
// EXPECT: [0] int(42) -> 42\n[1] char(X) -> 88\n[2] pair(10,20) -> 30\n[3] int(100) -> 100\n[4] char(Z) -> 90\neq(0,3): 0\neq(0,0): 1\neq(1,4): 0\neq pairs: 1\nsum of all: 350
// Test: union-based variant types

enum VarType {
    VAR_INT,
    VAR_CHAR,
    VAR_PAIR
};

struct IntPair {
    int first;
    int second;
};

union VarData {
    int i;
    char c;
    struct IntPair pair;
};

struct Variant {
    enum VarType type;
    union VarData data;
};

struct Variant make_int(int val) {
    struct Variant v;
    v.type = VAR_INT;
    v.data.i = val;
    return v;
}

struct Variant make_char(char ch) {
    struct Variant v;
    v.type = VAR_CHAR;
    v.data.c = ch;
    return v;
}

struct Variant make_pair(int a, int b) {
    struct Variant v;
    v.type = VAR_PAIR;
    v.data.pair.first = a;
    v.data.pair.second = b;
    return v;
}

void print_variant(struct Variant *v) {
    switch (v->type) {
        case VAR_INT:
            printf("int(%d)", v->data.i);
            break;
        case VAR_CHAR:
            printf("char(%c)", v->data.c);
            break;
        case VAR_PAIR:
            printf("pair(%d,%d)", v->data.pair.first, v->data.pair.second);
            break;
    }
}

int variant_to_int(struct Variant *v) {
    switch (v->type) {
        case VAR_INT:  return v->data.i;
        case VAR_CHAR: return (int)v->data.c;
        case VAR_PAIR: return v->data.pair.first + v->data.pair.second;
    }
    return 0;
}

int variant_equal(struct Variant *a, struct Variant *b) {
    if (a->type != b->type) return 0;
    switch (a->type) {
        case VAR_INT:  return a->data.i == b->data.i;
        case VAR_CHAR: return a->data.c == b->data.c;
        case VAR_PAIR: return a->data.pair.first == b->data.pair.first &&
                              a->data.pair.second == b->data.pair.second;
    }
    return 0;
}

int main(void) {
    struct Variant vars[5];
    vars[0] = make_int(42);
    vars[1] = make_char('X');
    vars[2] = make_pair(10, 20);
    vars[3] = make_int(100);
    vars[4] = make_char('Z');

    int i;
    for (i = 0; i < 5; i++) {
        printf("[%d] ", i);
        print_variant(&vars[i]);
        printf(" -> %d\n", variant_to_int(&vars[i]));
    }

    printf("eq(0,3): %d\n", variant_equal(&vars[0], &vars[3]));
    printf("eq(0,0): %d\n", variant_equal(&vars[0], &vars[0]));
    printf("eq(1,4): %d\n", variant_equal(&vars[1], &vars[4]));

    struct Variant p1 = make_pair(5, 5);
    struct Variant p2 = make_pair(5, 5);
    printf("eq pairs: %d\n", variant_equal(&p1, &p2));

    int sum = 0;
    for (i = 0; i < 5; i++) {
        sum = sum + variant_to_int(&vars[i]);
    }
    printf("sum of all: %d\n", sum);

    return 0;
}
