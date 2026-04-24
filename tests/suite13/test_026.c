int printf(const char *fmt, ...);

int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
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

void count_frequencies(char *s, int *freq) {
    int i = 0;
    while (i < 26) {
        freq[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (s[i] != 0) {
        if (is_alpha(s[i])) {
            char c = to_lower(s[i]);
            freq[c - 'a'] = freq[c - 'a'] + 1;
        }
        i = i + 1;
    }
}

char find_most_frequent(int *freq) {
    int max_count = 0;
    int max_idx = 0;
    int i = 0;
    while (i < 26) {
        if (freq[i] > max_count) {
            max_count = freq[i];
            max_idx = i;
        }
        i = i + 1;
    }
    return 'a' + max_idx;
}

int count_unique_chars(int *freq) {
    int count = 0;
    int i = 0;
    while (i < 26) {
        if (freq[i] > 0) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

int total_alpha_count(int *freq) {
    int total = 0;
    int i = 0;
    while (i < 26) {
        total = total + freq[i];
        i = i + 1;
    }
    return total;
}

void print_histogram_bar(char c, int count) {
    char bar[51];
    int i = 0;
    while (i < count && i < 50) {
        bar[i] = '#';
        i = i + 1;
    }
    bar[i] = 0;
    printf("%c: %s (%d)\n", c, bar, count);
}

int find_first_unique(char *s) {
    int freq[26];
    count_frequencies(s, freq);
    int i = 0;
    while (s[i] != 0) {
        if (is_alpha(s[i])) {
            char c = to_lower(s[i]);
            if (freq[c - 'a'] == 1) {
                return i;
            }
        }
        i = i + 1;
    }
    return -1;
}

int are_same_distribution(char *a, char *b) {
    int freq_a[26];
    int freq_b[26];
    count_frequencies(a, freq_a);
    count_frequencies(b, freq_b);
    int i = 0;
    while (i < 26) {
        if (freq_a[i] != freq_b[i]) return 0;
        i = i + 1;
    }
    return 1;
}

int main() {
    int freq[26];

    count_frequencies("hello", freq);
    printf("h:%d e:%d l:%d o:%d\n", freq['h'-'a'], freq['e'-'a'], freq['l'-'a'], freq['o'-'a']);
    // EXPECT: h:1 e:1 l:2 o:1

    char mf = find_most_frequent(freq);
    printf("most frequent in hello: %c\n", mf);
    // EXPECT: most frequent in hello: l

    int uc = count_unique_chars(freq);
    printf("unique chars in hello: %d\n", uc);
    // EXPECT: unique chars in hello: 4

    int ta = total_alpha_count(freq);
    printf("total alpha in hello: %d\n", ta);
    // EXPECT: total alpha in hello: 5

    count_frequencies("aabbbcccc", freq);
    printf("a:%d b:%d c:%d\n", freq['a'-'a'], freq['b'-'a'], freq['c'-'a']);
    // EXPECT: a:2 b:3 c:4

    mf = find_most_frequent(freq);
    printf("most frequent in aabbbcccc: %c\n", mf);
    // EXPECT: most frequent in aabbbcccc: c

    count_frequencies("Hello World", freq);
    printf("h:%d e:%d l:%d o:%d w:%d r:%d d:%d\n",
           freq['h'-'a'], freq['e'-'a'], freq['l'-'a'],
           freq['o'-'a'], freq['w'-'a'], freq['r'-'a'], freq['d'-'a']);
    // EXPECT: h:1 e:1 l:3 o:2 w:1 r:1 d:1

    print_histogram_bar('a', 3);
    // EXPECT: a: ### (3)

    print_histogram_bar('b', 5);
    // EXPECT: b: ##### (5)

    print_histogram_bar('z', 0);
    // EXPECT: z:  (0)

    int idx = find_first_unique("aabcc");
    printf("first unique in aabcc: %d\n", idx);
    // EXPECT: first unique in aabcc: 2

    idx = find_first_unique("aabb");
    printf("first unique in aabb: %d\n", idx);
    // EXPECT: first unique in aabb: -1

    idx = find_first_unique("abcabc");
    printf("first unique in abcabc: %d\n", idx);
    // EXPECT: first unique in abcabc: -1

    idx = find_first_unique("xyzxy");
    printf("first unique in xyzxy: %d\n", idx);
    // EXPECT: first unique in xyzxy: 2

    int same = are_same_distribution("abc", "cba");
    printf("abc vs cba same: %d\n", same);
    // EXPECT: abc vs cba same: 1

    same = are_same_distribution("abc", "abd");
    printf("abc vs abd same: %d\n", same);
    // EXPECT: abc vs abd same: 0

    same = are_same_distribution("listen", "silent");
    printf("listen vs silent same: %d\n", same);
    // EXPECT: listen vs silent same: 1

    count_frequencies("zzzzz", freq);
    uc = count_unique_chars(freq);
    printf("unique in zzzzz: %d\n", uc);
    // EXPECT: unique in zzzzz: 1

    printf("done\n");
    // EXPECT: done

    return 0;
}
