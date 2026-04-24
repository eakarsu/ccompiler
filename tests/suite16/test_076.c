int printf(const char *fmt, ...);

// Test 076: Bit packing - pack multiple small values into byte arrays

// Pack values of N bits each into a byte array (using int array as bytes 0-255)
void pack_bits(int *values, int count, int bits, int *bytes, int *byte_len) {
    int bit_pos = 0;
    int i;

    // Initialize bytes to 0
    for (i = 0; i < 40; i = i + 1) bytes[i] = 0;

    int mask = (1 << bits) - 1;
    for (i = 0; i < count; i = i + 1) {
        int val = values[i] & mask;
        int b;
        for (b = 0; b < bits; b = b + 1) {
            if (val & (1 << b)) {
                int byte_idx = (bit_pos + b) / 8;
                int bit_idx = (bit_pos + b) % 8;
                bytes[byte_idx] = bytes[byte_idx] | (1 << bit_idx);
            }
        }
        bit_pos = bit_pos + bits;
    }
    *byte_len = (bit_pos + 7) / 8;
}

void unpack_bits(int *bytes, int count, int bits, int *values) {
    int bit_pos = 0;
    int mask = (1 << bits) - 1;
    int i;

    for (i = 0; i < count; i = i + 1) {
        int val = 0;
        int b;
        for (b = 0; b < bits; b = b + 1) {
            int byte_idx = (bit_pos + b) / 8;
            int bit_idx = (bit_pos + b) % 8;
            if (bytes[byte_idx] & (1 << bit_idx)) {
                val = val | (1 << b);
            }
        }
        values[i] = val;
        bit_pos = bit_pos + bits;
    }
}

int main() {
    // Pack 8 values using 4 bits each (0-15)
    int vals4[8];
    vals4[0] = 3; vals4[1] = 7; vals4[2] = 15; vals4[3] = 0;
    vals4[4] = 1; vals4[5] = 9; vals4[6] = 12; vals4[7] = 5;

    int bytes[40];
    int blen;
    pack_bits(vals4, 8, 4, bytes, &blen);
    printf("4-bit packed bytes: %d\n", blen); // EXPECT: 4-bit packed bytes: 4

    // Total bits = 8*4 = 32 = 4 bytes
    int total_bits = 8 * 4;
    printf("Total bits: %d\n", total_bits); // EXPECT: Total bits: 32

    int unpacked[8];
    unpack_bits(bytes, 8, 4, unpacked);

    int i;
    int match = 1;
    for (i = 0; i < 8; i = i + 1) {
        if (vals4[i] != unpacked[i]) match = 0;
    }
    printf("4-bit roundtrip: %d\n", match); // EXPECT: 4-bit roundtrip: 1

    // Verify individual values
    printf("Unpack[0]: %d\n", unpacked[0]); // EXPECT: Unpack[0]: 3
    printf("Unpack[2]: %d\n", unpacked[2]); // EXPECT: Unpack[2]: 15
    printf("Unpack[7]: %d\n", unpacked[7]); // EXPECT: Unpack[7]: 5

    // Pack 10 values using 3 bits each (0-7)
    int vals3[10];
    vals3[0] = 5; vals3[1] = 3; vals3[2] = 7; vals3[3] = 0; vals3[4] = 1;
    vals3[5] = 6; vals3[6] = 2; vals3[7] = 4; vals3[8] = 7; vals3[9] = 3;

    pack_bits(vals3, 10, 3, bytes, &blen);
    printf("3-bit packed bytes: %d\n", blen); // EXPECT: 3-bit packed bytes: 4

    int unp3[10];
    unpack_bits(bytes, 10, 3, unp3);
    int match3 = 1;
    for (i = 0; i < 10; i = i + 1) {
        if (vals3[i] != unp3[i]) match3 = 0;
    }
    printf("3-bit roundtrip: %d\n", match3); // EXPECT: 3-bit roundtrip: 1

    // Pack 5 values using 7 bits each (0-127)
    int vals7[5];
    vals7[0] = 100; vals7[1] = 0; vals7[2] = 127; vals7[3] = 63; vals7[4] = 42;

    pack_bits(vals7, 5, 7, bytes, &blen);
    printf("7-bit packed bytes: %d\n", blen); // EXPECT: 7-bit packed bytes: 5

    int unp7[5];
    unpack_bits(bytes, 5, 7, unp7);
    int match7 = 1;
    for (i = 0; i < 5; i = i + 1) {
        if (vals7[i] != unp7[i]) match7 = 0;
    }
    printf("7-bit roundtrip: %d\n", match7); // EXPECT: 7-bit roundtrip: 1
    printf("7-bit[0]: %d\n", unp7[0]); // EXPECT: 7-bit[0]: 100
    printf("7-bit[2]: %d\n", unp7[2]); // EXPECT: 7-bit[2]: 127
    printf("7-bit[4]: %d\n", unp7[4]); // EXPECT: 7-bit[4]: 42

    // Compression ratio: 5 bytes (40 bits) raw -> 5 bytes packed (35 bits)
    int orig_bytes = 5;
    int comp_bytes = blen;
    printf("Orig bytes: %d\n", orig_bytes); // EXPECT: Orig bytes: 5
    printf("Packed bytes: %d\n", comp_bytes); // EXPECT: Packed bytes: 5

    // Pack 1-bit values (booleans)
    int bools[8];
    bools[0]=1; bools[1]=0; bools[2]=1; bools[3]=1;
    bools[4]=0; bools[5]=0; bools[6]=1; bools[7]=0;

    pack_bits(bools, 8, 1, bytes, &blen);
    printf("1-bit packed bytes: %d\n", blen); // EXPECT: 1-bit packed bytes: 1

    int unp1[8];
    unpack_bits(bytes, 8, 1, unp1);
    int match1 = 1;
    for (i = 0; i < 8; i = i + 1) {
        if (bools[i] != unp1[i]) match1 = 0;
    }
    printf("1-bit roundtrip: %d\n", match1); // EXPECT: 1-bit roundtrip: 1

    // Verify the packed byte value: bits = 10110100 = 0x... = 1+4+8+64 = 77?
    // bit0=1, bit1=0, bit2=1, bit3=1, bit4=0, bit5=0, bit6=1, bit7=0
    // = 1 + 0 + 4 + 8 + 0 + 0 + 64 + 0 = 77
    printf("Packed byte: %d\n", bytes[0]); // EXPECT: Packed byte: 77

    // Pack 2-bit values
    int vals2[4];
    vals2[0] = 3; vals2[1] = 1; vals2[2] = 0; vals2[3] = 2;
    pack_bits(vals2, 4, 2, bytes, &blen);
    printf("2-bit packed bytes: %d\n", blen); // EXPECT: 2-bit packed bytes: 1

    int unp2[4];
    unpack_bits(bytes, 4, 2, unp2);
    int match2 = 1;
    for (i = 0; i < 4; i = i + 1) {
        if (vals2[i] != unp2[i]) match2 = 0;
    }
    printf("2-bit roundtrip: %d\n", match2); // EXPECT: 2-bit roundtrip: 1

    printf("Done\n"); // EXPECT: Done
    return 0;
}
