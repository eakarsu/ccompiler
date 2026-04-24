int printf(const char *fmt, ...);
// EXPECT: value=16909060 bytes=[4, 3, 2, 1]\nvalue=255 bytes=[255, 0, 0, 0]\nvalue=256 bytes=[0, 1, 0, 0]\nvalue=65536 bytes=[0, 0, 1, 0]\nreconstruct(1,0,0,0) = 1\nreconstruct(0,1,0,0) = 256\nswap_bytes_16(0x0102) = 513\nparts[0]=10 parts[1]=20 whole=5130
// Test: union for type punning

union IntBytes {
    int value;
    char bytes[4];
};

union TwoShorts {
    int whole;
    char parts[4];
};

void print_bytes(union IntBytes *ib) {
    int i;
    printf("value=%d bytes=[", ib->value);
    for (i = 0; i < 4; i++) {
        if (i > 0) printf(", ");
        printf("%d", (int)(ib->bytes[i]) & 0xFF);
    }
    printf("]\n");
}

int reconstruct_int(char b0, char b1, char b2, char b3) {
    union IntBytes ib;
    ib.bytes[0] = b0;
    ib.bytes[1] = b1;
    ib.bytes[2] = b2;
    ib.bytes[3] = b3;
    return ib.value;
}

int swap_bytes_16(int val) {
    union IntBytes ib;
    ib.value = val;
    char tmp = ib.bytes[0];
    ib.bytes[0] = ib.bytes[1];
    ib.bytes[1] = tmp;
    tmp = ib.bytes[2];
    ib.bytes[2] = ib.bytes[3];
    ib.bytes[3] = tmp;
    return ib.value;
}

int main(void) {
    union IntBytes ib;
    ib.value = 0x01020304;
    print_bytes(&ib);

    ib.value = 255;
    print_bytes(&ib);

    ib.value = 256;
    print_bytes(&ib);

    ib.value = 65536;
    print_bytes(&ib);

    int rebuilt = reconstruct_int(1, 0, 0, 0);
    printf("reconstruct(1,0,0,0) = %d\n", rebuilt);

    rebuilt = reconstruct_int(0, 1, 0, 0);
    printf("reconstruct(0,1,0,0) = %d\n", rebuilt);

    int swapped = swap_bytes_16(0x0102);
    printf("swap_bytes_16(0x0102) = %d\n", swapped);

    union TwoShorts ts;
    ts.whole = 0;
    ts.parts[0] = 10;
    ts.parts[1] = 20;
    printf("parts[0]=%d parts[1]=%d whole=%d\n",
           (int)ts.parts[0], (int)ts.parts[1], ts.whole);

    return 0;
}
