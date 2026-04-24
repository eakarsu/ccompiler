int printf(const char *fmt, ...);

/* Variable-length integer encoding (like LEB128) */
/* Unsigned LEB128: 7 bits per byte, high bit = continuation */

int uleb128_encode(int value, int *buf) {
    int nbytes = 0;
    int v = value;
    do {
        int byte = v & 0x7F;
        v = v >> 7;
        if (v != 0) {
            byte = byte | 0x80;  /* set continuation bit */
        }
        buf[nbytes] = byte;
        nbytes = nbytes + 1;
    } while (v != 0);
    return nbytes;
}

int uleb128_decode(int *buf, int *result) {
    int value = 0;
    int shift = 0;
    int nbytes = 0;
    int more = 1;
    while (more) {
        int byte = buf[nbytes];
        value = value | ((byte & 0x7F) << shift);
        shift = shift + 7;
        nbytes = nbytes + 1;
        if ((byte & 0x80) == 0) more = 0;
    }
    *result = value;
    return nbytes;
}

/* Signed LEB128 encoding */
int sleb128_encode(int value, int *buf) {
    int nbytes = 0;
    int more = 1;
    int v = value;
    while (more) {
        int byte = v & 0x7F;
        v = v >> 7;
        /* For signed, check if sign bit of byte matches remaining value */
        if ((v == 0 && (byte & 0x40) == 0) || (v == -1 && (byte & 0x40) != 0)) {
            more = 0;
        } else {
            byte = byte | 0x80;
        }
        buf[nbytes] = byte;
        nbytes = nbytes + 1;
    }
    return nbytes;
}

int sleb128_decode(int *buf, int *result) {
    int value = 0;
    int shift = 0;
    int nbytes = 0;
    int byte;
    do {
        byte = buf[nbytes];
        value = value | ((byte & 0x7F) << shift);
        shift = shift + 7;
        nbytes = nbytes + 1;
    } while (byte & 0x80);
    /* Sign extend */
    if (shift < 32 && (byte & 0x40)) {
        value = value | ((-1) << shift);
    }
    *result = value;
    return nbytes;
}

/* VarInt encoding (protobuf-style): zigzag encoding for signed values */
int zigzag_encode(int n) {
    return (n << 1) ^ (n >> 31);
}

int zigzag_decode(int n) {
    return (n >> 1) ^ (-(n & 1));
}

/* Count bytes needed for a value in ULEB128 */
int uleb128_size(int value) {
    int buf[8];
    return uleb128_encode(value, buf);
}

/* Encode multiple values and return total bytes used */
int encode_sequence(int *values, int nvals, int *buf) {
    int total = 0;
    int i;
    for (i = 0; i < nvals; i++) {
        int n = uleb128_encode(values[i], buf + total);
        total = total + n;
    }
    return total;
}

/* Decode multiple values */
int decode_sequence(int *buf, int total_bytes, int *values) {
    int pos = 0;
    int nvals = 0;
    while (pos < total_bytes) {
        int val;
        int n = uleb128_decode(buf + pos, &val);
        values[nvals] = val;
        nvals = nvals + 1;
        pos = pos + n;
    }
    return nvals;
}

int main(void) {
    int buf[16];
    int result;

    /* ULEB128 encode small values */
    int n1 = uleb128_encode(0, buf);
    printf("encode 0: %d bytes, val=%d\n", n1, buf[0]);
    // EXPECT: encode 0: 1 bytes, val=0

    int n2 = uleb128_encode(127, buf);
    printf("encode 127: %d bytes, val=%d\n", n2, buf[0]);
    // EXPECT: encode 127: 1 bytes, val=127

    int n3 = uleb128_encode(128, buf);
    printf("encode 128: %d bytes, b0=%d b1=%d\n", n3, buf[0], buf[1]);
    // EXPECT: encode 128: 2 bytes, b0=128 b1=1

    /* 128 = 0b10000000. LEB: low 7 bits = 0 with continuation = 0x80, then 1 */

    int n4 = uleb128_encode(300, buf);
    printf("encode 300: %d bytes, b0=%d b1=%d\n", n4, buf[0], buf[1]);
    // EXPECT: encode 300: 2 bytes, b0=172 b1=2

    /* 300 = 0b100101100. Low 7: 0101100=44, with cont: 44|128=172. High: 10=2 */

    int n5 = uleb128_encode(16384, buf);
    printf("encode 16384: %d bytes\n", n5);
    // EXPECT: encode 16384: 3 bytes

    /* Decode tests */
    uleb128_encode(0, buf);
    uleb128_decode(buf, &result);
    printf("decode 0: %d\n", result);
    // EXPECT: decode 0: 0

    uleb128_encode(127, buf);
    uleb128_decode(buf, &result);
    printf("decode 127: %d\n", result);
    // EXPECT: decode 127: 127

    uleb128_encode(128, buf);
    uleb128_decode(buf, &result);
    printf("decode 128: %d\n", result);
    // EXPECT: decode 128: 128

    uleb128_encode(300, buf);
    uleb128_decode(buf, &result);
    printf("decode 300: %d\n", result);
    // EXPECT: decode 300: 300

    uleb128_encode(100000, buf);
    uleb128_decode(buf, &result);
    printf("decode 100000: %d\n", result);
    // EXPECT: decode 100000: 100000

    /* Signed LEB128 */
    int sn1 = sleb128_encode(-1, buf);
    printf("sleb -1: %d bytes, val=%d\n", sn1, buf[0]);
    // EXPECT: sleb -1: 1 bytes, val=127

    /* -1 = all 1s. Low 7 bits = 0x7F = 127, sign bit (bit 6) set, v>>7 = -1, matches */

    int sn2 = sleb128_encode(-128, buf);
    printf("sleb -128: %d bytes, b0=%d b1=%d\n", sn2, buf[0], buf[1]);
    // EXPECT: sleb -128: 2 bytes, b0=128 b1=127

    /* -128: low 7 bits of -128 = 0 (since -128 & 0x7F = 0), cont bit set -> 0x80=128 */
    /* v >>= 7: -128 >> 7 = -1. -1 & 0x7F = 127, bit 6 set, v>>7=-1, matches -> done. b1=127 */

    sleb128_decode(buf, &result);
    printf("sleb decode -128: %d\n", result);
    // EXPECT: sleb decode -128: -128

    sleb128_encode(64, buf);
    sleb128_decode(buf, &result);
    printf("sleb roundtrip 64: %d\n", result);
    // EXPECT: sleb roundtrip 64: 64

    sleb128_encode(-1000, buf);
    sleb128_decode(buf, &result);
    printf("sleb roundtrip -1000: %d\n", result);
    // EXPECT: sleb roundtrip -1000: -1000

    /* Zigzag encoding */
    printf("zigzag 0: %d\n", zigzag_encode(0));
    // EXPECT: zigzag 0: 0
    printf("zigzag -1: %d\n", zigzag_encode(-1));
    // EXPECT: zigzag -1: 1
    printf("zigzag 1: %d\n", zigzag_encode(1));
    // EXPECT: zigzag 1: 2
    printf("zigzag -2: %d\n", zigzag_encode(-2));
    // EXPECT: zigzag -2: 3
    printf("zigzag 2: %d\n", zigzag_encode(2));
    // EXPECT: zigzag 2: 4

    /* Zigzag decode */
    printf("unzigzag 0: %d\n", zigzag_decode(0));
    // EXPECT: unzigzag 0: 0
    printf("unzigzag 1: %d\n", zigzag_decode(1));
    // EXPECT: unzigzag 1: -1
    printf("unzigzag 4: %d\n", zigzag_decode(4));
    // EXPECT: unzigzag 4: 2

    /* Sequence encode/decode */
    int vals[4];
    vals[0] = 1; vals[1] = 300; vals[2] = 0; vals[3] = 127;
    int seqbuf[32];
    int total = encode_sequence(vals, 4, seqbuf);
    printf("seq total bytes: %d\n", total);
    // EXPECT: seq total bytes: 5

    /* 1=1byte, 300=2bytes, 0=1byte, 127=1byte = 5 */

    int decoded_vals[8];
    int nvals = decode_sequence(seqbuf, total, decoded_vals);
    printf("seq decoded: %d vals\n", nvals);
    // EXPECT: seq decoded: 4 vals
    printf("seq: %d %d %d %d\n", decoded_vals[0], decoded_vals[1], decoded_vals[2], decoded_vals[3]);
    // EXPECT: seq: 1 300 0 127

    /* Size calculations */
    printf("size 0: %d\n", uleb128_size(0));
    // EXPECT: size 0: 1
    printf("size 127: %d\n", uleb128_size(127));
    // EXPECT: size 127: 1
    printf("size 128: %d\n", uleb128_size(128));
    // EXPECT: size 128: 2
    printf("size 16383: %d\n", uleb128_size(16383));
    // EXPECT: size 16383: 2
    printf("size 16384: %d\n", uleb128_size(16384));
    // EXPECT: size 16384: 3

    printf("done\n");
    // EXPECT: done
    return 0;
}
