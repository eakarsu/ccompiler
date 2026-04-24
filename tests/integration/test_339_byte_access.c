int printf(const char *fmt, ...);
// EXPECT: byte0(0x04030201)=1\nbyte1(0x04030201)=2\nbyte2(0x04030201)=3\nbyte3(0x04030201)=4\nset byte1=0xFF: 255\nreverse(0x01020304)=0x04030201\npack(1,2,3,4)=0x04030201\n  byte0=1\n  byte1=2\n  byte2=3\n  byte3=4\nnonzero(0x010203)=3\nnonzero(0)=0\nnonzero(0x01020304)=4\nrotate(0x04030201)=0x03020104
// Test: union-based byte access

union Word32 {
    int value;
    char bytes[4];
};

int extract_byte(int val, int n) {
    union Word32 w;
    w.value = val;
    return (int)w.bytes[n] & 0xFF;
}

int set_byte(int val, int n, int b) {
    union Word32 w;
    w.value = val;
    w.bytes[n] = (char)b;
    return w.value;
}

int reverse_bytes(int val) {
    union Word32 src;
    union Word32 dst;
    src.value = val;
    dst.bytes[0] = src.bytes[3];
    dst.bytes[1] = src.bytes[2];
    dst.bytes[2] = src.bytes[1];
    dst.bytes[3] = src.bytes[0];
    return dst.value;
}

int rotate_left_byte(int val) {
    union Word32 w;
    w.value = val;
    char tmp = w.bytes[3];
    w.bytes[3] = w.bytes[2];
    w.bytes[2] = w.bytes[1];
    w.bytes[1] = w.bytes[0];
    w.bytes[0] = tmp;
    return w.value;
}

int count_nonzero_bytes(int val) {
    union Word32 w;
    w.value = val;
    int count = 0;
    int i;
    for (i = 0; i < 4; i++) {
        if (w.bytes[i] != 0) count = count + 1;
    }
    return count;
}

int pack_bytes(int b0, int b1, int b2, int b3) {
    union Word32 w;
    w.bytes[0] = (char)b0;
    w.bytes[1] = (char)b1;
    w.bytes[2] = (char)b2;
    w.bytes[3] = (char)b3;
    return w.value;
}

int main(void) {
    printf("byte0(0x04030201)=%d\n", extract_byte(0x04030201, 0));
    printf("byte1(0x04030201)=%d\n", extract_byte(0x04030201, 1));
    printf("byte2(0x04030201)=%d\n", extract_byte(0x04030201, 2));
    printf("byte3(0x04030201)=%d\n", extract_byte(0x04030201, 3));

    int val = 0x00000001;
    val = set_byte(val, 1, 0xFF);
    printf("set byte1=0xFF: %d\n", extract_byte(val, 1));

    int rev = reverse_bytes(0x01020304);
    printf("reverse(0x01020304)=0x%08x\n", rev);

    int packed = pack_bytes(1, 2, 3, 4);
    printf("pack(1,2,3,4)=0x%08x\n", packed);

    int i;
    for (i = 0; i < 4; i++) {
        printf("  byte%d=%d\n", i, extract_byte(packed, i));
    }

    printf("nonzero(0x010203)=%d\n", count_nonzero_bytes(0x010203));
    printf("nonzero(0)=%d\n", count_nonzero_bytes(0));
    printf("nonzero(0x01020304)=%d\n", count_nonzero_bytes(0x01020304));

    int rot = rotate_left_byte(0x04030201);
    printf("rotate(0x04030201)=0x%08x\n", rot);

    return 0;
}
