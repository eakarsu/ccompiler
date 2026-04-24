int printf(const char *fmt, ...);

int my_strlen(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

void my_strcpy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int is_palindrome(char *s) {
    int len = my_strlen(s);
    int i = 0;
    while (i < len / 2) {
        if (s[i] != s[len - 1 - i]) return 0;
        i = i + 1;
    }
    return 1;
}

char to_lower(char c) {
    if (c >= 'A' && c <= 'Z') return c + 32;
    return c;
}

int is_alpha(char c) {
    if (c >= 'a' && c <= 'z') return 1;
    if (c >= 'A' && c <= 'Z') return 1;
    return 0;
}

int is_palindrome_ignore_case(char *s) {
    char clean[256];
    int j = 0;
    int i = 0;
    while (s[i] != 0) {
        if (is_alpha(s[i])) {
            clean[j] = to_lower(s[i]);
            j = j + 1;
        }
        i = i + 1;
    }
    clean[j] = 0;
    return is_palindrome(clean);
}

int longest_palindrome_len(char *s) {
    int len = my_strlen(s);
    int best = 1;
    int i = 0;
    while (i < len) {
        int j = i;
        while (j < len) {
            int left = i;
            int right = j;
            int ok = 1;
            while (left < right) {
                if (s[left] != s[right]) {
                    ok = 0;
                    break;
                }
                left = left + 1;
                right = right - 1;
            }
            if (ok && (j - i + 1) > best) {
                best = j - i + 1;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return best;
}

int count_palindromic_substrings(char *s) {
    int len = my_strlen(s);
    int count = 0;
    int i = 0;
    while (i < len) {
        int j = i;
        while (j < len) {
            int left = i;
            int right = j;
            int ok = 1;
            while (left < right) {
                if (s[left] != s[right]) {
                    ok = 0;
                    break;
                }
                left = left + 1;
                right = right - 1;
            }
            if (ok) count = count + 1;
            j = j + 1;
        }
        i = i + 1;
    }
    return count;
}

void make_palindrome(char *dst, char *s) {
    int len = my_strlen(s);
    int i = 0;
    while (i < len) {
        dst[i] = s[i];
        i = i + 1;
    }
    int j = len - 2;
    while (j >= 0) {
        dst[i] = s[j];
        i = i + 1;
        j = j - 1;
    }
    dst[i] = 0;
}

int is_numeric_palindrome(int n) {
    if (n < 0) return 0;
    char buf[32];
    int i = 0;
    if (n == 0) {
        buf[0] = '0';
        buf[1] = 0;
    } else {
        int tmp = n;
        while (tmp > 0) {
            buf[i] = '0' + (tmp % 10);
            i = i + 1;
            tmp = tmp / 10;
        }
        buf[i] = 0;
        int a = 0;
        int b = i - 1;
        while (a < b) {
            char t = buf[a];
            buf[a] = buf[b];
            buf[b] = t;
            a = a + 1;
            b = b - 1;
        }
    }
    return is_palindrome(buf);
}

int main() {
    // EXPECT: pal racecar = 1
    printf("pal racecar = %d\n", is_palindrome("racecar"));
    // EXPECT: pal hello = 0
    printf("pal hello = %d\n", is_palindrome("hello"));
    // EXPECT: pal aba = 1
    printf("pal aba = %d\n", is_palindrome("aba"));
    // EXPECT: pal a = 1
    printf("pal a = %d\n", is_palindrome("a"));
    // EXPECT: pal abba = 1
    printf("pal abba = %d\n", is_palindrome("abba"));
    // EXPECT: pal ab = 0
    printf("pal ab = %d\n", is_palindrome("ab"));

    // EXPECT: pal_ic RaceCar = 1
    printf("pal_ic RaceCar = %d\n", is_palindrome_ignore_case("RaceCar"));
    // EXPECT: pal_ic Aba = 1
    printf("pal_ic Aba = %d\n", is_palindrome_ignore_case("Aba"));
    // EXPECT: pal_ic Hello = 0
    printf("pal_ic Hello = %d\n", is_palindrome_ignore_case("Hello"));
    // EXPECT: pal_ic A man a plan a canal Panama = 1
    printf("pal_ic A man a plan a canal Panama = %d\n", is_palindrome_ignore_case("A man a plan a canal Panama"));

    // EXPECT: longest pal in abacabad = 7
    printf("longest pal in abacabad = %d\n", longest_palindrome_len("abacabad"));
    // EXPECT: longest pal in abc = 1
    printf("longest pal in abc = %d\n", longest_palindrome_len("abc"));
    // EXPECT: longest pal in aaa = 3
    printf("longest pal in aaa = %d\n", longest_palindrome_len("aaa"));

    // EXPECT: count pal substr abc = 3
    printf("count pal substr abc = %d\n", count_palindromic_substrings("abc"));
    // EXPECT: count pal substr aaa = 6
    printf("count pal substr aaa = %d\n", count_palindromic_substrings("aaa"));
    // EXPECT: count pal substr aba = 4
    printf("count pal substr aba = %d\n", count_palindromic_substrings("aba"));

    char buf[64];
    make_palindrome(buf, "abc");
    // EXPECT: make pal abc = abcba
    printf("make pal abc = %s\n", buf);

    make_palindrome(buf, "hello");
    // EXPECT: make pal hello = hellolleh
    printf("make pal hello = %s\n", buf);

    // EXPECT: num pal 121 = 1
    printf("num pal 121 = %d\n", is_numeric_palindrome(121));
    // EXPECT: num pal 123 = 0
    printf("num pal 123 = %d\n", is_numeric_palindrome(123));
    // EXPECT: num pal 1001 = 1
    printf("num pal 1001 = %d\n", is_numeric_palindrome(1001));
    // EXPECT: num pal 0 = 1
    printf("num pal 0 = %d\n", is_numeric_palindrome(0));
    // EXPECT: num pal -121 = 0
    printf("num pal -121 = %d\n", is_numeric_palindrome(-121));

    return 0;
}
