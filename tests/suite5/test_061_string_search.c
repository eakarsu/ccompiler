int printf(const char *fmt, ...);

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) {
        n = n + 1;
    }
    return n;
}

int str_eq(char *a, char *b, int len) {
    int i = 0;
    while (i < len) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return 1;
}

int find_first(char *text, char *pat) {
    int tlen = str_len(text);
    int plen = str_len(pat);
    if (plen > tlen) return -1;
    int i = 0;
    while (i <= tlen - plen) {
        if (str_eq(text + i, pat, plen)) return i;
        i = i + 1;
    }
    return -1;
}

int find_last(char *text, char *pat) {
    int tlen = str_len(text);
    int plen = str_len(pat);
    if (plen > tlen) return -1;
    int i = tlen - plen;
    while (i >= 0) {
        if (str_eq(text + i, pat, plen)) return i;
        i = i - 1;
    }
    return -1;
}

int count_occurrences(char *text, char *pat) {
    int tlen = str_len(text);
    int plen = str_len(pat);
    if (plen > tlen) return 0;
    int count = 0;
    int i = 0;
    while (i <= tlen - plen) {
        if (str_eq(text + i, pat, plen)) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

int find_char_first(char *s, char c) {
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == c) return i;
        i = i + 1;
    }
    return -1;
}

int find_char_last(char *s, char c) {
    int len = str_len(s);
    int i = len - 1;
    while (i >= 0) {
        if (s[i] == c) return i;
        i = i - 1;
    }
    return -1;
}

int contains(char *text, char *pat) {
    return find_first(text, pat) >= 0;
}

int starts_with(char *text, char *pat) {
    int plen = str_len(pat);
    if (plen > str_len(text)) return 0;
    return str_eq(text, pat, plen);
}

int ends_with(char *text, char *pat) {
    int tlen = str_len(text);
    int plen = str_len(pat);
    if (plen > tlen) return 0;
    return str_eq(text + tlen - plen, pat, plen);
}

int main(void) {
    char text[40];
    text[0] = 'h'; text[1] = 'e'; text[2] = 'l';
    text[3] = 'l'; text[4] = 'o'; text[5] = ' ';
    text[6] = 'w'; text[7] = 'o'; text[8] = 'r';
    text[9] = 'l'; text[10] = 'd'; text[11] = 0;

    char pat1[4];
    pat1[0] = 'l'; pat1[1] = 'l'; pat1[2] = 'o'; pat1[3] = 0;

    // EXPECT: find_first llo: 2
    printf("find_first llo: %d\n", find_first(text, pat1));

    // EXPECT: find_last llo: 2
    printf("find_last llo: %d\n", find_last(text, pat1));

    // EXPECT: count llo: 1
    printf("count llo: %d\n", count_occurrences(text, pat1));

    char pat2[2];
    pat2[0] = 'l'; pat2[1] = 0;

    // EXPECT: find_first l: 2
    printf("find_first l: %d\n", find_first(text, pat2));

    // EXPECT: find_last l: 9
    printf("find_last l: %d\n", find_last(text, pat2));

    // EXPECT: count l: 3
    printf("count l: %d\n", count_occurrences(text, pat2));

    char pat3[4];
    pat3[0] = 'x'; pat3[1] = 'y'; pat3[2] = 'z'; pat3[3] = 0;

    // EXPECT: find_first xyz: -1
    printf("find_first xyz: %d\n", find_first(text, pat3));

    // EXPECT: find_last xyz: -1
    printf("find_last xyz: %d\n", find_last(text, pat3));

    // EXPECT: count xyz: 0
    printf("count xyz: %d\n", count_occurrences(text, pat3));

    // EXPECT: char_first o: 4
    printf("char_first o: %d\n", find_char_first(text, 'o'));

    // EXPECT: char_last o: 7
    printf("char_last o: %d\n", find_char_last(text, 'o'));

    // EXPECT: char_first z: -1
    printf("char_first z: %d\n", find_char_first(text, 'z'));

    // EXPECT: contains llo: 1
    printf("contains llo: %d\n", contains(text, pat1));

    // EXPECT: contains xyz: 0
    printf("contains xyz: %d\n", contains(text, pat3));

    char pre[3];
    pre[0] = 'h'; pre[1] = 'e'; pre[2] = 0;

    // EXPECT: starts_with he: 1
    printf("starts_with he: %d\n", starts_with(text, pre));

    char suf[3];
    suf[0] = 'l'; suf[1] = 'd'; suf[2] = 0;

    // EXPECT: ends_with ld: 1
    printf("ends_with ld: %d\n", ends_with(text, suf));

    char suf2[3];
    suf2[0] = 'l'; suf2[1] = 'o'; suf2[2] = 0;

    // EXPECT: ends_with lo: 0
    printf("ends_with lo: %d\n", ends_with(text, suf2));

    char rep[30];
    rep[0] = 'a'; rep[1] = 'b'; rep[2] = 'a';
    rep[3] = 'b'; rep[4] = 'a'; rep[5] = 'b';
    rep[6] = 'a'; rep[7] = 0;

    char rpat[3];
    rpat[0] = 'a'; rpat[1] = 'b'; rpat[2] = 0;

    // EXPECT: count ab in abababa: 3
    printf("count ab in abababa: %d\n",
           count_occurrences(rep, rpat));

    // EXPECT: find_first ab in abababa: 0
    printf("find_first ab in abababa: %d\n",
           find_first(rep, rpat));

    // EXPECT: find_last ab in abababa: 4
    printf("find_last ab in abababa: %d\n",
           find_last(rep, rpat));

    return 0;
}
