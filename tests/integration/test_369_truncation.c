int printf(const char *fmt, ...);
// EXPECT: 256 as char = 0\n300 as char = 44\n255 as char = -1\n128 as char = -128\n127 as char = 127\nmask truncation:\n  0x1FF -> byte=0xFF short=0x1FF\n  0x100 -> byte=0x0 short=0x100\n  0xABCD -> byte=0xCD short=0xABCD\n  0x345678 -> byte=0x78 short=0x5678\n  0xFFFFFF -> byte=0xFF short=0xFFFF\ntruncation cycle:\n  250 -> -6\n  251 -> -5\n  252 -> -4\n  253 -> -3\n  254 -> -2\n  255 -> -1\n  256 -> 0\n  257 -> 1\n  258 -> 2\n  259 -> 3\ndecompose 0xDEADBEEF:\n  byte0 = 0xEF\n  byte1 = 0xBE\n  byte2 = 0xAD\nround-trip:\n  42 -> char -> 42 (same=1)\n  200 -> char -> -56 (same=0)\n  -50 -> char -> -50 (same=1)\n  1000 -> char -> -24 (same=0)
// Test: truncation behavior

int main(void) {
    // Int to char truncation
    int big = 256;
    char c = (char)big;
    printf("256 as char = %d\n", (int)c);

    int big2 = 300;
    char c2 = (char)big2;
    printf("300 as char = %d\n", (int)c2);

    int big3 = 255;
    char c3 = (char)big3;
    printf("255 as char = %d\n", (int)c3);

    int big4 = 128;
    char c4 = (char)big4;
    printf("128 as char = %d\n", (int)c4);

    int big5 = 127;
    char c5 = (char)big5;
    printf("127 as char = %d\n", (int)c5);

    // Masking as explicit truncation
    printf("mask truncation:\n");
    int vals[5];
    vals[0] = 0x1FF;
    vals[1] = 0x100;
    vals[2] = 0xABCD;
    vals[3] = 0x12345678;
    vals[4] = -1;

    int i;
    for (i = 0; i < 5; i++) {
        int to_byte = vals[i] & 0xFF;
        int to_short = vals[i] & 0xFFFF;
        printf("  0x%X -> byte=0x%X short=0x%X\n",
               vals[i] & 0xFFFFFF, to_byte, to_short);
    }

    // Truncation in loops
    printf("truncation cycle:\n");
    int counter = 250;
    for (i = 0; i < 10; i++) {
        char trunc = (char)counter;
        printf("  %d -> %d\n", counter, (int)trunc);
        counter = counter + 1;
    }

    // Truncation preserves lower bits
    int val = 0xDEADBEEF;
    int byte0 = val & 0xFF;
    int byte1 = (val >> 8) & 0xFF;
    int byte2 = (val >> 16) & 0xFF;
    printf("decompose 0xDEADBEEF:\n");
    printf("  byte0 = 0x%X\n", byte0);
    printf("  byte1 = 0x%X\n", byte1);
    printf("  byte2 = 0x%X\n", byte2);

    // Round-trip truncation and extension
    printf("round-trip:\n");
    int test_vals[4];
    test_vals[0] = 42;
    test_vals[1] = 200;
    test_vals[2] = -50;
    test_vals[3] = 1000;
    for (i = 0; i < 4; i++) {
        char trunc = (char)test_vals[i];
        int extended = (int)trunc;
        int matches = (test_vals[i] == extended);
        printf("  %d -> char -> %d (same=%d)\n",
               test_vals[i], extended, matches);
    }

    return 0;
}
