int printf(const char *fmt, ...);
// EXPECT: === Size 1 (1 byte) ===\nS1: a=42\n=== Size 4 (4 bytes) ===\nS4: a=100\n=== Size 8 (8 bytes) ===\nS8: a=10 b=20\n=== Size 12 (12 bytes) ===\nS12: a=1 b=2 c=3\n=== Size 16 exact (via x0/x1) ===\nS16a: a=100 b=200 c=300 d=400\nS16b: a=1000000 b=2000000\n=== Size 17 (via x8 pointer) ===\nS17: a=10 b=20 c=30 d=40 e=50\n=== Size 20 (via x8 pointer) ===\nS20: a=5 b=10 c=15 d=20 e=25\n=== Size 24 (via x8 pointer) ===\nS24: a=11 b=22 c=33 d=44 e=55 f=66\n=== Size 32 (via x8 pointer) ===\nS32: a=1 b=2 c=3 d=4 e=5 f=6 g=7 h=8\n=== Arithmetic on returned structs ===\nadd16: a=15 b=25 c=35 d=45\nadd17: a=11 b=22 c=33 d=44 e=55\nadd24: a=14 b=24 c=34 d=44 e=54 f=64\nadd32: a=5 b=7 c=9 d=11 e=13 f=15 g=17 h=19\n=== Chain: return -> pass -> return ===\ndoubled16: a=200 b=400 c=600 d=800\ndoubled17: a=20 b=40 c=60 d=80 e=100\ndoubled24: a=22 b=44 c=66 d=88 e=110 f=132\ndoubled32: a=2 b=4 c=6 d=8 e=10 f=12 g=14 h=16\n=== Nested chain ===\nchain16: a=600 b=1200 c=1800 d=2400\nchain17: a=60 b=120 c=180 d=240 e=44\n=== Mix small and large returns ===\nmixed: s8=(30,70) s16=(11,22,33,44) s17=(6,7,8,9,10) s24=(2,4,6,8,10,12)\n=== Struct return into array ===\narr[0]: a=0 b=0 c=0 d=0\narr[1]: a=1 b=2 c=3 d=4\narr[2]: a=2 b=4 c=6 d=8\narr[3]: a=3 b=6 c=9 d=12\narr_sum: a=6 b=12 c=18 d=24\n=== Conditional struct return ===\ncond_true: a=1 b=2 c=3 d=4\ncond_false: a=10 b=20 c=30 d=40\ncond17_true: a=5 b=6 c=7 d=8 e=9\ncond17_false: a=50 b=60 c=70 d=80 e=90\n=== Field access on return value ===\nfield_x0x1: 100 + 400 = 500\nfield_x8: 10 + 50 = 60\nDone.
// Test 601: Struct return ABI boundary - x0/x1 vs x8 hidden pointer

// --- 1 byte struct ---
struct S1 { char a; };

struct S1 make_s1(char v) {
    struct S1 s;
    s.a = v;
    return s;
}

// --- 4 byte struct ---
struct S4 { int a; };

struct S4 make_s4(int v) {
    struct S4 s;
    s.a = v;
    return s;
}

// --- 8 byte struct (fits in x0) ---
struct S8 { int a; int b; };

struct S8 make_s8(int a, int b) {
    struct S8 s;
    s.a = a;
    s.b = b;
    return s;
}

// --- 12 byte struct (fits in x0/x1) ---
struct S12 { int a; int b; int c; };

struct S12 make_s12(int a, int b, int c) {
    struct S12 s;
    s.a = a;
    s.b = b;
    s.c = c;
    return s;
}

// --- 16 byte struct: exactly the boundary (x0/x1) ---
struct S16 { int a; int b; int c; int d; };

struct S16 make_s16(int a, int b, int c, int d) {
    struct S16 s;
    s.a = a;
    s.b = b;
    s.c = c;
    s.d = d;
    return s;
}

// --- 16 byte struct with two longs (x0/x1) ---
struct S16L { long a; long b; };

struct S16L make_s16l(long a, long b) {
    struct S16L s;
    s.a = a;
    s.b = b;
    return s;
}

// --- 17 byte struct: just over boundary (x8 pointer) ---
struct S17 { int a; int b; int c; int d; char e; };

struct S17 make_s17(int a, int b, int c, int d, int e) {
    struct S17 s;
    s.a = a;
    s.b = b;
    s.c = c;
    s.d = d;
    s.e = e;
    return s;
}

// --- 20 byte struct (x8 pointer) ---
struct S20 { int a; int b; int c; int d; int e; };

struct S20 make_s20(int a, int b, int c, int d, int e) {
    struct S20 s;
    s.a = a;
    s.b = b;
    s.c = c;
    s.d = d;
    s.e = e;
    return s;
}

// --- 24 byte struct (x8 pointer) ---
struct S24 { int a; int b; int c; int d; int e; int f; };

struct S24 make_s24(int a, int b, int c, int d, int e, int f) {
    struct S24 s;
    s.a = a;
    s.b = b;
    s.c = c;
    s.d = d;
    s.e = e;
    s.f = f;
    return s;
}

// --- 32 byte struct (x8 pointer) ---
struct S32 { int a; int b; int c; int d; int e; int f; int g; int h; };

struct S32 make_s32(int a, int b, int c, int d, int e, int f, int g, int h) {
    struct S32 s;
    s.a = a;
    s.b = b;
    s.c = c;
    s.d = d;
    s.e = e;
    s.f = f;
    s.g = g;
    s.h = h;
    return s;
}

// --- Addition functions for each size ---
struct S16 add_s16(struct S16 x, struct S16 y) {
    struct S16 r;
    r.a = x.a + y.a;
    r.b = x.b + y.b;
    r.c = x.c + y.c;
    r.d = x.d + y.d;
    return r;
}

struct S17 add_s17(struct S17 x, struct S17 y) {
    struct S17 r;
    r.a = x.a + y.a;
    r.b = x.b + y.b;
    r.c = x.c + y.c;
    r.d = x.d + y.d;
    r.e = x.e + y.e;
    return r;
}

struct S24 add_s24(struct S24 x, struct S24 y) {
    struct S24 r;
    r.a = x.a + y.a;
    r.b = x.b + y.b;
    r.c = x.c + y.c;
    r.d = x.d + y.d;
    r.e = x.e + y.e;
    r.f = x.f + y.f;
    return r;
}

struct S32 add_s32(struct S32 x, struct S32 y) {
    struct S32 r;
    r.a = x.a + y.a;
    r.b = x.b + y.b;
    r.c = x.c + y.c;
    r.d = x.d + y.d;
    r.e = x.e + y.e;
    r.f = x.f + y.f;
    r.g = x.g + y.g;
    r.h = x.h + y.h;
    return r;
}

// --- Double functions: chain return -> pass -> return ---
struct S16 double_s16(struct S16 x) {
    return add_s16(x, x);
}

struct S17 double_s17(struct S17 x) {
    return add_s17(x, x);
}

struct S24 double_s24(struct S24 x) {
    return add_s24(x, x);
}

struct S32 double_s32(struct S32 x) {
    return add_s32(x, x);
}

// --- Triple chain: make -> double -> double -> add ---
struct S16 triple_s16(int a, int b, int c, int d) {
    struct S16 base = make_s16(a, b, c, d);
    struct S16 d1 = double_s16(base);
    struct S16 d2 = double_s16(d1);
    return add_s16(d1, d2);
    // base: a,b,c,d -> d1: 2a,2b,2c,2d -> d2: 4a,4b,4c,4d
    // result: 6a,6b,6c,6d
}

struct S17 triple_s17(int a, int b, int c, int d, int e) {
    struct S17 base = make_s17(a, b, c, d, e);
    struct S17 d1 = double_s17(base);
    struct S17 d2 = double_s17(d1);
    return add_s17(d1, d2);
    // result: 6a,6b,6c,6d,6e
}

// --- Conditional return ---
struct S16 cond_s16(int flag) {
    if (flag) {
        return make_s16(1, 2, 3, 4);
    } else {
        return make_s16(10, 20, 30, 40);
    }
}

struct S17 cond_s17(int flag) {
    if (flag) {
        return make_s17(5, 6, 7, 8, 9);
    } else {
        return make_s17(50, 60, 70, 80, 90);
    }
}

int main(void) {
    // --- Basic returns of each size ---
    printf("=== Size 1 (1 byte) ===\n");
    struct S1 v1 = make_s1(42);
    printf("S1: a=%d\n", v1.a);

    printf("=== Size 4 (4 bytes) ===\n");
    struct S4 v4 = make_s4(100);
    printf("S4: a=%d\n", v4.a);

    printf("=== Size 8 (8 bytes) ===\n");
    struct S8 v8 = make_s8(10, 20);
    printf("S8: a=%d b=%d\n", v8.a, v8.b);

    printf("=== Size 12 (12 bytes) ===\n");
    struct S12 v12 = make_s12(1, 2, 3);
    printf("S12: a=%d b=%d c=%d\n", v12.a, v12.b, v12.c);

    printf("=== Size 16 exact (via x0/x1) ===\n");
    struct S16 v16 = make_s16(100, 200, 300, 400);
    printf("S16a: a=%d b=%d c=%d d=%d\n", v16.a, v16.b, v16.c, v16.d);
    struct S16L v16l = make_s16l(1000000, 2000000);
    printf("S16b: a=%ld b=%ld\n", v16l.a, v16l.b);

    printf("=== Size 17 (via x8 pointer) ===\n");
    struct S17 v17 = make_s17(10, 20, 30, 40, 50);
    printf("S17: a=%d b=%d c=%d d=%d e=%d\n", v17.a, v17.b, v17.c, v17.d, v17.e);

    printf("=== Size 20 (via x8 pointer) ===\n");
    struct S20 v20 = make_s20(5, 10, 15, 20, 25);
    printf("S20: a=%d b=%d c=%d d=%d e=%d\n", v20.a, v20.b, v20.c, v20.d, v20.e);

    printf("=== Size 24 (via x8 pointer) ===\n");
    struct S24 v24 = make_s24(11, 22, 33, 44, 55, 66);
    printf("S24: a=%d b=%d c=%d d=%d e=%d f=%d\n", v24.a, v24.b, v24.c, v24.d, v24.e, v24.f);

    printf("=== Size 32 (via x8 pointer) ===\n");
    struct S32 v32 = make_s32(1, 2, 3, 4, 5, 6, 7, 8);
    printf("S32: a=%d b=%d c=%d d=%d e=%d f=%d g=%d h=%d\n",
           v32.a, v32.b, v32.c, v32.d, v32.e, v32.f, v32.g, v32.h);

    // --- Arithmetic on returned structs ---
    printf("=== Arithmetic on returned structs ===\n");
    struct S16 a16 = make_s16(5, 10, 15, 20);
    struct S16 b16 = make_s16(10, 15, 20, 25);
    struct S16 c16 = add_s16(a16, b16);
    printf("add16: a=%d b=%d c=%d d=%d\n", c16.a, c16.b, c16.c, c16.d);

    struct S17 a17 = make_s17(1, 2, 3, 4, 5);
    struct S17 b17 = make_s17(10, 20, 30, 40, 50);
    struct S17 c17 = add_s17(a17, b17);
    printf("add17: a=%d b=%d c=%d d=%d e=%d\n", c17.a, c17.b, c17.c, c17.d, c17.e);

    struct S24 a24 = make_s24(4, 8, 12, 16, 20, 24);
    struct S24 b24 = make_s24(10, 16, 22, 28, 34, 40);
    struct S24 c24 = add_s24(a24, b24);
    printf("add24: a=%d b=%d c=%d d=%d e=%d f=%d\n", c24.a, c24.b, c24.c, c24.d, c24.e, c24.f);

    struct S32 a32 = make_s32(1, 2, 3, 4, 5, 6, 7, 8);
    struct S32 b32 = make_s32(4, 5, 6, 7, 8, 9, 10, 11);
    struct S32 c32 = add_s32(a32, b32);
    printf("add32: a=%d b=%d c=%d d=%d e=%d f=%d g=%d h=%d\n",
           c32.a, c32.b, c32.c, c32.d, c32.e, c32.f, c32.g, c32.h);

    // --- Chain: return -> pass -> return ---
    printf("=== Chain: return -> pass -> return ===\n");
    struct S16 dbl16 = double_s16(v16);
    printf("doubled16: a=%d b=%d c=%d d=%d\n", dbl16.a, dbl16.b, dbl16.c, dbl16.d);

    struct S17 dbl17 = double_s17(v17);
    printf("doubled17: a=%d b=%d c=%d d=%d e=%d\n", dbl17.a, dbl17.b, dbl17.c, dbl17.d, dbl17.e);

    struct S24 dbl24 = double_s24(v24);
    printf("doubled24: a=%d b=%d c=%d d=%d e=%d f=%d\n", dbl24.a, dbl24.b, dbl24.c, dbl24.d, dbl24.e, dbl24.f);

    struct S32 dbl32 = double_s32(v32);
    printf("doubled32: a=%d b=%d c=%d d=%d e=%d f=%d g=%d h=%d\n",
           dbl32.a, dbl32.b, dbl32.c, dbl32.d, dbl32.e, dbl32.f, dbl32.g, dbl32.h);

    // --- Nested chain: make -> double -> double -> add ---
    printf("=== Nested chain ===\n");
    struct S16 t16 = triple_s16(100, 200, 300, 400);
    printf("chain16: a=%d b=%d c=%d d=%d\n", t16.a, t16.b, t16.c, t16.d);

    struct S17 t17 = triple_s17(10, 20, 30, 40, 50);
    printf("chain17: a=%d b=%d c=%d d=%d e=%d\n", t17.a, t17.b, t17.c, t17.d, t17.e);

    // --- Mix small and large returns in one expression ---
    printf("=== Mix small and large returns ===\n");
    struct S8 ms8 = make_s8(30, 70);
    struct S16 ms16 = make_s16(11, 22, 33, 44);
    struct S17 ms17 = make_s17(6, 7, 8, 9, 10);
    struct S24 ms24 = make_s24(2, 4, 6, 8, 10, 12);
    printf("mixed: s8=(%d,%d) s16=(%d,%d,%d,%d) s17=(%d,%d,%d,%d,%d) s24=(%d,%d,%d,%d,%d,%d)\n",
           ms8.a, ms8.b,
           ms16.a, ms16.b, ms16.c, ms16.d,
           ms17.a, ms17.b, ms17.c, ms17.d, ms17.e,
           ms24.a, ms24.b, ms24.c, ms24.d, ms24.e, ms24.f);

    // --- Struct return stored into array ---
    printf("=== Struct return into array ===\n");
    struct S16 arr[4];
    int i = 0;
    while (i < 4) {
        arr[i] = make_s16(i, i * 2, i * 3, i * 4);
        i = i + 1;
    }
    i = 0;
    while (i < 4) {
        printf("arr[%d]: a=%d b=%d c=%d d=%d\n", i, arr[i].a, arr[i].b, arr[i].c, arr[i].d);
        i = i + 1;
    }
    struct S16 arr_sum = make_s16(0, 0, 0, 0);
    i = 0;
    while (i < 4) {
        arr_sum = add_s16(arr_sum, arr[i]);
        i = i + 1;
    }
    printf("arr_sum: a=%d b=%d c=%d d=%d\n", arr_sum.a, arr_sum.b, arr_sum.c, arr_sum.d);

    // --- Conditional struct return ---
    printf("=== Conditional struct return ===\n");
    struct S16 ct = cond_s16(1);
    printf("cond_true: a=%d b=%d c=%d d=%d\n", ct.a, ct.b, ct.c, ct.d);
    struct S16 cf = cond_s16(0);
    printf("cond_false: a=%d b=%d c=%d d=%d\n", cf.a, cf.b, cf.c, cf.d);

    struct S17 ct17 = cond_s17(1);
    printf("cond17_true: a=%d b=%d c=%d d=%d e=%d\n", ct17.a, ct17.b, ct17.c, ct17.d, ct17.e);
    struct S17 cf17 = cond_s17(0);
    printf("cond17_false: a=%d b=%d c=%d d=%d e=%d\n", cf17.a, cf17.b, cf17.c, cf17.d, cf17.e);

    // --- Direct field access on function return value ---
    printf("=== Field access on return value ===\n");
    int fa = make_s16(100, 200, 300, 400).a;
    int fd = make_s16(100, 200, 300, 400).d;
    printf("field_x0x1: %d + %d = %d\n", fa, fd, fa + fd);

    int ga = make_s17(10, 20, 30, 40, 50).a;
    int ge = make_s17(10, 20, 30, 40, 50).e;
    printf("field_x8: %d + %d = %d\n", ga, ge, ga + ge);

    printf("Done.\n");
    return 0;
}
