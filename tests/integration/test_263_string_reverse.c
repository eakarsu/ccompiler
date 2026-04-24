int printf(const char *fmt, ...);
// EXPECT: before: hello\nafter:  olleh\nsingle: x\nempty: ''\npalindrome reversed: abcba\ncopy reverse: world -> dlrow\nreversed words: tac god xof neh\ndouble reverse: test\ndigits reversed: 54321
// Test: string reversal

int my_strlen(char *s) {
    int i = 0;
    while (s[i] != '\0') i = i + 1;
    return i;
}

void my_strcpy(char *dst, char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = '\0';
}

void reverse_inplace(char *s) {
    int len = my_strlen(s);
    int i = 0;
    int j = len - 1;
    while (i < j) {
        char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        i = i + 1;
        j = j - 1;
    }
}

void reverse_copy(char *dst, char *src) {
    int len = my_strlen(src);
    int i;
    for (i = 0; i < len; i++) {
        dst[i] = src[len - 1 - i];
    }
    dst[len] = '\0';
}

int main(void) {
    // Reverse "hello"
    char s1[10];
    my_strcpy(s1, "hello");
    printf("before: %s\n", s1);
    reverse_inplace(s1);
    printf("after:  %s\n", s1);

    // Reverse single char
    char s2[10];
    my_strcpy(s2, "x");
    reverse_inplace(s2);
    printf("single: %s\n", s2);

    // Reverse empty
    char s3[10];
    s3[0] = '\0';
    reverse_inplace(s3);
    printf("empty: '%s'\n", s3);

    // Reverse palindrome
    char s4[10];
    my_strcpy(s4, "abcba");
    reverse_inplace(s4);
    printf("palindrome reversed: %s\n", s4);

    // Reverse copy
    char src[10];
    my_strcpy(src, "world");
    char dst[10];
    reverse_copy(dst, src);
    printf("copy reverse: %s -> %s\n", src, dst);

    // Reverse each word manually
    char words[4][10];
    my_strcpy(words[0], "cat");
    my_strcpy(words[1], "dog");
    my_strcpy(words[2], "fox");
    my_strcpy(words[3], "hen");

    printf("reversed words: ");
    int i;
    for (i = 0; i < 4; i++) {
        reverse_inplace(words[i]);
        if (i > 0) printf(" ");
        printf("%s", words[i]);
    }
    printf("\n");

    // Double reverse = original
    char s5[10];
    my_strcpy(s5, "test");
    reverse_inplace(s5);
    reverse_inplace(s5);
    printf("double reverse: %s\n", s5);

    // Reverse digits
    char digits[10];
    my_strcpy(digits, "12345");
    reverse_inplace(digits);
    printf("digits reversed: %s\n", digits);

    return 0;
}
