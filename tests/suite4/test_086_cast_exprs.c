int printf(const char *fmt, ...);

int main(void) {
    // Cast int to char: truncation
    int big = 300;
    char small = (char)big;
    // 300 = 256 + 44, so truncated to 44
    // EXPECT: 44
    printf("%d\n", (int)small);

    // Cast negative int to char
    int neg = -1;
    char cneg = (char)neg;
    // -1 in two's complement all 1s, char is typically signed, so stays -1
    // EXPECT: -1
    printf("%d\n", (int)cneg);

    // Cast char to int (sign extension)
    char ch = -50;
    int extended = (int)ch;
    // EXPECT: -50
    printf("%d\n", extended);

    // Cast positive char to int
    char pch = 65;
    int pext = (int)pch;
    // EXPECT: 65
    printf("%d\n", pext);

    // Cast in arithmetic expression
    int a = 256 + 10;
    char b = (char)a;
    // 266 % 256 = 10
    // EXPECT: 10
    printf("%d\n", (int)b);

    // Cast result of addition
    int x = 200;
    int y = 200;
    char sum = (char)(x + y);
    // 400 = 256 + 144, 144 as signed char = 144 - 256 = -112
    // EXPECT: -112
    printf("%d\n", (int)sum);

    // Cast in comparison
    int v1 = 300;
    int v2 = 44;
    int eq = ((char)v1 == (char)v2);
    // both truncate to 44
    // EXPECT: 1
    printf("%d\n", eq);

    // Multiple casts
    int big_val = 1000;
    char first_cast = (char)big_val;
    // 1000 = 3*256 + 232, 232 as signed = 232-256 = -24
    int back = (int)first_cast;
    // EXPECT: -24
    printf("%d\n", back);

    // Cast in loop
    int total = 0;
    for (int i = 0; i < 5; i++) {
        char cv = (char)(i * 60);
        // i=0: 0, i=1: 60, i=2: 120, i=3: 180->180-256=-76, i=4: 240->240-256=-16
        total = total + (int)cv;
    }
    // 0 + 60 + 120 + (-76) + (-16) = 88
    // EXPECT: 88
    printf("%d\n", total);

    // Cast pointer to int pointer and back via void*
    int ival = 42;
    int *ip = &ival;
    int read_back = *ip;
    // EXPECT: 42
    printf("%d\n", read_back);

    // Cast char array access
    char str[4];
    str[0] = 72;
    str[1] = 101;
    str[2] = 108;
    str[3] = 0;
    // EXPECT: 72
    printf("%d\n", (int)str[0]);
    // EXPECT: 101
    printf("%d\n", (int)str[1]);
    // EXPECT: 108
    printf("%d\n", (int)str[2]);

    // Cast in ternary
    int tv = 500;
    int result = (tv > 255) ? (int)(char)tv : tv;
    // 500 = 256 + 244, 244 as signed char = -12
    // EXPECT: -12
    printf("%d\n", result);

    // Cast affects truncation in assignment
    int w = 128;
    char cw = (char)w;
    // 128 as signed char = -128
    // EXPECT: -128
    printf("%d\n", (int)cw);

    // Cast 0 and 255 boundary
    int z0 = 0;
    char cz0 = (char)z0;
    // EXPECT: 0
    printf("%d\n", (int)cz0);

    int z255 = 255;
    char cz255 = (char)z255;
    // 255 as signed char = -1
    // EXPECT: -1
    printf("%d\n", (int)cz255);

    int z127 = 127;
    char cz127 = (char)z127;
    // 127 fits in signed char
    // EXPECT: 127
    printf("%d\n", (int)cz127);

    // Repeated cast does not change value
    int orig = 50;
    int double_cast = (int)(char)(int)(char)orig;
    // 50 fits in char, so no change
    // EXPECT: 50
    printf("%d\n", double_cast);

    // Cast in struct member access context
    struct { int val; } s;
    s.val = 1000;
    char sv = (char)s.val;
    // 1000 mod 256 = 232, signed = -24
    // EXPECT: -24
    printf("%d\n", (int)sv);

    // Cast result of bitwise operation
    int bw = 0xABCD;
    char low_byte = (char)bw;
    // 0xCD = 205, signed char = 205 - 256 = -51
    // EXPECT: -51
    printf("%d\n", (int)low_byte);

    // Cast to char then shift
    int shift_val = 0xFF00;
    char shifted = (char)(shift_val >> 8);
    // 0xFF00 >> 8 = 0xFF = 255, signed char = -1
    // EXPECT: -1
    printf("%d\n", (int)shifted);

    return 0;
}
