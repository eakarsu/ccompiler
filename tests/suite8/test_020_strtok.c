int printf(const char *fmt, ...);

int my_strlen(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

void my_strcpy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int is_delim(char c, char delim) {
    return c == delim;
}

int tokenize(char *src, char delim, char tokens[][64], int max_tokens) {
    int count = 0;
    int i = 0;
    int start = 0;
    int in_token = 0;
    int len = my_strlen(src);

    while (i <= len) {
        if (i == len || is_delim(src[i], delim)) {
            if (in_token && count < max_tokens) {
                int j = 0;
                int k = start;
                while (k < i) {
                    tokens[count][j] = src[k];
                    j = j + 1;
                    k = k + 1;
                }
                tokens[count][j] = 0;
                count = count + 1;
            }
            in_token = 0;
        } else {
            if (in_token == 0) {
                start = i;
                in_token = 1;
            }
        }
        i = i + 1;
    }
    return count;
}

int split_csv(char *src, char tokens[][64], int max_tokens) {
    int count = 0;
    int i = 0;
    int start = 0;
    int len = my_strlen(src);

    while (i <= len) {
        if (i == len || src[i] == ',') {
            if (count < max_tokens) {
                int j = 0;
                int k = start;
                while (k < i) {
                    tokens[count][j] = src[k];
                    j = j + 1;
                    k = k + 1;
                }
                tokens[count][j] = 0;
                count = count + 1;
            }
            start = i + 1;
        }
        i = i + 1;
    }
    return count;
}

void trim_spaces(char *dst, char *src) {
    int len = my_strlen(src);
    int start = 0;
    while (start < len && src[start] == ' ') {
        start = start + 1;
    }
    int end = len - 1;
    while (end >= start && src[end] == ' ') {
        end = end - 1;
    }
    int i = 0;
    int j = start;
    while (j <= end) {
        dst[i] = src[j];
        i = i + 1;
        j = j + 1;
    }
    dst[i] = 0;
}

void replace_char(char *dst, char *src, char old_c, char new_c) {
    int i = 0;
    while (src[i] != 0) {
        if (src[i] == old_c) {
            dst[i] = new_c;
        } else {
            dst[i] = src[i];
        }
        i = i + 1;
    }
    dst[i] = 0;
}

void squeeze(char *dst, char *src) {
    int i = 0;
    int j = 0;
    while (src[i] != 0) {
        if (src[i] != ' ' || (i > 0 && src[i - 1] != ' ')) {
            dst[j] = src[i];
            j = j + 1;
        }
        i = i + 1;
    }
    if (j > 0 && dst[j - 1] == ' ') {
        j = j - 1;
    }
    dst[j] = 0;
}

int parse_int(char *s) {
    int result = 0;
    int neg = 0;
    int i = 0;
    if (s[0] == '-') {
        neg = 1;
        i = 1;
    }
    while (s[i] >= '0' && s[i] <= '9') {
        result = result * 10 + (s[i] - '0');
        i = i + 1;
    }
    if (neg) result = -result;
    return result;
}

int main() {
    char tokens[16][64];
    int count;

    count = tokenize("hello world foo", ' ', tokens, 16);
    // EXPECT: token count = 3
    printf("token count = %d\n", count);
    // EXPECT: tok 0 = hello
    printf("tok 0 = %s\n", tokens[0]);
    // EXPECT: tok 1 = world
    printf("tok 1 = %s\n", tokens[1]);
    // EXPECT: tok 2 = foo
    printf("tok 2 = %s\n", tokens[2]);

    count = tokenize("  extra   spaces  here  ", ' ', tokens, 16);
    // EXPECT: token count2 = 3
    printf("token count2 = %d\n", count);
    // EXPECT: tok2 0 = extra
    printf("tok2 0 = %s\n", tokens[0]);
    // EXPECT: tok2 1 = spaces
    printf("tok2 1 = %s\n", tokens[1]);
    // EXPECT: tok2 2 = here
    printf("tok2 2 = %s\n", tokens[2]);

    count = tokenize("one", ' ', tokens, 16);
    // EXPECT: single token count = 1
    printf("single token count = %d\n", count);
    // EXPECT: single tok = one
    printf("single tok = %s\n", tokens[0]);

    count = split_csv("alpha,beta,gamma", tokens, 16);
    // EXPECT: csv count = 3
    printf("csv count = %d\n", count);
    // EXPECT: csv 0 = alpha
    printf("csv 0 = %s\n", tokens[0]);
    // EXPECT: csv 1 = beta
    printf("csv 1 = %s\n", tokens[1]);
    // EXPECT: csv 2 = gamma
    printf("csv 2 = %s\n", tokens[2]);

    count = split_csv("a,,b", tokens, 16);
    // EXPECT: csv empty count = 3
    printf("csv empty count = %d\n", count);
    // EXPECT: csv empty 0 = a
    printf("csv empty 0 = %s\n", tokens[0]);
    // EXPECT: csv empty 1 len = 0
    printf("csv empty 1 len = %d\n", my_strlen(tokens[1]));
    // EXPECT: csv empty 2 = b
    printf("csv empty 2 = %s\n", tokens[2]);

    char buf[128];
    trim_spaces(buf, "  hello  ");
    // EXPECT: trim = hello
    printf("trim = %s\n", buf);

    trim_spaces(buf, "   ");
    // EXPECT: trim empty len = 0
    printf("trim empty len = %d\n", my_strlen(buf));

    trim_spaces(buf, "no_trim");
    // EXPECT: trim no = no_trim
    printf("trim no = %s\n", buf);

    replace_char(buf, "hello world", ' ', '-');
    // EXPECT: replace = hello-world
    printf("replace = %s\n", buf);

    replace_char(buf, "aabaa", 'a', 'x');
    // EXPECT: replace2 = xxbxx
    printf("replace2 = %s\n", buf);

    squeeze(buf, "hello   world   foo");
    // EXPECT: squeeze = hello world foo
    printf("squeeze = %s\n", buf);

    squeeze(buf, "  a  b  ");
    // EXPECT: squeeze2 = a b
    printf("squeeze2 = %s\n", buf);

    // EXPECT: parse 42 = 42
    printf("parse 42 = %d\n", parse_int("42"));
    // EXPECT: parse -123 = -123
    printf("parse -123 = %d\n", parse_int("-123"));
    // EXPECT: parse 0 = 0
    printf("parse 0 = %d\n", parse_int("0"));

    count = split_csv("10,20,30", tokens, 16);
    int sum = 0;
    int idx = 0;
    while (idx < count) {
        sum = sum + parse_int(tokens[idx]);
        idx = idx + 1;
    }
    // EXPECT: csv sum = 60
    printf("csv sum = %d\n", sum);

    return 0;
}
