int printf(const char *fmt, ...);
// EXPECT: int 0x41424344 as bytes: 44 43 42 41 \nbytes as int: 0x12345678\nint array as bytes: 04 03 02 01 08 07 06 05 \nwritten through char*: 0xDEADBEEF\nptr round-trip: 42\nfields: 100 200 300\nendianness: little\nword[0] = 0x4030201\nword[1] = 0x8070605\nvia int*: 10 20 30 40
// Test: pointer casts

int main(void) {
    // Int pointer to char pointer
    int val = 0x41424344;  // ABCD in ASCII (little-endian: D C B A)
    char *cp = (char *)&val;
    printf("int 0x41424344 as bytes: ");
    int i;
    for (i = 0; i < 4; i++) {
        printf("%02X ", cp[i] & 0xFF);
    }
    printf("\n");

    // Char array to int pointer
    char bytes[4];
    bytes[0] = 0x78;
    bytes[1] = 0x56;
    bytes[2] = 0x34;
    bytes[3] = 0x12;
    int *ip = (int *)bytes;
    printf("bytes as int: 0x%X\n", *ip);

    // Int array accessed as chars
    int arr[2];
    arr[0] = 0x01020304;
    arr[1] = 0x05060708;
    char *carr = (char *)arr;
    printf("int array as bytes: ");
    for (i = 0; i < 8; i++) {
        printf("%02X ", carr[i] & 0xFF);
    }
    printf("\n");

    // Write through char pointer, read as int
    int target = 0;
    char *tp = (char *)&target;
    tp[0] = 0xEF;
    tp[1] = 0xBE;
    tp[2] = 0xAD;
    tp[3] = 0xDE;
    printf("written through char*: 0x%X\n", (unsigned)target);

    // Pointer to pointer cast
    int x = 42;
    int *px = &x;
    char *cx = (char *)px;
    int *px2 = (int *)cx;
    printf("ptr round-trip: %d\n", *px2);

    // Struct-like access through char pointer
    int data[3];
    data[0] = 100;    // "field1"
    data[1] = 200;    // "field2"
    data[2] = 300;    // "field3"
    char *raw = (char *)data;
    int *f1 = (int *)(raw + 0);
    int *f2 = (int *)(raw + 4);
    int *f3 = (int *)(raw + 8);
    printf("fields: %d %d %d\n", *f1, *f2, *f3);

    // Endianness detection
    int endian_test = 1;
    char *ep = (char *)&endian_test;
    if (ep[0] == 1) {
        printf("endianness: little\n");
    } else {
        printf("endianness: big\n");
    }

    // Array of chars reinterpreted
    char buffer[8];
    for (i = 0; i < 8; i++) buffer[i] = (char)(i + 1);
    int *words = (int *)buffer;
    printf("word[0] = 0x%X\n", words[0]);
    printf("word[1] = 0x%X\n", words[1]);

    // Pointer arithmetic with different types
    int iarr[4];
    iarr[0] = 10; iarr[1] = 20; iarr[2] = 30; iarr[3] = 40;
    int *iptr = iarr;
    printf("via int*: %d %d %d %d\n", iptr[0], iptr[1], iptr[2], iptr[3]);

    return 0;
}
