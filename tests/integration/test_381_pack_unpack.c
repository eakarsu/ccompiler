int printf(const char *fmt, ...);
// EXPECT: pack(1000, 2000) = 0x3E807D0\nunpack high = 1000\nunpack low = 2000\npack(0,0) -> unpack = (0,0) match=1\npack(100,200) -> unpack = (100,200) match=1\npack(65535,32768) -> unpack = (65535,32768) match=1\npack4(0x12,0x34,0x56,0x78) = 0x12345678\nbyte 0 = 0x78\nbyte 1 = 0x56\nbyte 2 = 0x34\nbyte 3 = 0x12\ncoord(10,20) packed=0xA0014\ncoord(150,200) packed=0x9600C8\nRGBA = 0xFF8040C8\nR=255 G=128 B=64 A=200\ndate pack: 2024/12/25 -> 0xFD199 -> 2024/12/25
// Test: pack/unpack ints

int pack_2x16(int a, int b) {
    return ((a & 0xFFFF) << 16) | (b & 0xFFFF);
}

int unpack_high16(int packed) {
    return (packed >> 16) & 0xFFFF;
}

int unpack_low16(int packed) {
    return packed & 0xFFFF;
}

int pack_4x8(int a, int b, int c, int d) {
    return ((a & 0xFF) << 24) | ((b & 0xFF) << 16) |
           ((c & 0xFF) << 8) | (d & 0xFF);
}

int unpack_byte(int packed, int index) {
    return (packed >> (index * 8)) & 0xFF;
}

int main(void) {
    // Pack two 16-bit values
    int packed1 = pack_2x16(1000, 2000);
    printf("pack(1000, 2000) = 0x%X\n", packed1);
    printf("unpack high = %d\n", unpack_high16(packed1));
    printf("unpack low = %d\n", unpack_low16(packed1));

    // Pack and unpack various pairs
    int pairs[6];
    pairs[0] = 0; pairs[1] = 0;
    pairs[2] = 100; pairs[3] = 200;
    pairs[4] = 65535; pairs[5] = 32768;
    int i;
    for (i = 0; i < 6; i += 2) {
        int p = pack_2x16(pairs[i], pairs[i+1]);
        int h = unpack_high16(p);
        int l = unpack_low16(p);
        printf("pack(%d,%d) -> unpack = (%d,%d) match=%d\n",
               pairs[i], pairs[i+1], h, l,
               h == pairs[i] && l == pairs[i+1]);
    }

    // Pack four bytes
    int packed4 = pack_4x8(0x12, 0x34, 0x56, 0x78);
    printf("pack4(0x12,0x34,0x56,0x78) = 0x%X\n", packed4);
    printf("byte 0 = 0x%X\n", unpack_byte(packed4, 0));
    printf("byte 1 = 0x%X\n", unpack_byte(packed4, 1));
    printf("byte 2 = 0x%X\n", unpack_byte(packed4, 2));
    printf("byte 3 = 0x%X\n", unpack_byte(packed4, 3));

    // Pack coordinates
    int coords[4];
    coords[0] = 10; coords[1] = 20;
    coords[2] = 150; coords[3] = 200;
    for (i = 0; i < 4; i += 2) {
        int packed = pack_2x16(coords[i], coords[i+1]);
        printf("coord(%d,%d) packed=0x%X\n", coords[i], coords[i+1], packed);
    }

    // Pack RGB + Alpha
    int r = 255;
    int g = 128;
    int b = 64;
    int a = 200;
    int rgba = pack_4x8(r, g, b, a);
    printf("RGBA = 0x%X\n", rgba);
    printf("R=%d G=%d B=%d A=%d\n",
           unpack_byte(rgba, 3),
           unpack_byte(rgba, 2),
           unpack_byte(rgba, 1),
           unpack_byte(rgba, 0));

    // Pack date: year(12) month(4) day(5) -> 21 bits
    int year = 2024;
    int month = 12;
    int day = 25;
    int date = ((year & 0xFFF) << 9) | ((month & 0xF) << 5) | (day & 0x1F);
    int uy = (date >> 9) & 0xFFF;
    int um = (date >> 5) & 0xF;
    int ud = date & 0x1F;
    printf("date pack: %d/%d/%d -> 0x%X -> %d/%d/%d\n",
           year, month, day, date, uy, um, ud);

    return 0;
}
