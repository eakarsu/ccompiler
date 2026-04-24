int printf(const char *fmt, ...);

/* XOR cipher with key schedule */

/* Generate a simple key schedule from a seed using linear operations */
void key_schedule(int seed, int *keys, int nkeys) {
    int i;
    int k;
    k = seed;
    i = 0;
    while (i < nkeys) {
        k = (k * 7 + 13) % 256;
        keys[i] = k;
        i = i + 1;
    }
}

/* XOR encrypt a block of data using the key schedule */
void xor_encrypt(int *data, int len, int *keys, int nkeys) {
    int i;
    i = 0;
    while (i < len) {
        data[i] = data[i] ^ keys[i % nkeys];
        i = i + 1;
    }
}

/* XOR decrypt is the same as encrypt */
void xor_decrypt(int *data, int len, int *keys, int nkeys) {
    xor_encrypt(data, len, keys, nkeys);
}

/* Compute a simple checksum */
int checksum(int *data, int len) {
    int sum;
    int i;
    sum = 0;
    i = 0;
    while (i < len) {
        sum = sum + data[i];
        i = i + 1;
    }
    return sum;
}

/* Check if two arrays are equal */
int arrays_equal(int *a, int *b, int len) {
    int i;
    i = 0;
    while (i < len) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return 1;
}

/* Copy array */
void copy_array(int *dst, int *src, int len) {
    int i;
    i = 0;
    while (i < len) {
        dst[i] = src[i];
        i = i + 1;
    }
}

/* Rotate key schedule by offset */
void rotate_keys(int *keys, int nkeys, int offset, int *out) {
    int i;
    i = 0;
    while (i < nkeys) {
        out[i] = keys[(i + offset) % nkeys];
        i = i + 1;
    }
}

int main() {
    int keys[8];
    int data[8];
    int original[8];
    int rotated[8];
    int i;

    /* Test 1: key schedule from seed 5 */
    /* k=5
       i=0: k=(5*7+13)%256 = 48%256 = 48
       i=1: k=(48*7+13)%256 = 349%256 = 93
       i=2: k=(93*7+13)%256 = 664%256 = 152
       i=3: k=(152*7+13)%256 = 1077%256 = 1077-4*256=1077-1024=53
       i=4: k=(53*7+13)%256 = 384%256 = 128
       i=5: k=(128*7+13)%256 = 909%256 = 909-3*256=909-768=141
       i=6: k=(141*7+13)%256 = 1000%256 = 1000-3*256=1000-768=232
       i=7: k=(232*7+13)%256 = 1637%256 = 1637-6*256=1637-1536=101
    */
    key_schedule(5, keys, 8);
    printf("keys: %d %d %d %d %d %d %d %d\n", keys[0], keys[1], keys[2], keys[3], keys[4], keys[5], keys[6], keys[7]);
    // EXPECT: keys: 48 93 152 53 128 141 232 101

    /* Test 2: Encrypt plaintext with key schedule */
    data[0] = 72;  /* H */
    data[1] = 101; /* e */
    data[2] = 108; /* l */
    data[3] = 108; /* l */
    data[4] = 111; /* o */
    data[5] = 33;  /* ! */
    data[6] = 65;  /* A */
    data[7] = 66;  /* B */
    copy_array(original, data, 8);

    xor_encrypt(data, 8, keys, 8);
    /* 72^48: 72=01001000, 48=00110000, xor=01111000=120
       101^93: 101=01100101, 93=01011101, xor=00111000=56
       108^152: 108=01101100, 152=10011000, xor=11110100=244
       108^53: 108=01101100, 53=00110101, xor=01011001=89
       111^128: 111=01101111, 128=10000000, xor=11101111=239
       33^141: 33=00100001, 141=10001101, xor=10101100=172
       65^232: 65=01000001, 232=11101000, xor=10101001=169
       66^101: 66=01000010, 101=01100101, xor=00100111=39 */
    printf("enc: %d %d %d %d %d %d %d %d\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
    // EXPECT: enc: 120 56 244 89 239 172 169 39

    /* Test 3: Decrypt recovers original */
    xor_decrypt(data, 8, keys, 8);
    printf("roundtrip: %d\n", arrays_equal(data, original, 8));
    // EXPECT: roundtrip: 1

    /* Test 4: Checksums */
    printf("orig_cksum: %d\n", checksum(original, 8));
    // EXPECT: orig_cksum: 664
    /* 72+101+108+108+111+33+65+66 = 664 */

    copy_array(data, original, 8);
    xor_encrypt(data, 8, keys, 8);
    printf("enc_cksum: %d\n", checksum(data, 8));
    // EXPECT: enc_cksum: 1128
    /* 120+56+244+89+239+172+169+39 = 1128 */

    /* Test 5: Use simple manual keys */
    keys[0] = 10; keys[1] = 20; keys[2] = 30; keys[3] = 40;
    keys[4] = 50; keys[5] = 60; keys[6] = 70; keys[7] = 80;
    copy_array(data, original, 8);
    xor_encrypt(data, 8, keys, 8);
    /* 72^10=66, 101^20=113, 108^30=114, 108^40=68, 111^50=93, 33^60=29, 65^70=7, 66^80=18 */
    /* Check 65^70: 65=01000001, 70=01000110, xor=00000111=7 */
    /* Check 66^80: 66=01000010, 80=01010000, xor=00010010=18 */
    printf("enc2: %d %d %d %d %d %d %d %d\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
    // EXPECT: enc2: 66 113 114 68 93 29 7 18

    /* Test 6: Rotate keys */
    rotate_keys(keys, 8, 3, rotated);
    /* rotated: keys[3..7, 0..2] = 40,50,60,70,80,10,20,30 */
    printf("rot: %d %d %d %d %d %d %d %d\n", rotated[0], rotated[1], rotated[2], rotated[3], rotated[4], rotated[5], rotated[6], rotated[7]);
    // EXPECT: rot: 40 50 60 70 80 10 20 30

    /* Test 7: Double encrypt with different key sets, then double decrypt */
    copy_array(data, original, 8);
    xor_encrypt(data, 8, keys, 8);
    xor_encrypt(data, 8, rotated, 8);
    /* First: 66,113,114,68,93,29,7,18
       Then: 66^40=106, 113^50=67, 114^60=78, 68^70=2, 93^80=13, 29^10=23, 7^20=19, 18^30=12 */
    /* Check 114^60: 114=01110010, 60=00111100, xor=01001110=78 */
    /* Check 68^70: 68=01000100, 70=01000110, xor=00000010=2 */
    /* Check 93^80: 93=01011101, 80=01010000, xor=00001101=13 */
    /* Check 29^10: 29=00011101, 10=00001010, xor=00010111=23 */
    /* Check 7^20: 7=00000111, 20=00010100, xor=00010011=19 */
    /* Check 18^30: 18=00010010, 30=00011110, xor=00001100=12 */
    printf("dbl: %d %d %d %d %d %d %d %d\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
    // EXPECT: dbl: 106 67 78 2 13 23 19 12

    xor_decrypt(data, 8, rotated, 8);
    xor_decrypt(data, 8, keys, 8);
    printf("dbl_rt: %d\n", arrays_equal(data, original, 8));
    // EXPECT: dbl_rt: 1

    /* Test 8: XOR with zero key is identity */
    copy_array(data, original, 8);
    i = 0;
    while (i < 8) { keys[i] = 0; i = i + 1; }
    xor_encrypt(data, 8, keys, 8);
    printf("zero_key: %d\n", arrays_equal(data, original, 8));
    // EXPECT: zero_key: 1

    /* Test 9: XOR with 0xFF flips bits */
    copy_array(data, original, 8);
    i = 0;
    while (i < 8) { keys[i] = 255; i = i + 1; }
    xor_encrypt(data, 8, keys, 8);
    /* 72^255=183, 101^255=154, 108^255=147, 108^255=147,
       111^255=144, 33^255=222, 65^255=190, 66^255=189 */
    printf("flip: %d %d %d %d %d %d %d %d\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
    // EXPECT: flip: 183 154 147 147 144 222 190 189

    /* Test 10: Double flip recovers original */
    xor_encrypt(data, 8, keys, 8);
    printf("dbl_flip: %d\n", arrays_equal(data, original, 8));
    // EXPECT: dbl_flip: 1

    printf("xor_cipher_done\n");
    // EXPECT: xor_cipher_done
    return 0;
}
