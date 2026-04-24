int printf(const char *fmt, ...);
// EXPECT: encrypt 'hello' shift=3: khoor\ndecrypt back: hello\nROT13 'hello': uryyb\nROT13 twice: hello\nencrypt 'Hello World' shift=5: Mjqqt Btwqi\ndecrypt back: Hello World\nAll shifts of 'abc':\n  shift 0: abc\n  shift 1: bcd\n  shift 2: cde\n  shift 3: def\n  shift 4: efg\n  shift 5: fgh\n  shift 6: ghi\n  shift 7: hij\n  shift 8: ijk\n  shift 9: jkl\n  shift 10: klm\n  shift 11: lmn\n  shift 12: mno\n  shift 13: nop\n  shift 14: opq\n  shift 15: pqr\n  shift 16: qrs\n  shift 17: rst\n  shift 18: stu\n  shift 19: tuv\n  shift 20: uvw\n  shift 21: vwx\n  shift 22: wxy\n  shift 23: xyz\n  shift 24: yza\n  shift 25: zab\nencrypted: haahjr\nbrute force:\n  shift 7: attack (FOUND!)
// Test: Caesar cipher encryption and decryption

void caesar_encrypt(char *input, char *output, int shift) {
    int i = 0;
    while (input[i] != '\0') {
        char c = input[i];
        if (c >= 'a' && c <= 'z') {
            output[i] = 'a' + (c - 'a' + shift) % 26;
        } else if (c >= 'A' && c <= 'Z') {
            output[i] = 'A' + (c - 'A' + shift) % 26;
        } else {
            output[i] = c;
        }
        i = i + 1;
    }
    output[i] = '\0';
}

void caesar_decrypt(char *input, char *output, int shift) {
    // Decrypt is encrypt with (26 - shift)
    caesar_encrypt(input, output, 26 - shift);
}

int my_strcmp(char *a, char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return 1;
        i = i + 1;
    }
    return a[i] != b[i];
}

int main(void) {
    char encrypted[100];
    char decrypted[100];

    // Basic shift=3 (classic Caesar)
    caesar_encrypt("hello", encrypted, 3);
    printf("encrypt 'hello' shift=3: %s\n", encrypted);

    caesar_decrypt(encrypted, decrypted, 3);
    printf("decrypt back: %s\n", decrypted);

    // Shift=13 (ROT13)
    caesar_encrypt("hello", encrypted, 13);
    printf("ROT13 'hello': %s\n", encrypted);

    // ROT13 twice = original
    char double_rot[100];
    caesar_encrypt(encrypted, double_rot, 13);
    printf("ROT13 twice: %s\n", double_rot);

    // With mixed case and spaces
    caesar_encrypt("Hello World", encrypted, 5);
    printf("encrypt 'Hello World' shift=5: %s\n", encrypted);
    caesar_decrypt(encrypted, decrypted, 5);
    printf("decrypt back: %s\n", decrypted);

    // All shifts
    printf("All shifts of 'abc':\n");
    int s;
    for (s = 0; s < 26; s++) {
        caesar_encrypt("abc", encrypted, s);
        printf("  shift %d: %s\n", s, encrypted);
    }

    // Brute force crack - try all shifts
    caesar_encrypt("attack", encrypted, 7);
    printf("encrypted: %s\n", encrypted);
    printf("brute force:\n");
    for (s = 0; s < 26; s++) {
        caesar_decrypt(encrypted, decrypted, s);
        if (my_strcmp(decrypted, "attack") == 0) {
            printf("  shift %d: %s (FOUND!)\n", s, decrypted);
        }
    }

    return 0;
}
