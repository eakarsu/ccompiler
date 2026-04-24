int printf(const char *fmt, ...);

/* Union for type punning: int as 4 bytes */
union IntBytes {
    int val;
    char bytes[4];
};

/* Extract individual bytes from an int */
int get_byte(int val, int index) {
    union IntBytes u;
    u.val = val;
    return (int)u.bytes[index] & 255;
}

/* Pack 4 bytes into an int */
int pack_bytes(int b0, int b1, int b2, int b3) {
    union IntBytes u;
    u.bytes[0] = (char)b0;
    u.bytes[1] = (char)b1;
    u.bytes[2] = (char)b2;
    u.bytes[3] = (char)b3;
    return u.val;
}

/* Set one byte in an int */
int set_byte(int val, int index, int byte_val) {
    union IntBytes u;
    u.val = val;
    u.bytes[index] = (char)byte_val;
    return u.val;
}

/* Swap bytes 0 and 3 */
int swap_outer_bytes(int val) {
    union IntBytes u;
    u.val = val;
    char temp = u.bytes[0];
    u.bytes[0] = u.bytes[3];
    u.bytes[3] = temp;
    return u.val;
}

/* Count non-zero bytes */
int count_nonzero_bytes(int val) {
    union IntBytes u;
    u.val = val;
    int count = 0;
    int i;
    for (i = 0; i < 4; i++) {
        if (u.bytes[i] != 0) count++;
    }
    return count;
}

/* Check if all bytes are the same */
int all_bytes_same(int val) {
    union IntBytes u;
    u.val = val;
    int i;
    for (i = 1; i < 4; i++) {
        if (u.bytes[i] != u.bytes[0]) return 0;
    }
    return 1;
}

/* Union to reinterpret two shorts as one int */
union TwoShorts {
    int whole;
    char parts[4];
};

/* XOR all bytes together */
int xor_bytes(int val) {
    union IntBytes u;
    u.val = val;
    int result = 0;
    int i;
    for (i = 0; i < 4; i++) {
        result = result ^ ((int)u.bytes[i] & 255);
    }
    return result;
}

/* Sum all bytes (treated as positive 0-255) */
int sum_bytes(int val) {
    union IntBytes u;
    u.val = val;
    int total = 0;
    int i;
    for (i = 0; i < 4; i++) {
        total += (int)u.bytes[i] & 255;
    }
    return total;
}

int main(void) {
    /* ARM64 is little-endian: least significant byte first */

    /* Test basic byte extraction (value 0x04030201 = 67305985) */
    int val = 67305985;  /* bytes: 01, 02, 03, 04 in little-endian */
    // EXPECT: Byte 0: 1
    printf("Byte 0: %d\n", get_byte(val, 0));
    // EXPECT: Byte 1: 2
    printf("Byte 1: %d\n", get_byte(val, 1));
    // EXPECT: Byte 2: 3
    printf("Byte 2: %d\n", get_byte(val, 2));
    // EXPECT: Byte 3: 4
    printf("Byte 3: %d\n", get_byte(val, 3));

    /* Test pack_bytes */
    int packed = pack_bytes(10, 20, 30, 40);
    // EXPECT: Packed byte0: 10
    printf("Packed byte0: %d\n", get_byte(packed, 0));
    // EXPECT: Packed byte1: 20
    printf("Packed byte1: %d\n", get_byte(packed, 1));
    // EXPECT: Packed byte2: 30
    printf("Packed byte2: %d\n", get_byte(packed, 2));
    // EXPECT: Packed byte3: 40
    printf("Packed byte3: %d\n", get_byte(packed, 3));

    /* Test round-trip: pack and unpack */
    int rt = pack_bytes(get_byte(val, 0), get_byte(val, 1),
                        get_byte(val, 2), get_byte(val, 3));
    // EXPECT: Round-trip match: 1
    printf("Round-trip match: %d\n", rt == val);

    /* Test set_byte */
    int modified = set_byte(0, 1, 42);
    // EXPECT: Set byte1 to 42: 42
    printf("Set byte1 to 42: %d\n", get_byte(modified, 1));
    // EXPECT: Other byte0 still 0: 0
    printf("Other byte0 still 0: %d\n", get_byte(modified, 0));

    /* Test swap_outer_bytes */
    int swapped = swap_outer_bytes(val);
    // EXPECT: Swapped byte0: 4
    printf("Swapped byte0: %d\n", get_byte(swapped, 0));
    // EXPECT: Swapped byte3: 1
    printf("Swapped byte3: %d\n", get_byte(swapped, 3));
    // EXPECT: Middle byte1 unchanged: 2
    printf("Middle byte1 unchanged: %d\n", get_byte(swapped, 1));

    /* Test count_nonzero_bytes */
    // EXPECT: Nonzero in val: 4
    printf("Nonzero in val: %d\n", count_nonzero_bytes(val));
    // EXPECT: Nonzero in 0: 0
    printf("Nonzero in 0: %d\n", count_nonzero_bytes(0));
    // EXPECT: Nonzero in 256: 1
    printf("Nonzero in 256: %d\n", count_nonzero_bytes(256));

    /* Test all_bytes_same */
    int same = pack_bytes(7, 7, 7, 7);
    // EXPECT: All same(7,7,7,7): 1
    printf("All same(7,7,7,7): %d\n", all_bytes_same(same));
    // EXPECT: All same(val): 0
    printf("All same(val): %d\n", all_bytes_same(val));
    // EXPECT: All same(0): 1
    printf("All same(0): %d\n", all_bytes_same(0));

    /* Test xor_bytes */
    /* val bytes: 1,2,3,4 -> 1^2^3^4 = 4 */
    // EXPECT: XOR bytes: 4
    printf("XOR bytes: %d\n", xor_bytes(val));

    /* Test sum_bytes */
    /* val bytes: 1,2,3,4 -> 1+2+3+4 = 10 */
    // EXPECT: Sum bytes: 10
    printf("Sum bytes: %d\n", sum_bytes(val));

    /* Larger values: 0x0A141E28 = 168_636_968 doesn't fit. Use pack */
    int big = pack_bytes(100, 150, 200, 50);
    // EXPECT: Big sum: 500
    printf("Big sum: %d\n", sum_bytes(big));

    /* Test that writing via union and reading back works */
    union IntBytes u;
    u.val = 0;
    u.bytes[0] = 1;
    u.bytes[1] = 0;
    u.bytes[2] = 0;
    u.bytes[3] = 0;
    // EXPECT: Single byte 1: 1
    printf("Single byte 1: %d\n", u.val);

    return 0;
}
