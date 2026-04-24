int printf(const char *fmt, ...);

void xor_encrypt(int *out, int *in, int len, int key) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        out[i] = in[i] ^ key;
    }
}

void xor_decrypt(int *out, int *in, int len, int key) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        out[i] = in[i] ^ key;
    }
}

void xor_multi_key(int *out, int *in, int len, int *key, int klen) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        out[i] = in[i] ^ key[i % klen];
    }
}

int arrays_equal(int *a, int *b, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int main(void) {
    int data[8];
    int cipher[8];
    int decoded[8];
    int key[4];
    int i;

    data[0] = 72; data[1] = 101; data[2] = 108; data[3] = 108;
    data[4] = 111; data[5] = 0; data[6] = 0; data[7] = 0;

    xor_encrypt(cipher, data, 5, 42);
    // EXPECT: XOR encrypted[0]: 98
    printf("XOR encrypted[0]: %d\n", cipher[0]);
    // EXPECT: XOR encrypted[1]: 79
    printf("XOR encrypted[1]: %d\n", cipher[1]);

    xor_decrypt(decoded, cipher, 5, 42);
    // EXPECT: Roundtrip OK: 1
    printf("Roundtrip OK: %d\n", arrays_equal(data, decoded, 5));

    // EXPECT: Decoded[0]: 72
    printf("Decoded[0]: %d\n", decoded[0]);
    // EXPECT: Decoded[1]: 101
    printf("Decoded[1]: %d\n", decoded[1]);

    xor_encrypt(cipher, data, 5, 0);
    // EXPECT: XOR key 0 unchanged: 1
    printf("XOR key 0 unchanged: %d\n", arrays_equal(data, cipher, 5));

    xor_encrypt(cipher, data, 5, 255);
    xor_decrypt(decoded, cipher, 5, 255);
    // EXPECT: Key 255 roundtrip: 1
    printf("Key 255 roundtrip: %d\n", arrays_equal(data, decoded, 5));

    int val = 123;
    int enc = val ^ 99;
    int dec = enc ^ 99;
    // EXPECT: Single val roundtrip: 123
    printf("Single val roundtrip: %d\n", dec);

    key[0] = 10; key[1] = 20; key[2] = 30; key[3] = 40;
    xor_multi_key(cipher, data, 5, key, 4);
    // EXPECT: Multi key cipher[0]: 66
    printf("Multi key cipher[0]: %d\n", cipher[0]);
    // EXPECT: Multi key cipher[1]: 113
    printf("Multi key cipher[1]: %d\n", cipher[1]);
    // EXPECT: Multi key cipher[2]: 114
    printf("Multi key cipher[2]: %d\n", cipher[2]);
    // EXPECT: Multi key cipher[3]: 68
    printf("Multi key cipher[3]: %d\n", cipher[3]);

    xor_multi_key(decoded, cipher, 5, key, 4);
    // EXPECT: Multi key roundtrip: 1
    printf("Multi key roundtrip: %d\n", arrays_equal(data, decoded, 5));

    int a = 0;
    int b = a ^ 255;
    // EXPECT: 0 XOR 255: 255
    printf("0 XOR 255: %d\n", b);

    int c = 255 ^ 255;
    // EXPECT: 255 XOR 255: 0
    printf("255 XOR 255: %d\n", c);

    int seq[4];
    seq[0] = 1; seq[1] = 2; seq[2] = 3; seq[3] = 4;
    xor_encrypt(cipher, seq, 4, 7);
    // EXPECT: Seq enc: 6 5 4 3
    printf("Seq enc: %d %d %d %d\n", cipher[0], cipher[1], cipher[2], cipher[3]);

    xor_decrypt(decoded, cipher, 4, 7);
    // EXPECT: Seq dec: 1 2 3 4
    printf("Seq dec: %d %d %d %d\n", decoded[0], decoded[1], decoded[2], decoded[3]);

    int xor_sum = 0;
    for (i = 0; i < 5; i = i + 1) {
        xor_sum = xor_sum ^ data[i];
    }
    // EXPECT: XOR sum of data: 66
    printf("XOR sum of data: %d\n", xor_sum);

    return 0;
}
