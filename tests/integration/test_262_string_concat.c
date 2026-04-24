int printf(const char *fmt, ...);
// EXPECT: after first: Hello\nafter concat: Hello World\nlen = 11\nsentence: The quick brown fox jumps\ndigits: 01234\nrepeat: ababababab (len=10)\nwith empty: startend
// Test: string concatenation (manual strcat)

int my_strlen(char *s) {
    int i = 0;
    while (s[i] != '\0') i = i + 1;
    return i;
}

void my_strcat(char *dst, char *src) {
    int dlen = my_strlen(dst);
    int i = 0;
    while (src[i] != '\0') {
        dst[dlen + i] = src[i];
        i = i + 1;
    }
    dst[dlen + i] = '\0';
}

void my_strcpy(char *dst, char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = '\0';
}

int main(void) {
    // Basic concat
    char buf[50];
    buf[0] = '\0';
    my_strcat(buf, "Hello");
    printf("after first: %s\n", buf);

    my_strcat(buf, " ");
    my_strcat(buf, "World");
    printf("after concat: %s\n", buf);
    printf("len = %d\n", my_strlen(buf));

    // Concat multiple words
    char sentence[100];
    sentence[0] = '\0';
    char *words[5];
    words[0] = "The";
    words[1] = "quick";
    words[2] = "brown";
    words[3] = "fox";
    words[4] = "jumps";

    int i;
    for (i = 0; i < 5; i++) {
        if (i > 0) my_strcat(sentence, " ");
        my_strcat(sentence, words[i]);
    }
    printf("sentence: %s\n", sentence);

    // Concat numbers as chars
    char num_str[20];
    num_str[0] = '\0';
    for (i = 0; i < 5; i++) {
        char digit[2];
        digit[0] = '0' + i;
        digit[1] = '\0';
        my_strcat(num_str, digit);
    }
    printf("digits: %s\n", num_str);

    // Build a repeated string
    char repeat[40];
    repeat[0] = '\0';
    for (i = 0; i < 5; i++) {
        my_strcat(repeat, "ab");
    }
    printf("repeat: %s (len=%d)\n", repeat, my_strlen(repeat));

    // Concat with empty
    char test[20];
    my_strcpy(test, "start");
    my_strcat(test, "");
    my_strcat(test, "end");
    printf("with empty: %s\n", test);

    return 0;
}
