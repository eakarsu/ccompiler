// EXPECT: === Caesar Cipher ===\nPlain:  HELLOWORLD\nKey: 3\nCipher: KHOORZRUOG\nDecrypt: HELLOWORLD\nCaesar roundtrip OK\n=== Vigenere Cipher ===\nPlain:  ATTACKATDAWN\nKey:    LEMONLEMONLE\nCipher: LXFOPVEFRNHR\nDecrypt: ATTACKATDAWN\nVigenere roundtrip OK\n=== Playfair Preparation ===\nKey: MONARCHY\nPlayfair grid:\nM O N A R\nC H Y B D\nE F G I K\nL P Q S T\nU V W X Z\n=== Hill Cipher 2x2 ===\nKey matrix: [6,24;1,16]\nPlain: AC -> values 0 2\nCipher values: 22 6 -> WG\nDecrypt values: 0 4 -> AE\nHill roundtrip FAIL\n=== One-Time Pad (XOR) ===\nPlain:  72 69 76 76 79\nKey:    41 55 33 28 90\nCipher: 97 114 109 80 21\nDecrypt: 72 69 76 76 79\nOTP roundtrip OK\n=== Transposition Cipher ===\nPlain:  HELLOWORLD\nCols: 3\nTransposed: HLODEORLWL\nUntransposed: HELLOWORLD\nTransposition roundtrip OK\n=== Frequency Analysis ===\nInput: AABABCABCDABCDE\nFrequencies:\nA: 5\nB: 4\nC: 3\nD: 2\nE: 1\nMost frequent: A (5)\n=== Substitution Cipher ===\nPlain:  HELLO\nCipher: IFMMP\nDecrypt: HELLO\nSubstitution roundtrip OK\n=== Affine Cipher ===\nPlain:  HELLOWORLD\nA=5, B=8\nCipher: RCLLAOAPLX\nDecrypt: HELLOWORLD\nAffine roundtrip OK\nDone.\n
int printf(const char *fmt, ...);

// ---- Caesar Cipher ----

void caesar_encrypt(char *plain, char *cipher, int key, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (plain[i] >= 'A' && plain[i] <= 'Z') {
            cipher[i] = 'A' + (plain[i] - 'A' + key) % 26;
        } else if (plain[i] >= 'a' && plain[i] <= 'z') {
            cipher[i] = 'a' + (plain[i] - 'a' + key) % 26;
        } else {
            cipher[i] = plain[i];
        }
    }
    cipher[len] = 0;
}

void caesar_decrypt(char *cipher, char *plain, int key, int len) {
    int i;
    int shift;
    for (i = 0; i < len; i = i + 1) {
        if (cipher[i] >= 'A' && cipher[i] <= 'Z') {
            shift = (cipher[i] - 'A' - key + 26) % 26;
            plain[i] = 'A' + shift;
        } else if (cipher[i] >= 'a' && cipher[i] <= 'z') {
            shift = (cipher[i] - 'a' - key + 26) % 26;
            plain[i] = 'a' + shift;
        } else {
            plain[i] = cipher[i];
        }
    }
    plain[len] = 0;
}

// ---- Vigenere Cipher ----

void vigenere_encrypt(char *plain, char *key, char *cipher, int plen, int klen) {
    int i;
    int ki;
    int p;
    int k;
    ki = 0;
    for (i = 0; i < plen; i = i + 1) {
        if (plain[i] >= 'A' && plain[i] <= 'Z') {
            p = plain[i] - 'A';
            k = key[ki % klen] - 'A';
            cipher[i] = 'A' + (p + k) % 26;
            ki = ki + 1;
        } else {
            cipher[i] = plain[i];
        }
    }
    cipher[plen] = 0;
}

void vigenere_decrypt(char *cipher, char *key, char *plain, int clen, int klen) {
    int i;
    int ki;
    int c;
    int k;
    ki = 0;
    for (i = 0; i < clen; i = i + 1) {
        if (cipher[i] >= 'A' && cipher[i] <= 'Z') {
            c = cipher[i] - 'A';
            k = key[ki % klen] - 'A';
            plain[i] = 'A' + (c - k + 26) % 26;
            ki = ki + 1;
        } else {
            plain[i] = cipher[i];
        }
    }
    plain[clen] = 0;
}

// ---- Playfair Preparation ----

int playfair_grid[5][5];
int playfair_pos_row[26];
int playfair_pos_col[26];

void playfair_prepare(char *key, int klen) {
    int used[26];
    int i;
    int j;
    int idx;
    int ch;
    int row;
    int col;

    for (i = 0; i < 26; i = i + 1) {
        used[i] = 0;
    }
    // J = I in Playfair
    used['J' - 'A'] = 1;

    idx = 0;
    // Place key chars
    for (i = 0; i < klen; i = i + 1) {
        ch = key[i] - 'A';
        if (ch == 'J' - 'A') {
            ch = 'I' - 'A';
        }
        if (!used[ch]) {
            row = idx / 5;
            col = idx % 5;
            playfair_grid[row][col] = ch;
            playfair_pos_row[ch] = row;
            playfair_pos_col[ch] = col;
            used[ch] = 1;
            idx = idx + 1;
        }
    }
    // Fill rest
    for (i = 0; i < 26; i = i + 1) {
        if (!used[i]) {
            row = idx / 5;
            col = idx % 5;
            playfair_grid[row][col] = i;
            playfair_pos_row[i] = row;
            playfair_pos_col[i] = col;
            idx = idx + 1;
        }
    }
}

void print_playfair_grid() {
    int i;
    int j;
    for (i = 0; i < 5; i = i + 1) {
        for (j = 0; j < 5; j = j + 1) {
            if (j > 0) {
                printf(" ");
            }
            printf("%c", 'A' + playfair_grid[i][j]);
        }
        printf("\n");
    }
}

// ---- Hill Cipher 2x2 ----

// Key matrix [[a,b],[c,d]], encrypt pairs of letters
// Cipher = Key * Plain (mod 26)

struct HillKey {
    int a;
    int b;
    int c;
    int d;
};

void hill_encrypt_pair(struct HillKey key, int p1, int p2, int *c1, int *c2) {
    *c1 = (key.a * p1 + key.b * p2) % 26;
    *c2 = (key.c * p1 + key.d * p2) % 26;
}

// Modular inverse mod 26
int mod_inverse(int a, int m) {
    int t;
    int new_t;
    int r;
    int new_r;
    int q;
    int tmp;
    t = 0;
    new_t = 1;
    r = m;
    new_r = a % m;
    if (new_r < 0) {
        new_r = new_r + m;
    }
    while (new_r != 0) {
        q = r / new_r;
        tmp = new_t;
        new_t = t - q * new_t;
        t = tmp;
        tmp = new_r;
        new_r = r - q * new_r;
        r = tmp;
    }
    if (t < 0) {
        t = t + m;
    }
    return t;
}

void hill_decrypt_pair(struct HillKey key, int c1, int c2, int *p1, int *p2) {
    int det;
    int det_inv;
    int inv_a;
    int inv_b;
    int inv_c;
    int inv_d;

    det = (key.a * key.d - key.b * key.c) % 26;
    if (det < 0) {
        det = det + 26;
    }
    det_inv = mod_inverse(det, 26);

    inv_a = (key.d * det_inv) % 26;
    inv_b = ((-key.b + 26) * det_inv) % 26;
    inv_c = ((-key.c + 26) * det_inv) % 26;
    inv_d = (key.a * det_inv) % 26;

    *p1 = (inv_a * c1 + inv_b * c2) % 26;
    *p2 = (inv_c * c1 + inv_d * c2) % 26;
    if (*p1 < 0) *p1 = *p1 + 26;
    if (*p2 < 0) *p2 = *p2 + 26;
}

// ---- One-Time Pad (XOR) ----

void otp_encrypt(int *plain, int *key, int *cipher, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        cipher[i] = plain[i] ^ key[i];
    }
}

void otp_decrypt(int *cipher, int *key, int *plain, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        plain[i] = cipher[i] ^ key[i];
    }
}

// ---- Transposition Cipher ----

void transpose_encrypt(char *plain, char *cipher, int len, int cols) {
    int rows;
    int i;
    int r;
    int c;
    int idx;

    rows = (len + cols - 1) / cols;
    idx = 0;
    for (c = 0; c < cols; c = c + 1) {
        for (r = 0; r < rows; r = r + 1) {
            i = r * cols + c;
            if (i < len) {
                cipher[idx] = plain[i];
                idx = idx + 1;
            }
        }
    }
    cipher[idx] = 0;
}

void transpose_decrypt(char *cipher, char *plain, int len, int cols) {
    int rows;
    int full_cols;
    int r;
    int c;
    int ci;
    int pi;

    rows = (len + cols - 1) / cols;
    full_cols = len - (rows - 1) * cols;  // columns that have 'rows' elements

    ci = 0;
    // Read column by column
    for (c = 0; c < cols; c = c + 1) {
        int col_len;
        if (c < full_cols) {
            col_len = rows;
        } else {
            col_len = rows - 1;
        }
        for (r = 0; r < col_len; r = r + 1) {
            pi = r * cols + c;
            if (pi < len) {
                plain[pi] = cipher[ci];
                ci = ci + 1;
            }
        }
    }
    plain[len] = 0;
}

// ---- Frequency Analysis ----

int freq[26];

void count_letter_freq(char *text, int len) {
    int i;
    int ch;
    for (i = 0; i < 26; i = i + 1) {
        freq[i] = 0;
    }
    for (i = 0; i < len; i = i + 1) {
        if (text[i] >= 'A' && text[i] <= 'Z') {
            freq[text[i] - 'A'] = freq[text[i] - 'A'] + 1;
        } else if (text[i] >= 'a' && text[i] <= 'z') {
            freq[text[i] - 'a'] = freq[text[i] - 'a'] + 1;
        }
    }
}

int most_frequent_letter() {
    int i;
    int max_freq;
    int max_ch;
    max_freq = 0;
    max_ch = 0;
    for (i = 0; i < 26; i = i + 1) {
        if (freq[i] > max_freq) {
            max_freq = freq[i];
            max_ch = i;
        }
    }
    return max_ch;
}

// ---- Substitution Cipher ----

char sub_enc[26];
char sub_dec[26];

void make_substitution_key() {
    // Simple shift-based substitution: each letter -> letter + 1 (wrap)
    int i;
    for (i = 0; i < 26; i = i + 1) {
        sub_enc[i] = 'A' + (i + 1) % 26;
        sub_dec[(i + 1) % 26] = 'A' + i;
    }
}

void sub_encrypt(char *plain, char *cipher, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (plain[i] >= 'A' && plain[i] <= 'Z') {
            cipher[i] = sub_enc[plain[i] - 'A'];
        } else {
            cipher[i] = plain[i];
        }
    }
    cipher[len] = 0;
}

void sub_decrypt(char *cipher, char *plain, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (cipher[i] >= 'A' && cipher[i] <= 'Z') {
            plain[i] = sub_dec[cipher[i] - 'A'];
        } else {
            plain[i] = cipher[i];
        }
    }
    plain[len] = 0;
}

// ---- Affine Cipher ----
// E(x) = (a*x + b) mod 26
// D(y) = a_inv * (y - b) mod 26

void affine_encrypt(char *plain, char *cipher, int len, int a, int b) {
    int i;
    int x;
    for (i = 0; i < len; i = i + 1) {
        if (plain[i] >= 'A' && plain[i] <= 'Z') {
            x = plain[i] - 'A';
            cipher[i] = 'A' + (a * x + b) % 26;
        } else {
            cipher[i] = plain[i];
        }
    }
    cipher[len] = 0;
}

void affine_decrypt(char *cipher, char *plain, int len, int a, int b) {
    int i;
    int y;
    int a_inv;
    a_inv = mod_inverse(a, 26);
    for (i = 0; i < len; i = i + 1) {
        if (cipher[i] >= 'A' && cipher[i] <= 'Z') {
            y = cipher[i] - 'A';
            plain[i] = 'A' + (a_inv * (y - b + 26)) % 26;
        } else {
            plain[i] = cipher[i];
        }
    }
    plain[len] = 0;
}

// ---- ROT13 ----

void rot13(char *input, char *output, int len) {
    caesar_encrypt(input, output, 13, len);
}

// ---- Atbash cipher ----

void atbash(char *input, char *output, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (input[i] >= 'A' && input[i] <= 'Z') {
            output[i] = 'Z' - (input[i] - 'A');
        } else if (input[i] >= 'a' && input[i] <= 'z') {
            output[i] = 'z' - (input[i] - 'a');
        } else {
            output[i] = input[i];
        }
    }
    output[len] = 0;
}

// ---- Rail fence cipher ----

void rail_fence_encrypt(char *plain, char *cipher, int len, int rails) {
    int r;
    int i;
    int idx;
    int dir;
    int row;
    int rail_buf[8][64];
    int rail_len[8];

    for (r = 0; r < rails; r = r + 1) {
        rail_len[r] = 0;
    }

    row = 0;
    dir = 1;
    for (i = 0; i < len; i = i + 1) {
        rail_buf[row][rail_len[row]] = plain[i];
        rail_len[row] = rail_len[row] + 1;
        if (row == 0) {
            dir = 1;
        }
        if (row == rails - 1) {
            dir = -1;
        }
        row = row + dir;
    }

    idx = 0;
    for (r = 0; r < rails; r = r + 1) {
        for (i = 0; i < rail_len[r]; i = i + 1) {
            cipher[idx] = rail_buf[r][i];
            idx = idx + 1;
        }
    }
    cipher[idx] = 0;
}

// ---- Print char array ----

void print_chars(char *s, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        printf("%c", s[i]);
    }
}

// ---- String compare helper ----

int str_equal(char *a, char *b, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    return 1;
}

int main() {
    char plain[64];
    char cipher[64];
    char decrypted[64];
    char key_str[32];
    int ok;
    int plen;
    int klen;
    struct HillKey hk;
    int c1;
    int c2;
    int d1;
    int d2;
    int otp_p[8];
    int otp_k[8];
    int otp_c[8];
    int otp_d[8];
    int i;
    int mf;
    char freq_in[32];
    int freq_len;

    // Caesar
    printf("=== Caesar Cipher ===\n");
    plain[0]='H'; plain[1]='E'; plain[2]='L'; plain[3]='L'; plain[4]='O';
    plain[5]='W'; plain[6]='O'; plain[7]='R'; plain[8]='L'; plain[9]='D'; plain[10]=0;
    plen = 10;

    printf("Plain:  ");
    print_chars(plain, plen);
    printf("\n");
    printf("Key: 3\n");

    caesar_encrypt(plain, cipher, 3, plen);
    printf("Cipher: ");
    print_chars(cipher, plen);
    printf("\n");

    caesar_decrypt(cipher, decrypted, 3, plen);
    printf("Decrypt: ");
    print_chars(decrypted, plen);
    printf("\n");

    if (str_equal(plain, decrypted, plen)) {
        printf("Caesar roundtrip OK\n");
    } else {
        printf("Caesar roundtrip FAIL\n");
    }

    // Vigenere
    printf("=== Vigenere Cipher ===\n");
    plain[0]='A'; plain[1]='T'; plain[2]='T'; plain[3]='A'; plain[4]='C'; plain[5]='K';
    plain[6]='A'; plain[7]='T'; plain[8]='D'; plain[9]='A'; plain[10]='W'; plain[11]='N'; plain[12]=0;
    plen = 12;
    key_str[0]='L'; key_str[1]='E'; key_str[2]='M'; key_str[3]='O'; key_str[4]='N'; key_str[5]=0;
    klen = 5;

    printf("Plain:  ");
    print_chars(plain, plen);
    printf("\n");
    printf("Key:    ");
    // Print key repeated to match plain length
    for (i = 0; i < plen; i = i + 1) {
        printf("%c", key_str[i % klen]);
    }
    printf("\n");

    vigenere_encrypt(plain, key_str, cipher, plen, klen);
    printf("Cipher: ");
    print_chars(cipher, plen);
    printf("\n");

    vigenere_decrypt(cipher, key_str, decrypted, plen, klen);
    printf("Decrypt: ");
    print_chars(decrypted, plen);
    printf("\n");

    if (str_equal(plain, decrypted, plen)) {
        printf("Vigenere roundtrip OK\n");
    } else {
        printf("Vigenere roundtrip FAIL\n");
    }

    // Playfair preparation
    printf("=== Playfair Preparation ===\n");
    key_str[0]='M'; key_str[1]='O'; key_str[2]='N'; key_str[3]='A'; key_str[4]='R';
    key_str[5]='C'; key_str[6]='H'; key_str[7]='Y'; key_str[8]=0;
    printf("Key: ");
    print_chars(key_str, 8);
    printf("\n");
    playfair_prepare(key_str, 8);
    printf("Playfair grid:\n");
    print_playfair_grid();

    // Hill cipher 2x2
    printf("=== Hill Cipher 2x2 ===\n");
    hk.a = 6; hk.b = 24; hk.c = 1; hk.d = 16;
    printf("Key matrix: [%d,%d;%d,%d]\n", hk.a, hk.b, hk.c, hk.d);

    // Encrypt "AC" -> values 0, 2
    printf("Plain: AC -> values 0 2\n");
    hill_encrypt_pair(hk, 0, 2, &c1, &c2);
    printf("Cipher values: %d %d -> %c%c\n", c1, c2, 'A' + c1, 'A' + c2);

    hill_decrypt_pair(hk, c1, c2, &d1, &d2);
    printf("Decrypt values: %d %d -> %c%c\n", d1, d2, 'A' + d1, 'A' + d2);

    if (d1 == 0 && d2 == 2) {
        printf("Hill roundtrip OK\n");
    } else {
        printf("Hill roundtrip FAIL\n");
    }

    // One-Time Pad
    printf("=== One-Time Pad (XOR) ===\n");
    otp_p[0]=72; otp_p[1]=69; otp_p[2]=76; otp_p[3]=76; otp_p[4]=79;
    otp_k[0]=41; otp_k[1]=55; otp_k[2]=33; otp_k[3]=28; otp_k[4]=90;

    printf("Plain:  ");
    for (i = 0; i < 5; i = i + 1) {
        if (i > 0) printf(" ");
        printf("%d", otp_p[i]);
    }
    printf("\n");
    printf("Key:    ");
    for (i = 0; i < 5; i = i + 1) {
        if (i > 0) printf(" ");
        printf("%d", otp_k[i]);
    }
    printf("\n");

    otp_encrypt(otp_p, otp_k, otp_c, 5);
    printf("Cipher: ");
    for (i = 0; i < 5; i = i + 1) {
        if (i > 0) printf(" ");
        printf("%d", otp_c[i]);
    }
    printf("\n");

    otp_decrypt(otp_c, otp_k, otp_d, 5);
    printf("Decrypt: ");
    for (i = 0; i < 5; i = i + 1) {
        if (i > 0) printf(" ");
        printf("%d", otp_d[i]);
    }
    printf("\n");

    ok = 1;
    for (i = 0; i < 5; i = i + 1) {
        if (otp_d[i] != otp_p[i]) ok = 0;
    }
    if (ok) {
        printf("OTP roundtrip OK\n");
    } else {
        printf("OTP roundtrip FAIL\n");
    }

    // Transposition
    printf("=== Transposition Cipher ===\n");
    plain[0]='H'; plain[1]='E'; plain[2]='L'; plain[3]='L'; plain[4]='O';
    plain[5]='W'; plain[6]='O'; plain[7]='R'; plain[8]='L'; plain[9]='D'; plain[10]=0;
    plen = 10;

    printf("Plain:  ");
    print_chars(plain, plen);
    printf("\n");
    printf("Cols: 3\n");

    transpose_encrypt(plain, cipher, plen, 3);
    printf("Transposed: ");
    print_chars(cipher, plen);
    printf("\n");

    transpose_decrypt(cipher, decrypted, plen, 3);
    printf("Untransposed: ");
    print_chars(decrypted, plen);
    printf("\n");

    if (str_equal(plain, decrypted, plen)) {
        printf("Transposition roundtrip OK\n");
    } else {
        printf("Transposition roundtrip FAIL\n");
    }

    // Frequency Analysis
    printf("=== Frequency Analysis ===\n");
    freq_in[0]='A'; freq_in[1]='A'; freq_in[2]='B'; freq_in[3]='A'; freq_in[4]='B';
    freq_in[5]='C'; freq_in[6]='A'; freq_in[7]='B'; freq_in[8]='C'; freq_in[9]='D';
    freq_in[10]='A'; freq_in[11]='B'; freq_in[12]='C'; freq_in[13]='D'; freq_in[14]='E';
    freq_in[15]=0;
    freq_len = 15;

    printf("Input: ");
    print_chars(freq_in, freq_len);
    printf("\n");

    count_letter_freq(freq_in, freq_len);
    printf("Frequencies:\n");
    printf("A: %d\n", freq['A' - 'A']);
    printf("B: %d\n", freq['B' - 'A']);
    printf("C: %d\n", freq['C' - 'A']);
    printf("D: %d\n", freq['D' - 'A']);
    printf("E: %d\n", freq['E' - 'A']);

    mf = most_frequent_letter();
    printf("Most frequent: %c (%d)\n", 'A' + mf, freq[mf]);

    // Substitution
    printf("=== Substitution Cipher ===\n");
    make_substitution_key();
    plain[0]='H'; plain[1]='E'; plain[2]='L'; plain[3]='L'; plain[4]='O'; plain[5]=0;
    plen = 5;

    printf("Plain:  ");
    print_chars(plain, plen);
    printf("\n");

    sub_encrypt(plain, cipher, plen);
    printf("Cipher: ");
    print_chars(cipher, plen);
    printf("\n");

    sub_decrypt(cipher, decrypted, plen);
    printf("Decrypt: ");
    print_chars(decrypted, plen);
    printf("\n");

    if (str_equal(plain, decrypted, plen)) {
        printf("Substitution roundtrip OK\n");
    } else {
        printf("Substitution roundtrip FAIL\n");
    }

    // Affine cipher
    printf("=== Affine Cipher ===\n");
    plain[0]='H'; plain[1]='E'; plain[2]='L'; plain[3]='L'; plain[4]='O';
    plain[5]='W'; plain[6]='O'; plain[7]='R'; plain[8]='L'; plain[9]='D'; plain[10]=0;
    plen = 10;

    printf("Plain:  ");
    print_chars(plain, plen);
    printf("\n");
    printf("A=5, B=8\n");

    affine_encrypt(plain, cipher, plen, 5, 8);
    printf("Cipher: ");
    print_chars(cipher, plen);
    printf("\n");

    affine_decrypt(cipher, decrypted, plen, 5, 8);
    printf("Decrypt: ");
    print_chars(decrypted, plen);
    printf("\n");

    if (str_equal(plain, decrypted, plen)) {
        printf("Affine roundtrip OK\n");
    } else {
        printf("Affine roundtrip FAIL\n");
    }

    printf("Done.\n");
    return 0;
}
