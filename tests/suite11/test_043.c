int printf(const char *fmt, ...);

// Transposition Cipher - columnar transposition encrypt/decrypt

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) n = n + 1;
    return n;
}

void str_copy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int str_eq(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

// Sort column order based on key characters, return permutation
void get_column_order(char *key, int *order) {
    int klen = str_len(key);
    int i;
    int j;
    // Initialize
    for (i = 0; i < klen; i++) {
        order[i] = i;
    }
    // Insertion sort by key character
    for (i = 1; i < klen; i++) {
        int val = order[i];
        char ch = key[val];
        j = i - 1;
        while (j >= 0 && key[order[j]] > ch) {
            order[j + 1] = order[j];
            j = j - 1;
        }
        order[j + 1] = val;
    }
}

void transpose_encrypt(char *plain, char *key, char *cipher) {
    int plen = str_len(plain);
    int klen = str_len(key);
    int rows = (plen + klen - 1) / klen;
    int order[16];
    get_column_order(key, order);

    // Pad plaintext into grid
    char grid[128];
    int i;
    for (i = 0; i < rows * klen; i++) {
        if (i < plen) {
            grid[i] = plain[i];
        } else {
            grid[i] = 'X'; // padding
        }
    }

    // Read columns in order
    int ci = 0;
    int col;
    int row;
    for (i = 0; i < klen; i++) {
        col = order[i];
        for (row = 0; row < rows; row++) {
            cipher[ci] = grid[row * klen + col];
            ci = ci + 1;
        }
    }
    cipher[ci] = 0;
}

void transpose_decrypt(char *cipher, char *key, char *plain) {
    int clen = str_len(cipher);
    int klen = str_len(key);
    int rows = clen / klen;
    int order[16];
    get_column_order(key, order);

    // Fill columns in order
    char grid[128];
    int ci = 0;
    int i;
    int col;
    int row;
    for (i = 0; i < klen; i++) {
        col = order[i];
        for (row = 0; row < rows; row++) {
            grid[row * klen + col] = cipher[ci];
            ci = ci + 1;
        }
    }

    // Read row by row
    for (i = 0; i < rows * klen; i++) {
        plain[i] = grid[i];
    }
    plain[rows * klen] = 0;
}

// Rail fence cipher (zigzag)
void rail_fence_encrypt(char *plain, int rails, char *cipher) {
    int plen = str_len(plain);
    int rail_row[256];
    int i;
    // Determine which rail each character goes to
    int row = 0;
    int dir = 1;
    for (i = 0; i < plen; i++) {
        rail_row[i] = row;
        if (row == 0) dir = 1;
        if (row == rails - 1) dir = -1;
        row = row + dir;
    }
    // Read off by rail
    int ci = 0;
    int r;
    for (r = 0; r < rails; r++) {
        for (i = 0; i < plen; i++) {
            if (rail_row[i] == r) {
                cipher[ci] = plain[i];
                ci = ci + 1;
            }
        }
    }
    cipher[ci] = 0;
}

int main() {
    char cipher[128];
    char decrypted[128];

    // Test 1: basic columnar transposition
    char *msg1 = "HELLOWORLD";
    char *key1 = "CAB";
    transpose_encrypt(msg1, key1, cipher);
    printf("Encrypted: %s\n", cipher);
    // EXPECT: Encrypted: EORXLWLXHLOD

    // Test 2: decrypt
    transpose_decrypt(cipher, key1, decrypted);
    // Remove padding X's for comparison - just check first 10 chars
    int i;
    int match = 1;
    for (i = 0; i < 10; i++) {
        if (decrypted[i] != msg1[i]) match = 0;
    }
    printf("Decrypt matches: %d\n", match);
    // EXPECT: Decrypt matches: 1

    // Test 3: message exactly fits grid
    char *msg2 = "ABCDEF";
    char *key2 = "BA";
    transpose_encrypt(msg2, key2, cipher);
    printf("Exact fit: %s\n", cipher);
    // EXPECT: Exact fit: BDFACE

    transpose_decrypt(cipher, key2, decrypted);
    int eq = str_eq(decrypted, msg2);
    printf("Exact decrypt: %d\n", eq);
    // EXPECT: Exact decrypt: 1

    // Test 4: single column key
    char *msg3 = "TEST";
    char *key3 = "A";
    transpose_encrypt(msg3, key3, cipher);
    printf("Single col: %s\n", cipher);
    // EXPECT: Single col: TEST

    // Test 5: rail fence cipher with 3 rails
    char *msg4 = "WEAREDISCOVERED";
    rail_fence_encrypt(msg4, 3, cipher);
    printf("Rail fence 3: %s\n", cipher);
    // EXPECT: Rail fence 3: WECRERDSOEEAIVD

    // Test 6: rail fence with 2 rails
    char *msg5 = "ABCDEFGH";
    rail_fence_encrypt(msg5, 2, cipher);
    printf("Rail fence 2: %s\n", cipher);
    // EXPECT: Rail fence 2: ACEGBDFH

    // Test 7: length preservation
    int cipher_len = str_len(cipher);
    printf("Length preserved: %d\n", cipher_len == str_len(msg5));
    // EXPECT: Length preserved: 1

    // Test 8: column order verification
    int order[16];
    char *key4 = "ZEBRA";
    get_column_order(key4, order);
    printf("Col order: %d %d %d %d %d\n", order[0], order[1], order[2], order[3], order[4]);
    // EXPECT: Col order: 4 2 1 3 0

    printf("Transposition tests passed\n");
    // EXPECT: Transposition tests passed

    return 0;
}
