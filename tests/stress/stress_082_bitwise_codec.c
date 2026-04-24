// stress_082_bitwise_codec.c - Bitwise codec: pack/unpack, bit reversal, gray code
// EXPECT: pack4_result=2017801234
// EXPECT: unpack4=120 69 52 18
// EXPECT: roundtrip4=1
// EXPECT: pack_struct=65793
// EXPECT: unpack_struct=1 1 1 1
// EXPECT: arr_pack_0=132353
// EXPECT: arr_pack_9=457271414
// EXPECT: arr_unpack_all_ok=1
// EXPECT: rev8_0=0
// EXPECT: rev8_1=128
// EXPECT: rev8_255=255
// EXPECT: rev8_170=85
// EXPECT: rev8_85=170
// EXPECT: rev32_1=2147483648
// EXPECT: gray_0=0
// EXPECT: gray_1=1
// EXPECT: gray_2=3
// EXPECT: gray_3=2
// EXPECT: gray_15=8
// EXPECT: ungray_roundtrip=1
// EXPECT: gray_sequence=0 1 3 2 6 7 5 4
// EXPECT: pack16_result=4660
// EXPECT: unpack16=1 2 3 4
// EXPECT: rotr8_ok=1
// EXPECT: popcount_0=0
// EXPECT: popcount_1=1
// EXPECT: popcount_255=8
// EXPECT: popcount_neg1=32
// EXPECT: leading_zeros_1=31
// EXPECT: leading_zeros_128=24
// EXPECT: pass_done=1

int printf(const char *fmt, ...);

// Pack four 8-bit values into a 32-bit int (a=MSB, d=LSB)
unsigned int pack4(int a, int b, int c, int d) {
    return ((a & 0xFF) << 24) | ((b & 0xFF) << 16) | ((c & 0xFF) << 8) | (d & 0xFF);
}

void unpack4(unsigned int packed, int *a, int *b, int *c, int *d) {
    *a = (packed >> 24) & 0xFF;
    *b = (packed >> 16) & 0xFF;
    *c = (packed >> 8) & 0xFF;
    *d = packed & 0xFF;
}

// Pack struct fields manually into bits
struct Fields {
    int flag_a;  // 1 bit
    int flag_b;  // 1 bit
    int flag_c;  // 1 bit
    int flag_d;  // 1 bit
    int value;   // 8 bits
    int count;   // 16 bits
};

unsigned int pack_struct(struct Fields f) {
    unsigned int result = 0;
    result |= (f.flag_a & 1) << 0;
    result |= (f.flag_b & 1) << 1;
    result |= (f.flag_c & 1) << 2;
    result |= (f.flag_d & 1) << 3;
    result |= (f.value & 0xFF) << 8;
    result |= (f.count & 0xFFFF) << 16;
    return result;
}

struct Fields unpack_struct(unsigned int packed) {
    struct Fields f;
    f.flag_a = (packed >> 0) & 1;
    f.flag_b = (packed >> 1) & 1;
    f.flag_c = (packed >> 2) & 1;
    f.flag_d = (packed >> 3) & 1;
    f.value = (packed >> 8) & 0xFF;
    f.count = (packed >> 16) & 0xFFFF;
    return f;
}

// Reverse bits in an 8-bit value
int reverse_bits8(int val) {
    int result = 0;
    int i;
    for (i = 0; i < 8; i++) {
        result |= ((val >> i) & 1) << (7 - i);
    }
    return result;
}

// Reverse bits in a 32-bit value
unsigned int reverse_bits32(unsigned int val) {
    unsigned int result = 0;
    int i;
    for (i = 0; i < 32; i++) {
        result |= ((val >> i) & 1) << (31 - i);
    }
    return result;
}

// Gray code encode
unsigned int to_gray(unsigned int n) {
    return n ^ (n >> 1);
}

// Gray code decode
unsigned int from_gray(unsigned int gray) {
    unsigned int n = gray;
    unsigned int mask = gray >> 1;
    while (mask != 0) {
        n = n ^ mask;
        mask = mask >> 1;
    }
    return n;
}

// Pack four 4-bit values into a 16-bit value
int pack16(int a, int b, int c, int d) {
    return ((a & 0xF) << 12) | ((b & 0xF) << 8) | ((c & 0xF) << 4) | (d & 0xF);
}

void unpack16(int packed, int *a, int *b, int *c, int *d) {
    *a = (packed >> 12) & 0xF;
    *b = (packed >> 8) & 0xF;
    *c = (packed >> 4) & 0xF;
    *d = packed & 0xF;
}

// Rotate right 8-bit
int rotr8(int val, int n) {
    val = val & 0xFF;
    n = n & 7;
    return ((val >> n) | (val << (8 - n))) & 0xFF;
}

// Population count (Kernighan's method)
int popcount(unsigned int x) {
    int count = 0;
    while (x) {
        x = x & (x - 1);
        count++;
    }
    return count;
}

// Count leading zeros
int clz(unsigned int x) {
    if (x == 0) return 32;
    int n = 0;
    while ((x & 0x80000000) == 0) {
        x = x << 1;
        n++;
    }
    return n;
}

int main(void) {
    int a, b, c, d;
    int i;

    // Test pack4/unpack4
    unsigned int packed = pack4(120, 69, 52, 18);
    printf("pack4_result=%u\n", packed);

    unpack4(packed, &a, &b, &c, &d);
    printf("unpack4=%d %d %d %d\n", a, b, c, d);

    // Round-trip test
    int ok = 1;
    for (i = 0; i < 256; i += 17) {
        unsigned int p = pack4(i, 255 - i, i / 2, i % 100);
        int va, vb, vc, vd;
        unpack4(p, &va, &vb, &vc, &vd);
        if (va != i || vb != 255 - i || vc != i / 2 || vd != i % 100) ok = 0;
    }
    printf("roundtrip4=%d\n", ok);

    // Pack struct
    struct Fields f;
    f.flag_a = 1; f.flag_b = 0; f.flag_c = 0; f.flag_d = 0;
    f.value = 1; f.count = 1;
    unsigned int sp = pack_struct(f);
    printf("pack_struct=%u\n", sp);

    struct Fields f2 = unpack_struct(sp);
    printf("unpack_struct=%d %d %d %d\n", f2.flag_a, f2.value, f2.count, f2.flag_a);

    // Array pack/unpack
    int arr_a[10], arr_b[10], arr_c[10], arr_d[10];
    unsigned int arr_packed[10];
    for (i = 0; i < 10; i++) {
        arr_a[i] = (i * 3) & 0xFF;
        arr_b[i] = (i * 7 + 2) & 0xFF;
        arr_c[i] = (i * 11 + 5) & 0xFF;
        arr_d[i] = (i * 13 + 1) & 0xFF;
        arr_packed[i] = pack4(arr_a[i], arr_b[i], arr_c[i], arr_d[i]);
    }
    printf("arr_pack_0=%u\n", arr_packed[0]);
    printf("arr_pack_9=%u\n", arr_packed[9]);

    ok = 1;
    for (i = 0; i < 10; i++) {
        int ra, rb, rc, rd;
        unpack4(arr_packed[i], &ra, &rb, &rc, &rd);
        if (ra != arr_a[i] || rb != arr_b[i] || rc != arr_c[i] || rd != arr_d[i]) ok = 0;
    }
    printf("arr_unpack_all_ok=%d\n", ok);

    // Bit reversal
    printf("rev8_0=%d\n", reverse_bits8(0));
    printf("rev8_1=%d\n", reverse_bits8(1));
    printf("rev8_255=%d\n", reverse_bits8(255));
    printf("rev8_170=%d\n", reverse_bits8(170));
    printf("rev8_85=%d\n", reverse_bits8(85));

    printf("rev32_1=%u\n", reverse_bits32(1));

    // Gray code
    printf("gray_0=%u\n", to_gray(0));
    printf("gray_1=%u\n", to_gray(1));
    printf("gray_2=%u\n", to_gray(2));
    printf("gray_3=%u\n", to_gray(3));
    printf("gray_15=%u\n", to_gray(15));

    // Round-trip gray code
    ok = 1;
    for (i = 0; i < 1000; i++) {
        unsigned int ui = (unsigned int)i;
        if (from_gray(to_gray(ui)) != ui) ok = 0;
    }
    printf("ungray_roundtrip=%d\n", ok);

    // Gray sequence
    printf("gray_sequence=");
    for (i = 0; i < 8; i++) {
        if (i > 0) printf(" ");
        printf("%u", to_gray((unsigned int)i));
    }
    printf("\n");

    // Pack16
    int p16 = pack16(1, 2, 3, 4);
    printf("pack16_result=%d\n", p16);
    int pa, pb, pc, pd;
    unpack16(p16, &pa, &pb, &pc, &pd);
    printf("unpack16=%d %d %d %d\n", pa, pb, pc, pd);

    // Rotate right
    ok = 1;
    // rotr8(0b10000001, 1) = 0b11000000 = 192
    if (rotr8(0x81, 1) != 0xC0) ok = 0;
    // rotr8(val, 0) = val
    if (rotr8(0xAB, 0) != 0xAB) ok = 0;
    // rotr8(val, 8) = val
    if (rotr8(0x55, 8) != 0x55) ok = 0;
    printf("rotr8_ok=%d\n", ok);

    // Popcount
    printf("popcount_0=%d\n", popcount(0));
    printf("popcount_1=%d\n", popcount(1));
    printf("popcount_255=%d\n", popcount(255));
    printf("popcount_neg1=%d\n", popcount(0xFFFFFFFF));

    // Leading zeros
    printf("leading_zeros_1=%d\n", clz(1));
    printf("leading_zeros_128=%d\n", clz(128));

    printf("pass_done=1\n");
    return 0;
}
