int printf(const char *fmt, ...);

struct IntPair {
    int a;
    int b;
};

union PairOrSingle {
    struct IntPair pair;
    int single;
};

struct Vec2 {
    int x;
    int y;
};

struct Vec3 {
    int x;
    int y;
    int z;
};

union Vec {
    struct Vec2 v2;
    struct Vec3 v3;
};

struct TaggedInt {
    int tag; // 0 = int value, 1 = pair
    union {
        int value;
        struct IntPair pair;
    } data;
};

struct Color {
    union {
        int components[3];
        struct {
            int r;
            int g;
            int b;
        } rgb;
    } color;
};

struct Variant {
    int kind;
    union PairOrSingle content;
};

int test_union_with_struct(void) {
    union PairOrSingle u;
    u.pair.a = 10;
    u.pair.b = 20;
    return u.pair.a + u.pair.b; // 30
}

int test_single_overwrites_pair(void) {
    union PairOrSingle u;
    u.pair.a = 10;
    u.pair.b = 20;
    u.single = 99;
    // single overlaps with pair.a
    return u.single; // 99
}

int test_pair_after_single(void) {
    union PairOrSingle u;
    u.single = 55;
    // pair.a overlaps single
    return u.pair.a; // 55
}

int test_vec_union(void) {
    union Vec v;
    v.v3.x = 1;
    v.v3.y = 2;
    v.v3.z = 3;
    // v2 overlaps v3, so v2.x=1, v2.y=2
    return v.v2.x + v.v2.y; // 3
}

int test_tagged_int(void) {
    struct TaggedInt t;
    t.tag = 0;
    t.data.value = 42;
    return t.data.value; // 42
}

int test_tagged_pair(void) {
    struct TaggedInt t;
    t.tag = 1;
    t.data.pair.a = 10;
    t.data.pair.b = 20;
    return t.data.pair.a + t.data.pair.b; // 30
}

int test_tagged_dispatch(void) {
    struct TaggedInt items[2];
    items[0].tag = 0;
    items[0].data.value = 100;
    items[1].tag = 1;
    items[1].data.pair.a = 30;
    items[1].data.pair.b = 70;

    int sum = 0;
    int i;
    for (i = 0; i < 2; i = i + 1) {
        if (items[i].tag == 0) {
            sum = sum + items[i].data.value;
        } else {
            sum = sum + items[i].data.pair.a + items[i].data.pair.b;
        }
    }
    return sum; // 100 + 30 + 70 = 200
}

int test_color_struct(void) {
    struct Color c;
    c.color.rgb.r = 255;
    c.color.rgb.g = 128;
    c.color.rgb.b = 64;
    return c.color.rgb.r; // 255
}

int test_color_via_array(void) {
    struct Color c;
    c.color.components[0] = 10;
    c.color.components[1] = 20;
    c.color.components[2] = 30;
    // components and rgb share memory
    return c.color.rgb.r + c.color.rgb.g + c.color.rgb.b;
    // 10 + 20 + 30 = 60
}

int test_color_crossover(void) {
    struct Color c;
    c.color.rgb.r = 100;
    c.color.rgb.g = 200;
    c.color.rgb.b = 50;
    // reading through components array
    return c.color.components[0] + c.color.components[1] + c.color.components[2];
    // 100 + 200 + 50 = 350
}

int test_variant(void) {
    struct Variant v;
    v.kind = 0;
    v.content.single = 77;
    return v.content.single; // 77
}

int test_variant_pair(void) {
    struct Variant v;
    v.kind = 1;
    v.content.pair.a = 11;
    v.content.pair.b = 22;
    return v.content.pair.a + v.content.pair.b; // 33
}

int test_sizeof_tagged(void) {
    // tag=4, union(int=4, IntPair=8) = 8, total = 12
    return sizeof(struct TaggedInt);
}

int test_sizeof_variant(void) {
    // kind=4, PairOrSingle(max(IntPair=8, int=4))=8, total = 12
    return sizeof(struct Variant);
}

int test_nested_pointer(void) {
    struct TaggedInt t;
    t.tag = 0;
    t.data.value = 88;
    struct TaggedInt *p = &t;
    return p->data.value; // 88
}

int main(void) {
    // EXPECT: union with struct: 30
    printf("union with struct: %d\n", test_union_with_struct());

    // EXPECT: single overwrites: 99
    printf("single overwrites: %d\n", test_single_overwrites_pair());

    // EXPECT: pair after single: 55
    printf("pair after single: %d\n", test_pair_after_single());

    // EXPECT: vec union: 3
    printf("vec union: %d\n", test_vec_union());

    // EXPECT: tagged int: 42
    printf("tagged int: %d\n", test_tagged_int());

    // EXPECT: tagged pair: 30
    printf("tagged pair: %d\n", test_tagged_pair());

    // EXPECT: tagged dispatch: 200
    printf("tagged dispatch: %d\n", test_tagged_dispatch());

    // EXPECT: color r: 255
    printf("color r: %d\n", test_color_struct());

    // EXPECT: color via array: 60
    printf("color via array: %d\n", test_color_via_array());

    // EXPECT: color crossover: 350
    printf("color crossover: %d\n", test_color_crossover());

    // EXPECT: variant single: 77
    printf("variant single: %d\n", test_variant());

    // EXPECT: variant pair: 33
    printf("variant pair: %d\n", test_variant_pair());

    // EXPECT: sizeof TaggedInt: 12
    printf("sizeof TaggedInt: %d\n", test_sizeof_tagged());

    // EXPECT: sizeof Variant: 12
    printf("sizeof Variant: %d\n", test_sizeof_variant());

    // EXPECT: nested pointer: 88
    printf("nested pointer: %d\n", test_nested_pointer());

    union PairOrSingle local;
    local.pair.a = 7;
    local.pair.b = 3;
    // EXPECT: local pair sum: 10
    printf("local pair sum: %d\n", local.pair.a + local.pair.b);

    return 0;
}
