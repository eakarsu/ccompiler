int printf(const char *fmt, ...);

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) {
        n = n + 1;
    }
    return n;
}

int split_by_char(char *s, char delim,
                  int starts[20], int lengths[20]) {
    int count = 0;
    int i = 0;
    int len = str_len(s);
    while (i < len) {
        if (s[i] == delim) {
            i = i + 1;
            continue;
        }
        starts[count] = i;
        int j = i;
        while (j < len && s[j] != delim) {
            j = j + 1;
        }
        lengths[count] = j - i;
        count = count + 1;
        i = j;
    }
    return count;
}

int word_count(char *s) {
    int count = 0;
    int in_word = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == 32) {
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

void extract(char *src, int start, int length,
             char *dst) {
    int i = 0;
    while (i < length) {
        dst[i] = src[start + i];
        i = i + 1;
    }
    dst[i] = 0;
}

int split_by_space(char *s, int starts[20],
                   int lengths[20]) {
    return split_by_char(s, 32, starts, lengths);
}

int count_delimiters(char *s, char delim) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == delim) count = count + 1;
        i = i + 1;
    }
    return count;
}

int find_nth_word_start(char *s, int n) {
    int starts[20];
    int lengths[20];
    int count = split_by_space(s, starts, lengths);
    if (n >= count) return -1;
    return starts[n];
}

int main(void) {
    char s1[30];
    s1[0]='h'; s1[1]='e'; s1[2]='l';
    s1[3]='l'; s1[4]='o'; s1[5]=' ';
    s1[6]='w'; s1[7]='o'; s1[8]='r';
    s1[9]='l'; s1[10]='d'; s1[11]=0;

    int starts[20];
    int lengths[20];

    int n = split_by_space(s1, starts, lengths);
    // EXPECT: word_count: 2
    printf("word_count: %d\n", n);

    // EXPECT: w0 start: 0
    printf("w0 start: %d\n", starts[0]);
    // EXPECT: w0 len: 5
    printf("w0 len: %d\n", lengths[0]);
    // EXPECT: w1 start: 6
    printf("w1 start: %d\n", starts[1]);
    // EXPECT: w1 len: 5
    printf("w1 len: %d\n", lengths[1]);

    char w[20];
    extract(s1, starts[0], lengths[0], w);
    // EXPECT: word0: hello
    printf("word0: %s\n", w);

    extract(s1, starts[1], lengths[1], w);
    // EXPECT: word1: world
    printf("word1: %s\n", w);

    char s2[40];
    s2[0]='a'; s2[1]=','; s2[2]='b';
    s2[3]='b'; s2[4]=','; s2[5]='c';
    s2[6]='c'; s2[7]='c'; s2[8]=0;

    n = split_by_char(s2, ',', starts, lengths);
    // EXPECT: csv parts: 3
    printf("csv parts: %d\n", n);
    // EXPECT: csv0 len: 1
    printf("csv0 len: %d\n", lengths[0]);
    // EXPECT: csv1 len: 2
    printf("csv1 len: %d\n", lengths[1]);
    // EXPECT: csv2 len: 3
    printf("csv2 len: %d\n", lengths[2]);

    extract(s2, starts[2], lengths[2], w);
    // EXPECT: csv2: ccc
    printf("csv2: %s\n", w);

    // EXPECT: word_count s1: 2
    printf("word_count s1: %d\n", word_count(s1));

    char s3[40];
    s3[0]=' '; s3[1]=' '; s3[2]='a';
    s3[3]=' '; s3[4]='b'; s3[5]=' ';
    s3[6]=' '; s3[7]='c'; s3[8]=' '; s3[9]=0;

    // EXPECT: word_count s3: 3
    printf("word_count s3: %d\n", word_count(s3));

    n = split_by_space(s3, starts, lengths);
    // EXPECT: split s3 parts: 3
    printf("split s3 parts: %d\n", n);

    // EXPECT: delimiters comma s2: 2
    printf("delimiters comma s2: %d\n",
           count_delimiters(s2, ','));

    // EXPECT: delimiters space s1: 1
    printf("delimiters space s1: %d\n",
           count_delimiters(s1, ' '));

    // EXPECT: nth_word 0: 2
    printf("nth_word 0: %d\n",
           find_nth_word_start(s3, 0));

    // EXPECT: nth_word 1: 4
    printf("nth_word 1: %d\n",
           find_nth_word_start(s3, 1));

    // EXPECT: nth_word 2: 7
    printf("nth_word 2: %d\n",
           find_nth_word_start(s3, 2));

    char empty[5];
    empty[0] = 0;
    // EXPECT: word_count empty: 0
    printf("word_count empty: %d\n", word_count(empty));

    return 0;
}
