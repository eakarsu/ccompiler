int printf(const char *fmt, ...);

// Run-Length Encoding and Decoding

void rle_encode(char *input, char *output) {
    int i = 0;
    int j = 0;
    while (input[i] != 0) {
        char ch = input[i];
        int count = 1;
        while (input[i + count] == ch) {
            count = count + 1;
        }
        if (count > 1) {
            // Write count digit (assume single digit counts for simplicity)
            output[j] = '0' + count;
            j = j + 1;
        }
        output[j] = ch;
        j = j + 1;
        i = i + count;
    }
    output[j] = 0;
}

void rle_decode(char *input, char *output) {
    int i = 0;
    int j = 0;
    while (input[i] != 0) {
        int count = 1;
        if (input[i] >= '2' && input[i] <= '9') {
            count = input[i] - '0';
            i = i + 1;
        }
        char ch = input[i];
        int k = 0;
        while (k < count) {
            output[j] = ch;
            j = j + 1;
            k = k + 1;
        }
        i = i + 1;
    }
    output[j] = 0;
}

int str_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) len = len + 1;
    return len;
}

int compute_ratio(int orig, int compressed) {
    if (orig == 0) return 0;
    return (compressed * 100) / orig;
}

int main() {
    char encoded[256];
    char decoded[256];

    // Test 1: Simple repeated chars
    char input1[32];
    input1[0] = 'a'; input1[1] = 'a'; input1[2] = 'a';
    input1[3] = 'b'; input1[4] = 'b';
    input1[5] = 'c'; input1[6] = 'c'; input1[7] = 'c'; input1[8] = 'c';
    input1[9] = 0;
    rle_encode(input1, encoded);
    printf("Encoded: %s\n", encoded); // EXPECT: Encoded: 3a2b4c

    rle_decode(encoded, decoded);
    printf("Decoded: %s\n", decoded); // EXPECT: Decoded: aaabbcccc

    int match1 = str_equal(input1, decoded);
    printf("Roundtrip match: %d\n", match1); // EXPECT: Roundtrip match: 1

    // Test 2: No repeats
    char input2[8];
    input2[0] = 'x'; input2[1] = 'y'; input2[2] = 'z'; input2[3] = 0;
    rle_encode(input2, encoded);
    printf("Encoded no-repeat: %s\n", encoded); // EXPECT: Encoded no-repeat: xyz

    rle_decode(encoded, decoded);
    printf("Decoded no-repeat: %s\n", decoded); // EXPECT: Decoded no-repeat: xyz

    // Test 3: All same
    char input3[16];
    int i = 0;
    while (i < 7) {
        input3[i] = 'm';
        i = i + 1;
    }
    input3[7] = 0;
    rle_encode(input3, encoded);
    printf("All same encoded: %s\n", encoded); // EXPECT: All same encoded: 7m

    rle_decode(encoded, decoded);
    int match3 = str_equal(input3, decoded);
    printf("All same roundtrip: %d\n", match3); // EXPECT: All same roundtrip: 1

    // Test 4: Compression ratio
    int orig_len = str_len(input1);
    int enc_len = str_len(encoded);
    // recompute encoded for input1
    rle_encode(input1, encoded);
    enc_len = str_len(encoded);
    int ratio = compute_ratio(orig_len, enc_len);
    printf("Original len: %d\n", orig_len); // EXPECT: Original len: 9
    printf("Compressed len: %d\n", enc_len); // EXPECT: Compressed len: 6
    printf("Ratio percent: %d\n", ratio); // EXPECT: Ratio percent: 66

    // Test 5: Single char
    char input5[4];
    input5[0] = 'q'; input5[1] = 0;
    rle_encode(input5, encoded);
    printf("Single encoded: %s\n", encoded); // EXPECT: Single encoded: q
    rle_decode(encoded, decoded);
    printf("Single decoded: %s\n", decoded); // EXPECT: Single decoded: q

    // Test 6: Two different chars
    char input6[4];
    input6[0] = 'a'; input6[1] = 'b'; input6[2] = 0;
    rle_encode(input6, encoded);
    printf("Two diff encoded: %s\n", encoded); // EXPECT: Two diff encoded: ab

    printf("Done\n"); // EXPECT: Done
    return 0;
}
