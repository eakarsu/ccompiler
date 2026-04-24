int printf(const char *fmt, ...);

/* Simplified base64-style encoding/decoding using a 64-character alphabet */

char base64_table[65];

void init_table(void) {
    int i;
    for (i = 0; i < 26; i = i + 1) {
        base64_table[i] = 65 + i;
    }
    for (i = 0; i < 26; i = i + 1) {
        base64_table[26 + i] = 97 + i;
    }
    for (i = 0; i < 10; i = i + 1) {
        base64_table[52 + i] = 48 + i;
    }
    base64_table[62] = 43;
    base64_table[63] = 47;
    base64_table[64] = 0;
}

int b64_char_to_index(int c) {
    if (c >= 65 && c <= 90) return c - 65;
    if (c >= 97 && c <= 122) return c - 97 + 26;
    if (c >= 48 && c <= 57) return c - 48 + 52;
    if (c == 43) return 62;
    if (c == 47) return 63;
    return -1;
}

int str_len(char *s) {
    int n;
    n = 0;
    while (s[n]) n = n + 1;
    return n;
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

/* Encode input bytes to base64 string */
int base64_encode(char *input, int inlen, char *output) {
    int i;
    int j;
    int a;
    int b;
    int c;
    int triple;
    j = 0;
    for (i = 0; i < inlen; i = i + 3) {
        a = input[i] & 255;
        b = (i + 1 < inlen) ? (input[i + 1] & 255) : 0;
        c = (i + 2 < inlen) ? (input[i + 2] & 255) : 0;
        triple = (a << 16) | (b << 8) | c;
        output[j] = base64_table[(triple >> 18) & 63];
        j = j + 1;
        output[j] = base64_table[(triple >> 12) & 63];
        j = j + 1;
        if (i + 1 < inlen) {
            output[j] = base64_table[(triple >> 6) & 63];
        } else {
            output[j] = 61;
        }
        j = j + 1;
        if (i + 2 < inlen) {
            output[j] = base64_table[triple & 63];
        } else {
            output[j] = 61;
        }
        j = j + 1;
    }
    output[j] = 0;
    return j;
}

/* Decode base64 string back to bytes, returns length */
int base64_decode(char *input, int inlen, char *output) {
    int i;
    int j;
    int a;
    int b;
    int c;
    int d;
    int triple;
    j = 0;
    for (i = 0; i < inlen; i = i + 4) {
        a = b64_char_to_index(input[i]);
        b = b64_char_to_index(input[i + 1]);
        c = (input[i + 2] != 61) ? b64_char_to_index(input[i + 2]) : 0;
        d = (input[i + 3] != 61) ? b64_char_to_index(input[i + 3]) : 0;
        triple = (a << 18) | (b << 12) | (c << 6) | d;
        output[j] = (triple >> 16) & 255;
        j = j + 1;
        if (input[i + 2] != 61) {
            output[j] = (triple >> 8) & 255;
            j = j + 1;
        }
        if (input[i + 3] != 61) {
            output[j] = triple & 255;
            j = j + 1;
        }
    }
    output[j] = 0;
    return j;
}

void test_encode_basic(void) {
    char out[128];
    init_table();
    base64_encode("Man", 3, out);
    // EXPECT: encode Man: TWFu
    printf("encode Man: %s\n", out);
    base64_encode("Ma", 2, out);
    // EXPECT: encode Ma: TWE=
    printf("encode Ma: %s\n", out);
    base64_encode("M", 1, out);
    // EXPECT: encode M: TQ==
    printf("encode M: %s\n", out);
}

void test_encode_longer(void) {
    char out[128];
    base64_encode("Hello", 5, out);
    // EXPECT: encode Hello: SGVsbG8=
    printf("encode Hello: %s\n", out);
    base64_encode("AB", 2, out);
    // EXPECT: encode AB: QUI=
    printf("encode AB: %s\n", out);
}

void test_decode_basic(void) {
    char out[128];
    base64_decode("TWFu", 4, out);
    // EXPECT: decode TWFu: Man
    printf("decode TWFu: %s\n", out);
    base64_decode("TWE=", 4, out);
    // EXPECT: decode TWE=: Ma
    printf("decode TWE=: %s\n", out);
    base64_decode("TQ==", 4, out);
    // EXPECT: decode TQ==: M
    printf("decode TQ==: %s\n", out);
}

void test_roundtrip(void) {
    char encoded[128];
    char decoded[128];
    int match;
    base64_encode("hello world", 11, encoded);
    base64_decode(encoded, str_len(encoded), decoded);
    match = str_eq(decoded, "hello world");
    // EXPECT: roundtrip hello world: 1
    printf("roundtrip hello world: %d\n", match);
}

void test_roundtrip_various(void) {
    char *tests[3];
    char encoded[128];
    char decoded[128];
    int i;
    int all_ok;
    tests[0] = "abc";
    tests[1] = "test data";
    tests[2] = "12345";
    all_ok = 1;
    for (i = 0; i < 3; i = i + 1) {
        base64_encode(tests[i], str_len(tests[i]), encoded);
        base64_decode(encoded, str_len(encoded), decoded);
        if (!str_eq(decoded, tests[i])) {
            all_ok = 0;
        }
    }
    // EXPECT: all roundtrips ok: 1
    printf("all roundtrips ok: %d\n", all_ok);
}

void test_output_length(void) {
    char out[128];
    int elen;
    elen = base64_encode("abc", 3, out);
    // EXPECT: encode len 3 -> 4
    printf("encode len 3 -> %d\n", elen);
    elen = base64_encode("ab", 2, out);
    // EXPECT: encode len 2 -> 4
    printf("encode len 2 -> %d\n", elen);
    elen = base64_encode("a", 1, out);
    // EXPECT: encode len 1 -> 4
    printf("encode len 1 -> %d\n", elen);
}

int main(void) {
    init_table();
    test_encode_basic();
    test_encode_longer();
    test_decode_basic();
    test_roundtrip();
    test_roundtrip_various();
    test_output_length();
    // EXPECT: done
    printf("done\n");
    return 0;
}
