// EXPECT: === XOR Cipher Tests ===
// EXPECT: Single-byte XOR key=42:
// EXPECT:   Original:  HELLO
// EXPECT:   Encrypted: boffe
// EXPECT:   Decrypted: HELLO
// EXPECT:   Roundtrip: OK
// EXPECT: Single-byte XOR key=255:
// EXPECT:   Roundtrip: OK
// EXPECT: Multi-byte XOR key="KEY":
// EXPECT:   Original:  ABCDEF
// EXPECT:   Decrypted: ABCDEF
// EXPECT:   Roundtrip: OK
// EXPECT: Multi-byte XOR key="SECRET":
// EXPECT:   Original:  Attack at dawn!
// EXPECT:   Decrypted: Attack at dawn!
// EXPECT:   Roundtrip: OK
// EXPECT: XOR properties:
// EXPECT:   A ^ 0 = A: OK
// EXPECT:   A ^ A = 0: OK
// EXPECT:   (A^B)^B = A: OK
// EXPECT: Binary data test:
// EXPECT:   byte 0: orig=0 dec=0 OK
// EXPECT:   byte 1: orig=1 dec=1 OK
// EXPECT:   byte 2: orig=127 dec=127 OK
// EXPECT:   byte 3: orig=66 dec=66 OK
// EXPECT:   byte 4: orig=100 dec=100 OK
// EXPECT: All 5 bytes match
// EXPECT: Key length test:
// EXPECT:   key len 1 roundtrip: OK
// EXPECT:   key len 3 roundtrip: OK
// EXPECT:   key len 7 roundtrip: OK
// EXPECT: All XOR cipher tests passed!

int printf(const char *fmt, ...);

int str_len(char *s) {
    int len = 0;
    while (s[len] != '\0') len = len + 1;
    return len;
}

int str_eq(char *a, char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

void xor_single(char *input, int len, char key, char *output) {
    int i;
    for (i = 0; i < len; i++) {
        output[i] = input[i] ^ key;
    }
    output[len] = '\0';
}

void xor_multi(char *input, int len, char *key, int keylen, char *output) {
    int i;
    for (i = 0; i < len; i++) {
        output[i] = input[i] ^ key[i % keylen];
    }
    output[len] = '\0';
}

int main() {
    char encrypted[128];
    char decrypted[128];
    int len;

    printf("=== XOR Cipher Tests ===\n");

    // Single-byte XOR with key=42
    printf("Single-byte XOR key=42:\n");
    char *msg1 = "HELLO";
    len = str_len(msg1);
    xor_single(msg1, len, 42, encrypted);
    printf("  Original:  %s\n", msg1);
    printf("  Encrypted: %s\n", encrypted);
    xor_single(encrypted, len, 42, decrypted);
    printf("  Decrypted: %s\n", decrypted);
    printf("  Roundtrip: %s\n", str_eq(msg1, decrypted) ? "OK" : "FAIL");

    // Single-byte XOR with key=255
    printf("Single-byte XOR key=255:\n");
    char *msg2 = "TestData";
    len = str_len(msg2);
    xor_single(msg2, len, (char)255, encrypted);
    xor_single(encrypted, len, (char)255, decrypted);
    printf("  Roundtrip: %s\n", str_eq(msg2, decrypted) ? "OK" : "FAIL");

    // Multi-byte XOR
    printf("Multi-byte XOR key=\"KEY\":\n");
    char *msg3 = "ABCDEF";
    len = str_len(msg3);
    char *key1 = "KEY";
    int klen1 = str_len(key1);
    xor_multi(msg3, len, key1, klen1, encrypted);
    printf("  Original:  %s\n", msg3);
    xor_multi(encrypted, len, key1, klen1, decrypted);
    printf("  Decrypted: %s\n", decrypted);
    printf("  Roundtrip: %s\n", str_eq(msg3, decrypted) ? "OK" : "FAIL");

    // Multi-byte XOR with longer key
    printf("Multi-byte XOR key=\"SECRET\":\n");
    char *msg4 = "Attack at dawn!";
    len = str_len(msg4);
    char *key2 = "SECRET";
    int klen2 = str_len(key2);
    xor_multi(msg4, len, key2, klen2, encrypted);
    printf("  Original:  %s\n", msg4);
    xor_multi(encrypted, len, key2, klen2, decrypted);
    printf("  Decrypted: %s\n", decrypted);
    printf("  Roundtrip: %s\n", str_eq(msg4, decrypted) ? "OK" : "FAIL");

    // XOR properties
    printf("XOR properties:\n");
    char a = 'X';
    // A ^ 0 = A
    printf("  A ^ 0 = A: %s\n", (a ^ 0) == a ? "OK" : "FAIL");
    // A ^ A = 0
    printf("  A ^ A = 0: %s\n", (a ^ a) == 0 ? "OK" : "FAIL");
    // (A ^ B) ^ B = A
    char b = 'Y';
    printf("  (A^B)^B = A: %s\n", ((a ^ b) ^ b) == a ? "OK" : "FAIL");

    // Binary data test (non-printable bytes)
    printf("Binary data test:\n");
    char bindata[6];
    bindata[0] = 0;
    bindata[1] = 1;
    bindata[2] = 127;
    bindata[3] = 66;
    bindata[4] = 100;
    bindata[5] = '\0';
    char binenc[6];
    char bindec[6];
    xor_single(bindata, 5, 99, binenc);
    xor_single(binenc, 5, 99, bindec);
    int all_match = 1;
    int i;
    for (i = 0; i < 5; i++) {
        int orig = (int)(bindata[i]) & 0xFF;
        int dec = (int)(bindec[i]) & 0xFF;
        int ok = (orig == dec);
        printf("  byte %d: orig=%d dec=%d %s\n", i, orig, dec, ok ? "OK" : "FAIL");
        if (!ok) all_match = 0;
    }
    printf("All %d bytes match\n", all_match ? 5 : 0);

    // Different key lengths
    printf("Key length test:\n");
    char *testmsg = "ABCDEFGHIJ";
    int tlen = str_len(testmsg);

    // Key length 1
    xor_single(testmsg, tlen, 77, encrypted);
    xor_single(encrypted, tlen, 77, decrypted);
    decrypted[tlen] = '\0';
    printf("  key len 1 roundtrip: %s\n", str_eq(testmsg, decrypted) ? "OK" : "FAIL");

    // Key length 3
    char *k3 = "ABC";
    xor_multi(testmsg, tlen, k3, 3, encrypted);
    xor_multi(encrypted, tlen, k3, 3, decrypted);
    printf("  key len 3 roundtrip: %s\n", str_eq(testmsg, decrypted) ? "OK" : "FAIL");

    // Key length 7
    char *k7 = "LONGKEY";
    xor_multi(testmsg, tlen, k7, 7, encrypted);
    xor_multi(encrypted, tlen, k7, 7, decrypted);
    printf("  key len 7 roundtrip: %s\n", str_eq(testmsg, decrypted) ? "OK" : "FAIL");

    printf("All XOR cipher tests passed!\n");
    return 0;
}
