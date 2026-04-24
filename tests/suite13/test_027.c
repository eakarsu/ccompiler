int printf(const char *fmt, ...);

int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

void str_copy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int tokenize(char *s, char delim, char tokens[10][20], int *count) {
    int si = 0;
    int ti = 0;
    int ci = 0;
    *count = 0;
    int slen = str_len(s);

    while (si <= slen) {
        if (si == slen || s[si] == delim) {
            if (ci > 0) {
                tokens[ti][ci] = 0;
                ti = ti + 1;
                ci = 0;
            }
            si = si + 1;
        } else {
            tokens[ti][ci] = s[si];
            ci = ci + 1;
            si = si + 1;
        }
    }
    *count = ti;
    return ti;
}

void join_tokens(char tokens[10][20], int count, char delim, char *result) {
    int ri = 0;
    int i = 0;
    while (i < count) {
        int j = 0;
        while (tokens[i][j] != 0) {
            result[ri] = tokens[i][j];
            ri = ri + 1;
            j = j + 1;
        }
        if (i < count - 1) {
            result[ri] = delim;
            ri = ri + 1;
        }
        i = i + 1;
    }
    result[ri] = 0;
}

int find_token(char tokens[10][20], int count, char *target) {
    int i = 0;
    while (i < count) {
        int j = 0;
        int match = 1;
        while (tokens[i][j] != 0 || target[j] != 0) {
            if (tokens[i][j] != target[j]) {
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

void reverse_tokens(char tokens[10][20], int count) {
    int i = 0;
    while (i < count / 2) {
        char tmp[20];
        str_copy(tmp, tokens[i]);
        str_copy(tokens[i], tokens[count - 1 - i]);
        str_copy(tokens[count - 1 - i], tmp);
        i = i + 1;
    }
}

int count_delimiters(char *s, char delim) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == delim) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

int main() {
    char tokens[10][20];
    int count;
    char result[200];

    tokenize("hello world foo", ' ', tokens, &count);
    printf("token count: %d\n", count);
    // EXPECT: token count: 3
    printf("t0: %s\n", tokens[0]);
    // EXPECT: t0: hello
    printf("t1: %s\n", tokens[1]);
    // EXPECT: t1: world
    printf("t2: %s\n", tokens[2]);
    // EXPECT: t2: foo

    tokenize("a,b,c,d", ',', tokens, &count);
    printf("csv count: %d\n", count);
    // EXPECT: csv count: 4
    printf("t0: %s\n", tokens[0]);
    // EXPECT: t0: a
    printf("t3: %s\n", tokens[3]);
    // EXPECT: t3: d

    join_tokens(tokens, count, '-', result);
    printf("joined: %s\n", result);
    // EXPECT: joined: a-b-c-d

    tokenize("one", ' ', tokens, &count);
    printf("single token count: %d\n", count);
    // EXPECT: single token count: 1
    printf("t0: %s\n", tokens[0]);
    // EXPECT: t0: one

    tokenize("x:y:z", ':', tokens, &count);
    printf("colon count: %d\n", count);
    // EXPECT: colon count: 3
    int idx = find_token(tokens, count, "y");
    printf("found y at: %d\n", idx);
    // EXPECT: found y at: 1

    idx = find_token(tokens, count, "w");
    printf("found w at: %d\n", idx);
    // EXPECT: found w at: -1

    tokenize("alpha beta gamma delta", ' ', tokens, &count);
    printf("before reverse: %s %s %s %s\n", tokens[0], tokens[1], tokens[2], tokens[3]);
    // EXPECT: before reverse: alpha beta gamma delta

    reverse_tokens(tokens, count);
    printf("after reverse: %s %s %s %s\n", tokens[0], tokens[1], tokens[2], tokens[3]);
    // EXPECT: after reverse: delta gamma beta alpha

    join_tokens(tokens, count, ' ', result);
    printf("reversed joined: %s\n", result);
    // EXPECT: reversed joined: delta gamma beta alpha

    int dc = count_delimiters("a,b,,c,d", ',');
    printf("delimiters in a,b,,c,d: %d\n", dc);
    // EXPECT: delimiters in a,b,,c,d: 4

    dc = count_delimiters("hello", ',');
    printf("delimiters in hello: %d\n", dc);
    // EXPECT: delimiters in hello: 0

    tokenize("cat dog bird", ' ', tokens, &count);
    join_tokens(tokens, count, ',', result);
    printf("re-delimited: %s\n", result);
    // EXPECT: re-delimited: cat,dog,bird

    tokenize("p-q-r-s-t", '-', tokens, &count);
    printf("dash count: %d\n", count);
    // EXPECT: dash count: 5
    printf("first: %s last: %s\n", tokens[0], tokens[4]);
    // EXPECT: first: p last: t

    printf("done\n");
    // EXPECT: done

    return 0;
}
