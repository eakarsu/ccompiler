int printf(const char *fmt, ...);

int my_strlen(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

int count_char(char *s, int target) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == target) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

int has_null_at(char *s, int pos) {
    return (s[pos] == 0) ? 1 : 0;
}

int char_at(char *s, int pos) {
    return s[pos];
}

void copy_until_null(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int strings_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return (a[i] == b[i]) ? 1 : 0;
}

int main(void) {
    char newline_str[4];
    newline_str[0] = 'a';
    newline_str[1] = '\n';
    newline_str[2] = 'b';
    newline_str[3] = 0;

    // EXPECT: nl_len=3
    printf("nl_len=%d\n", my_strlen(newline_str));

    // EXPECT: nl_val=10
    printf("nl_val=%d\n", (int)newline_str[1]);

    char tab_str[4];
    tab_str[0] = 'x';
    tab_str[1] = '\t';
    tab_str[2] = 'y';
    tab_str[3] = 0;

    // EXPECT: tab_len=3
    printf("tab_len=%d\n", my_strlen(tab_str));

    // EXPECT: tab_val=9
    printf("tab_val=%d\n", (int)tab_str[1]);

    char bs_str[3];
    bs_str[0] = '\\';
    bs_str[1] = '\\';
    bs_str[2] = 0;

    // EXPECT: bs_len=2
    printf("bs_len=%d\n", my_strlen(bs_str));

    // EXPECT: bs_val=92
    printf("bs_val=%d\n", (int)bs_str[0]);

    // EXPECT: bs_val2=92
    printf("bs_val2=%d\n", (int)bs_str[1]);

    char null_test[5];
    null_test[0] = 'A';
    null_test[1] = '\0';
    null_test[2] = 'B';
    null_test[3] = '\0';
    null_test[4] = 0;

    // EXPECT: nt_len=1
    printf("nt_len=%d\n", my_strlen(null_test));

    // EXPECT: nt_0=65
    printf("nt_0=%d\n", (int)null_test[0]);

    // EXPECT: nt_1=0
    printf("nt_1=%d\n", (int)null_test[1]);

    // EXPECT: nt_2=66
    printf("nt_2=%d\n", (int)null_test[2]);

    char lit[] = "hello";
    // EXPECT: lit_len=5
    printf("lit_len=%d\n", my_strlen(lit));

    // EXPECT: lit_null=1
    printf("lit_null=%d\n", has_null_at(lit, 5));

    // EXPECT: lit_h=104
    printf("lit_h=%d\n", char_at(lit, 0));

    char mixed[10];
    mixed[0] = '\t';
    mixed[1] = 'A';
    mixed[2] = '\n';
    mixed[3] = 'B';
    mixed[4] = '\\';
    mixed[5] = 0;

    // EXPECT: mix_len=5
    printf("mix_len=%d\n", my_strlen(mixed));

    // EXPECT: mix_tab=9
    printf("mix_tab=%d\n", (int)mixed[0]);

    // EXPECT: mix_nl=10
    printf("mix_nl=%d\n", (int)mixed[2]);

    // EXPECT: mix_bs=92
    printf("mix_bs=%d\n", (int)mixed[4]);

    char buf[32];
    copy_until_null(buf, lit);
    // EXPECT: copy_eq=1
    printf("copy_eq=%d\n", strings_equal(buf, lit));

    char a[] = "test";
    char b[] = "test";
    // EXPECT: eq_test=1
    printf("eq_test=%d\n", strings_equal(a, b));

    char c[] = "tess";
    // EXPECT: neq_test=0
    printf("neq_test=%d\n", strings_equal(a, c));

    // EXPECT: nl_count=1
    printf("nl_count=%d\n", count_char(newline_str, 10));

    // EXPECT: a_in_hello=0
    printf("a_in_hello=%d\n", count_char(lit, 'a'));

    // EXPECT: l_in_hello=2
    printf("l_in_hello=%d\n", count_char(lit, 'l'));

    return 0;
}
