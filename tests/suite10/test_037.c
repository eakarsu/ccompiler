int printf(const char *fmt, ...);

// Dictionary-Based Compression

struct DictEntry {
    char word[16];
    int code;
    int len;
};

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) n = n + 1;
    return n;
}

int str_ncmp(char *a, char *b, int n) {
    int i = 0;
    while (i < n) {
        if (a[i] != b[i]) return a[i] - b[i];
        if (a[i] == 0) return 0;
        i = i + 1;
    }
    return 0;
}

void str_copy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

void dict_init(struct DictEntry *dict, int *count) {
    // Initialize with common words
    str_copy(dict[0].word, "the");
    dict[0].code = 0; dict[0].len = 3;
    str_copy(dict[1].word, "is");
    dict[1].code = 1; dict[1].len = 2;
    str_copy(dict[2].word, "cat");
    dict[2].code = 2; dict[2].len = 3;
    str_copy(dict[3].word, "on");
    dict[3].code = 3; dict[3].len = 2;
    str_copy(dict[4].word, "mat");
    dict[4].code = 4; dict[4].len = 3;
    str_copy(dict[5].word, "sat");
    dict[5].code = 5; dict[5].len = 3;
    str_copy(dict[6].word, "a");
    dict[6].code = 6; dict[6].len = 1;
    str_copy(dict[7].word, "hat");
    dict[7].code = 7; dict[7].len = 3;
    *count = 8;
}

// Find longest matching dictionary entry at position pos in text
int dict_find(struct DictEntry *dict, int dict_count, char *text, int pos, int *match_len) {
    int best_code = -1;
    int best_len = 0;
    int i = 0;
    while (i < dict_count) {
        int wlen = dict[i].len;
        if (str_ncmp(text + pos, dict[i].word, wlen) == 0) {
            // Make sure we match the full word boundary
            char next_ch = text[pos + wlen];
            if (next_ch == ' ' || next_ch == 0) {
                if (wlen > best_len) {
                    best_len = wlen;
                    best_code = dict[i].code;
                }
            }
        }
        i = i + 1;
    }
    *match_len = best_len;
    return best_code;
}

// Compress text using dictionary; output is array of codes (-1 = literal space, -2 = end)
int dict_compress(struct DictEntry *dict, int dict_count, char *text, int *codes) {
    int pos = 0;
    int code_count = 0;
    int text_len = str_len(text);

    while (pos < text_len) {
        if (text[pos] == ' ') {
            codes[code_count] = -1; // space literal
            code_count = code_count + 1;
            pos = pos + 1;
        } else {
            int match_len = 0;
            int code = dict_find(dict, dict_count, text, pos, &match_len);
            if (code >= 0) {
                codes[code_count] = code;
                code_count = code_count + 1;
                pos = pos + match_len;
            } else {
                // Store as literal char (code 100+ascii)
                codes[code_count] = 100 + text[pos];
                code_count = code_count + 1;
                pos = pos + 1;
            }
        }
    }
    codes[code_count] = -2; // end marker
    return code_count;
}

// Decompress codes back to text
void dict_decompress(struct DictEntry *dict, int dict_count, int *codes, int code_count, char *output) {
    int pos = 0;
    int i = 0;
    while (i < code_count) {
        if (codes[i] == -1) {
            output[pos] = ' ';
            pos = pos + 1;
        } else if (codes[i] >= 100) {
            output[pos] = codes[i] - 100;
            pos = pos + 1;
        } else {
            int j = 0;
            while (j < dict_count) {
                if (dict[j].code == codes[i]) {
                    int k = 0;
                    while (dict[j].word[k] != 0) {
                        output[pos] = dict[j].word[k];
                        pos = pos + 1;
                        k = k + 1;
                    }
                    break;
                }
                j = j + 1;
            }
        }
        i = i + 1;
    }
    output[pos] = 0;
}

int str_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

int main() {
    struct DictEntry dict[16];
    int dict_count = 0;
    dict_init(dict, &dict_count);
    printf("Dictionary size: %d\n", dict_count); // EXPECT: Dictionary size: 8

    // Test 1: Compress "the cat sat on a mat"
    char text1[64];
    str_copy(text1, "the cat sat on a mat");

    int codes[64];
    int code_count = dict_compress(dict, dict_count, text1, codes);
    printf("Code count: %d\n", code_count); // EXPECT: Code count: 11

    printf("Codes:");
    int i = 0;
    while (i < code_count) {
        printf(" %d", codes[i]);
        i = i + 1;
    }
    printf("\n"); // EXPECT: Codes: 0 -1 2 -1 5 -1 3 -1 6 -1 4

    // Decompress and verify
    char output[64];
    dict_decompress(dict, dict_count, codes, code_count, output);
    printf("Decompressed: %s\n", output); // EXPECT: Decompressed: the cat sat on a mat

    int match = str_equal(text1, output);
    printf("Match: %d\n", match); // EXPECT: Match: 1

    // Test 2: Compression ratio
    int orig_len = str_len(text1);
    printf("Original bytes: %d\n", orig_len); // EXPECT: Original bytes: 20
    printf("Compressed codes: %d\n", code_count); // EXPECT: Compressed codes: 11
    int ratio = (code_count * 100) / orig_len;
    printf("Ratio: %d percent\n", ratio); // EXPECT: Ratio: 55 percent

    // Test 3: Text with repeated dictionary words
    char text2[64];
    str_copy(text2, "the cat is on the mat");
    int codes2[64];
    int cc2 = dict_compress(dict, dict_count, text2, codes2);
    printf("Repeated code count: %d\n", cc2); // EXPECT: Repeated code count: 11

    char out2[64];
    dict_decompress(dict, dict_count, codes2, cc2, out2);
    printf("Repeated decompressed: %s\n", out2); // EXPECT: Repeated decompressed: the cat is on the mat
    int match2 = str_equal(text2, out2);
    printf("Repeated match: %d\n", match2); // EXPECT: Repeated match: 1

    printf("Dictionary compression done\n"); // EXPECT: Dictionary compression done
    return 0;
}
