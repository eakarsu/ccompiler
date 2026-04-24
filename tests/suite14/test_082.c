int printf(const char *fmt, ...);

/* Substitution cipher with key array */

/* Build a substitution table from a key permutation */
/* key is a permutation of 0..25, maps letter index to substituted index */
void build_sub_table(int *key, int *enc_table, int *dec_table) {
    int i;
    i = 0;
    while (i < 26) {
        enc_table[i] = key[i];
        dec_table[key[i]] = i;
        i = i + 1;
    }
}

/* Encrypt a single letter (0-25) using substitution */
int sub_encrypt_letter(int letter, int *enc_table) {
    if (letter < 0 || letter > 25) return letter;
    return enc_table[letter];
}

/* Decrypt a single letter using substitution */
int sub_decrypt_letter(int letter, int *dec_table) {
    if (letter < 0 || letter > 25) return letter;
    return dec_table[letter];
}

/* Encrypt an array of letter indices */
void sub_encrypt(int *data, int len, int *enc_table) {
    int i;
    i = 0;
    while (i < len) {
        data[i] = sub_encrypt_letter(data[i], enc_table);
        i = i + 1;
    }
}

/* Decrypt an array of letter indices */
void sub_decrypt(int *data, int len, int *dec_table) {
    int i;
    i = 0;
    while (i < len) {
        data[i] = sub_decrypt_letter(data[i], dec_table);
        i = i + 1;
    }
}

/* Check arrays equal */
int arrays_eq(int *a, int *b, int len) {
    int i;
    i = 0;
    while (i < len) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return 1;
}

/* Copy array */
void arr_copy(int *dst, int *src, int len) {
    int i;
    i = 0;
    while (i < len) {
        dst[i] = src[i];
        i = i + 1;
    }
}

/* Check if key is a valid permutation of 0..25 */
int is_valid_perm(int *key) {
    int seen[26];
    int i;
    i = 0;
    while (i < 26) {
        seen[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (i < 26) {
        if (key[i] < 0 || key[i] > 25) return 0;
        if (seen[key[i]]) return 0;
        seen[key[i]] = 1;
        i = i + 1;
    }
    return 1;
}

/* Count fixed points (letters that map to themselves) */
int count_fixed(int *key) {
    int count;
    int i;
    count = 0;
    i = 0;
    while (i < 26) {
        if (key[i] == i) count = count + 1;
        i = i + 1;
    }
    return count;
}

int main() {
    /* Simple rotation key: shift by 3 (Caesar cipher as substitution) */
    int key[26];
    int enc_table[26];
    int dec_table[26];
    int data[8];
    int original[8];
    int i;

    /* Build Caesar-3 key: a->d, b->e, ..., w->z, x->a, y->b, z->c */
    i = 0;
    while (i < 26) {
        key[i] = (i + 3) % 26;
        i = i + 1;
    }

    printf("valid_perm: %d\n", is_valid_perm(key));
    // EXPECT: valid_perm: 1

    printf("fixed_pts: %d\n", count_fixed(key));
    // EXPECT: fixed_pts: 0

    build_sub_table(key, enc_table, dec_table);

    /* Test: enc_table[0]=3, enc_table[7]=10 */
    printf("enc_0: %d\n", enc_table[0]);
    // EXPECT: enc_0: 3
    printf("enc_7: %d\n", enc_table[7]);
    // EXPECT: enc_7: 10
    /* dec_table[3]=0, dec_table[10]=7 */
    printf("dec_3: %d\n", dec_table[3]);
    // EXPECT: dec_3: 0
    printf("dec_10: %d\n", dec_table[10]);
    // EXPECT: dec_10: 7

    /* Encrypt "hello" = 7,4,11,11,14 */
    data[0] = 7; data[1] = 4; data[2] = 11; data[3] = 11; data[4] = 14;
    arr_copy(original, data, 5);
    sub_encrypt(data, 5, enc_table);
    /* enc(7)=(7+3)%26=10, enc(4)=7, enc(11)=14, enc(11)=14, enc(14)=17 */
    printf("enc: %d %d %d %d %d\n", data[0], data[1], data[2], data[3], data[4]);
    // EXPECT: enc: 10 7 14 14 17

    /* Decrypt */
    sub_decrypt(data, 5, dec_table);
    printf("dec_ok: %d\n", arrays_eq(data, original, 5));
    // EXPECT: dec_ok: 1

    /* Test with a different key: reverse alphabet */
    /* a->z(25), b->y(24), ..., z->a(0) */
    i = 0;
    while (i < 26) {
        key[i] = 25 - i;
        i = i + 1;
    }

    printf("rev_valid: %d\n", is_valid_perm(key));
    // EXPECT: rev_valid: 1

    /* fixed points: only i where 25-i == i, i.e., i=12.5, so none */
    printf("rev_fixed: %d\n", count_fixed(key));
    // EXPECT: rev_fixed: 0

    build_sub_table(key, enc_table, dec_table);

    /* Encrypt "hello" = 7,4,11,11,14 with reverse */
    data[0] = 7; data[1] = 4; data[2] = 11; data[3] = 11; data[4] = 14;
    arr_copy(original, data, 5);
    sub_encrypt(data, 5, enc_table);
    /* enc(7)=25-7=18, enc(4)=25-4=21, enc(11)=25-11=14, enc(11)=14, enc(14)=25-14=11 */
    printf("rev_enc: %d %d %d %d %d\n", data[0], data[1], data[2], data[3], data[4]);
    // EXPECT: rev_enc: 18 21 14 14 11

    sub_decrypt(data, 5, dec_table);
    printf("rev_dec_ok: %d\n", arrays_eq(data, original, 5));
    // EXPECT: rev_dec_ok: 1

    /* Reverse cipher is its own inverse: encrypting twice gives original */
    arr_copy(data, original, 5);
    sub_encrypt(data, 5, enc_table);
    sub_encrypt(data, 5, enc_table);
    printf("rev_involution: %d\n", arrays_eq(data, original, 5));
    // EXPECT: rev_involution: 1

    /* Test with identity key */
    i = 0;
    while (i < 26) {
        key[i] = i;
        i = i + 1;
    }
    printf("id_fixed: %d\n", count_fixed(key));
    // EXPECT: id_fixed: 26

    build_sub_table(key, enc_table, dec_table);
    arr_copy(data, original, 5);
    sub_encrypt(data, 5, enc_table);
    printf("id_enc: %d\n", arrays_eq(data, original, 5));
    // EXPECT: id_enc: 1

    /* Test: swap only a<->b (indices 0<->1) */
    i = 0;
    while (i < 26) { key[i] = i; i = i + 1; }
    key[0] = 1;
    key[1] = 0;
    printf("swap_fixed: %d\n", count_fixed(key));
    // EXPECT: swap_fixed: 24

    build_sub_table(key, enc_table, dec_table);
    data[0] = 0; data[1] = 1; data[2] = 2; data[3] = 0; data[4] = 1;
    sub_encrypt(data, 5, enc_table);
    /* enc(0)=1, enc(1)=0, enc(2)=2, enc(0)=1, enc(1)=0 */
    printf("swap_enc: %d %d %d %d %d\n", data[0], data[1], data[2], data[3], data[4]);
    // EXPECT: swap_enc: 1 0 2 1 0

    printf("subst_cipher_done\n");
    // EXPECT: subst_cipher_done
    return 0;
}
