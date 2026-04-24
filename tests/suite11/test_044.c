int printf(const char *fmt, ...);

// One-Time Pad (XOR cipher) - encryption and decryption

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) n = n + 1;
    return n;
}

// XOR encrypt/decrypt - same operation for both
void xor_cipher(char *input, int *key, int len, char *output) {
    int i;
    for (i = 0; i < len; i++) {
        int val = (int)input[i] ^ key[i];
        output[i] = (char)val;
    }
    output[len] = 0;
}

// Generate a pseudo-random key from a seed
void generate_key(int seed, int len, int *key) {
    int state = seed;
    int i;
    for (i = 0; i < len; i++) {
        state = state * 1103515245 + 12345;
        int val = state;
        if (val < 0) val = -val;
        key[i] = (val % 95) + 32; // printable ASCII range
    }
}

// Check if XOR is its own inverse
int verify_self_inverse(char *original, int *key, int len) {
    char encrypted[128];
    char decrypted[128];

    xor_cipher(original, key, len, encrypted);
    xor_cipher(encrypted, key, len, decrypted);

    int i;
    for (i = 0; i < len; i++) {
        if (decrypted[i] != original[i]) return 0;
    }
    return 1;
}

// XOR two byte arrays
void xor_arrays(int *a, int *b, int len, int *result) {
    int i;
    for (i = 0; i < len; i++) {
        result[i] = a[i] ^ b[i];
    }
}

// Verify XOR properties: associativity and commutativity
int verify_xor_properties(int a, int b, int c) {
    // Commutativity: a ^ b == b ^ a
    if ((a ^ b) != (b ^ a)) return 0;
    // Associativity: (a ^ b) ^ c == a ^ (b ^ c)
    if (((a ^ b) ^ c) != (a ^ (b ^ c))) return 0;
    // Self inverse: a ^ a == 0
    if ((a ^ a) != 0) return 0;
    // Identity: a ^ 0 == a
    if ((a ^ 0) != a) return 0;
    return 1;
}

// Simple stream cipher using XOR with PRNG
struct StreamState {
    int state;
    int counter;
};

void stream_init(struct StreamState *ss, int seed) {
    ss->state = seed;
    ss->counter = 0;
}

int stream_next_byte(struct StreamState *ss) {
    ss->state = ss->state * 1103515245 + 12345;
    ss->counter = ss->counter + 1;
    int val = ss->state;
    if (val < 0) val = -val;
    return val % 256;
}

void stream_encrypt(char *input, int len, int seed, int *output) {
    struct StreamState ss;
    stream_init(&ss, seed);
    int i;
    for (i = 0; i < len; i++) {
        int kb = stream_next_byte(&ss);
        output[i] = (int)input[i] ^ kb;
    }
}

void stream_decrypt(int *input, int len, int seed, char *output) {
    struct StreamState ss;
    stream_init(&ss, seed);
    int i;
    for (i = 0; i < len; i++) {
        int kb = stream_next_byte(&ss);
        output[i] = (char)(input[i] ^ kb);
    }
    output[len] = 0;
}

int main() {
    // Test 1: XOR properties
    int props_ok = verify_xor_properties(42, 137, 255);
    printf("XOR properties: %d\n", props_ok);
    // EXPECT: XOR properties: 1

    // Test 2: simple XOR encrypt/decrypt
    char *msg1 = "HELLO";
    int key1[5];
    key1[0] = 10;
    key1[1] = 20;
    key1[2] = 30;
    key1[3] = 40;
    key1[4] = 50;
    int selfInv = verify_self_inverse(msg1, key1, 5);
    printf("Self inverse: %d\n", selfInv);
    // EXPECT: Self inverse: 1

    // Test 3: XOR with zero key is identity
    int zerokey[5];
    zerokey[0] = 0; zerokey[1] = 0; zerokey[2] = 0; zerokey[3] = 0; zerokey[4] = 0;
    char out[128];
    xor_cipher(msg1, zerokey, 5, out);
    int is_identity = 1;
    int i;
    for (i = 0; i < 5; i++) {
        if (out[i] != msg1[i]) is_identity = 0;
    }
    printf("Zero key identity: %d\n", is_identity);
    // EXPECT: Zero key identity: 1

    // Test 4: key reuse vulnerability - XOR of two ciphertexts = XOR of two plaintexts
    char *msg2 = "WORLD";
    int key2[5];
    generate_key(42, 5, key2);
    char cipher1[128];
    char cipher2[128];
    xor_cipher(msg1, key2, 5, cipher1);
    xor_cipher(msg2, key2, 5, cipher2);

    // cipher1 ^ cipher2 should equal msg1 ^ msg2
    int ct_arr[5];
    int pt_arr1[5];
    for (i = 0; i < 5; i++) {
        ct_arr[i] = (int)cipher1[i] ^ (int)cipher2[i];
        pt_arr1[i] = (int)msg1[i] ^ (int)msg2[i];
    }
    int vulnerability = 1;
    for (i = 0; i < 5; i++) {
        if (ct_arr[i] != pt_arr1[i]) vulnerability = 0;
    }
    printf("Key reuse vulnerability: %d\n", vulnerability);
    // EXPECT: Key reuse vulnerability: 1

    // Test 5: stream cipher encrypt/decrypt
    char *msg3 = "SECRETMSG";
    int slen = str_len(msg3);
    int encrypted[64];
    char decrypted[64];
    stream_encrypt(msg3, slen, 12345, encrypted);
    stream_decrypt(encrypted, slen, 12345, decrypted);

    int match = 1;
    for (i = 0; i < slen; i++) {
        if (decrypted[i] != msg3[i]) match = 0;
    }
    printf("Stream cipher round trip: %d\n", match);
    // EXPECT: Stream cipher round trip: 1

    // Test 6: different seeds produce different ciphertexts
    int enc1[64];
    int enc2[64];
    stream_encrypt(msg3, slen, 100, enc1);
    stream_encrypt(msg3, slen, 200, enc2);
    int differ = 0;
    for (i = 0; i < slen; i++) {
        if (enc1[i] != enc2[i]) differ = 1;
    }
    printf("Different seeds differ: %d\n", differ);
    // EXPECT: Different seeds differ: 1

    // Test 7: stream counter
    struct StreamState ss;
    stream_init(&ss, 0);
    for (i = 0; i < 10; i++) {
        stream_next_byte(&ss);
    }
    printf("Stream counter after 10: %d\n", ss.counter);
    // EXPECT: Stream counter after 10: 10

    // Test 8: XOR bit counts
    int xval = 170 ^ 85; // 10101010 ^ 01010101 = 11111111
    printf("XOR 170 85: %d\n", xval);
    // EXPECT: XOR 170 85: 255

    printf("OTP XOR tests passed\n");
    // EXPECT: OTP XOR tests passed

    return 0;
}
