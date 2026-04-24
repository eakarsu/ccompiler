int printf(const char *fmt, ...);
// EXPECT: ROT13('Hello World') = 'Uryyb Jbeyq'\nROT13 back: 'Hello World'\nRound-trip match: 1\nROT13('The Quick Brown Fox') = 'Gur Dhvpx Oebja Sbk'\nROT13 alphabet: nopqrstuvwxyzabcdefghijklm\n\nCaesar cipher variations:\n  ROT1: BUUBDL BU EBXO\n  Back:  ATTACK AT DAWN (match=1)\n  ROT3: DWWDFN DW GDZQ\n  Back:  ATTACK AT DAWN (match=1)\n  ROT5: FYYFHP FY IFBS\n  Back:  ATTACK AT DAWN (match=1)\n  ROT10: KDDKMU KD NKGX\n  Back:  ATTACK AT DAWN (match=1)\n  ROT25: ZSSZBJ ZS CZVM\n  Back:  ATTACK AT DAWN (match=1)\n\nFrequency analysis:\n  d: 1\n  e: 2\n  h: 2\n  l: 5\n  o: 3\n  r: 1\n  w: 1
int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) len++;
    return len;
}

char rot13_char(char c) {
    if (c >= 'a' && c <= 'z') {
        return 'a' + (c - 'a' + 13) % 26;
    }
    if (c >= 'A' && c <= 'Z') {
        return 'A' + (c - 'A' + 13) % 26;
    }
    return c;
}

void rot13(char *input, char *output) {
    int i = 0;
    while (input[i] != 0) {
        output[i] = rot13_char(input[i]);
        i++;
    }
    output[i] = 0;
}

char rot_n_char(char c, int n) {
    if (c >= 'a' && c <= 'z') {
        return 'a' + (c - 'a' + n) % 26;
    }
    if (c >= 'A' && c <= 'Z') {
        return 'A' + (c - 'A' + n) % 26;
    }
    return c;
}

void rot_n(char *input, char *output, int n) {
    int i = 0;
    while (input[i] != 0) {
        output[i] = rot_n_char(input[i], n);
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
    char buf1[100];
    char buf2[100];

    char *test1 = "Hello World";
    rot13(test1, buf1);
    printf("ROT13('%s') = '%s'\n", test1, buf1);
    rot13(buf1, buf2);
    printf("ROT13 back: '%s'\n", buf2);
    printf("Round-trip match: %d\n", strings_equal(test1, buf2));

    char *test2 = "The Quick Brown Fox";
    rot13(test2, buf1);
    printf("ROT13('%s') = '%s'\n", test2, buf1);

    char *test3 = "abcdefghijklmnopqrstuvwxyz";
    rot13(test3, buf1);
    printf("ROT13 alphabet: %s\n", buf1);

    printf("\nCaesar cipher variations:\n");
    char *msg = "ATTACK AT DAWN";
    int shifts[5];
    shifts[0] = 1; shifts[1] = 3; shifts[2] = 5; shifts[3] = 10; shifts[4] = 25;
    int i;
    for (i = 0; i < 5; i++) {
        rot_n(msg, buf1, shifts[i]);
        printf("  ROT%d: %s\n", shifts[i], buf1);
        rot_n(buf1, buf2, 26 - shifts[i]);
        printf("  Back:  %s (match=%d)\n", buf2, strings_equal(msg, buf2));
    }

    printf("\nFrequency analysis:\n");
    char *text = "Hello World Hello";
    int freq[26];
    for (i = 0; i < 26; i++) freq[i] = 0;
    for (i = 0; text[i] != 0; i++) {
        char c = text[i];
        if (c >= 'a' && c <= 'z') freq[c - 'a']++;
        if (c >= 'A' && c <= 'Z') freq[c - 'A']++;
    }
    for (i = 0; i < 26; i++) {
        if (freq[i] > 0) {
            printf("  %c: %d\n", 'a' + i, freq[i]);
        }
    }

    return 0;
}
