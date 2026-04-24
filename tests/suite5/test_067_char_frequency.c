int printf(const char *fmt, ...);

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) {
        n = n + 1;
    }
    return n;
}

void count_freq(char *s, int freq[128]) {
    int i = 0;
    while (i < 128) {
        freq[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (s[i] != 0) {
        int idx = (int)s[i];
        if (idx >= 0 && idx < 128) {
            freq[idx] = freq[idx] + 1;
        }
        i = i + 1;
    }
}

int most_common(int freq[128]) {
    int best = -1;
    int best_count = 0;
    int i = 0;
    while (i < 128) {
        if (freq[i] > best_count) {
            best_count = freq[i];
            best = i;
        }
        i = i + 1;
    }
    return best;
}

int count_distinct(int freq[128]) {
    int count = 0;
    int i = 0;
    while (i < 128) {
        if (freq[i] > 0) count = count + 1;
        i = i + 1;
    }
    return count;
}

int is_anagram(char *a, char *b) {
    int fa[128];
    int fb[128];
    count_freq(a, fa);
    count_freq(b, fb);
    int i = 0;
    while (i < 128) {
        if (fa[i] != fb[i]) return 0;
        i = i + 1;
    }
    return 1;
}

int max_freq(int freq[128]) {
    int m = 0;
    int i = 0;
    while (i < 128) {
        if (freq[i] > m) m = freq[i];
        i = i + 1;
    }
    return m;
}

int count_letters(int freq[128]) {
    int count = 0;
    int i = 97;
    while (i <= 122) {
        count = count + freq[i];
        i = i + 1;
    }
    return count;
}

int count_digits(int freq[128]) {
    int count = 0;
    int i = 48;
    while (i <= 57) {
        count = count + freq[i];
        i = i + 1;
    }
    return count;
}

int count_spaces(int freq[128]) {
    return freq[32];
}

int main(void) {
    int freq[128];

    char s1[30];
    s1[0]='h'; s1[1]='e'; s1[2]='l';
    s1[3]='l'; s1[4]='o'; s1[5]=0;

    count_freq(s1, freq);

    // EXPECT: freq h: 1
    printf("freq h: %d\n", freq[104]);
    // EXPECT: freq e: 1
    printf("freq e: %d\n", freq[101]);
    // EXPECT: freq l: 2
    printf("freq l: %d\n", freq[108]);
    // EXPECT: freq o: 1
    printf("freq o: %d\n", freq[111]);
    // EXPECT: freq z: 0
    printf("freq z: %d\n", freq[122]);

    // EXPECT: most common: 108
    printf("most common: %d\n", most_common(freq));

    // EXPECT: distinct: 4
    printf("distinct: %d\n", count_distinct(freq));

    // EXPECT: max_freq: 2
    printf("max_freq: %d\n", max_freq(freq));

    char s2[20];
    s2[0]='a'; s2[1]='a'; s2[2]='b';
    s2[3]='b'; s2[4]='b'; s2[5]=0;

    count_freq(s2, freq);
    // EXPECT: freq a: 2
    printf("freq a: %d\n", freq[97]);
    // EXPECT: freq b: 3
    printf("freq b: %d\n", freq[98]);
    // EXPECT: most common2: 98
    printf("most common2: %d\n", most_common(freq));

    char a1[10];
    a1[0]='l'; a1[1]='i'; a1[2]='s';
    a1[3]='t'; a1[4]='e'; a1[5]='n'; a1[6]=0;

    char a2[10];
    a2[0]='s'; a2[1]='i'; a2[2]='l';
    a2[3]='e'; a2[4]='n'; a2[5]='t'; a2[6]=0;

    // EXPECT: anagram listen/silent: 1
    printf("anagram listen/silent: %d\n",
           is_anagram(a1, a2));

    char a3[10];
    a3[0]='h'; a3[1]='e'; a3[2]='l';
    a3[3]='l'; a3[4]='o'; a3[5]=0;

    // EXPECT: anagram hello/silent: 0
    printf("anagram hello/silent: %d\n",
           is_anagram(a3, a2));

    char mixed[20];
    mixed[0]='a'; mixed[1]='1'; mixed[2]=' ';
    mixed[3]='b'; mixed[4]='2'; mixed[5]=0;

    count_freq(mixed, freq);
    // EXPECT: letters: 2
    printf("letters: %d\n", count_letters(freq));
    // EXPECT: digits: 2
    printf("digits: %d\n", count_digits(freq));
    // EXPECT: spaces: 1
    printf("spaces: %d\n", count_spaces(freq));

    // EXPECT: distinct mixed: 5
    printf("distinct mixed: %d\n",
           count_distinct(freq));

    return 0;
}
