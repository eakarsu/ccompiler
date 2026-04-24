int printf(const char *fmt, ...);
// EXPECT: nibble array: 0 1 2 3 4 5 6 7 8 9 A B C D E F\nnibble verify: 1\nDNA: ACGTACGT\npixels: 0 1 5 15 8 3 12 7 9 14\nstorage: 10 nibbles in 8 bytes vs 40 bytes\nafter overwrite [3]=0: 0\nneighbor [2] unchanged: 5\nneighbor [4] unchanged: 8
// Test: compact storage using bits

// Store 4-bit values (nibbles) in an int array
void set_nibble(int *arr, int index, int value) {
    int word = index / 8;  // 8 nibbles per int
    int pos = (index % 8) * 4;
    arr[word] = arr[word] & ~(0xF << pos);
    arr[word] = arr[word] | ((value & 0xF) << pos);
}

int get_nibble(int *arr, int index) {
    int word = index / 8;
    int pos = (index % 8) * 4;
    return (arr[word] >> pos) & 0xF;
}

// Store 2-bit values
void set_2bit(int *arr, int index, int value) {
    int word = index / 16;
    int pos = (index % 16) * 2;
    arr[word] = arr[word] & ~(0x3 << pos);
    arr[word] = arr[word] | ((value & 0x3) << pos);
}

int get_2bit(int *arr, int index) {
    int word = index / 16;
    int pos = (index % 16) * 2;
    return (arr[word] >> pos) & 0x3;
}

int main(void) {
    // Nibble storage: 16 nibbles in 2 ints
    int nibbles[2];
    nibbles[0] = 0; nibbles[1] = 0;

    int i;
    // Store values 0-15 in nibble array
    for (i = 0; i < 16; i++) {
        set_nibble(nibbles, i, i);
    }

    // Read back
    printf("nibble array: ");
    for (i = 0; i < 16; i++) {
        if (i > 0) printf(" ");
        printf("%X", get_nibble(nibbles, i));
    }
    printf("\n");

    // Verify
    int all_ok = 1;
    for (i = 0; i < 16; i++) {
        if (get_nibble(nibbles, i) != i) all_ok = 0;
    }
    printf("nibble verify: %d\n", all_ok);

    // 2-bit storage: DNA encoding (A=0, C=1, G=2, T=3)
    int dna[1];  // 16 bases in 1 int
    dna[0] = 0;

    // Encode ACGTACGT
    int sequence[8];
    sequence[0] = 0; sequence[1] = 1; sequence[2] = 2; sequence[3] = 3;
    sequence[4] = 0; sequence[5] = 1; sequence[6] = 2; sequence[7] = 3;

    for (i = 0; i < 8; i++) {
        set_2bit(dna, i, sequence[i]);
    }

    // Decode
    char bases[5];
    bases[0] = 'A'; bases[1] = 'C'; bases[2] = 'G'; bases[3] = 'T';
    bases[4] = 0;

    printf("DNA: ");
    for (i = 0; i < 8; i++) {
        int val = get_2bit(dna, i);
        printf("%c", bases[val]);
    }
    printf("\n");

    // Compact color palette (4 bits = 16 colors)
    int palette[2];
    palette[0] = 0; palette[1] = 0;

    // Store pixel colors
    int pixels[10];
    pixels[0] = 0; pixels[1] = 1; pixels[2] = 5; pixels[3] = 15;
    pixels[4] = 8; pixels[5] = 3; pixels[6] = 12; pixels[7] = 7;
    pixels[8] = 9; pixels[9] = 14;
    for (i = 0; i < 10; i++) {
        set_nibble(palette, i, pixels[i]);
    }

    printf("pixels: ");
    for (i = 0; i < 10; i++) {
        if (i > 0) printf(" ");
        printf("%d", get_nibble(palette, i));
    }
    printf("\n");

    // Space savings
    printf("storage: 10 nibbles in %d bytes vs %d bytes\n",
           2 * 4, 10 * 4);  // 2 ints vs 10 ints

    // Overwrite test
    set_nibble(palette, 3, 0);
    printf("after overwrite [3]=0: %d\n", get_nibble(palette, 3));
    printf("neighbor [2] unchanged: %d\n", get_nibble(palette, 2));
    printf("neighbor [4] unchanged: %d\n", get_nibble(palette, 4));

    return 0;
}
