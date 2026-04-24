int printf(const char *fmt, ...);
// EXPECT: Compress 'aaabbbcccc': 'a3b3c4' (len 10->6)\nDecompress: 'aaabbbcccc'\nCompress 'abcdef': 'abcdef' (len 6->6)\nDecompress: 'abcdef'\nCompress 'aaaaaaaaaabbbbbbbbbb': 'a10b10' (len 20->6)\nDecompress: 'aaaaaaaaaabbbbbbbbbb'\nCompress 'abbbba': 'ab4a'\nDecompress: 'abbbba'
// Test: Simple string compression (run-length encoding variant)

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) n++;
    return n;
}

int compress(char *input, char *output) {
    int n = str_len(input);
    int i = 0;
    int out = 0;

    while (i < n) {
        char c = input[i];
        int count = 1;
        while (i + count < n && input[i + count] == c) {
            count++;
        }
        output[out] = c;
        out++;
        if (count > 1) {
            int digits[10];
            int nd = 0;
            int tmp = count;
            while (tmp > 0) {
                digits[nd] = tmp % 10;
                nd++;
                tmp = tmp / 10;
            }
            int d;
            for (d = nd - 1; d >= 0; d--) {
                output[out] = '0' + digits[d];
                out++;
            }
        }
        i = i + count;
    }
    output[out] = 0;
    return out;
}

void decompress(char *input, char *output) {
    int n = str_len(input);
    int i = 0;
    int out = 0;

    while (i < n) {
        char c = input[i];
        i++;
        int count = 0;
        while (i < n && input[i] >= '0' && input[i] <= '9') {
            count = count * 10 + (input[i] - '0');
            i++;
        }
        if (count == 0) count = 1;
        int j;
        for (j = 0; j < count; j++) {
            output[out] = c;
            out++;
        }
    }
    output[out] = 0;
}

int main(void) {
    char out[256];
    char back[256];

    char s1[] = "aaabbbcccc";
    int clen = compress(s1, out);
    printf("Compress '%s': '%s' (len %d->%d)\n", s1, out, str_len(s1), clen);
    decompress(out, back);
    printf("Decompress: '%s'\n", back);

    char s2[] = "abcdef";
    clen = compress(s2, out);
    printf("Compress '%s': '%s' (len %d->%d)\n", s2, out, str_len(s2), clen);
    decompress(out, back);
    printf("Decompress: '%s'\n", back);

    char s3[] = "aaaaaaaaaabbbbbbbbbb";
    clen = compress(s3, out);
    printf("Compress '%s': '%s' (len %d->%d)\n", s3, out, str_len(s3), clen);
    decompress(out, back);
    printf("Decompress: '%s'\n", back);

    char s4[] = "abbbba";
    clen = compress(s4, out);
    printf("Compress '%s': '%s'\n", s4, out);
    decompress(out, back);
    printf("Decompress: '%s'\n", back);

    return 0;
}
