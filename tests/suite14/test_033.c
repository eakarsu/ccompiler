int printf(const char *fmt, ...);

/* Run-length encoding with bit packing */

struct rle_pair {
    int value;
    int count;
};

int rle_encode(int *data, int len, struct rle_pair *out) {
    if (len == 0) return 0;
    int oi = 0;
    int cur = data[0];
    int count = 1;
    int i;
    for (i = 1; i < len; i++) {
        if (data[i] == cur && count < 255) {
            count = count + 1;
        } else {
            out[oi].value = cur;
            out[oi].count = count;
            oi = oi + 1;
            cur = data[i];
            count = 1;
        }
    }
    out[oi].value = cur;
    out[oi].count = count;
    oi = oi + 1;
    return oi;
}

int rle_decode(struct rle_pair *pairs, int npairs, int *out) {
    int oi = 0;
    int i;
    int j;
    for (i = 0; i < npairs; i++) {
        for (j = 0; j < pairs[i].count; j++) {
            out[oi] = pairs[i].value;
            oi = oi + 1;
        }
    }
    return oi;
}

/* Pack an RLE pair into a single int: high 8 bits = count, low 8 bits = value */
int rle_pack(struct rle_pair *p) {
    return (p->count << 8) | (p->value & 0xFF);
}

void rle_unpack(int packed, struct rle_pair *p) {
    p->value = packed & 0xFF;
    p->count = (packed >> 8) & 0xFF;
}

/* Bit-level RLE: count runs of 0s and 1s in an integer */
int bit_rle_encode(int val, int nbits, int *runs) {
    int nruns = 0;
    int current_bit = (val >> (nbits - 1)) & 1;
    int count = 0;
    int i;
    for (i = nbits - 1; i >= 0; i--) {
        int bit = (val >> i) & 1;
        if (bit == current_bit) {
            count = count + 1;
        } else {
            runs[nruns] = count;
            nruns = nruns + 1;
            current_bit = bit;
            count = 1;
        }
    }
    runs[nruns] = count;
    nruns = nruns + 1;
    return nruns;
}

int bit_rle_decode(int start_bit, int *runs, int nruns) {
    int val = 0;
    int bit = start_bit;
    int i;
    int j;
    for (i = 0; i < nruns; i++) {
        for (j = 0; j < runs[i]; j++) {
            val = (val << 1) | bit;
        }
        bit = 1 - bit;
    }
    return val;
}

int compression_ratio(int orig_size, int compressed_size) {
    if (orig_size == 0) return 0;
    return (compressed_size * 100) / orig_size;
}

int main(void) {
    /* Data: 1,1,1,2,2,3,3,3,3,1,1,1 */
    /* Runs: (1,3), (2,2), (3,4), (1,3) = 4 pairs */
    int data1[12];
    data1[0] = 1; data1[1] = 1; data1[2] = 1;
    data1[3] = 2; data1[4] = 2;
    data1[5] = 3; data1[6] = 3; data1[7] = 3; data1[8] = 3;
    data1[9] = 1; data1[10] = 1; data1[11] = 1;

    struct rle_pair pairs1[20];
    int npairs = rle_encode(data1, 12, pairs1);
    printf("pairs: %d\n", npairs);
    // EXPECT: pairs: 4

    printf("p0: v=%d c=%d\n", pairs1[0].value, pairs1[0].count);
    // EXPECT: p0: v=1 c=3
    printf("p1: v=%d c=%d\n", pairs1[1].value, pairs1[1].count);
    // EXPECT: p1: v=2 c=2
    printf("p2: v=%d c=%d\n", pairs1[2].value, pairs1[2].count);
    // EXPECT: p2: v=3 c=4
    printf("p3: v=%d c=%d\n", pairs1[3].value, pairs1[3].count);
    // EXPECT: p3: v=1 c=3

    /* Decode and verify roundtrip */
    int decoded1[20];
    int dlen = rle_decode(pairs1, npairs, decoded1);
    printf("decoded len: %d\n", dlen);
    // EXPECT: decoded len: 12

    int match = 1;
    int k;
    for (k = 0; k < 12; k++) {
        if (data1[k] != decoded1[k]) match = 0;
    }
    printf("roundtrip ok: %d\n", match);
    // EXPECT: roundtrip ok: 1

    /* Pack/unpack test: pair0 = (value=1, count=3) */
    int packed = rle_pack(&pairs1[0]);
    printf("packed: %d\n", packed);
    // EXPECT: packed: 769
    /* 3 << 8 | 1 = 768 + 1 = 769 */

    struct rle_pair unpacked;
    rle_unpack(packed, &unpacked);
    printf("unpacked: v=%d c=%d\n", unpacked.value, unpacked.count);
    // EXPECT: unpacked: v=1 c=3

    /* All same values - best case for RLE */
    int data2[8];
    int i;
    for (i = 0; i < 8; i++) data2[i] = 42;
    struct rle_pair pairs2[20];
    int np2 = rle_encode(data2, 8, pairs2);
    printf("same vals pairs: %d\n", np2);
    // EXPECT: same vals pairs: 1
    printf("same vals: v=%d c=%d\n", pairs2[0].value, pairs2[0].count);
    // EXPECT: same vals: v=42 c=8

    /* All different - worst case */
    int data3[5];
    data3[0] = 10; data3[1] = 20; data3[2] = 30; data3[3] = 40; data3[4] = 50;
    struct rle_pair pairs3[20];
    int np3 = rle_encode(data3, 5, pairs3);
    printf("all diff pairs: %d\n", np3);
    // EXPECT: all diff pairs: 5

    /* Alternating pattern */
    int data4[6];
    data4[0] = 1; data4[1] = 2; data4[2] = 1; data4[3] = 2; data4[4] = 1; data4[5] = 2;
    struct rle_pair pairs4[20];
    int np4 = rle_encode(data4, 6, pairs4);
    printf("alternating pairs: %d\n", np4);
    // EXPECT: alternating pairs: 6

    /* Bit-level RLE: 0b10110000 = 176 in 8 bits */
    /* Runs: 1(1), 0(1), 1(2), 0(4) => 4 runs */
    int runs[20];
    int nruns = bit_rle_encode(176, 8, runs);
    printf("bit runs: %d\n", nruns);
    // EXPECT: bit runs: 4
    printf("r0=%d r1=%d r2=%d r3=%d\n", runs[0], runs[1], runs[2], runs[3]);
    // EXPECT: r0=1 r1=1 r2=2 r3=4

    /* Decode bit RLE: starts with 1 */
    int restored = bit_rle_decode(1, runs, nruns);
    printf("bit restored: %d\n", restored);
    // EXPECT: bit restored: 176

    /* Bit RLE of 0xFF = 11111111: single run of 8 ones */
    int runs2[20];
    int nr2 = bit_rle_encode(255, 8, runs2);
    printf("0xFF runs: %d run0=%d\n", nr2, runs2[0]);
    // EXPECT: 0xFF runs: 1 run0=8

    /* Bit RLE of 0xAA = 10101010: 8 runs of length 1 */
    int runs3[20];
    int nr3 = bit_rle_encode(170, 8, runs3);
    printf("0xAA runs: %d\n", nr3);
    // EXPECT: 0xAA runs: 8

    /* Bit RLE of 0x00 = 00000000: single run of 8 zeros */
    int runs4[20];
    int nr4 = bit_rle_encode(0, 8, runs4);
    printf("0x00 runs: %d run0=%d\n", nr4, runs4[0]);
    // EXPECT: 0x00 runs: 1 run0=8

    /* Compression ratio */
    int cr1 = compression_ratio(12, 4);
    printf("ratio 12->4: %d\n", cr1);
    // EXPECT: ratio 12->4: 33

    int cr2 = compression_ratio(8, 1);
    printf("ratio 8->1: %d\n", cr2);
    // EXPECT: ratio 8->1: 12

    int cr3 = compression_ratio(5, 5);
    printf("ratio 5->5: %d\n", cr3);
    // EXPECT: ratio 5->5: 100

    printf("done\n");
    // EXPECT: done
    return 0;
}
