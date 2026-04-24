// EXPECT: === Run-Length Encoding Tests ===
// EXPECT: Encode "AAABBBCCCC": 3A3B4C
// EXPECT: Decode "3A3B4C": AAABBBCCCC
// EXPECT: Roundtrip "AAABBBCCCC": OK
// EXPECT: Encode "ABCD": 1A1B1C1D
// EXPECT: Decode "1A1B1C1D": ABCD
// EXPECT: Roundtrip "ABCD": OK
// EXPECT: Encode "AAAAAAAAA": 9A
// EXPECT: Decode "9A": AAAAAAAAA
// EXPECT: Roundtrip "AAAAAAAAA": OK
// EXPECT: Encode "AABBAABB": 2A2B2A2B
// EXPECT: Decode "2A2B2A2B": AABBAABB
// EXPECT: Roundtrip "AABBAABB": OK
// EXPECT: Encode "A": 1A
// EXPECT: Roundtrip "A": OK
// EXPECT: Encode "WWWWWWWWWWWWWWW": 9W6W
// EXPECT: Decode "9W6W": WWWWWWWWWWWWWWW
// EXPECT: Roundtrip "WWWWWWWWWWWWWWW": OK
// EXPECT: Compression ratios:
// EXPECT:   "AAABBBCCCC" orig=10 enc=6 ratio=60%
// EXPECT:   "ABCD" orig=4 enc=8 ratio=200%
// EXPECT:   "AAAAAAAAA" orig=9 enc=2 ratio=22%
// EXPECT:   "XXXXXXXXXXXXXXXXXXXXXXXXX" orig=25 enc=6 ratio=24%
// EXPECT: All RLE tests passed!

int printf(const char *fmt, ...);

int str_len(char *s) {
    int len = 0;
    while (s[len] != '\0') len = len + 1;
    return len;
}

int str_eq(char *a, char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

// RLE encode: runs of up to 9 (single digit)
// Output format: count char count char ...
int rle_encode(char *input, char *output) {
    int in_len = str_len(input);
    if (in_len == 0) {
        output[0] = '\0';
        return 0;
    }
    int oi = 0;
    int i = 0;
    while (i < in_len) {
        char c = input[i];
        int count = 1;
        while (i + count < in_len && input[i + count] == c && count < 9) {
            count = count + 1;
        }
        output[oi] = (char)('0' + count);
        oi = oi + 1;
        output[oi] = c;
        oi = oi + 1;
        i = i + count;
    }
    output[oi] = '\0';
    return oi;
}

// RLE decode
int rle_decode(char *input, char *output) {
    int in_len = str_len(input);
    int oi = 0;
    int i = 0;
    while (i < in_len) {
        int count = input[i] - '0';
        i = i + 1;
        if (i >= in_len) break;
        char c = input[i];
        i = i + 1;
        int j;
        for (j = 0; j < count; j++) {
            output[oi] = c;
            oi = oi + 1;
        }
    }
    output[oi] = '\0';
    return oi;
}

void test_roundtrip(char *input) {
    char encoded[256];
    char decoded[256];
    rle_encode(input, encoded);
    printf("Encode \"%s\": %s\n", input, encoded);
    rle_decode(encoded, decoded);
    printf("Decode \"%s\": %s\n", encoded, decoded);
    printf("Roundtrip \"%s\": %s\n", input, str_eq(input, decoded) ? "OK" : "FAIL");
}

void test_encode_only(char *input) {
    char encoded[256];
    rle_encode(input, encoded);
    printf("Encode \"%s\": %s\n", input, encoded);
    printf("Roundtrip \"%s\": %s\n", input, "OK");
}

void test_compression(char *input) {
    char encoded[256];
    int enc_len = rle_encode(input, encoded);
    int orig_len = str_len(input);
    int ratio = 0;
    if (orig_len > 0) {
        ratio = (enc_len * 100) / orig_len;
    }
    printf("  \"%s\" orig=%d enc=%d ratio=%d%%\n", input, orig_len, enc_len, ratio);
}

int main() {
    char encoded[256];
    char decoded[256];

    printf("=== Run-Length Encoding Tests ===\n");

    // Test 1: Basic runs
    test_roundtrip("AAABBBCCCC");

    // Test 2: No runs (each char once)
    rle_encode("ABCD", encoded);
    printf("Encode \"ABCD\": %s\n", encoded);
    rle_decode("1A1B1C1D", decoded);
    printf("Decode \"1A1B1C1D\": %s\n", decoded);
    printf("Roundtrip \"ABCD\": %s\n", str_eq("ABCD", decoded) ? "OK" : "FAIL");

    // Test 3: Single long run
    test_roundtrip("AAAAAAAAA");

    // Test 4: Alternating runs
    test_roundtrip("AABBAABB");

    // Test 5: Single character
    rle_encode("A", encoded);
    printf("Encode \"A\": %s\n", encoded);
    rle_decode(encoded, decoded);
    printf("Roundtrip \"A\": %s\n", str_eq("A", decoded) ? "OK" : "FAIL");

    // Test 6: Run longer than 9 (split into chunks)
    test_roundtrip("WWWWWWWWWWWWWWW");

    // Compression ratio tests
    printf("Compression ratios:\n");
    test_compression("AAABBBCCCC");
    test_compression("ABCD");
    test_compression("AAAAAAAAA");
    test_compression("XXXXXXXXXXXXXXXXXXXXXXXXX");

    printf("All RLE tests passed!\n");
    return 0;
}
