int printf(const char *fmt, ...);

struct Packed {
    int data;
};

/* Pack 4 bytes into one int: [byte3:24-31][byte2:16-23][byte1:8-15][byte0:0-7] */
void pack_set_byte(struct Packed *p, int index, int val) {
    int shift = index * 8;
    int mask = 255 << shift;
    p->data = (p->data & ~mask) | ((val & 255) << shift);
}

int pack_get_byte(struct Packed *p, int index) {
    int shift = index * 8;
    return (p->data >> shift) & 255;
}

/* Pack two 16-bit values: [hi:16-31][lo:0-15] */
struct TwoPack {
    int data;
};

void twopack_set(struct TwoPack *tp, int lo, int hi) {
    tp->data = (lo & 65535) | ((hi & 65535) << 16);
}

int twopack_get_lo(struct TwoPack *tp) {
    return tp->data & 65535;
}

int twopack_get_hi(struct TwoPack *tp) {
    return (tp->data >> 16) & 65535;
}

/* Pack RGB color: [unused:24-31][blue:16-23][green:8-15][red:0-7] */
struct Color {
    int rgb;
};

void color_set(struct Color *c, int r, int g, int b) {
    c->rgb = (r & 255) | ((g & 255) << 8) | ((b & 255) << 16);
}

int color_red(struct Color *c) { return c->rgb & 255; }
int color_green(struct Color *c) { return (c->rgb >> 8) & 255; }
int color_blue(struct Color *c) { return (c->rgb >> 16) & 255; }

/* Flags: individual bits in an int */
struct Flags {
    int bits;
};

void flags_clear(struct Flags *f) { f->bits = 0; }
void flags_set(struct Flags *f, int bit) { f->bits = f->bits | (1 << bit); }
void flags_unset(struct Flags *f, int bit) { f->bits = f->bits & ~(1 << bit); }
int flags_test(struct Flags *f, int bit) { return (f->bits >> bit) & 1; }
int flags_count(struct Flags *f) {
    int c = 0;
    int v = f->bits;
    while (v) {
        c = c + (v & 1);
        v = v >> 1;
    }
    return c;
}

/* Pack array of 4-bit nibbles into int array (8 nibbles per int) */
struct NibbleArray {
    int data[4];
    int count;
};

void nibble_init(struct NibbleArray *na) {
    int i;
    for (i = 0; i < 4; i = i + 1) na->data[i] = 0;
    na->count = 0;
}

void nibble_set(struct NibbleArray *na, int index, int val) {
    int word = index / 8;
    int pos = (index % 8) * 4;
    int mask = 15 << pos;
    na->data[word] = (na->data[word] & ~mask) | ((val & 15) << pos);
}

int nibble_get(struct NibbleArray *na, int index) {
    int word = index / 8;
    int pos = (index % 8) * 4;
    return (na->data[word] >> pos) & 15;
}

int main(void) {
    /* Byte packing */
    struct Packed p;
    p.data = 0;
    pack_set_byte(&p, 0, 10);
    pack_set_byte(&p, 1, 20);
    pack_set_byte(&p, 2, 30);
    pack_set_byte(&p, 3, 40);

    // EXPECT: byte0=10
    printf("byte0=%d\n", pack_get_byte(&p, 0));
    // EXPECT: byte1=20
    printf("byte1=%d\n", pack_get_byte(&p, 1));
    // EXPECT: byte2=30
    printf("byte2=%d\n", pack_get_byte(&p, 2));
    // EXPECT: byte3=40
    printf("byte3=%d\n", pack_get_byte(&p, 3));

    pack_set_byte(&p, 1, 99);
    // EXPECT: modified byte1=99
    printf("modified byte1=%d\n", pack_get_byte(&p, 1));
    // EXPECT: byte0 unchanged=10
    printf("byte0 unchanged=%d\n", pack_get_byte(&p, 0));

    /* Two-value packing */
    struct TwoPack tp;
    twopack_set(&tp, 1000, 2000);
    // EXPECT: lo=1000
    printf("lo=%d\n", twopack_get_lo(&tp));
    // EXPECT: hi=2000
    printf("hi=%d\n", twopack_get_hi(&tp));

    /* Color packing */
    struct Color c;
    color_set(&c, 255, 128, 64);
    // EXPECT: red=255
    printf("red=%d\n", color_red(&c));
    // EXPECT: green=128
    printf("green=%d\n", color_green(&c));
    // EXPECT: blue=64
    printf("blue=%d\n", color_blue(&c));

    color_set(&c, 0, 0, 0);
    // EXPECT: black r=0 g=0 b=0
    printf("black r=%d g=%d b=%d\n", color_red(&c), color_green(&c), color_blue(&c));

    /* Bit flags */
    struct Flags f;
    flags_clear(&f);
    flags_set(&f, 0);
    flags_set(&f, 3);
    flags_set(&f, 7);
    // EXPECT: flag0=1 flag1=0 flag3=1 flag7=1
    printf("flag0=%d flag1=%d flag3=%d flag7=%d\n",
           flags_test(&f, 0), flags_test(&f, 1), flags_test(&f, 3), flags_test(&f, 7));
    // EXPECT: flag count=3
    printf("flag count=%d\n", flags_count(&f));

    flags_unset(&f, 3);
    // EXPECT: after unset flag3=0
    printf("after unset flag3=%d\n", flags_test(&f, 3));
    // EXPECT: flag count=2
    printf("flag count=%d\n", flags_count(&f));

    /* Nibble array */
    struct NibbleArray na;
    nibble_init(&na);
    nibble_set(&na, 0, 5);
    nibble_set(&na, 1, 10);
    nibble_set(&na, 2, 15);
    nibble_set(&na, 8, 7);
    // EXPECT: nib[0]=5 nib[1]=10 nib[2]=15 nib[8]=7
    printf("nib[0]=%d nib[1]=%d nib[2]=%d nib[8]=%d\n",
           nibble_get(&na, 0), nibble_get(&na, 1), nibble_get(&na, 2), nibble_get(&na, 8));

    nibble_set(&na, 1, 3);
    // EXPECT: modified nib[1]=3
    printf("modified nib[1]=%d\n", nibble_get(&na, 1));

    return 0;
}
