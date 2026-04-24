int printf(const char *fmt, ...);

typedef struct {
    int data;
} BitFields;

int bf_get(BitFields *bf, int start, int width) {
    return (bf->data >> start) & ((1 << width) - 1);
}

void bf_set(BitFields *bf, int start, int width, int value) {
    int mask;
    mask = ((1 << width) - 1) << start;
    bf->data = (bf->data & (~mask)) | ((value << start) & mask);
}

void bf_clear(BitFields *bf, int start, int width) {
    int mask;
    mask = ((1 << width) - 1) << start;
    bf->data = bf->data & (~mask);
}

void bf_toggle(BitFields *bf, int start, int width) {
    int mask;
    mask = ((1 << width) - 1) << start;
    bf->data = bf->data ^ mask;
}

int bf_test_any(BitFields *bf, int start, int width) {
    int mask;
    mask = ((1 << width) - 1) << start;
    return (bf->data & mask) != 0;
}

int bf_test_all(BitFields *bf, int start, int width) {
    int mask;
    mask = ((1 << width) - 1) << start;
    return (bf->data & mask) == mask;
}

int bf_count_set(BitFields *bf, int start, int width) {
    int val;
    int count;
    int i;
    val = bf_get(bf, start, width);
    count = 0;
    for (i = 0; i < width; i = i + 1) {
        count = count + ((val >> i) & 1);
    }
    return count;
}

void permission_demo(BitFields *perms) {
    int read_bit;
    int write_bit;
    int exec_bit;
    read_bit = bf_get(perms, 2, 1);
    write_bit = bf_get(perms, 1, 1);
    exec_bit = bf_get(perms, 0, 1);
    printf("perms: r=%d w=%d x=%d\n", read_bit, write_bit, exec_bit);
}

void status_register_demo() {
    BitFields sr;
    int carry;
    int zero;
    int neg;
    int overflow;

    sr.data = 0;

    bf_set(&sr, 0, 1, 1);
    bf_set(&sr, 1, 1, 0);
    bf_set(&sr, 2, 1, 1);
    bf_set(&sr, 3, 1, 0);

    carry = bf_get(&sr, 0, 1);
    zero = bf_get(&sr, 1, 1);
    neg = bf_get(&sr, 2, 1);
    overflow = bf_get(&sr, 3, 1);

    printf("status: C=%d Z=%d N=%d V=%d\n", carry, zero, neg, overflow);

    bf_toggle(&sr, 0, 4);
    carry = bf_get(&sr, 0, 1);
    zero = bf_get(&sr, 1, 1);
    neg = bf_get(&sr, 2, 1);
    overflow = bf_get(&sr, 3, 1);

    printf("toggled: C=%d Z=%d N=%d V=%d\n", carry, zero, neg, overflow);
}

void color_packing_demo() {
    BitFields color;
    int r;
    int g;
    int b;
    int a;

    color.data = 0;
    bf_set(&color, 0, 8, 255);
    bf_set(&color, 8, 8, 128);
    bf_set(&color, 16, 8, 64);
    bf_set(&color, 24, 4, 15);

    r = bf_get(&color, 0, 8);
    g = bf_get(&color, 8, 8);
    b = bf_get(&color, 16, 8);
    a = bf_get(&color, 24, 4);

    printf("color: r=%d g=%d b=%d a=%d\n", r, g, b, a);
}

void ip_address_demo() {
    BitFields ip;
    int b0;
    int b1;
    int b2;
    int b3;

    ip.data = 0;
    bf_set(&ip, 24, 8, 192);
    bf_set(&ip, 16, 8, 168);
    bf_set(&ip, 8, 8, 1);
    bf_set(&ip, 0, 8, 100);

    b3 = bf_get(&ip, 24, 8);
    b2 = bf_get(&ip, 16, 8);
    b1 = bf_get(&ip, 8, 8);
    b0 = bf_get(&ip, 0, 8);

    printf("ip: %d.%d.%d.%d\n", b3, b2, b1, b0);
}

int main() {
    BitFields bf;
    int i;

    printf("=== Basic BitField Ops ===\n");
    // EXPECT: === Basic BitField Ops ===

    bf.data = 0;
    bf_set(&bf, 0, 4, 9);
    printf("set [0:4]=9 -> data=%d\n", bf.data);
    // EXPECT: set [0:4]=9 -> data=9

    bf_set(&bf, 4, 4, 5);
    printf("set [4:4]=5 -> data=%d\n", bf.data);
    // EXPECT: set [4:4]=5 -> data=89

    printf("get [0:4]=%d\n", bf_get(&bf, 0, 4));
    // EXPECT: get [0:4]=9
    printf("get [4:4]=%d\n", bf_get(&bf, 4, 4));
    // EXPECT: get [4:4]=5

    bf_clear(&bf, 0, 4);
    printf("clear [0:4] -> data=%d\n", bf.data);
    // EXPECT: clear [0:4] -> data=80

    bf_toggle(&bf, 4, 4);
    printf("toggle [4:4] -> data=%d get=%d\n", bf.data, bf_get(&bf, 4, 4));
    // EXPECT: toggle [4:4] -> data=160 get=10

    printf("=== Test Any/All ===\n");
    // EXPECT: === Test Any/All ===

    bf.data = 0;
    printf("any [0:4] = %d\n", bf_test_any(&bf, 0, 4));
    // EXPECT: any [0:4] = 0
    printf("all [0:4] = %d\n", bf_test_all(&bf, 0, 4));
    // EXPECT: all [0:4] = 0

    bf_set(&bf, 0, 4, 5);
    printf("any [0:4] after set 5 = %d\n", bf_test_any(&bf, 0, 4));
    // EXPECT: any [0:4] after set 5 = 1
    printf("all [0:4] after set 5 = %d\n", bf_test_all(&bf, 0, 4));
    // EXPECT: all [0:4] after set 5 = 0

    bf_set(&bf, 0, 4, 15);
    printf("all [0:4] after set 15 = %d\n", bf_test_all(&bf, 0, 4));
    // EXPECT: all [0:4] after set 15 = 1

    printf("=== Count Set ===\n");
    // EXPECT: === Count Set ===

    bf.data = 170;
    printf("count [0:8] of 170 = %d\n", bf_count_set(&bf, 0, 8));
    // EXPECT: count [0:8] of 170 = 4

    bf.data = 255;
    printf("count [0:8] of 255 = %d\n", bf_count_set(&bf, 0, 8));
    // EXPECT: count [0:8] of 255 = 8

    bf.data = 240;
    printf("count [0:4] of 240 = %d\n", bf_count_set(&bf, 0, 4));
    // EXPECT: count [0:4] of 240 = 0
    printf("count [4:4] of 240 = %d\n", bf_count_set(&bf, 4, 4));
    // EXPECT: count [4:4] of 240 = 4

    printf("=== Permission Demo ===\n");
    // EXPECT: === Permission Demo ===

    bf.data = 0;
    bf_set(&bf, 2, 1, 1);
    bf_set(&bf, 0, 1, 1);
    permission_demo(&bf);
    // EXPECT: perms: r=1 w=0 x=1

    printf("=== Status Register Demo ===\n");
    // EXPECT: === Status Register Demo ===
    status_register_demo();
    // EXPECT: status: C=1 Z=0 N=1 V=0
    // EXPECT: toggled: C=0 Z=1 N=0 V=1

    printf("=== Color Packing Demo ===\n");
    // EXPECT: === Color Packing Demo ===
    color_packing_demo();
    // EXPECT: color: r=255 g=128 b=64 a=15

    printf("=== IP Address Demo ===\n");
    // EXPECT: === IP Address Demo ===
    ip_address_demo();
    // EXPECT: ip: 192.168.1.100

    printf("=== Multi-field Packing ===\n");
    // EXPECT: === Multi-field Packing ===

    bf.data = 0;
    bf_set(&bf, 0, 5, 31);
    bf_set(&bf, 5, 6, 63);
    bf_set(&bf, 11, 5, 31);

    printf("day=%d month=%d year_off=%d\n", bf_get(&bf, 0, 5), bf_get(&bf, 5, 6), bf_get(&bf, 11, 5));
    // EXPECT: day=31 month=63 year_off=31

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
