int printf(const char *fmt, ...);

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) {
        n = n + 1;
    }
    return n;
}

int count_chars(char *s) {
    return str_len(s);
}

int count_words(char *s) {
    int count = 0;
    int in_word = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == 32 || s[i] == 10) {
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

int count_lines(char *s) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == 10) count = count + 1;
        i = i + 1;
    }
    if (str_len(s) > 0 && s[str_len(s) - 1] != 10) {
        count = count + 1;
    }
    return count;
}

int total_word_chars(char *s) {
    int total = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] != 32 && s[i] != 10) {
            total = total + 1;
        }
        i = i + 1;
    }
    return total;
}

int avg_word_length(char *s) {
    int words = count_words(s);
    if (words == 0) return 0;
    int total = total_word_chars(s);
    return total / words;
}

int longest_word_len(char *s) {
    int best = 0;
    int cur = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == 32 || s[i] == 10) {
            if (cur > best) best = cur;
            cur = 0;
        } else {
            cur = cur + 1;
        }
        i = i + 1;
    }
    if (cur > best) best = cur;
    return best;
}

int shortest_word_len(char *s) {
    int best = 9999;
    int cur = 0;
    int in_word = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == 32 || s[i] == 10) {
            if (in_word && cur < best) best = cur;
            cur = 0;
            in_word = 0;
        } else {
            cur = cur + 1;
            in_word = 1;
        }
        i = i + 1;
    }
    if (in_word && cur < best) best = cur;
    if (best == 9999) return 0;
    return best;
}

int count_sentences(char *s) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == 46) count = count + 1;
        i = i + 1;
    }
    return count;
}

int line_length(char *s, int line_num) {
    int cur_line = 0;
    int len = 0;
    int i = 0;
    while (s[i] != 0) {
        if (cur_line == line_num) {
            if (s[i] == 10) return len;
            len = len + 1;
        }
        if (s[i] == 10) {
            cur_line = cur_line + 1;
        }
        i = i + 1;
    }
    if (cur_line == line_num) return len;
    return -1;
}

int main(void) {
    char t1[50];
    t1[0]='h'; t1[1]='i'; t1[2]=' ';
    t1[3]='t'; t1[4]='h'; t1[5]='e';
    t1[6]='r'; t1[7]='e'; t1[8]=0;

    // EXPECT: chars: 8
    printf("chars: %d\n", count_chars(t1));
    // EXPECT: words: 2
    printf("words: %d\n", count_words(t1));
    // EXPECT: lines: 1
    printf("lines: %d\n", count_lines(t1));

    char t2[50];
    t2[0]='a'; t2[1]='b'; t2[2]='c';
    t2[3]=10;
    t2[4]='d'; t2[5]='e'; t2[6]=10;
    t2[7]='f'; t2[8]=0;

    // EXPECT: lines2: 3
    printf("lines2: %d\n", count_lines(t2));
    // EXPECT: words2: 3
    printf("words2: %d\n", count_words(t2));

    // EXPECT: total_wchars: 7
    printf("total_wchars: %d\n", total_word_chars(t1));

    // EXPECT: avg_word_len: 3
    printf("avg_word_len: %d\n", avg_word_length(t1));

    char t3[50];
    t3[0]='I'; t3[1]=' ';
    t3[2]='a'; t3[3]='m'; t3[4]=' ';
    t3[5]='h'; t3[6]='e'; t3[7]='r';
    t3[8]='e'; t3[9]=0;

    // EXPECT: words3: 3
    printf("words3: %d\n", count_words(t3));
    // EXPECT: longest3: 4
    printf("longest3: %d\n", longest_word_len(t3));
    // EXPECT: shortest3: 1
    printf("shortest3: %d\n", shortest_word_len(t3));
    // EXPECT: avg3: 2
    printf("avg3: %d\n", avg_word_length(t3));

    char t4[50];
    t4[0]='o'; t4[1]='k'; t4[2]='.';
    t4[3]=' '; t4[4]='h'; t4[5]='i';
    t4[6]='.'; t4[7]=0;

    // EXPECT: sentences: 2
    printf("sentences: %d\n", count_sentences(t4));

    // EXPECT: line0 len: 3
    printf("line0 len: %d\n", line_length(t2, 0));
    // EXPECT: line1 len: 2
    printf("line1 len: %d\n", line_length(t2, 1));
    // EXPECT: line2 len: 1
    printf("line2 len: %d\n", line_length(t2, 2));

    char empty[5];
    empty[0] = 0;
    // EXPECT: empty words: 0
    printf("empty words: %d\n", count_words(empty));
    // EXPECT: empty avg: 0
    printf("empty avg: %d\n", avg_word_length(empty));
    // EXPECT: empty shortest: 0
    printf("empty shortest: %d\n",
           shortest_word_len(empty));

    char spaces[10];
    spaces[0]=' '; spaces[1]=' '; spaces[2]=' ';
    spaces[3]=0;
    // EXPECT: spaces words: 0
    printf("spaces words: %d\n", count_words(spaces));

    char big[50];
    big[0]='a'; big[1]='b'; big[2]='c';
    big[3]='d'; big[4]='e'; big[5]='f';
    big[6]='g'; big[7]=' ';
    big[8]='h'; big[9]='i'; big[10]=0;

    // EXPECT: longest big: 7
    printf("longest big: %d\n",
           longest_word_len(big));
    // EXPECT: shortest big: 2
    printf("shortest big: %d\n",
           shortest_word_len(big));

    return 0;
}
