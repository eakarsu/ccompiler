int printf(const char *fmt, ...);
// EXPECT: bit 0 = 1\nbit 1 = 0\nbit 7 = 1\nbit 31 = 1\nbit 32 = 1\nbit 33 = 0\nbit 63 = 1\nbit 64 = 1\nbit 100 = 1\nbit 127 = 1\ntotal set bits = 8\nrange [10,20) set bits = 10\nbits 9-20: 011111111110\neven bits set = 32\nword0 = 0x55555555\nword1 = 0x55555555\nset idempotent: 1
// Test: bit array - set bit

void set_bit(int *arr, int bit_index) {
    int word = bit_index / 32;
    int pos = bit_index % 32;
    arr[word] = arr[word] | (1 << pos);
}

int get_bit(int *arr, int bit_index) {
    int word = bit_index / 32;
    int pos = bit_index % 32;
    return (arr[word] >> pos) & 1;
}

int count_set_bits(int *arr, int num_words) {
    int count = 0;
    int i;
    int j;
    for (i = 0; i < num_words; i++) {
        int val = arr[i];
        for (j = 0; j < 32; j++) {
            count = count + (val & 1);
            val = val >> 1;
        }
    }
    return count;
}

int main(void) {
    // Create a bit array of 128 bits (4 ints)
    int bits[4];
    bits[0] = 0; bits[1] = 0; bits[2] = 0; bits[3] = 0;

    // Set individual bits
    set_bit(bits, 0);
    set_bit(bits, 7);
    set_bit(bits, 31);
    set_bit(bits, 32);
    set_bit(bits, 63);
    set_bit(bits, 64);
    set_bit(bits, 100);
    set_bit(bits, 127);

    // Read back
    printf("bit 0 = %d\n", get_bit(bits, 0));
    printf("bit 1 = %d\n", get_bit(bits, 1));
    printf("bit 7 = %d\n", get_bit(bits, 7));
    printf("bit 31 = %d\n", get_bit(bits, 31));
    printf("bit 32 = %d\n", get_bit(bits, 32));
    printf("bit 33 = %d\n", get_bit(bits, 33));
    printf("bit 63 = %d\n", get_bit(bits, 63));
    printf("bit 64 = %d\n", get_bit(bits, 64));
    printf("bit 100 = %d\n", get_bit(bits, 100));
    printf("bit 127 = %d\n", get_bit(bits, 127));

    // Count set bits
    printf("total set bits = %d\n", count_set_bits(bits, 4));

    // Set a range of bits
    int range_bits[4];
    range_bits[0] = 0; range_bits[1] = 0;
    range_bits[2] = 0; range_bits[3] = 0;
    int i;
    for (i = 10; i < 20; i++) {
        set_bit(range_bits, i);
    }
    printf("range [10,20) set bits = %d\n", count_set_bits(range_bits, 4));

    // Verify range
    printf("bits 9-20: ");
    for (i = 9; i <= 20; i++) {
        printf("%d", get_bit(range_bits, i));
    }
    printf("\n");

    // Set all even bits
    int even_bits[2];
    even_bits[0] = 0; even_bits[1] = 0;
    for (i = 0; i < 64; i += 2) {
        set_bit(even_bits, i);
    }
    printf("even bits set = %d\n", count_set_bits(even_bits, 2));
    printf("word0 = 0x%X\n", (unsigned)even_bits[0]);
    printf("word1 = 0x%X\n", (unsigned)even_bits[1]);

    // Idempotent: setting an already set bit
    int test[1];
    test[0] = 0;
    set_bit(test, 5);
    int before = test[0];
    set_bit(test, 5);
    int after = test[0];
    printf("set idempotent: %d\n", before == after);

    return 0;
}
