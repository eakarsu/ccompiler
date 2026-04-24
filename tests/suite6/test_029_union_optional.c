int printf(const char *fmt, ...);

/* Optional value pattern using struct with has_value flag and union */
enum OptType {
    OPT_INT = 0,
    OPT_CHAR = 1,
    OPT_PAIR = 2
};

struct IntPair {
    int a;
    int b;
};

union OptVal {
    int i;
    char c;
    struct IntPair pair;
};

struct Optional {
    int has_value;
    int type;
    union OptVal val;
};

/* Create empty optional */
struct Optional opt_none(void) {
    struct Optional o;
    o.has_value = 0;
    o.type = OPT_INT;
    o.val.i = 0;
    return o;
}

/* Create optional with int */
struct Optional opt_int(int v) {
    struct Optional o;
    o.has_value = 1;
    o.type = OPT_INT;
    o.val.i = v;
    return o;
}

/* Create optional with char */
struct Optional opt_char(char c) {
    struct Optional o;
    o.has_value = 1;
    o.type = OPT_CHAR;
    o.val.c = c;
    return o;
}

/* Create optional with pair */
struct Optional opt_pair(int a, int b) {
    struct Optional o;
    o.has_value = 1;
    o.type = OPT_PAIR;
    o.val.pair.a = a;
    o.val.pair.b = b;
    return o;
}

/* Safe extraction with default */
int opt_get_int(struct Optional *o, int def) {
    if (!o->has_value || o->type != OPT_INT) return def;
    return o->val.i;
}

char opt_get_char(struct Optional *o, char def) {
    if (!o->has_value || o->type != OPT_CHAR) return def;
    return o->val.c;
}

int opt_get_pair_sum(struct Optional *o) {
    if (!o->has_value || o->type != OPT_PAIR) return -1;
    return o->val.pair.a + o->val.pair.b;
}

/* Map: apply function to optional int, return new optional */
struct Optional opt_map_double(struct Optional o) {
    if (!o.has_value || o.type != OPT_INT) return opt_none();
    return opt_int(o.val.i * 2);
}

struct Optional opt_map_add(struct Optional o, int n) {
    if (!o.has_value || o.type != OPT_INT) return opt_none();
    return opt_int(o.val.i + n);
}

/* Flat map: safe division returning optional */
struct Optional safe_div(int a, int b) {
    if (b == 0) return opt_none();
    return opt_int(a / b);
}

/* Chain: safe_div then multiply by 2 */
struct Optional div_and_double(int a, int b) {
    struct Optional r = safe_div(a, b);
    return opt_map_double(r);
}

/* Find first non-empty optional in array */
int find_first_value(struct Optional arr[], int count) {
    int i;
    for (i = 0; i < count; i++) {
        if (arr[i].has_value) return i;
    }
    return -1;
}

/* Count non-empty optionals */
int count_values(struct Optional arr[], int count) {
    int n = 0;
    int i;
    for (i = 0; i < count; i++) {
        if (arr[i].has_value) n++;
    }
    return n;
}

/* Sum all int optionals that have values */
int sum_int_optionals(struct Optional arr[], int count) {
    int total = 0;
    int i;
    for (i = 0; i < count; i++) {
        if (arr[i].has_value && arr[i].type == OPT_INT) {
            total += arr[i].val.i;
        }
    }
    return total;
}

int main(void) {
    /* Test opt_none */
    struct Optional empty = opt_none();
    // EXPECT: Has value: 0
    printf("Has value: %d\n", empty.has_value);

    /* Test opt_int */
    struct Optional oi = opt_int(42);
    // EXPECT: Has value: 1
    printf("Has value: %d\n", oi.has_value);
    // EXPECT: Int val: 42
    printf("Int val: %d\n", opt_get_int(&oi, -1));

    /* Test opt_char */
    struct Optional oc = opt_char(65);
    // EXPECT: Char val: 65
    printf("Char val: %d\n", (int)opt_get_char(&oc, 0));

    /* Test opt_pair */
    struct Optional op = opt_pair(10, 20);
    // EXPECT: Pair sum: 30
    printf("Pair sum: %d\n", opt_get_pair_sum(&op));

    /* Test default on empty */
    // EXPECT: Default: -1
    printf("Default: %d\n", opt_get_int(&empty, -1));

    /* Test wrong type returns default */
    // EXPECT: Wrong type: -1
    printf("Wrong type: %d\n", opt_get_int(&oc, -1));

    /* Test opt_map_double */
    struct Optional doubled = opt_map_double(opt_int(21));
    // EXPECT: Doubled: 42
    printf("Doubled: %d\n", opt_get_int(&doubled, -1));

    struct Optional doubled_none = opt_map_double(opt_none());
    // EXPECT: Doubled none: 0
    printf("Doubled none: %d\n", doubled_none.has_value);

    /* Test opt_map_add */
    struct Optional added = opt_map_add(opt_int(10), 5);
    // EXPECT: Added: 15
    printf("Added: %d\n", opt_get_int(&added, -1));

    /* Test safe_div */
    struct Optional div_ok = safe_div(10, 3);
    // EXPECT: 10/3: 3
    printf("10/3: %d\n", opt_get_int(&div_ok, -1));

    struct Optional div_fail = safe_div(10, 0);
    // EXPECT: 10/0 has value: 0
    printf("10/0 has value: %d\n", div_fail.has_value);

    /* Test chaining: div_and_double */
    struct Optional chain = div_and_double(20, 4);
    // EXPECT: 20/4*2: 10
    printf("20/4*2: %d\n", opt_get_int(&chain, -1));

    struct Optional chain_fail = div_and_double(20, 0);
    // EXPECT: Chain fail: 0
    printf("Chain fail: %d\n", chain_fail.has_value);

    /* Test array of optionals */
    struct Optional arr[6];
    arr[0] = opt_none();
    arr[1] = opt_int(10);
    arr[2] = opt_none();
    arr[3] = opt_int(20);
    arr[4] = opt_char(65);
    arr[5] = opt_int(30);

    // EXPECT: First value at: 1
    printf("First value at: %d\n", find_first_value(arr, 6));
    // EXPECT: Value count: 4
    printf("Value count: %d\n", count_values(arr, 6));
    // EXPECT: Int sum: 60
    printf("Int sum: %d\n", sum_int_optionals(arr, 6));

    /* Test pair in optional */
    struct Optional op2 = opt_pair(100, 200);
    // EXPECT: Pair has val: 1
    printf("Pair has val: %d\n", op2.has_value);
    // EXPECT: Pair type: 2
    printf("Pair type: %d\n", op2.type);
    // EXPECT: Pair a: 100
    printf("Pair a: %d\n", op2.val.pair.a);
    // EXPECT: Pair b: 200
    printf("Pair b: %d\n", op2.val.pair.b);

    return 0;
}
