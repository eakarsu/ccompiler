int printf(const char *fmt, ...);
// EXPECT: initial set bits = 128\nafter clearing 4 bits = 124\nbit 0 = 0\nbit 1 = 1\nbit 31 = 0\nbit 32 = 0\nbit 33 = 1\nbit 127 = 0\nafter clearing [8,24): 48 bits set\nbits 0-31: 11111111 00000000 00000000 11111111\nodd cleared = 0x55555555\nall set = 0xFFFFFFFF\nall cleared = 0x0\nclear idempotent: 1
// Test: bit array - clear bit

void set_bit(int *arr, int idx) {
    arr[idx / 32] = arr[idx / 32] | (1 << (idx % 32));
}

void clear_bit(int *arr, int idx) {
    arr[idx / 32] = arr[idx / 32] & ~(1 << (idx % 32));
}

int get_bit(int *arr, int idx) {
    return (arr[idx / 32] >> (idx % 32)) & 1;
}

int popcount_arr(int *arr, int words) {
    int count = 0;
    int i;
    int j;
    for (i = 0; i < words; i++) {
        int v = arr[i];
        for (j = 0; j < 32; j++) {
            count = count + (v & 1);
            v = v >> 1;
        }
    }
    return count;
}

int main(void) {
    // Start with all bits set
    int bits[4];
    bits[0] = -1; bits[1] = -1; bits[2] = -1; bits[3] = -1;
    printf("initial set bits = %d\n", popcount_arr(bits, 4));

    // Clear specific bits
    clear_bit(bits, 0);
    clear_bit(bits, 31);
    clear_bit(bits, 32);
    clear_bit(bits, 127);
    printf("after clearing 4 bits = %d\n", popcount_arr(bits, 4));

    // Verify cleared
    printf("bit 0 = %d\n", get_bit(bits, 0));
    printf("bit 1 = %d\n", get_bit(bits, 1));
    printf("bit 31 = %d\n", get_bit(bits, 31));
    printf("bit 32 = %d\n", get_bit(bits, 32));
    printf("bit 33 = %d\n", get_bit(bits, 33));
    printf("bit 127 = %d\n", get_bit(bits, 127));

    // Clear a range
    int range[2];
    range[0] = -1; range[1] = -1;
    int i;
    for (i = 8; i < 24; i++) {
        clear_bit(range, i);
    }
    printf("after clearing [8,24): %d bits set\n", popcount_arr(range, 2));

    // Show pattern
    printf("bits 0-31: ");
    for (i = 0; i < 32; i++) {
        if (i > 0 && i % 8 == 0) printf(" ");
        printf("%d", get_bit(range, i));
    }
    printf("\n");

    // Clear all odd bits from a fully set word
    int odd_clear[1];
    odd_clear[0] = -1;
    for (i = 1; i < 32; i += 2) {
        clear_bit(odd_clear, i);
    }
    printf("odd cleared = 0x%X\n", (unsigned)odd_clear[0]);

    // Set then clear (should return to zero)
    int test[1];
    test[0] = 0;
    for (i = 0; i < 32; i++) {
        set_bit(test, i);
    }
    printf("all set = 0x%X\n", (unsigned)test[0]);
    for (i = 0; i < 32; i++) {
        clear_bit(test, i);
    }
    printf("all cleared = 0x%X\n", (unsigned)test[0]);

    // Clear is idempotent
    test[0] = 0xFF;
    clear_bit(test, 3);
    int before = test[0];
    clear_bit(test, 3);
    int after = test[0];
    printf("clear idempotent: %d\n", before == after);

    return 0;
}
