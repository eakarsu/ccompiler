int printf(const char *fmt, ...);

/* Hamming code encode/decode with error correction */
/* Hamming(7,4): 4 data bits, 3 parity bits */

int hamming_encode(int data) {
    /* data is 4 bits: d3 d2 d1 d0 (bits 3,2,1,0) */
    int d0 = data & 1;
    int d1 = (data >> 1) & 1;
    int d2 = (data >> 2) & 1;
    int d3 = (data >> 3) & 1;

    /* Hamming(7,4) layout: p1 p2 d3 p4 d2 d1 d0 */
    /* bit positions:        6  5  4  3  2  1  0  */
    /* Hamming positions:    1  2  3  4  5  6  7  */
    int p1 = d3 ^ d2 ^ d0;    /* covers Hamming pos 1,3,5,7 */
    int p2 = d3 ^ d1 ^ d0;    /* covers Hamming pos 2,3,6,7 */
    int p4 = d2 ^ d1 ^ d0;    /* covers Hamming pos 4,5,6,7 */

    int code = (p1 << 6) | (p2 << 5) | (d3 << 4) | (p4 << 3) | (d2 << 2) | (d1 << 1) | d0;
    return code;
}

int hamming_decode(int code) {
    /* Extract bits: bit6=p1, bit5=p2, bit4=d3, bit3=p4, bit2=d2, bit1=d1, bit0=d0 */
    int b1 = (code >> 6) & 1;  /* p1 */
    int b2 = (code >> 5) & 1;  /* p2 */
    int b3 = (code >> 4) & 1;  /* d3 */
    int b4 = (code >> 3) & 1;  /* p4 */
    int b5 = (code >> 2) & 1;  /* d2 */
    int b6 = (code >> 1) & 1;  /* d1 */
    int b7 = code & 1;         /* d0 */

    /* Compute syndrome */
    int s1 = b1 ^ b3 ^ b5 ^ b7;
    int s2 = b2 ^ b3 ^ b6 ^ b7;
    int s4 = b4 ^ b5 ^ b6 ^ b7;

    int syndrome = (s4 << 2) | (s2 << 1) | s1;

    /* Correct error if syndrome != 0 */
    /* syndrome tells us which Hamming position (1-7) has an error */
    /* Hamming pos 1=bit6, pos 2=bit5, pos 3=bit4, pos 4=bit3, pos 5=bit2, pos 6=bit1, pos 7=bit0 */
    if (syndrome != 0) {
        int errbit = 7 - syndrome;
        code = code ^ (1 << errbit);
    }

    /* Extract corrected data bits */
    int d3c = (code >> 4) & 1;
    int d2c = (code >> 2) & 1;
    int d1c = (code >> 1) & 1;
    int d0c = code & 1;

    return (d3c << 3) | (d2c << 2) | (d1c << 1) | d0c;
}

int count_ones(int val, int bits) {
    int count = 0;
    int i;
    for (i = 0; i < bits; i++) {
        if (val & (1 << i)) count = count + 1;
    }
    return count;
}

int hamming_distance(int a, int b, int bits) {
    return count_ones(a ^ b, bits);
}

int main(void) {
    /* Encode 0: all zeros -> p1=p2=p4=0, code=0 */
    int enc0 = hamming_encode(0);
    printf("encode 0: %d\n", enc0);
    // EXPECT: encode 0: 0

    /* Encode 1: d3=0,d2=0,d1=0,d0=1 */
    /* p1=0^0^1=1, p2=0^0^1=1, p4=0^0^1=1 */
    /* code = 1<<6|1<<5|0<<4|1<<3|0<<2|0<<1|1 = 64+32+8+1 = 105 */
    int enc1 = hamming_encode(1);
    printf("encode 1: %d\n", enc1);
    // EXPECT: encode 1: 105

    /* Encode 5: d3=0,d2=1,d1=0,d0=1 */
    /* p1=0^1^1=0, p2=0^0^1=1, p4=1^0^1=0 */
    /* code = 0<<6|1<<5|0<<4|0<<3|1<<2|0<<1|1 = 32+4+1 = 37 */
    int enc5 = hamming_encode(5);
    printf("encode 5: %d\n", enc5);
    // EXPECT: encode 5: 37

    /* Encode 15: d3=1,d2=1,d1=1,d0=1 */
    /* p1=1^1^1=1, p2=1^1^1=1, p4=1^1^1=1 */
    /* code = 1<<6|1<<5|1<<4|1<<3|1<<2|1<<1|1 = 127 */
    int enc15 = hamming_encode(15);
    printf("encode 15: %d\n", enc15);
    // EXPECT: encode 15: 127

    /* Decode without errors */
    printf("decode 0: %d\n", hamming_decode(enc0));
    // EXPECT: decode 0: 0
    printf("decode 1: %d\n", hamming_decode(enc1));
    // EXPECT: decode 1: 1
    printf("decode 5: %d\n", hamming_decode(enc5));
    // EXPECT: decode 5: 5
    printf("decode 15: %d\n", hamming_decode(enc15));
    // EXPECT: decode 15: 15

    /* Introduce single-bit errors and correct them */
    /* Flip bit 0 of enc5 (37 -> 36) */
    int err5a = enc5 ^ 1;
    printf("corrupted 5a: %d\n", err5a);
    // EXPECT: corrupted 5a: 36
    printf("corrected 5a: %d\n", hamming_decode(err5a));
    // EXPECT: corrected 5a: 5

    /* Flip bit 4 of enc1 (105 = 1101001 -> 1111001 = 121) */
    int err1a = enc1 ^ (1 << 4);
    printf("corrupted 1a: %d\n", err1a);
    // EXPECT: corrupted 1a: 121
    printf("corrected 1a: %d\n", hamming_decode(err1a));
    // EXPECT: corrected 1a: 1

    /* Flip bit 6 (MSB) of enc15 (127 -> 63) */
    int err15a = enc15 ^ (1 << 6);
    printf("corrupted 15a: %d\n", err15a);
    // EXPECT: corrupted 15a: 63
    printf("corrected 15a: %d\n", hamming_decode(err15a));
    // EXPECT: corrected 15a: 15

    /* Hamming distance tests */
    printf("hamming dist 0,127: %d\n", hamming_distance(0, 127, 7));
    // EXPECT: hamming dist 0,127: 7

    printf("hamming dist 37,36: %d\n", hamming_distance(37, 36, 7));
    // EXPECT: hamming dist 37,36: 1

    /* Min distance of Hamming(7,4) code is 3 */
    int min_dist = 100;
    int i;
    int j;
    for (i = 0; i < 16; i++) {
        for (j = i + 1; j < 16; j++) {
            int d = hamming_distance(hamming_encode(i), hamming_encode(j), 7);
            if (d < min_dist) min_dist = d;
        }
    }
    printf("min distance: %d\n", min_dist);
    // EXPECT: min distance: 3

    /* Encode/decode all 16 values roundtrip */
    int all_ok = 1;
    for (i = 0; i < 16; i++) {
        int encoded = hamming_encode(i);
        int decoded = hamming_decode(encoded);
        if (decoded != i) all_ok = 0;
    }
    printf("all roundtrip ok: %d\n", all_ok);
    // EXPECT: all roundtrip ok: 1

    /* Single bit error correction for all 16 values, all 7 bit positions */
    int all_corrected = 1;
    for (i = 0; i < 16; i++) {
        int encoded = hamming_encode(i);
        for (j = 0; j < 7; j++) {
            int corrupted = encoded ^ (1 << j);
            int corrected = hamming_decode(corrupted);
            if (corrected != i) all_corrected = 0;
        }
    }
    printf("all single-bit corrected: %d\n", all_corrected);
    // EXPECT: all single-bit corrected: 1

    /* Count ones test */
    printf("ones in 0: %d\n", count_ones(0, 8));
    // EXPECT: ones in 0: 0
    printf("ones in 255: %d\n", count_ones(255, 8));
    // EXPECT: ones in 255: 8
    printf("ones in 170: %d\n", count_ones(170, 8));
    // EXPECT: ones in 170: 4

    printf("done\n");
    // EXPECT: done
    return 0;
}
