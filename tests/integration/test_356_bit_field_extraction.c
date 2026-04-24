int printf(const char *fmt, ...);
// EXPECT: low nibble of 0xAB = 0xB\nhigh nibble of 0xAB = 0xA\nbyte 0 = 0x78\nbyte 1 = 0x56\nbyte 2 = 0x34\nbyte 3 = 0x12\nbits [0,3] = 0xE\nbits [4,7] = 0xF\nbits [8,11] = 0xA\nbits [12,15] = 0xC\ninsert 0xA at [0,3]: 0xA\ninsert 0xB at [4,7]: 0xBA\ninsert 0xC at [8,11]: 0xCBA\nclear middle byte: 0xF00F\nRGB(255,128,64) = 0xFF8040\nR = 255\nG = 128\nB = 64\nage=25 height=180 weight=75
// Test: bit field extraction

int extract_bits(int value, int start, int width) {
    int mask = (1 << width) - 1;
    return (value >> start) & mask;
}

int insert_bits(int dest, int value, int start, int width) {
    int mask = ((1 << width) - 1) << start;
    dest = dest & ~mask;
    dest = dest | ((value << start) & mask);
    return dest;
}

int main(void) {
    // Extract nibbles from a byte
    int byte_val = 0xAB;
    printf("low nibble of 0xAB = 0x%X\n", extract_bits(byte_val, 0, 4));
    printf("high nibble of 0xAB = 0x%X\n", extract_bits(byte_val, 4, 4));

    // Extract bytes from a word
    int word = 0x12345678;
    printf("byte 0 = 0x%X\n", extract_bits(word, 0, 8));
    printf("byte 1 = 0x%X\n", extract_bits(word, 8, 8));
    printf("byte 2 = 0x%X\n", extract_bits(word, 16, 8));
    printf("byte 3 = 0x%X\n", extract_bits(word, 24, 8));

    // Extract arbitrary bit ranges
    int data = 0xCAFE;
    printf("bits [0,3] = 0x%X\n", extract_bits(data, 0, 4));
    printf("bits [4,7] = 0x%X\n", extract_bits(data, 4, 4));
    printf("bits [8,11] = 0x%X\n", extract_bits(data, 8, 4));
    printf("bits [12,15] = 0x%X\n", extract_bits(data, 12, 4));

    // Insert bits
    int reg = 0;
    reg = insert_bits(reg, 0xA, 0, 4);
    printf("insert 0xA at [0,3]: 0x%X\n", reg);
    reg = insert_bits(reg, 0xB, 4, 4);
    printf("insert 0xB at [4,7]: 0x%X\n", reg);
    reg = insert_bits(reg, 0xC, 8, 4);
    printf("insert 0xC at [8,11]: 0x%X\n", reg);

    // Overwrite field
    int val = 0xFFFF;
    val = insert_bits(val, 0x0, 4, 8);
    printf("clear middle byte: 0x%X\n", val);

    // Pack RGB color
    int r = 255;
    int g = 128;
    int b = 64;
    int color = 0;
    color = insert_bits(color, r, 16, 8);
    color = insert_bits(color, g, 8, 8);
    color = insert_bits(color, b, 0, 8);
    printf("RGB(%d,%d,%d) = 0x%X\n", r, g, b, color);

    // Unpack RGB
    printf("R = %d\n", extract_bits(color, 16, 8));
    printf("G = %d\n", extract_bits(color, 8, 8));
    printf("B = %d\n", extract_bits(color, 0, 8));

    // Multi-field structure simulation
    // age: 7 bits, height: 8 bits, weight: 8 bits
    int person = 0;
    person = insert_bits(person, 25, 0, 7);    // age
    person = insert_bits(person, 180, 7, 8);   // height
    person = insert_bits(person, 75, 15, 8);   // weight
    printf("age=%d height=%d weight=%d\n",
           extract_bits(person, 0, 7),
           extract_bits(person, 7, 8),
           extract_bits(person, 15, 8));

    return 0;
}
