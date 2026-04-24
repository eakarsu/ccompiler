int printf(const char *fmt, ...);
// EXPECT: OTP encrypt('HELLO WORLD') = 'OSWWC SCIWG'\nOTP decrypt: 'HELLO WORLD'\nMatch: 1\n\nGenerated key: 1 12 11 6 1 2 9 10 17 2 9 2 3 2 \nOTP encrypt('ATTACK AT DAWN') = 'BFEGDM KK MCZP'\nOTP decrypt: 'ATTACK AT DAWN'\nMatch: 1\n\nSame message, different keys:\n  seed=1: 'SECRET' -> 'OJCQCS'\n           back: 'SECRET' match=1\n  seed=101: 'SECRET' -> 'IBGCUQ'\n           back: 'SECRET' match=1\n  seed=201: 'SECRET' -> 'ETMOKQ'\n           back: 'SECRET' match=1\n  seed=301: 'SECRET' -> 'YLQAAO'\n           back: 'SECRET' match=1
int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) len++;
    return len;
}

int is_upper(char c) { return c >= 'A' && c <= 'Z'; }

void otp_encrypt(char *plaintext, int *key, char *output) {
    int i = 0;
    while (plaintext[i] != 0) {
        if (is_upper(plaintext[i])) {
            output[i] = 'A' + (plaintext[i] - 'A' + key[i]) % 26;
        } else {
            output[i] = plaintext[i];
        }
        i++;
    }
    output[i] = 0;
}

void otp_decrypt(char *ciphertext, int *key, char *output) {
    int i = 0;
    while (ciphertext[i] != 0) {
        if (is_upper(ciphertext[i])) {
            output[i] = 'A' + (ciphertext[i] - 'A' - key[i] + 26) % 26;
        } else {
            output[i] = ciphertext[i];
        }
        i++;
    }
    output[i] = 0;
}

/* simple PRNG for key generation */
int prng_state = 12345;
int prng_next(void) {
    prng_state = (prng_state * 1103515245 + 12345) & 0x7fffffff;
    return prng_state;
}

void generate_key(int *key, int len) {
    int i;
    for (i = 0; i < len; i++) {
        int val = prng_next();
        if (val < 0) val = -val;
        key[i] = val % 26;
    }
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
    char encrypted[80];
    char decrypted[80];

    /* Manual key test */
    int key1[20];
    key1[0] = 7; key1[1] = 14; key1[2] = 11; key1[3] = 11; key1[4] = 14;
    key1[5] = 0; key1[6] = 22; key1[7] = 14; key1[8] = 17; key1[9] = 11;
    key1[10] = 3;

    char *msg1 = "HELLO WORLD";
    otp_encrypt(msg1, key1, encrypted);
    printf("OTP encrypt('%s') = '%s'\n", msg1, encrypted);
    otp_decrypt(encrypted, key1, decrypted);
    printf("OTP decrypt: '%s'\n", decrypted);
    printf("Match: %d\n", strings_equal(msg1, decrypted));

    /* Generated key test */
    char *msg2 = "ATTACK AT DAWN";
    int len2 = str_len(msg2);
    int key2[20];
    prng_state = 42;
    generate_key(key2, len2);

    printf("\nGenerated key: ");
    int i;
    for (i = 0; i < len2; i++) printf("%d ", key2[i]);
    printf("\n");

    otp_encrypt(msg2, key2, encrypted);
    printf("OTP encrypt('%s') = '%s'\n", msg2, encrypted);
    otp_decrypt(encrypted, key2, decrypted);
    printf("OTP decrypt: '%s'\n", decrypted);
    printf("Match: %d\n", strings_equal(msg2, decrypted));

    /* Same message, different keys */
    char *msg3 = "SECRET";
    printf("\nSame message, different keys:\n");
    int k;
    for (k = 0; k < 4; k++) {
        prng_state = k * 100 + 1;
        int key3[10];
        generate_key(key3, str_len(msg3));
        otp_encrypt(msg3, key3, encrypted);
        printf("  seed=%d: '%s' -> '%s'\n", k * 100 + 1, msg3, encrypted);
        otp_decrypt(encrypted, key3, decrypted);
        printf("           back: '%s' match=%d\n", decrypted, strings_equal(msg3, decrypted));
    }

    return 0;
}
