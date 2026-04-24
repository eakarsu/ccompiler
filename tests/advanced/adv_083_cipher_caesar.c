// EXPECT: === Caesar Cipher Tests ===
// EXPECT: Encrypt "HELLO" shift 3: KHOOR
// EXPECT: Decrypt "KHOOR" shift 3: HELLO
// EXPECT: Encrypt "abc" shift 1: bcd
// EXPECT: Decrypt "bcd" shift 1: abc
// EXPECT: Encrypt "XYZ" shift 3: ABC
// EXPECT: Encrypt "xyz" shift 3: abc
// EXPECT: Encrypt "Hello, World!" shift 5: Mjqqt, Btwqi!
// EXPECT: Decrypt "Mjqqt, Btwqi!" shift 5: Hello, World!
// EXPECT: Encrypt "ATTACK AT DAWN" shift 13: NGGNPX NG QNJA
// EXPECT: Decrypt "NGGNPX NG QNJA" shift 13: ATTACK AT DAWN
// EXPECT: Encrypt "" shift 7: (empty)
// EXPECT: Encrypt "12345" shift 5: 12345
// EXPECT: Encrypt shift 0 is identity: HELLO
// EXPECT: Encrypt shift 26 is identity: HELLO
// EXPECT: Encrypt then decrypt roundtrip: SUCCESS
// EXPECT: Brute force crack of "KHOOR":
// EXPECT:   shift 3: HELLO
// EXPECT: Found target at shift 3
// EXPECT: All Caesar cipher tests passed!

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

void caesar_encrypt(char *input, int shift, char *output) {
    int s = shift % 26;
    if (s < 0) s = s + 26;
    int i = 0;
    while (input[i] != '\0') {
        char c = input[i];
        if (c >= 'A' && c <= 'Z') {
            output[i] = (char)('A' + (c - 'A' + s) % 26);
        } else if (c >= 'a' && c <= 'z') {
            output[i] = (char)('a' + (c - 'a' + s) % 26);
        } else {
            output[i] = c;
        }
        i = i + 1;
    }
    output[i] = '\0';
}

void caesar_decrypt(char *input, int shift, char *output) {
    caesar_encrypt(input, 26 - (shift % 26), output);
}

int main() {
    char buf[128];
    char buf2[128];

    printf("=== Caesar Cipher Tests ===\n");

    // Basic encrypt/decrypt uppercase
    caesar_encrypt("HELLO", 3, buf);
    printf("Encrypt \"HELLO\" shift 3: %s\n", buf);

    caesar_decrypt("KHOOR", 3, buf);
    printf("Decrypt \"KHOOR\" shift 3: %s\n", buf);

    // Lowercase
    caesar_encrypt("abc", 1, buf);
    printf("Encrypt \"abc\" shift 1: %s\n", buf);

    caesar_decrypt("bcd", 1, buf);
    printf("Decrypt \"bcd\" shift 1: %s\n", buf);

    // Wrap around
    caesar_encrypt("XYZ", 3, buf);
    printf("Encrypt \"XYZ\" shift 3: %s\n", buf);

    caesar_encrypt("xyz", 3, buf);
    printf("Encrypt \"xyz\" shift 3: %s\n", buf);

    // Mixed case with punctuation
    caesar_encrypt("Hello, World!", 5, buf);
    printf("Encrypt \"Hello, World!\" shift 5: %s\n", buf);

    caesar_decrypt("Mjqqt, Btwqi!", 5, buf);
    printf("Decrypt \"Mjqqt, Btwqi!\" shift 5: %s\n", buf);

    // ROT13
    caesar_encrypt("ATTACK AT DAWN", 13, buf);
    printf("Encrypt \"ATTACK AT DAWN\" shift 13: %s\n", buf);

    caesar_decrypt("NGGNPX NG QNJA", 13, buf);
    printf("Decrypt \"NGGNPX NG QNJA\" shift 13: %s\n", buf);

    // Edge cases
    caesar_encrypt("", 7, buf);
    printf("Encrypt \"\" shift 7: %s\n", buf[0] == '\0' ? "(empty)" : buf);

    caesar_encrypt("12345", 5, buf);
    printf("Encrypt \"12345\" shift 5: %s\n", buf);

    caesar_encrypt("HELLO", 0, buf);
    printf("Encrypt shift 0 is identity: %s\n", buf);

    caesar_encrypt("HELLO", 26, buf);
    printf("Encrypt shift 26 is identity: %s\n", buf);

    // Roundtrip test
    caesar_encrypt("The Quick Brown Fox", 17, buf);
    caesar_decrypt(buf, 17, buf2);
    printf("Encrypt then decrypt roundtrip: %s\n",
           str_eq(buf2, "The Quick Brown Fox") ? "SUCCESS" : "FAIL");

    // Brute force crack
    printf("Brute force crack of \"KHOOR\":\n");
    int found_shift = -1;
    int shift;
    for (shift = 0; shift < 26; shift++) {
        caesar_decrypt("KHOOR", shift, buf);
        if (str_eq(buf, "HELLO")) {
            printf("  shift %d: %s\n", shift, buf);
            found_shift = shift;
        }
    }
    printf("Found target at shift %d\n", found_shift);

    printf("All Caesar cipher tests passed!\n");
    return 0;
}
