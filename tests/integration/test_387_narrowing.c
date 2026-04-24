int printf(const char *fmt, ...);
// EXPECT: int to char:\n  0 -> 0\n  127 -> 127\n  128 -> -128\n  255 -> -1\n  256 -> 0\n  -1 -> -1\n  -128 -> -128\n  -129 -> 127\n(char)(1000+500) = -36\nnarrowing cycle: -6 -5 -4 -3 -2 -1 0 1 2 3 4 5\n0x12345678 bytes: 120 86 52 18\nchar(15*20) = char(300) = 44\n42 * 100 = 4200, narrowed = 104\nsuccessive: 0x12ABCDEF -> 0xCDEF -> 239\nnarrowing comparison:\n  (char)300 == (char)44: 1\n  300 == 44: 0\nnarrowed string: ABCDE
// Test: narrowing conversions

int main(void) {
    // Int to char narrowing
    printf("int to char:\n");
    int vals[8];
    vals[0] = 0; vals[1] = 127; vals[2] = 128; vals[3] = 255;
    vals[4] = 256; vals[5] = -1; vals[6] = -128; vals[7] = -129;

    int i;
    for (i = 0; i < 8; i++) {
        char c = (char)vals[i];
        printf("  %d -> %d\n", vals[i], (int)c);
    }

    // Narrowing in arithmetic
    int big1 = 1000;
    int big2 = 500;
    char small = (char)(big1 + big2);
    printf("(char)(1000+500) = %d\n", (int)small);

    // Narrowing in loop
    printf("narrowing cycle: ");
    int val = 250;
    for (i = 0; i < 12; i++) {
        char c = (char)val;
        if (i > 0) printf(" ");
        printf("%d", (int)c);
        val = val + 1;
    }
    printf("\n");

    // Narrowing preserves low bits
    int test = 0x12345678;
    char byte0 = (char)test;
    char byte1 = (char)(test >> 8);
    char byte2 = (char)(test >> 16);
    char byte3 = (char)(test >> 24);
    printf("0x12345678 bytes: %d %d %d %d\n",
           byte0 & 0xFF, byte1 & 0xFF, byte2 & 0xFF, byte3 & 0xFF);

    // Narrowing of multiplication result
    char a = 15;
    char b = 20;
    int product = a * b;  // promoted
    char narrow = (char)product;
    printf("char(15*20) = char(%d) = %d\n", product, (int)narrow);

    // Widening then narrowing
    char small_val = 42;
    int widened = small_val;  // widen to int
    widened = widened * 100;  // operate in int
    char narrowed = (char)widened;  // narrow back
    printf("42 * 100 = %d, narrowed = %d\n", widened, (int)narrowed);

    // Successive narrowing
    int n32 = 0x12ABCDEF;
    int n16 = n32 & 0xFFFF;
    char n8 = (char)n32;
    printf("successive: 0x%X -> 0x%X -> %d\n", n32, n16, n8 & 0xFF);

    // Narrowing comparison
    printf("narrowing comparison:\n");
    int x = 300;
    int y = 44;  // 300 mod 256 = 44
    char cx = (char)x;
    char cy = (char)y;
    printf("  (char)300 == (char)44: %d\n", cx == cy);
    printf("  300 == 44: %d\n", x == y);

    // Array of ints to chars
    int src[5];
    src[0] = 65; src[1] = 66; src[2] = 67; src[3] = 68; src[4] = 69;
    char dst[6];
    for (i = 0; i < 5; i++) dst[i] = (char)src[i];
    dst[5] = 0;
    printf("narrowed string: %s\n", dst);

    return 0;
}
