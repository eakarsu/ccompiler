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

int replace_first_char(char *s, char old, char new_c) {
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == old) {
            s[i] = new_c;
            return 1;
        }
        i = i + 1;
    }
    return 0;
}

int replace_all_char(char *s, char old, char new_c) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == old) {
            s[i] = new_c;
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

void char_map(char *s, char from[128], char to[128],
              int map_count) {
    int i = 0;
    while (s[i] != 0) {
        int j = 0;
        while (j < map_count) {
            if (s[i] == from[j]) {
                s[i] = to[j];
                break;
            }
            j = j + 1;
        }
        i = i + 1;
    }
}

void remove_char(char *s, char c) {
    int r = 0;
    int w = 0;
    while (s[r] != 0) {
        if (s[r] != c) {
            s[w] = s[r];
            w = w + 1;
        }
        r = r + 1;
    }
    s[w] = 0;
}

void squeeze(char *s) {
    if (s[0] == 0) return;
    int w = 1;
    int r = 1;
    while (s[r] != 0) {
        if (s[r] != s[r - 1]) {
            s[w] = s[r];
            w = w + 1;
        }
        r = r + 1;
    }
    s[w] = 0;
}

void caesar_shift(char *s, int shift) {
    int i = 0;
    while (s[i] != 0) {
        if (s[i] >= 97 && s[i] <= 122) {
            int v = s[i] - 97;
            v = (v + shift) % 26;
            if (v < 0) v = v + 26;
            s[i] = (char)(v + 97);
        }
        i = i + 1;
    }
}

int main(void) {
    char buf[40];

    buf[0]='h'; buf[1]='e'; buf[2]='l';
    buf[3]='l'; buf[4]='o'; buf[5]=0;

    int r = replace_first_char(buf, 'l', 'L');
    // EXPECT: replace_first: heLlo
    printf("replace_first: %s\n", buf);
    // EXPECT: replaced: 1
    printf("replaced: %d\n", r);

    buf[0]='h'; buf[1]='e'; buf[2]='l';
    buf[3]='l'; buf[4]='o'; buf[5]=0;

    r = replace_all_char(buf, 'l', 'r');
    // EXPECT: replace_all: herro
    printf("replace_all: %s\n", buf);
    // EXPECT: replaced_count: 2
    printf("replaced_count: %d\n", r);

    r = replace_first_char(buf, 'z', 'q');
    // EXPECT: no_replace: 0
    printf("no_replace: %d\n", r);

    char from[128];
    char to[128];
    from[0] = 'a'; to[0] = '1';
    from[1] = 'b'; to[1] = '2';
    from[2] = 'c'; to[2] = '3';

    char m[10];
    m[0]='a'; m[1]='b'; m[2]='c';
    m[3]='d'; m[4]='a'; m[5]=0;

    char_map(m, from, to, 3);
    // EXPECT: mapped: 123d1
    printf("mapped: %s\n", m);

    char rm[20];
    rm[0]='h'; rm[1]='e'; rm[2]='l';
    rm[3]='l'; rm[4]='o'; rm[5]=' ';
    rm[6]='w'; rm[7]=0;

    remove_char(rm, 'l');
    // EXPECT: removed l: heo w
    printf("removed l: %s\n", rm);
    // EXPECT: removed len: 5
    printf("removed len: %d\n", str_len(rm));

    char sq[20];
    sq[0]='a'; sq[1]='a'; sq[2]='b';
    sq[3]='b'; sq[4]='b'; sq[5]='c';
    sq[6]='a'; sq[7]='a'; sq[8]=0;

    squeeze(sq);
    // EXPECT: squeezed: abca
    printf("squeezed: %s\n", sq);
    // EXPECT: squeezed len: 4
    printf("squeezed len: %d\n", str_len(sq));

    char cs[20];
    cs[0]='a'; cs[1]='b'; cs[2]='c'; cs[3]=0;

    caesar_shift(cs, 1);
    // EXPECT: caesar +1: bcd
    printf("caesar +1: %s\n", cs);

    caesar_shift(cs, -1);
    // EXPECT: caesar back: abc
    printf("caesar back: %s\n", cs);

    caesar_shift(cs, 25);
    // EXPECT: caesar +25: zab
    printf("caesar +25: %s\n", cs);

    char sp[20];
    sp[0]='h'; sp[1]=' '; sp[2]=' ';
    sp[3]='i'; sp[4]=0;

    remove_char(sp, ' ');
    // EXPECT: no spaces: hi
    printf("no spaces: %s\n", sp);

    char all[10];
    all[0]='x'; all[1]='x'; all[2]='x'; all[3]=0;
    r = replace_all_char(all, 'x', 'y');
    // EXPECT: all replaced: yyy
    printf("all replaced: %s\n", all);
    // EXPECT: all count: 3
    printf("all count: %d\n", r);

    return 0;
}
