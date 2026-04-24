int printf(const char *fmt, ...);
// EXPECT: Hello, World!\nbuf = Testing\nlen of 'compiler' = 8\nchars: [c][o][m][p][i][l][e][r]\ncopied: abcdef\nvowels in 'hello world' = 3\nupper: ABCDE\nempty len = 1
// Test: string literals and char arrays

int main(void) {
    // String literal
    char *greeting = "Hello, World!";
    printf("%s\n", greeting);

    // Char array initialized character by character
    char buf[20];
    buf[0] = 'T'; buf[1] = 'e'; buf[2] = 's'; buf[3] = 't';
    buf[4] = 'i'; buf[5] = 'n'; buf[6] = 'g'; buf[7] = '\0';
    printf("buf = %s\n", buf);

    // String length manually
    char *s = "compiler";
    int len = 0;
    while (s[len] != '\0') {
        len = len + 1;
    }
    printf("len of '%s' = %d\n", s, len);

    // Print char by char
    printf("chars: ");
    int i;
    for (i = 0; i < len; i++) {
        printf("[%c]", s[i]);
    }
    printf("\n");

    // Copy string literal to char array
    char *src = "abcdef";
    char dest[10];
    i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i = i + 1;
    }
    dest[i] = '\0';
    printf("copied: %s\n", dest);

    // Count vowels
    char *text = "hello world";
    int vowels = 0;
    i = 0;
    while (text[i] != '\0') {
        char ch = text[i];
        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
            vowels = vowels + 1;
        }
        i = i + 1;
    }
    printf("vowels in '%s' = %d\n", text, vowels);

    // Upper case conversion
    char lower[6];
    lower[0] = 'a'; lower[1] = 'b'; lower[2] = 'c';
    lower[3] = 'd'; lower[4] = 'e'; lower[5] = '\0';
    printf("upper: ");
    for (i = 0; i < 5; i++) {
        printf("%c", lower[i] - 32);
    }
    printf("\n");

    // Empty string
    char empty[1];
    empty[0] = '\0';
    printf("empty len = %d\n", (int)(empty[0] == '\0'));

    return 0;
}
