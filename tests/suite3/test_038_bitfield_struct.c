int printf(const char *fmt, ...);

/* Basic bitfield struct */
struct Flags {
    unsigned int active  : 1;
    unsigned int visible : 1;
    unsigned int locked  : 1;
    unsigned int level   : 4;   /* 0-15 */
    unsigned int mode    : 3;   /* 0-7  */
};

/* Signed bitfield */
struct SignedBits {
    int low  : 4;   /* -8..7  */
    int high : 4;   /* -8..7  */
};

/* Bitfield alongside normal fields */
struct Packed {
    int          id;
    unsigned int bits : 8;   /* 0-255 */
    int          value;
};

/* Bitfield for a small status word */
struct Status {
    unsigned int code    : 5;   /* 0-31 */
    unsigned int error   : 1;
    unsigned int ready   : 1;
    unsigned int pending : 1;
};

/* Count set bits in an unsigned int (popcount) */
int popcount(unsigned int x) {
    int n = 0;
    while (x) {
        n += x & 1;
        x >>= 1;
    }
    return n;
}

int main(void) {
    /* --- Basic unsigned bitfields --- */
    struct Flags f;
    f.active  = 1;
    f.visible = 0;
    f.locked  = 1;
    f.level   = 9;
    f.mode    = 5;

    printf("%d\n", f.active);               // EXPECT: 1
    printf("%d\n", f.visible);              // EXPECT: 0
    printf("%d\n", f.locked);              // EXPECT: 1
    printf("%d\n", f.level);               // EXPECT: 9
    printf("%d\n", f.mode);                // EXPECT: 5

    /* Write max values into bitfields */
    f.level = 15;   /* max 4-bit = 15 */
    f.mode  = 7;    /* max 3-bit = 7  */
    printf("%d\n", f.level);               // EXPECT: 15
    printf("%d\n", f.mode);                // EXPECT: 7

    /* Write zero */
    f.active  = 0;
    f.locked  = 0;
    f.level   = 0;
    f.mode    = 0;
    printf("%d\n", f.active);              // EXPECT: 0
    printf("%d\n", f.locked);              // EXPECT: 0
    printf("%d\n", f.level);              // EXPECT: 0

    /* Overflow wraps: writing 16 into 4-bit field = 0 */
    f.level = 16;   /* 16 & 0xF = 0 */
    printf("%d\n", f.level);              // EXPECT: 0

    /* Writing 17 into 4-bit = 1 */
    f.level = 17;
    printf("%d\n", f.level);              // EXPECT: 1

    /* --- Signed bitfields --- */
    struct SignedBits sb;
    sb.low  = 3;
    sb.high = -4;
    printf("%d\n", sb.low);               // EXPECT: 3
    printf("%d\n", sb.high);              // EXPECT: -4

    sb.low  = -8;   /* min 4-bit signed */
    sb.high = 7;    /* max 4-bit signed */
    printf("%d\n", sb.low);               // EXPECT: -8
    printf("%d\n", sb.high);              // EXPECT: 7

    /* Arithmetic on signed bitfields */
    sb.low  = 5;
    sb.high = -3;
    printf("%d\n", sb.low + sb.high);     // EXPECT: 2

    /* --- Bitfield alongside normal fields --- */
    struct Packed pk;
    pk.id    = 42;
    pk.bits  = 200;
    pk.value = -99;

    printf("%d\n", pk.id);                // EXPECT: 42
    printf("%d\n", pk.bits);              // EXPECT: 200
    printf("%d\n", pk.value);             // EXPECT: -99

    pk.bits = 255;   /* max 8-bit */
    printf("%d\n", pk.bits);              // EXPECT: 255

    pk.bits = 256;   /* wraps to 0 */
    printf("%d\n", pk.bits);              // EXPECT: 0

    /* id and value unchanged by bits overflow */
    printf("%d\n", pk.id);                // EXPECT: 42
    printf("%d\n", pk.value);             // EXPECT: -99

    /* --- Status struct --- */
    struct Status st;
    st.code    = 7;
    st.error   = 0;
    st.ready   = 1;
    st.pending = 0;

    printf("%d\n", st.code);              // EXPECT: 7
    printf("%d\n", st.error);             // EXPECT: 0
    printf("%d\n", st.ready);             // EXPECT: 1
    printf("%d\n", st.pending);           // EXPECT: 0

    st.error   = 1;
    st.ready   = 0;
    st.pending = 1;
    printf("%d\n", st.error);             // EXPECT: 1
    printf("%d\n", st.ready);             // EXPECT: 0
    printf("%d\n", st.pending);           // EXPECT: 1

    /* Max code value: 31 */
    st.code = 31;
    printf("%d\n", st.code);              // EXPECT: 31

    /* --- Bitfield in loop --- */
    struct Flags flags_arr[4];
    int i;
    for (i = 0; i < 4; i++) {
        flags_arr[i].active  = i % 2;
        flags_arr[i].visible = 1;
        flags_arr[i].level   = i * 3;
    }
    /* i=0: active=0,level=0 | i=1: active=1,level=3
       i=2: active=0,level=6 | i=3: active=1,level=9 */
    printf("%d %d\n", flags_arr[0].active, flags_arr[0].level);  // EXPECT: 0 0
    printf("%d %d\n", flags_arr[1].active, flags_arr[1].level);  // EXPECT: 1 3
    printf("%d %d\n", flags_arr[2].active, flags_arr[2].level);  // EXPECT: 0 6
    printf("%d %d\n", flags_arr[3].active, flags_arr[3].level);  // EXPECT: 1 9

    /* --- Bitfield toggle --- */
    struct Flags tf;
    tf.active = 0;
    tf.active = 1;
    tf.active = !tf.active;   /* toggle: 0 */
    printf("%d\n", tf.active);             // EXPECT: 0
    tf.active = !tf.active;   /* toggle: 1 */
    printf("%d\n", tf.active);             // EXPECT: 1

    /* --- Sum of mode values --- */
    struct Flags fl[3];
    fl[0].mode = 1;
    fl[1].mode = 3;
    fl[2].mode = 7;
    int msum = fl[0].mode + fl[1].mode + fl[2].mode;
    /* 1+3+7 = 11 */
    printf("%d\n", msum);                  // EXPECT: 11

    return 0;
}
