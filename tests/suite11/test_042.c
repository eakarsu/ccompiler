int printf(const char *fmt, ...);

// Substitution Cipher - encrypt/decrypt using a permuted alphabet

void make_substitution_key(char *key, int seed) {
    // Initialize with standard alphabet
    int i;
    for (i = 0; i < 26; i++) {
        key[i] = (char)(65 + i);
    }
    key[26] = 0;

    // Fisher-Yates shuffle with a simple PRNG
    int state = seed;
    for (i = 25; i > 0; i--) {
        state = (state * 1103515245 + 12345);
        int raw = state;
        if (raw < 0) raw = -raw;
        int j = raw % (i + 1);
        char tmp = key[i];
        key[i] = key[j];
        key[j] = tmp;
    }
}

void make_inverse_key(char *key, char *inv) {
    int i;
    for (i = 0; i < 26; i++) {
        int pos = key[i] - 65;
        inv[pos] = (char)(65 + i);
    }
    inv[26] = 0;
}

void subst_encrypt(char *plain, char *key, char *cipher) {
    int i = 0;
    while (plain[i] != 0) {
        char ch = plain[i];
        if (ch >= 65 && ch <= 90) {
            cipher[i] = key[ch - 65];
        } else if (ch >= 97 && ch <= 122) {
            char upper = key[ch - 97];
            cipher[i] = (char)(upper + 32);
        } else {
            cipher[i] = ch;
        }
        i = i + 1;
    }
    cipher[i] = 0;
}

void subst_decrypt(char *cipher, char *inv_key, char *plain) {
    int i = 0;
    while (cipher[i] != 0) {
        char ch = cipher[i];
        if (ch >= 65 && ch <= 90) {
            plain[i] = inv_key[ch - 65];
        } else if (ch >= 97 && ch <= 122) {
            char upper = inv_key[ch - 97];
            plain[i] = (char)(upper + 32);
        } else {
            plain[i] = ch;
        }
        i = i + 1;
    }
    plain[i] = 0;
}

int is_permutation(char *key) {
    int seen[26];
    int i;
    for (i = 0; i < 26; i++) seen[i] = 0;
    for (i = 0; i < 26; i++) {
        int idx = key[i] - 65;
        if (idx < 0 || idx >= 26) return 0;
        if (seen[idx]) return 0;
        seen[idx] = 1;
    }
    return 1;
}

int str_eq(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

int main() {
    char key[27];
    char inv_key[27];
    char cipher[128];
    char decrypted[128];

    // Generate substitution key with seed 42
    make_substitution_key(key, 42);
    printf("Key is permutation: %d\n", is_permutation(key));
    // EXPECT: Key is permutation: 1

    // Build inverse
    make_inverse_key(key, inv_key);
    printf("Inverse is permutation: %d\n", is_permutation(inv_key));
    // EXPECT: Inverse is permutation: 1

    // Verify inverse relationship: encrypt A..Z should give key, decrypt key should give A..Z
    char alpha[27];
    int i;
    for (i = 0; i < 26; i++) alpha[i] = (char)(65 + i);
    alpha[26] = 0;

    subst_encrypt(alpha, key, cipher);
    int match_key = str_eq(cipher, key);
    printf("Encrypt ALPHA matches key: %d\n", match_key);
    // EXPECT: Encrypt ALPHA matches key: 1

    subst_decrypt(cipher, inv_key, decrypted);
    int match_alpha = str_eq(decrypted, alpha);
    printf("Decrypt back to ALPHA: %d\n", match_alpha);
    // EXPECT: Decrypt back to ALPHA: 1

    // Encrypt a message
    char *msg = "HELLO WORLD";
    subst_encrypt(msg, key, cipher);
    subst_decrypt(cipher, inv_key, decrypted);
    int round_trip = str_eq(decrypted, msg);
    printf("Round trip HELLO WORLD: %d\n", round_trip);
    // EXPECT: Round trip HELLO WORLD: 1

    // Test lowercase preservation
    char *msg2 = "secret message";
    subst_encrypt(msg2, key, cipher);
    subst_decrypt(cipher, inv_key, decrypted);
    int rt2 = str_eq(decrypted, msg2);
    printf("Round trip lowercase: %d\n", rt2);
    // EXPECT: Round trip lowercase: 1

    // Different seed produces different key
    char key2[27];
    make_substitution_key(key2, 99);
    int same = str_eq(key, key2);
    printf("Different seeds differ: %d\n", 1 - same);
    // EXPECT: Different seeds differ: 1

    // Count fixed points (letters that map to themselves)
    int fixed = 0;
    for (i = 0; i < 26; i++) {
        if (key[i] == (char)(65 + i)) fixed = fixed + 1;
    }
    printf("Fixed points in key: %d\n", fixed);
    // EXPECT: Fixed points in key: 2

    printf("Substitution cipher tests passed\n");
    // EXPECT: Substitution cipher tests passed

    return 0;
}
