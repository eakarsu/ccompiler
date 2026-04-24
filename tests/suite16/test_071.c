int printf(const char *fmt, ...);

// Test 071: Run-Length Encoding (RLE) - basic compression and decompression

void rle_encode(char *input, int inlen, char *out_chars, int *out_counts, int *out_len) {
    int i = 0;
    int pos = 0;
    while (i < inlen) {
        char c = input[i];
        int count = 1;
        while (i + count < inlen && input[i + count] == c && count < 255) {
            count = count + 1;
        }
        out_chars[pos] = c;
        out_counts[pos] = count;
        pos = pos + 1;
        i = i + count;
    }
    *out_len = pos;
}

void rle_decode(char *chars, int *counts, int pairs, char *output, int *out_len) {
    int pos = 0;
    int i;
    for (i = 0; i < pairs; i = i + 1) {
        int j;
        for (j = 0; j < counts[i]; j = j + 1) {
            output[pos] = chars[i];
            pos = pos + 1;
        }
    }
    *out_len = pos;
}

int strings_equal(char *a, char *b, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int compute_ratio(int original, int compressed) {
    if (original == 0) return 0;
    return (compressed * 100) / original;
}

int main() {
    char input1[20];
    input1[0] = 'A'; input1[1] = 'A'; input1[2] = 'A';
    input1[3] = 'B'; input1[4] = 'B';
    input1[5] = 'C'; input1[6] = 'C'; input1[7] = 'C'; input1[8] = 'C';
    input1[9] = 'A';
    int inlen1 = 10;

    char enc_chars[20];
    int enc_counts[20];
    int enc_len;

    rle_encode(input1, inlen1, enc_chars, enc_counts, &enc_len);
    printf("RLE pairs: %d\n", enc_len); // EXPECT: RLE pairs: 4

    int i;
    for (i = 0; i < enc_len; i = i + 1) {
        printf("Pair %d: char=%d count=%d\n", i, enc_chars[i], enc_counts[i]); // EXPECT: Pair 0: char=65 count=3
        // EXPECT: Pair 1: char=66 count=2
        // EXPECT: Pair 2: char=67 count=4
        // EXPECT: Pair 3: char=65 count=1
    }

    char decoded[20];
    int dec_len;
    rle_decode(enc_chars, enc_counts, enc_len, decoded, &dec_len);
    printf("Decoded length: %d\n", dec_len); // EXPECT: Decoded length: 10

    int match = strings_equal(input1, decoded, inlen1);
    printf("Roundtrip match: %d\n", match); // EXPECT: Roundtrip match: 1

    int ratio = compute_ratio(inlen1, enc_len * 2);
    printf("Compression ratio: %d\n", ratio); // EXPECT: Compression ratio: 80

    // Test with all same characters
    char input2[16];
    for (i = 0; i < 16; i = i + 1) {
        input2[i] = 'X';
    }
    rle_encode(input2, 16, enc_chars, enc_counts, &enc_len);
    printf("All same pairs: %d\n", enc_len); // EXPECT: All same pairs: 1
    printf("All same count: %d\n", enc_counts[0]); // EXPECT: All same count: 16

    // Test with all different characters
    char input3[5];
    input3[0] = 'A'; input3[1] = 'B'; input3[2] = 'C';
    input3[3] = 'D'; input3[4] = 'E';
    rle_encode(input3, 5, enc_chars, enc_counts, &enc_len);
    printf("All diff pairs: %d\n", enc_len); // EXPECT: All diff pairs: 5

    int ratio2 = compute_ratio(5, enc_len * 2);
    printf("All diff ratio: %d\n", ratio2); // EXPECT: All diff ratio: 200

    // Verify decode of all-same
    rle_encode(input2, 16, enc_chars, enc_counts, &enc_len);
    rle_decode(enc_chars, enc_counts, enc_len, decoded, &dec_len);
    printf("All same decoded len: %d\n", dec_len); // EXPECT: All same decoded len: 16
    int match2 = strings_equal(input2, decoded, 16);
    printf("All same roundtrip: %d\n", match2); // EXPECT: All same roundtrip: 1

    // Test total encoded size
    int total_encoded = 0;
    for (i = 0; i < enc_len; i = i + 1) {
        total_encoded = total_encoded + enc_counts[i];
    }
    printf("Total decoded chars: %d\n", total_encoded); // EXPECT: Total decoded chars: 16

    printf("Done\n"); // EXPECT: Done
    return 0;
}
