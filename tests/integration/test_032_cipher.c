int printf(const char *fmt, ...);
// EXPECT: c1=K c2=h c3=o c4=o c5=r c6=32 c7=Z c8=r c9=u c10=o c11=g d1=H d2=e d3=l d4=l d5=o d6=32 d7=W d8=o d9=r d10=l d11=d r1=U r2=r r3=y r4=y r5=b rr1=H rr2=e rr3=l rr4=l rr5=o a1=Z a2=Y a3=X aa1=H aa2=E aa3=L aa4=L aa5=O s1=J s2=V s3=N s4=N s5=E sd1=H sd2=E sd3=L sd4=L sd5=O v1=H v2=f v3=n v4=o v5=o vd1=H vd2=e vd3=l vd4=l vd5=o x1=1 x2=1 x3=1 x4=1 x5=1 fe=4 fm=69 g1=4 g2=1

/* Helper: is uppercase */
int is_upper(char c) {
    if (c >= 65 && c <= 90) return 1;
    return 0;
}

/* Helper: is lowercase */
int is_lower(char c) {
    if (c >= 97 && c <= 122) return 1;
    return 0;
}

/* Helper: is alpha */
int is_alpha(char c) {
    if (is_upper(c)) return 1;
    if (is_lower(c)) return 1;
    return 0;
}

/* Helper: to upper */
char to_upper(char c) {
    if (is_lower(c)) return c - 32;
    return c;
}

/* Helper: to lower */
char to_lower(char c) {
    if (is_upper(c)) return c + 32;
    return c;
}

/* String length */
int str_len(char *s) {
    int n;
    n = 0;
    while (s[n] != 0) {
        n = n + 1;
    }
    return n;
}

/* String copy */
void str_copy(char *dst, char *src) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

/* String compare: 1 if equal, 0 if not */
int str_equal(char *a, char *b) {
    int i;
    i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    if (a[i] != b[i]) return 0;
    return 1;
}

/* Caesar encrypt: shift each alpha char by 'shift' positions */
void caesar_encrypt(char *input, int shift, char *out) {
    int i;
    char c;
    int base;
    int pos;
    shift = shift % 26;
    if (shift < 0) shift = shift + 26;
    i = 0;
    while (input[i] != 0) {
        c = input[i];
        if (is_upper(c)) {
            base = 65;
            pos = c - base;
            pos = (pos + shift) % 26;
            out[i] = base + pos;
        } else if (is_lower(c)) {
            base = 97;
            pos = c - base;
            pos = (pos + shift) % 26;
            out[i] = base + pos;
        } else {
            out[i] = c;
        }
        i = i + 1;
    }
    out[i] = 0;
}

/* Caesar decrypt */
void caesar_decrypt(char *input, int shift, char *out) {
    caesar_encrypt(input, 26 - (shift % 26), out);
}

/* ROT13 */
void rot13(char *input, char *out) {
    caesar_encrypt(input, 13, out);
}

/* Atbash: A<->Z, B<->Y, etc */
void atbash(char *input, char *out) {
    int i;
    char c;
    i = 0;
    while (input[i] != 0) {
        c = input[i];
        if (is_upper(c)) {
            out[i] = 90 - (c - 65);
        } else if (is_lower(c)) {
            out[i] = 122 - (c - 97);
        } else {
            out[i] = c;
        }
        i = i + 1;
    }
    out[i] = 0;
}

/* Substitution encrypt: key[0] = replacement for A, etc. */
void sub_encrypt(char *input, char *key, char *out) {
    int i;
    char c;
    int idx;
    i = 0;
    while (input[i] != 0) {
        c = to_upper(input[i]);
        if (is_upper(c)) {
            idx = c - 65;
            out[i] = key[idx];
        } else {
            out[i] = input[i];
        }
        i = i + 1;
    }
    out[i] = 0;
}

/* Build inverse substitution key */
void build_inv_key(char *key, char *inv) {
    int i;
    int idx;
    i = 0;
    while (i < 26) {
        idx = key[i] - 65;
        inv[idx] = 65 + i;
        i = i + 1;
    }
    inv[26] = 0;
}

/* Substitution decrypt */
void sub_decrypt(char *input, char *key, char *out) {
    char inv[27];
    build_inv_key(key, inv);
    sub_encrypt(input, inv, out);
}

/* Vigenere encrypt with int key array */
void vig_encrypt(char *input, int *key, int klen, char *out) {
    int i;
    int ki;
    char c;
    int base;
    int pos;
    int shift;
    i = 0;
    ki = 0;
    while (input[i] != 0) {
        c = input[i];
        if (is_alpha(c)) {
            shift = key[ki % klen];
            if (is_upper(c)) {
                base = 65;
            } else {
                base = 97;
            }
            pos = c - base;
            pos = (pos + shift) % 26;
            out[i] = base + pos;
            ki = ki + 1;
        } else {
            out[i] = c;
        }
        i = i + 1;
    }
    out[i] = 0;
}

/* Vigenere decrypt */
void vig_decrypt(char *input, int *key, int klen, char *out) {
    int inv_key[100];
    int i;
    i = 0;
    while (i < klen) {
        inv_key[i] = (26 - key[i]) % 26;
        i = i + 1;
    }
    vig_encrypt(input, inv_key, klen, out);
}

/* XOR cipher: store numeric results */
void xor_cipher(char *input, int key, int *out, int *olen) {
    int i;
    i = 0;
    while (input[i] != 0) {
        out[i] = input[i] ^ key;
        i = i + 1;
    }
    *olen = i;
}

/* XOR decipher */
void xor_decipher(int *input, int len, int key, char *out) {
    int i;
    i = 0;
    while (i < len) {
        out[i] = input[i] ^ key;
        i = i + 1;
    }
    out[len] = 0;
}

/* GCD */
int gcd(int a, int b) {
    int t;
    while (b != 0) {
        t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Modular inverse of a mod m */
int mod_inverse(int a, int m) {
    int i;
    a = a % m;
    i = 1;
    while (i < m) {
        if ((a * i) % m == 1) return i;
        i = i + 1;
    }
    return -1;
}

/* Affine encrypt: E(x) = (a*x + b) mod 26 */
void affine_encrypt(char *input, int a, int b, char *out) {
    int i;
    char c;
    int x;
    int y;
    i = 0;
    while (input[i] != 0) {
        c = input[i];
        if (is_upper(c)) {
            x = c - 65;
            y = (a * x + b) % 26;
            out[i] = 65 + y;
        } else if (is_lower(c)) {
            x = c - 97;
            y = (a * x + b) % 26;
            out[i] = 97 + y;
        } else {
            out[i] = c;
        }
        i = i + 1;
    }
    out[i] = 0;
}

/* Affine decrypt */
void affine_decrypt(char *input, int a, int b, char *out) {
    int i;
    char c;
    int x;
    int y;
    int a_inv;
    a_inv = mod_inverse(a, 26);
    i = 0;
    while (input[i] != 0) {
        c = input[i];
        if (is_upper(c)) {
            x = c - 65;
            y = (a_inv * (x - b + 26)) % 26;
            out[i] = 65 + y;
        } else if (is_lower(c)) {
            x = c - 97;
            y = (a_inv * (x - b + 26)) % 26;
            out[i] = 97 + y;
        } else {
            out[i] = c;
        }
        i = i + 1;
    }
    out[i] = 0;
}

/* Letter frequency: count occurrences of each letter (A-Z) */
void letter_freq(char *input, int *freq) {
    int i;
    char c;
    i = 0;
    while (i < 26) {
        freq[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (input[i] != 0) {
        c = to_upper(input[i]);
        if (is_upper(c)) {
            freq[c - 65] = freq[c - 65] + 1;
        }
        i = i + 1;
    }
}

/* Most frequent letter (returns ASCII value) */
int most_freq_letter(char *input) {
    int freq[26];
    int max_f;
    int max_i;
    int i;
    letter_freq(input, freq);
    max_f = -1;
    max_i = 0;
    i = 0;
    while (i < 26) {
        if (freq[i] > max_f) {
            max_f = freq[i];
            max_i = i;
        }
        i = i + 1;
    }
    return 65 + max_i;
}

/* Reverse string in place */
void reverse_str(char *s) {
    int len;
    int i;
    char tmp;
    len = str_len(s);
    i = 0;
    while (i < len / 2) {
        tmp = s[i];
        s[i] = s[len - 1 - i];
        s[len - 1 - i] = tmp;
        i = i + 1;
    }
}

/* Rail fence encrypt (2 rails) */
void rail_encrypt2(char *input, char *out) {
    int len;
    int i;
    int j;
    len = str_len(input);
    j = 0;
    i = 0;
    while (i < len) {
        out[j] = input[i];
        j = j + 1;
        i = i + 2;
    }
    i = 1;
    while (i < len) {
        out[j] = input[i];
        j = j + 1;
        i = i + 2;
    }
    out[j] = 0;
}

/* Rail fence decrypt (2 rails) */
void rail_decrypt2(char *input, char *out) {
    int len;
    int half;
    int i;
    int j;
    int k;
    len = str_len(input);
    half = (len + 1) / 2;
    j = 0;
    k = half;
    i = 0;
    while (i < len) {
        if (i % 2 == 0) {
            out[i] = input[j];
            j = j + 1;
        } else {
            out[i] = input[k];
            k = k + 1;
        }
        i = i + 1;
    }
    out[len] = 0;
}

/* Columnar transposition encrypt (simplified, 3 columns) */
void columnar_encrypt3(char *input, char *out) {
    int len;
    int col;
    int i;
    int j;
    len = str_len(input);
    j = 0;
    col = 0;
    while (col < 3) {
        i = col;
        while (i < len) {
            out[j] = input[i];
            j = j + 1;
            i = i + 3;
        }
        col = col + 1;
    }
    out[j] = 0;
}

/* Count alphabetic characters */
int count_alpha(char *s) {
    int i;
    int cnt;
    cnt = 0;
    i = 0;
    while (s[i] != 0) {
        if (is_alpha(s[i])) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

/* Count uppercase */
int count_upper(char *s) {
    int i;
    int cnt;
    cnt = 0;
    i = 0;
    while (s[i] != 0) {
        if (is_upper(s[i])) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

/* Count lowercase */
int count_lower(char *s) {
    int i;
    int cnt;
    cnt = 0;
    i = 0;
    while (s[i] != 0) {
        if (is_lower(s[i])) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

int main(void) {
    char buf1[256];
    char buf2[256];
    int xbuf[256];
    int xlen;
    int freq[26];
    int vkey[10];
    char skey[27];

    /* Set up "Hello World" as input */
    char hw[12];
    hw[0] = 72; hw[1] = 101; hw[2] = 108; hw[3] = 108; hw[4] = 111;
    hw[5] = 32; hw[6] = 87; hw[7] = 111; hw[8] = 114; hw[9] = 108;
    hw[10] = 100; hw[11] = 0;

    /* Caesar encrypt shift=3: H->K e->h l->o l->o o->r ' '->' ' W->Z o->r r->u l->o d->g */
    caesar_encrypt(hw, 3, buf1);
    printf("c1=%c ", buf1[0]);
    printf("c2=%c ", buf1[1]);
    printf("c3=%c ", buf1[2]);
    printf("c4=%c ", buf1[3]);
    printf("c5=%c ", buf1[4]);
    printf("c6=%d ", buf1[5]);
    printf("c7=%c ", buf1[6]);
    printf("c8=%c ", buf1[7]);
    printf("c9=%c ", buf1[8]);
    printf("c10=%c ", buf1[9]);
    printf("c11=%c ", buf1[10]);

    /* Caesar decrypt shift=3 should give original */
    caesar_decrypt(buf1, 3, buf2);
    printf("d1=%c ", buf2[0]);
    printf("d2=%c ", buf2[1]);
    printf("d3=%c ", buf2[2]);
    printf("d4=%c ", buf2[3]);
    printf("d5=%c ", buf2[4]);
    printf("d6=%d ", buf2[5]);
    printf("d7=%c ", buf2[6]);
    printf("d8=%c ", buf2[7]);
    printf("d9=%c ", buf2[8]);
    printf("d10=%c ", buf2[9]);
    printf("d11=%c ", buf2[10]);

    /* ROT13 of "Hello" */
    {
        char hi[6];
        hi[0] = 72; hi[1] = 101; hi[2] = 108; hi[3] = 108; hi[4] = 111; hi[5] = 0;
        rot13(hi, buf1);
        printf("r1=%c ", buf1[0]);
        printf("r2=%c ", buf1[1]);
        printf("r3=%c ", buf1[2]);
        printf("r4=%c ", buf1[3]);
        printf("r5=%c ", buf1[4]);
        /* Double ROT13 = original */
        rot13(buf1, buf2);
        printf("rr1=%c ", buf2[0]);
        printf("rr2=%c ", buf2[1]);
        printf("rr3=%c ", buf2[2]);
        printf("rr4=%c ", buf2[3]);
        printf("rr5=%c ", buf2[4]);
    }

    /* Atbash of "ABC" -> "ZYX" */
    {
        char abc[4];
        abc[0] = 65; abc[1] = 66; abc[2] = 67; abc[3] = 0;
        atbash(abc, buf1);
        printf("a1=%c ", buf1[0]);
        printf("a2=%c ", buf1[1]);
        printf("a3=%c ", buf1[2]);
    }

    /* Double Atbash = original */
    {
        char hi[6];
        hi[0] = 72; hi[1] = 69; hi[2] = 76; hi[3] = 76; hi[4] = 79; hi[5] = 0;
        atbash(hi, buf1);
        atbash(buf1, buf2);
        printf("aa1=%c ", buf2[0]);
        printf("aa2=%c ", buf2[1]);
        printf("aa3=%c ", buf2[2]);
        printf("aa4=%c ", buf2[3]);
        printf("aa5=%c ", buf2[4]);
    }

    /* Substitution cipher: "HELLO" with custom key */
    {
        char hi[6];
        hi[0] = 72; hi[1] = 69; hi[2] = 76; hi[3] = 76; hi[4] = 79; hi[5] = 0;
        /* Key: A->I, B->F, C->S, D->Q, E->V, F->U, G->H, H->J, */
        /* I->K, J->L, K->M, L->N, M->B, N->G, O->E, P->Z, Q->W, R->X, */
        /* S->Y, T->D, U->C, V->A, W->P, X->R, Y->T, Z->O */
        skey[0] = 73;  /* A->I */
        skey[1] = 70;  /* B->F */
        skey[2] = 83;  /* C->S */
        skey[3] = 81;  /* D->Q */
        skey[4] = 86;  /* E->V */
        skey[5] = 85;  /* F->U */
        skey[6] = 72;  /* G->H */
        skey[7] = 74;  /* H->J */
        skey[8] = 75;  /* I->K */
        skey[9] = 76;  /* J->L */
        skey[10] = 77; /* K->M */
        skey[11] = 78; /* L->N */
        skey[12] = 66; /* M->B */
        skey[13] = 71; /* N->G */
        skey[14] = 69; /* O->E */
        skey[15] = 90; /* P->Z */
        skey[16] = 87; /* Q->W */
        skey[17] = 88; /* R->X */
        skey[18] = 89; /* S->Y */
        skey[19] = 68; /* T->D */
        skey[20] = 67; /* U->C */
        skey[21] = 65; /* V->A */
        skey[22] = 80; /* W->P */
        skey[23] = 82; /* X->R */
        skey[24] = 84; /* Y->T */
        skey[25] = 79; /* Z->O */
        skey[26] = 0;

        /* H(7)->J, E(4)->V, L(11)->N, L(11)->N, O(14)->E */
        /* Wait: H=index 7 -> skey[7]=74='J'... but we want to print as single chars */
        /* Let me use: I, F, Y, Y, E -> from code above: skey[7]=J */
        /* Actually let's just compute: H=72, 72-65=7, skey[7]=74='J' */
        /* E=69, 69-65=4, skey[4]=86='V' */
        /* L=76, 76-65=11, skey[11]=78='N' */
        /* L=76, same='N' */
        /* O=79, 79-65=14, skey[14]=69='E' */
        /* So enc = "JVNNE" */
        /* Hmm wait, we wanted IFYYE. Let me recalculate. */
        /* Actually let me just change the key to get what EXPECT says. */
        /* Let me change EXPECT to match what skey gives: JVNNE */

        sub_encrypt(hi, skey, buf1);
        /* buf1 should be: J V N N E */
        printf("s1=%c ", buf1[0]);
        printf("s2=%c ", buf1[1]);
        printf("s3=%c ", buf1[2]);
        printf("s4=%c ", buf1[3]);
        printf("s5=%c ", buf1[4]);

        /* Decrypt should give back HELLO */
        sub_decrypt(buf1, skey, buf2);
        printf("sd1=%c ", buf2[0]);
        printf("sd2=%c ", buf2[1]);
        printf("sd3=%c ", buf2[2]);
        printf("sd4=%c ", buf2[3]);
        printf("sd5=%c ", buf2[4]);
    }

    /* Vigenere encrypt: "Hello" with key [0,1,2,3] */
    {
        char hi[6];
        hi[0] = 72; hi[1] = 101; hi[2] = 108; hi[3] = 108; hi[4] = 111; hi[5] = 0;
        vkey[0] = 0; vkey[1] = 1; vkey[2] = 2; vkey[3] = 3;
        /* H+0=H, e+1=f, l+2=n, l+3=o, o+0=o */
        vig_encrypt(hi, vkey, 4, buf1);
        printf("v1=%c ", buf1[0]);
        printf("v2=%c ", buf1[1]);
        printf("v3=%c ", buf1[2]);
        printf("v4=%c ", buf1[3]);
        printf("v5=%c ", buf1[4]);

        vig_decrypt(buf1, vkey, 4, buf2);
        printf("vd1=%c ", buf2[0]);
        printf("vd2=%c ", buf2[1]);
        printf("vd3=%c ", buf2[2]);
        printf("vd4=%c ", buf2[3]);
        printf("vd5=%c ", buf2[4]);
    }

    /* XOR roundtrip */
    {
        char abc[6];
        abc[0] = 65; abc[1] = 66; abc[2] = 67; abc[3] = 68; abc[4] = 69; abc[5] = 0;
        xor_cipher(abc, 42, xbuf, &xlen);
        xor_decipher(xbuf, xlen, 42, buf1);
        /* Should match original */
        printf("x1=%d ", (buf1[0] == 65));
        printf("x2=%d ", (buf1[1] == 66));
        printf("x3=%d ", (buf1[2] == 67));
        printf("x4=%d ", (buf1[3] == 68));
        printf("x5=%d ", (buf1[4] == 69));
    }

    /* Frequency of "HELLO THERE" */
    {
        char ht[14];
        ht[0] = 72; ht[1] = 69; ht[2] = 76; ht[3] = 76; ht[4] = 79;
        ht[5] = 32; ht[6] = 84; ht[7] = 72; ht[8] = 69; ht[9] = 82;
        ht[10] = 69; ht[11] = 0;
        /* E appears at index 1,8,10 = 3 times... wait: */
        /* H=72, E=69, L=76, L=76, O=79, ' ', T=84, H=72, E=69, R=82, E=69 */
        /* E count: indices 1,8,10 = 3 */
        /* Hmm let me add one more E */
        ht[10] = 69;
        ht[11] = 69; /* Add another E */
        ht[12] = 0;
        /* Now E count: 1,8,10,11 = 4 */
        letter_freq(ht, freq);
        printf("fe=%d ", freq[4]); /* E = index 4 */
        /* Most frequent letter */
        printf("fm=%d ", most_freq_letter(ht)); /* E=69 */
    }

    /* GCD */
    printf("g1=%d ", gcd(12, 8));
    printf("g2=%d", gcd(17, 13));
    printf("\n");

    return 0;
}
