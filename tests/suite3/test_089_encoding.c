int printf(const char *fmt, ...);

/* ---- Base64-like encoding (custom alphabet, 6-bit groups) ----
   Uses standard Base64 alphabet. Encodes 3 bytes -> 4 chars.
   Padding with '=' when input length not multiple of 3.
*/

static char b64_table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void base64_encode(unsigned char *in, int inlen, char *out) {
    int i = 0, j = 0;
    unsigned char a, b, c;
    while (i < inlen) {
        a = in[i]; b = (i+1 < inlen) ? in[i+1] : 0;
        c = (i+2 < inlen) ? in[i+2] : 0;
        out[j++] = b64_table[a >> 2];
        out[j++] = b64_table[((a & 3) << 4) | (b >> 4)];
        out[j++] = (i+1 < inlen) ? b64_table[((b & 15) << 2) | (c >> 6)] : '=';
        out[j++] = (i+2 < inlen) ? b64_table[c & 63] : '=';
        i += 3;
    }
    out[j] = '\0';
}

int b64_val(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

int base64_decode(char *in, int inlen, unsigned char *out) {
    int i = 0, j = 0;
    while (i < inlen) {
        int a = b64_val(in[i]);
        int b = b64_val(in[i+1]);
        int c = (in[i+2] == '=') ? 0 : b64_val(in[i+2]);
        int d = (in[i+3] == '=') ? 0 : b64_val(in[i+3]);
        out[j++] = (unsigned char)((a << 2) | (b >> 4));
        if (in[i+2] != '=') out[j++] = (unsigned char)(((b & 15) << 4) | (c >> 2));
        if (in[i+3] != '=') out[j++] = (unsigned char)(((c & 3) << 6) | d);
        i += 4;
    }
    return j;
}

/* ---- Run-length encoding ---- */
int rle_encode(unsigned char *in, int n, unsigned char *out) {
    int i = 0, j = 0;
    while (i < n) {
        unsigned char cur = in[i];
        int cnt = 1;
        while (i + cnt < n && in[i+cnt] == cur && cnt < 127) cnt++;
        out[j++] = (unsigned char)cnt;
        out[j++] = cur;
        i += cnt;
    }
    return j;
}

int rle_decode(unsigned char *in, int n, unsigned char *out) {
    int i = 0, j = 0;
    while (i < n) {
        int cnt = in[i++];
        unsigned char val = in[i++];
        int k;
        for (k = 0; k < cnt; k++) out[j++] = val;
    }
    return j;
}

/* ---- XOR cipher ---- */
void xor_cipher(unsigned char *in, int n, unsigned char key, unsigned char *out) {
    int i;
    for (i = 0; i < n; i++) out[i] = in[i] ^ key;
}

/* ---- Caesar cipher ---- */
void caesar_encode(char *in, int shift, char *out) {
    int i;
    for (i = 0; in[i] != '\0'; i++) {
        char c = in[i];
        if (c >= 'a' && c <= 'z') out[i] = (char)('a' + (c - 'a' + shift) % 26);
        else if (c >= 'A' && c <= 'Z') out[i] = (char)('A' + (c - 'A' + shift) % 26);
        else out[i] = c;
    }
    out[i] = '\0';
}

void caesar_decode(char *in, int shift, char *out) {
    caesar_encode(in, 26 - (shift % 26), out);
}

/* ---- Simple string length helper ---- */
int slen(char *s) { int n = 0; while (s[n]) n++; return n; }
int ulen(unsigned char *s, int n) { return n; }

/* String equality */
int str_eq(char *a, char *b) {
    int i = 0;
    while (a[i] && b[i] && a[i] == b[i]) i++;
    return a[i] == b[i];
}

int main(void) {
    int i;

    /* Base64 encode */
    {
        unsigned char m1[] = "Man";
        char enc[16];
        base64_encode(m1, 3, enc);
        printf("%s\n", enc);  // EXPECT: TWFu

        unsigned char m2[] = "Ma";
        base64_encode(m2, 2, enc);
        printf("%s\n", enc);  // EXPECT: TWE=

        unsigned char m3[] = "M";
        base64_encode(m3, 1, enc);
        printf("%s\n", enc);  // EXPECT: TQ==

        unsigned char m4[] = "Hello";
        char enc2[12];
        base64_encode(m4, 5, enc2);
        printf("%s\n", enc2);  // EXPECT: SGVsbG8=
    }

    /* Base64 decode roundtrip */
    {
        unsigned char original[] = "Test";
        char enc[12];
        unsigned char dec[12];
        int declen;
        base64_encode(original, 4, enc);
        printf("%s\n", enc);  // EXPECT: VGVzdA==
        declen = base64_decode(enc, 8, dec);
        dec[declen] = '\0';
        printf("%s\n", (char*)dec);  // EXPECT: Test
    }

    /* RLE encode/decode */
    {
        unsigned char data[] = {3, 3, 3, 7, 7, 5};
        unsigned char enc[16], dec[16];
        int elen = rle_encode(data, 6, enc);
        /* enc should be: 3,3, 2,7, 1,5 */
        printf("%d\n", elen);     // EXPECT: 6
        printf("%d\n", enc[0]);   // EXPECT: 3
        printf("%d\n", enc[1]);   // EXPECT: 3
        printf("%d\n", enc[2]);   // EXPECT: 2
        printf("%d\n", enc[3]);   // EXPECT: 7
        printf("%d\n", enc[4]);   // EXPECT: 1
        printf("%d\n", enc[5]);   // EXPECT: 5

        int dlen = rle_decode(enc, elen, dec);
        printf("%d\n", dlen);  // EXPECT: 6
        for (i = 0; i < dlen; i++) printf("%d\n", dec[i]);
        // EXPECT: 3
        // EXPECT: 3
        // EXPECT: 3
        // EXPECT: 7
        // EXPECT: 7
        // EXPECT: 5
    }

    /* XOR cipher roundtrip */
    {
        unsigned char plain[] = {72, 101, 108, 108, 111}; /* "Hello" */
        unsigned char enc[8], dec[8];
        unsigned char key = 42;
        xor_cipher(plain, 5, key, enc);
        xor_cipher(enc, 5, key, dec);
        /* Check roundtrip */
        int ok = 1;
        for (i = 0; i < 5; i++) if (dec[i] != plain[i]) ok = 0;
        printf("%d\n", ok);  // EXPECT: 1
        /* Check enc[0] = 72^42 = 98 */
        printf("%d\n", enc[0]);  // EXPECT: 98
        /* Check enc[1] = 101^42 = 79 */
        printf("%d\n", enc[1]);  // EXPECT: 79
    }

    /* Caesar cipher */
    {
        char plain[] = "Hello World";
        char enc[20], dec[20];
        caesar_encode(plain, 3, enc);
        printf("%s\n", enc);  // EXPECT: Khoor Zruog
        caesar_decode(enc, 3, dec);
        printf("%s\n", dec);  // EXPECT: Hello World

        char plain2[] = "xyz";
        char enc2[8];
        caesar_encode(plain2, 3, enc2);
        printf("%s\n", enc2);  // EXPECT: abc

        char plain3[] = "ABC";
        char enc3[8];
        caesar_encode(plain3, 13, enc3);
        printf("%s\n", enc3);  // EXPECT: NOP
    }

    return 0;
}
