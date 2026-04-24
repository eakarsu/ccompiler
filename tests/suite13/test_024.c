int printf(const char *fmt, ...);

int str_len(char *s) {
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
    return a[i] == b[i];
}

void rle_encode(char *src, char *dst) {
    int si = 0;
    int di = 0;
    int slen = str_len(src);
    while (si < slen) {
        char ch = src[si];
        int count = 1;
        while (si + count < slen && src[si + count] == ch) {
            count = count + 1;
        }
        if (count > 1) {
            if (count < 10) {
                dst[di] = '0' + count;
                di = di + 1;
            } else {
                int tens = count / 10;
                int ones = count % 10;
                dst[di] = '0' + tens;
                di = di + 1;
                dst[di] = '0' + ones;
                di = di + 1;
            }
        } else {
            dst[di] = '1';
            di = di + 1;
        }
        dst[di] = ch;
        di = di + 1;
        si = si + count;
    }
    dst[di] = 0;
}

void rle_decode(char *src, char *dst) {
    int si = 0;
    int di = 0;
    int slen = str_len(src);
    while (si < slen) {
        int count = 0;
        while (si < slen && src[si] >= '0' && src[si] <= '9') {
            count = count * 10 + (src[si] - '0');
            si = si + 1;
        }
        if (count == 0) count = 1;
        if (si < slen) {
            char ch = src[si];
            int k = 0;
            while (k < count) {
                dst[di] = ch;
                di = di + 1;
                k = k + 1;
            }
            si = si + 1;
        }
    }
    dst[di] = 0;
}

int rle_compressed_size(char *s) {
    char encoded[200];
    rle_encode(s, encoded);
    return str_len(encoded);
}

int main() {
    char encoded[200];
    char decoded[200];

    rle_encode("aaabbbccc", encoded);
    printf("encode aaabbbccc: %s\n", encoded);
    // EXPECT: encode aaabbbccc: 3a3b3c

    rle_decode(encoded, decoded);
    printf("decode back: %s\n", decoded);
    // EXPECT: decode back: aaabbbccc

    rle_encode("abc", encoded);
    printf("encode abc: %s\n", encoded);
    // EXPECT: encode abc: 1a1b1c

    rle_decode(encoded, decoded);
    printf("decode back: %s\n", decoded);
    // EXPECT: decode back: abc

    rle_encode("aabccdd", encoded);
    printf("encode aabccdd: %s\n", encoded);
    // EXPECT: encode aabccdd: 2a1b2c2d

    rle_decode(encoded, decoded);
    printf("decode back: %s\n", decoded);
    // EXPECT: decode back: aabccdd

    rle_encode("a", encoded);
    printf("encode a: %s\n", encoded);
    // EXPECT: encode a: 1a

    rle_decode(encoded, decoded);
    printf("decode back: %s\n", decoded);
    // EXPECT: decode back: a

    rle_encode("aaaaaaaaaa", encoded);
    printf("encode 10 a's: %s\n", encoded);
    // EXPECT: encode 10 a's: 10a

    rle_decode(encoded, decoded);
    printf("decode back: %s\n", decoded);
    // EXPECT: decode back: aaaaaaaaaa

    rle_encode("aabbaa", encoded);
    printf("encode aabbaa: %s\n", encoded);
    // EXPECT: encode aabbaa: 2a2b2a

    rle_decode(encoded, decoded);
    printf("decode back: %s\n", decoded);
    // EXPECT: decode back: aabbaa

    int orig_len = str_len("aaabbbccc");
    int comp_len = rle_compressed_size("aaabbbccc");
    printf("original len: %d, compressed len: %d\n", orig_len, comp_len);
    // EXPECT: original len: 9, compressed len: 6

    orig_len = str_len("abc");
    comp_len = rle_compressed_size("abc");
    printf("original len: %d, compressed len: %d\n", orig_len, comp_len);
    // EXPECT: original len: 3, compressed len: 6

    rle_encode("xxxyyyzzz", encoded);
    printf("encode xxxyyyzzz: %s\n", encoded);
    // EXPECT: encode xxxyyyzzz: 3x3y3z

    rle_decode("5a3b1c", decoded);
    printf("decode 5a3b1c: %s\n", decoded);
    // EXPECT: decode 5a3b1c: aaaaabbbc

    int round_trip = str_equal("aabccdd", decoded);
    rle_encode("aabccdd", encoded);
    rle_decode(encoded, decoded);
    round_trip = str_equal("aabccdd", decoded);
    printf("round trip ok: %d\n", round_trip);
    // EXPECT: round trip ok: 1

    printf("done\n");
    // EXPECT: done

    return 0;
}
