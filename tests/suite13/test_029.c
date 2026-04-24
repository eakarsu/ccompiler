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

void str_copy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

void remove_consecutive_dups(char *src, char *dst) {
    int si = 0;
    int di = 0;
    int slen = str_len(src);
    if (slen == 0) {
        dst[0] = 0;
        return;
    }
    dst[di] = src[si];
    di = di + 1;
    si = si + 1;
    while (si < slen) {
        if (src[si] != src[si - 1]) {
            dst[di] = src[si];
            di = di + 1;
        }
        si = si + 1;
    }
    dst[di] = 0;
}

void remove_all_dups(char *src, char *dst) {
    int seen[128];
    int i = 0;
    while (i < 128) {
        seen[i] = 0;
        i = i + 1;
    }
    int si = 0;
    int di = 0;
    while (src[si] != 0) {
        int idx = src[si];
        if (seen[idx] == 0) {
            dst[di] = src[si];
            di = di + 1;
            seen[idx] = 1;
        }
        si = si + 1;
    }
    dst[di] = 0;
}

int compress_count(char *src, char *dst) {
    int si = 0;
    int di = 0;
    int slen = str_len(src);
    while (si < slen) {
        char ch = src[si];
        int count = 0;
        while (si < slen && src[si] == ch) {
            count = count + 1;
            si = si + 1;
        }
        dst[di] = ch;
        di = di + 1;
        if (count > 1) {
            if (count >= 10) {
                dst[di] = '0' + (count / 10);
                di = di + 1;
                dst[di] = '0' + (count % 10);
                di = di + 1;
            } else {
                dst[di] = '0' + count;
                di = di + 1;
            }
        }
    }
    dst[di] = 0;
    return di;
}

void squeeze(char *s, char c) {
    int si = 0;
    int di = 0;
    while (s[si] != 0) {
        if (s[si] != c) {
            s[di] = s[si];
            di = di + 1;
        }
        si = si + 1;
    }
    s[di] = 0;
}

int compression_ratio(char *s) {
    char compressed[200];
    remove_consecutive_dups(s, compressed);
    int orig = str_len(s);
    int comp = str_len(compressed);
    if (orig == 0) return 100;
    return (comp * 100) / orig;
}

void iterative_compress(char *s, char *dst) {
    char tmp1[200];
    char tmp2[200];
    str_copy(tmp1, s);
    int changed = 1;
    while (changed) {
        remove_consecutive_dups(tmp1, tmp2);
        if (str_equal(tmp1, tmp2)) {
            changed = 0;
        } else {
            str_copy(tmp1, tmp2);
        }
    }
    str_copy(dst, tmp1);
}

int main() {
    char result[200];

    remove_consecutive_dups("aabbcc", result);
    printf("compress aabbcc: %s\n", result);
    // EXPECT: compress aabbcc: abc

    remove_consecutive_dups("aaabbbccc", result);
    printf("compress aaabbbccc: %s\n", result);
    // EXPECT: compress aaabbbccc: abc

    remove_consecutive_dups("abcabc", result);
    printf("compress abcabc: %s\n", result);
    // EXPECT: compress abcabc: abcabc

    remove_consecutive_dups("aabba", result);
    printf("compress aabba: %s\n", result);
    // EXPECT: compress aabba: aba

    remove_consecutive_dups("a", result);
    printf("compress a: %s\n", result);
    // EXPECT: compress a: a

    remove_consecutive_dups("aaaa", result);
    printf("compress aaaa: %s\n", result);
    // EXPECT: compress aaaa: a

    remove_all_dups("hello world", result);
    printf("remove all dups: %s\n", result);
    // EXPECT: remove all dups: helo wrd

    remove_all_dups("abcabc", result);
    printf("remove all dups abcabc: %s\n", result);
    // EXPECT: remove all dups abcabc: abc

    remove_all_dups("aaa", result);
    printf("remove all dups aaa: %s\n", result);
    // EXPECT: remove all dups aaa: a

    int clen = compress_count("aaabbc", result);
    printf("compress count aaabbc: %s (len %d)\n", result, clen);
    // EXPECT: compress count aaabbc: a3b2c (len 5)

    clen = compress_count("abc", result);
    printf("compress count abc: %s (len %d)\n", result, clen);
    // EXPECT: compress count abc: abc (len 3)

    clen = compress_count("aabbccdd", result);
    printf("compress count aabbccdd: %s (len %d)\n", result, clen);
    // EXPECT: compress count aabbccdd: a2b2c2d2 (len 8)

    char buf[200];
    str_copy(buf, "hello");
    squeeze(buf, 'l');
    printf("squeeze l from hello: %s\n", buf);
    // EXPECT: squeeze l from hello: heo

    str_copy(buf, "aabbcc");
    squeeze(buf, 'b');
    printf("squeeze b from aabbcc: %s\n", buf);
    // EXPECT: squeeze b from aabbcc: aacc

    str_copy(buf, "xyz");
    squeeze(buf, 'a');
    printf("squeeze a from xyz: %s\n", buf);
    // EXPECT: squeeze a from xyz: xyz

    int ratio = compression_ratio("aaabbbccc");
    printf("ratio aaabbbccc: %d\n", ratio);
    // EXPECT: ratio aaabbbccc: 33

    ratio = compression_ratio("abc");
    printf("ratio abc: %d\n", ratio);
    // EXPECT: ratio abc: 100

    iterative_compress("aabbcc", result);
    printf("iterative aabbcc: %s\n", result);
    // EXPECT: iterative aabbcc: abc

    iterative_compress("abba", result);
    printf("iterative abba: %s\n", result);
    // EXPECT: iterative abba: aba

    iterative_compress("abccba", result);
    printf("iterative abccba: %s\n", result);
    // EXPECT: iterative abccba: abcba

    printf("done\n");
    // EXPECT: done

    return 0;
}
