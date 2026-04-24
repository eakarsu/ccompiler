int printf(const char *fmt, ...);

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) {
        n = n + 1;
    }
    return n;
}

int str_eq_n(char *a, char *b, int n) {
    int i = 0;
    while (i < n) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return 1;
}

int match_wildcard(char *pattern, char *text) {
    int pi = 0;
    int ti = 0;
    int plen = str_len(pattern);
    int tlen = str_len(text);

    while (pi < plen && ti < tlen) {
        if (pattern[pi] == 63) {
            pi = pi + 1;
            ti = ti + 1;
        } else if (pattern[pi] == text[ti]) {
            pi = pi + 1;
            ti = ti + 1;
        } else {
            return 0;
        }
    }
    return pi == plen && ti == tlen;
}

int prefix_match(char *text, char *prefix) {
    int plen = str_len(prefix);
    int tlen = str_len(text);
    if (plen > tlen) return 0;
    return str_eq_n(text, prefix, plen);
}

int suffix_match(char *text, char *suffix) {
    int slen = str_len(suffix);
    int tlen = str_len(text);
    if (slen > tlen) return 0;
    return str_eq_n(text + tlen - slen, suffix, slen);
}

int count_pattern_matches(char *text, char *pattern) {
    int tlen = str_len(text);
    int plen = str_len(pattern);
    if (plen > tlen) return 0;
    int count = 0;
    int i = 0;
    while (i <= tlen - plen) {
        char sub[40];
        int j = 0;
        while (j < plen) {
            sub[j] = text[i + j];
            j = j + 1;
        }
        sub[j] = 0;
        if (match_wildcard(pattern, sub)) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

int find_pattern(char *text, char *pattern) {
    int tlen = str_len(text);
    int plen = str_len(pattern);
    if (plen > tlen) return -1;
    int i = 0;
    while (i <= tlen - plen) {
        char sub[40];
        int j = 0;
        while (j < plen) {
            sub[j] = text[i + j];
            j = j + 1;
        }
        sub[j] = 0;
        if (match_wildcard(pattern, sub)) {
            return i;
        }
        i = i + 1;
    }
    return -1;
}

int match_exact(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

int main(void) {
    char t1[20];
    t1[0]='h'; t1[1]='e'; t1[2]='l';
    t1[3]='l'; t1[4]='o'; t1[5]=0;

    char p1[20];
    p1[0]='h'; p1[1]='e'; p1[2]='l';
    p1[3]='l'; p1[4]='o'; p1[5]=0;

    // EXPECT: exact match: 1
    printf("exact match: %d\n",
           match_wildcard(p1, t1));

    char p2[20];
    p2[0]='h'; p2[1]=63; p2[2]='l';
    p2[3]='l'; p2[4]='o'; p2[5]=0;

    // EXPECT: wildcard h?llo: 1
    printf("wildcard h?llo: %d\n",
           match_wildcard(p2, t1));

    char p3[20];
    p3[0]=63; p3[1]=63; p3[2]=63;
    p3[3]=63; p3[4]=63; p3[5]=0;

    // EXPECT: all wildcard ?????: 1
    printf("all wildcard ?????: %d\n",
           match_wildcard(p3, t1));

    char p4[20];
    p4[0]=63; p4[1]=63; p4[2]=63; p4[3]=0;

    // EXPECT: short pattern ???: 0
    printf("short pattern ???: %d\n",
           match_wildcard(p4, t1));

    char p5[20];
    p5[0]='h'; p5[1]='a'; p5[2]='l';
    p5[3]='l'; p5[4]='o'; p5[5]=0;

    // EXPECT: no match hallo: 0
    printf("no match hallo: %d\n",
           match_wildcard(p5, t1));

    char pre[4];
    pre[0]='h'; pre[1]='e'; pre[2]='l'; pre[3]=0;
    // EXPECT: prefix hel: 1
    printf("prefix hel: %d\n", prefix_match(t1, pre));

    char pre2[4];
    pre2[0]='e'; pre2[1]='l'; pre2[2]=0;
    // EXPECT: prefix el: 0
    printf("prefix el: %d\n", prefix_match(t1, pre2));

    char suf[4];
    suf[0]='l'; suf[1]='l'; suf[2]='o'; suf[3]=0;
    // EXPECT: suffix llo: 1
    printf("suffix llo: %d\n", suffix_match(t1, suf));

    char suf2[4];
    suf2[0]='l'; suf2[1]='l'; suf2[2]=0;
    // EXPECT: suffix ll: 0
    printf("suffix ll: %d\n", suffix_match(t1, suf2));

    char text2[20];
    text2[0]='a'; text2[1]='b'; text2[2]='c';
    text2[3]='a'; text2[4]='b'; text2[5]='c'; text2[6]=0;

    char wp[4];
    wp[0]=63; wp[1]='b'; wp[2]=63; wp[3]=0;

    // EXPECT: count ?b?: 2
    printf("count ?b?: %d\n",
           count_pattern_matches(text2, wp));

    // EXPECT: find ?b?: 0
    printf("find ?b?: %d\n",
           find_pattern(text2, wp));

    char wp2[4];
    wp2[0]='c'; wp2[1]=63; wp2[2]=0;

    // EXPECT: count c?: 1
    printf("count c?: %d\n",
           count_pattern_matches(text2, wp2));

    // EXPECT: find c?: 2
    printf("find c?: %d\n",
           find_pattern(text2, wp2));

    char wp3[4];
    wp3[0]='z'; wp3[1]=63; wp3[2]=0;

    // EXPECT: find z?: -1
    printf("find z?: %d\n",
           find_pattern(text2, wp3));

    char wp4[4];
    wp4[0]=63; wp4[1]=63; wp4[2]=0;
    // EXPECT: count ??: 5
    printf("count ??: %d\n",
           count_pattern_matches(text2, wp4));

    char single[5];
    single[0]='x'; single[1]=0;
    char sp[5];
    sp[0]='x'; sp[1]=0;
    // EXPECT: exact single: 1
    printf("exact single: %d\n",
           match_wildcard(sp, single));

    return 0;
}
