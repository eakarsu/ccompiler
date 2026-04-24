int printf(const char *fmt, ...);

// LZ77-style Sliding Window Compression

struct LZ77Token {
    int offset;
    int length;
    int next_char;
};

int find_match(char *data, int pos, int window_start, int *best_offset, int *best_length) {
    int max_len = 0;
    int max_off = 0;
    int s = window_start;
    while (s < pos) {
        int len = 0;
        while (data[pos + len] != 0 && data[s + len] == data[pos + len] && len < 15) {
            len = len + 1;
        }
        if (len > max_len) {
            max_len = len;
            max_off = pos - s;
        }
        s = s + 1;
    }
    *best_offset = max_off;
    *best_length = max_len;
    return max_len;
}

int lz77_compress(char *input, struct LZ77Token *tokens, int window_size) {
    int pos = 0;
    int token_count = 0;
    int input_len = 0;
    while (input[input_len] != 0) input_len = input_len + 1;

    while (pos < input_len) {
        int window_start = pos - window_size;
        if (window_start < 0) window_start = 0;

        int best_offset = 0;
        int best_length = 0;
        find_match(input, pos, window_start, &best_offset, &best_length);

        if (best_length >= 2) {
            int next_pos = pos + best_length;
            int nc = 0;
            if (next_pos < input_len) {
                nc = input[next_pos];
            }
            tokens[token_count].offset = best_offset;
            tokens[token_count].length = best_length;
            tokens[token_count].next_char = nc;
            pos = next_pos + 1;
        } else {
            tokens[token_count].offset = 0;
            tokens[token_count].length = 0;
            tokens[token_count].next_char = input[pos];
            pos = pos + 1;
        }
        token_count = token_count + 1;
    }
    return token_count;
}

int lz77_decompress(struct LZ77Token *tokens, int count, char *output) {
    int pos = 0;
    int i = 0;
    while (i < count) {
        if (tokens[i].length > 0) {
            int start = pos - tokens[i].offset;
            int j = 0;
            while (j < tokens[i].length) {
                output[pos] = output[start + j];
                pos = pos + 1;
                j = j + 1;
            }
        }
        if (tokens[i].next_char != 0) {
            output[pos] = tokens[i].next_char;
            pos = pos + 1;
        }
        i = i + 1;
    }
    output[pos] = 0;
    return pos;
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
    struct LZ77Token tokens[64];

    // Test 1: Repeated pattern
    char input1[32];
    input1[0] = 'a'; input1[1] = 'b'; input1[2] = 'c';
    input1[3] = 'a'; input1[4] = 'b'; input1[5] = 'c';
    input1[6] = 'a'; input1[7] = 'b'; input1[8] = 'c';
    input1[9] = 0;

    int count = lz77_compress(input1, tokens, 8);
    printf("Token count: %d\n", count); // EXPECT: Token count: 4

    // Print tokens
    int i = 0;
    while (i < count) {
        printf("T%d: off=%d len=%d ch=%d\n", i, tokens[i].offset, tokens[i].length, tokens[i].next_char);
        i = i + 1;
    }
    // EXPECT: T0: off=0 len=0 ch=97
    // EXPECT: T1: off=0 len=0 ch=98
    // EXPECT: T2: off=0 len=0 ch=99
    // EXPECT: T3: off=3 len=6 ch=0

    // Decompress and verify
    char output[64];
    int out_len = lz77_decompress(tokens, count, output);
    printf("Decompressed: %s\n", output); // EXPECT: Decompressed: abcabcabc
    printf("Output length: %d\n", out_len); // EXPECT: Output length: 9

    int match = str_equal(input1, output);
    printf("Match: %d\n", match); // EXPECT: Match: 1

    // Test 2: No repetition
    char input2[8];
    input2[0] = 'x'; input2[1] = 'y'; input2[2] = 'z'; input2[3] = 'w'; input2[4] = 0;

    int count2 = lz77_compress(input2, tokens, 8);
    printf("No-rep tokens: %d\n", count2); // EXPECT: No-rep tokens: 4

    char output2[64];
    lz77_decompress(tokens, count2, output2);
    printf("No-rep decoded: %s\n", output2); // EXPECT: No-rep decoded: xyzw

    // Test 3: All same chars
    char input3[16];
    i = 0;
    while (i < 8) { input3[i] = 'k'; i = i + 1; }
    input3[8] = 0;

    int count3 = lz77_compress(input3, tokens, 8);
    printf("All-same tokens: %d\n", count3); // EXPECT: All-same tokens: 2

    char output3[64];
    lz77_decompress(tokens, count3, output3);
    printf("All-same decoded: %s\n", output3); // EXPECT: All-same decoded: kkkkkkkk

    int match3 = str_equal(input3, output3);
    printf("All-same match: %d\n", match3); // EXPECT: All-same match: 1

    printf("LZ77 done\n"); // EXPECT: LZ77 done
    return 0;
}
