int printf(const char *fmt, ...);

/* HMAC-like construction: hash with key */

/* Simple hash function (DJB2-like) */
int simple_hash(int *data, int len) {
    int h;
    int i;
    h = 5381;
    i = 0;
    while (i < len) {
        h = ((h << 5) + h) + data[i];
        i = i + 1;
    }
    if (h < 0) h = 0 - h;
    return h % 10000;
}

/* XOR a block with a key (repeated) */
void xor_block(int *data, int len, int *key, int klen, int *out) {
    int i;
    i = 0;
    while (i < len) {
        out[i] = data[i] ^ key[i % klen];
        i = i + 1;
    }
}

/* HMAC-like: H(key ^ opad || H(key ^ ipad || message))
   Simplified: we use a fixed ipad=0x36 and opad=0x5C */
int hmac(int *key, int klen, int *msg, int mlen) {
    int ipad_key[8];
    int opad_key[8];
    int inner_input[16];
    int outer_input[12];
    int i;
    int inner_hash;
    int inner_data[1];

    /* Prepare ipad_key and opad_key (key XOR'd with pad bytes) */
    i = 0;
    while (i < 8) {
        if (i < klen) {
            ipad_key[i] = key[i] ^ 0x36;
            opad_key[i] = key[i] ^ 0x5C;
        } else {
            ipad_key[i] = 0x36;
            opad_key[i] = 0x5C;
        }
        i = i + 1;
    }

    /* Inner: H(ipad_key || message) */
    i = 0;
    while (i < 8) {
        inner_input[i] = ipad_key[i];
        i = i + 1;
    }
    i = 0;
    while (i < mlen && (i + 8) < 16) {
        inner_input[i + 8] = msg[i];
        i = i + 1;
    }
    inner_hash = simple_hash(inner_input, 8 + mlen);

    /* Outer: H(opad_key || inner_hash) */
    i = 0;
    while (i < 8) {
        outer_input[i] = opad_key[i];
        i = i + 1;
    }
    inner_data[0] = inner_hash;
    outer_input[8] = inner_hash;

    return simple_hash(outer_input, 9);
}

/* Verify HMAC: compute and compare */
int hmac_verify(int *key, int klen, int *msg, int mlen, int expected) {
    return hmac(key, klen, msg, mlen) == expected;
}

/* Simple key stretching: hash the key multiple rounds */
int key_stretch(int *key, int klen, int rounds) {
    int h;
    int data[1];
    int i;
    h = simple_hash(key, klen);
    i = 0;
    while (i < rounds) {
        data[0] = h;
        h = simple_hash(data, 1);
        i = i + 1;
    }
    return h;
}

int main() {
    int key1[4];
    int key2[4];
    int msg1[4];
    int msg2[4];
    int h1;
    int h2;
    int h3;
    int i;

    key1[0] = 10; key1[1] = 20; key1[2] = 30; key1[3] = 40;
    key2[0] = 50; key2[1] = 60; key2[2] = 70; key2[3] = 80;
    msg1[0] = 72; msg1[1] = 101; msg1[2] = 108; msg1[3] = 111;
    msg2[0] = 87; msg2[1] = 111; msg2[2] = 114; msg2[3] = 108;

    /* Test 1: Basic hash */
    printf("hash1: %d\n", simple_hash(msg1, 4));
    // EXPECT: hash1: 4733

    printf("hash2: %d\n", simple_hash(msg2, 4));
    // EXPECT: hash2: 4873

    /* Test 2: HMAC with key1 and msg1 */
    h1 = hmac(key1, 4, msg1, 4);
    printf("hmac1: %d\n", h1);
    // EXPECT: hmac1: 4738

    /* Test 3: Same key, different message -> different HMAC */
    h2 = hmac(key1, 4, msg2, 4);
    printf("hmac2: %d\n", h2);
    // EXPECT: hmac2: 4878

    printf("diff_msg: %d\n", h1 != h2 ? 1 : 0);
    // EXPECT: diff_msg: 1

    /* Test 4: Different key, same message -> different HMAC */
    h3 = hmac(key2, 4, msg1, 4);
    printf("hmac3: %d\n", h3);
    // EXPECT: hmac3: 8366

    printf("diff_key: %d\n", h1 != h3 ? 1 : 0);
    // EXPECT: diff_key: 1

    /* Test 5: HMAC verification */
    printf("verify_ok: %d\n", hmac_verify(key1, 4, msg1, 4, h1));
    // EXPECT: verify_ok: 1

    printf("verify_bad: %d\n", hmac_verify(key1, 4, msg1, 4, h1 + 1));
    // EXPECT: verify_bad: 0

    /* Test 6: Deterministic */
    printf("determ: %d\n", hmac(key1, 4, msg1, 4) == h1 ? 1 : 0);
    // EXPECT: determ: 1

    /* Test 7: Key stretching */
    h1 = key_stretch(key1, 4, 0);
    /* 0 rounds: just hash of key1 */
    printf("stretch0: %d\n", h1);
    // EXPECT: stretch0: 5785

    h2 = key_stretch(key1, 4, 1);
    /* 1 round: hash(key1), then hash that single value */
    printf("stretch1: %d\n", h2);
    // EXPECT: stretch1: 3358

    h3 = key_stretch(key1, 4, 5);
    printf("stretch5: %d\n", h3);
    // EXPECT: stretch5: 3650

    /* Test 8: More stretching rounds is different */
    printf("stretch_diff: %d\n", h2 != h3 ? 1 : 0);
    // EXPECT: stretch_diff: 1

    /* Test 9: HMAC with zero key */
    key1[0] = 0; key1[1] = 0; key1[2] = 0; key1[3] = 0;
    h1 = hmac(key1, 4, msg1, 4);
    printf("hmac_zk: %d\n", h1);
    // EXPECT: hmac_zk: 7928

    /* Test 10: HMAC with empty message (length 0) */
    key1[0] = 10; key1[1] = 20; key1[2] = 30; key1[3] = 40;
    h1 = hmac(key1, 4, msg1, 0);
    printf("hmac_empty: %d\n", h1);
    // EXPECT: hmac_empty: 3274

    /* Different from HMAC with full message */
    h2 = hmac(key1, 4, msg1, 4);
    printf("empty_diff: %d\n", h1 != h2 ? 1 : 0);
    // EXPECT: empty_diff: 1

    printf("hmac_done\n");
    // EXPECT: hmac_done
    return 0;
}
