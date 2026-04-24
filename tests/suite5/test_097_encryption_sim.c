int printf(const char *fmt, ...);

int str_len(const char *s) {
    int n = 0;
    while (s[n] != 0) n = n + 1;
    return n;
}

void copy_str(char *dst, const char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int str_eq(const char *a, const char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

void caesar_encrypt(const char *plain, char *cipher, int shift) {
    int i = 0;
    while (plain[i] != 0) {
        int c = plain[i];
        if (c >= 97 && c <= 122) {
            c = ((c - 97 + shift) % 26) + 97;
        } else if (c >= 65 && c <= 90) {
            c = ((c - 65 + shift) % 26) + 65;
        }
        cipher[i] = c;
        i = i + 1;
    }
    cipher[i] = 0;
}

void caesar_decrypt(const char *cipher, char *plain, int shift) {
    int i = 0;
    while (cipher[i] != 0) {
        int c = cipher[i];
        if (c >= 97 && c <= 122) {
            c = ((c - 97 - shift + 26) % 26) + 97;
        } else if (c >= 65 && c <= 90) {
            c = ((c - 65 - shift + 26) % 26) + 65;
        }
        plain[i] = c;
        i = i + 1;
    }
    plain[i] = 0;
}

void xor_cipher(const char *input, char *output, int key, int len) {
    int i = 0;
    while (i < len) {
        output[i] = input[i] ^ key;
        i = i + 1;
    }
    output[len] = 0;
}

int simple_hash(const char *data, int len) {
    int hash = 5381;
    int i = 0;
    while (i < len) {
        hash = hash * 33 + data[i];
        i = i + 1;
    }
    if (hash < 0) hash = 0 - hash;
    return hash % 10000;
}

int rot13_char(int c) {
    if (c >= 97 && c <= 122) {
        return ((c - 97 + 13) % 26) + 97;
    }
    if (c >= 65 && c <= 90) {
        return ((c - 65 + 13) % 26) + 65;
    }
    return c;
}

void rot13(const char *input, char *output) {
    int i = 0;
    while (input[i] != 0) {
        output[i] = rot13_char(input[i]);
        i = i + 1;
    }
    output[i] = 0;
}

int checksum(const char *data, int len) {
    int sum = 0;
    int i = 0;
    while (i < len) {
        sum = sum + data[i];
        i = i + 1;
    }
    return sum % 256;
}

void vigenere_enc(const char *plain, char *cipher,
                  const char *key) {
    int klen = str_len(key);
    int i = 0;
    while (plain[i] != 0) {
        int c = plain[i];
        if (c >= 97 && c <= 122) {
            int shift = key[i % klen] - 97;
            c = ((c - 97 + shift) % 26) + 97;
        }
        cipher[i] = c;
        i = i + 1;
    }
    cipher[i] = 0;
}

void vigenere_dec(const char *cipher, char *plain,
                  const char *key) {
    int klen = str_len(key);
    int i = 0;
    while (cipher[i] != 0) {
        int c = cipher[i];
        if (c >= 97 && c <= 122) {
            int shift = key[i % klen] - 97;
            c = ((c - 97 - shift + 26) % 26) + 97;
        }
        plain[i] = c;
        i = i + 1;
    }
    plain[i] = 0;
}

int main(void) {
    char buf[64];
    char buf2[64];

    caesar_encrypt("hello", buf, 3);
    // EXPECT: caesar_enc=khoor
    printf("caesar_enc=%s\n", buf);

    caesar_decrypt(buf, buf2, 3);
    // EXPECT: caesar_dec=hello
    printf("caesar_dec=%s\n", buf2);

    caesar_encrypt("xyz", buf, 3);
    // EXPECT: caesar_wrap=abc
    printf("caesar_wrap=%s\n", buf);

    caesar_encrypt("ABC", buf, 1);
    // EXPECT: caesar_upper=BCD
    printf("caesar_upper=%s\n", buf);

    char xor_in[8];
    char xor_out[8];
    char xor_back[8];
    xor_in[0] = 10;
    xor_in[1] = 20;
    xor_in[2] = 30;
    xor_in[3] = 40;
    xor_cipher(xor_in, xor_out, 55, 4);
    xor_cipher(xor_out, xor_back, 55, 4);
    // EXPECT: xor_roundtrip_0=10
    printf("xor_roundtrip_0=%d\n", (int)xor_back[0]);
    // EXPECT: xor_roundtrip_1=20
    printf("xor_roundtrip_1=%d\n", (int)xor_back[1]);
    // EXPECT: xor_roundtrip_2=30
    printf("xor_roundtrip_2=%d\n", (int)xor_back[2]);
    // EXPECT: xor_roundtrip_3=40
    printf("xor_roundtrip_3=%d\n", (int)xor_back[3]);

    int h1 = simple_hash("hello", 5);
    int h2 = simple_hash("hello", 5);
    // EXPECT: hash_consistent=1
    printf("hash_consistent=%d\n", h1 == h2);

    int h3 = simple_hash("world", 5);
    // EXPECT: hash_different=1
    printf("hash_different=%d\n", h1 != h3);

    rot13("hello", buf);
    // EXPECT: rot13_enc=uryyb
    printf("rot13_enc=%s\n", buf);

    rot13(buf, buf2);
    // EXPECT: rot13_dec=hello
    printf("rot13_dec=%s\n", buf2);

    int cs = checksum("ABCD", 4);
    // EXPECT: checksum=10
    printf("checksum=%d\n", cs);

    vigenere_enc("hello", buf, "key");
    // EXPECT: vig_enc=rijvs
    printf("vig_enc=%s\n", buf);

    vigenere_dec(buf, buf2, "key");
    // EXPECT: vig_dec=hello
    printf("vig_dec=%s\n", buf2);

    // EXPECT: len_hello=5
    printf("len_hello=%d\n", str_len("hello"));

    return 0;
}
