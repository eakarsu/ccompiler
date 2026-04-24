int printf(const char *fmt, ...);

int read_byte(char *p, int idx) {
    return *(p + idx) & 255;
}

void write_byte(char *p, int idx, int val) {
    *(p + idx) = (char)val;
}

int reconstruct_int(char *bytes) {
    /* Little-endian reconstruction */
    int b0 = bytes[0] & 255;
    int b1 = bytes[1] & 255;
    int b2 = bytes[2] & 255;
    int b3 = bytes[3] & 255;
    return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}

int count_nonzero_bytes(int *ip, int num_ints) {
    char *cp = (char *)ip;
    int total = num_ints * 4;
    int count = 0;
    int i;
    for (i = 0; i < total; i = i + 1) {
        if ((cp[i] & 255) != 0) {
            count = count + 1;
        }
    }
    return count;
}

int main(void) {
    /* Cast int* to char* to inspect bytes (little-endian ARM64) */
    int val = 1;
    char *bp = (char *)&val;

    /* 1 in little-endian: 01 00 00 00 */
    // EXPECT: 1
    printf("%d\n", read_byte(bp, 0));
    // EXPECT: 0
    printf("%d\n", read_byte(bp, 1));
    // EXPECT: 0
    printf("%d\n", read_byte(bp, 2));
    // EXPECT: 0
    printf("%d\n", read_byte(bp, 3));

    /* 0x01020304 = 16909060 in little-endian: 04 03 02 01 */
    int val2 = 16909060;
    bp = (char *)&val2;
    // EXPECT: 4
    printf("%d\n", read_byte(bp, 0));
    // EXPECT: 3
    printf("%d\n", read_byte(bp, 1));
    // EXPECT: 2
    printf("%d\n", read_byte(bp, 2));
    // EXPECT: 1
    printf("%d\n", read_byte(bp, 3));

    /* Reconstruct from bytes */
    // EXPECT: 16909060
    printf("%d\n", reconstruct_int(bp));

    /* Write bytes then read as int: 0x00000005 = 5 */
    int target = 0;
    char *tp = (char *)&target;
    write_byte(tp, 0, 5);
    write_byte(tp, 1, 0);
    write_byte(tp, 2, 0);
    write_byte(tp, 3, 0);
    // EXPECT: 5
    printf("%d\n", target);

    /* 0xFF = 255 in byte 0 */
    target = 0;
    tp = (char *)&target;
    write_byte(tp, 0, 255);
    // EXPECT: 255
    printf("%d\n", target);

    /* 1 in byte 1 => 256 */
    target = 0;
    tp = (char *)&target;
    write_byte(tp, 1, 1);
    // EXPECT: 256
    printf("%d\n", target);

    /* 1 in byte 2 => 65536 */
    target = 0;
    tp = (char *)&target;
    write_byte(tp, 2, 1);
    // EXPECT: 65536
    printf("%d\n", target);

    /* Count nonzero bytes in an array */
    int arr[3];
    arr[0] = 1;     /* 01 00 00 00 -> 1 nonzero */
    arr[1] = 256;   /* 00 01 00 00 -> 1 nonzero */
    arr[2] = 0;     /* 00 00 00 00 -> 0 nonzero */
    // EXPECT: 2
    printf("%d\n", count_nonzero_bytes(arr, 3));

    /* Cast between int* types and verify sizeof */
    int x = 42;
    int *ip = &x;
    char *cp = (char *)ip;
    int *ip2 = (int *)cp;
    // EXPECT: 42
    printf("%d\n", *ip2);

    /* sizeof checks */
    // EXPECT: 4
    printf("%d\n", sizeof(int));
    // EXPECT: 1
    printf("%d\n", sizeof(char));
    // EXPECT: 8
    printf("%d\n", sizeof(int *));
    // EXPECT: 8
    printf("%d\n", sizeof(char *));

    /* Treating char array as int (aligned) */
    int aligned_buf[2];
    aligned_buf[0] = 100;
    aligned_buf[1] = 200;
    char *cbuf = (char *)aligned_buf;
    /* byte 0 of 100 (little-endian) = 100 */
    // EXPECT: 100
    printf("%d\n", cbuf[0] & 255);

    /* Cast from char* back to int* to read second int */
    int *rp = (int *)(cbuf + 4);
    // EXPECT: 200
    printf("%d\n", *rp);

    return 0;
}
