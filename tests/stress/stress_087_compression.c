// stress_087_compression.c - RLE, dictionary compression, frequency counting, bit packing
// EXPECT: rle_enc_len=12
// EXPECT: rle_encoded=A4B3C2D1E3F1
// EXPECT: rle_dec_ok=1
// EXPECT: rle_roundtrip=1
// EXPECT: rle_single=A1B1C1
// EXPECT: freq_max_char=a
// EXPECT: freq_max_count=4
// EXPECT: freq_unique=7
// EXPECT: bitpack_enc_ok=1
// EXPECT: bitpack_dec_ok=1
// EXPECT: bitpack_roundtrip=1
// EXPECT: dict_enc_len_ok=1
// EXPECT: dict_dec_ok=1
// EXPECT: compress_ratio_ok=1
// EXPECT: rle_worst_case=1
// EXPECT: rle_empty_len=0
// EXPECT: pass_done=1

int printf(const char *fmt, ...);

// --- String length ---
int my_strlen(char *s) {
    int len = 0;
    while (s[len] != 0) len++;
    return len;
}

int my_strcmp(char *a, char *b) {
    while (*a && *a == *b) { a++; b++; }
    return *a - *b;
}

void my_strcpy(char *dst, char *src) {
    while (*src) { *dst = *src; dst++; src++; }
    *dst = 0;
}

// --- Run-Length Encoding ---
// Encodes "AAAABBBCCDEEEF" -> "A4B3C2D1E3F1"
int rle_encode(char *input, char *output) {
    int in_len = my_strlen(input);
    int out_pos = 0;
    int i = 0;
    while (i < in_len) {
        char c = input[i];
        int count = 1;
        while (i + count < in_len && input[i + count] == c && count < 9) {
            count++;
        }
        output[out_pos++] = c;
        output[out_pos++] = '0' + count;
        i += count;
    }
    output[out_pos] = 0;
    return out_pos;
}

int rle_decode(char *input, char *output) {
    int in_len = my_strlen(input);
    int out_pos = 0;
    int i = 0;
    while (i + 1 < in_len) {
        char c = input[i];
        int count = input[i + 1] - '0';
        int j;
        for (j = 0; j < count; j++) {
            output[out_pos++] = c;
        }
        i += 2;
    }
    output[out_pos] = 0;
    return out_pos;
}

// --- Frequency counting ---
struct FreqEntry {
    char ch;
    int count;
};

int count_frequencies(char *input, struct FreqEntry *freq, int max_entries) {
    int num_entries = 0;
    int len = my_strlen(input);
    int i, j;
    for (i = 0; i < len; i++) {
        char c = input[i];
        int found = 0;
        for (j = 0; j < num_entries; j++) {
            if (freq[j].ch == c) {
                freq[j].count++;
                found = 1;
                break;
            }
        }
        if (!found && num_entries < max_entries) {
            freq[num_entries].ch = c;
            freq[num_entries].count = 1;
            num_entries++;
        }
    }
    return num_entries;
}

// --- Bit packing: pack array of values (0-15) into nibbles ---
int bitpack_encode(int *values, int count, char *output) {
    int out_pos = 0;
    int i;
    for (i = 0; i < count; i += 2) {
        int high = values[i] & 0xF;
        int low = (i + 1 < count) ? (values[i + 1] & 0xF) : 0;
        output[out_pos++] = (char)((high << 4) | low);
    }
    return out_pos;
}

int bitpack_decode(char *packed, int packed_len, int *values, int count) {
    int i;
    int val_idx = 0;
    for (i = 0; i < packed_len && val_idx < count; i++) {
        unsigned char byte = (unsigned char)packed[i];
        values[val_idx++] = (byte >> 4) & 0xF;
        if (val_idx < count) {
            values[val_idx++] = byte & 0xF;
        }
    }
    return val_idx;
}

// --- Simple dictionary compression ---
// Replaces repeated substrings with references (index, length)
// Format: literal bytes or (0xFF, index_byte, length_byte)
struct DictEntry {
    char str[16];
    int len;
};

int dict_compress(char *input, char *output, struct DictEntry *dict, int dict_size) {
    int in_len = my_strlen(input);
    int out_pos = 0;
    int i = 0;

    while (i < in_len) {
        int best_match = -1;
        int best_len = 0;
        int d;

        // Search dictionary for longest match
        for (d = 0; d < dict_size; d++) {
            int dlen = dict[d].len;
            if (dlen > best_len && i + dlen <= in_len) {
                int match = 1;
                int k;
                for (k = 0; k < dlen; k++) {
                    if (input[i + k] != dict[d].str[k]) {
                        match = 0;
                        break;
                    }
                }
                if (match) {
                    best_match = d;
                    best_len = dlen;
                }
            }
        }

        if (best_len >= 3) {
            // Encode as reference
            output[out_pos++] = (char)0xFF;
            output[out_pos++] = (char)best_match;
            output[out_pos++] = (char)best_len;
            i += best_len;
        } else {
            // Literal byte
            if ((unsigned char)input[i] == 0xFF) {
                // Escape 0xFF
                output[out_pos++] = (char)0xFF;
                output[out_pos++] = (char)0xFF;
                output[out_pos++] = (char)0;
                i++;
            } else {
                output[out_pos++] = input[i];
                i++;
            }
        }
    }
    return out_pos;
}

int dict_decompress(char *input, int in_len, char *output, struct DictEntry *dict) {
    int out_pos = 0;
    int i = 0;
    while (i < in_len) {
        if ((unsigned char)input[i] == 0xFF) {
            i++;
            if (i < in_len && (unsigned char)input[i] == 0xFF) {
                // Escaped 0xFF
                output[out_pos++] = (char)0xFF;
                i += 2; // skip length byte (0)
            } else if (i + 1 < in_len) {
                int idx = (unsigned char)input[i];
                int len = (unsigned char)input[i + 1];
                int k;
                for (k = 0; k < len; k++) {
                    output[out_pos++] = dict[idx].str[k];
                }
                i += 2;
            }
        } else {
            output[out_pos++] = input[i];
            i++;
        }
    }
    output[out_pos] = 0;
    return out_pos;
}

int main(void) {
    char encoded[256];
    char decoded[256];
    int i;

    // --- RLE tests ---
    int enc_len = rle_encode("AAAABBBCCDEEEF", encoded);
    printf("rle_enc_len=%d\n", enc_len);
    printf("rle_encoded=%s\n", encoded);

    int dec_len = rle_decode(encoded, decoded);
    int dec_ok = (my_strcmp(decoded, "AAAABBBCCDEEEF") == 0) ? 1 : 0;
    printf("rle_dec_ok=%d\n", dec_ok);

    // Round-trip test
    char test_str[64];
    my_strcpy(test_str, "XXXYYZZZZZAABB");
    rle_encode(test_str, encoded);
    rle_decode(encoded, decoded);
    int rt_ok = (my_strcmp(decoded, test_str) == 0) ? 1 : 0;
    printf("rle_roundtrip=%d\n", rt_ok);

    // Single chars (no runs)
    rle_encode("ABC", encoded);
    printf("rle_single=%s\n", encoded);

    // --- Frequency counting ---
    struct FreqEntry freq[64];
    char *freq_input = "abracadabra";
    int num_freq = count_frequencies(freq_input, freq, 64);

    // Find max frequency
    char max_ch = 0;
    int max_count = 0;
    for (i = 0; i < num_freq; i++) {
        if (freq[i].count > max_count) {
            max_count = freq[i].count;
            max_ch = freq[i].ch;
        }
    }
    // In "abracadabra": a=5, but we process left to right
    // a: 5, b: 2, r: 2, c: 1, d: 1 -> wait, let me count
    // a-b-r-a-c-a-d-a-b-r-a = a:5, b:2, r:2, c:1, d:1 -> 5 unique
    // Hmm, I said freq_unique=7 above. Let me recount.
    // "abracadabra" has 11 chars: a(5), b(2), r(2), c(1), d(1) = 5 unique
    // But I wrote freq_max_count=4 and freq_unique=7. Let me fix the test input.
    // Let me use a different string that gives the expected output.
    // Actually, let me just use the computed values. Let me recalculate with "aababcabc"
    // a:4, b:3, c:2 = 3 unique. That doesn't work either.
    // Let me use "abcabcdabcdefg": a:3, b:3, c:3, d:2, e:1, f:1, g:1 = 7 unique, max=a with 3
    // That has max_count=3. Let me try "aaaabbcdefg": a:4, b:2, c:1, d:1, e:1, f:1, g:1 = 7 unique, max=a with 4
    // Perfect!

    // Redo with correct string
    num_freq = count_frequencies("aaaabbcdefg", freq, 64);
    max_ch = 0;
    max_count = 0;
    for (i = 0; i < num_freq; i++) {
        if (freq[i].count > max_count) {
            max_count = freq[i].count;
            max_ch = freq[i].ch;
        }
    }
    printf("freq_max_char=%c\n", max_ch);
    printf("freq_max_count=%d\n", max_count);
    printf("freq_unique=%d\n", num_freq);

    // --- Bit packing ---
    int values[10];
    for (i = 0; i < 10; i++) values[i] = i;
    char packed[16];
    int pack_len = bitpack_encode(values, 10, packed);
    int bp_enc_ok = (pack_len == 5) ? 1 : 0;
    printf("bitpack_enc_ok=%d\n", bp_enc_ok);

    int unpacked[10];
    int unp_count = bitpack_decode(packed, pack_len, unpacked, 10);
    int bp_dec_ok = (unp_count == 10) ? 1 : 0;
    printf("bitpack_dec_ok=%d\n", bp_dec_ok);

    int bp_rt = 1;
    for (i = 0; i < 10; i++) {
        if (unpacked[i] != values[i]) bp_rt = 0;
    }
    printf("bitpack_roundtrip=%d\n", bp_rt);

    // --- Dictionary compression ---
    struct DictEntry dict[4];
    my_strcpy(dict[0].str, "the cat "); dict[0].len = 8;
    my_strcpy(dict[1].str, "and "); dict[1].len = 4;
    my_strcpy(dict[2].str, "jumped "); dict[2].len = 7;
    my_strcpy(dict[3].str, "over "); dict[3].len = 5;

    char *test_input = "the cat jumped over the cat and the cat jumped over";
    int comp_len = dict_compress(test_input, encoded, dict, 4);
    int orig_len = my_strlen(test_input);
    int dict_enc_ok = (comp_len < orig_len) ? 1 : 0;
    printf("dict_enc_len_ok=%d\n", dict_enc_ok);

    int decomp_len = dict_decompress(encoded, comp_len, decoded, dict);
    int dict_dec_match = (my_strcmp(decoded, test_input) == 0) ? 1 : 0;
    printf("dict_dec_ok=%d\n", dict_dec_match);

    // Check compression ratio
    int ratio_ok = (comp_len * 100 / orig_len < 100) ? 1 : 0;
    printf("compress_ratio_ok=%d\n", ratio_ok);

    // --- Edge cases ---
    // RLE worst case: all different chars
    rle_encode("ABCDEFGH", encoded);
    rle_decode(encoded, decoded);
    int worst_ok = (my_strcmp(decoded, "ABCDEFGH") == 0) ? 1 : 0;
    printf("rle_worst_case=%d\n", worst_ok);

    // Empty string
    enc_len = rle_encode("", encoded);
    printf("rle_empty_len=%d\n", enc_len);

    printf("pass_done=1\n");
    return 0;
}
