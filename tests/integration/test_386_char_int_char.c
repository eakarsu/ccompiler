int printf(const char *fmt, ...);
// EXPECT: 'A' -> 65 -> 'A'\n'a' - 'a' + 'A' = 'A'\nfrom codes: Hello\nto codes: 72 101 108 108 111\nnegative: -50 -> -50 -> -50\nchar 100 + 100 = int 200, truncated = -56\nASCII transforms:\n  original: HeLlO\n  upper: HELLO\n  lower: hello\n  ROT13: NOP\n  ROT26: ABC
// Test: char->int->char conversion chains

int main(void) {
    // Basic char -> int -> char round trip
    char c1 = 'A';
    int i1 = c1;
    char c2 = (char)i1;
    printf("'A' -> %d -> '%c'\n", i1, c2);

    // Arithmetic in int domain, back to char
    char lower = 'a';
    int offset = lower - 'a';
    int upper_int = 'A' + offset;
    char upper = (char)upper_int;
    printf("'a' - 'a' + 'A' = '%c'\n", upper);

    // Build string from int codes
    int codes[5];
    codes[0] = 72;  // H
    codes[1] = 101; // e
    codes[2] = 108; // l
    codes[3] = 108; // l
    codes[4] = 111; // o
    char str[6];
    int i;
    for (i = 0; i < 5; i++) {
        str[i] = (char)codes[i];
    }
    str[5] = 0;
    printf("from codes: %s\n", str);

    // Extract codes from string
    printf("to codes: ");
    for (i = 0; i < 5; i++) {
        if (i > 0) printf(" ");
        printf("%d", (int)str[i]);
    }
    printf("\n");

    // Negative char values through int
    char neg = -50;
    int neg_int = neg;
    char neg_back = (char)neg_int;
    printf("negative: %d -> %d -> %d\n", (int)neg, neg_int, (int)neg_back);

    // Char math with promotion
    char a = 100;
    char b = 100;
    int sum = a + b;  // promoted to int
    char trunc = (char)sum;
    printf("char 100 + 100 = int %d, truncated = %d\n", sum, (int)trunc);

    // ASCII transformations
    printf("ASCII transforms:\n");
    char letters[6];
    letters[0] = 'H'; letters[1] = 'e'; letters[2] = 'L';
    letters[3] = 'l'; letters[4] = 'O'; letters[5] = 0;
    printf("  original: %s\n", letters);

    // To uppercase
    printf("  upper: ");
    for (i = 0; i < 5; i++) {
        int ch = letters[i];
        if (ch >= 'a' && ch <= 'z') ch = ch - 32;
        printf("%c", (char)ch);
    }
    printf("\n");

    // To lowercase
    printf("  lower: ");
    for (i = 0; i < 5; i++) {
        int ch = letters[i];
        if (ch >= 'A' && ch <= 'Z') ch = ch + 32;
        printf("%c", (char)ch);
    }
    printf("\n");

    // ROT13 through int domain
    char msg[4];
    msg[0] = 'A'; msg[1] = 'B'; msg[2] = 'C'; msg[3] = 0;
    printf("  ROT13: ");
    for (i = 0; i < 3; i++) {
        int ch = msg[i];
        if (ch >= 'A' && ch <= 'Z') {
            ch = ((ch - 'A' + 13) % 26) + 'A';
        }
        printf("%c", (char)ch);
    }
    printf("\n");

    // Double ROT13 = identity
    printf("  ROT26: ");
    for (i = 0; i < 3; i++) {
        int ch = msg[i];
        if (ch >= 'A' && ch <= 'Z') {
            ch = ((ch - 'A' + 26) % 26) + 'A';
        }
        printf("%c", (char)ch);
    }
    printf("\n");

    return 0;
}
