int printf(const char *fmt, ...);
// EXPECT: Key: [3,1,4,2]\nEncrypt('HELLO WORLD') = 'E LLOXHORLWD'\nDecrypt: 'HELLO WORLDX'\n\nKey: [2,3,1]\nEncrypt('ATTACKATDAWN') = 'TKDNAAAATCTW'\nDecrypt: 'ATTACKATDAWN'\n\nKey: [4,1,5,2,3]\nEncrypt('THEQUICKBROWNFOX') = 'HCWXQBFXUROXTIOXEKNX'\nDecrypt: 'THEQUICKBROWNFOXXXXX'\n\nMessage lengths: 11, 12, 16
int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) len++;
    return len;
}

void columnar_encrypt(char *text, int *key, int key_len, char *output) {
    int len = str_len(text);
    int rows = (len + key_len - 1) / key_len;
    char grid[10][10];
    int r;
    int c;

    /* fill grid row by row */
    int idx = 0;
    for (r = 0; r < rows; r++) {
        for (c = 0; c < key_len; c++) {
            if (idx < len) {
                grid[r][c] = text[idx];
            } else {
                grid[r][c] = 'X';
            }
            idx++;
        }
    }

    /* read columns in key order */
    int oi = 0;
    int k;
    for (k = 1; k <= key_len; k++) {
        for (c = 0; c < key_len; c++) {
            if (key[c] == k) {
                for (r = 0; r < rows; r++) {
                    output[oi] = grid[r][c];
                    oi++;
                }
                break;
            }
        }
    }
    output[oi] = 0;
}

void columnar_decrypt(char *cipher, int *key, int key_len, char *output) {
    int len = str_len(cipher);
    int rows = (len + key_len - 1) / key_len;
    char grid[10][10];
    int r;
    int c;

    /* fill columns in key order */
    int ci = 0;
    int k;
    for (k = 1; k <= key_len; k++) {
        for (c = 0; c < key_len; c++) {
            if (key[c] == k) {
                for (r = 0; r < rows; r++) {
                    if (ci < len) {
                        grid[r][c] = cipher[ci];
                        ci++;
                    }
                }
                break;
            }
        }
    }

    /* read row by row */
    int oi = 0;
    for (r = 0; r < rows; r++) {
        for (c = 0; c < key_len; c++) {
            output[oi] = grid[r][c];
            oi++;
        }
    }
    output[oi] = 0;
}

int main(void) {
    char encrypted[80];
    char decrypted[80];

    int key1[4];
    key1[0] = 3; key1[1] = 1; key1[2] = 4; key1[3] = 2;

    char *msg1 = "HELLO WORLD";
    printf("Key: [%d,%d,%d,%d]\n", key1[0], key1[1], key1[2], key1[3]);
    columnar_encrypt(msg1, key1, 4, encrypted);
    printf("Encrypt('%s') = '%s'\n", msg1, encrypted);
    columnar_decrypt(encrypted, key1, 4, decrypted);
    printf("Decrypt: '%s'\n", decrypted);

    int key2[3];
    key2[0] = 2; key2[1] = 3; key2[2] = 1;

    char *msg2 = "ATTACKATDAWN";
    printf("\nKey: [%d,%d,%d]\n", key2[0], key2[1], key2[2]);
    columnar_encrypt(msg2, key2, 3, encrypted);
    printf("Encrypt('%s') = '%s'\n", msg2, encrypted);
    columnar_decrypt(encrypted, key2, 3, decrypted);
    printf("Decrypt: '%s'\n", decrypted);

    int key3[5];
    key3[0] = 4; key3[1] = 1; key3[2] = 5; key3[3] = 2; key3[4] = 3;

    char *msg3 = "THEQUICKBROWNFOX";
    printf("\nKey: [%d,%d,%d,%d,%d]\n", key3[0], key3[1], key3[2], key3[3], key3[4]);
    columnar_encrypt(msg3, key3, 5, encrypted);
    printf("Encrypt('%s') = '%s'\n", msg3, encrypted);
    columnar_decrypt(encrypted, key3, 5, decrypted);
    printf("Decrypt: '%s'\n", decrypted);

    printf("\nMessage lengths: %d, %d, %d\n",
           str_len(msg1), str_len(msg2), str_len(msg3));

    return 0;
}
