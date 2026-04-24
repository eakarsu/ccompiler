int printf(const char *fmt, ...);

// Checksum Verifier

int compute_checksum_simple(int *data, int len) {
    int sum = 0;
    int i;
    for (i = 0; i < len; i++) {
        sum = sum + data[i];
    }
    return sum & 255;
}

int compute_checksum_twos_complement(int *data, int len) {
    int sum = 0;
    int i;
    for (i = 0; i < len; i++) {
        sum = sum + data[i];
        // Fold carry
        while (sum > 65535) {
            sum = (sum & 65535) + 1;
        }
    }
    return (~sum) & 65535;
}

int compute_xor_checksum(int *data, int len) {
    int result = 0;
    int i;
    for (i = 0; i < len; i++) {
        result = result ^ data[i];
    }
    return result;
}

int compute_fletcher16(int *data, int len) {
    int sum1 = 0;
    int sum2 = 0;
    int i;
    for (i = 0; i < len; i++) {
        sum1 = (sum1 + data[i]) % 255;
        sum2 = (sum2 + sum1) % 255;
    }
    return sum2 * 256 + sum1;
}

int verify_checksum_simple(int *data, int len, int expected) {
    return compute_checksum_simple(data, len) == expected;
}

struct ChecksumResult {
    int value;
    int valid;
    char *method;
};

struct ChecksumResult test_simple(int *data, int len) {
    struct ChecksumResult r;
    r.value = compute_checksum_simple(data, len);
    r.valid = 1;
    r.method = "simple";
    return r;
}

struct ChecksumResult test_xor(int *data, int len) {
    struct ChecksumResult r;
    r.value = compute_xor_checksum(data, len);
    r.valid = 1;
    r.method = "xor";
    return r;
}

int crc8_table[8];

void init_crc8_table() {
    int i;
    for (i = 0; i < 8; i++) {
        int val = 1;
        int j;
        for (j = 0; j < i; j++) {
            val = val * 2;
        }
        crc8_table[i] = val;
    }
}

int compute_crc8_simple(int *data, int len) {
    int crc = 0;
    int i;
    for (i = 0; i < len; i++) {
        crc = crc ^ data[i];
        int j;
        for (j = 0; j < 8; j++) {
            if (crc & 128) {
                crc = ((crc * 2) & 255) ^ 7;
            } else {
                crc = (crc * 2) & 255;
            }
        }
    }
    return crc;
}

int adler32_low(int *data, int len) {
    int a = 1;
    int b = 0;
    int i;
    for (i = 0; i < len; i++) {
        a = (a + data[i]) % 65521;
        b = (b + a) % 65521;
    }
    return a;
}

int adler32_high(int *data, int len) {
    int a = 1;
    int b = 0;
    int i;
    for (i = 0; i < len; i++) {
        a = (a + data[i]) % 65521;
        b = (b + a) % 65521;
    }
    return b;
}

int main() {
    // Test 1: simple checksum
    int data1[5];
    data1[0] = 10; data1[1] = 20; data1[2] = 30; data1[3] = 40; data1[4] = 50;
    int cs1 = compute_checksum_simple(data1, 5);
    printf("simple checksum: %d\n", cs1);
    // EXPECT: simple checksum: 150

    // Test 2: simple checksum with overflow
    int data2[4];
    data2[0] = 200; data2[1] = 100; data2[2] = 50; data2[3] = 25;
    printf("simple overflow: %d\n", compute_checksum_simple(data2, 4));
    // EXPECT: simple overflow: 119

    // Test 3: XOR checksum
    int data3[4];
    data3[0] = 170; data3[1] = 85; data3[2] = 170; data3[3] = 85;
    printf("xor checksum: %d\n", compute_xor_checksum(data3, 4));
    // EXPECT: xor checksum: 0

    // Test 4: XOR with different data
    int data4[3];
    data4[0] = 15; data4[1] = 240; data4[2] = 128;
    printf("xor checksum 2: %d\n", compute_xor_checksum(data4, 3));
    // EXPECT: xor checksum 2: 127

    // Test 5: fletcher16
    int data5[4];
    data5[0] = 1; data5[1] = 2; data5[2] = 3; data5[3] = 4;
    int f16 = compute_fletcher16(data5, 4);
    printf("fletcher16: %d\n", f16);
    // EXPECT: fletcher16: 5130

    // Test 6: verify checksum
    printf("verify ok: %d\n", verify_checksum_simple(data1, 5, 150));
    // EXPECT: verify ok: 1
    printf("verify bad: %d\n", verify_checksum_simple(data1, 5, 99));
    // EXPECT: verify bad: 0

    // Test 7: struct result - simple
    struct ChecksumResult r = test_simple(data1, 5);
    printf("result method=%s value=%d valid=%d\n", r.method, r.value, r.valid);
    // EXPECT: result method=simple value=150 valid=1

    // Test 8: struct result - xor
    r = test_xor(data4, 3);
    printf("result method=%s value=%d valid=%d\n", r.method, r.value, r.valid);
    // EXPECT: result method=xor value=127 valid=1

    // Test 9: CRC8
    init_crc8_table();
    int data6[3];
    data6[0] = 65; data6[1] = 66; data6[2] = 67;
    int crc = compute_crc8_simple(data6, 3);
    printf("crc8: %d\n", crc);
    // EXPECT: crc8: 82

    // Test 10: twos complement checksum
    int data7[4];
    data7[0] = 256; data7[1] = 512; data7[2] = 1024; data7[3] = 2048;
    int tc = compute_checksum_twos_complement(data7, 4);
    printf("twos comp: %d\n", tc);
    // EXPECT: twos comp: 61695

    // Test 11: Adler parts
    int data8[5];
    data8[0] = 72; data8[1] = 101; data8[2] = 108; data8[3] = 108; data8[4] = 111;
    printf("adler low: %d\n", adler32_low(data8, 5));
    // EXPECT: adler low: 501
    printf("adler high: %d\n", adler32_high(data8, 5));
    // EXPECT: adler high: 1420

    // Test 12: empty data checksums
    int empty[1];
    empty[0] = 0;
    printf("empty simple: %d\n", compute_checksum_simple(empty, 0));
    // EXPECT: empty simple: 0
    printf("empty xor: %d\n", compute_xor_checksum(empty, 0));
    // EXPECT: empty xor: 0

    return 0;
}
