// stress_049_bitwise_crypto.c - Bitwise operations as crypto primitives
// EXPECT: xor_roundtrip=1
// EXPECT: xor_cipher=72
// EXPECT: rotl=0x9A000003
// EXPECT: rotr=0xE0000004
// EXPECT: sbox_lookup=119
// EXPECT: permute=0xAA
// EXPECT: feistel_rounds=1
// EXPECT: crc=0xCBF43926
// EXPECT: lfsr_period=65535
// EXPECT: bit_reverse=0xB4
// EXPECT: popcount=5
// EXPECT: parity=1

int printf(const char *fmt, ...);

// --- XOR cipher ---
void xor_cipher(char *data, int len, char key) {
    int i;
    for (i = 0; i < len; i++) {
        data[i] = data[i] ^ key;
    }
}

// --- Bit rotation (32-bit) ---
unsigned int rotl32(unsigned int val, int n) {
    n = n & 31;
    return (val << n) | (val >> (32 - n));
}

unsigned int rotr32(unsigned int val, int n) {
    n = n & 31;
    return (val >> n) | (val << (32 - n));
}

// --- Simple S-box (substitution box) ---
// A fixed permutation lookup table (first 16 entries)
int sbox[16];

void init_sbox(void) {
    sbox[0] = 14;  sbox[1] = 4;   sbox[2] = 13;  sbox[3] = 1;
    sbox[4] = 2;   sbox[5] = 15;  sbox[6] = 11;  sbox[7] = 8;
    sbox[8] = 3;   sbox[9] = 10;  sbox[10] = 6;  sbox[11] = 12;
    sbox[12] = 5;  sbox[13] = 9;  sbox[14] = 0;  sbox[15] = 7;
}

// Apply sbox to each nibble of a byte
int sbox_byte(int val) {
    int lo = val & 0x0F;
    int hi = (val >> 4) & 0x0F;
    return (sbox[hi] << 4) | sbox[lo];
}

// --- Bit permutation: extract bits and rearrange ---
// Reverse bit order of a byte
int permute_byte(int val) {
    int result = 0;
    int i;
    for (i = 0; i < 8; i++) {
        result = result | (((val >> i) & 1) << (7 - i));
    }
    return result;
}

// --- Feistel network round ---
unsigned int feistel_f(unsigned int half, unsigned int key) {
    // Simple round function: rotate, xor with key, add constant
    unsigned int t = rotl32(half, 5);
    t = t ^ key;
    t = t + 0x9E3779B9;  // golden ratio constant
    return t;
}

void feistel_encrypt(unsigned int *left, unsigned int *right, unsigned int *keys, int rounds) {
    int i;
    for (i = 0; i < rounds; i++) {
        unsigned int new_left = *right;
        unsigned int new_right = *left ^ feistel_f(*right, keys[i]);
        *left = new_left;
        *right = new_right;
    }
}

void feistel_decrypt(unsigned int *left, unsigned int *right, unsigned int *keys, int rounds) {
    int i;
    for (i = rounds - 1; i >= 0; i--) {
        unsigned int new_right = *left;
        unsigned int new_left = *right ^ feistel_f(*left, keys[i]);
        *left = new_left;
        *right = new_right;
    }
}

// --- CRC-32 (bit-at-a-time implementation) ---
unsigned int crc32_byte(unsigned int crc, unsigned int byte) {
    int i;
    crc = crc ^ byte;
    for (i = 0; i < 8; i++) {
        if (crc & 1)
            crc = (crc >> 1) ^ 0xEDB88320;
        else
            crc = crc >> 1;
    }
    return crc;
}

unsigned int crc32(char *data, int len) {
    unsigned int crc = 0xFFFFFFFF;
    int i;
    for (i = 0; i < len; i++) {
        crc = crc32_byte(crc, (unsigned int)(unsigned char)data[i]);
    }
    return crc ^ 0xFFFFFFFF;
}

// --- LFSR (16-bit, taps at 16,15,13,4 -> maximal length) ---
// Actually using taps at bits 16,14,13,11 for maximal period
unsigned int lfsr16_step(unsigned int state) {
    // Galois LFSR with polynomial x^16 + x^14 + x^13 + x^11 + 1
    // Feedback polynomial = 0xB400
    unsigned int bit = state & 1;
    state = state >> 1;
    if (bit) {
        state = state ^ 0xB400;
    }
    return state;
}

// --- Bit reverse of a byte ---
int bit_reverse_byte(int val) {
    int result = 0;
    int i;
    for (i = 0; i < 8; i++) {
        if (val & (1 << i))
            result = result | (1 << (7 - i));
    }
    return result;
}

// --- Population count (number of set bits) ---
int popcount32(unsigned int val) {
    int count = 0;
    while (val) {
        count = count + (val & 1);
        val = val >> 1;
    }
    return count;
}

// --- Parity (1 if odd number of bits set) ---
int parity32(unsigned int val) {
    return popcount32(val) & 1;
}

int main(void) {
    // --- XOR cipher roundtrip ---
    {
        char msg[6];
        msg[0] = 'H'; msg[1] = 'e'; msg[2] = 'l'; msg[3] = 'l'; msg[4] = 'o'; msg[5] = 0;
        char key = 0x5A;
        xor_cipher(msg, 5, key);
        // Encrypted
        xor_cipher(msg, 5, key);
        // Decrypted back to "Hello"
        int ok = (msg[0] == 'H') && (msg[1] == 'e') && (msg[2] == 'l')
              && (msg[3] == 'l') && (msg[4] == 'o');
        printf("xor_roundtrip=%d\n", ok);
    }

    // --- XOR cipher specific value ---
    {
        // 'H' = 0x48 = 72, XOR with 0x5A = 0x12 = 18
        char c = 'H';
        c = c ^ 0x5A;  // encrypted
        c = c ^ 0x5A;  // decrypted
        printf("xor_cipher=%d\n", (int)(unsigned char)c);  // back to 72
    }

    // --- Bit rotations ---
    {
        // rotl32(0xCD000001, 1) = 0x9A000003
        unsigned int v1 = 0xCD000001;
        printf("rotl=0x%X\n", rotl32(v1, 1));

        // rotr32(0xC0000009, 1) = 0xE0000004
        unsigned int v2 = 0xC0000009;
        printf("rotr=0x%X\n", rotr32(v2, 1));
    }

    // --- S-box lookup ---
    {
        init_sbox();
        // sbox_byte(0xAB):
        // hi nibble A=10: sbox[10]=6
        // lo nibble B=11: sbox[11]=12
        // result = (6 << 4) | 12 = 96 + 12 = 108...
        // Actually let me pick input 0x73:
        // hi=7: sbox[7]=8, lo=3: sbox[3]=1, result = (8<<4)|1 = 129
        // Let me try 0x0F: hi=0: sbox[0]=14, lo=15: sbox[15]=7, result = (14<<4)|7 = 224+7=231
        // Let me just print sbox_byte(0x0E):
        // hi=0: sbox[0]=14, lo=14: sbox[14]=0, result = (14<<4)|0 = 224
        // This is hard to predict. Let me just compute and use:
        // sbox_byte(0x71): hi=7: sbox[7]=8, lo=1: sbox[1]=4, result=(8<<4)|4=132
        // Let me use input where I can easily verify:
        // input 0x00: sbox[0]=14, sbox[0]=14, result=(14<<4)|14 = 238
        // For 119: 119 = 0x77 = 7*16+7. So I need (sbox[h]<<4)|sbox[l] = 0x77
        // sbox[h]=7 -> h=15: sbox[15]=7. sbox[l]=7 -> l=15: sbox[15]=7.
        // So input 0xFF gives output (7<<4)|7 = 119.
        printf("sbox_lookup=%d\n", sbox_byte(0xFF));
    }

    // --- Bit permutation (reverse byte) ---
    {
        // permute_byte(0x55) = reverse of 01010101 = 10101010 = 0xAA = 170
        printf("permute=0x%X\n", permute_byte(0x55));
    }

    // --- Feistel network: encrypt then decrypt, verify roundtrip ---
    {
        unsigned int left = 0xDEADBEEF;
        unsigned int right = 0xCAFEBABE;
        unsigned int orig_l = left;
        unsigned int orig_r = right;
        unsigned int keys[4];
        keys[0] = 0x01234567;
        keys[1] = 0x89ABCDEF;
        keys[2] = 0xFEDCBA98;
        keys[3] = 0x76543210;

        feistel_encrypt(&left, &right, keys, 4);
        feistel_decrypt(&left, &right, keys, 4);

        int ok = (left == orig_l) && (right == orig_r);
        printf("feistel_rounds=%d\n", ok);
    }

    // --- CRC-32 of "123456789" (standard test vector) ---
    {
        char test[10];
        test[0]='1'; test[1]='2'; test[2]='3'; test[3]='4'; test[4]='5';
        test[5]='6'; test[6]='7'; test[7]='8'; test[8]='9'; test[9]=0;
        unsigned int c = crc32(test, 9);
        printf("crc=0x%X\n", c);  // Standard CRC-32 of "123456789" = 0xCBF43926
    }

    // --- LFSR period test (16-bit maximal LFSR has period 2^16-1 = 65535) ---
    {
        unsigned int state = 1;  // seed
        unsigned int initial = state;
        int count = 0;
        do {
            state = lfsr16_step(state);
            count = count + 1;
        } while (state != initial && count < 70000);
        printf("lfsr_period=%d\n", count);
    }

    // --- Bit reverse ---
    {
        // 0x2D = 00101101, reversed = 10110100 = 0xB4
        printf("bit_reverse=0x%X\n", bit_reverse_byte(0x2D));
    }

    // --- Population count ---
    {
        // 0x1D = 00011101, popcount = 4... let me recount: bits 0,2,3,4 = 4 set bits
        // For 5: 0x1F = 00011111, popcount = 5
        printf("popcount=%d\n", popcount32(0x1F));
    }

    // --- Parity ---
    {
        // 0x1F has 5 set bits -> parity = 1 (odd)
        printf("parity=%d\n", parity32(0x1F));
    }

    return 0;
}
