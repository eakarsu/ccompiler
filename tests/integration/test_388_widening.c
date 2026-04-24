int printf(const char *fmt, ...);
// EXPECT: char to int:\n  0 -> 0\n  1 -> 1\n  127 -> 127\n  128 -> -128\n  255 -> -1\n  65 -> 65\n  97 -> 97\n  48 -> 48\npositive preservation:\n  0-127 preserved: 1\nnegative sign extension:\n  char -1 -> int -1 (hex 0xFFFF)\n  char -50 -> int -50 (hex 0xFFCE)\n  char -100 -> int -100 (hex 0xFF9C)\n  char -128 -> int -128 (hex 0xFF80)\nwidening arithmetic:\n  100 + 50 = 150\n  100 * 50 = 5000\n  100 - 50 = 50\narr[char 3] = 40\nchar 100 < int 1000: 1\nchar 100 == int 100: 1\ncombined bytes: 0x78563412\nchar loop sum: 55\nchain: 42 -> 42 -> 1042
// Test: widening conversions

int main(void) {
    // Char to int widening
    printf("char to int:\n");
    char chars[8];
    chars[0] = 0;
    chars[1] = 1;
    chars[2] = 127;
    chars[3] = -128;
    chars[4] = -1;
    chars[5] = 65;   // 'A'
    chars[6] = 97;   // 'a'
    chars[7] = 48;   // '0'

    int i;
    for (i = 0; i < 8; i++) {
        int widened = chars[i];
        printf("  %d -> %d\n", chars[i] & 0xFF, widened);
    }

    // Widening preserves value for positive chars
    printf("positive preservation:\n");
    int all_ok = 1;
    for (i = 0; i < 128; i++) {
        char c = (char)i;
        int w = c;
        if (w != i) all_ok = 0;
    }
    printf("  0-127 preserved: %d\n", all_ok);

    // Widening with sign extension for negative chars
    printf("negative sign extension:\n");
    char neg_chars[4];
    neg_chars[0] = -1;
    neg_chars[1] = -50;
    neg_chars[2] = -100;
    neg_chars[3] = -128;
    for (i = 0; i < 4; i++) {
        int w = neg_chars[i];
        printf("  char %d -> int %d (hex 0x%X)\n",
               (int)neg_chars[i], w, w & 0xFFFF);
    }

    // Widening in expressions
    char a = 100;
    char b = 50;
    int sum = a + b;
    int product = a * b;
    int difference = a - b;
    printf("widening arithmetic:\n");
    printf("  100 + 50 = %d\n", sum);
    printf("  100 * 50 = %d\n", product);
    printf("  100 - 50 = %d\n", difference);

    // Widening for array indexing
    char idx = 3;
    int arr[5];
    arr[0] = 10; arr[1] = 20; arr[2] = 30; arr[3] = 40; arr[4] = 50;
    printf("arr[char 3] = %d\n", arr[idx]);

    // Widening in comparison
    char small = 100;
    int big = 1000;
    printf("char 100 < int 1000: %d\n", small < big);
    printf("char 100 == int 100: %d\n", small == 100);

    // Widening in function-like operations
    char bytes[4];
    bytes[0] = 0x12;
    bytes[1] = 0x34;
    bytes[2] = 0x56;
    bytes[3] = 0x78;
    int combined = ((bytes[3] & 0xFF) << 24) | ((bytes[2] & 0xFF) << 16) |
                   ((bytes[1] & 0xFF) << 8) | (bytes[0] & 0xFF);
    printf("combined bytes: 0x%X\n", combined);

    // Widening in loops
    printf("char loop sum: ");
    int total = 0;
    char c;
    for (c = 1; c <= 10; c++) {
        total = total + c;
    }
    printf("%d\n", total);

    // Widening chain
    char step1 = 42;
    int step2 = step1;
    int step3 = step2 + 1000;
    printf("chain: %d -> %d -> %d\n", (int)step1, step2, step3);

    return 0;
}
