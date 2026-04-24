int printf(const char *fmt, ...);

char encode_char(int val) {
    if (val < 26) return 'A' + val;
    if (val < 52) return 'a' + (val - 26);
    if (val < 62) return '0' + (val - 52);
    if (val == 62) return '+';
    return '/';
}

int decode_char(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

int encode_triplet(int *data, int len, char *out) {
    int outlen = 0;
    int i = 0;
    while (i < len) {
        int b0 = data[i];
        int b1 = (i + 1 < len) ? data[i + 1] : 0;
        int b2 = (i + 2 < len) ? data[i + 2] : 0;

        int c0 = (b0 >> 2) & 63;
        int c1 = ((b0 & 3) << 4) | ((b1 >> 4) & 15);
        int c2 = ((b1 & 15) << 2) | ((b2 >> 6) & 3);
        int c3 = b2 & 63;

        out[outlen] = encode_char(c0); outlen = outlen + 1;
        out[outlen] = encode_char(c1); outlen = outlen + 1;

        if (i + 1 < len) {
            out[outlen] = encode_char(c2); outlen = outlen + 1;
        } else {
            out[outlen] = '='; outlen = outlen + 1;
        }

        if (i + 2 < len) {
            out[outlen] = encode_char(c3); outlen = outlen + 1;
        } else {
            out[outlen] = '='; outlen = outlen + 1;
        }

        i = i + 3;
    }
    out[outlen] = 0;
    return outlen;
}

int decode_quad(char *enc, int enclen, int *out) {
    int outlen = 0;
    int i = 0;
    while (i < enclen) {
        int c0 = decode_char(enc[i]);
        int c1 = decode_char(enc[i + 1]);
        int c2 = (enc[i + 2] != '=') ? decode_char(enc[i + 2]) : 0;
        int c3 = (enc[i + 3] != '=') ? decode_char(enc[i + 3]) : 0;

        out[outlen] = (c0 << 2) | ((c1 >> 4) & 3);
        outlen = outlen + 1;

        if (enc[i + 2] != '=') {
            out[outlen] = ((c1 & 15) << 4) | ((c2 >> 2) & 15);
            outlen = outlen + 1;
        }
        if (enc[i + 3] != '=') {
            out[outlen] = ((c2 & 3) << 6) | c3;
            outlen = outlen + 1;
        }

        i = i + 4;
    }
    return outlen;
}

int main(void) {
    // EXPECT: Encode val 0 is A: 1
    printf("Encode val 0 is A: %d\n", encode_char(0) == 'A');
    // EXPECT: Encode val 25 is Z: 1
    printf("Encode val 25 is Z: %d\n", encode_char(25) == 'Z');
    // EXPECT: Encode val 26 is a: 1
    printf("Encode val 26 is a: %d\n", encode_char(26) == 'a');
    // EXPECT: Encode val 52 is 0: 1
    printf("Encode val 52 is 0: %d\n", encode_char(52) == '0');

    // EXPECT: Decode A: 0
    printf("Decode A: %d\n", decode_char('A'));
    // EXPECT: Decode Z: 25
    printf("Decode Z: %d\n", decode_char('Z'));
    // EXPECT: Decode a: 26
    printf("Decode a: %d\n", decode_char('a'));

    int data3[3];
    char encoded[64];
    int decoded[64];
    int elen;
    int dlen;

    data3[0] = 77; data3[1] = 97; data3[2] = 110;
    elen = encode_triplet(data3, 3, encoded);
    // EXPECT: Encoded Man: TWFu
    printf("Encoded Man: %s\n", encoded);
    // EXPECT: Encoded len: 4
    printf("Encoded len: %d\n", elen);

    dlen = decode_quad(encoded, elen, decoded);
    // EXPECT: Decoded len: 3
    printf("Decoded len: %d\n", dlen);
    // EXPECT: Decoded[0]: 77
    printf("Decoded[0]: %d\n", decoded[0]);
    // EXPECT: Decoded[1]: 97
    printf("Decoded[1]: %d\n", decoded[1]);
    // EXPECT: Decoded[2]: 110
    printf("Decoded[2]: %d\n", decoded[2]);

    int data1[1];
    data1[0] = 77;
    elen = encode_triplet(data1, 1, encoded);
    // EXPECT: Single byte encoded: TQ==
    printf("Single byte encoded: %s\n", encoded);

    int data2[2];
    data2[0] = 77; data2[1] = 97;
    elen = encode_triplet(data2, 2, encoded);
    // EXPECT: Two byte encoded: TWE=
    printf("Two byte encoded: %s\n", encoded);

    int data6[6];
    data6[0] = 72; data6[1] = 101; data6[2] = 108;
    data6[3] = 108; data6[4] = 111; data6[5] = 33;
    elen = encode_triplet(data6, 6, encoded);
    // EXPECT: Hello! encoded: SGVsbG8h
    printf("Hello! encoded: %s\n", encoded);
    // EXPECT: Hello! enc len: 8
    printf("Hello! enc len: %d\n", elen);

    dlen = decode_quad(encoded, elen, decoded);
    int match = 1;
    int i;
    for (i = 0; i < 6; i = i + 1) {
        if (decoded[i] != data6[i]) match = 0;
    }
    // EXPECT: Hello! roundtrip: 1
    printf("Hello! roundtrip: %d\n", match);

    return 0;
}
