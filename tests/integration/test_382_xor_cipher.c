int printf(const char *fmt, ...);
// EXPECT: original: Hello\nencrypted: 0A 27 2E 2E 2D \ndecrypted: Hello\nmatches: 1\nencrypted2: BC DB CE AC 8A CA 8C EF DD 8D \ndecrypted2: Secret!123\nmatches2: 1\nXOR properties:\n  x ^ 0 = x: 1\n  x ^ x = 0: 1\n  x ^ k ^ k = x: 1\n  (x ^ y) ^ z = x ^ (y ^ z): 1\nAAA encrypted: 14 14 14 (same=1)
// Test: XOR cipher

void xor_encrypt(char *data, int len, int key) {
    int i;
    for (i = 0; i < len; i++) {
        data[i] = data[i] ^ ((key >> ((i % 4) * 8)) & 0xFF);
    }
}

void xor_decrypt(char *data, int len, int key) {
    // XOR cipher is symmetric: encrypt = decrypt
    xor_encrypt(data, len, key);
}

int strings_equal(char *a, char *b, int len) {
    int i;
    for (i = 0; i < len; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int main(void) {
    // Simple single-byte XOR
    char msg1[6];
    msg1[0] = 'H'; msg1[1] = 'e'; msg1[2] = 'l';
    msg1[3] = 'l'; msg1[4] = 'o'; msg1[5] = 0;
    printf("original: %s\n", msg1);

    // Save original
    char orig[6];
    int i;
    for (i = 0; i < 5; i++) orig[i] = msg1[i];
    orig[5] = 0;

    // Encrypt with single byte key
    int key1 = 0x42;
    for (i = 0; i < 5; i++) {
        msg1[i] = msg1[i] ^ key1;
    }
    printf("encrypted: ");
    for (i = 0; i < 5; i++) printf("%02X ", msg1[i] & 0xFF);
    printf("\n");

    // Decrypt
    for (i = 0; i < 5; i++) {
        msg1[i] = msg1[i] ^ key1;
    }
    printf("decrypted: %s\n", msg1);
    printf("matches: %d\n", strings_equal(msg1, orig, 5));

    // Multi-byte key XOR
    char msg2[12];
    msg2[0] = 'S'; msg2[1] = 'e'; msg2[2] = 'c';
    msg2[3] = 'r'; msg2[4] = 'e'; msg2[5] = 't';
    msg2[6] = '!'; msg2[7] = '1'; msg2[8] = '2';
    msg2[9] = '3'; msg2[10] = 0; msg2[11] = 0;
    int len2 = 10;

    char orig2[12];
    for (i = 0; i < len2; i++) orig2[i] = msg2[i];

    int key2 = 0xDEADBEEF;
    xor_encrypt(msg2, len2, key2);
    printf("encrypted2: ");
    for (i = 0; i < len2; i++) printf("%02X ", msg2[i] & 0xFF);
    printf("\n");

    xor_decrypt(msg2, len2, key2);
    printf("decrypted2: ");
    for (i = 0; i < len2; i++) printf("%c", msg2[i]);
    printf("\n");
    printf("matches2: %d\n", strings_equal(msg2, orig2, len2));

    // XOR properties verification
    int val = 0x42;
    int k = 0xFF;
    printf("XOR properties:\n");
    printf("  x ^ 0 = x: %d\n", (val ^ 0) == val);
    printf("  x ^ x = 0: %d\n", (val ^ val) == 0);
    printf("  x ^ k ^ k = x: %d\n", ((val ^ k) ^ k) == val);
    printf("  (x ^ y) ^ z = x ^ (y ^ z): %d\n",
           ((val ^ k) ^ 0xAA) == (val ^ (k ^ 0xAA)));

    // Frequency analysis hint: XOR of same chars
    char aaa[4];
    aaa[0] = 'A'; aaa[1] = 'A'; aaa[2] = 'A'; aaa[3] = 0;
    for (i = 0; i < 3; i++) aaa[i] = aaa[i] ^ 0x55;
    printf("AAA encrypted: %02X %02X %02X (same=%d)\n",
           aaa[0] & 0xFF, aaa[1] & 0xFF, aaa[2] & 0xFF,
           aaa[0] == aaa[1] && aaa[1] == aaa[2]);

    return 0;
}
