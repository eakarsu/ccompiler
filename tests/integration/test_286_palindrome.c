int printf(const char *fmt, ...);
// EXPECT: racecar: 1\nhello: 0\nabba: 1\na: 1\nab: 0\naba: 1\nabcba: 1\nempty: 1\nabca almost: 1\nabc almost: 0\naba almost: 1\nabcbxa almost: 1\nlongest in 'babad': 3\nlongest in 'cbbd': 2\nlongest in 'racecar': 7\ngenerated: abccba palindrome=1
// Test: palindrome check

int my_strlen(char *s) {
    int i = 0;
    while (s[i] != '\0') i = i + 1;
    return i;
}

int is_palindrome(char *s) {
    int len = my_strlen(s);
    int i = 0;
    int j = len - 1;
    while (i < j) {
        if (s[i] != s[j]) return 0;
        i = i + 1;
        j = j - 1;
    }
    return 1;
}

// Check if string can be made palindrome by removing at most one char
int almost_palindrome(char *s) {
    int len = my_strlen(s);
    int i = 0;
    int j = len - 1;
    while (i < j) {
        if (s[i] != s[j]) {
            // Try skipping left or right
            int li = i + 1; int lj = j;
            int left_ok = 1;
            while (li < lj) {
                if (s[li] != s[lj]) { left_ok = 0; break; }
                li = li + 1; lj = lj - 1;
            }
            int ri = i; int rj = j - 1;
            int right_ok = 1;
            while (ri < rj) {
                if (s[ri] != s[rj]) { right_ok = 0; break; }
                ri = ri + 1; rj = rj - 1;
            }
            return left_ok || right_ok;
        }
        i = i + 1;
        j = j - 1;
    }
    return 1;
}

// Find longest palindromic substring length (brute force)
int longest_palindrome_len(char *s) {
    int len = my_strlen(s);
    int best = 1;
    int i; int j;
    for (i = 0; i < len; i++) {
        for (j = i + 1; j < len; j++) {
            // Check if s[i..j] is palindrome
            int lo = i; int hi = j;
            int ok = 1;
            while (lo < hi) {
                if (s[lo] != s[hi]) { ok = 0; break; }
                lo = lo + 1; hi = hi - 1;
            }
            if (ok && j - i + 1 > best) {
                best = j - i + 1;
            }
        }
    }
    return best;
}

int main(void) {
    printf("racecar: %d\n", is_palindrome("racecar"));
    printf("hello: %d\n", is_palindrome("hello"));
    printf("abba: %d\n", is_palindrome("abba"));
    printf("a: %d\n", is_palindrome("a"));
    printf("ab: %d\n", is_palindrome("ab"));
    printf("aba: %d\n", is_palindrome("aba"));
    printf("abcba: %d\n", is_palindrome("abcba"));

    // Empty string
    char empty[1];
    empty[0] = '\0';
    printf("empty: %d\n", is_palindrome(empty));

    // Almost palindrome
    printf("abca almost: %d\n", almost_palindrome("abca"));
    printf("abc almost: %d\n", almost_palindrome("abc"));
    printf("aba almost: %d\n", almost_palindrome("aba"));
    printf("abcbxa almost: %d\n", almost_palindrome("abcbxa"));

    // Longest palindromic substring
    printf("longest in 'babad': %d\n", longest_palindrome_len("babad"));
    printf("longest in 'cbbd': %d\n", longest_palindrome_len("cbbd"));
    printf("longest in 'racecar': %d\n", longest_palindrome_len("racecar"));

    // Generate palindrome from string
    char word[10];
    word[0]='a'; word[1]='b'; word[2]='c'; word[3]='\0';
    int wlen = my_strlen(word);
    char pal[20];
    int k;
    for (k = 0; k < wlen; k++) pal[k] = word[k];
    for (k = 0; k < wlen; k++) pal[wlen + k] = word[wlen - 1 - k];
    pal[2 * wlen] = '\0';
    printf("generated: %s palindrome=%d\n", pal, is_palindrome(pal));

    return 0;
}
