// Bitfield stress test: varying widths, signed/unsigned mixing, arrays of structs
int printf(const char *fmt, ...);

struct packed1 {
    unsigned int a : 1;
    unsigned int b : 2;
    unsigned int c : 3;
    unsigned int d : 4;
    unsigned int e : 7;
    unsigned int f : 8;
    unsigned int g : 1;
    unsigned int h : 2;
    unsigned int i : 3;
    unsigned int j : 1;
};

struct signed_bits {
    signed int x : 4;
    signed int y : 8;
    unsigned int z : 4;
    signed int w : 3;
};

struct adjacent {
    unsigned int a : 3;
    unsigned int b : 3;
    unsigned int c : 3;
    unsigned int d : 3;
    unsigned int e : 4;
    unsigned int f : 8;
    unsigned int g : 8;
};

void test_basic(void) {
    struct packed1 p;
    p.a = 1;
    p.b = 3;
    p.c = 7;
    p.d = 15;
    p.e = 127;
    p.f = 255;
    p.g = 0;
    p.h = 1;
    p.i = 5;
    p.j = 1;
    printf("%u %u %u %u %u %u %u %u %u %u\n",
           p.a, p.b, p.c, p.d, p.e, p.f, p.g, p.h, p.i, p.j);
    // EXPECT: 1 3 7 15 127 255 0 1 5 1
}

void test_overflow(void) {
    struct packed1 p;
    p.a = 3;
    p.b = 7;
    p.c = 15;
    p.d = 31;
    unsigned int va = p.a;
    unsigned int vb = p.b;
    unsigned int vc = p.c;
    unsigned int vd = p.d;
    printf("%u %u %u %u\n", va & 1, vb & 3, vc & 7, vd & 15);
    // EXPECT: 1 3 7 15
}

void test_signed_bits(void) {
    struct signed_bits s;
    s.x = -1;
    s.y = -128;
    s.z = 15;
    s.w = -4;
    printf("%d %d %u %d\n", s.x, s.y, s.z, s.w);
    // EXPECT: -1 -128 15 -4
}

void test_signed_positive(void) {
    struct signed_bits s;
    s.x = 7;
    s.y = 127;
    s.z = 0;
    s.w = 3;
    printf("%d %d %u %d\n", s.x, s.y, s.z, s.w);
    // EXPECT: 7 127 0 3
}

void test_adjacent(void) {
    struct adjacent a;
    a.a = 5;
    a.b = 3;
    a.c = 7;
    a.d = 1;
    a.e = 9;
    a.f = 200;
    a.g = 42;
    printf("%u %u %u %u %u %u %u\n", a.a, a.b, a.c, a.d, a.e, a.f, a.g);
    // EXPECT: 5 3 7 1 9 200 42
}

void test_array_of_bitfield_structs(void) {
    struct packed1 arr[5];
    int i;
    for (i = 0; i < 5; i++) {
        arr[i].a = i & 1;
        arr[i].b = i & 3;
        arr[i].c = i;
        arr[i].d = i * 2;
        arr[i].e = i * 10;
        arr[i].f = i * 50;
        arr[i].g = 1;
        arr[i].h = 0;
        arr[i].i = i;
        arr[i].j = 1;
    }
    for (i = 0; i < 5; i++) {
        printf("%u %u %u %u %u %u %u %u %u %u\n",
               arr[i].a, arr[i].b, arr[i].c, arr[i].d,
               arr[i].e, arr[i].f, arr[i].g, arr[i].h,
               arr[i].i, arr[i].j);
    }
    // EXPECT: 0 0 0 0 0 0 1 0 0 1
    // EXPECT: 1 1 1 2 10 50 1 0 1 1
    // EXPECT: 0 2 2 4 20 100 1 0 2 1
    // EXPECT: 1 3 3 6 30 150 1 0 3 1
    // EXPECT: 0 0 4 8 40 200 1 0 4 1
}

void test_modify_single_field(void) {
    struct adjacent a;
    a.a = 7;
    a.b = 7;
    a.c = 7;
    a.d = 7;
    a.e = 15;
    a.f = 255;
    a.g = 255;
    a.b = 2;
    printf("%u %u %u %u %u %u %u\n", a.a, a.b, a.c, a.d, a.e, a.f, a.g);
    // EXPECT: 7 2 7 7 15 255 255
}

void test_bitfield_arithmetic(void) {
    struct signed_bits s;
    s.x = 3;
    s.y = 10;
    s.z = 5;
    s.w = -2;
    int result1 = s.x + s.y;
    int result2 = s.x * s.w;
    unsigned int result3 = s.z + 10;
    printf("%d %d %u\n", result1, result2, result3);
    // EXPECT: 13 -6 15
}

void test_zero_width_adjacent(void) {
    struct packed1 p;
    p.a = 0;
    p.b = 0;
    p.c = 0;
    p.d = 0;
    p.e = 0;
    p.f = 0;
    p.g = 0;
    p.h = 0;
    p.i = 0;
    p.j = 0;
    printf("%u %u %u %u %u %u %u %u %u %u\n",
           p.a, p.b, p.c, p.d, p.e, p.f, p.g, p.h, p.i, p.j);
    // EXPECT: 0 0 0 0 0 0 0 0 0 0
    p.e = 100;
    p.f = 200;
    printf("%u %u\n", p.e, p.f);
    // EXPECT: 100 200
}

void test_bitfield_in_condition(void) {
    struct packed1 p;
    p.a = 1;
    p.g = 0;
    if (p.a) {
        printf("a is set\n");
        // EXPECT: a is set
    }
    if (!p.g) {
        printf("g is not set\n");
        // EXPECT: g is not set
    }
    p.b = 2;
    if (p.b == 2) {
        printf("b equals 2\n");
        // EXPECT: b equals 2
    }
}

int main(void) {
    test_basic();
    test_overflow();
    test_signed_bits();
    test_signed_positive();
    test_adjacent();
    test_array_of_bitfield_structs();
    test_modify_single_field();
    test_bitfield_arithmetic();
    test_zero_width_adjacent();
    test_bitfield_in_condition();
    return 0;
}
