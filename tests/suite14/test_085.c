int printf(const char *fmt, ...);

/* Simple block cipher: Feistel network, 2 rounds */

/* Round function */
int feistel_f(int half, int rkey) {
    int x;
    x = half ^ rkey;
    x = ((x * 7) + 13) & 0xFFFF;
    x = x ^ ((x >> 5) & 0x7FF);
    return x & 0xFFFF;
}

/* One Feistel round: L' = R, R' = L ^ F(R, K) */
void feistel_round(int *left, int *right, int rkey) {
    int new_left;
    int new_right;
    new_left = *right;
    new_right = (*left ^ feistel_f(*right, rkey)) & 0xFFFF;
    *left = new_left;
    *right = new_right;
}

/* Encrypt: apply rounds with K1, K2 (no final swap) */
void feistel_encrypt(int *left, int *right, int k1, int k2) {
    feistel_round(left, right, k1);
    feistel_round(left, right, k2);
}

/* Decrypt: swap L,R, apply rounds with K2, K1, swap back */
void feistel_decrypt(int *left, int *right, int k1, int k2) {
    int temp;
    /* Swap input */
    temp = *left; *left = *right; *right = temp;
    /* Apply rounds in reverse key order */
    feistel_round(left, right, k2);
    feistel_round(left, right, k1);
    /* Swap output */
    temp = *left; *left = *right; *right = temp;
}

/* Derive round keys from master key */
void derive_keys(int master, int *k1, int *k2) {
    *k1 = (master * 7 + 11) & 0xFFFF;
    *k2 = (master * 13 + 37) & 0xFFFF;
}

int main() {
    int left;
    int right;
    int orig_l;
    int orig_r;
    int k1;
    int k2;
    int i;
    int blocks_l[4];
    int blocks_r[4];
    int enc1_l;
    int enc1_r;

    /* Test 1: Round function */
    printf("f_0_0: %d\n", feistel_f(0, 0));
    // EXPECT: f_0_0: 13

    printf("f_1_0: %d\n", feistel_f(1, 0));
    // EXPECT: f_1_0: 20

    printf("f_10_5: %d\n", feistel_f(10, 5));
    // EXPECT: f_10_5: 117

    printf("f_100_50: %d\n", feistel_f(100, 50));
    // EXPECT: f_100_50: 628

    /* Test 2: Key derivation */
    derive_keys(42, &k1, &k2);
    printf("keys: %d %d\n", k1, k2);
    // EXPECT: keys: 305 583

    /* Test 3: Encrypt and decrypt roundtrip */
    k1 = 5; k2 = 7;
    left = 10; right = 20;
    orig_l = 10; orig_r = 20;

    feistel_encrypt(&left, &right, k1, k2);
    printf("enc_ne: %d\n", (left != 10 || right != 20) ? 1 : 0);
    // EXPECT: enc_ne: 1

    feistel_decrypt(&left, &right, k1, k2);
    printf("dec: %d %d\n", left, right);
    // EXPECT: dec: 10 20

    printf("rt: %d\n", (left == orig_l && right == orig_r) ? 1 : 0);
    // EXPECT: rt: 1

    /* Test 4: Zero block roundtrip */
    left = 0; right = 0;
    feistel_encrypt(&left, &right, k1, k2);
    printf("enc_z_ne: %d\n", (left != 0 || right != 0) ? 1 : 0);
    // EXPECT: enc_z_ne: 1

    feistel_decrypt(&left, &right, k1, k2);
    printf("dec_zero: %d %d\n", left, right);
    // EXPECT: dec_zero: 0 0

    /* Test 5: Different keys produce different ciphertext */
    left = 10; right = 20;
    feistel_encrypt(&left, &right, 5, 7);
    enc1_l = left; enc1_r = right;

    left = 10; right = 20;
    feistel_encrypt(&left, &right, 8, 3);
    printf("diff_keys: %d\n", (enc1_l != left || enc1_r != right) ? 1 : 0);
    // EXPECT: diff_keys: 1

    /* Test 6: Encrypt/decrypt multiple blocks */
    blocks_l[0] = 100; blocks_r[0] = 200;
    blocks_l[1] = 300; blocks_r[1] = 400;
    blocks_l[2] = 500; blocks_r[2] = 600;
    blocks_l[3] = 0;   blocks_r[3] = 0;

    i = 0;
    while (i < 4) {
        feistel_encrypt(&blocks_l[i], &blocks_r[i], 5, 7);
        i = i + 1;
    }

    printf("changed: %d\n", (blocks_l[0] != 100 || blocks_r[0] != 200) ? 1 : 0);
    // EXPECT: changed: 1

    i = 0;
    while (i < 4) {
        feistel_decrypt(&blocks_l[i], &blocks_r[i], 5, 7);
        i = i + 1;
    }
    printf("multi: %d %d %d %d %d %d %d %d\n",
           blocks_l[0], blocks_r[0], blocks_l[1], blocks_r[1],
           blocks_l[2], blocks_r[2], blocks_l[3], blocks_r[3]);
    // EXPECT: multi: 100 200 300 400 500 600 0 0

    /* Test 7: Avalanche effect */
    left = 10; right = 20;
    feistel_encrypt(&left, &right, 5, 7);
    enc1_l = left; enc1_r = right;

    left = 11; right = 20;
    feistel_encrypt(&left, &right, 5, 7);
    printf("aval: %d\n", (enc1_l != left || enc1_r != right) ? 1 : 0);
    // EXPECT: aval: 1

    /* Test 8: Master key roundtrip */
    derive_keys(100, &k1, &k2);
    printf("mk: %d %d\n", k1, k2);
    // EXPECT: mk: 711 1337

    left = 1000; right = 2000;
    feistel_encrypt(&left, &right, k1, k2);
    feistel_decrypt(&left, &right, k1, k2);
    printf("mk_rt: %d %d\n", left, right);
    // EXPECT: mk_rt: 1000 2000

    /* Test 9: Deterministic */
    left = 42; right = 84;
    feistel_encrypt(&left, &right, 5, 7);
    enc1_l = left; enc1_r = right;
    left = 42; right = 84;
    feistel_encrypt(&left, &right, 5, 7);
    printf("determ: %d\n", (left == enc1_l && right == enc1_r) ? 1 : 0);
    // EXPECT: determ: 1

    /* Test 10: Large value roundtrip */
    left = 65535; right = 65535;
    feistel_encrypt(&left, &right, 12345, 54321);
    feistel_decrypt(&left, &right, 12345, 54321);
    printf("large_rt: %d %d\n", left, right);
    // EXPECT: large_rt: 65535 65535

    printf("feistel_done\n");
    // EXPECT: feistel_done
    return 0;
}
