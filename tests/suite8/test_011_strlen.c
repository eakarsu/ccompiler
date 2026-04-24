int printf(const char *fmt, ...);

int my_strlen(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

int my_strlen_ptr(char *s) {
    char *p = s;
    while (*p != 0) {
        p = p + 1;
    }
    return (int)(p - s);
}

int count_non_space(char *s) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] != ' ') {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

int count_vowels(char *s) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        char c = s[i];
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count = count + 1;
        }
        if (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

int last_index_of(char *s, char c) {
    int len = my_strlen(s);
    int i = len - 1;
    while (i >= 0) {
        if (s[i] == c) {
            return i;
        }
        i = i - 1;
    }
    return -1;
}

int word_count(char *s) {
    int count = 0;
    int in_word = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == ' ') {
            in_word = 0;
        } else {
            if (in_word == 0) {
                count = count + 1;
                in_word = 1;
            }
        }
        i = i + 1;
    }
    return count;
}

int is_alpha(char c) {
    if (c >= 'a' && c <= 'z') return 1;
    if (c >= 'A' && c <= 'Z') return 1;
    return 0;
}

int count_alpha(char *s) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (is_alpha(s[i])) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

int main() {
    char *s1 = "hello";
    char *s2 = "world test";
    char *s3 = "";
    char *s4 = "a";
    char *s5 = "Hello World";

    // EXPECT: strlen hello = 5
    printf("strlen hello = %d\n", my_strlen(s1));
    // EXPECT: strlen world test = 10
    printf("strlen world test = %d\n", my_strlen(s2));
    // EXPECT: strlen empty = 0
    printf("strlen empty = %d\n", my_strlen(s3));
    // EXPECT: strlen a = 1
    printf("strlen a = %d\n", my_strlen(s4));

    // EXPECT: strlen_ptr hello = 5
    printf("strlen_ptr hello = %d\n", my_strlen_ptr(s1));
    // EXPECT: strlen_ptr world test = 10
    printf("strlen_ptr world test = %d\n", my_strlen_ptr(s2));

    // EXPECT: non_space hello = 5
    printf("non_space hello = %d\n", count_non_space(s1));
    // EXPECT: non_space world test = 9
    printf("non_space world test = %d\n", count_non_space(s2));

    // EXPECT: vowels hello = 2
    printf("vowels hello = %d\n", count_vowels(s1));
    // EXPECT: vowels Hello World = 3
    printf("vowels Hello World = %d\n", count_vowels(s5));

    // EXPECT: last l in hello = 3
    printf("last l in hello = %d\n", last_index_of(s1, 'l'));
    // EXPECT: last z in hello = -1
    printf("last z in hello = %d\n", last_index_of(s1, 'z'));
    // EXPECT: last t in world test = 9
    printf("last t in world test = %d\n", last_index_of(s2, 't'));

    // EXPECT: words in hello = 1
    printf("words in hello = %d\n", word_count(s1));
    // EXPECT: words in world test = 2
    printf("words in world test = %d\n", word_count(s2));
    // EXPECT: words in Hello World = 2
    printf("words in Hello World = %d\n", word_count(s5));

    // EXPECT: alpha in Hello World = 10
    printf("alpha in Hello World = %d\n", count_alpha(s5));
    // EXPECT: alpha in empty = 0
    printf("alpha in empty = %d\n", count_alpha(s3));

    return 0;
}
