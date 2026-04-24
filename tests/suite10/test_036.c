int printf(const char *fmt, ...);

// Bit Packing - packing multiple small values into integers

struct BitStream {
    int buffer[16];
    int bit_pos;
    int word_count;
};

void bs_init(struct BitStream *bs) {
    int i = 0;
    while (i < 16) {
        bs->buffer[i] = 0;
        i = i + 1;
    }
    bs->bit_pos = 0;
    bs->word_count = 0;
}

void bs_write_bits(struct BitStream *bs, int value, int num_bits) {
    int bits_written = 0;
    while (bits_written < num_bits) {
        int word_idx = bs->bit_pos / 32;
        int bit_idx = bs->bit_pos % 32;

        // Get the current bit from value
        int bit = (value >> bits_written) & 1;

        // Set the bit in the buffer
        bs->buffer[word_idx] = bs->buffer[word_idx] | (bit << bit_idx);

        bs->bit_pos = bs->bit_pos + 1;
        bits_written = bits_written + 1;

        if (word_idx + 1 > bs->word_count) {
            bs->word_count = word_idx + 1;
        }
    }
}

int bs_read_bits(struct BitStream *bs, int start_bit, int num_bits) {
    int result = 0;
    int bits_read = 0;
    int pos = start_bit;
    while (bits_read < num_bits) {
        int word_idx = pos / 32;
        int bit_idx = pos % 32;

        int bit = (bs->buffer[word_idx] >> bit_idx) & 1;
        result = result | (bit << bits_read);

        pos = pos + 1;
        bits_read = bits_read + 1;
    }
    return result;
}

int bs_total_bits(struct BitStream *bs) {
    return bs->bit_pos;
}

// Pack an array of values with a given bit width
void pack_values(int *values, int count, int bit_width, struct BitStream *bs) {
    int i = 0;
    while (i < count) {
        bs_write_bits(bs, values[i], bit_width);
        i = i + 1;
    }
}

// Unpack values from a bitstream
void unpack_values(struct BitStream *bs, int count, int bit_width, int *values) {
    int i = 0;
    int pos = 0;
    while (i < count) {
        values[i] = bs_read_bits(bs, pos, bit_width);
        pos = pos + bit_width;
        i = i + 1;
    }
}

// Calculate minimum bits needed to represent max value
int min_bits_needed(int max_value) {
    if (max_value == 0) return 1;
    int bits = 0;
    int v = max_value;
    while (v > 0) {
        bits = bits + 1;
        v = v >> 1;
    }
    return bits;
}

int main() {
    struct BitStream bs;

    // Test 1: Pack 3-bit values (0-7)
    bs_init(&bs);
    int vals1[8];
    vals1[0] = 5; vals1[1] = 3; vals1[2] = 7; vals1[3] = 1;
    vals1[4] = 0; vals1[5] = 6; vals1[6] = 2; vals1[7] = 4;

    pack_values(vals1, 8, 3, &bs);
    int total_bits = bs_total_bits(&bs);
    printf("8 values x 3 bits = %d bits\n", total_bits); // EXPECT: 8 values x 3 bits = 24 bits

    // Unpack and verify
    int unpacked[8];
    unpack_values(&bs, 8, 3, unpacked);
    int i = 0;
    int all_match = 1;
    while (i < 8) {
        if (unpacked[i] != vals1[i]) all_match = 0;
        i = i + 1;
    }
    printf("3-bit roundtrip match: %d\n", all_match); // EXPECT: 3-bit roundtrip match: 1
    printf("Unpacked: %d %d %d %d %d %d %d %d\n", unpacked[0], unpacked[1], unpacked[2], unpacked[3], unpacked[4], unpacked[5], unpacked[6], unpacked[7]);
    // EXPECT: Unpacked: 5 3 7 1 0 6 2 4

    // Test 2: Pack 5-bit values
    bs_init(&bs);
    int vals2[4];
    vals2[0] = 17; vals2[1] = 31; vals2[2] = 8; vals2[3] = 25;
    pack_values(vals2, 4, 5, &bs);
    printf("4 values x 5 bits = %d bits\n", bs_total_bits(&bs)); // EXPECT: 4 values x 5 bits = 20 bits

    int unpacked2[4];
    unpack_values(&bs, 4, 5, unpacked2);
    printf("5-bit vals: %d %d %d %d\n", unpacked2[0], unpacked2[1], unpacked2[2], unpacked2[3]);
    // EXPECT: 5-bit vals: 17 31 8 25

    // Test 3: Space savings calculation
    // 8 values that fit in 3 bits (max=7) vs 32 bits each
    int original_bits = 8 * 32;
    int packed_bits = 8 * 3;
    int savings = 100 - (packed_bits * 100 / original_bits);
    printf("Original: %d bits, Packed: %d bits\n", original_bits, packed_bits); // EXPECT: Original: 256 bits, Packed: 24 bits
    printf("Savings: %d percent\n", savings); // EXPECT: Savings: 91 percent

    // Test 4: Min bits calculation
    printf("Bits for 0: %d\n", min_bits_needed(0)); // EXPECT: Bits for 0: 1
    printf("Bits for 1: %d\n", min_bits_needed(1)); // EXPECT: Bits for 1: 1
    printf("Bits for 7: %d\n", min_bits_needed(7)); // EXPECT: Bits for 7: 3
    printf("Bits for 8: %d\n", min_bits_needed(8)); // EXPECT: Bits for 8: 4
    printf("Bits for 255: %d\n", min_bits_needed(255)); // EXPECT: Bits for 255: 8
    printf("Bits for 1000: %d\n", min_bits_needed(1000)); // EXPECT: Bits for 1000: 10

    // Test 5: Mixed bit widths in sequence
    bs_init(&bs);
    bs_write_bits(&bs, 3, 2);   // 2-bit value
    bs_write_bits(&bs, 15, 4);  // 4-bit value
    bs_write_bits(&bs, 1, 1);   // 1-bit value
    printf("Mixed total: %d bits\n", bs_total_bits(&bs)); // EXPECT: Mixed total: 7 bits

    int r1 = bs_read_bits(&bs, 0, 2);
    int r2 = bs_read_bits(&bs, 2, 4);
    int r3 = bs_read_bits(&bs, 6, 1);
    printf("Mixed read: %d %d %d\n", r1, r2, r3); // EXPECT: Mixed read: 3 15 1

    printf("Bit packing done\n"); // EXPECT: Bit packing done
    return 0;
}
