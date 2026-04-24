int printf(const char *fmt, ...);

/* Base64 encoding/decoding (simplified, integer-based) */

char b64_table[65];

void init_b64_table(void) {
    int i;
    for (i = 0; i < 26; i++) b64_table[i] = 'A' + i;
    for (i = 0; i < 26; i++) b64_table[26 + i] = 'a' + i;
    for (i = 0; i < 10; i++) b64_table[52 + i] = '0' + i;
    b64_table[62] = '+';
    b64_table[63] = '/';
    b64_table[64] = 0;
}

int b64_char_to_val(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

int b64_encode_triple(int b0, int b1, int b2, char *out) {
    int combined = (b0 << 16) | (b1 << 8) | b2;
    out[0] = b64_table[(combined >> 18) & 0x3F];
    out[1] = b64_table[(combined >> 12) & 0x3F];
    out[2] = b64_table[(combined >> 6) & 0x3F];
    out[3] = b64_table[combined & 0x3F];
    return 4;
}

int b64_safe_val(char c) {
    int v = b64_char_to_val(c);
    if (v < 0) return 0;
    return v;
}

int b64_decode_quad(char *in, int *out) {
    int v0 = b64_safe_val(in[0]);
    int v1 = b64_safe_val(in[1]);
    int v2 = b64_safe_val(in[2]);
    int v3 = b64_safe_val(in[3]);
    int combined = (v0 << 18) | (v1 << 12) | (v2 << 6) | v3;
    out[0] = (combined >> 16) & 0xFF;
    out[1] = (combined >> 8) & 0xFF;
    out[2] = combined & 0xFF;
    return 3;
}

int b64_encode(int *data, int len, char *out) {
    int i = 0;
    int oi = 0;
    while (i + 2 < len) {
        b64_encode_triple(data[i], data[i+1], data[i+2], out + oi);
        i = i + 3;
        oi = oi + 4;
    }
    if (i < len) {
        int b0 = data[i];
        int b1 = (i + 1 < len) ? data[i+1] : 0;
        int b2 = 0;
        b64_encode_triple(b0, b1, b2, out + oi);
        if (len - i == 1) {
            out[oi + 2] = '=';
            out[oi + 3] = '=';
        } else {
            out[oi + 3] = '=';
        }
        oi = oi + 4;
    }
    out[oi] = 0;
    return oi;
}

int b64_decode(char *in, int inlen, int *out) {
    int i = 0;
    int oi = 0;
    int decoded[3];
    while (i + 4 <= inlen) {
        if (in[i] == '=' || in[i+1] == '=') break;
        b64_decode_quad(in + i, decoded);
        out[oi] = decoded[0];
        oi = oi + 1;
        if (in[i+2] != '=') {
            out[oi] = decoded[1];
            oi = oi + 1;
        }
        if (in[i+3] != '=') {
            out[oi] = decoded[2];
            oi = oi + 1;
        }
        i = i + 4;
    }
    return oi;
}

int main(void) {
    init_b64_table();

    printf("%c", b64_table[0]);
    printf("%c", b64_table[25]);
    printf("%c", b64_table[26]);
    printf("%c", b64_table[51]);
    printf("%c", b64_table[62]);
    printf("\n");
    // EXPECT: AZaz+

    /* Encode "Man" = 77, 97, 110 -> "TWFu" */
    int data1[3];
    data1[0] = 77; data1[1] = 97; data1[2] = 110;
    char enc1[16];
    b64_encode(data1, 3, enc1);
    printf("%s\n", enc1);
    // EXPECT: TWFu

    /* Decode "TWFu" back */
    int dec1[16];
    int dlen = b64_decode(enc1, 4, dec1);
    printf("%d %d %d\n", dec1[0], dec1[1], dec1[2]);
    // EXPECT: 77 97 110

    /* Encode "Ma" = 77, 97 -> "TWE=" (2 bytes, 1 pad) */
    int data2[2];
    data2[0] = 77; data2[1] = 97;
    char enc2[16];
    b64_encode(data2, 2, enc2);
    printf("%s\n", enc2);
    // EXPECT: TWE=

    /* Encode "M" = 77 -> "TQ==" (1 byte, 2 pads) */
    int data3[1];
    data3[0] = 77;
    char enc3[16];
    b64_encode(data3, 1, enc3);
    printf("%s\n", enc3);
    // EXPECT: TQ==

    /* Encode "Hello" = 72, 101, 108, 108, 111 */
    int data4[5];
    data4[0] = 72; data4[1] = 101; data4[2] = 108;
    data4[3] = 108; data4[4] = 111;
    char enc4[16];
    b64_encode(data4, 5, enc4);
    printf("%s\n", enc4);
    // EXPECT: SGVsbG8=

    /* Decode it back */
    int dec4[16];
    int dlen4 = b64_decode(enc4, 8, dec4);
    printf("%d %d %d %d %d\n", dec4[0], dec4[1], dec4[2], dec4[3], dec4[4]);
    // EXPECT: 72 101 108 108 111
    printf("decoded %d bytes\n", dlen4);
    // EXPECT: decoded 5 bytes

    /* Test char-to-val mapping */
    printf("%d\n", b64_char_to_val('A'));
    // EXPECT: 0
    printf("%d\n", b64_char_to_val('z'));
    // EXPECT: 51
    printf("%d\n", b64_char_to_val('9'));
    // EXPECT: 61
    printf("%d\n", b64_char_to_val('+'));
    // EXPECT: 62
    printf("%d\n", b64_char_to_val('?'));
    // EXPECT: -1

    /* Roundtrip: encode then decode a sequence */
    int data5[6];
    data5[0] = 0; data5[1] = 255; data5[2] = 128;
    data5[3] = 1; data5[4] = 127; data5[5] = 64;
    char enc5[16];
    b64_encode(data5, 6, enc5);
    int dec5[16];
    int dlen5 = b64_decode(enc5, 8, dec5);
    int match = 1;
    int k;
    for (k = 0; k < 6; k++) {
        if (data5[k] != dec5[k]) match = 0;
    }
    printf("roundtrip match: %d\n", match);
    // EXPECT: roundtrip match: 1
    printf("roundtrip len: %d\n", dlen5);
    // EXPECT: roundtrip len: 6

    printf("done\n");
    // EXPECT: done
    return 0;
}
