// EXPECT: === Checksum Algorithm Tests ===
// EXPECT: Simple sum checksum:
// EXPECT:   "Hello" = 500
// EXPECT:   "World" = 520
// EXPECT:   "" = 0
// EXPECT:   "A" = 65
// EXPECT:   "AAAA" = 260
// EXPECT: Fletcher-16 checksum:
// EXPECT:   "abcde" = fletcher(51440)
// EXPECT:   "abcdef" = fletcher(8279)
// EXPECT:   "abcdefgh" = fletcher(1575)
// EXPECT: Adler-32-like checksum:
// EXPECT:   "Wikipedia" = adler(300286872)
// EXPECT:   "a" = adler(6422626)
// EXPECT:   "" = adler(1)
// EXPECT: CRC-8 checksum:
// EXPECT:   "123456789" = crc8(244)
// EXPECT:   "Hello" = crc8(246)
// EXPECT:   "" = crc8(0)
// EXPECT: Data integrity verification:
// EXPECT:   Original data checksum: 500
// EXPECT:   Same data checksum:     500
// EXPECT:   Integrity check: PASS
// EXPECT:   Modified data checksum: 501
// EXPECT:   Tamper detected: PASS
// EXPECT: Checksum distribution test:
// EXPECT:   10 unique checksums out of 10 inputs: GOOD
// EXPECT: All checksum tests passed!

int printf(const char *fmt, ...);

int str_len(char *s) {
    int len = 0;
    while (s[len] != '\0') len = len + 1;
    return len;
}

// Simple additive checksum
int checksum_simple(char *data, int len) {
    int sum = 0;
    int i;
    for (i = 0; i < len; i++) {
        sum = sum + (int)(data[i] & 0xFF);
    }
    return sum;
}

// Fletcher-16 checksum
int fletcher16(char *data, int len) {
    int sum1 = 0;
    int sum2 = 0;
    int i;
    for (i = 0; i < len; i++) {
        sum1 = (sum1 + (data[i] & 0xFF)) % 255;
        sum2 = (sum2 + sum1) % 255;
    }
    return (sum2 << 8) | sum1;
}

// Adler-32-like checksum (uses 65521 as prime)
int adler32(char *data, int len) {
    int a = 1;
    int b = 0;
    int i;
    for (i = 0; i < len; i++) {
        a = (a + (data[i] & 0xFF)) % 65521;
        b = (b + a) % 65521;
    }
    return (b << 16) | a;
}

// CRC-8 with polynomial 0x07
int crc8(char *data, int len) {
    // Build CRC-8 table
    int table[256];
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
        table[i] = crc;
    }
    int crc = 0;
    for (i = 0; i < len; i++) {
        crc = table[(crc ^ (data[i] & 0xFF)) & 0xFF];
    }
    return crc;
}

int main() {
    printf("=== Checksum Algorithm Tests ===\n");

    // Simple sum
    printf("Simple sum checksum:\n");
    printf("  \"Hello\" = %d\n", checksum_simple("Hello", 5));
    printf("  \"World\" = %d\n", checksum_simple("World", 5));
    printf("  \"\" = %d\n", checksum_simple("", 0));
    printf("  \"A\" = %d\n", checksum_simple("A", 1));
    printf("  \"AAAA\" = %d\n", checksum_simple("AAAA", 4));

    // Fletcher-16
    printf("Fletcher-16 checksum:\n");
    printf("  \"abcde\" = fletcher(%d)\n", fletcher16("abcde", 5));
    printf("  \"abcdef\" = fletcher(%d)\n", fletcher16("abcdef", 6));
    printf("  \"abcdefgh\" = fletcher(%d)\n", fletcher16("abcdefgh", 8));

    // Adler-32
    printf("Adler-32-like checksum:\n");
    printf("  \"Wikipedia\" = adler(%d)\n", adler32("Wikipedia", 9));
    printf("  \"a\" = adler(%d)\n", adler32("a", 1));
    printf("  \"\" = adler(%d)\n", adler32("", 0));

    // CRC-8
    printf("CRC-8 checksum:\n");
    printf("  \"123456789\" = crc8(%d)\n", crc8("123456789", 9));
    printf("  \"Hello\" = crc8(%d)\n", crc8("Hello", 5));
    printf("  \"\" = crc8(%d)\n", crc8("", 0));

    // Data integrity test
    printf("Data integrity verification:\n");
    char *original = "Hello";
    int orig_sum = checksum_simple(original, 5);
    printf("  Original data checksum: %d\n", orig_sum);

    char copy[6];
    copy[0] = 'H'; copy[1] = 'e'; copy[2] = 'l'; copy[3] = 'l'; copy[4] = 'o'; copy[5] = '\0';
    int copy_sum = checksum_simple(copy, 5);
    printf("  Same data checksum:     %d\n", copy_sum);
    printf("  Integrity check: %s\n", orig_sum == copy_sum ? "PASS" : "FAIL");

    // Tamper detection
    copy[4] = 'p'; // change 'o' to 'p'
    int tampered_sum = checksum_simple(copy, 5);
    printf("  Modified data checksum: %d\n", tampered_sum);
    printf("  Tamper detected: %s\n", tampered_sum != orig_sum ? "PASS" : "FAIL");

    // Distribution test - different inputs should give different checksums
    printf("Checksum distribution test:\n");
    int csums[10];
    csums[0] = fletcher16("alpha", 5);
    csums[1] = fletcher16("bravo", 5);
    csums[2] = fletcher16("charl", 5);
    csums[3] = fletcher16("delta", 5);
    csums[4] = fletcher16("echoo", 5);
    csums[5] = fletcher16("foxtr", 5);
    csums[6] = fletcher16("golff", 5);
    csums[7] = fletcher16("hotel", 5);
    csums[8] = fletcher16("india", 5);
    csums[9] = fletcher16("julet", 5);
    int unique = 0;
    int ii;
    int jj;
    for (ii = 0; ii < 10; ii++) {
        int is_unique = 1;
        for (jj = 0; jj < ii; jj++) {
            if (csums[ii] == csums[jj]) {
                is_unique = 0;
            }
        }
        if (is_unique) unique = unique + 1;
    }
    printf("  %d unique checksums out of 10 inputs: %s\n", unique,
           unique >= 8 ? "GOOD" : "BAD");

    printf("All checksum tests passed!\n");
    return 0;
}
