int printf(const char *fmt, ...);
// EXPECT: parity(0) = 0\nparity(1) = 1\nparity(2) = 1\nparity(3) = 0\nparity(7) = 1\nparity(255) = 0\nparity(256) = 1\nmethods agree: 1\nparity[0..15]: 0110100110010110\nadd_parity:\n  0x41 -> 0x41 (valid=1)\n  0x42 -> 0x42 (valid=1)\n  0x43 -> 0xC3 (valid=1)\n  0x00 -> 0x00 (valid=1)\ngood parity check: 1\nbad parity check: 0\nXOR of 0..255 = 0\nparity(0xAA)=0 parity(0x55)=0 parity(0xAA^0x55)=0\nparity distributes: 1
// Test: parity computation

int parity_naive(int x) {
    int p = 0;
    int val = x;
    while (val) {
        p = p ^ (val & 1);
        val = (unsigned)val >> 1;
    }
    return p;
}

// Parallel parity using XOR fold
int parity_fast(int x) {
    int val = x;
    val = val ^ (val >> 16);
    val = val ^ (val >> 8);
    val = val ^ (val >> 4);
    val = val ^ (val >> 2);
    val = val ^ (val >> 1);
    return val & 1;
}

// Add even parity bit (returns byte with parity in bit 7)
int add_parity(int byte_val) {
    int p = parity_naive(byte_val & 0x7F);
    return (byte_val & 0x7F) | (p << 7);
}

// Check parity of received byte (returns 1 if valid)
int check_parity(int byte_val) {
    return parity_naive(byte_val) == 0;
}

int main(void) {
    // Basic parity
    printf("parity(0) = %d\n", parity_naive(0));
    printf("parity(1) = %d\n", parity_naive(1));
    printf("parity(2) = %d\n", parity_naive(2));
    printf("parity(3) = %d\n", parity_naive(3));
    printf("parity(7) = %d\n", parity_naive(7));
    printf("parity(255) = %d\n", parity_naive(255));
    printf("parity(256) = %d\n", parity_naive(256));

    // Fast parity should match naive
    int test_vals[8];
    test_vals[0] = 0;
    test_vals[1] = 1;
    test_vals[2] = 42;
    test_vals[3] = 255;
    test_vals[4] = 1023;
    test_vals[5] = 0xDEAD;
    test_vals[6] = 0x5555;
    test_vals[7] = 0xFFFF;

    int i;
    int all_match = 1;
    for (i = 0; i < 8; i++) {
        if (parity_naive(test_vals[i]) != parity_fast(test_vals[i])) {
            all_match = 0;
        }
    }
    printf("methods agree: %d\n", all_match);

    // Parity of each byte in 0..15
    printf("parity[0..15]: ");
    for (i = 0; i < 16; i++) {
        printf("%d", parity_naive(i));
    }
    printf("\n");

    // Even parity bit
    printf("add_parity:\n");
    int bytes[4];
    bytes[0] = 0x41;  // 'A' = 01000001, 2 bits -> parity 0
    bytes[1] = 0x42;  // 'B' = 01000010, 2 bits -> parity 0
    bytes[2] = 0x43;  // 'C' = 01000011, 3 bits -> parity 1
    bytes[3] = 0x00;  // 0 bits -> parity 0
    for (i = 0; i < 4; i++) {
        int with_parity = add_parity(bytes[i]);
        printf("  0x%02X -> 0x%02X (valid=%d)\n",
               bytes[i], with_parity, check_parity(with_parity));
    }

    // Detect single bit error
    int good = add_parity(0x41);
    int bad = good ^ 0x04;  // flip one bit
    printf("good parity check: %d\n", check_parity(good));
    printf("bad parity check: %d\n", check_parity(bad));

    // XOR of all values 0..255 should be 0
    int xor_all = 0;
    for (i = 0; i < 256; i++) {
        xor_all = xor_all ^ i;
    }
    printf("XOR of 0..255 = %d\n", xor_all);

    // Parity is XOR of all bits, so it distributes over XOR
    int a = 0xAA;
    int b = 0x55;
    int p_a = parity_naive(a);
    int p_b = parity_naive(b);
    int p_xor = parity_naive(a ^ b);
    printf("parity(0xAA)=%d parity(0x55)=%d parity(0xAA^0x55)=%d\n",
           p_a, p_b, p_xor);
    printf("parity distributes: %d\n", p_xor == (p_a ^ p_b));

    return 0;
}
