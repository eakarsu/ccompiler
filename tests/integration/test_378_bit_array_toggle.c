int printf(const char *fmt, ...);
// EXPECT: after toggling 6 bits:\nbit 0 = 1\nbit 5 = 1\nbit 10 = 1\nbit 15 = 0\nbit 31 = 1\nbit 32 = 1\nbit 63 = 1\nafter double toggle: word0=0 word1=0\nall toggled on = 0xFFFFFFFF\nall toggled off = 0x0\nevery 3rd bit: 22 bits set\nfirst 24 bits: 100100100100100100100100\nencrypt/decrypt match: 1\nencrypted word0 = 0x1234DEF0
// Test: bit array - toggle bit

void toggle_bit(int *arr, int idx) {
    arr[idx / 32] = arr[idx / 32] ^ (1 << (idx % 32));
}

int get_bit(int *arr, int idx) {
    return (arr[idx / 32] >> (idx % 32)) & 1;
}

int popcount_word(int x) {
    int count = 0;
    int v = x;
    while (v) {
        v = v & (v - 1);
        count++;
    }
    return count;
}

int main(void) {
    // Toggle from zero
    int bits[2];
    bits[0] = 0; bits[1] = 0;

    toggle_bit(bits, 0);
    toggle_bit(bits, 5);
    toggle_bit(bits, 10);
    toggle_bit(bits, 31);
    toggle_bit(bits, 32);
    toggle_bit(bits, 63);

    printf("after toggling 6 bits:\n");
    printf("bit 0 = %d\n", get_bit(bits, 0));
    printf("bit 5 = %d\n", get_bit(bits, 5));
    printf("bit 10 = %d\n", get_bit(bits, 10));
    printf("bit 15 = %d\n", get_bit(bits, 15));
    printf("bit 31 = %d\n", get_bit(bits, 31));
    printf("bit 32 = %d\n", get_bit(bits, 32));
    printf("bit 63 = %d\n", get_bit(bits, 63));

    // Toggle again (should return to 0)
    toggle_bit(bits, 0);
    toggle_bit(bits, 5);
    toggle_bit(bits, 10);
    toggle_bit(bits, 31);
    toggle_bit(bits, 32);
    toggle_bit(bits, 63);
    printf("after double toggle: word0=%d word1=%d\n", bits[0], bits[1]);

    // Toggle all bits in a word
    int all[1];
    all[0] = 0;
    int i;
    for (i = 0; i < 32; i++) {
        toggle_bit(all, i);
    }
    printf("all toggled on = 0x%X\n", (unsigned)all[0]);
    for (i = 0; i < 32; i++) {
        toggle_bit(all, i);
    }
    printf("all toggled off = 0x%X\n", (unsigned)all[0]);

    // Toggle pattern: every 3rd bit
    int pattern[2];
    pattern[0] = 0; pattern[1] = 0;
    for (i = 0; i < 64; i += 3) {
        toggle_bit(pattern, i);
    }
    int count = popcount_word(pattern[0]) + popcount_word(pattern[1]);
    printf("every 3rd bit: %d bits set\n", count);

    // Show first 24 bits of pattern
    printf("first 24 bits: ");
    for (i = 0; i < 24; i++) {
        printf("%d", get_bit(pattern, i));
    }
    printf("\n");

    // XOR toggle for encryption/decryption simulation
    int data[2];
    data[0] = 0x12345678;
    data[1] = 0xABCDEF00;
    int key_bits[8];
    key_bits[0] = 3; key_bits[1] = 7; key_bits[2] = 11; key_bits[3] = 15;
    key_bits[4] = 35; key_bits[5] = 40; key_bits[6] = 50; key_bits[7] = 60;

    // Encrypt
    for (i = 0; i < 8; i++) toggle_bit(data, key_bits[i]);
    int enc0 = data[0];
    int enc1 = data[1];

    // Decrypt
    for (i = 0; i < 8; i++) toggle_bit(data, key_bits[i]);
    printf("encrypt/decrypt match: %d\n",
           data[0] == 0x12345678 && data[1] == (int)0xABCDEF00);

    printf("encrypted word0 = 0x%X\n", (unsigned)enc0);

    return 0;
}
