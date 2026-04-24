int printf(const char *fmt, ...);

int simple_checksum(int *data, int len) {
    int sum = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        sum = sum + data[i];
    }
    return sum & 255;
}

int xor_checksum(int *data, int len) {
    int xval = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        xval = xval ^ data[i];
    }
    return xval;
}

int crc_like(int *data, int len) {
    int crc = 0;
    int i;
    int j;
    for (i = 0; i < len; i = i + 1) {
        crc = crc ^ (data[i] << 8);
        for (j = 0; j < 8; j = j + 1) {
            if (crc & 32768) {
                crc = (crc << 1) ^ 4129;
            } else {
                crc = crc << 1;
            }
            crc = crc & 65535;
        }
    }
    return crc;
}

int adler_style(int *data, int len) {
    int a = 1;
    int b = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        a = (a + data[i]) % 251;
        b = (b + a) % 251;
    }
    return (b << 8) | a;
}

int fletcher_style(int *data, int len) {
    int s1 = 0;
    int s2 = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        s1 = (s1 + data[i]) % 255;
        s2 = (s2 + s1) % 255;
    }
    return (s2 << 8) | s1;
}

int verify_checksum(int *data, int len, int expected) {
    return simple_checksum(data, len) == expected;
}

int main(void) {
    int data[8];
    int i;

    data[0] = 1; data[1] = 2; data[2] = 3; data[3] = 4;
    data[4] = 5; data[5] = 0; data[6] = 0; data[7] = 0;

    // EXPECT: Simple checksum: 15
    printf("Simple checksum: %d\n", simple_checksum(data, 5));

    // EXPECT: XOR checksum: 1
    printf("XOR checksum: %d\n", xor_checksum(data, 5));

    int data2[4];
    data2[0] = 255; data2[1] = 255; data2[2] = 255; data2[3] = 255;
    // EXPECT: Checksum 255s: 252
    printf("Checksum 255s: %d\n", simple_checksum(data2, 4));

    // EXPECT: XOR 255s: 0
    printf("XOR 255s: %d\n", xor_checksum(data2, 4));

    int data3[3];
    data3[0] = 0; data3[1] = 0; data3[2] = 0;
    // EXPECT: Zero checksum: 0
    printf("Zero checksum: %d\n", simple_checksum(data3, 3));

    // EXPECT: Zero XOR: 0
    printf("Zero XOR: %d\n", xor_checksum(data3, 3));

    int crc = crc_like(data, 5);
    // EXPECT: CRC-like: 33288
    printf("CRC-like: %d\n", crc);

    int data4[3];
    data4[0] = 65; data4[1] = 66; data4[2] = 67;
    int crc2 = crc_like(data4, 3);
    // EXPECT: CRC ABC: 14740
    printf("CRC ABC: %d\n", crc2);

    int adl = adler_style(data, 5);
    // EXPECT: Adler-style: 10256
    printf("Adler-style: %d\n", adl);

    int adl2 = adler_style(data4, 3);
    // EXPECT: Adler ABC: 37575
    printf("Adler ABC: %d\n", adl2);

    int fl = fletcher_style(data, 5);
    // EXPECT: Fletcher-style: 8975
    printf("Fletcher-style: %d\n", fl);

    int fl2 = fletcher_style(data4, 3);
    // EXPECT: Fletcher ABC: 35782
    printf("Fletcher ABC: %d\n", fl2);

    // EXPECT: Verify OK: 1
    printf("Verify OK: %d\n", verify_checksum(data, 5, 15));

    // EXPECT: Verify fail: 0
    printf("Verify fail: %d\n", verify_checksum(data, 5, 99));

    int single[1];
    single[0] = 42;
    // EXPECT: Single checksum: 42
    printf("Single checksum: %d\n", simple_checksum(single, 1));
    // EXPECT: Single XOR: 42
    printf("Single XOR: %d\n", xor_checksum(single, 1));

    return 0;
}
