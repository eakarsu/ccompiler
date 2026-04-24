int printf(const char *fmt, ...);

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) {
        n = n + 1;
    }
    return n;
}

int rle_encode(char *src, char *chars, int *counts) {
    int slen = str_len(src);
    if (slen == 0) return 0;
    int out = 0;
    int i = 0;
    while (i < slen) {
        char c = src[i];
        int count = 1;
        while (i + count < slen && src[i + count] == c) {
            count = count + 1;
        }
        chars[out] = c;
        counts[out] = count;
        out = out + 1;
        i = i + count;
    }
    return out;
}

int rle_decode(char *chars, int *counts, int n,
               char *dst) {
    int pos = 0;
    int i = 0;
    while (i < n) {
        int j = 0;
        while (j < counts[i]) {
            dst[pos] = chars[i];
            pos = pos + 1;
            j = j + 1;
        }
        i = i + 1;
    }
    dst[pos] = 0;
    return pos;
}

int strings_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

int rle_compressed_size(char *chars, int *counts,
                        int n) {
    int size = 0;
    int i = 0;
    while (i < n) {
        size = size + 1;
        if (counts[i] > 1) {
            int c = counts[i];
            while (c > 0) {
                size = size + 1;
                c = c / 10;
            }
        }
        i = i + 1;
    }
    return size;
}

int main(void) {
    char src1[40];
    src1[0]='a'; src1[1]='a'; src1[2]='a';
    src1[3]='b'; src1[4]='b'; src1[5]='c';
    src1[6]='c'; src1[7]='c'; src1[8]='c';
    src1[9]='d'; src1[10]=0;

    char chars[40];
    int counts[40];

    int n = rle_encode(src1, chars, counts);
    // EXPECT: rle groups: 4
    printf("rle groups: %d\n", n);

    // EXPECT: group0: 97 x3
    printf("group0: %d x%d\n", (int)chars[0], counts[0]);
    // EXPECT: group1: 98 x2
    printf("group1: %d x%d\n", (int)chars[1], counts[1]);
    // EXPECT: group2: 99 x4
    printf("group2: %d x%d\n", (int)chars[2], counts[2]);
    // EXPECT: group3: 100 x1
    printf("group3: %d x%d\n", (int)chars[3], counts[3]);

    char decoded[40];
    int dlen = rle_decode(chars, counts, n, decoded);
    // EXPECT: decoded len: 10
    printf("decoded len: %d\n", dlen);

    // EXPECT: roundtrip ok: 1
    printf("roundtrip ok: %d\n",
           strings_equal(src1, decoded));

    char src2[20];
    src2[0]='x'; src2[1]='y'; src2[2]='z'; src2[3]=0;

    n = rle_encode(src2, chars, counts);
    // EXPECT: no_repeat groups: 3
    printf("no_repeat groups: %d\n", n);
    // EXPECT: nr count0: 1
    printf("nr count0: %d\n", counts[0]);
    // EXPECT: nr count1: 1
    printf("nr count1: %d\n", counts[1]);
    // EXPECT: nr count2: 1
    printf("nr count2: %d\n", counts[2]);

    dlen = rle_decode(chars, counts, n, decoded);
    // EXPECT: roundtrip2 ok: 1
    printf("roundtrip2 ok: %d\n",
           strings_equal(src2, decoded));

    char src3[20];
    src3[0]='a'; src3[1]='a'; src3[2]='a';
    src3[3]='a'; src3[4]='a'; src3[5]='a';
    src3[6]='a'; src3[7]='a'; src3[8]='a';
    src3[9]='a'; src3[10]=0;

    n = rle_encode(src3, chars, counts);
    // EXPECT: all_same groups: 1
    printf("all_same groups: %d\n", n);
    // EXPECT: all_same count: 10
    printf("all_same count: %d\n", counts[0]);

    dlen = rle_decode(chars, counts, n, decoded);
    // EXPECT: roundtrip3 ok: 1
    printf("roundtrip3 ok: %d\n",
           strings_equal(src3, decoded));
    // EXPECT: roundtrip3 len: 10
    printf("roundtrip3 len: %d\n", dlen);

    char src4[2];
    src4[0] = 0;
    n = rle_encode(src4, chars, counts);
    // EXPECT: empty groups: 0
    printf("empty groups: %d\n", n);

    char src5[10];
    src5[0]='a'; src5[1]='b'; src5[2]='a';
    src5[3]='b'; src5[4]=0;

    n = rle_encode(src5, chars, counts);
    // EXPECT: alternating groups: 4
    printf("alternating groups: %d\n", n);

    dlen = rle_decode(chars, counts, n, decoded);
    // EXPECT: roundtrip4 ok: 1
    printf("roundtrip4 ok: %d\n",
           strings_equal(src5, decoded));

    return 0;
}
