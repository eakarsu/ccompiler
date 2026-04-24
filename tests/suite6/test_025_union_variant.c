int printf(const char *fmt, ...);

/* Tagged union (variant type) */
enum VarType {
    VAR_INT = 0,
    VAR_CHAR = 1,
    VAR_PAIR = 2
};

struct Pair {
    int first;
    int second;
};

union VarData {
    int i;
    char c;
    struct Pair p;
};

struct Variant {
    int type;
    union VarData data;
};

/* Constructors */
struct Variant make_int(int val) {
    struct Variant v;
    v.type = VAR_INT;
    v.data.i = val;
    return v;
}

struct Variant make_char(char val) {
    struct Variant v;
    v.type = VAR_CHAR;
    v.data.c = val;
    return v;
}

struct Variant make_pair(int a, int b) {
    struct Variant v;
    v.type = VAR_PAIR;
    v.data.p.first = a;
    v.data.p.second = b;
    return v;
}

/* Type-safe accessors */
int get_int(struct Variant *v) {
    if (v->type != VAR_INT) return -1;
    return v->data.i;
}

char get_char(struct Variant *v) {
    if (v->type != VAR_CHAR) return '?';
    return v->data.c;
}

int get_pair_first(struct Variant *v) {
    if (v->type != VAR_PAIR) return -1;
    return v->data.p.first;
}

int get_pair_second(struct Variant *v) {
    if (v->type != VAR_PAIR) return -1;
    return v->data.p.second;
}

/* Type name */
char *type_name(int type) {
    if (type == VAR_INT) return "INT";
    if (type == VAR_CHAR) return "CHAR";
    if (type == VAR_PAIR) return "PAIR";
    return "UNKNOWN";
}

/* Print a variant */
void print_variant(struct Variant *v) {
    if (v->type == VAR_INT) {
        printf("Int(%d)\n", v->data.i);
    } else if (v->type == VAR_CHAR) {
        printf("Char(%d)\n", (int)v->data.c);
    } else if (v->type == VAR_PAIR) {
        printf("Pair(%d,%d)\n", v->data.p.first, v->data.p.second);
    }
}

/* Compare two variants: 1 if equal, 0 if not */
int variant_equal(struct Variant *a, struct Variant *b) {
    if (a->type != b->type) return 0;
    if (a->type == VAR_INT) return a->data.i == b->data.i;
    if (a->type == VAR_CHAR) return a->data.c == b->data.c;
    if (a->type == VAR_PAIR) {
        return a->data.p.first == b->data.p.first &&
               a->data.p.second == b->data.p.second;
    }
    return 0;
}

/* Sum all int variants in array */
int sum_ints(struct Variant arr[], int count) {
    int total = 0;
    int i;
    for (i = 0; i < count; i++) {
        if (arr[i].type == VAR_INT) {
            total += arr[i].data.i;
        }
    }
    return total;
}

/* Count variants by type */
int count_type(struct Variant arr[], int count, int type) {
    int n = 0;
    int i;
    for (i = 0; i < count; i++) {
        if (arr[i].type == type) n++;
    }
    return n;
}

int main(void) {
    /* Create variants */
    struct Variant vi = make_int(42);
    struct Variant vc = make_char(65);  /* 'A' = 65 */
    struct Variant vp = make_pair(10, 20);

    /* Test type names */
    // EXPECT: Type: INT
    printf("Type: %s\n", type_name(vi.type));
    // EXPECT: Type: CHAR
    printf("Type: %s\n", type_name(vc.type));
    // EXPECT: Type: PAIR
    printf("Type: %s\n", type_name(vp.type));

    /* Test accessors */
    // EXPECT: Int val: 42
    printf("Int val: %d\n", get_int(&vi));
    // EXPECT: Char val: 65
    printf("Char val: %d\n", (int)get_char(&vc));
    // EXPECT: Pair first: 10
    printf("Pair first: %d\n", get_pair_first(&vp));
    // EXPECT: Pair second: 20
    printf("Pair second: %d\n", get_pair_second(&vp));

    /* Test wrong type access */
    // EXPECT: Wrong access: -1
    printf("Wrong access: %d\n", get_int(&vc));

    /* Test print_variant */
    // EXPECT: Int(42)
    print_variant(&vi);
    // EXPECT: Char(65)
    print_variant(&vc);
    // EXPECT: Pair(10,20)
    print_variant(&vp);

    /* Test variant_equal */
    struct Variant vi2 = make_int(42);
    struct Variant vi3 = make_int(99);
    // EXPECT: Equal same: 1
    printf("Equal same: %d\n", variant_equal(&vi, &vi2));
    // EXPECT: Equal diff val: 0
    printf("Equal diff val: %d\n", variant_equal(&vi, &vi3));
    // EXPECT: Equal diff type: 0
    printf("Equal diff type: %d\n", variant_equal(&vi, &vc));

    /* Variant array */
    struct Variant arr[6];
    arr[0] = make_int(10);
    arr[1] = make_char(66);
    arr[2] = make_int(20);
    arr[3] = make_pair(1, 2);
    arr[4] = make_int(30);
    arr[5] = make_char(67);

    /* Sum ints */
    // EXPECT: Sum ints: 60
    printf("Sum ints: %d\n", sum_ints(arr, 6));

    /* Count by type */
    // EXPECT: Int count: 3
    printf("Int count: %d\n", count_type(arr, 6, VAR_INT));
    // EXPECT: Char count: 2
    printf("Char count: %d\n", count_type(arr, 6, VAR_CHAR));
    // EXPECT: Pair count: 1
    printf("Pair count: %d\n", count_type(arr, 6, VAR_PAIR));

    /* Print all variants */
    int i;
    for (i = 0; i < 6; i++) {
        print_variant(&arr[i]);
    }
    // EXPECT: Int(10)
    // EXPECT: Char(66)
    // EXPECT: Int(20)
    // EXPECT: Pair(1,2)
    // EXPECT: Int(30)
    // EXPECT: Char(67)

    return 0;
}
