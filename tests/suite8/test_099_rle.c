int printf(const char *fmt, ...);

/* Run-Length Encoding (RLE): compress and decompress byte sequences */

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

/* RLE encode: stores as pairs (char, count_digit)
   For simplicity, counts are stored as single ASCII digits '1'-'9'.
   Runs longer than 9 are split into multiple runs. */
int rle_encode(char *input, char *output) {
    int i;
    int j;
    int len;
    int count;
    char current;
    len = str_len(input);
    if (len == 0) {
        output[0] = 0;
        return 0;
    }
    j = 0;
    i = 0;
    while (i < len) {
        current = input[i];
        count = 1;
        while (i + count < len && input[i + count] == current && count < 9) {
            count = count + 1;
        }
        output[j] = current;
        j = j + 1;
        output[j] = 48 + count;
        j = j + 1;
        i = i + count;
    }
    output[j] = 0;
    return j;
}

/* RLE decode: reads pairs (char, count_digit) */
int rle_decode(char *input, char *output) {
    int i;
    int j;
    int len;
    int count;
    int k;
    len = str_len(input);
    j = 0;
    for (i = 0; i < len; i = i + 2) {
        char ch;
        ch = input[i];
        count = input[i + 1] - 48;
        for (k = 0; k < count; k = k + 1) {
            output[j] = ch;
            j = j + 1;
        }
    }
    output[j] = 0;
    return j;
}

void test_basic_encode(void) {
    char out[256];
    rle_encode("aaa", out);
    // EXPECT: encode aaa: a3
    printf("encode aaa: %s\n", out);
    rle_encode("aabbb", out);
    // EXPECT: encode aabbb: a2b3
    printf("encode aabbb: %s\n", out);
    rle_encode("abc", out);
    // EXPECT: encode abc: a1b1c1
    printf("encode abc: %s\n", out);
}

void test_basic_decode(void) {
    char out[256];
    rle_decode("a3", out);
    // EXPECT: decode a3: aaa
    printf("decode a3: %s\n", out);
    rle_decode("a2b3", out);
    // EXPECT: decode a2b3: aabbb
    printf("decode a2b3: %s\n", out);
    rle_decode("a1b1c1", out);
    // EXPECT: decode a1b1c1: abc
    printf("decode a1b1c1: %s\n", out);
}

void test_roundtrip(void) {
    char encoded[256];
    char decoded[256];
    int match;
    rle_encode("aaabbbccc", encoded);
    rle_decode(encoded, decoded);
    match = str_eq(decoded, "aaabbbccc");
    // EXPECT: roundtrip aaabbbccc: 1
    printf("roundtrip aaabbbccc: %d\n", match);
}

void test_single_char(void) {
    char enc[256];
    char dec[256];
    int match;
    rle_encode("x", enc);
    // EXPECT: encode x: x1
    printf("encode x: %s\n", enc);
    rle_decode(enc, dec);
    match = str_eq(dec, "x");
    // EXPECT: roundtrip x: 1
    printf("roundtrip x: %d\n", match);
}

void test_long_run(void) {
    char input[32];
    char enc[256];
    char dec[256];
    int i;
    int match;
    /* Create a 12-character run */
    for (i = 0; i < 12; i = i + 1) {
        input[i] = 'z';
    }
    input[12] = 0;
    rle_encode(input, enc);
    // EXPECT: encode 12 z's: z9z3
    printf("encode 12 z's: %s\n", enc);
    rle_decode(enc, dec);
    match = str_eq(dec, input);
    // EXPECT: roundtrip 12 z's: 1
    printf("roundtrip 12 z's: %d\n", match);
}

void test_compression_ratio(void) {
    char enc[256];
    int orig_len;
    int enc_len;
    /* Highly compressible */
    orig_len = str_len("aaaaaaaaa");
    enc_len = rle_encode("aaaaaaaaa", enc);
    // EXPECT: compress 9 a's: 9 -> 2
    printf("compress 9 a's: %d -> %d\n", orig_len, enc_len);
    /* Not compressible */
    orig_len = str_len("abcdef");
    enc_len = rle_encode("abcdef", enc);
    // EXPECT: no compress abcdef: 6 -> 12
    printf("no compress abcdef: %d -> %d\n", orig_len, enc_len);
}

void test_empty(void) {
    char enc[256];
    int elen;
    elen = rle_encode("", enc);
    // EXPECT: empty encode len=0
    printf("empty encode len=%d\n", elen);
}

void test_mixed_runs(void) {
    char enc[256];
    char dec[256];
    int match;
    rle_encode("aabbccddee", enc);
    // EXPECT: encode mixed: a2b2c2d2e2
    printf("encode mixed: %s\n", enc);
    rle_decode(enc, dec);
    match = str_eq(dec, "aabbccddee");
    // EXPECT: roundtrip mixed: 1
    printf("roundtrip mixed: %d\n", match);
}

void test_various_roundtrips(void) {
    char *inputs[4];
    char enc[256];
    char dec[256];
    int i;
    int all_ok;
    inputs[0] = "aaaa";
    inputs[1] = "abab";
    inputs[2] = "xxxyyyzzz";
    inputs[3] = "abcabc";
    all_ok = 1;
    for (i = 0; i < 4; i = i + 1) {
        rle_encode(inputs[i], enc);
        rle_decode(enc, dec);
        if (!str_eq(dec, inputs[i])) {
            all_ok = 0;
        }
    }
    // EXPECT: all roundtrips ok: 1
    printf("all roundtrips ok: %d\n", all_ok);
}

int main(void) {
    test_basic_encode();
    test_basic_decode();
    test_roundtrip();
    test_single_char();
    test_long_run();
    test_compression_ratio();
    test_empty();
    test_mixed_runs();
    test_various_roundtrips();
    // EXPECT: done
    printf("done\n");
    return 0;
}
