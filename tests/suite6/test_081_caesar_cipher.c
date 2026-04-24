int printf(const char *fmt, ...);

void caesar_encrypt(char *out, char *in, int shift) {
    int i;
    for (i = 0; in[i] != 0; i = i + 1) {
        char c = in[i];
        if (c >= 'a' && c <= 'z') {
            out[i] = 'a' + (c - 'a' + shift) % 26;
        } else if (c >= 'A' && c <= 'Z') {
            out[i] = 'A' + (c - 'A' + shift) % 26;
        } else {
            out[i] = c;
        }
    }
    out[i] = 0;
}

void caesar_decrypt(char *out, char *in, int shift) {
    int i;
    for (i = 0; in[i] != 0; i = i + 1) {
        char c = in[i];
        if (c >= 'a' && c <= 'z') {
            out[i] = 'a' + (c - 'a' - shift + 26) % 26;
        } else if (c >= 'A' && c <= 'Z') {
            out[i] = 'A' + (c - 'A' - shift + 26) % 26;
        } else {
            out[i] = c;
        }
    }
    out[i] = 0;
}

int str_equal(char *a, char *b) {
    int i;
    for (i = 0; a[i] != 0 && b[i] != 0; i = i + 1) {
        if (a[i] != b[i]) return 0;
    }
    return a[i] == b[i];
}

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) n = n + 1;
    return n;
}

void freq_count(char *s, int *freq) {
    int i;
    for (i = 0; i < 26; i = i + 1) freq[i] = 0;
    for (i = 0; s[i] != 0; i = i + 1) {
        char c = s[i];
        if (c >= 'a' && c <= 'z') freq[c - 'a'] = freq[c - 'a'] + 1;
        if (c >= 'A' && c <= 'Z') freq[c - 'A'] = freq[c - 'A'] + 1;
    }
}

int brute_force_shift(char *cipher, char *expected) {
    char buf[128];
    int shift;
    for (shift = 0; shift < 26; shift = shift + 1) {
        caesar_decrypt(buf, cipher, shift);
        if (str_equal(buf, expected)) return shift;
    }
    return -1;
}

int main(void) {
    char plain[64];
    char cipher[64];
    char decoded[64];
    int freq[26];
    int i;

    plain[0]='h'; plain[1]='e'; plain[2]='l'; plain[3]='l'; plain[4]='o';
    plain[5]=0;

    caesar_encrypt(cipher, plain, 3);
    // EXPECT: Encrypted shift 3: khoor
    printf("Encrypted shift 3: %s\n", cipher);

    caesar_decrypt(decoded, cipher, 3);
    // EXPECT: Decrypted: hello
    printf("Decrypted: %s\n", decoded);

    // EXPECT: Roundtrip OK: 1
    printf("Roundtrip OK: %d\n", str_equal(plain, decoded));

    caesar_encrypt(cipher, plain, 13);
    // EXPECT: ROT13: uryyb
    printf("ROT13: %s\n", cipher);

    caesar_decrypt(decoded, cipher, 13);
    // EXPECT: ROT13 back: hello
    printf("ROT13 back: %s\n", decoded);

    caesar_encrypt(cipher, plain, 0);
    // EXPECT: Shift 0: hello
    printf("Shift 0: %s\n", cipher);

    caesar_encrypt(cipher, plain, 25);
    // EXPECT: Shift 25: gdkkn
    printf("Shift 25: %s\n", cipher);

    caesar_decrypt(decoded, cipher, 25);
    // EXPECT: Shift 25 back: hello
    printf("Shift 25 back: %s\n", decoded);

    char msg[16];
    msg[0]='A'; msg[1]='B'; msg[2]='C'; msg[3]=0;
    caesar_encrypt(cipher, msg, 1);
    // EXPECT: Upper shift 1: BCD
    printf("Upper shift 1: %s\n", cipher);

    caesar_decrypt(decoded, cipher, 1);
    // EXPECT: Upper back: ABC
    printf("Upper back: %s\n", decoded);

    char mixed[16];
    mixed[0]='H'; mixed[1]='i'; mixed[2]='!'; mixed[3]=0;
    caesar_encrypt(cipher, mixed, 5);
    // EXPECT: Mixed shift 5: Mn!
    printf("Mixed shift 5: %s\n", cipher);

    caesar_decrypt(decoded, cipher, 5);
    // EXPECT: Mixed back: Hi!
    printf("Mixed back: %s\n", decoded);

    freq_count(plain, freq);
    // EXPECT: Freq h: 1
    printf("Freq h: %d\n", freq['h' - 'a']);
    // EXPECT: Freq l: 2
    printf("Freq l: %d\n", freq['l' - 'a']);
    // EXPECT: Freq e: 1
    printf("Freq e: %d\n", freq['e' - 'a']);

    caesar_encrypt(cipher, plain, 7);
    int found = brute_force_shift(cipher, plain);
    // EXPECT: Brute force found shift: 7
    printf("Brute force found shift: %d\n", found);

    char long_msg[32];
    long_msg[0]='a'; long_msg[1]='t'; long_msg[2]='t'; long_msg[3]='a';
    long_msg[4]='c'; long_msg[5]='k'; long_msg[6]=0;
    caesar_encrypt(cipher, long_msg, 4);
    // EXPECT: attack encrypted: exxego
    printf("attack encrypted: %s\n", cipher);

    caesar_decrypt(decoded, cipher, 4);
    // EXPECT: attack roundtrip: 1
    printf("attack roundtrip: %d\n", str_equal(long_msg, decoded));

    // EXPECT: Len hello: 5
    printf("Len hello: %d\n", str_len(plain));

    return 0;
}
