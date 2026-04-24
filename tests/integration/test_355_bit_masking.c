int printf(const char *fmt, ...);
// EXPECT: Masks:\n  1 bits: 0x1\n  2 bits: 0x3\n  3 bits: 0x7\n  4 bits: 0xF\n  5 bits: 0x1F\n  6 bits: 0x3F\n  7 bits: 0x7F\n  8 bits: 0xFF\n0xDEAD & 0xFF = 0xAD\n0xDEAD & 0xFFF = 0xEAD\n0xDEAD & 0xF = 0xD\nbits [4,12) of 0xABCD = 0xBC\nset lower nibble: 0xF\nset both nibbles: 0xFF\nclear lower nibble: 0xF0\ntoggle 0xAA = 0x55\nconfig = 5\nfeature1 enabled: 1\nfeature2 enabled: 0\nfeature3 enabled: 1\n16 aligned to 16: 1\n17 aligned to 16: 0\n32 aligned to 16: 1\n15 aligned to 16: 0\nwrap around 16:\n  14 mod 16 = 14\n  15 mod 16 = 15\n  16 mod 16 = 0\n  17 mod 16 = 1\n  18 mod 16 = 2\n  19 mod 16 = 3
// Test: bit masking patterns

int main(void) {
    // Create masks of N bits
    int i;
    printf("Masks:\n");
    for (i = 1; i <= 8; i++) {
        int mask = (1 << i) - 1;
        printf("  %d bits: 0x%X\n", i, mask);
    }

    // Apply mask to extract lower bits
    int value = 0xDEAD;
    printf("0xDEAD & 0xFF = 0x%X\n", value & 0xFF);
    printf("0xDEAD & 0xFFF = 0x%X\n", value & 0xFFF);
    printf("0xDEAD & 0xF = 0x%X\n", value & 0xF);

    // Mask to isolate a range of bits
    int data = 0xABCD;
    int start = 4;
    int width = 8;
    int range_mask = ((1 << width) - 1) << start;
    int extracted = (data & range_mask) >> start;
    printf("bits [4,12) of 0xABCD = 0x%X\n", extracted);

    // Set bits using OR with mask
    int reg = 0;
    reg = reg | 0x0F;
    printf("set lower nibble: 0x%X\n", reg);
    reg = reg | 0xF0;
    printf("set both nibbles: 0x%X\n", reg);

    // Clear bits using AND with inverted mask
    int flags = 0xFF;
    flags = flags & ~0x0F;
    printf("clear lower nibble: 0x%X\n", flags);

    // Toggle bits using XOR with mask
    int toggle = 0xAA;
    toggle = toggle ^ 0xFF;
    printf("toggle 0xAA = 0x%X\n", toggle);

    // Conditional set/clear using mask
    int config = 0;
    int enable_feature1 = 1;
    int enable_feature2 = 0;
    int enable_feature3 = 1;

    if (enable_feature1) config = config | (1 << 0);
    if (enable_feature2) config = config | (1 << 1);
    if (enable_feature3) config = config | (1 << 2);
    printf("config = %d\n", config);

    // Check if specific bits are set
    printf("feature1 enabled: %d\n", (config & (1 << 0)) != 0);
    printf("feature2 enabled: %d\n", (config & (1 << 1)) != 0);
    printf("feature3 enabled: %d\n", (config & (1 << 2)) != 0);

    // Mask for alignment checking
    int vals[4];
    vals[0] = 16;
    vals[1] = 17;
    vals[2] = 32;
    vals[3] = 15;
    for (i = 0; i < 4; i++) {
        int aligned = (vals[i] & 0xF) == 0;
        printf("%d aligned to 16: %d\n", vals[i], aligned);
    }

    // Mask to wrap around (modulo power of 2)
    printf("wrap around 16:\n");
    for (i = 14; i < 20; i++) {
        printf("  %d mod 16 = %d\n", i, i & 15);
    }

    return 0;
}
