int printf(const char *fmt, ...);

/* XOR cipher: symmetric encryption using XOR with a key */

int str_len(char *s) {
    int n;
    n = 0;
    while (s[n]) n = n + 1;
    return n;
}

int str_eq(char *a, char *b) {
    int i;
    i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return (a[i] == 0 && b[i] == 0);
}

/* XOR cipher with single-byte key, stores result as int array to avoid null issues */
void xor_encrypt_bytes(char *input, int *output, int len, int key) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        output[i] = input[i] ^ key;
    }
}

void xor_decrypt_bytes(int *input, char *output, int len, int key) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        output[i] = input[i] ^ key;
    }
    output[len] = 0;
}

/* Multi-byte key XOR */
void xor_multi_encrypt(char *input, int *output, int len, char *key, int keylen) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        output[i] = input[i] ^ key[i % keylen];
    }
}

void xor_multi_decrypt(int *input, char *output, int len, char *key, int keylen) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        output[i] = input[i] ^ key[i % keylen];
    }
    output[len] = 0;
}

void test_single_byte_xor(void) {
    char *msg;
    int encrypted[64];
    char decrypted[64];
    int len;
    int key;
    int match;
    msg = "hello";
    len = str_len(msg);
    key = 42;
    xor_encrypt_bytes(msg, encrypted, len, key);
    /* Encrypted bytes should differ from original */
    // EXPECT: encrypted differs: 1
    printf("encrypted differs: %d\n", encrypted[0] != 104);
    xor_decrypt_bytes(encrypted, decrypted, len, key);
    match = str_eq(decrypted, "hello");
    // EXPECT: single byte roundtrip: 1
    printf("single byte roundtrip: %d\n", match);
}

void test_xor_properties(void) {
    int a;
    int b;
    a = 170;
    b = 85;
    // EXPECT: xor self=0
    printf("xor self=%d\n", a ^ a);
    // EXPECT: xor double=170
    printf("xor double=%d\n", (a ^ b) ^ b);
    // EXPECT: xor commutative=1
    printf("xor commutative=%d\n", (a ^ b) == (b ^ a));
}

void test_multi_key(void) {
    char *msg;
    int encrypted[64];
    char decrypted[64];
    int len;
    char *key;
    int keylen;
    int match;
    msg = "attack at dawn";
    len = str_len(msg);
    key = "secret";
    keylen = str_len(key);
    xor_multi_encrypt(msg, encrypted, len, key, keylen);
    xor_multi_decrypt(encrypted, decrypted, len, key, keylen);
    match = str_eq(decrypted, "attack at dawn");
    // EXPECT: multi key roundtrip: 1
    printf("multi key roundtrip: %d\n", match);
}

void test_known_values(void) {
    /* XOR of 'A' (65) with key 0xFF (255) */
    int result;
    result = 65 ^ 255;
    // EXPECT: A xor FF=190
    printf("A xor FF=%d\n", result);
    result = result ^ 255;
    // EXPECT: back to A=65
    printf("back to A=%d\n", result);
}

void test_zero_key(void) {
    char *msg;
    int encrypted[64];
    char decrypted[64];
    int len;
    int match;
    msg = "unchanged";
    len = str_len(msg);
    xor_encrypt_bytes(msg, encrypted, len, 0);
    xor_decrypt_bytes(encrypted, decrypted, len, 0);
    match = str_eq(decrypted, "unchanged");
    // EXPECT: zero key preserves: 1
    printf("zero key preserves: %d\n", match);
    /* With key 0, encrypted should equal original bytes */
    // EXPECT: zero key no change: 1
    printf("zero key no change: %d\n", encrypted[0] == 117);
}

void test_all_keys(void) {
    char *msg;
    int encrypted[64];
    char decrypted[64];
    int len;
    int key;
    int all_ok;
    msg = "test";
    len = str_len(msg);
    all_ok = 1;
    for (key = 1; key < 256; key = key + 1) {
        xor_encrypt_bytes(msg, encrypted, len, key);
        xor_decrypt_bytes(encrypted, decrypted, len, key);
        if (!str_eq(decrypted, "test")) {
            all_ok = 0;
        }
    }
    // EXPECT: all 255 keys roundtrip: 1
    printf("all 255 keys roundtrip: %d\n", all_ok);
}

void test_different_messages(void) {
    char *msgs[4];
    int encrypted[64];
    char decrypted[64];
    int key;
    int i;
    int all_ok;
    msgs[0] = "short";
    msgs[1] = "a longer message here";
    msgs[2] = "12345";
    msgs[3] = "MiXeD CaSe";
    key = 137;
    all_ok = 1;
    for (i = 0; i < 4; i = i + 1) {
        int len;
        len = str_len(msgs[i]);
        xor_encrypt_bytes(msgs[i], encrypted, len, key);
        xor_decrypt_bytes(encrypted, decrypted, len, key);
        if (!str_eq(decrypted, msgs[i])) {
            all_ok = 0;
        }
    }
    // EXPECT: different messages ok: 1
    printf("different messages ok: %d\n", all_ok);
}

int main(void) {
    test_single_byte_xor();
    test_xor_properties();
    test_multi_key();
    test_known_values();
    test_zero_key();
    test_all_keys();
    test_different_messages();
    // EXPECT: done
    printf("done\n");
    return 0;
}
