int printf(const char *fmt, ...);

int binary_to_gray(int n) {
    return n ^ ((n >> 1) & 0x7FFFFFFF);
}

int gray_to_binary(int gray) {
    int binary = gray;
    int mask = (gray >> 1) & 0x7FFFFFFF;
    while (mask) {
        binary = binary ^ mask;
        mask = (mask >> 1) & 0x7FFFFFFF;
    }
    return binary;
}

int popcount(int x) {
    int count = 0;
    while (x) {
        count = count + (x & 1);
        x = (x >> 1) & 0x7FFFFFFF;
    }
    return count;
}

int gray_distance(int a, int b) {
    return popcount(a ^ b);
}

int is_valid_gray_sequence(int seq[], int n) {
    int i = 1;
    while (i < n) {
        int diff = seq[i - 1] ^ seq[i];
        if (popcount(diff) != 1) return 0;
        i = i + 1;
    }
    return 1;
}

void generate_gray_sequence(int seq[], int nbits) {
    int count = 1 << nbits;
    int i = 0;
    while (i < count) {
        seq[i] = binary_to_gray(i);
        i = i + 1;
    }
}

int gray_increment(int gray, int nbits) {
    int binary = gray_to_binary(gray);
    binary = binary + 1;
    if (binary >= (1 << nbits)) binary = 0;
    return binary_to_gray(binary);
}

int gray_decrement(int gray, int nbits) {
    int binary = gray_to_binary(gray);
    binary = binary - 1;
    if (binary < 0) binary = (1 << nbits) - 1;
    return binary_to_gray(binary);
}

int nth_gray(int n) {
    return binary_to_gray(n);
}

int gray_rank(int gray) {
    return gray_to_binary(gray);
}

int gray_changed_bit(int prev_gray, int next_gray) {
    int diff = prev_gray ^ next_gray;
    int pos = 0;
    while (diff > 1) {
        diff = (diff >> 1) & 0x7FFFFFFF;
        pos = pos + 1;
    }
    return pos;
}

int reflect_gray(int code, int nbits) {
    int result = 0;
    int i = 0;
    while (i < nbits) {
        if (code & (1 << i)) {
            result = result | (1 << (nbits - 1 - i));
        }
        i = i + 1;
    }
    return result;
}

int main(void) {
    // EXPECT: b2g(0) = 0
    printf("b2g(0) = %d\n", binary_to_gray(0));
    // EXPECT: b2g(1) = 1
    printf("b2g(1) = %d\n", binary_to_gray(1));
    // EXPECT: b2g(2) = 3
    printf("b2g(2) = %d\n", binary_to_gray(2));
    // EXPECT: b2g(3) = 2
    printf("b2g(3) = %d\n", binary_to_gray(3));
    // EXPECT: b2g(4) = 6
    printf("b2g(4) = %d\n", binary_to_gray(4));
    // EXPECT: b2g(5) = 7
    printf("b2g(5) = %d\n", binary_to_gray(5));
    // EXPECT: b2g(6) = 5
    printf("b2g(6) = %d\n", binary_to_gray(6));
    // EXPECT: b2g(7) = 4
    printf("b2g(7) = %d\n", binary_to_gray(7));

    // EXPECT: g2b(0) = 0
    printf("g2b(0) = %d\n", gray_to_binary(0));
    // EXPECT: g2b(3) = 2
    printf("g2b(3) = %d\n", gray_to_binary(3));
    // EXPECT: g2b(6) = 4
    printf("g2b(6) = %d\n", gray_to_binary(6));
    // EXPECT: g2b(4) = 7
    printf("g2b(4) = %d\n", gray_to_binary(4));

    int seq[16];
    generate_gray_sequence(seq, 3);
    // EXPECT: gray3: 0 1 3 2 6 7 5 4
    printf("gray3: %d %d %d %d %d %d %d %d\n",
        seq[0], seq[1], seq[2], seq[3], seq[4], seq[5], seq[6], seq[7]);

    // EXPECT: valid_gray = 1
    printf("valid_gray = %d\n", is_valid_gray_sequence(seq, 8));

    int bad_seq[4];
    bad_seq[0] = 0; bad_seq[1] = 1; bad_seq[2] = 2; bad_seq[3] = 3;
    // EXPECT: valid_bad = 0
    printf("valid_bad = %d\n", is_valid_gray_sequence(bad_seq, 4));

    // EXPECT: gray_inc(0, 3) = 1
    printf("gray_inc(0, 3) = %d\n", gray_increment(0, 3));
    // EXPECT: gray_inc(1, 3) = 3
    printf("gray_inc(1, 3) = %d\n", gray_increment(1, 3));
    // EXPECT: gray_inc(4, 3) = 0
    printf("gray_inc(4, 3) = %d\n", gray_increment(4, 3));

    // EXPECT: gray_dec(1, 3) = 0
    printf("gray_dec(1, 3) = %d\n", gray_decrement(1, 3));
    // EXPECT: gray_dec(0, 3) = 4
    printf("gray_dec(0, 3) = %d\n", gray_decrement(0, 3));

    // EXPECT: rank(6) = 4
    printf("rank(6) = %d\n", gray_rank(6));
    // EXPECT: rank(7) = 5
    printf("rank(7) = %d\n", gray_rank(7));

    // EXPECT: changed_bit(0, 1) = 0
    printf("changed_bit(0, 1) = %d\n", gray_changed_bit(0, 1));
    // EXPECT: changed_bit(1, 3) = 1
    printf("changed_bit(1, 3) = %d\n", gray_changed_bit(1, 3));
    // EXPECT: changed_bit(3, 2) = 0
    printf("changed_bit(3, 2) = %d\n", gray_changed_bit(3, 2));

    generate_gray_sequence(seq, 4);
    // EXPECT: gray4_valid = 1
    printf("gray4_valid = %d\n", is_valid_gray_sequence(seq, 16));

    int roundtrip = 13;
    int g = binary_to_gray(roundtrip);
    int back = gray_to_binary(g);
    // EXPECT: roundtrip(13) = 13
    printf("roundtrip(13) = %d\n", back);

    return 0;
}
