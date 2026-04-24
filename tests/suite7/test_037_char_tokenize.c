int printf(const char *fmt, ...);

int my_strlen(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

int tokenize(char *s, int delim, int *starts, int *lens, int max_tokens) {
    int count = 0;
    int i = 0;

    while (s[i] != 0 && count < max_tokens) {
        while (s[i] == delim) {
            i = i + 1;
        }
        if (s[i] == 0) break;

        starts[count] = i;
        int start = i;
        while (s[i] != 0 && s[i] != delim) {
            i = i + 1;
        }
        lens[count] = i - start;
        count = count + 1;
    }
    return count;
}

void extract_token(char *src, int start, int len, char *dst) {
    int i = 0;
    while (i < len) {
        dst[i] = src[start + i];
        i = i + 1;
    }
    dst[i] = 0;
}

int count_delimiters(char *s, int delim) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == delim) count = count + 1;
        i = i + 1;
    }
    return count;
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

int longest_token(char *s, int delim) {
    int starts[32];
    int lens[32];
    int n = tokenize(s, delim, starts, lens, 32);
    int maxlen = 0;
    int i = 0;
    while (i < n) {
        if (lens[i] > maxlen) maxlen = lens[i];
        i = i + 1;
    }
    return maxlen;
}

int shortest_token(char *s, int delim) {
    int starts[32];
    int lens[32];
    int n = tokenize(s, delim, starts, lens, 32);
    if (n == 0) return 0;
    int minlen = lens[0];
    int i = 1;
    while (i < n) {
        if (lens[i] < minlen) minlen = lens[i];
        i = i + 1;
    }
    return minlen;
}

int main(void) {
    char str[] = "hello world foo bar";
    int starts[16];
    int lens[16];

    int n = tokenize(str, ' ', starts, lens, 16);
    // EXPECT: tokens=4
    printf("tokens=%d\n", n);

    char tok[32];
    extract_token(str, starts[0], lens[0], tok);
    // EXPECT: tok0=hello
    printf("tok0=%s\n", tok);

    extract_token(str, starts[1], lens[1], tok);
    // EXPECT: tok1=world
    printf("tok1=%s\n", tok);

    extract_token(str, starts[2], lens[2], tok);
    // EXPECT: tok2=foo
    printf("tok2=%s\n", tok);

    extract_token(str, starts[3], lens[3], tok);
    // EXPECT: tok3=bar
    printf("tok3=%s\n", tok);

    // EXPECT: len0=5
    printf("len0=%d\n", lens[0]);

    // EXPECT: len1=5
    printf("len1=%d\n", lens[1]);

    // EXPECT: len2=3
    printf("len2=%d\n", lens[2]);

    // EXPECT: len3=3
    printf("len3=%d\n", lens[3]);

    char csv[] = "a,bb,ccc,dd";
    n = tokenize(csv, ',', starts, lens, 16);
    // EXPECT: csv_tokens=4
    printf("csv_tokens=%d\n", n);

    extract_token(csv, starts[0], lens[0], tok);
    // EXPECT: csv0=a
    printf("csv0=%s\n", tok);

    extract_token(csv, starts[2], lens[2], tok);
    // EXPECT: csv2=ccc
    printf("csv2=%s\n", tok);

    // EXPECT: delims=3
    printf("delims=%d\n", count_delimiters(csv, ','));

    // EXPECT: words=4
    printf("words=%d\n", word_count(str));

    char spaced[] = "  lots   of    spaces  ";
    // EXPECT: sp_words=3
    printf("sp_words=%d\n", word_count(spaced));

    n = tokenize(spaced, ' ', starts, lens, 16);
    // EXPECT: sp_tokens=3
    printf("sp_tokens=%d\n", n);

    extract_token(spaced, starts[0], lens[0], tok);
    // EXPECT: sp_tok0=lots
    printf("sp_tok0=%s\n", tok);

    // EXPECT: longest=5
    printf("longest=%d\n", longest_token(str, ' '));

    // EXPECT: shortest=3
    printf("shortest=%d\n", shortest_token(str, ' '));

    char single[] = "alone";
    n = tokenize(single, ' ', starts, lens, 16);
    // EXPECT: single_tok=1
    printf("single_tok=%d\n", n);

    extract_token(single, starts[0], lens[0], tok);
    // EXPECT: single_val=alone
    printf("single_val=%s\n", tok);

    char empty[] = "";
    n = tokenize(empty, ' ', starts, lens, 16);
    // EXPECT: empty_tok=0
    printf("empty_tok=%d\n", n);

    return 0;
}
