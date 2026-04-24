int printf(const char *fmt, ...);
// EXPECT: listen/silent: 1\nhello/world: 0\nabc/cba: 1\naab/baa: 1\nab/abc: 0\na/a: 1\nempty/empty: 1\nsorted listen: eilnst\nsorted silent: eilnst\nequal: 1\nanagram pairs:\n  eat - tea\n  eat - ate\n  tea - ate\n  tan - nat\nanagrams of 'eat': 2
// Test: anagram check

int my_strlen(char *s) {
    int i = 0;
    while (s[i] != '\0') i = i + 1;
    return i;
}

int is_anagram(char *a, char *b) {
    int freq[26];
    int i;
    for (i = 0; i < 26; i++) freq[i] = 0;

    i = 0;
    while (a[i] != '\0') {
        if (a[i] >= 'a' && a[i] <= 'z')
            freq[a[i] - 'a'] = freq[a[i] - 'a'] + 1;
        i = i + 1;
    }
    i = 0;
    while (b[i] != '\0') {
        if (b[i] >= 'a' && b[i] <= 'z')
            freq[b[i] - 'a'] = freq[b[i] - 'a'] - 1;
        i = i + 1;
    }
    for (i = 0; i < 26; i++) {
        if (freq[i] != 0) return 0;
    }
    return 1;
}

// Group anagrams: given words, find which pairs are anagrams
void sort_chars(char *s, char *out) {
    int len = my_strlen(s);
    int i;
    for (i = 0; i < len; i++) out[i] = s[i];
    out[len] = '\0';

    // Bubble sort chars
    int j;
    for (i = 0; i < len - 1; i++) {
        for (j = 0; j < len - 1 - i; j++) {
            if (out[j] > out[j + 1]) {
                char tmp = out[j];
                out[j] = out[j + 1];
                out[j + 1] = tmp;
            }
        }
    }
}

int str_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == '\0' && b[i] == '\0';
}

int main(void) {
    // Basic anagram tests
    printf("listen/silent: %d\n", is_anagram("listen", "silent"));
    printf("hello/world: %d\n", is_anagram("hello", "world"));
    printf("abc/cba: %d\n", is_anagram("abc", "cba"));
    printf("aab/baa: %d\n", is_anagram("aab", "baa"));
    printf("ab/abc: %d\n", is_anagram("ab", "abc"));
    printf("a/a: %d\n", is_anagram("a", "a"));

    // Empty strings
    char e[1]; e[0] = '\0';
    printf("empty/empty: %d\n", is_anagram(e, e));

    // Sort characters approach
    char sorted1[20]; char sorted2[20];
    sort_chars("listen", sorted1);
    sort_chars("silent", sorted2);
    printf("sorted listen: %s\n", sorted1);
    printf("sorted silent: %s\n", sorted2);
    printf("equal: %d\n", str_equal(sorted1, sorted2));

    // Find anagram pairs among words
    char *words[5];
    words[0] = "eat";
    words[1] = "tea";
    words[2] = "tan";
    words[3] = "ate";
    words[4] = "nat";

    printf("anagram pairs:\n");
    int i; int j;
    for (i = 0; i < 5; i++) {
        for (j = i + 1; j < 5; j++) {
            if (is_anagram(words[i], words[j])) {
                printf("  %s - %s\n", words[i], words[j]);
            }
        }
    }

    // Count anagrams of word in list
    int count = 0;
    for (i = 1; i < 5; i++) {
        if (is_anagram(words[0], words[i])) count = count + 1;
    }
    printf("anagrams of '%s': %d\n", words[0], count);

    return 0;
}
