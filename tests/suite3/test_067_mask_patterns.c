int printf(const char *fmt, ...);

/* Bitmask creation, field extraction/insertion/clearing, bit range operations */

/* Create a mask of n consecutive 1-bits starting at position pos */
unsigned int make_mask(int pos, int width) {
    /* mask = ((1 << width) - 1) << pos */
    unsigned int ones = (1u << width) - 1u;
    return ones << pos;
}

/* Extract a bit-field: bits [pos .. pos+width-1] */
unsigned int extract_field(unsigned int val, int pos, int width) {
    return (val >> pos) & ((1u << width) - 1u);
}

/* Insert a bit-field: put 'field' value into val at bits [pos..pos+width-1] */
unsigned int insert_field(unsigned int val, int pos, int width, unsigned int field) {
    unsigned int mask = ((1u << width) - 1u) << pos;
    return (val & ~mask) | ((field << pos) & mask);
}

/* Clear a bit-field */
unsigned int clear_field(unsigned int val, int pos, int width) {
    unsigned int mask = ((1u << width) - 1u) << pos;
    return val & ~mask;
}

/* Swap bits at positions i and j */
unsigned int swap_bits(unsigned int val, int i, int j) {
    /* XOR swap: if bits differ, flip both */
    unsigned int xi = (val >> i) & 1u;
    unsigned int xj = (val >> j) & 1u;
    if (xi != xj) {
        val = val ^ ((1u << i) | (1u << j));
    }
    return val;
}

/* Swap two bit-ranges of equal width */
/* Swap bits [l1..l1+w-1] with [l2..l2+w-1] */
unsigned int swap_ranges(unsigned int val, int l1, int l2, int w) {
    unsigned int f1 = extract_field(val, l1, w);
    unsigned int f2 = extract_field(val, l2, w);
    val = insert_field(val, l1, w, f2);
    val = insert_field(val, l2, w, f1);
    return val;
}

int main(void) {
    /* --- MASK CREATION --- */
    printf("%u\n", make_mask(0, 1));       // EXPECT: 1
    printf("%u\n", make_mask(1, 1));       // EXPECT: 2
    printf("%u\n", make_mask(0, 4));       // EXPECT: 15
    /* (1<<4)-1 = 15 */
    printf("%u\n", make_mask(4, 4));       // EXPECT: 240
    /* 0b11110000 = 240 */
    printf("%u\n", make_mask(0, 8));       // EXPECT: 255
    printf("%u\n", make_mask(8, 8));       // EXPECT: 65280
    /* 0xFF00 = 65280 */
    printf("%u\n", make_mask(0, 16));      // EXPECT: 65535
    printf("%u\n", make_mask(16, 8));      // EXPECT: 16711680
    /* 0xFF0000 = 16711680 */

    /* --- FIELD EXTRACTION --- */
    unsigned int val = 0xABCDEF12u;
    /* Extract low nibble: bits[3:0] = 2 */
    printf("%u\n", extract_field(val, 0, 4));   // EXPECT: 2
    /* Extract next nibble: bits[7:4] = 1 */
    printf("%u\n", extract_field(val, 4, 4));   // EXPECT: 1
    /* Extract byte 1: bits[15:8] = 0xEF = 239 */
    printf("%u\n", extract_field(val, 8, 8));   // EXPECT: 239
    /* Extract byte 2: bits[23:16] = 0xCD = 205 */
    printf("%u\n", extract_field(val, 16, 8));  // EXPECT: 205
    /* Extract top byte: bits[31:24] = 0xAB = 171 */
    printf("%u\n", extract_field(val, 24, 8));  // EXPECT: 171
    /* Extract 3-bit field at position 5: bits[7:5] of 0x12 = 0b001 */
    /* val & 0xFF = 0x12 = 0b00010010; bits[7:5] = 0b000 = 0 */
    printf("%u\n", extract_field(val, 5, 3));   // EXPECT: 0
    /* 0x12 = 0001 0010, bits 5,6,7 = 000 */

    /* --- FIELD INSERTION --- */
    unsigned int base = 0u;
    base = insert_field(base, 0, 8, 0xAAu);
    printf("%u\n", base);                        // EXPECT: 170
    /* 0xAA = 170 */
    base = insert_field(base, 8, 8, 0xBBu);
    printf("%u\n", base);                        // EXPECT: 48042
    /* 0xBBAA = 187*256 + 170 = 47872 + 170 = 48042 */
    base = insert_field(base, 16, 8, 0xCCu);
    printf("%u\n", base);                        // EXPECT: 13417386
    /* 0xCCBBAA: 0xCC=204, 0xBB=187, 0xAA=170 */
    /* 204*65536 + 187*256 + 170 = 13369344 + 47872 + 170 = 13417386 */

    /* --- FIELD CLEARING --- */
    unsigned int v2 = 0xFFFFFFFFu;
    v2 = clear_field(v2, 0, 8);
    printf("%u\n", v2);                          // EXPECT: 4294967040
    /* 0xFFFFFF00 = 4294967040 */
    v2 = clear_field(v2, 24, 8);
    printf("%u\n", v2);                          // EXPECT: 16776960
    /* 0x00FFFF00 = 16776960 */

    unsigned int v3 = 0x12345678u;
    printf("%u\n", clear_field(v3, 4, 8));       // EXPECT: 305418248
    /* clear bits[11:4]: mask = 0xFF0 = 4080; ~mask = 0xFFFFF00F */
    /* 0x12345678 & 0xFFFFF00F = 0x12345008 = 305418248 */

    /* --- SWAP BITS --- */
    unsigned int sv = 0b10110100u;  /* 180 */
    /* 0b10110100: bit0=0,bit1=0,bit2=1,bit3=0,bit4=1,bit5=1,bit6=0,bit7=1 */
    /* swap bit2(=1) and bit7(=1): same value, no change */
    sv = swap_bits(sv, 2, 7);
    printf("%u\n", sv);                          // EXPECT: 180
    /* bits 2 and 7 are both 1, so no change, still 180 */

    /* --- SWAP BIT RANGES --- */
    unsigned int sv2 = 0b11001010u;   /* 202 */
    /* swap nibble at bits[3:0] with nibble at bits[7:4] */
    sv2 = swap_ranges(sv2, 0, 4, 4);
    printf("%u\n", sv2);                         // EXPECT: 172
    /* low nibble = 1010 = 10, high nibble = 1100 = 12 */
    /* after swap: low=1100=12, high=1010=10 => 0b10101100 = 172 */

    unsigned int sv3 = 0x1234u;  /* 4660: byte0=0x34=52, byte1=0x12=18 */
    /* swap byte 0 with byte 1 */
    sv3 = swap_ranges(sv3, 0, 8, 8);
    printf("%u\n", sv3);                         // EXPECT: 13330
    /* After swap: byte0=0x12=18, byte1=0x34=52 => 0x3412 = 13330 */

    /* --- COUNTING BITS IN A MASK --- */
    /* Use popcount manually inline */
    unsigned int m1 = make_mask(0, 5);    /* 0b11111 = 31 */
    int cnt = 0;
    unsigned int tmp = m1;
    while (tmp) { tmp = tmp & (tmp - 1); cnt++; }
    printf("%d\n", cnt);                         // EXPECT: 5

    unsigned int m2 = make_mask(4, 12);   /* 12 bits starting at 4 */
    cnt = 0; tmp = m2;
    while (tmp) { tmp = tmp & (tmp-1); cnt++; }
    printf("%d\n", cnt);                         // EXPECT: 12

    /* --- COMBINING MASKS --- */
    unsigned int combined = make_mask(0, 4) | make_mask(8, 4);
    printf("%u\n", combined);                    // EXPECT: 3855
    /* 0x00F | 0xF00 = 0xF0F = 3855 */

    unsigned int exclusive = make_mask(0, 8) ^ make_mask(4, 8);
    printf("%u\n", exclusive);                   // EXPECT: 3855
    /* 0x00FF ^ 0x0FF0 = 0x0F0F = 3855 */

    return 0;
}
