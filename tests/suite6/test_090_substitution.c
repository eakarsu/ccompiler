int printf(const char *fmt, ...);

void make_identity_key(int *key) {
    int i;
    for (i = 0; i < 26; i = i + 1) {
        key[i] = i;
    }
}

void make_reverse_key(int *key) {
    int i;
    for (i = 0; i < 26; i = i + 1) {
        key[i] = 25 - i;
    }
}

void invert_key(int *key, int *inv) {
    int i;
    for (i = 0; i < 26; i = i + 1) {
        inv[key[i]] = i;
    }
}

void sub_encrypt(char *out, char *in, int *key) {
    int i;
    for (i = 0; in[i] != 0; i = i + 1) {
        char c = in[i];
        if (c >= 'a' && c <= 'z') {
            out[i] = 'a' + key[c - 'a'];
        } else if (c >= 'A' && c <= 'Z') {
            out[i] = 'A' + key[c - 'A'];
        } else {
            out[i] = c;
        }
    }
    out[i] = 0;
}

void sub_decrypt(char *out, char *in, int *key) {
    int inv[26];
    invert_key(key, inv);
    sub_encrypt(out, in, inv);
}

int str_equal(char *a, char *b) {
    int i;
    for (i = 0; a[i] != 0 && b[i] != 0; i = i + 1) {
        if (a[i] != b[i]) return 0;
    }
    return a[i] == b[i];
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

int freq_preserved(char *plain, char *cipher, int *key) {
    int pf[26];
    int cf[26];
    freq_count(plain, pf);
    freq_count(cipher, cf);
    int i;
    for (i = 0; i < 26; i = i + 1) {
        if (pf[i] > 0) {
            if (cf[key[i]] != pf[i]) return 0;
        }
    }
    return 1;
}

int is_valid_key(int *key) {
    int seen[26];
    int i;
    for (i = 0; i < 26; i = i + 1) seen[i] = 0;
    for (i = 0; i < 26; i = i + 1) {
        if (key[i] < 0 || key[i] > 25) return 0;
        if (seen[key[i]]) return 0;
        seen[key[i]] = 1;
    }
    return 1;
}

int main(void) {
    int key[26];
    int inv[26];
    char plain[64];
    char cipher[64];
    char decoded[64];
    int i;

    make_identity_key(key);
    // EXPECT: Identity key valid: 1
    printf("Identity key valid: %d\n", is_valid_key(key));

    plain[0]='h'; plain[1]='e'; plain[2]='l'; plain[3]='l'; plain[4]='o';
    plain[5]=0;

    sub_encrypt(cipher, plain, key);
    // EXPECT: Identity encrypt: hello
    printf("Identity encrypt: %s\n", cipher);

    make_reverse_key(key);
    // EXPECT: Reverse key valid: 1
    printf("Reverse key valid: %d\n", is_valid_key(key));

    // EXPECT: Rev key[0]: 25
    printf("Rev key[0]: %d\n", key[0]);
    // EXPECT: Rev key[25]: 0
    printf("Rev key[25]: %d\n", key[25]);

    sub_encrypt(cipher, plain, key);
    // EXPECT: Reverse encrypt: svool
    printf("Reverse encrypt: %s\n", cipher);

    sub_decrypt(decoded, cipher, key);
    // EXPECT: Reverse decrypt: hello
    printf("Reverse decrypt: %s\n", decoded);

    // EXPECT: Reverse roundtrip: 1
    printf("Reverse roundtrip: %d\n", str_equal(plain, decoded));

    invert_key(key, inv);
    int self_inv = 1;
    for (i = 0; i < 26; i = i + 1) {
        if (key[i] != inv[i]) self_inv = 0;
    }
    // EXPECT: Reverse is self-inverse: 1
    printf("Reverse is self-inverse: %d\n", self_inv);

    // EXPECT: Freq preserved: 1
    printf("Freq preserved: %d\n", freq_preserved(plain, cipher, key));

    int shift_key[26];
    for (i = 0; i < 26; i = i + 1) {
        shift_key[i] = (i + 3) % 26;
    }
    // EXPECT: Shift key valid: 1
    printf("Shift key valid: %d\n", is_valid_key(shift_key));

    sub_encrypt(cipher, plain, shift_key);
    // EXPECT: Shift3 encrypt: khoor
    printf("Shift3 encrypt: %s\n", cipher);

    sub_decrypt(decoded, cipher, shift_key);
    // EXPECT: Shift3 roundtrip: 1
    printf("Shift3 roundtrip: %d\n", str_equal(plain, decoded));

    char msg[16];
    msg[0]='A'; msg[1]='B'; msg[2]='C'; msg[3]='!'; msg[4]=0;
    make_reverse_key(key);
    sub_encrypt(cipher, msg, key);
    // EXPECT: Upper+punct: ZYX!
    printf("Upper+punct: %s\n", cipher);

    sub_decrypt(decoded, cipher, key);
    // EXPECT: Upper+punct back: ABC!
    printf("Upper+punct back: %s\n", decoded);

    int bad_key[26];
    for (i = 0; i < 26; i = i + 1) bad_key[i] = 0;
    // EXPECT: Bad key valid: 0
    printf("Bad key valid: %d\n", is_valid_key(bad_key));

    int freq_plain[26];
    int freq_cipher[26];
    char longer[32];
    longer[0]='a'; longer[1]='b'; longer[2]='r'; longer[3]='a';
    longer[4]='c'; longer[5]='a'; longer[6]='d'; longer[7]='a';
    longer[8]='b'; longer[9]='r'; longer[10]='a'; longer[11]=0;
    freq_count(longer, freq_plain);
    // EXPECT: Freq a in abracadabra: 5
    printf("Freq a in abracadabra: %d\n", freq_plain[0]);
    // EXPECT: Freq b in abracadabra: 2
    printf("Freq b in abracadabra: %d\n", freq_plain[1]);

    return 0;
}
