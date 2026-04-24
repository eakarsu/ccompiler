int printf(const char *fmt, ...);
// EXPECT: 2-rail encrypt('HELLO WORLD') = 'HLOWRDEL OL'\nDecrypt: 'HELLO WORLD'\nMatch: 1\n3-rail encrypt('WEAREDISCOVERED') = 'WECRERDSOEEAIVD'\nDecrypt: 'WEAREDISCOVERED'\nMatch: 1\n4-rail encrypt('ATTACKATDAWN') = 'AATKTNTCDWAA'\nDecrypt: 'ATTACKATDAWN'\nMatch: 1\n\nRail variations for 'WEAREDISCOVERED':\n  2 rails: WAEICVRDERDSOEE\n  3 rails: WECRERDSOEEAIVD\n  4 rails: WIREDSEEAECVDRO
int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) len++;
    return len;
}

void rail_fence_encrypt(char *text, int rails, char *output) {
    int len = str_len(text);
    int fence[4][80];
    int counts[4];
    int i;
    int r;

    for (r = 0; r < rails; r++) counts[r] = 0;

    int rail = 0;
    int dir = 1;
    for (i = 0; i < len; i++) {
        fence[rail][counts[rail]] = text[i];
        counts[rail]++;
        if (rail == 0) dir = 1;
        if (rail == rails - 1) dir = -1;
        rail = rail + dir;
    }

    int oi = 0;
    for (r = 0; r < rails; r++) {
        for (i = 0; i < counts[r]; i++) {
            output[oi] = fence[r][i];
            oi++;
        }
    }
    output[oi] = 0;
}

void rail_fence_decrypt(char *cipher, int rails, char *output) {
    int len = str_len(cipher);
    int pattern[80];
    int rail = 0;
    int dir = 1;
    int i;
    int r;

    for (i = 0; i < len; i++) {
        pattern[i] = rail;
        if (rail == 0) dir = 1;
        if (rail == rails - 1) dir = -1;
        rail = rail + dir;
    }

    int rail_sizes[4];
    for (r = 0; r < rails; r++) rail_sizes[r] = 0;
    for (i = 0; i < len; i++) rail_sizes[pattern[i]]++;

    int rail_start[4];
    rail_start[0] = 0;
    for (r = 1; r < rails; r++) {
        rail_start[r] = rail_start[r - 1] + rail_sizes[r - 1];
    }

    int rail_pos[4];
    for (r = 0; r < rails; r++) rail_pos[r] = rail_start[r];

    for (i = 0; i < len; i++) {
        r = pattern[i];
        output[i] = cipher[rail_pos[r]];
        rail_pos[r]++;
    }
    output[len] = 0;
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

    char *msg1 = "HELLO WORLD";
    rail_fence_encrypt(msg1, 2, encrypted);
    printf("2-rail encrypt('%s') = '%s'\n", msg1, encrypted);
    rail_fence_decrypt(encrypted, 2, decrypted);
    printf("Decrypt: '%s'\n", decrypted);
    printf("Match: %d\n", strings_equal(msg1, decrypted));

    char *msg2 = "WEAREDISCOVERED";
    rail_fence_encrypt(msg2, 3, encrypted);
    printf("3-rail encrypt('%s') = '%s'\n", msg2, encrypted);
    rail_fence_decrypt(encrypted, 3, decrypted);
    printf("Decrypt: '%s'\n", decrypted);
    printf("Match: %d\n", strings_equal(msg2, decrypted));

    char *msg3 = "ATTACKATDAWN";
    rail_fence_encrypt(msg3, 4, encrypted);
    printf("4-rail encrypt('%s') = '%s'\n", msg3, encrypted);
    rail_fence_decrypt(encrypted, 4, decrypted);
    printf("Decrypt: '%s'\n", decrypted);
    printf("Match: %d\n", strings_equal(msg3, decrypted));

    printf("\nRail variations for '%s':\n", msg2);
    int r;
    for (r = 2; r <= 4; r++) {
        rail_fence_encrypt(msg2, r, encrypted);
        printf("  %d rails: %s\n", r, encrypted);
    }

    return 0;
}
