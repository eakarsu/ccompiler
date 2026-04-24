int printf(const char *fmt, ...);

/* ROT13 encoding: a symmetric substitution cipher */

int str_len(char *s) {
    int n;
    n = 0;
    while (s[n]) n = n + 1;
    return n;
}

void str_copy(char *dst, char *src) {
    int i;
    i = 0;
    while (src[i]) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int str_eq(char *a, char *b) {
    int i;
    i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return (a[i] == 0 && b[i] == 0);
}

void rot13(char *input, char *output) {
    int i;
    int len;
    len = str_len(input);
    for (i = 0; i < len; i = i + 1) {
        int c;
        c = input[i];
        if (c >= 65 && c <= 90) {
            output[i] = ((c - 65 + 13) % 26) + 65;
        } else if (c >= 97 && c <= 122) {
            output[i] = ((c - 97 + 13) % 26) + 97;
        } else {
            output[i] = c;
        }
    }
    output[len] = 0;
}

void rot13_inplace(char *s) {
    int i;
    i = 0;
    while (s[i]) {
        int c;
        c = s[i];
        if (c >= 65 && c <= 90) {
            s[i] = ((c - 65 + 13) % 26) + 65;
        } else if (c >= 97 && c <= 122) {
            s[i] = ((c - 97 + 13) % 26) + 97;
        }
        i = i + 1;
    }
}

void test_basic_rot13(void) {
    char buf[128];
    rot13("hello", buf);
    // EXPECT: rot13 hello: uryyb
    printf("rot13 hello: %s\n", buf);
    rot13("HELLO", buf);
    // EXPECT: rot13 HELLO: URYYB
    printf("rot13 HELLO: %s\n", buf);
    rot13("abc", buf);
    // EXPECT: rot13 abc: nop
    printf("rot13 abc: %s\n", buf);
    rot13("nop", buf);
    // EXPECT: rot13 nop: abc
    printf("rot13 nop: %s\n", buf);
}

void test_symmetry(void) {
    char first[128];
    char second[128];
    int match;
    rot13("The quick brown fox", first);
    rot13(first, second);
    match = str_eq(second, "The quick brown fox");
    // EXPECT: rot13 symmetric: 1
    printf("rot13 symmetric: %d\n", match);
}

void test_preserve(void) {
    char buf[128];
    rot13("Hello, World! 123", buf);
    // EXPECT: preserve: Uryyb, Jbeyq! 123
    printf("preserve: %s\n", buf);
}

void test_empty(void) {
    char buf[128];
    rot13("", buf);
    int len;
    len = str_len(buf);
    // EXPECT: empty len=0
    printf("empty len=%d\n", len);
}

void test_inplace(void) {
    char data[64];
    str_copy(data, "secret");
    rot13_inplace(data);
    // EXPECT: inplace: frperg
    printf("inplace: %s\n", data);
    rot13_inplace(data);
    // EXPECT: inplace back: secret
    printf("inplace back: %s\n", data);
}

void test_all_letters(void) {
    char lower_enc[64];
    char upper_enc[64];
    rot13("abcdefghijklmnopqrstuvwxyz", lower_enc);
    rot13("ABCDEFGHIJKLMNOPQRSTUVWXYZ", upper_enc);
    // EXPECT: lower: nopqrstuvwxyzabcdefghijklm
    printf("lower: %s\n", lower_enc);
    // EXPECT: upper: NOPQRSTUVWXYZABCDEFGHIJKLM
    printf("upper: %s\n", upper_enc);
}

void test_double_rot13_various(void) {
    char *words[4];
    char enc[64];
    char dec[64];
    int i;
    int all_ok;
    words[0] = "alpha";
    words[1] = "BRAVO";
    words[2] = "Charlie";
    words[3] = "delta";
    all_ok = 1;
    for (i = 0; i < 4; i = i + 1) {
        rot13(words[i], enc);
        rot13(enc, dec);
        if (!str_eq(dec, words[i])) {
            all_ok = 0;
        }
    }
    // EXPECT: all double rot13 ok: 1
    printf("all double rot13 ok: %d\n", all_ok);
}

int main(void) {
    test_basic_rot13();
    test_symmetry();
    test_preserve();
    test_empty();
    test_inplace();
    test_all_letters();
    test_double_rot13_various();
    // EXPECT: done
    printf("done\n");
    return 0;
}
