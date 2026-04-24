int printf(const char *fmt, ...);
// EXPECT: simple checksum: 15\nxor checksum: 1\nfletcher16: 0x230F\ncorruption detected: 1\nxor of DEADBEEF CAFEBABE: 0x12\nfletcher16 (10 items): 0xAFCD\nones complement: 0x3AD1\nxor verify: 0\nsensitivity test:\n  change[0]: 100 -> 101 (diff=1)\n  change[1]: 100 -> 101 (diff=1)\n  change[2]: 100 -> 101 (diff=1)\n  change[3]: 100 -> 101 (diff=1)
// Test: checksum

int simple_checksum(int *data, int len) {
    int sum = 0;
    int i;
    for (i = 0; i < len; i++) {
        sum = sum + data[i];
    }
    return sum & 0xFF;
}

int xor_checksum(int *data, int len) {
    int result = 0;
    int i;
    for (i = 0; i < len; i++) {
        result = result ^ data[i];
    }
    return result;
}

int fletcher16(int *data, int len) {
    int sum1 = 0;
    int sum2 = 0;
    int i;
    for (i = 0; i < len; i++) {
        sum1 = (sum1 + (data[i] & 0xFF)) % 255;
        sum2 = (sum2 + sum1) % 255;
    }
    return (sum2 << 8) | sum1;
}

int ones_complement_sum(int *data, int len) {
    int sum = 0;
    int i;
    for (i = 0; i < len; i++) {
        sum = sum + (data[i] & 0xFFFF);
        // Fold carry
        while (sum > 0xFFFF) {
            sum = (sum & 0xFFFF) + (sum >> 16);
        }
    }
    return ~sum & 0xFFFF;
}

int main(void) {
    // Test data
    int data1[5];
    data1[0] = 1; data1[1] = 2; data1[2] = 3;
    data1[3] = 4; data1[4] = 5;

    printf("simple checksum: %d\n", simple_checksum(data1, 5));
    printf("xor checksum: %d\n", xor_checksum(data1, 5));
    printf("fletcher16: 0x%X\n", fletcher16(data1, 5));

    // Verify checksum detects changes
    int data2[5];
    int i;
    for (i = 0; i < 5; i++) data2[i] = data1[i];
    int orig_ck = simple_checksum(data2, 5);
    data2[2] = 99;  // corrupt one element
    int new_ck = simple_checksum(data2, 5);
    printf("corruption detected: %d\n", orig_ck != new_ck);

    // XOR checksum on bytes
    int bytes[8];
    bytes[0] = 0xDE; bytes[1] = 0xAD; bytes[2] = 0xBE; bytes[3] = 0xEF;
    bytes[4] = 0xCA; bytes[5] = 0xFE; bytes[6] = 0xBA; bytes[7] = 0xBE;
    printf("xor of DEADBEEF CAFEBABE: 0x%X\n", xor_checksum(bytes, 8));

    // Fletcher checksum on larger data
    int data3[10];
    for (i = 0; i < 10; i++) data3[i] = i * 10 + 1;
    printf("fletcher16 (10 items): 0x%X\n", fletcher16(data3, 10));

    // Ones complement checksum
    int ip_header[5];
    ip_header[0] = 0x4500;
    ip_header[1] = 0x0028;
    ip_header[2] = 0x0000;
    ip_header[3] = 0x4000;
    ip_header[4] = 0x4006;
    printf("ones complement: 0x%X\n", ones_complement_sum(ip_header, 5));

    // Verify: adding checksum to data gives 0
    int ck_data[3];
    ck_data[0] = 100; ck_data[1] = 200; ck_data[2] = 300;
    int xck = xor_checksum(ck_data, 3);
    // Verify: XOR of all data + checksum = 0
    int verify = xck;
    for (i = 0; i < 3; i++) verify = verify ^ ck_data[i];
    printf("xor verify: %d\n", verify);

    // Checksum sensitivity
    printf("sensitivity test:\n");
    int base[4];
    base[0] = 10; base[1] = 20; base[2] = 30; base[3] = 40;
    int base_sum = simple_checksum(base, 4);
    for (i = 0; i < 4; i++) {
        int modified[4];
        int j;
        for (j = 0; j < 4; j++) modified[j] = base[j];
        modified[i] = modified[i] + 1;
        int mod_sum = simple_checksum(modified, 4);
        printf("  change[%d]: %d -> %d (diff=%d)\n",
               i, base_sum, mod_sum, mod_sum - base_sum);
    }

    return 0;
}
