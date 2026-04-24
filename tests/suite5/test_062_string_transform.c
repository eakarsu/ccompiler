int printf(const char *fmt, ...);

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) {
        n = n + 1;
    }
    return n;
}

void str_copy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

void to_upper(char *s) {
    int i = 0;
    while (s[i] != 0) {
        if (s[i] >= 97 && s[i] <= 122) {
            s[i] = s[i] - 32;
        }
        i = i + 1;
    }
}

void to_lower(char *s) {
    int i = 0;
    while (s[i] != 0) {
        if (s[i] >= 65 && s[i] <= 90) {
            s[i] = s[i] + 32;
        }
        i = i + 1;
    }
}

int is_upper(char c) {
    return c >= 65 && c <= 90;
}

int is_lower(char c) {
    return c >= 97 && c <= 122;
}

void trim_left(char *s) {
    int i = 0;
    while (s[i] == 32) {
        i = i + 1;
    }
    if (i == 0) return;
    int j = 0;
    while (s[i] != 0) {
        s[j] = s[i];
        j = j + 1;
        i = i + 1;
    }
    s[j] = 0;
}

void trim_right(char *s) {
    int len = str_len(s);
    int i = len - 1;
    while (i >= 0 && s[i] == 32) {
        i = i - 1;
    }
    s[i + 1] = 0;
}

void trim(char *s) {
    trim_left(s);
    trim_right(s);
}

void pad_right(char *s, int total, char c) {
    int len = str_len(s);
    while (len < total) {
        s[len] = c;
        len = len + 1;
    }
    s[len] = 0;
}

void pad_left(char *dst, char *src, int total, char c) {
    int len = str_len(src);
    int pad = total - len;
    if (pad < 0) pad = 0;
    int i = 0;
    while (i < pad) {
        dst[i] = c;
        i = i + 1;
    }
    int j = 0;
    while (src[j] != 0) {
        dst[i] = src[j];
        i = i + 1;
        j = j + 1;
    }
    dst[i] = 0;
}

void reverse(char *s) {
    int len = str_len(s);
    int i = 0;
    int j = len - 1;
    while (i < j) {
        char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        i = i + 1;
        j = j - 1;
    }
}

int count_upper(char *s) {
    int c = 0;
    int i = 0;
    while (s[i] != 0) {
        if (is_upper(s[i])) c = c + 1;
        i = i + 1;
    }
    return c;
}

int count_lower(char *s) {
    int c = 0;
    int i = 0;
    while (s[i] != 0) {
        if (is_lower(s[i])) c = c + 1;
        i = i + 1;
    }
    return c;
}

int main(void) {
    char buf[40];

    buf[0]='h'; buf[1]='e'; buf[2]='l';
    buf[3]='l'; buf[4]='o'; buf[5]=0;

    // EXPECT: len hello: 5
    printf("len hello: %d\n", str_len(buf));

    to_upper(buf);
    // EXPECT: upper: HELLO
    printf("upper: %s\n", buf);

    to_lower(buf);
    // EXPECT: lower: hello
    printf("lower: %s\n", buf);

    // EXPECT: count_upper hello: 0
    printf("count_upper hello: %d\n", count_upper(buf));

    to_upper(buf);
    // EXPECT: count_upper HELLO: 5
    printf("count_upper HELLO: %d\n", count_upper(buf));

    char sp[30];
    sp[0]=' '; sp[1]=' '; sp[2]='a';
    sp[3]='b'; sp[4]=' '; sp[5]=' '; sp[6]=0;

    trim_left(sp);
    // EXPECT: trim_left: 4
    printf("trim_left: %d\n", str_len(sp));

    char sp2[30];
    sp2[0]=' '; sp2[1]=' '; sp2[2]='x';
    sp2[3]='y'; sp2[4]=' '; sp2[5]=' '; sp2[6]=0;

    trim(sp2);
    // EXPECT: trim len: 2
    printf("trim len: %d\n", str_len(sp2));
    // EXPECT: trim: xy
    printf("trim: %s\n", sp2);

    char p[20];
    p[0]='h'; p[1]='i'; p[2]=0;
    pad_right(p, 5, '.');
    // EXPECT: pad_right: hi...
    printf("pad_right: %s\n", p);
    // EXPECT: pad_right len: 5
    printf("pad_right len: %d\n", str_len(p));

    char src[10];
    src[0]='4'; src[1]='2'; src[2]=0;
    char dst[20];
    pad_left(dst, src, 5, '0');
    // EXPECT: pad_left: 00042
    printf("pad_left: %s\n", dst);
    // EXPECT: pad_left len: 5
    printf("pad_left len: %d\n", str_len(dst));

    char rv[10];
    rv[0]='a'; rv[1]='b'; rv[2]='c';
    rv[3]='d'; rv[4]=0;
    reverse(rv);
    // EXPECT: reverse: dcba
    printf("reverse: %s\n", rv);

    char mix[20];
    mix[0]='H'; mix[1]='e'; mix[2]='L';
    mix[3]='l'; mix[4]='O'; mix[5]=0;

    // EXPECT: upper_count: 3
    printf("upper_count: %d\n", count_upper(mix));
    // EXPECT: lower_count: 2
    printf("lower_count: %d\n", count_lower(mix));

    char empty[5];
    empty[0] = 0;
    // EXPECT: empty len: 0
    printf("empty len: %d\n", str_len(empty));

    to_upper(empty);
    // EXPECT: empty upper len: 0
    printf("empty upper len: %d\n", str_len(empty));

    return 0;
}
