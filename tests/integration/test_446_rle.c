int printf(const char *fmt, ...);
// EXPECT: Input: AAABBBCCCCDDEEEEEE\nRuns: 5\nEncoded: A3B3C4D2E6\nDecoded: AAABBBCCCCDDEEEEEE\nMatch: 1\n\nInput: ABCDEF\nRuns: 6\nCompressed size: 6 (original: 6)\n\nInput: AAAAAAAAAAAAAAAAAAAABBBBBBBBBB\nRuns: 2\nEncoded: A20B10\nDecoded: AAAAAAAAAAAAAAAAAAAABBBBBBBBBB\nCompressed size: 6 (original: 30)
// Test: Run-length encoding and decoding

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) n++;
    return n;
}

int rle_encode(char *input, char *out_chars, int *out_counts) {
    int n = str_len(input);
    int runs = 0;
    int i = 0;

    while (i < n) {
        char c = input[i];
        int count = 1;
        while (i + count < n && input[i + count] == c) {
            count++;
        }
        out_chars[runs] = c;
        out_counts[runs] = count;
        runs++;
        i = i + count;
    }
    return runs;
}

void rle_decode(char *out_chars, int *out_counts, int runs, char *output) {
    int pos = 0;
    int i, j;
    for (i = 0; i < runs; i++) {
        for (j = 0; j < out_counts[i]; j++) {
            output[pos] = out_chars[i];
            pos++;
        }
    }
    output[pos] = 0;
}

int rle_compressed_size(char *out_chars, int *out_counts, int runs) {
    int size = 0;
    int i;
    for (i = 0; i < runs; i++) {
        size = size + 1;
        int c = out_counts[i];
        if (c > 1) {
            while (c > 0) { size++; c = c / 10; }
        }
    }
    return size;
}

int main(void) {
    char chars[64];
    int counts[64];
    char decoded[256];

    char s1[] = "AAABBBCCCCDDEEEEEE";
    int runs = rle_encode(s1, chars, counts);
    printf("Input: %s\n", s1);
    printf("Runs: %d\n", runs);
    printf("Encoded: ");
    int i;
    for (i = 0; i < runs; i++) {
        printf("%c%d", chars[i], counts[i]);
    }
    printf("\n");
    rle_decode(chars, counts, runs, decoded);
    printf("Decoded: %s\n", decoded);
    printf("Match: %d\n", str_len(s1) == str_len(decoded));

    char s2[] = "ABCDEF";
    runs = rle_encode(s2, chars, counts);
    printf("\nInput: %s\n", s2);
    printf("Runs: %d\n", runs);
    int csize = rle_compressed_size(chars, counts, runs);
    printf("Compressed size: %d (original: %d)\n", csize, str_len(s2));

    char s3[] = "AAAAAAAAAAAAAAAAAAAABBBBBBBBBB";
    runs = rle_encode(s3, chars, counts);
    printf("\nInput: %s\n", s3);
    printf("Runs: %d\n", runs);
    printf("Encoded: ");
    for (i = 0; i < runs; i++) {
        printf("%c%d", chars[i], counts[i]);
    }
    printf("\n");
    rle_decode(chars, counts, runs, decoded);
    printf("Decoded: %s\n", decoded);
    csize = rle_compressed_size(chars, counts, runs);
    printf("Compressed size: %d (original: %d)\n", csize, str_len(s3));

    return 0;
}
