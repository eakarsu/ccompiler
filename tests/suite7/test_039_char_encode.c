int printf(const char *fmt, ...);

int my_strlen(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

int str_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return (a[i] == b[i]) ? 1 : 0;
}

void str_copy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

void rot13(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        int c = src[i];
        if (c >= 'A' && c <= 'Z') {
            c = c - 'A';
            c = (c + 13) % 26;
            c = c + 'A';
        } else if (c >= 'a' && c <= 'z') {
            c = c - 'a';
            c = (c + 13) % 26;
            c = c + 'a';
        }
        dst[i] = c;
        i = i + 1;
    }
    dst[i] = 0;
}

void caesar_encrypt(char *dst, char *src, int shift) {
    int i = 0;
    while (src[i] != 0) {
        int c = src[i];
        if (c >= 'a' && c <= 'z') {
            c = c - 'a';
            c = (c + shift) % 26;
            if (c < 0) c = c + 26;
            c = c + 'a';
        } else if (c >= 'A' && c <= 'Z') {
            c = c - 'A';
            c = (c + shift) % 26;
            if (c < 0) c = c + 26;
            c = c + 'A';
        }
        dst[i] = c;
        i = i + 1;
    }
    dst[i] = 0;
}

void caesar_decrypt(char *dst, char *src, int shift) {
    caesar_encrypt(dst, src, 26 - shift);
}

void xor_encode(char *dst, char *src, int key) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i] ^ key;
        i = i + 1;
    }
    dst[i] = 0;
}

void shift_encode(char *dst, char *src, int shift) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i] + shift;
        i = i + 1;
    }
    dst[i] = 0;
}

void shift_decode(char *dst, char *src, int shift) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i] - shift;
        i = i + 1;
    }
    dst[i] = 0;
}

int main(void) {
    char buf[64];
    char buf2[64];

    rot13(buf, "hello");
    // EXPECT: rot13_hello=uryyb
    printf("rot13_hello=%s\n", buf);

    rot13(buf2, buf);
    // EXPECT: rot13_back=hello
    printf("rot13_back=%s\n", buf2);

    rot13(buf, "ABC");
    // EXPECT: rot13_ABC=NOP
    printf("rot13_ABC=%s\n", buf);

    rot13(buf2, buf);
    // EXPECT: rot13_NOP=ABC
    printf("rot13_NOP=%s\n", buf2);

    rot13(buf, "Hello World");
    // EXPECT: rot13_hw=Uryyb Jbeyq
    printf("rot13_hw=%s\n", buf);

    rot13(buf2, buf);
    // EXPECT: rot13_rt=Hello World
    printf("rot13_rt=%s\n", buf2);

    caesar_encrypt(buf, "abc", 3);
    // EXPECT: caesar3=def
    printf("caesar3=%s\n", buf);

    caesar_decrypt(buf2, buf, 3);
    // EXPECT: caesar3_back=abc
    printf("caesar3_back=%s\n", buf2);

    caesar_encrypt(buf, "xyz", 3);
    // EXPECT: caesar_wrap=abc
    printf("caesar_wrap=%s\n", buf);

    caesar_encrypt(buf, "ABC", 1);
    // EXPECT: caesar_upper=BCD
    printf("caesar_upper=%s\n", buf);

    caesar_encrypt(buf, "Hello", 5);
    caesar_decrypt(buf2, buf, 5);
    // EXPECT: caesar_rt=Hello
    printf("caesar_rt=%s\n", buf2);

    char msg[] = "test";
    xor_encode(buf, msg, 42);
    xor_encode(buf2, buf, 42);
    // EXPECT: xor_rt=1
    printf("xor_rt=%d\n", str_equal(buf2, msg));

    // EXPECT: xor_changed=1
    printf("xor_changed=%d\n", str_equal(buf, msg) == 0 ? 1 : 0);

    shift_encode(buf, "abc", 1);
    // EXPECT: shift_0=98
    printf("shift_0=%d\n", (int)buf[0]);

    // EXPECT: shift_1=99
    printf("shift_1=%d\n", (int)buf[1]);

    // EXPECT: shift_2=100
    printf("shift_2=%d\n", (int)buf[2]);

    shift_decode(buf2, buf, 1);
    // EXPECT: shift_rt=abc
    printf("shift_rt=%s\n", buf2);

    char plain[] = "secret";
    shift_encode(buf, plain, 5);
    shift_decode(buf2, buf, 5);
    // EXPECT: shift_secret=1
    printf("shift_secret=%d\n", str_equal(buf2, plain));

    rot13(buf, "12345");
    // EXPECT: rot13_nums=12345
    printf("rot13_nums=%s\n", buf);

    caesar_encrypt(buf, "a b", 1);
    // EXPECT: caesar_sp=b c
    printf("caesar_sp=%s\n", buf);

    return 0;
}
