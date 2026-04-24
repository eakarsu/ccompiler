int printf(const char *fmt, ...);

// Vigenere Cipher - encrypt and decrypt using a repeating key

void vigenere_encrypt(char *plaintext, char *key, char *ciphertext) {
    int klen = 0;
    while (key[klen] != 0) {
        klen = klen + 1;
    }
    int i = 0;
    int ki = 0;
    while (plaintext[i] != 0) {
        char ch = plaintext[i];
        if (ch >= 65 && ch <= 90) {
            int shift = key[ki % klen] - 65;
            int val = ((ch - 65) + shift) % 26;
            ciphertext[i] = (char)(val + 65);
            ki = ki + 1;
        } else if (ch >= 97 && ch <= 122) {
            int shift = key[ki % klen] - 65;
            int val = ((ch - 97) + shift) % 26;
            ciphertext[i] = (char)(val + 97);
            ki = ki + 1;
        } else {
            ciphertext[i] = ch;
        }
        i = i + 1;
    }
    ciphertext[i] = 0;
}

void vigenere_decrypt(char *ciphertext, char *key, char *plaintext) {
    int klen = 0;
    while (key[klen] != 0) {
        klen = klen + 1;
    }
    int i = 0;
    int ki = 0;
    while (ciphertext[i] != 0) {
        char ch = ciphertext[i];
        if (ch >= 65 && ch <= 90) {
            int shift = key[ki % klen] - 65;
            int val = ((ch - 65) - shift + 26) % 26;
            plaintext[i] = (char)(val + 65);
            ki = ki + 1;
        } else if (ch >= 97 && ch <= 122) {
            int shift = key[ki % klen] - 65;
            int val = ((ch - 97) - shift + 26) % 26;
            plaintext[i] = (char)(val + 97);
            ki = ki + 1;
        } else {
            plaintext[i] = ch;
        }
        i = i + 1;
    }
    plaintext[i] = 0;
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
    int n = 0;
    while (s[n] != 0) n = n + 1;
    return n;
}

int main() {
    char cipher1[64];
    char decrypted1[64];

    // Test 1: simple encryption
    char *msg1 = "HELLO";
    char *key1 = "KEY";
    vigenere_encrypt(msg1, key1, cipher1);
    printf("%s\n", cipher1);
    // EXPECT: RIJVS

    // Test 2: decrypt back
    vigenere_decrypt(cipher1, key1, decrypted1);
    printf("%s\n", decrypted1);
    // EXPECT: HELLO

    // Test 3: lowercase
    char *msg2 = "attack";
    char *key2 = "KEY";
    vigenere_encrypt(msg2, key2, cipher1);
    printf("%s\n", cipher1);
    // EXPECT: kxrkgi

    vigenere_decrypt(cipher1, key2, decrypted1);
    printf("%s\n", decrypted1);
    // EXPECT: attack

    // Test 4: mixed case with spaces
    char *msg3 = "HELLO WORLD";
    char *key3 = "ABC";
    vigenere_encrypt(msg3, key3, cipher1);
    printf("%s\n", cipher1);
    // EXPECT: HFNLP YOSND

    vigenere_decrypt(cipher1, key3, decrypted1);
    printf("%s\n", decrypted1);
    // EXPECT: HELLO WORLD

    // Test 5: verify round-trip integrity
    int ok = str_equal(decrypted1, msg3);
    printf("Round trip OK: %d\n", ok);
    // EXPECT: Round trip OK: 1

    // Test 6: single char key (Caesar cipher equivalent)
    char *msg4 = "ABC";
    char *key4 = "D";
    vigenere_encrypt(msg4, key4, cipher1);
    printf("%s\n", cipher1);
    // EXPECT: DEF

    // Test 7: key same length as message
    char *msg5 = "CAT";
    char *key5 = "DOG";
    vigenere_encrypt(msg5, key5, cipher1);
    printf("%s\n", cipher1);
    // EXPECT: FOZ

    vigenere_decrypt(cipher1, key5, decrypted1);
    printf("%s\n", decrypted1);
    // EXPECT: CAT

    // Test 8: longer message
    char *msg6 = "THEQUICKBROWNFOX";
    char *key6 = "SECRET";
    vigenere_encrypt(msg6, key6, cipher1);
    printf("Encrypted len: %d\n", str_len(cipher1));
    // EXPECT: Encrypted len: 16

    vigenere_decrypt(cipher1, key6, decrypted1);
    printf("%s\n", decrypted1);
    // EXPECT: THEQUICKBROWNFOX

    printf("Vigenere tests passed\n");
    // EXPECT: Vigenere tests passed

    return 0;
}
