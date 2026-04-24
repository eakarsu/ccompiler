int printf(const char *fmt, ...);

int parity_bit(int val) {
    int p = 0;
    int v = val;
    while (v > 0) {
        p = p ^ (v & 1);
        v = v >> 1;
    }
    return p;
}

int check_parity(int val, int expected_parity) {
    return parity_bit(val) == expected_parity;
}

int count_bits(int val) {
    int count = 0;
    int v = val;
    while (v > 0) {
        count = count + (v & 1);
        v = v >> 1;
    }
    return count;
}

int hamming_encode_4bit(int data) {
    int d1 = (data >> 3) & 1;
    int d2 = (data >> 2) & 1;
    int d3 = (data >> 1) & 1;
    int d4 = data & 1;

    int p1 = d1 ^ d2 ^ d4;
    int p2 = d1 ^ d3 ^ d4;
    int p3 = d2 ^ d3 ^ d4;

    return (p1 << 6) | (p2 << 5) | (d1 << 4) | (p3 << 3) | (d2 << 2) | (d3 << 1) | d4;
}

int hamming_decode(int code) {
    int p1 = (code >> 6) & 1;
    int p2 = (code >> 5) & 1;
    int d1 = (code >> 4) & 1;
    int p3 = (code >> 3) & 1;
    int d2 = (code >> 2) & 1;
    int d3 = (code >> 1) & 1;
    int d4 = code & 1;

    int s1 = p1 ^ d1 ^ d2 ^ d4;
    int s2 = p2 ^ d1 ^ d3 ^ d4;
    int s3 = p3 ^ d2 ^ d3 ^ d4;

    int syndrome = (s1 << 2) | (s2 << 1) | s3;
    return (d1 << 3) | (d2 << 2) | (d3 << 1) | d4;
}

int hamming_detect_error(int code) {
    int p1 = (code >> 6) & 1;
    int p2 = (code >> 5) & 1;
    int d1 = (code >> 4) & 1;
    int p3 = (code >> 3) & 1;
    int d2 = (code >> 2) & 1;
    int d3 = (code >> 1) & 1;
    int d4 = code & 1;

    int s1 = p1 ^ d1 ^ d2 ^ d4;
    int s2 = p2 ^ d1 ^ d3 ^ d4;
    int s3 = p3 ^ d2 ^ d3 ^ d4;

    return (s1 << 2) | (s2 << 1) | s3;
}

int hamming_correct(int code) {
    int syndrome = hamming_detect_error(code);
    if (syndrome == 0) return code;
    int bit_pos = 7 - syndrome;
    return code ^ (1 << bit_pos);
}

int main(void) {
    // EXPECT: Parity(0): 0
    printf("Parity(0): %d\n", parity_bit(0));
    // EXPECT: Parity(1): 1
    printf("Parity(1): %d\n", parity_bit(1));
    // EXPECT: Parity(3): 0
    printf("Parity(3): %d\n", parity_bit(3));
    // EXPECT: Parity(7): 1
    printf("Parity(7): %d\n", parity_bit(7));
    // EXPECT: Parity(15): 0
    printf("Parity(15): %d\n", parity_bit(15));

    // EXPECT: Check parity 5,0: 1
    printf("Check parity 5,0: %d\n", check_parity(5, 0));
    // EXPECT: Check parity 5,1: 0
    printf("Check parity 5,1: %d\n", check_parity(5, 1));

    // EXPECT: Bits in 0: 0
    printf("Bits in 0: %d\n", count_bits(0));
    // EXPECT: Bits in 7: 3
    printf("Bits in 7: %d\n", count_bits(7));
    // EXPECT: Bits in 255: 8
    printf("Bits in 255: %d\n", count_bits(255));

    int enc5 = hamming_encode_4bit(5);
    // EXPECT: Hamming encode 5: 37
    printf("Hamming encode 5: %d\n", enc5);

    int dec5 = hamming_decode(enc5);
    // EXPECT: Hamming decode 5: 5
    printf("Hamming decode 5: %d\n", dec5);

    int syn_ok = hamming_detect_error(enc5);
    // EXPECT: Syndrome no error: 0
    printf("Syndrome no error: %d\n", syn_ok);

    int corrupted = enc5 ^ 4;
    int syn_err = hamming_detect_error(corrupted);
    // EXPECT: Syndrome with error: 5
    printf("Syndrome with error: %d\n", syn_err);

    int corrected = hamming_correct(corrupted);
    // EXPECT: Corrected equals original: 1
    printf("Corrected equals original: %d\n", corrected == enc5 ? 1 : 0);

    int enc0 = hamming_encode_4bit(0);
    // EXPECT: Hamming encode 0: 0
    printf("Hamming encode 0: %d\n", enc0);

    int enc15 = hamming_encode_4bit(15);
    int dec15 = hamming_decode(enc15);
    // EXPECT: Roundtrip 15: 15
    printf("Roundtrip 15: %d\n", dec15);

    int all_ok = 1;
    int i;
    for (i = 0; i < 16; i = i + 1) {
        int e = hamming_encode_4bit(i);
        int d = hamming_decode(e);
        if (d != i) all_ok = 0;
    }
    // EXPECT: All 4-bit roundtrip: 1
    printf("All 4-bit roundtrip: %d\n", all_ok);

    return 0;
}
