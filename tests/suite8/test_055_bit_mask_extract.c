int printf(const char *fmt, ...);

int create_mask(int width) {
    if (width >= 32) return -1;
    if (width <= 0) return 0;
    return (1 << width) - 1;
}

int extract_bits(int val, int start, int width) {
    return (val >> start) & create_mask(width);
}

int insert_bits(int dest, int src, int start, int width) {
    int mask;
    mask = create_mask(width) << start;
    dest = dest & (~mask);
    dest = dest | ((src << start) & mask);
    return dest;
}

int set_bit(int val, int pos) {
    return val | (1 << pos);
}

int clear_bit(int val, int pos) {
    return val & (~(1 << pos));
}

int toggle_bit(int val, int pos) {
    return val ^ (1 << pos);
}

int test_bit(int val, int pos) {
    return (val >> pos) & 1;
}

int set_bits_range(int val, int lo, int hi) {
    int i;
    for (i = lo; i <= hi; i = i + 1) {
        val = set_bit(val, i);
    }
    return val;
}

int clear_bits_range(int val, int lo, int hi) {
    int i;
    for (i = lo; i <= hi; i = i + 1) {
        val = clear_bit(val, i);
    }
    return val;
}

int count_set_in_range(int val, int lo, int hi) {
    int count;
    int i;
    count = 0;
    for (i = lo; i <= hi; i = i + 1) {
        count = count + test_bit(val, i);
    }
    return count;
}

int pack_rgb(int r, int g, int b) {
    return ((r & 255) << 16) | ((g & 255) << 8) | (b & 255);
}

void unpack_rgb(int packed, int *r, int *g, int *b) {
    *r = (packed >> 16) & 255;
    *g = (packed >> 8) & 255;
    *b = packed & 255;
}

int pack_four_bytes(int a, int b, int c, int d) {
    return ((a & 255) << 24) | ((b & 255) << 16) | ((c & 255) << 8) | (d & 255);
}

int main() {
    int val;
    int packed;
    int r;
    int g;
    int b;
    int i;

    printf("=== Create Mask ===\n");
    // EXPECT: === Create Mask ===

    printf("mask(%d) = %d\n", 1, create_mask(1));
    // EXPECT: mask(1) = 1
    printf("mask(%d) = %d\n", 4, create_mask(4));
    // EXPECT: mask(4) = 15
    printf("mask(%d) = %d\n", 8, create_mask(8));
    // EXPECT: mask(8) = 255
    printf("mask(%d) = %d\n", 16, create_mask(16));
    // EXPECT: mask(16) = 65535

    printf("=== Extract Bits ===\n");
    // EXPECT: === Extract Bits ===

    val = 305419896;
    printf("extract(%d, 0, 8) = %d\n", val, extract_bits(val, 0, 8));
    // EXPECT: extract(305419896, 0, 8) = 120
    printf("extract(%d, 8, 8) = %d\n", val, extract_bits(val, 8, 8));
    // EXPECT: extract(305419896, 8, 8) = 86
    printf("extract(%d, 16, 8) = %d\n", val, extract_bits(val, 16, 8));
    // EXPECT: extract(305419896, 16, 8) = 52
    printf("extract(%d, 24, 8) = %d\n", val, extract_bits(val, 24, 8));
    // EXPECT: extract(305419896, 24, 8) = 18

    printf("=== Insert Bits ===\n");
    // EXPECT: === Insert Bits ===

    printf("insert(0, 255, 0, 8) = %d\n", insert_bits(0, 255, 0, 8));
    // EXPECT: insert(0, 255, 0, 8) = 255
    printf("insert(0, 255, 8, 8) = %d\n", insert_bits(0, 255, 8, 8));
    // EXPECT: insert(0, 255, 8, 8) = 65280
    printf("insert(255, 0, 0, 8) = %d\n", insert_bits(255, 0, 0, 8));
    // EXPECT: insert(255, 0, 0, 8) = 0

    printf("=== Set/Clear/Toggle/Test ===\n");
    // EXPECT: === Set/Clear/Toggle/Test ===

    val = 0;
    val = set_bit(val, 0);
    val = set_bit(val, 3);
    val = set_bit(val, 7);
    printf("set bits 0,3,7: %d\n", val);
    // EXPECT: set bits 0,3,7: 137

    printf("test bit 0: %d\n", test_bit(val, 0));
    // EXPECT: test bit 0: 1
    printf("test bit 1: %d\n", test_bit(val, 1));
    // EXPECT: test bit 1: 0
    printf("test bit 3: %d\n", test_bit(val, 3));
    // EXPECT: test bit 3: 1

    val = clear_bit(val, 3);
    printf("clear bit 3: %d\n", val);
    // EXPECT: clear bit 3: 129

    val = toggle_bit(val, 7);
    printf("toggle bit 7: %d\n", val);
    // EXPECT: toggle bit 7: 1

    val = toggle_bit(val, 7);
    printf("toggle bit 7 again: %d\n", val);
    // EXPECT: toggle bit 7 again: 129

    printf("=== Range Operations ===\n");
    // EXPECT: === Range Operations ===

    printf("set_range(0, 2, 5) = %d\n", set_bits_range(0, 2, 5));
    // EXPECT: set_range(0, 2, 5) = 60
    printf("clear_range(255, 2, 5) = %d\n", clear_bits_range(255, 2, 5));
    // EXPECT: clear_range(255, 2, 5) = 195
    printf("count_in_range(255, 0, 7) = %d\n", count_set_in_range(255, 0, 7));
    // EXPECT: count_in_range(255, 0, 7) = 8
    printf("count_in_range(170, 0, 7) = %d\n", count_set_in_range(170, 0, 7));
    // EXPECT: count_in_range(170, 0, 7) = 4

    printf("=== RGB Pack/Unpack ===\n");
    // EXPECT: === RGB Pack/Unpack ===

    packed = pack_rgb(255, 128, 64);
    printf("packed rgb(255,128,64) = %d\n", packed);
    // EXPECT: packed rgb(255,128,64) = 16744512
    unpack_rgb(packed, &r, &g, &b);
    printf("unpacked: r=%d g=%d b=%d\n", r, g, b);
    // EXPECT: unpacked: r=255 g=128 b=64

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
