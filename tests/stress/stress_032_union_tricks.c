// stress_032_union_tricks.c - Union abuse and type punning
// EXPECT: pun_bytes=4
// EXPECT: byte0_nonzero=1
// EXPECT: reassembled=305419896
// EXPECT: tagged_int=42
// EXPECT: tagged_float_approx=1
// EXPECT: tagged_str=hello
// EXPECT: union_of_structs_a=10 20
// EXPECT: union_of_structs_b=100 200 300
// EXPECT: sizeof_union_arr=40
// EXPECT: union_arr_first=1
// EXPECT: union_arr_last=9
// EXPECT: sizeof_u1=8
// EXPECT: sizeof_u2=12
// EXPECT: sizeof_u3=40
// EXPECT: ptr_cast_val=99
// EXPECT: multi_member=1 2 3
// EXPECT: union_in_array_0=111
// EXPECT: union_in_array_1=222
// EXPECT: union_init_zero=0
// EXPECT: overwrite_test=255
// EXPECT: pass_done=1

int printf(const char *fmt, ...);
void *malloc(unsigned long);
void free(void *);

// --- Type punning int <-> bytes ---
union IntBytes {
    int i;
    char b[4];
};

// --- Tagged union pattern ---
struct Tagged {
    int tag; // 0=int, 1=float_as_int, 2=string
    union {
        int ival;
        int fval_as_int; // approximate float as int
        char *sval;
    } data;
};

// --- Union of structs ---
struct PointA { int x; int y; };
struct PointB { int a; int b; int c; };

union StructUnion {
    struct PointA pa;
    struct PointB pb;
};

// --- Union of arrays ---
union ArrUnion {
    int arr1[10];
    char arr2[40];
};

// --- Various size unions for sizeof tests ---
union U1 { int a; long b; };
union U2 { char c[12]; int d; };
union U3 { int arr[10]; };

// --- Union pointer casting ---
union PtrCast {
    int *ip;
    char *cp;
};

// --- Union with multiple same-size members ---
struct Pair { int a; int b; };
struct Duo { int x; int y; };

union MultiMember {
    int raw[3];
    struct { int f1; int f2; int f3; } fields;
};

// Helper to print tagged union
void print_tagged(struct Tagged *t) {
    if (t->tag == 0) {
        printf("tagged_int=%d\n", t->data.ival);
    } else if (t->tag == 1) {
        // Just check it's nonzero as an approximation
        printf("tagged_float_approx=%d\n", t->data.fval_as_int != 0 ? 1 : 0);
    } else if (t->tag == 2) {
        printf("tagged_str=%s\n", t->data.sval);
    }
}

int main(void) {
    // Type punning int <-> bytes
    union IntBytes ib;
    ib.i = 305419896; // 0x12345678
    printf("pun_bytes=%d\n", (int)sizeof(ib.b));
    // At least one byte should be nonzero
    int any_nonzero = 0;
    int idx;
    for (idx = 0; idx < 4; idx++) {
        if (ib.b[idx] != 0) any_nonzero = 1;
    }
    printf("byte0_nonzero=%d\n", any_nonzero);

    // Reassemble bytes back
    union IntBytes ib2;
    for (idx = 0; idx < 4; idx++) ib2.b[idx] = ib.b[idx];
    printf("reassembled=%d\n", ib2.i);

    // Tagged union
    struct Tagged t1;
    t1.tag = 0;
    t1.data.ival = 42;
    print_tagged(&t1);

    struct Tagged t2;
    t2.tag = 1;
    t2.data.fval_as_int = 1065353216; // IEEE 754 for 1.0f
    print_tagged(&t2);

    struct Tagged t3;
    t3.tag = 2;
    t3.data.sval = "hello";
    print_tagged(&t3);

    // Union of structs
    union StructUnion su;
    su.pa.x = 10;
    su.pa.y = 20;
    printf("union_of_structs_a=%d %d\n", su.pa.x, su.pa.y);
    su.pb.a = 100;
    su.pb.b = 200;
    su.pb.c = 300;
    printf("union_of_structs_b=%d %d %d\n", su.pb.a, su.pb.b, su.pb.c);

    // Union of arrays
    union ArrUnion au;
    printf("sizeof_union_arr=%d\n", (int)sizeof(au));
    au.arr1[0] = 1;
    au.arr1[9] = 9;
    printf("union_arr_first=%d\n", au.arr1[0]);
    printf("union_arr_last=%d\n", au.arr1[9]);

    // sizeof verification
    printf("sizeof_u1=%d\n", (int)sizeof(union U1));
    printf("sizeof_u2=%d\n", (int)sizeof(union U2));
    printf("sizeof_u3=%d\n", (int)sizeof(union U3));

    // Union pointer casting
    int val = 99;
    union PtrCast pc;
    pc.ip = &val;
    printf("ptr_cast_val=%d\n", *pc.ip);

    // Multi-member union
    union MultiMember mm;
    mm.fields.f1 = 1;
    mm.fields.f2 = 2;
    mm.fields.f3 = 3;
    printf("multi_member=%d %d %d\n", mm.raw[0], mm.raw[1], mm.raw[2]);

    // Array of tagged unions
    struct Tagged arr[2];
    arr[0].tag = 0;
    arr[0].data.ival = 111;
    arr[1].tag = 0;
    arr[1].data.ival = 222;
    printf("union_in_array_0=%d\n", arr[0].data.ival);
    printf("union_in_array_1=%d\n", arr[1].data.ival);

    // Zero init union
    union IntBytes zib;
    zib.i = 0;
    printf("union_init_zero=%d\n", zib.i);

    // Overwrite test: write int then read bytes
    union IntBytes ow;
    ow.i = 0;
    ow.b[0] = -1; // 0xFF
    // Read back the whole int - low byte should be 0xFF
    // Just verify b[0] is what we set
    printf("overwrite_test=%d\n", (int)(unsigned char)ow.b[0]);

    printf("pass_done=1\n");
    return 0;
}
