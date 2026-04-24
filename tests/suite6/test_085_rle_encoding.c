int printf(const char *fmt, ...);

int rle_encode(int *in, int inlen, int *out) {
    int oi = 0;
    int i = 0;
    while (i < inlen) {
        int val = in[i];
        int count = 1;
        while (i + count < inlen && in[i + count] == val && count < 255) {
            count = count + 1;
        }
        out[oi] = val;
        out[oi + 1] = count;
        oi = oi + 2;
        i = i + count;
    }
    return oi;
}

int rle_decode(int *in, int inlen, int *out) {
    int oi = 0;
    int i = 0;
    while (i < inlen) {
        int val = in[i];
        int count = in[i + 1];
        int j;
        for (j = 0; j < count; j = j + 1) {
            out[oi] = val;
            oi = oi + 1;
        }
        i = i + 2;
    }
    return oi;
}

int arrays_equal(int *a, int *b, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int main(void) {
    int data[32];
    int encoded[64];
    int decoded[64];
    int elen;
    int dlen;
    int i;

    data[0] = 1; data[1] = 1; data[2] = 1; data[3] = 2;
    data[4] = 2; data[5] = 3; data[6] = 3; data[7] = 3;
    data[8] = 3; data[9] = 0; data[10] = 0; data[11] = 0;

    elen = rle_encode(data, 9, encoded);
    // EXPECT: Encoded len: 6
    printf("Encoded len: %d\n", elen);

    // EXPECT: Pair 0: val=1 count=3
    printf("Pair 0: val=%d count=%d\n", encoded[0], encoded[1]);
    // EXPECT: Pair 1: val=2 count=2
    printf("Pair 1: val=%d count=%d\n", encoded[2], encoded[3]);
    // EXPECT: Pair 2: val=3 count=4
    printf("Pair 2: val=%d count=%d\n", encoded[4], encoded[5]);

    dlen = rle_decode(encoded, elen, decoded);
    // EXPECT: Decoded len: 9
    printf("Decoded len: %d\n", dlen);

    // EXPECT: Roundtrip OK: 1
    printf("Roundtrip OK: %d\n", arrays_equal(data, decoded, 9));

    int norun[5];
    norun[0] = 1; norun[1] = 2; norun[2] = 3; norun[3] = 4; norun[4] = 5;
    elen = rle_encode(norun, 5, encoded);
    // EXPECT: No runs encoded len: 10
    printf("No runs encoded len: %d\n", elen);

    dlen = rle_decode(encoded, elen, decoded);
    // EXPECT: No runs roundtrip: 1
    printf("No runs roundtrip: %d\n", arrays_equal(norun, decoded, 5));

    int allsame[8];
    for (i = 0; i < 8; i = i + 1) allsame[i] = 7;
    elen = rle_encode(allsame, 8, encoded);
    // EXPECT: All same encoded len: 2
    printf("All same encoded len: %d\n", elen);
    // EXPECT: All same: val=7 count=8
    printf("All same: val=%d count=%d\n", encoded[0], encoded[1]);

    dlen = rle_decode(encoded, elen, decoded);
    // EXPECT: All same roundtrip: 1
    printf("All same roundtrip: %d\n", arrays_equal(allsame, decoded, 8));

    int single[1];
    single[0] = 42;
    elen = rle_encode(single, 1, encoded);
    // EXPECT: Single elem len: 2
    printf("Single elem len: %d\n", elen);
    // EXPECT: Single: val=42 count=1
    printf("Single: val=%d count=%d\n", encoded[0], encoded[1]);

    int alt[6];
    alt[0] = 1; alt[1] = 0; alt[2] = 1; alt[3] = 0; alt[4] = 1; alt[5] = 0;
    elen = rle_encode(alt, 6, encoded);
    // EXPECT: Alternating encoded len: 12
    printf("Alternating encoded len: %d\n", elen);

    dlen = rle_decode(encoded, elen, decoded);
    // EXPECT: Alternating roundtrip: 1
    printf("Alternating roundtrip: %d\n", arrays_equal(alt, decoded, 6));

    int compressed = (elen < 6 * 2) ? 1 : 0;
    // EXPECT: Alternating compressed: 0
    printf("Alternating compressed: %d\n", compressed);

    int runs[12];
    for (i = 0; i < 4; i = i + 1) runs[i] = 10;
    for (i = 4; i < 8; i = i + 1) runs[i] = 20;
    for (i = 8; i < 12; i = i + 1) runs[i] = 30;
    elen = rle_encode(runs, 12, encoded);
    // EXPECT: Three runs len: 6
    printf("Three runs len: %d\n", elen);

    return 0;
}
