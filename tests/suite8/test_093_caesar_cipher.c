int printf(const char *fmt, ...);

/* Caesar cipher: shift-based encryption and decryption */

int str_len(char *s) {
    int n;
    n = 0;
    while (s[n]) n = n + 1;
    return n;
}

int is_upper(int c) {
    return (c >= 65 && c <= 90);
}

int is_lower(int c) {
    return (c >= 97 && c <= 122);
}

void caesar_encrypt(char *input, char *output, int shift) {
    int i;
    int len;
    len = str_len(input);
    for (i = 0; i < len; i = i + 1) {
        int c;
        c = input[i];
        if (is_upper(c)) {
            output[i] = ((c - 65 + shift) % 26) + 65;
        } else if (is_lower(c)) {
            output[i] = ((c - 97 + shift) % 26) + 97;
        } else {
            output[i] = c;
        }
    }
    output[len] = 0;
}

void caesar_decrypt(char *input, char *output, int shift) {
    int effective_shift;
    effective_shift = 26 - (shift % 26);
    caesar_encrypt(input, output, effective_shift);
}

int strings_equal(char *a, char *b) {
    int i;
    i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return (a[i] == 0 && b[i] == 0);
}

void test_basic_encrypt(void) {
    char buf[64];
    caesar_encrypt("abc", buf, 3);
    // EXPECT: encrypt abc by 3: def
    printf("encrypt abc by 3: %s\n", buf);
    caesar_encrypt("xyz", buf, 3);
    // EXPECT: encrypt xyz by 3: abc
    printf("encrypt xyz by 3: %s\n", buf);
    caesar_encrypt("ABC", buf, 1);
    // EXPECT: encrypt ABC by 1: BCD
    printf("encrypt ABC by 1: %s\n", buf);
}

void test_decrypt(void) {
    char buf[64];
    caesar_decrypt("def", buf, 3);
    // EXPECT: decrypt def by 3: abc
    printf("decrypt def by 3: %s\n", buf);
    caesar_decrypt("abc", buf, 3);
    // EXPECT: decrypt abc by 3: xyz
    printf("decrypt abc by 3: %s\n", buf);
}

void test_roundtrip(void) {
    char encrypted[64];
    char decrypted[64];
    int match;
    caesar_encrypt("hello", encrypted, 13);
    caesar_decrypt(encrypted, decrypted, 13);
    match = strings_equal(decrypted, "hello");
    // EXPECT: roundtrip hello shift=13: match=1
    printf("roundtrip hello shift=13: match=%d\n", match);
    caesar_encrypt("WORLD", encrypted, 7);
    caesar_decrypt(encrypted, decrypted, 7);
    match = strings_equal(decrypted, "WORLD");
    // EXPECT: roundtrip WORLD shift=7: match=1
    printf("roundtrip WORLD shift=7: match=%d\n", match);
}

void test_preserve_nonalpha(void) {
    char buf[64];
    caesar_encrypt("hi 42", buf, 5);
    // EXPECT: encrypt with spaces: mn 42
    printf("encrypt with spaces: %s\n", buf);
}

void test_all_shifts(void) {
    char enc[64];
    char dec[64];
    int shift;
    int all_ok;
    all_ok = 1;
    for (shift = 0; shift < 26; shift = shift + 1) {
        caesar_encrypt("test", enc, shift);
        caesar_decrypt(enc, dec, shift);
        if (!strings_equal(dec, "test")) {
            all_ok = 0;
        }
    }
    // EXPECT: all 26 shifts roundtrip: 1
    printf("all 26 shifts roundtrip: %d\n", all_ok);
}

void test_shift_zero(void) {
    char buf[64];
    caesar_encrypt("same", buf, 0);
    // EXPECT: shift 0: same
    printf("shift 0: %s\n", buf);
}

void test_full_alphabet(void) {
    char buf[64];
    caesar_encrypt("abcdefghijklmnopqrstuvwxyz", buf, 13);
    // EXPECT: full rot13: nopqrstuvwxyzabcdefghijklm
    printf("full rot13: %s\n", buf);
}

int main(void) {
    test_basic_encrypt();
    test_decrypt();
    test_roundtrip();
    test_preserve_nonalpha();
    test_all_shifts();
    test_shift_zero();
    test_full_alphabet();
    // EXPECT: done
    printf("done\n");
    return 0;
}
