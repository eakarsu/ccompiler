int printf(const char *fmt, ...);

/* One-time pad simulation */

/* Simple linear congruential generator for pseudo-random pad */
int lcg_next(int state) {
    /* state = (state * 1103515245 + 12345) is too large, use smaller values */
    int r;
    r = (state * 37 + 17) % 256;
    if (r < 0) r = r + 256;
    return r;
}

/* Generate a pad of given length from a seed */
void generate_pad(int seed, int *pad, int len) {
    int i;
    int s;
    s = seed;
    i = 0;
    while (i < len) {
        s = lcg_next(s);
        pad[i] = s;
        i = i + 1;
    }
}

/* OTP encrypt: plaintext XOR pad */
void otp_encrypt(int *data, int *pad, int len) {
    int i;
    i = 0;
    while (i < len) {
        data[i] = data[i] ^ pad[i];
        i = i + 1;
    }
}

/* OTP decrypt: ciphertext XOR pad (same operation) */
void otp_decrypt(int *data, int *pad, int len) {
    otp_encrypt(data, pad, len);
}

/* Copy array */
void cp(int *d, int *s, int n) {
    int i;
    i = 0;
    while (i < n) { d[i] = s[i]; i = i + 1; }
}

/* Arrays equal */
int aeq(int *a, int *b, int n) {
    int i;
    i = 0;
    while (i < n) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return 1;
}

/* Count how many bytes differ between two arrays */
int count_diff(int *a, int *b, int n) {
    int c;
    int i;
    c = 0;
    i = 0;
    while (i < n) {
        if (a[i] != b[i]) c = c + 1;
        i = i + 1;
    }
    return c;
}

/* XOR two pads together */
void xor_pads(int *a, int *b, int *result, int len) {
    int i;
    i = 0;
    while (i < len) {
        result[i] = a[i] ^ b[i];
        i = i + 1;
    }
}

/* Count zero bytes in array */
int count_zeros(int *data, int len) {
    int c;
    int i;
    c = 0;
    i = 0;
    while (i < len) {
        if (data[i] == 0) c = c + 1;
        i = i + 1;
    }
    return c;
}

int main() {
    int pad1[8];
    int pad2[8];
    int data[8];
    int orig[8];
    int cipher1[8];
    int cipher2[8];
    int xored[8];
    int i;

    /* Test 1: Generate pad from seed 10 */
    /* lcg_next(10) = (10*37+17)%256 = 387%256 = 131
       lcg_next(131) = (131*37+17)%256 = 4864%256 = 4864-19*256=4864-4864=0
       lcg_next(0) = (0*37+17)%256 = 17
       lcg_next(17) = (17*37+17)%256 = 646%256 = 646-2*256=646-512=134
       lcg_next(134) = (134*37+17)%256 = 4975%256 = 4975-19*256=4975-4864=111
       lcg_next(111) = (111*37+17)%256 = 4124%256 = 4124-16*256=4124-4096=28
       lcg_next(28) = (28*37+17)%256 = 1053%256 = 1053-4*256=1053-1024=29
       lcg_next(29) = (29*37+17)%256 = 1090%256 = 1090-4*256=1090-1024=66 */
    generate_pad(10, pad1, 8);
    printf("pad1: %d %d %d %d %d %d %d %d\n",
           pad1[0], pad1[1], pad1[2], pad1[3], pad1[4], pad1[5], pad1[6], pad1[7]);
    // EXPECT: pad1: 131 0 17 134 111 28 29 66

    /* Test 2: Generate pad from different seed */
    /* lcg_next(20) = (20*37+17)%256 = 757%256 = 757-2*256=757-512=245
       lcg_next(245) = (245*37+17)%256 = 9082%256 = 9082-35*256=9082-8960=122
       lcg_next(122) = (122*37+17)%256 = 4531%256 = 4531-17*256=4531-4352=179
       lcg_next(179) = (179*37+17)%256 = 6640%256 = 6640-25*256=6640-6400=240
       lcg_next(240) = (240*37+17)%256 = 8897%256 = 8897-34*256=8897-8704=193
       lcg_next(193) = (193*37+17)%256 = 7158%256 = 7158-27*256=7158-6912=246
       lcg_next(246) = (246*37+17)%256 = 9119%256 = 9119-35*256=9119-8960=159
       lcg_next(159) = (159*37+17)%256 = 5900%256 = 5900-23*256=5900-5888=12 */
    generate_pad(20, pad2, 8);
    printf("pad2: %d %d %d %d %d %d %d %d\n",
           pad2[0], pad2[1], pad2[2], pad2[3], pad2[4], pad2[5], pad2[6], pad2[7]);
    // EXPECT: pad2: 245 122 179 240 193 246 159 12

    /* Test 3: Encrypt plaintext */
    data[0]=72; data[1]=101; data[2]=108; data[3]=108;
    data[4]=111; data[5]=33; data[6]=65; data[7]=66;
    cp(orig, data, 8);

    otp_encrypt(data, pad1, 8);
    /* 72^131: 72=01001000, 131=10000011, xor=11001011=203
       101^0 = 101
       108^17: 108=01101100, 17=00010001, xor=01111101=125
       108^134: 108=01101100, 134=10000110, xor=11101010=234
       111^111 = 0
       33^28: 33=00100001, 28=00011100, xor=00111101=61
       65^29: 65=01000001, 29=00011101, xor=01011100=92
       66^66 = 0 */
    printf("enc: %d %d %d %d %d %d %d %d\n",
           data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
    // EXPECT: enc: 203 101 125 234 0 61 92 0

    /* Test 4: Decrypt recovers plaintext */
    otp_decrypt(data, pad1, 8);
    printf("rt: %d\n", aeq(data, orig, 8));
    // EXPECT: rt: 1

    /* Test 5: Different pad produces different ciphertext */
    cp(data, orig, 8);
    otp_encrypt(data, pad2, 8);
    /* 72^245: 72=01001000, 245=11110101, xor=10111101=189
       101^122: 101=01100101, 122=01111010, xor=00011111=31
       108^179: 108=01101100, 179=10110011, xor=11011111=223
       108^240: 108=01101100, 240=11110000, xor=10011100=156
       111^193: 111=01101111, 193=11000001, xor=10101110=174
       33^246: 33=00100001, 246=11110110, xor=11010111=215
       65^159: 65=01000001, 159=10011111, xor=11011110=222
       66^12: 66=01000010, 12=00001100, xor=01001110=78 */
    printf("enc2: %d %d %d %d %d %d %d %d\n",
           data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
    // EXPECT: enc2: 189 31 223 156 174 215 222 78

    /* Test 6: Decrypt with pad2 recovers */
    otp_decrypt(data, pad2, 8);
    printf("rt2: %d\n", aeq(data, orig, 8));
    // EXPECT: rt2: 1

    /* Test 7: Reuse detection - XOR of two ciphertexts = XOR of two plaintexts
       if same pad is reused */
    cp(cipher1, orig, 8);
    otp_encrypt(cipher1, pad1, 8);
    /* cipher1 = [203,101,125,234,0,61,92,0] */

    /* Second plaintext */
    cipher2[0]=87; cipher2[1]=111; cipher2[2]=114; cipher2[3]=108;
    cipher2[4]=100; cipher2[5]=33; cipher2[6]=33; cipher2[7]=33;
    otp_encrypt(cipher2, pad1, 8);
    /* 87^131: 87=01010111, 131=10000011, xor=11010100=212
       111^0=111
       114^17: 114=01110010, 17=00010001, xor=01100011=99
       108^134=234
       100^111: 100=01100100, 111=01101111, xor=00001011=11
       33^28=61
       33^29: 33=00100001, 29=00011101, xor=00111100=60
       33^66: 33=00100001, 66=01000010, xor=01100011=99 */

    xor_pads(cipher1, cipher2, xored, 8);
    /* XOR of ciphertexts: 203^212, 101^111, 125^99, 234^234, 0^11, 61^61, 92^60, 0^99 */
    /* 203^212: 203=11001011, 212=11010100, xor=00011111=31
       101^111: 101=01100101, 111=01101111, xor=00001010=10
       125^99: 125=01111101, 99=01100011, xor=00011110=30
       234^234=0
       0^11=11
       61^61=0
       92^60: 92=01011100, 60=00111100, xor=01100000=96
       0^99=99 */
    printf("xor_c: %d %d %d %d %d %d %d %d\n",
           xored[0], xored[1], xored[2], xored[3], xored[4], xored[5], xored[6], xored[7]);
    // EXPECT: xor_c: 31 10 30 0 11 0 96 99

    /* This should equal XOR of the two plaintexts */
    /* plain1: 72,101,108,108,111,33,65,66
       plain2: 87,111,114,108,100,33,33,33
       72^87: 72=01001000, 87=01010111, xor=00011111=31
       101^111: 10, 108^114: 108=01101100, 114=01110010, xor=00011110=30
       108^108=0, 111^100=11, 33^33=0, 65^33=96, 66^33=99 */
    printf("reuse_leak: %d\n", xored[0] == (72^87));
    // EXPECT: reuse_leak: 1

    /* Test 8: Count zeros in ciphertexts */
    cp(data, orig, 8);
    otp_encrypt(data, pad1, 8);
    printf("zeros: %d\n", count_zeros(data, 8));
    // EXPECT: zeros: 2

    /* Test 9: Count differences between ciphertexts from different pads */
    cp(cipher1, orig, 8);
    otp_encrypt(cipher1, pad1, 8);
    cp(cipher2, orig, 8);
    otp_encrypt(cipher2, pad2, 8);
    printf("diff: %d\n", count_diff(cipher1, cipher2, 8));
    // EXPECT: diff: 8

    /* Test 10: Pads from same seed are identical */
    generate_pad(10, pad2, 8);
    printf("same_seed: %d\n", aeq(pad1, pad2, 8));
    // EXPECT: same_seed: 1

    printf("otp_done\n");
    // EXPECT: otp_done
    return 0;
}
