int printf(const char *fmt, ...);
// EXPECT: encode 'aaabbc': a3b2c1\nencode 'aabbbcccc': a2b3c4\nencode 'abcd': a1b1c1d1\nencode 'aaaaaaa': a7\nencode 'x': x1\ndecode 'a3b2c1': aaabbc\ndecode 'x5y3z1': xxxxxyyyz\nroundtrip: 'aaabbccdddd' -> 'a3b2c2d4' -> 'aaabbccdddd'\nroundtrip equal: 1\noriginal len: 11, encoded len: 8
// Test: run-length encoding and decoding

int my_strlen(char *s) {
    int i = 0;
    while (s[i] != '\0') i = i + 1;
    return i;
}

// Encode: "aaabbc" -> "a3b2c1"
int rle_encode(char *input, char *output) {
    int i = 0;
    int wi = 0;
    int len = my_strlen(input);

    while (i < len) {
        char ch = input[i];
        int count = 1;
        while (i + count < len && input[i + count] == ch) {
            count = count + 1;
        }
        output[wi] = ch;
        wi = wi + 1;

        // Write count (single digit for simplicity)
        if (count < 10) {
            output[wi] = '0' + count;
            wi = wi + 1;
        } else {
            // two digit
            output[wi] = '0' + count / 10;
            wi = wi + 1;
            output[wi] = '0' + count % 10;
            wi = wi + 1;
        }
        i = i + count;
    }
    output[wi] = '\0';
    return wi;
}

// Decode: "a3b2c1" -> "aaabbc"
int rle_decode(char *input, char *output) {
    int i = 0;
    int wi = 0;
    int len = my_strlen(input);

    while (i < len) {
        char ch = input[i];
        i = i + 1;
        int count = 0;
        while (i < len && input[i] >= '0' && input[i] <= '9') {
            count = count * 10 + (input[i] - '0');
            i = i + 1;
        }
        int j;
        for (j = 0; j < count; j++) {
            output[wi] = ch;
            wi = wi + 1;
        }
    }
    output[wi] = '\0';
    return wi;
}

int main(void) {
    char encoded[100];
    char decoded[100];

    // Basic encoding
    rle_encode("aaabbc", encoded);
    printf("encode 'aaabbc': %s\n", encoded);

    rle_encode("aabbbcccc", encoded);
    printf("encode 'aabbbcccc': %s\n", encoded);

    // Single chars
    rle_encode("abcd", encoded);
    printf("encode 'abcd': %s\n", encoded);

    // All same
    rle_encode("aaaaaaa", encoded);
    printf("encode 'aaaaaaa': %s\n", encoded);

    // Single char
    rle_encode("x", encoded);
    printf("encode 'x': %s\n", encoded);

    // Decoding
    rle_decode("a3b2c1", decoded);
    printf("decode 'a3b2c1': %s\n", decoded);

    rle_decode("x5y3z1", decoded);
    printf("decode 'x5y3z1': %s\n", decoded);

    // Round-trip
    char *original = "aaabbccdddd";
    rle_encode(original, encoded);
    rle_decode(encoded, decoded);
    printf("roundtrip: '%s' -> '%s' -> '%s'\n", original, encoded, decoded);

    // Check equality
    int eq = 1;
    int i = 0;
    while (original[i] != '\0' && decoded[i] != '\0') {
        if (original[i] != decoded[i]) { eq = 0; break; }
        i = i + 1;
    }
    if (original[i] != decoded[i]) eq = 0;
    printf("roundtrip equal: %d\n", eq);

    // Compression ratio
    int orig_len = my_strlen(original);
    int enc_len = my_strlen(encoded);
    printf("original len: %d, encoded len: %d\n", orig_len, enc_len);

    return 0;
}
