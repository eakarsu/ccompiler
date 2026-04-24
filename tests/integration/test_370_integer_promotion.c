int printf(const char *fmt, ...);
// EXPECT: char 100 + char 50 = 150\nchar 10 * char 20 = 200\nchar 127 + char 1 = 128\nchar -1 < char 1: 1\n!0 = 1\n!1 = 0\nchar 0xAA << 8 = 0xAA00\n-char(42) = -42\n'A' + 32 = 97 ('a')\narr[char 3] = 40\nchar loop: 0 1 4 9 16\nchar 100 / char 7 = 14 rem 2\nchar 0xFF ^ char 0xAA = 0x55\nmax(char 10, char 20) = 20\n3*3 + 4*4 = 25 (5*5 = 25, pythagorean = 1)
// Test: integer promotion in expressions

int main(void) {
    // Char arithmetic promotes to int
    char a = 100;
    char b = 50;
    int sum = a + b;
    printf("char 100 + char 50 = %d\n", sum);

    // Char multiplication
    char c = 10;
    char d = 20;
    int product = c * d;
    printf("char 10 * char 20 = %d\n", product);

    // Overflow in char arithmetic (promoted to int first)
    char e = 127;
    char f = 1;
    int over = e + f;
    printf("char 127 + char 1 = %d\n", over);

    // Char comparison promotes to int
    char g = -1;
    char h = 1;
    printf("char -1 < char 1: %d\n", g < h);

    // Char in conditional
    char flag = 0;
    printf("!0 = %d\n", !flag);
    flag = 1;
    printf("!1 = %d\n", !flag);

    // Char bitwise operations promote to int
    char m = 0xAA;
    int shifted = m << 8;
    printf("char 0xAA << 8 = 0x%X\n", shifted & 0xFFFF);

    // Char negation
    char n = 42;
    int negated = -n;
    printf("-char(42) = %d\n", negated);

    // Mixed char and int arithmetic
    char ch = 65;  // 'A'
    int offset = 32;
    int result = ch + offset;
    printf("'A' + 32 = %d ('%c')\n", result, result);

    // Array indexing with char
    int arr[5];
    arr[0] = 10; arr[1] = 20; arr[2] = 30; arr[3] = 40; arr[4] = 50;
    char idx = 3;
    printf("arr[char 3] = %d\n", arr[idx]);

    // Char in loop counter
    printf("char loop: ");
    char i;
    for (i = 0; i < 5; i++) {
        if (i > 0) printf(" ");
        printf("%d", (int)i * (int)i);
    }
    printf("\n");

    // Char division
    char x = 100;
    char y = 7;
    int div_result = x / y;
    int mod_result = x % y;
    printf("char 100 / char 7 = %d rem %d\n", div_result, mod_result);

    // Char XOR
    char p = 0xFF;
    char q = 0xAA;
    int xor_result = p ^ q;
    printf("char 0xFF ^ char 0xAA = 0x%X\n", xor_result & 0xFF);

    // Promotion in ternary
    char t1 = 10;
    char t2 = 20;
    int tern = (t1 > t2) ? t1 : t2;
    printf("max(char 10, char 20) = %d\n", tern);

    // Multiple char operations
    char v1 = 3;
    char v2 = 4;
    char v3 = 5;
    int hyp2 = v1 * v1 + v2 * v2;
    printf("3*3 + 4*4 = %d (5*5 = %d, pythagorean = %d)\n",
           hyp2, v3 * v3, hyp2 == v3 * v3);

    return 0;
}
