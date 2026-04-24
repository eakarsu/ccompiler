int printf(const char *fmt, ...);

/* Vigenere cipher */

/* Encrypt a single letter with a shift */
int vig_enc(int plain, int key_shift) {
    return (plain + key_shift) % 26;
}

/* Decrypt a single letter with a shift */
int vig_dec(int cipher, int key_shift) {
    int r;
    r = (cipher - key_shift) % 26;
    if (r < 0) r = r + 26;
    return r;
}

/* Encrypt an array of letter indices (0-25) with a key */
void vigenere_encrypt(int *data, int dlen, int *key, int klen) {
    int i;
    i = 0;
    while (i < dlen) {
        data[i] = vig_enc(data[i], key[i % klen]);
        i = i + 1;
    }
}

/* Decrypt an array */
void vigenere_decrypt(int *data, int dlen, int *key, int klen) {
    int i;
    i = 0;
    while (i < dlen) {
        data[i] = vig_dec(data[i], key[i % klen]);
        i = i + 1;
    }
}

/* Copy array */
void copy_arr(int *dst, int *src, int n) {
    int i;
    i = 0;
    while (i < n) {
        dst[i] = src[i];
        i = i + 1;
    }
}

/* Check arrays equal */
int eq_arr(int *a, int *b, int n) {
    int i;
    i = 0;
    while (i < n) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return 1;
}

/* Frequency analysis: count occurrences of each letter */
void freq_count(int *data, int len, int *freq) {
    int i;
    i = 0;
    while (i < 26) {
        freq[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (i < len) {
        if (data[i] >= 0 && data[i] < 26) {
            freq[data[i]] = freq[data[i]] + 1;
        }
        i = i + 1;
    }
}

/* Find the most frequent letter */
int most_frequent(int *freq) {
    int best;
    int best_count;
    int i;
    best = 0;
    best_count = freq[0];
    i = 1;
    while (i < 26) {
        if (freq[i] > best_count) {
            best_count = freq[i];
            best = i;
        }
        i = i + 1;
    }
    return best;
}

/* Index of coincidence (scaled by 1000 to avoid float) */
int index_of_coincidence(int *data, int len) {
    int freq[26];
    int i;
    int num;
    freq_count(data, len, freq);
    num = 0;
    i = 0;
    while (i < 26) {
        num = num + freq[i] * (freq[i] - 1);
        i = i + 1;
    }
    if (len <= 1) return 0;
    /* return num * 1000 / (len * (len - 1)) */
    return (num * 1000) / (len * (len - 1));
}

int main() {
    int plain[12];
    int orig[12];
    int cipher[12];
    int key[4];
    int freq[26];
    int i;

    /* Key "KEY" = 10, 4, 24 */
    key[0] = 10; key[1] = 4; key[2] = 24;

    /* Plaintext "HELLOWORLD" = 7,4,11,11,14,22,14,17,11,3 (10 chars) */
    plain[0]=7; plain[1]=4; plain[2]=11; plain[3]=11; plain[4]=14;
    plain[5]=22; plain[6]=14; plain[7]=17; plain[8]=11; plain[9]=3;
    copy_arr(orig, plain, 10);

    /* Test 1: Single letter encrypt/decrypt */
    printf("enc_a: %d\n", vig_enc(0, 10));
    // EXPECT: enc_a: 10
    printf("enc_z: %d\n", vig_enc(25, 10));
    // EXPECT: enc_z: 9
    /* (25+10)%26 = 35%26 = 9 */
    printf("dec_10: %d\n", vig_dec(10, 10));
    // EXPECT: dec_10: 0
    printf("dec_9: %d\n", vig_dec(9, 10));
    // EXPECT: dec_9: 25
    /* (9-10)%26 = -1%26 ... in C, -1%26 = -1, then +26 = 25 */

    /* Test 2: Encrypt full plaintext */
    vigenere_encrypt(plain, 10, key, 3);
    /* i=0: (7+10)%26=17   key[0]=10
       i=1: (4+4)%26=8     key[1]=4
       i=2: (11+24)%26=9   key[2]=24
       i=3: (11+10)%26=21  key[0]=10
       i=4: (14+4)%26=18   key[1]=4
       i=5: (22+24)%26=20  key[2]=24
       i=6: (14+10)%26=24  key[0]=10
       i=7: (17+4)%26=21   key[1]=4
       i=8: (11+24)%26=9   key[2]=24
       i=9: (3+10)%26=13   key[0]=10 */
    printf("vig_enc: %d %d %d %d %d %d %d %d %d %d\n",
           plain[0], plain[1], plain[2], plain[3], plain[4],
           plain[5], plain[6], plain[7], plain[8], plain[9]);
    // EXPECT: vig_enc: 17 8 9 21 18 20 24 21 9 13

    /* Test 3: Decrypt recovers original */
    vigenere_decrypt(plain, 10, key, 3);
    printf("vig_rt: %d\n", eq_arr(plain, orig, 10));
    // EXPECT: vig_rt: 1

    /* Test 4: Frequency analysis of plaintext */
    freq_count(orig, 10, freq);
    /* Letters: 7,4,11,11,14,22,14,17,11,3
       freq[3]=1, freq[4]=1, freq[7]=1, freq[11]=3, freq[14]=2, freq[17]=1, freq[22]=1 */
    printf("freq_11: %d\n", freq[11]);
    // EXPECT: freq_11: 3
    printf("freq_14: %d\n", freq[14]);
    // EXPECT: freq_14: 2
    printf("most_freq: %d\n", most_frequent(freq));
    // EXPECT: most_freq: 11

    /* Test 5: Frequency of ciphertext */
    copy_arr(cipher, orig, 10);
    vigenere_encrypt(cipher, 10, key, 3);
    freq_count(cipher, 10, freq);
    /* cipher: 17,8,9,21,18,20,24,21,9,13
       freq[8]=1, freq[9]=2, freq[13]=1, freq[17]=1, freq[18]=1, freq[20]=1, freq[21]=2, freq[24]=1 */
    printf("cfreq_9: %d\n", freq[9]);
    // EXPECT: cfreq_9: 2
    printf("cfreq_21: %d\n", freq[21]);
    // EXPECT: cfreq_21: 2

    /* Test 6: Index of coincidence of plaintext */
    /* num = sum of freq[i]*(freq[i]-1) for plain:
       freq[3]=1: 0, freq[4]=1: 0, freq[7]=1: 0, freq[11]=3: 6, freq[14]=2: 2, freq[17]=1: 0, freq[22]=1: 0
       num = 8, len=10, denom=90
       IC = 8*1000/90 = 88 */
    printf("ic_plain: %d\n", index_of_coincidence(orig, 10));
    // EXPECT: ic_plain: 88

    /* Test 7: IC of ciphertext (should be lower, more uniform) */
    /* cipher freq: 8:1(0), 9:2(2), 13:1(0), 17:1(0), 18:1(0), 20:1(0), 21:2(2), 24:1(0)
       num = 4, IC = 4*1000/90 = 44 */
    printf("ic_cipher: %d\n", index_of_coincidence(cipher, 10));
    // EXPECT: ic_cipher: 44

    /* Test 8: Key of all zeros = identity */
    key[0] = 0; key[1] = 0; key[2] = 0;
    copy_arr(plain, orig, 10);
    vigenere_encrypt(plain, 10, key, 3);
    printf("zero_key: %d\n", eq_arr(plain, orig, 10));
    // EXPECT: zero_key: 1

    /* Test 9: Key length 1 = Caesar cipher */
    key[0] = 5;
    copy_arr(plain, orig, 10);
    vigenere_encrypt(plain, 10, key, 1);
    /* Every letter shifted by 5:
       (7+5)=12, (4+5)=9, (11+5)=16, (11+5)=16, (14+5)=19,
       (22+5)%26=1, (14+5)=19, (17+5)=22, (11+5)=16, (3+5)=8 */
    printf("caesar5: %d %d %d %d %d %d %d %d %d %d\n",
           plain[0], plain[1], plain[2], plain[3], plain[4],
           plain[5], plain[6], plain[7], plain[8], plain[9]);
    // EXPECT: caesar5: 12 9 16 16 19 1 19 22 16 8

    /* Test 10: Decrypt with same key recovers */
    vigenere_decrypt(plain, 10, key, 1);
    printf("caesar_rt: %d\n", eq_arr(plain, orig, 10));
    // EXPECT: caesar_rt: 1

    printf("vigenere_done\n");
    // EXPECT: vigenere_done
    return 0;
}
