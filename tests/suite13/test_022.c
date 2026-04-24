int printf(const char *fmt, ...);

int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

void str_copy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int is_alpha(char c) {
    if (c >= 'a' && c <= 'z') return 1;
    if (c >= 'A' && c <= 'Z') return 1;
    return 0;
}

char to_lower(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + 32;
    }
    return c;
}

void preprocess(char *dst, char *src) {
    int si = 0;
    int di = 0;
    while (src[si] != 0) {
        if (is_alpha(src[si])) {
            dst[di] = to_lower(src[si]);
            di = di + 1;
        }
        si = si + 1;
    }
    dst[di] = 0;
}

int is_palindrome_simple(char *s) {
    int len = str_len(s);
    int i = 0;
    while (i < len / 2) {
        if (s[i] != s[len - 1 - i]) {
            return 0;
        }
        i = i + 1;
    }
    return 1;
}

int is_palindrome_preprocessed(char *s) {
    char clean[100];
    preprocess(clean, s);
    return is_palindrome_simple(clean);
}

int is_palindrome_range(char *s, int start, int end) {
    while (start < end) {
        if (s[start] != s[end]) {
            return 0;
        }
        start = start + 1;
        end = end - 1;
    }
    return 1;
}

int longest_palindrome_substr_len(char *s) {
    int len = str_len(s);
    int best = 1;
    int i = 0;
    while (i < len) {
        int j = i;
        while (j < len) {
            if (j - i + 1 > best) {
                if (is_palindrome_range(s, i, j)) {
                    best = j - i + 1;
                }
            }
            j = j + 1;
        }
        i = i + 1;
    }
    if (len == 0) return 0;
    return best;
}

int count_palindromic_substrings(char *s) {
    int len = str_len(s);
    int count = 0;
    int i = 0;
    while (i < len) {
        int j = i;
        while (j < len) {
            if (is_palindrome_range(s, i, j)) {
                count = count + 1;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return count;
}

int main() {
    printf("%d\n", is_palindrome_simple("racecar"));
    // EXPECT: 1

    printf("%d\n", is_palindrome_simple("hello"));
    // EXPECT: 0

    printf("%d\n", is_palindrome_simple("madam"));
    // EXPECT: 1

    printf("%d\n", is_palindrome_simple("a"));
    // EXPECT: 1

    printf("%d\n", is_palindrome_simple("ab"));
    // EXPECT: 0

    printf("%d\n", is_palindrome_simple("abba"));
    // EXPECT: 1

    printf("%d\n", is_palindrome_preprocessed("Race Car"));
    // EXPECT: 1

    printf("%d\n", is_palindrome_preprocessed("A man a plan a canal Panama"));
    // EXPECT: 1

    printf("%d\n", is_palindrome_preprocessed("Hello World"));
    // EXPECT: 0

    printf("%d\n", is_palindrome_preprocessed("Was it a car or a cat I saw"));
    // EXPECT: 1

    printf("%d\n", is_palindrome_preprocessed("No lemon no melon"));
    // EXPECT: 1

    char clean[100];
    preprocess(clean, "RaCeCaR!");
    printf("%s\n", clean);
    // EXPECT: racecar

    preprocess(clean, "A B C");
    printf("%s\n", clean);
    // EXPECT: abc

    int lp = longest_palindrome_substr_len("babad");
    printf("longest palindrome in babad: %d\n", lp);
    // EXPECT: longest palindrome in babad: 3

    lp = longest_palindrome_substr_len("cbbd");
    printf("longest palindrome in cbbd: %d\n", lp);
    // EXPECT: longest palindrome in cbbd: 2

    lp = longest_palindrome_substr_len("abacaba");
    printf("longest palindrome in abacaba: %d\n", lp);
    // EXPECT: longest palindrome in abacaba: 7

    int cp = count_palindromic_substrings("abc");
    printf("palindromic substrings in abc: %d\n", cp);
    // EXPECT: palindromic substrings in abc: 3

    cp = count_palindromic_substrings("aaa");
    printf("palindromic substrings in aaa: %d\n", cp);
    // EXPECT: palindromic substrings in aaa: 6

    cp = count_palindromic_substrings("abba");
    printf("palindromic substrings in abba: %d\n", cp);
    // EXPECT: palindromic substrings in abba: 6

    printf("done\n");
    // EXPECT: done

    return 0;
}
