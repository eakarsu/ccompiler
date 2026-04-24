int printf(const char *fmt, ...);

// HMAC Simulation - keyed hash for message authentication

struct HState {
    int h0;
    int h1;
    int h2;
    int h3;
};

void hinit(struct HState *hs) {
    hs->h0 = 1732584193;
    hs->h1 = -271733879;
    hs->h2 = -1732584194;
    hs->h3 = 271733878;
}

void hprocess(struct HState *hs, int bval) {
    int t0 = hs->h0 + bval;
    t0 = (t0 << 5) | ((t0 >> 27) & 31);
    t0 = t0 ^ hs->h1;

    int t1 = hs->h1 + (bval * 31);
    t1 = (t1 << 7) | ((t1 >> 25) & 127);
    t1 = t1 ^ hs->h2;

    int t2 = hs->h2 + (bval * 127);
    t2 = (t2 << 11) | ((t2 >> 21) & 2047);
    t2 = t2 ^ hs->h3;

    int t3 = hs->h3 + (bval * 8191);
    t3 = (t3 << 13) | ((t3 >> 19) & 8191);
    t3 = t3 ^ t0;

    hs->h0 = t0;
    hs->h1 = t1;
    hs->h2 = t2;
    hs->h3 = t3;
}

int hfinish(struct HState *hs) {
    return hs->h0 ^ hs->h1 ^ hs->h2 ^ hs->h3;
}

int hash_bytes(int *data, int len) {
    struct HState hs;
    hinit(&hs);
    int i;
    for (i = 0; i < len; i++) {
        hprocess(&hs, data[i]);
    }
    hprocess(&hs, len); // length padding
    return hfinish(&hs);
}

// HMAC(key, message) = hash(key ^ opad || hash(key ^ ipad || message))
int hmac_compute(int *key, int klen, int *msg, int mlen) {
    int ipad_val = 54; // 0x36
    int opad_val = 92; // 0x5C
    int block_size = 16;

    // Prepare key block (pad or truncate key to block_size)
    int key_block[16];
    int i;
    for (i = 0; i < block_size; i++) {
        if (i < klen) {
            key_block[i] = key[i];
        } else {
            key_block[i] = 0;
        }
    }

    // Inner hash: hash(key ^ ipad || message)
    int inner_data[64];
    int inner_len = 0;
    for (i = 0; i < block_size; i++) {
        inner_data[inner_len] = key_block[i] ^ ipad_val;
        inner_len = inner_len + 1;
    }
    for (i = 0; i < mlen; i++) {
        inner_data[inner_len] = msg[i];
        inner_len = inner_len + 1;
    }
    int inner_hash = hash_bytes(inner_data, inner_len);

    // Outer hash: hash(key ^ opad || inner_hash)
    int outer_data[32];
    int outer_len = 0;
    for (i = 0; i < block_size; i++) {
        outer_data[outer_len] = key_block[i] ^ opad_val;
        outer_len = outer_len + 1;
    }
    // Break inner_hash into 4 bytes
    outer_data[outer_len] = inner_hash & 255;
    outer_len = outer_len + 1;
    outer_data[outer_len] = (inner_hash >> 8) & 255;
    outer_len = outer_len + 1;
    outer_data[outer_len] = (inner_hash >> 16) & 255;
    outer_len = outer_len + 1;
    outer_data[outer_len] = (inner_hash >> 24) & 255;
    outer_len = outer_len + 1;

    return hash_bytes(outer_data, outer_len);
}

int verify_hmac(int *key, int klen, int *msg, int mlen, int expected_hmac) {
    int computed = hmac_compute(key, klen, msg, mlen);
    return computed == expected_hmac;
}

int main() {
    int key1[4];
    key1[0] = 65; key1[1] = 66; key1[2] = 67; key1[3] = 68; // "ABCD"

    int msg1[5];
    msg1[0] = 72; msg1[1] = 69; msg1[2] = 76; msg1[3] = 76; msg1[4] = 79; // "HELLO"

    // Test 1: compute HMAC
    int hmac1 = hmac_compute(key1, 4, msg1, 5);
    printf("HMAC computed: %d\n", hmac1 != 0);
    // EXPECT: HMAC computed: 1

    // Test 2: same inputs produce same HMAC
    int hmac2 = hmac_compute(key1, 4, msg1, 5);
    printf("HMAC deterministic: %d\n", hmac1 == hmac2);
    // EXPECT: HMAC deterministic: 1

    // Test 3: different message produces different HMAC
    int msg2[5];
    msg2[0] = 72; msg2[1] = 69; msg2[2] = 76; msg2[3] = 76; msg2[4] = 80; // "HELLP"
    int hmac3 = hmac_compute(key1, 4, msg2, 5);
    printf("Diff msg diff HMAC: %d\n", hmac1 != hmac3);
    // EXPECT: Diff msg diff HMAC: 1

    // Test 4: different key produces different HMAC
    int key2[4];
    key2[0] = 88; key2[1] = 89; key2[2] = 90; key2[3] = 87; // "XYZW"
    int hmac4 = hmac_compute(key2, 4, msg1, 5);
    printf("Diff key diff HMAC: %d\n", hmac1 != hmac4);
    // EXPECT: Diff key diff HMAC: 1

    // Test 5: verification succeeds with correct HMAC
    int ver_ok = verify_hmac(key1, 4, msg1, 5, hmac1);
    printf("Verify correct: %d\n", ver_ok);
    // EXPECT: Verify correct: 1

    // Test 6: verification fails with wrong HMAC
    int ver_fail = verify_hmac(key1, 4, msg1, 5, hmac1 + 1);
    printf("Verify wrong: %d\n", ver_fail);
    // EXPECT: Verify wrong: 0

    // Test 7: verification fails with tampered message
    int ver_tamper = verify_hmac(key1, 4, msg2, 5, hmac1);
    printf("Verify tampered: %d\n", ver_tamper);
    // EXPECT: Verify tampered: 0

    // Test 8: HMAC is not just hash of message (key matters)
    int plain_hash = hash_bytes(msg1, 5);
    printf("HMAC differs from hash: %d\n", hmac1 != plain_hash);
    // EXPECT: HMAC differs from hash: 1

    // Test 9: empty message still produces valid HMAC
    int empty_msg[1];
    empty_msg[0] = 0;
    int hmac_empty = hmac_compute(key1, 4, empty_msg, 0);
    printf("Empty msg HMAC nonzero: %d\n", hmac_empty != 0);
    // EXPECT: Empty msg HMAC nonzero: 1

    // Test 10: key length variation
    int short_key[1];
    short_key[0] = 42;
    int hmac_short = hmac_compute(short_key, 1, msg1, 5);
    int long_key[8];
    int i;
    for (i = 0; i < 8; i++) long_key[i] = 42 + i;
    int hmac_long = hmac_compute(long_key, 8, msg1, 5);
    printf("Key length matters: %d\n", hmac_short != hmac_long);
    // EXPECT: Key length matters: 1

    // Test 11: batch verification
    int all_ok = 1;
    int j;
    for (j = 0; j < 5; j++) {
        int test_msg[3];
        test_msg[0] = j + 10;
        test_msg[1] = j + 20;
        test_msg[2] = j + 30;
        int mac = hmac_compute(key1, 4, test_msg, 3);
        if (verify_hmac(key1, 4, test_msg, 3, mac) != 1) {
            all_ok = 0;
        }
    }
    printf("Batch verify: %d\n", all_ok);
    // EXPECT: Batch verify: 1

    printf("HMAC simulation tests passed\n");
    // EXPECT: HMAC simulation tests passed

    return 0;
}
