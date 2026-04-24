int printf(const char *fmt, ...);

int is_digit(int c) {
    return (c >= '0' && c <= '9');
}

int is_alpha(int c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int is_upper(int c) {
    return (c >= 'A' && c <= 'Z');
}

int is_lower(int c) {
    return (c >= 'a' && c <= 'z');
}

int is_space(int c) {
    return (c == 32 || c == 9 || c == 10 || c == 13);
}

int is_alnum(int c) {
    return is_alpha(c) || is_digit(c);
}

int is_punct(int c) {
    if (c >= 33 && c <= 47) return 1;
    if (c >= 58 && c <= 64) return 1;
    if (c >= 91 && c <= 96) return 1;
    if (c >= 123 && c <= 126) return 1;
    return 0;
}

int is_printable(int c) {
    return (c >= 32 && c <= 126);
}

int is_hex_digit(int c) {
    if (c >= '0' && c <= '9') return 1;
    if (c >= 'A' && c <= 'F') return 1;
    if (c >= 'a' && c <= 'f') return 1;
    return 0;
}

int classify(int c) {
    if (is_digit(c)) return 1;
    if (is_alpha(c)) return 2;
    if (is_space(c)) return 3;
    if (is_punct(c)) return 4;
    return 0;
}

int count_digits(char *s) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (is_digit(s[i])) count = count + 1;
        i = i + 1;
    }
    return count;
}

int count_alpha(char *s) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (is_alpha(s[i])) count = count + 1;
        i = i + 1;
    }
    return count;
}

int count_spaces(char *s) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (is_space(s[i])) count = count + 1;
        i = i + 1;
    }
    return count;
}

int count_upper(char *s) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (is_upper(s[i])) count = count + 1;
        i = i + 1;
    }
    return count;
}

int count_lower(char *s) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (is_lower(s[i])) count = count + 1;
        i = i + 1;
    }
    return count;
}

int main(void) {
    // EXPECT: is_digit_5=1
    printf("is_digit_5=%d\n", is_digit('5'));

    // EXPECT: is_digit_A=0
    printf("is_digit_A=%d\n", is_digit('A'));

    // EXPECT: is_alpha_z=1
    printf("is_alpha_z=%d\n", is_alpha('z'));

    // EXPECT: is_alpha_3=0
    printf("is_alpha_3=%d\n", is_alpha('3'));

    // EXPECT: is_upper_M=1
    printf("is_upper_M=%d\n", is_upper('M'));

    // EXPECT: is_upper_m=0
    printf("is_upper_m=%d\n", is_upper('m'));

    // EXPECT: is_lower_q=1
    printf("is_lower_q=%d\n", is_lower('q'));

    // EXPECT: is_lower_Q=0
    printf("is_lower_Q=%d\n", is_lower('Q'));

    // EXPECT: is_space_sp=1
    printf("is_space_sp=%d\n", is_space(32));

    // EXPECT: is_space_tab=1
    printf("is_space_tab=%d\n", is_space(9));

    // EXPECT: is_space_A=0
    printf("is_space_A=%d\n", is_space('A'));

    // EXPECT: is_alnum_7=1
    printf("is_alnum_7=%d\n", is_alnum('7'));

    // EXPECT: is_alnum_B=1
    printf("is_alnum_B=%d\n", is_alnum('B'));

    // EXPECT: is_alnum_bang=0
    printf("is_alnum_bang=%d\n", is_alnum('!'));

    // EXPECT: is_punct_bang=1
    printf("is_punct_bang=%d\n", is_punct('!'));

    // EXPECT: is_punct_A=0
    printf("is_punct_A=%d\n", is_punct('A'));

    // EXPECT: is_printable_A=1
    printf("is_printable_A=%d\n", is_printable('A'));

    // EXPECT: is_hex_a=1
    printf("is_hex_a=%d\n", is_hex_digit('a'));

    // EXPECT: is_hex_g=0
    printf("is_hex_g=%d\n", is_hex_digit('g'));

    // EXPECT: classify_5=1
    printf("classify_5=%d\n", classify('5'));

    // EXPECT: classify_X=2
    printf("classify_X=%d\n", classify('X'));

    // EXPECT: classify_sp=3
    printf("classify_sp=%d\n", classify(32));

    // EXPECT: classify_dot=4
    printf("classify_dot=%d\n", classify('.'));

    char test1[20];
    test1[0] = 'H'; test1[1] = 'i'; test1[2] = ' ';
    test1[3] = '4'; test1[4] = '2'; test1[5] = '!';
    test1[6] = 0;

    // EXPECT: digits=2
    printf("digits=%d\n", count_digits(test1));

    // EXPECT: alpha=2
    printf("alpha=%d\n", count_alpha(test1));

    // EXPECT: spaces=1
    printf("spaces=%d\n", count_spaces(test1));

    // EXPECT: uppers=1
    printf("uppers=%d\n", count_upper(test1));

    // EXPECT: lowers=1
    printf("lowers=%d\n", count_lower(test1));

    return 0;
}
