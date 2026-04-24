int printf(const char *fmt, ...);
// EXPECT: char_to_digit:\n  '0' = 0\n  '1' = 1\n  '2' = 2\n  '3' = 3\n  '4' = 4\n  '5' = 5\n  '6' = 6\n  '7' = 7\n  '8' = 8\n  '9' = 9\nchar values:\n  '0' = 48\n  '9' = 57\n  'A' = 65\n  'Z' = 90\n  'a' = 97\n  'z' = 122\nhex_char_to_int:\n  '0' = 0\n  '1' = 1\n  '2' = 2\n  '3' = 3\n  '4' = 4\n  '5' = 5\n  '6' = 6\n  '7' = 7\n  '8' = 8\n  '9' = 9\n  'A' = 10\n  'B' = 11\n  'C' = 12\n  'D' = 13\n  'E' = 14\n  'F' = 15\nstr_to_int:\n  "123" = 123\n  "-4567" = -4567\n  "0" = 0\ncase conversion:\n  'A' | 0x20 = 'a'\n  'z' & ~0x20 = 'Z'\nletter index:\n  'A' -> 0\n  'E' -> 4\n  'M' -> 12\n  'Z' -> 25\n  'a' -> 0
// Test: char to int conversions

int char_to_digit(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    return -1;
}

int hex_char_to_int(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

int str_to_int(char *s) {
    int result = 0;
    int sign = 1;
    int i = 0;
    if (s[0] == '-') {
        sign = -1;
        i = 1;
    }
    while (s[i] != 0) {
        result = result * 10 + (s[i] - '0');
        i++;
    }
    return result * sign;
}

int main(void) {
    // Basic char to int
    char digits[10];
    digits[0] = '0'; digits[1] = '1'; digits[2] = '2';
    digits[3] = '3'; digits[4] = '4'; digits[5] = '5';
    digits[6] = '6'; digits[7] = '7'; digits[8] = '8';
    digits[9] = '9';

    int i;
    printf("char_to_digit:\n");
    for (i = 0; i < 10; i++) {
        printf("  '%c' = %d\n", digits[i], char_to_digit(digits[i]));
    }

    // Char arithmetic values
    printf("char values:\n");
    printf("  '0' = %d\n", (int)'0');
    printf("  '9' = %d\n", (int)'9');
    printf("  'A' = %d\n", (int)'A');
    printf("  'Z' = %d\n", (int)'Z');
    printf("  'a' = %d\n", (int)'a');
    printf("  'z' = %d\n", (int)'z');

    // Hex char conversion
    printf("hex_char_to_int:\n");
    char hex_chars[16];
    hex_chars[0] = '0'; hex_chars[1] = '1'; hex_chars[2] = '2';
    hex_chars[3] = '3'; hex_chars[4] = '4'; hex_chars[5] = '5';
    hex_chars[6] = '6'; hex_chars[7] = '7'; hex_chars[8] = '8';
    hex_chars[9] = '9'; hex_chars[10] = 'A'; hex_chars[11] = 'B';
    hex_chars[12] = 'C'; hex_chars[13] = 'D'; hex_chars[14] = 'E';
    hex_chars[15] = 'F';
    for (i = 0; i < 16; i++) {
        printf("  '%c' = %d\n", hex_chars[i], hex_char_to_int(hex_chars[i]));
    }

    // String to integer conversion
    char s1[4]; s1[0] = '1'; s1[1] = '2'; s1[2] = '3'; s1[3] = 0;
    char s2[6]; s2[0] = '-'; s2[1] = '4'; s2[2] = '5'; s2[3] = '6'; s2[4] = '7'; s2[5] = 0;
    char s3[2]; s3[0] = '0'; s3[1] = 0;
    printf("str_to_int:\n");
    printf("  \"%s\" = %d\n", s1, str_to_int(s1));
    printf("  \"%s\" = %d\n", s2, str_to_int(s2));
    printf("  \"%s\" = %d\n", s3, str_to_int(s3));

    // Case conversion via bit manipulation
    printf("case conversion:\n");
    char upper = 'A';
    char lower = upper | 0x20;  // set bit 5 to convert to lowercase
    printf("  '%c' | 0x20 = '%c'\n", upper, lower);

    char lower2 = 'z';
    char upper2 = lower2 & ~0x20;  // clear bit 5 to convert to uppercase
    printf("  '%c' & ~0x20 = '%c'\n", lower2, upper2);

    // Convert letter to index (0-25)
    printf("letter index:\n");
    char letters[5];
    letters[0] = 'A'; letters[1] = 'E'; letters[2] = 'M';
    letters[3] = 'Z'; letters[4] = 'a';
    for (i = 0; i < 5; i++) {
        int idx = (letters[i] | 0x20) - 'a';
        printf("  '%c' -> %d\n", letters[i], idx);
    }

    return 0;
}
