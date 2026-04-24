int printf(const char *fmt, ...);

int my_strlen(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

int str_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return (a[i] == b[i]) ? 1 : 0;
}

int starts_with(char *str, char *prefix) {
    int i = 0;
    while (prefix[i] != 0) {
        if (str[i] == 0 || str[i] != prefix[i]) return 0;
        i = i + 1;
    }
    return 1;
}

int ends_with(char *str, char *suffix) {
    int slen = my_strlen(str);
    int xlen = my_strlen(suffix);
    if (xlen > slen) return 0;
    int offset = slen - xlen;
    int i = 0;
    while (suffix[i] != 0) {
        if (str[offset + i] != suffix[i]) return 0;
        i = i + 1;
    }
    return 1;
}

int contains(char *str, char *sub) {
    int slen = my_strlen(str);
    int plen = my_strlen(sub);
    if (plen > slen) return 0;
    int i = 0;
    while (i <= slen - plen) {
        int j = 0;
        int match = 1;
        while (j < plen) {
            if (str[i + j] != sub[j]) {
                match = 0;
                break;
            }
            j = j + 1;
        }
        if (match) return 1;
        i = i + 1;
    }
    return 0;
}

int find_substr(char *str, char *sub) {
    int slen = my_strlen(str);
    int plen = my_strlen(sub);
    if (plen > slen) return -1;
    int i = 0;
    while (i <= slen - plen) {
        int j = 0;
        int match = 1;
        while (j < plen) {
            if (str[i + j] != sub[j]) {
                match = 0;
                break;
            }
            j = j + 1;
        }
        if (match) return i;
        i = i + 1;
    }
    return -1;
}

int count_substr(char *str, char *sub) {
    int slen = my_strlen(str);
    int plen = my_strlen(sub);
    if (plen == 0 || plen > slen) return 0;
    int count = 0;
    int i = 0;
    while (i <= slen - plen) {
        int j = 0;
        int match = 1;
        while (j < plen) {
            if (str[i + j] != sub[j]) {
                match = 0;
                break;
            }
            j = j + 1;
        }
        if (match) {
            count = count + 1;
            i = i + plen;
        } else {
            i = i + 1;
        }
    }
    return count;
}

int main(void) {
    char str[] = "hello world";

    // EXPECT: eq_yes=1
    printf("eq_yes=%d\n", str_equal("abc", "abc"));

    // EXPECT: eq_no=0
    printf("eq_no=%d\n", str_equal("abc", "abd"));

    // EXPECT: eq_len=0
    printf("eq_len=%d\n", str_equal("abc", "ab"));

    // EXPECT: prefix_hel=1
    printf("prefix_hel=%d\n", starts_with(str, "hel"));

    // EXPECT: prefix_hello=1
    printf("prefix_hello=%d\n", starts_with(str, "hello"));

    // EXPECT: prefix_world=0
    printf("prefix_world=%d\n", starts_with(str, "world"));

    // EXPECT: prefix_empty=1
    printf("prefix_empty=%d\n", starts_with(str, ""));

    // EXPECT: suffix_orld=1
    printf("suffix_orld=%d\n", ends_with(str, "orld"));

    // EXPECT: suffix_world=1
    printf("suffix_world=%d\n", ends_with(str, "world"));

    // EXPECT: suffix_hello=0
    printf("suffix_hello=%d\n", ends_with(str, "hello"));

    // EXPECT: suffix_long=0
    printf("suffix_long=%d\n", ends_with(str, "hello world extra"));

    // EXPECT: cont_lo_w=1
    printf("cont_lo_w=%d\n", contains(str, "lo w"));

    // EXPECT: cont_xyz=0
    printf("cont_xyz=%d\n", contains(str, "xyz"));

    // EXPECT: cont_hello=1
    printf("cont_hello=%d\n", contains(str, "hello"));

    // EXPECT: find_world=6
    printf("find_world=%d\n", find_substr(str, "world"));

    // EXPECT: find_hel=0
    printf("find_hel=%d\n", find_substr(str, "hel"));

    // EXPECT: find_xyz=-1
    printf("find_xyz=%d\n", find_substr(str, "xyz"));

    char rep[] = "abcabcabc";
    // EXPECT: count_abc=3
    printf("count_abc=%d\n", count_substr(rep, "abc"));

    // EXPECT: count_ab=3
    printf("count_ab=%d\n", count_substr(rep, "ab"));

    // EXPECT: count_x=0
    printf("count_x=%d\n", count_substr(rep, "x"));

    char sentence[] = "the cat sat on the mat";
    // EXPECT: find_cat=4
    printf("find_cat=%d\n", find_substr(sentence, "cat"));

    // EXPECT: find_mat=19
    printf("find_mat=%d\n", find_substr(sentence, "mat"));

    // EXPECT: count_the=2
    printf("count_the=%d\n", count_substr(sentence, "the"));

    // EXPECT: count_at=3
    printf("count_at=%d\n", count_substr(sentence, "at"));

    return 0;
}
