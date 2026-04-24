int printf(const char *fmt, ...);

// Test 072: LZ77-style sliding window compression

// Match structure stored as parallel arrays
// For each output token: type (0=literal, 1=match), value/offset, length

int find_match(char *data, int pos, int window_start, int *best_off, int *best_len) {
    int max_len = 0;
    int max_off = 0;
    int s;
    for (s = window_start; s < pos; s = s + 1) {
        int len = 0;
        while (pos + len < 100 && data[s + len] == data[pos + len] && len < 15) {
            len = len + 1;
        }
        if (len > max_len) {
            max_len = len;
            max_off = pos - s;
        }
    }
    *best_off = max_off;
    *best_len = max_len;
    return max_len >= 3;
}

int lz77_compress(char *input, int inlen, int *types, int *values, int *lengths) {
    int pos = 0;
    int out = 0;
    int window_size = 16;
    while (pos < inlen) {
        int window_start = pos - window_size;
        if (window_start < 0) window_start = 0;
        int off;
        int len;
        int found = find_match(input, pos, window_start, &off, &len);
        if (found) {
            types[out] = 1;
            values[out] = off;
            lengths[out] = len;
            pos = pos + len;
        } else {
            types[out] = 0;
            values[out] = input[pos];
            lengths[out] = 1;
            pos = pos + 1;
        }
        out = out + 1;
    }
    return out;
}

int lz77_decompress(int *types, int *values, int *lengths, int count, char *output) {
    int pos = 0;
    int i;
    for (i = 0; i < count; i = i + 1) {
        if (types[i] == 0) {
            output[pos] = (char)values[i];
            pos = pos + 1;
        } else {
            int off = values[i];
            int len = lengths[i];
            int j;
            for (j = 0; j < len; j = j + 1) {
                output[pos] = output[pos - off];
                pos = pos + 1;
            }
        }
    }
    return pos;
}

int main() {
    // Test with repetitive data
    char data1[30];
    data1[0] = 'A'; data1[1] = 'B'; data1[2] = 'C';
    data1[3] = 'A'; data1[4] = 'B'; data1[5] = 'C';
    data1[6] = 'A'; data1[7] = 'B'; data1[8] = 'C';
    data1[9] = 'D';
    int len1 = 10;

    int types[50];
    int values[50];
    int lengths[50];

    int tokens = lz77_compress(data1, len1, types, values, lengths);
    printf("Tokens: %d\n", tokens); // EXPECT: Tokens: 5

    int i;
    for (i = 0; i < tokens; i = i + 1) {
        if (types[i] == 0) {
            printf("Token %d: literal %d\n", i, values[i]); // EXPECT: Token 0: literal 65
            // EXPECT: Token 1: literal 66
            // EXPECT: Token 2: literal 67
            // EXPECT: Token 3: match off=3 len=6
        } else {
            printf("Token %d: match off=%d len=%d\n", i, values[i], lengths[i]);
            // EXPECT: Token 4: literal 68
        }
    }

    // Decompress and verify
    char output[50];
    int dec_len = lz77_decompress(types, values, lengths, tokens, output);
    printf("Decompressed len: %d\n", dec_len); // EXPECT: Decompressed len: 10

    // But token 3 is a match not literal, let me re-check the flow
    // After the match at token index 3, pos jumps by 6 (ABC ABC), then D is literal
    // Actually let me recount: tokens 0,1,2 are A,B,C literals, token 3 is match(3,6) covering ABCABC
    // Wait that's only 3+6=9, then D at pos 9 should be token 4
    // Let me just verify by running

    int match = 1;
    int j;
    for (j = 0; j < len1; j = j + 1) {
        if (data1[j] != output[j]) match = 0;
    }
    printf("Roundtrip ok: %d\n", match); // EXPECT: Roundtrip ok: 1

    // Test with no repetition
    char data2[5];
    data2[0] = 'W'; data2[1] = 'X'; data2[2] = 'Y'; data2[3] = 'Z'; data2[4] = 'Q';
    int tokens2 = lz77_compress(data2, 5, types, values, lengths);
    printf("No repeat tokens: %d\n", tokens2); // EXPECT: No repeat tokens: 5

    // Test with all same character
    char data3[8];
    for (i = 0; i < 8; i = i + 1) data3[i] = 'M';
    int tokens3 = lz77_compress(data3, 8, types, values, lengths);
    printf("All same tokens: %d\n", tokens3); // EXPECT: All same tokens: 2

    char out3[20];
    int dec3 = lz77_decompress(types, values, lengths, tokens3, out3);
    printf("All same decoded: %d\n", dec3); // EXPECT: All same decoded: 8

    int ok3 = 1;
    for (i = 0; i < 8; i = i + 1) {
        if (out3[i] != 'M') ok3 = 0;
    }
    printf("All same match: %d\n", ok3); // EXPECT: All same match: 1

    printf("Done\n"); // EXPECT: Done
    return 0;
}
