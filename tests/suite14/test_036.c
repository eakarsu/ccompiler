int printf(const char *fmt, ...);

/* CRC computation (CRC-8) */
/* CRC-8 with polynomial 0x07 (x^8 + x^2 + x + 1) */

int crc8_table[256];

void crc8_init_table(void) {
    int i;
    int j;
    for (i = 0; i < 256; i++) {
        int crc = i;
        for (j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = ((crc << 1) ^ 0x07) & 0xFF;
            } else {
                crc = (crc << 1) & 0xFF;
            }
        }
        crc8_table[i] = crc;
    }
}

int crc8_compute(int *data, int len) {
    int crc = 0;
    int i;
    for (i = 0; i < len; i++) {
        crc = crc8_table[(crc ^ data[i]) & 0xFF];
    }
    return crc;
}

/* Bitwise CRC-8 (no table) */
int crc8_bitwise(int *data, int len) {
    int crc = 0;
    int i;
    int j;
    for (i = 0; i < len; i++) {
        crc = crc ^ data[i];
        for (j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = ((crc << 1) ^ 0x07) & 0xFF;
            } else {
                crc = (crc << 1) & 0xFF;
            }
        }
    }
    return crc;
}

/* Simple XOR checksum */
int xor_checksum(int *data, int len) {
    int cs = 0;
    int i;
    for (i = 0; i < len; i++) {
        cs = cs ^ data[i];
    }
    return cs & 0xFF;
}

/* Sum checksum mod 256 */
int sum_checksum(int *data, int len) {
    int cs = 0;
    int i;
    for (i = 0; i < len; i++) {
        cs = cs + data[i];
    }
    return cs & 0xFF;
}

/* Two's complement checksum */
int twos_comp_checksum(int *data, int len) {
    int sum = 0;
    int i;
    for (i = 0; i < len; i++) {
        sum = sum + data[i];
    }
    return ((~sum) + 1) & 0xFF;
}

/* Verify data with appended CRC */
int crc8_verify(int *data, int len, int expected_crc) {
    int crc = crc8_compute(data, len);
    return crc == expected_crc;
}

/* Demonstrate CRC detects single-bit errors */
int crc8_detect_error(int *data, int len) {
    int orig_crc = crc8_compute(data, len);
    int errors_detected = 0;
    int i;
    int b;
    for (i = 0; i < len; i++) {
        for (b = 0; b < 8; b++) {
            int saved = data[i];
            data[i] = data[i] ^ (1 << b);
            int new_crc = crc8_compute(data, len);
            if (new_crc != orig_crc) errors_detected = errors_detected + 1;
            data[i] = saved;
        }
    }
    return errors_detected;
}

int main(void) {
    crc8_init_table();

    /* Verify table entries */
    printf("table[0]: %d\n", crc8_table[0]);
    // EXPECT: table[0]: 0
    printf("table[1]: %d\n", crc8_table[1]);
    // EXPECT: table[1]: 7
    printf("table[2]: %d\n", crc8_table[2]);
    // EXPECT: table[2]: 14
    printf("table[255]: %d\n", crc8_table[255]);
    // EXPECT: table[255]: 243

    /* CRC of empty data */
    int empty[1];
    int crc_empty = crc8_compute(empty, 0);
    printf("crc empty: %d\n", crc_empty);
    // EXPECT: crc empty: 0

    /* CRC of single byte */
    int d1[1];
    d1[0] = 0x31; /* ASCII '1' = 49 */
    int crc1 = crc8_compute(d1, 1);
    printf("crc 0x31: %d\n", crc1);
    // EXPECT: crc 0x31: 151

    /* Verify bitwise matches table */
    int crc1b = crc8_bitwise(d1, 1);
    printf("bitwise match: %d\n", crc1 == crc1b);
    // EXPECT: bitwise match: 1

    /* CRC of "123" = [49, 50, 51] */
    int d2[3];
    d2[0] = 49; d2[1] = 50; d2[2] = 51;
    int crc2 = crc8_compute(d2, 3);
    printf("crc 123: %d\n", crc2);
    // EXPECT: crc 123: 192

    int crc2b = crc8_bitwise(d2, 3);
    printf("bitwise match 2: %d\n", crc2 == crc2b);
    // EXPECT: bitwise match 2: 1

    /* CRC of all zeros */
    int d3[4];
    d3[0] = 0; d3[1] = 0; d3[2] = 0; d3[3] = 0;
    int crc3 = crc8_compute(d3, 4);
    printf("crc zeros: %d\n", crc3);
    // EXPECT: crc zeros: 0

    /* CRC of all 0xFF */
    int d4[4];
    d4[0] = 255; d4[1] = 255; d4[2] = 255; d4[3] = 255;
    int crc4 = crc8_compute(d4, 4);
    printf("crc FFs: %d\n", crc4);
    // EXPECT: crc FFs: 222

    /* Verify function */
    printf("verify ok: %d\n", crc8_verify(d2, 3, 192));
    // EXPECT: verify ok: 1
    printf("verify bad: %d\n", crc8_verify(d2, 3, 193));
    // EXPECT: verify bad: 0

    /* Error detection: all single-bit errors in 3-byte message detected */
    int d5[3];
    d5[0] = 65; d5[1] = 66; d5[2] = 67;
    int detected = crc8_detect_error(d5, 3);
    printf("errors detected: %d of %d\n", detected, 3 * 8);
    // EXPECT: errors detected: 24 of 24

    /* XOR checksum */
    int xor1 = xor_checksum(d2, 3);
    printf("xor checksum: %d\n", xor1);
    // EXPECT: xor checksum: 48

    /* 49 ^ 50 ^ 51: 49^50 = 3, 3^51 = 48 */

    /* Sum checksum */
    int sum1 = sum_checksum(d2, 3);
    printf("sum checksum: %d\n", sum1);
    // EXPECT: sum checksum: 150

    /* 49 + 50 + 51 = 150 */

    /* Two's complement checksum */
    int tc1 = twos_comp_checksum(d2, 3);
    printf("twos comp: %d\n", tc1);
    // EXPECT: twos comp: 106

    /* ~150 + 1 = ~(10010110) + 1. 150 = 0x96, ~0x96 = 0x69 = 105, + 1 = 106 */

    /* Verify sum + twos_comp = 0 (mod 256) */
    int verify_tc = (sum1 + tc1) & 0xFF;
    printf("tc verify: %d\n", verify_tc);
    // EXPECT: tc verify: 0

    /* CRC is order-dependent */
    int d6[3];
    d6[0] = 51; d6[1] = 50; d6[2] = 49;
    int crc6 = crc8_compute(d6, 3);
    printf("crc reversed: %d\n", crc6);
    // EXPECT: crc reversed: 24

    printf("order matters: %d\n", crc2 != crc6);
    // EXPECT: order matters: 1

    printf("done\n");
    // EXPECT: done
    return 0;
}
