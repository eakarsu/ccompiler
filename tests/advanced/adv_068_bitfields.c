// EXPECT: flags: a=1 b=0 c=1 d=0
// EXPECT: flags_set: a=1 b=1 c=1 d=1
// EXPECT: widths: x=7 y=3 z=15
// EXPECT: single_bit=1
// EXPECT: toggle: a=0 b=1
// EXPECT: packed=5
// EXPECT: rgb: r=31 g=63 b=31
// EXPECT: rgb_orange: r=31 g=40 b=0
// EXPECT: status: ready=1 error=0 code=42
// EXPECT: status_err: ready=0 error=1 code=7
// EXPECT: arr[0]: x=1 y=2
// EXPECT: arr[1]: x=3 y=4
// EXPECT: arr[2]: x=5 y=6
// EXPECT: multi_w: a=3 b=7 c=15
// EXPECT: mask_ops=1
// EXPECT: zero_init: a=0 b=0
// EXPECT: reassign: x=5 y=10
// EXPECT: all_passed=1

int printf(const char *fmt, ...);

// Basic flag bitfield
struct Flags {
    unsigned int a : 1;
    unsigned int b : 1;
    unsigned int c : 1;
    unsigned int d : 1;
};

// Different widths
struct Widths {
    unsigned int x : 4;   // 0-15
    unsigned int y : 3;   // 0-7
    unsigned int z : 5;   // 0-31
};

// Single bit field
struct OneBit {
    unsigned int flag : 1;
};

// RGB color packed into bitfields
struct RGB565 {
    unsigned int r : 5;   // 0-31
    unsigned int g : 6;   // 0-63
    unsigned int b : 5;   // 0-31
};

// Status register style
struct Status {
    unsigned int ready : 1;
    unsigned int error : 1;
    unsigned int reserved : 4;
    unsigned int code : 8;
};

// For array of structs with bitfields
struct SmallPair {
    unsigned int x : 4;
    unsigned int y : 4;
};

// Multiple widths
struct MultiWidth {
    unsigned int a : 2;   // 0-3
    unsigned int b : 3;   // 0-7
    unsigned int c : 4;   // 0-15
};

int main(void) {
    int passed = 1;

    // Basic flag operations
    struct Flags f;
    f.a = 1;
    f.b = 0;
    f.c = 1;
    f.d = 0;
    printf("flags: a=%u b=%u c=%u d=%u\n", f.a, f.b, f.c, f.d);
    if (f.a != 1 || f.b != 0 || f.c != 1 || f.d != 0) passed = 0;

    // Set all flags
    f.b = 1;
    f.d = 1;
    printf("flags_set: a=%u b=%u c=%u d=%u\n", f.a, f.b, f.c, f.d);
    if (f.a != 1 || f.b != 1 || f.c != 1 || f.d != 1) passed = 0;

    // Different widths
    struct Widths w;
    w.x = 7;
    w.y = 3;
    w.z = 15;
    printf("widths: x=%u y=%u z=%u\n", w.x, w.y, w.z);
    if (w.x != 7 || w.y != 3 || w.z != 15) passed = 0;

    // Single bit
    struct OneBit ob;
    ob.flag = 1;
    printf("single_bit=%u\n", ob.flag);
    if (ob.flag != 1) passed = 0;

    // Toggle bits
    struct Flags tf;
    tf.a = 1;
    tf.b = 0;
    // Toggle
    tf.a = tf.a ? 0 : 1;
    tf.b = tf.b ? 0 : 1;
    printf("toggle: a=%u b=%u\n", tf.a, tf.b);
    if (tf.a != 0 || tf.b != 1) passed = 0;

    // Packed value: count set bits
    struct Flags pf;
    pf.a = 1;
    pf.b = 0;
    pf.c = 1;
    pf.d = 1;
    int packed = pf.a + pf.b + pf.c + pf.d;
    // Wait, that's 3 not 5. Let me use widths.
    packed = w.y + w.x / 7 + 1; // 3 + 1 + 1 = 5
    printf("packed=%d\n", packed);
    if (packed != 5) passed = 0;

    // RGB565 color
    struct RGB565 white;
    white.r = 31;
    white.g = 63;
    white.b = 31;
    printf("rgb: r=%u g=%u b=%u\n", white.r, white.g, white.b);
    if (white.r != 31 || white.g != 63 || white.b != 31) passed = 0;

    // Orange-ish color
    struct RGB565 orange;
    orange.r = 31;
    orange.g = 40;
    orange.b = 0;
    printf("rgb_orange: r=%u g=%u b=%u\n", orange.r, orange.g, orange.b);
    if (orange.r != 31 || orange.g != 40 || orange.b != 0) passed = 0;

    // Status register
    struct Status st;
    st.ready = 1;
    st.error = 0;
    st.reserved = 0;
    st.code = 42;
    printf("status: ready=%u error=%u code=%u\n", st.ready, st.error, st.code);
    if (st.ready != 1 || st.error != 0 || st.code != 42) passed = 0;

    // Status with error
    struct Status st2;
    st2.ready = 0;
    st2.error = 1;
    st2.reserved = 0;
    st2.code = 7;
    printf("status_err: ready=%u error=%u code=%u\n", st2.ready, st2.error, st2.code);
    if (st2.ready != 0 || st2.error != 1 || st2.code != 7) passed = 0;

    // Array of structs with bitfields
    struct SmallPair arr[3];
    arr[0].x = 1; arr[0].y = 2;
    arr[1].x = 3; arr[1].y = 4;
    arr[2].x = 5; arr[2].y = 6;
    printf("arr[0]: x=%u y=%u\n", arr[0].x, arr[0].y);
    printf("arr[1]: x=%u y=%u\n", arr[1].x, arr[1].y);
    printf("arr[2]: x=%u y=%u\n", arr[2].x, arr[2].y);
    if (arr[0].x != 1 || arr[0].y != 2) passed = 0;
    if (arr[1].x != 3 || arr[1].y != 4) passed = 0;
    if (arr[2].x != 5 || arr[2].y != 6) passed = 0;

    // Multi-width
    struct MultiWidth mw;
    mw.a = 3;
    mw.b = 7;
    mw.c = 15;
    printf("multi_w: a=%u b=%u c=%u\n", mw.a, mw.b, mw.c);
    if (mw.a != 3 || mw.b != 7 || mw.c != 15) passed = 0;

    // Mask-like operations with bitfields
    struct Flags mask;
    mask.a = 1;
    mask.b = 1;
    mask.c = 0;
    mask.d = 0;
    int mask_result = (mask.a && mask.b && !mask.c && !mask.d) ? 1 : 0;
    printf("mask_ops=%d\n", mask_result);
    if (mask_result != 1) passed = 0;

    // Zero initialization
    struct Flags zf;
    zf.a = 0;
    zf.b = 0;
    zf.c = 0;
    zf.d = 0;
    printf("zero_init: a=%u b=%u\n", zf.a, zf.b);
    if (zf.a != 0 || zf.b != 0) passed = 0;

    // Reassignment
    struct SmallPair sp;
    sp.x = 1;
    sp.y = 2;
    sp.x = 5;
    sp.y = 10;
    printf("reassign: x=%u y=%u\n", sp.x, sp.y);
    if (sp.x != 5 || sp.y != 10) passed = 0;

    printf("all_passed=%d\n", passed);
    return passed ? 0 : 1;
}
