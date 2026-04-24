int printf(const char *fmt, ...);

int main(void) {
    int val = 0x04030201;
    char *cp = (char *)&val;

    // Little-endian: lowest byte at lowest address
    // EXPECT: byte0 = 1
    printf("byte0 = %d\n", (int)cp[0]);

    // EXPECT: byte1 = 2
    printf("byte1 = %d\n", (int)cp[1]);

    // EXPECT: byte2 = 3
    printf("byte2 = %d\n", (int)cp[2]);

    // EXPECT: byte3 = 4
    printf("byte3 = %d\n", (int)cp[3]);

    int val2 = 0x00FF00FF;
    char *cp2 = (char *)&val2;
    // EXPECT: alt_byte0 = -1
    printf("alt_byte0 = %d\n", (int)cp2[0]);

    // EXPECT: alt_byte1 = 0
    printf("alt_byte1 = %d\n", (int)cp2[1]);

    // EXPECT: alt_byte2 = -1
    printf("alt_byte2 = %d\n", (int)cp2[2]);

    // EXPECT: alt_byte3 = 0
    printf("alt_byte3 = %d\n", (int)cp2[3]);

    int arr[3];
    arr[0] = 0x11223344;
    arr[1] = 0x55667788;
    arr[2] = 0x99001122;
    char *bp = (char *)arr;

    // EXPECT: arr_b0 = 68
    printf("arr_b0 = %d\n", (int)bp[0]);

    // EXPECT: arr_b4 = -120
    printf("arr_b4 = %d\n", (int)bp[4]);

    // EXPECT: arr_b8 = 34
    printf("arr_b8 = %d\n", (int)bp[8]);

    // Cast char array back to int pointer
    char bytes[4];
    bytes[0] = 10;
    bytes[1] = 0;
    bytes[2] = 0;
    bytes[3] = 0;
    int *ip = (int *)bytes;
    // EXPECT: reassembled = 10
    printf("reassembled = %d\n", *ip);

    bytes[0] = 1;
    bytes[1] = 1;
    bytes[2] = 0;
    bytes[3] = 0;
    // EXPECT: combined = 257
    printf("combined = %d\n", *ip);

    // Construct a known value byte by byte
    char raw[4];
    raw[0] = 0;
    raw[1] = 1;
    raw[2] = 0;
    raw[3] = 0;
    int *rp = (int *)raw;
    // EXPECT: raw_int = 256
    printf("raw_int = %d\n", *rp);

    // Verify zero
    int zero = 0;
    char *zp = (char *)&zero;
    int all_zero = 1;
    int k;
    for (k = 0; k < 4; k++) {
        if (zp[k] != 0) all_zero = 0;
    }
    // EXPECT: all_zero = 1
    printf("all_zero = %d\n", all_zero);

    // Verify -1 (all 0xFF bytes)
    int neg = -1;
    char *np = (char *)&neg;
    int all_ff = 1;
    for (k = 0; k < 4; k++) {
        if (np[k] != -1) all_ff = 0;
    }
    // EXPECT: all_ff = 1
    printf("all_ff = %d\n", all_ff);

    // Modify one byte of an int
    int modify = 0x00000000;
    char *mp = (char *)&modify;
    mp[0] = 42;
    // EXPECT: modified = 42
    printf("modified = %d\n", modify);

    mp[1] = 1;
    // EXPECT: modified2 = 298
    printf("modified2 = %d\n", modify);

    // Sum all bytes of an int
    int test_val = 0x01020304;
    char *tp = (char *)&test_val;
    int bsum = 0;
    for (k = 0; k < 4; k++) {
        bsum = bsum + (int)tp[k];
    }
    // EXPECT: byte_sum = 10
    printf("byte_sum = %d\n", bsum);

    return 0;
}
