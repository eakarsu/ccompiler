int printf(const char *fmt, ...);
// EXPECT: Encrypt('HELLO WORLD', key='KEY') = 'RIJVS UYVJN'\nDecrypt: 'HELLO WORLD'\nMatch: 1\nEncrypt('ATTACK AT DAWN', key='LEMON') = 'LXFOPV EF RNHR'\nDecrypt: 'ATTACK AT DAWN'\nMatch: 1\nEncrypt('the quick brown fox', key='SECRET') = 'llg hybuo dispf jqo'\nDecrypt: 'the quick brown fox'\nMatch: 1\n\nKey analysis:\n  Key 'KEY': length=3\n  Key 'LEMON': length=5\n  Key 'SECRET': length=6
int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) len++;
    return len;
}

char to_upper(char c) {
    if (c >= 'a' && c <= 'z') return c - 32;
    return c;
}

char to_lower(char c) {
    if (c >= 'A' && c <= 'Z') return c + 32;
    return c;
}

int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void vigenere_encrypt(char *plaintext, char *key, char *output) {
    int key_len = str_len(key);
    int ki = 0;
    int i = 0;
    while (plaintext[i] != 0) {
        if (is_alpha(plaintext[i])) {
            char base = (plaintext[i] >= 'a' && plaintext[i] <= 'z') ? 'a' : 'A';
            int shift = to_upper(key[ki % key_len]) - 'A';
            output[i] = base + (plaintext[i] - base + shift) % 26;
            ki++;
        } else {
            output[i] = plaintext[i];
        }
        i++;
    }
    output[i] = 0;
}

void vigenere_decrypt(char *ciphertext, char *key, char *output) {
    int key_len = str_len(key);
    int ki = 0;
    int i = 0;
    while (ciphertext[i] != 0) {
        if (is_alpha(ciphertext[i])) {
            char base = (ciphertext[i] >= 'a' && ciphertext[i] <= 'z') ? 'a' : 'A';
            int shift = to_upper(key[ki % key_len]) - 'A';
            output[i] = base + (ciphertext[i] - base - shift + 26) % 26;
            ki++;
        } else {
            output[i] = ciphertext[i];
        }
        i++;
    }
    output[i] = 0;
}

int strings_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return a[i] == b[i];
}

int main(void) {
    char encrypted[100];
    char decrypted[100];

    char *msg1 = "HELLO WORLD";
    char *key1 = "KEY";
    vigenere_encrypt(msg1, key1, encrypted);
    printf("Encrypt('%s', key='%s') = '%s'\n", msg1, key1, encrypted);
    vigenere_decrypt(encrypted, key1, decrypted);
    printf("Decrypt: '%s'\n", decrypted);
    printf("Match: %d\n", strings_equal(msg1, decrypted));

    char *msg2 = "ATTACK AT DAWN";
    char *key2 = "LEMON";
    vigenere_encrypt(msg2, key2, encrypted);
    printf("Encrypt('%s', key='%s') = '%s'\n", msg2, key2, encrypted);
    vigenere_decrypt(encrypted, key2, decrypted);
    printf("Decrypt: '%s'\n", decrypted);
    printf("Match: %d\n", strings_equal(msg2, decrypted));

    char *msg3 = "the quick brown fox";
    char *key3 = "SECRET";
    vigenere_encrypt(msg3, key3, encrypted);
    printf("Encrypt('%s', key='%s') = '%s'\n", msg3, key3, encrypted);
    vigenere_decrypt(encrypted, key3, decrypted);
    printf("Decrypt: '%s'\n", decrypted);
    printf("Match: %d\n", strings_equal(msg3, decrypted));

    printf("\nKey analysis:\n");
    char *keys[3];
    keys[0] = "KEY"; keys[1] = "LEMON"; keys[2] = "SECRET";
    int i;
    for (i = 0; i < 3; i++) {
        printf("  Key '%s': length=%d\n", keys[i], str_len(keys[i]));
    }

    return 0;
}
