// EXPECT: === Gray Code ===\nGray codes for 4 bits:\n0: 0000\n1: 0001\n2: 0011\n3: 0010\n4: 0110\n5: 0111\n6: 0101\n7: 0100\n8: 1100\n9: 1101\n10: 1111\n11: 1110\n12: 1010\n13: 1011\n14: 1001\n15: 1000\nGray decode test:\nGray(5)=7, decode(7)=5\nGray(10)=15, decode(15)=10\nGray(13)=11, decode(11)=13\nGray roundtrip OK\n=== Hamming Code (7,4) ===\nData: 1011\nEncoded: 0110011\nSyndrome (no error): 0\nFlip bit 4:\nSyndrome: 4\nCorrected: 0110011\nDecoded data: 1011\nHamming roundtrip OK\n=== Parity Bits ===\nData: 10110100\nEven parity: 0\nOdd parity: 1\nWith parity: 101101000\nParity check (no error): OK\nFlipped bit -> parity check: FAIL\n=== Checksum ===\nData: 10 20 30 40 50\nSimple sum checksum: 150\n8-bit checksum: 150\nVerify checksum: OK\nComplement checksum: 105\nVerify complement: OK\n=== CRC Simulation ===\nData: 1101011\nGenerator: 1011\nCRC remainder: 110\nData with CRC: 1101011110\nVerify CRC (no error): 0\nCorrupt bit 2:\nVerify CRC (error): nonzero\n=== Error Detection ===\nOriginal:  65 66 67 68 69\nChecksum: 335\nTransmit and verify: OK\nCorrupt byte 2 (67->99):\nNew checksum: 367\nDetected: error\nRepetition code (3x):\nEncode 5 bits: 111000111111000\nDecode: 10110\nRepetition roundtrip OK\n=== Binary to BCD ===\n255 -> BCD: 0010 0101 0101\n42 -> BCD: 0100 0010\n100 -> BCD: 0001 0000 0000\n=== Two's Complement ===\n5 in 8-bit: 00000101\n-5 in 8-bit: 11111011\nVerify: -5 + 5 = 0 (mod 256): 1\nDone.\n
int printf(const char *fmt, ...);

// ---- Gray Code Generation ----

int gray_encode(int n) {
    return n ^ (n >> 1);
}

int gray_decode(int g) {
    int n;
    n = g;
    g = g >> 1;
    while (g > 0) {
        n = n ^ g;
        g = g >> 1;
    }
    return n;
}

void print_binary(int val, int bits) {
    int i;
    for (i = bits - 1; i >= 0; i = i - 1) {
        printf("%d", (val >> i) & 1);
    }
}

// ---- Generate all Gray codes for n bits ----

void generate_gray_codes(int num_bits) {
    int count;
    int i;
    count = 1 << num_bits;
    for (i = 0; i < count; i = i + 1) {
        printf("%d: ", i);
        print_binary(gray_encode(i), num_bits);
        printf("\n");
    }
}

// ---- Hamming Code (7,4) ----
// 4 data bits -> 7 coded bits
// Positions: p1 p2 d1 p3 d2 d3 d4
// p1 covers bits 1,3,5,7
// p2 covers bits 2,3,6,7
// p3 covers bits 4,5,6,7

int hamming_encoded[8];

void hamming_encode(int d1, int d2, int d3, int d4) {
    int p1;
    int p2;
    int p3;

    // Positions (1-indexed): 1=p1, 2=p2, 3=d1, 4=p3, 5=d2, 6=d3, 7=d4
    p1 = d1 ^ d2 ^ d4;
    p2 = d1 ^ d3 ^ d4;
    p3 = d2 ^ d3 ^ d4;

    hamming_encoded[1] = p1;
    hamming_encoded[2] = p2;
    hamming_encoded[3] = d1;
    hamming_encoded[4] = p3;
    hamming_encoded[5] = d2;
    hamming_encoded[6] = d3;
    hamming_encoded[7] = d4;
}

int hamming_syndrome() {
    int s1;
    int s2;
    int s3;

    s1 = hamming_encoded[1] ^ hamming_encoded[3] ^ hamming_encoded[5] ^ hamming_encoded[7];
    s2 = hamming_encoded[2] ^ hamming_encoded[3] ^ hamming_encoded[6] ^ hamming_encoded[7];
    s3 = hamming_encoded[4] ^ hamming_encoded[5] ^ hamming_encoded[6] ^ hamming_encoded[7];

    return s1 * 1 + s2 * 2 + s3 * 4;
}

void hamming_correct() {
    int err_pos;
    err_pos = hamming_syndrome();
    if (err_pos > 0 && err_pos <= 7) {
        hamming_encoded[err_pos] = hamming_encoded[err_pos] ^ 1;
    }
}

void hamming_decode(int *d1, int *d2, int *d3, int *d4) {
    *d1 = hamming_encoded[3];
    *d2 = hamming_encoded[5];
    *d3 = hamming_encoded[6];
    *d4 = hamming_encoded[7];
}

// ---- Parity Bits ----

int count_ones(int *bits, int len) {
    int i;
    int cnt;
    cnt = 0;
    for (i = 0; i < len; i = i + 1) {
        if (bits[i]) {
            cnt = cnt + 1;
        }
    }
    return cnt;
}

int even_parity(int *bits, int len) {
    return count_ones(bits, len) % 2;
}

int odd_parity(int *bits, int len) {
    return 1 - (count_ones(bits, len) % 2);
}

// ---- Checksum ----

int simple_checksum(int *data, int len) {
    int i;
    int sum;
    sum = 0;
    for (i = 0; i < len; i = i + 1) {
        sum = sum + data[i];
    }
    return sum;
}

int checksum_8bit(int *data, int len) {
    int i;
    int sum;
    sum = 0;
    for (i = 0; i < len; i = i + 1) {
        sum = sum + data[i];
    }
    return sum & 255;
}

int complement_checksum(int *data, int len) {
    int sum;
    sum = simple_checksum(data, len);
    return (~sum) & 255;
}

int verify_complement(int *data, int len, int checksum) {
    int sum;
    sum = simple_checksum(data, len);
    sum = (sum + checksum) & 255;
    return sum == 255;
}

// ---- CRC Simulation (integer-based) ----
// Working with bit arrays for clarity

int crc_data[32];
int crc_gen[32];
int crc_work[32];
int crc_result[32];

void crc_init_work(int *data, int data_len, int gen_len) {
    int i;
    // Copy data to work buffer, pad with zeros for CRC
    for (i = 0; i < data_len; i = i + 1) {
        crc_work[i] = data[i];
    }
    for (i = data_len; i < data_len + gen_len - 1; i = i + 1) {
        crc_work[i] = 0;
    }
}

void crc_compute(int *data, int data_len, int *gen, int gen_len) {
    int total_len;
    int i;
    int j;

    total_len = data_len + gen_len - 1;
    crc_init_work(data, data_len, gen_len);

    for (i = 0; i <= total_len - gen_len; i = i + 1) {
        if (crc_work[i] == 1) {
            for (j = 0; j < gen_len; j = j + 1) {
                crc_work[i + j] = crc_work[i + j] ^ gen[j];
            }
        }
    }

    // Remainder is in last gen_len-1 bits
    for (i = 0; i < gen_len - 1; i = i + 1) {
        crc_result[i] = crc_work[data_len + i];
    }
}

int crc_verify(int *data_with_crc, int total_len, int *gen, int gen_len) {
    int i;
    int j;
    int remainder;

    // Copy to work buffer
    for (i = 0; i < total_len; i = i + 1) {
        crc_work[i] = data_with_crc[i];
    }

    for (i = 0; i <= total_len - gen_len; i = i + 1) {
        if (crc_work[i] == 1) {
            for (j = 0; j < gen_len; j = j + 1) {
                crc_work[i + j] = crc_work[i + j] ^ gen[j];
            }
        }
    }

    // Check if remainder is zero
    remainder = 0;
    for (i = total_len - gen_len + 1; i < total_len; i = i + 1) {
        if (crc_work[i] != 0) {
            remainder = 1;
        }
    }
    return remainder;
}

// ---- Error Detection with simple checksum ----

int ed_data[32];
int ed_len;
int ed_checksum;

void ed_compute(int *data, int len) {
    ed_checksum = simple_checksum(data, len);
}

int ed_verify(int *data, int len, int checksum) {
    return simple_checksum(data, len) == checksum;
}

// ---- Repetition Code ----

int rep_encoded[128];
int rep_decoded[64];

void rep_encode(int *bits, int len, int rep) {
    int i;
    int j;
    int idx;
    idx = 0;
    for (i = 0; i < len; i = i + 1) {
        for (j = 0; j < rep; j = j + 1) {
            rep_encoded[idx] = bits[i];
            idx = idx + 1;
        }
    }
}

void rep_decode(int *encoded, int len, int rep) {
    int i;
    int j;
    int cnt;
    int idx;
    idx = 0;
    i = 0;
    while (i < len) {
        cnt = 0;
        for (j = 0; j < rep && i + j < len; j = j + 1) {
            cnt = cnt + encoded[i + j];
        }
        if (cnt > rep / 2) {
            rep_decoded[idx] = 1;
        } else {
            rep_decoded[idx] = 0;
        }
        idx = idx + 1;
        i = i + rep;
    }
}

// ---- Binary to BCD ----

void int_to_bcd(int val, int *bcd, int *bcd_len) {
    int digits[8];
    int num_digits;
    int i;
    int j;
    int d;

    // Extract decimal digits
    num_digits = 0;
    if (val == 0) {
        digits[0] = 0;
        num_digits = 1;
    } else {
        while (val > 0) {
            digits[num_digits] = val % 10;
            num_digits = num_digits + 1;
            val = val / 10;
        }
    }

    // Convert each digit to 4-bit BCD (MSB first)
    *bcd_len = 0;
    for (i = num_digits - 1; i >= 0; i = i - 1) {
        d = digits[i];
        for (j = 3; j >= 0; j = j - 1) {
            bcd[*bcd_len] = (d >> j) & 1;
            *bcd_len = *bcd_len + 1;
        }
    }
}

void print_bcd(int *bcd, int bcd_len) {
    int i;
    for (i = 0; i < bcd_len; i = i + 1) {
        printf("%d", bcd[i]);
        if ((i + 1) % 4 == 0 && i + 1 < bcd_len) {
            printf(" ");
        }
    }
}

// ---- Two's Complement ----

int twos_complement(int val, int bits) {
    int mask;
    mask = (1 << bits) - 1;
    if (val >= 0) {
        return val & mask;
    }
    // For negative: invert and add 1
    return ((~(-val)) + 1) & mask;
}

void print_twos_complement(int val, int bits) {
    int tc;
    tc = twos_complement(val, bits);
    print_binary(tc, bits);
}

// ---- Bit interleaving (Morton code) ----

int interleave_bits(int x, int y) {
    int result;
    int i;
    result = 0;
    for (i = 0; i < 8; i = i + 1) {
        result = result | (((x >> i) & 1) << (2 * i));
        result = result | (((y >> i) & 1) << (2 * i + 1));
    }
    return result;
}

void deinterleave_bits(int z, int *x, int *y) {
    int i;
    *x = 0;
    *y = 0;
    for (i = 0; i < 8; i = i + 1) {
        *x = *x | (((z >> (2 * i)) & 1) << i);
        *y = *y | (((z >> (2 * i + 1)) & 1) << i);
    }
}

// ---- Bit reversal ----

int reverse_bits(int val, int bits) {
    int result;
    int i;
    result = 0;
    for (i = 0; i < bits; i = i + 1) {
        result = result | (((val >> i) & 1) << (bits - 1 - i));
    }
    return result;
}

// ---- Hamming distance ----

int hamming_distance(int a, int b) {
    int xor_val;
    int dist;
    xor_val = a ^ b;
    dist = 0;
    while (xor_val > 0) {
        dist = dist + (xor_val & 1);
        xor_val = xor_val >> 1;
    }
    return dist;
}

// ---- Popcount ----

int popcount(int val) {
    int cnt;
    cnt = 0;
    while (val > 0) {
        cnt = cnt + (val & 1);
        val = val >> 1;
    }
    return cnt;
}

// ---- Leading zeros ----

int leading_zeros(int val, int bits) {
    int i;
    int cnt;
    cnt = 0;
    for (i = bits - 1; i >= 0; i = i - 1) {
        if (((val >> i) & 1) == 0) {
            cnt = cnt + 1;
        } else {
            return cnt;
        }
    }
    return cnt;
}

// ---- Trailing zeros ----

int trailing_zeros(int val, int bits) {
    int i;
    int cnt;
    cnt = 0;
    for (i = 0; i < bits; i = i + 1) {
        if (((val >> i) & 1) == 0) {
            cnt = cnt + 1;
        } else {
            return cnt;
        }
    }
    return cnt;
}

int main() {
    int i;
    int g;
    int d;
    int ok;
    int syndrome;
    int d1;
    int d2;
    int d3;
    int d4;
    int parity_data[8];
    int p;
    int data[8];
    int ck;
    int crc_d[8];
    int crc_g[4];
    int crc_full[16];
    int total_len;
    int ver;
    int ed[8];
    int bits[8];
    int bcd[32];
    int bcd_len;
    int tc;

    // Gray Code
    printf("=== Gray Code ===\n");
    printf("Gray codes for 4 bits:\n");
    generate_gray_codes(4);

    printf("Gray decode test:\n");
    g = gray_encode(5);
    d = gray_decode(g);
    printf("Gray(5)=%d, decode(%d)=%d\n", g, g, d);
    g = gray_encode(10);
    d = gray_decode(g);
    printf("Gray(10)=%d, decode(%d)=%d\n", g, g, d);
    g = gray_encode(13);
    d = gray_decode(g);
    printf("Gray(13)=%d, decode(%d)=%d\n", g, g, d);

    // Verify roundtrip for all 4-bit values
    ok = 1;
    for (i = 0; i < 16; i = i + 1) {
        if (gray_decode(gray_encode(i)) != i) {
            ok = 0;
        }
    }
    if (ok) {
        printf("Gray roundtrip OK\n");
    } else {
        printf("Gray roundtrip FAIL\n");
    }

    // Hamming Code
    printf("=== Hamming Code (7,4) ===\n");
    // Data: 1011
    printf("Data: 1011\n");
    hamming_encode(1, 0, 1, 1);
    printf("Encoded: ");
    for (i = 1; i <= 7; i = i + 1) {
        printf("%d", hamming_encoded[i]);
    }
    printf("\n");

    syndrome = hamming_syndrome();
    printf("Syndrome (no error): %d\n", syndrome);

    // Introduce error at bit 4
    printf("Flip bit 4:\n");
    hamming_encoded[4] = hamming_encoded[4] ^ 1;
    syndrome = hamming_syndrome();
    printf("Syndrome: %d\n", syndrome);

    hamming_correct();
    printf("Corrected: ");
    for (i = 1; i <= 7; i = i + 1) {
        printf("%d", hamming_encoded[i]);
    }
    printf("\n");

    hamming_decode(&d1, &d2, &d3, &d4);
    printf("Decoded data: %d%d%d%d\n", d1, d2, d3, d4);

    if (d1 == 1 && d2 == 0 && d3 == 1 && d4 == 1) {
        printf("Hamming roundtrip OK\n");
    } else {
        printf("Hamming roundtrip FAIL\n");
    }

    // Parity bits
    printf("=== Parity Bits ===\n");
    parity_data[0]=1; parity_data[1]=0; parity_data[2]=1; parity_data[3]=1;
    parity_data[4]=0; parity_data[5]=1; parity_data[6]=0; parity_data[7]=0;
    printf("Data: ");
    for (i = 0; i < 8; i = i + 1) {
        printf("%d", parity_data[i]);
    }
    printf("\n");

    p = even_parity(parity_data, 8);
    printf("Even parity: %d\n", p);
    printf("Odd parity: %d\n", odd_parity(parity_data, 8));

    // Append even parity bit
    printf("With parity: ");
    for (i = 0; i < 8; i = i + 1) {
        printf("%d", parity_data[i]);
    }
    printf("%d\n", p);

    // Verify parity
    {
        int check_data[9];
        for (i = 0; i < 8; i = i + 1) {
            check_data[i] = parity_data[i];
        }
        check_data[8] = p;
        if (even_parity(check_data, 9) == 0) {
            printf("Parity check (no error): OK\n");
        } else {
            printf("Parity check (no error): FAIL\n");
        }
        // Flip a bit
        check_data[3] = 1 - check_data[3];
        if (even_parity(check_data, 9) == 0) {
            printf("Flipped bit -> parity check: OK\n");
        } else {
            printf("Flipped bit -> parity check: FAIL\n");
        }
    }

    // Checksum
    printf("=== Checksum ===\n");
    data[0]=10; data[1]=20; data[2]=30; data[3]=40; data[4]=50;
    printf("Data: %d %d %d %d %d\n", data[0], data[1], data[2], data[3], data[4]);
    printf("Simple sum checksum: %d\n", simple_checksum(data, 5));
    printf("8-bit checksum: %d\n", checksum_8bit(data, 5));

    // Verify
    ck = simple_checksum(data, 5);
    if (ed_verify(data, 5, ck)) {
        printf("Verify checksum: OK\n");
    } else {
        printf("Verify checksum: FAIL\n");
    }

    // Complement checksum
    ck = complement_checksum(data, 5);
    printf("Complement checksum: %d\n", ck);
    if (verify_complement(data, 5, ck)) {
        printf("Verify complement: OK\n");
    } else {
        printf("Verify complement: FAIL\n");
    }

    // CRC simulation
    printf("=== CRC Simulation ===\n");
    // Data: 1101011, Generator: 1011
    crc_d[0]=1; crc_d[1]=1; crc_d[2]=0; crc_d[3]=1; crc_d[4]=0; crc_d[5]=1; crc_d[6]=1;
    crc_g[0]=1; crc_g[1]=0; crc_g[2]=1; crc_g[3]=1;

    printf("Data: ");
    for (i = 0; i < 7; i = i + 1) {
        printf("%d", crc_d[i]);
    }
    printf("\n");
    printf("Generator: ");
    for (i = 0; i < 4; i = i + 1) {
        printf("%d", crc_g[i]);
    }
    printf("\n");

    crc_compute(crc_d, 7, crc_g, 4);
    printf("CRC remainder: ");
    for (i = 0; i < 3; i = i + 1) {
        printf("%d", crc_result[i]);
    }
    printf("\n");

    // Build full message with CRC
    for (i = 0; i < 7; i = i + 1) {
        crc_full[i] = crc_d[i];
    }
    for (i = 0; i < 3; i = i + 1) {
        crc_full[7 + i] = crc_result[i];
    }
    total_len = 10;

    printf("Data with CRC: ");
    for (i = 0; i < total_len; i = i + 1) {
        printf("%d", crc_full[i]);
    }
    printf("\n");

    ver = crc_verify(crc_full, total_len, crc_g, 4);
    printf("Verify CRC (no error): %d\n", ver);

    // Corrupt a bit
    printf("Corrupt bit 2:\n");
    crc_full[2] = 1 - crc_full[2];
    ver = crc_verify(crc_full, total_len, crc_g, 4);
    if (ver != 0) {
        printf("Verify CRC (error): nonzero\n");
    } else {
        printf("Verify CRC (error): zero\n");
    }

    // Error detection with checksum
    printf("=== Error Detection ===\n");
    ed[0]=65; ed[1]=66; ed[2]=67; ed[3]=68; ed[4]=69;
    printf("Original:  ");
    for (i = 0; i < 5; i = i + 1) {
        if (i > 0) printf(" ");
        printf("%d", ed[i]);
    }
    printf("\n");

    ck = simple_checksum(ed, 5);
    printf("Checksum: %d\n", ck);

    if (ed_verify(ed, 5, ck)) {
        printf("Transmit and verify: OK\n");
    } else {
        printf("Transmit and verify: FAIL\n");
    }

    // Corrupt
    printf("Corrupt byte 2 (67->99):\n");
    ed[2] = 99;
    printf("New checksum: %d\n", simple_checksum(ed, 5));
    if (ed_verify(ed, 5, ck)) {
        printf("Detected: ok\n");
    } else {
        printf("Detected: error\n");
    }

    // Repetition code
    printf("Repetition code (3x):\n");
    bits[0]=1; bits[1]=0; bits[2]=1; bits[3]=1; bits[4]=0;
    rep_encode(bits, 5, 3);
    printf("Encode 5 bits: ");
    for (i = 0; i < 15; i = i + 1) {
        printf("%d", rep_encoded[i]);
    }
    printf("\n");

    rep_decode(rep_encoded, 15, 3);
    printf("Decode: ");
    for (i = 0; i < 5; i = i + 1) {
        printf("%d", rep_decoded[i]);
    }
    printf("\n");

    ok = 1;
    for (i = 0; i < 5; i = i + 1) {
        if (rep_decoded[i] != bits[i]) ok = 0;
    }
    if (ok) {
        printf("Repetition roundtrip OK\n");
    } else {
        printf("Repetition roundtrip FAIL\n");
    }

    // Binary to BCD
    printf("=== Binary to BCD ===\n");
    int_to_bcd(255, bcd, &bcd_len);
    printf("255 -> BCD: ");
    print_bcd(bcd, bcd_len);
    printf("\n");

    int_to_bcd(42, bcd, &bcd_len);
    printf("42 -> BCD: ");
    print_bcd(bcd, bcd_len);
    printf("\n");

    int_to_bcd(100, bcd, &bcd_len);
    printf("100 -> BCD: ");
    print_bcd(bcd, bcd_len);
    printf("\n");

    // Two's complement
    printf("=== Two's Complement ===\n");
    printf("5 in 8-bit: ");
    print_twos_complement(5, 8);
    printf("\n");

    printf("-5 in 8-bit: ");
    print_twos_complement(-5, 8);
    printf("\n");

    tc = twos_complement(5, 8) + twos_complement(-5, 8);
    if ((tc & 255) == 0) {
        printf("Verify: -5 + 5 = 0 (mod 256): 1\n");
    } else {
        printf("Verify: -5 + 5 = 0 (mod 256): 0\n");
    }

    printf("Done.\n");
    return 0;
}
