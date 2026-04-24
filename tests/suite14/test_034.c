int printf(const char *fmt, ...);

/* LZ77-style sliding window compression */

struct lz77_token {
    int offset;   /* back-reference offset (0 = literal) */
    int length;   /* match length */
    int literal;  /* the next literal char (-1 if end of data) */
};

/* Find longest match in the sliding window, allowing self-referencing */
void find_match(int *data, int pos, int len, int win, int *out_off, int *out_len) {
    int best_off = 0;
    int best_len = 0;
    int start = pos - win;
    if (start < 0) start = 0;
    int i;
    for (i = start; i < pos; i++) {
        int ml = 0;
        /* Self-referencing: copy byte-by-byte, src wraps around */
        while (pos + ml < len) {
            int src_idx = i + (ml % (pos - i));
            if (data[src_idx] != data[pos + ml]) break;
            ml = ml + 1;
        }
        if (ml > best_len) {
            best_len = ml;
            best_off = pos - i;
        }
    }
    if (best_len < 2) {
        *out_off = 0;
        *out_len = 0;
    } else {
        *out_off = best_off;
        *out_len = best_len;
    }
}

/* Compress: emit (offset, length, literal) tokens */
int lz77_compress(int *data, int len, struct lz77_token *tokens, int win) {
    int pos = 0;
    int nt = 0;
    while (pos < len) {
        int off;
        int ml;
        find_match(data, pos, len, win, &off, &ml);
        if (ml == 0) {
            tokens[nt].offset = 0;
            tokens[nt].length = 0;
            tokens[nt].literal = data[pos];
            nt = nt + 1;
            pos = pos + 1;
        } else {
            int lit = -1;
            if (pos + ml < len) {
                lit = data[pos + ml];
            }
            tokens[nt].offset = off;
            tokens[nt].length = ml;
            tokens[nt].literal = lit;
            nt = nt + 1;
            pos = pos + ml;
            if (lit >= 0) pos = pos + 1;
        }
    }
    return nt;
}

/* Decompress with self-referencing support */
int lz77_decompress(struct lz77_token *tokens, int nt, int *out) {
    int pos = 0;
    int i;
    int j;
    for (i = 0; i < nt; i++) {
        if (tokens[i].offset == 0 && tokens[i].length == 0) {
            out[pos] = tokens[i].literal;
            pos = pos + 1;
        } else {
            int src = pos - tokens[i].offset;
            for (j = 0; j < tokens[i].length; j++) {
                out[pos] = out[src + j];
                pos = pos + 1;
            }
            if (tokens[i].literal >= 0) {
                out[pos] = tokens[i].literal;
                pos = pos + 1;
            }
        }
    }
    return pos;
}

int arrays_equal(int *a, int *b, int len) {
    int i;
    for (i = 0; i < len; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int main(void) {
    /* Test 1: no repeats [10,20,30,40,50] */
    int data1[5];
    data1[0] = 10; data1[1] = 20; data1[2] = 30; data1[3] = 40; data1[4] = 50;

    struct lz77_token tok1[20];
    int nt1 = lz77_compress(data1, 5, tok1, 8);
    printf("norep tokens: %d\n", nt1);
    // EXPECT: norep tokens: 5

    int dec1[20];
    int dl1 = lz77_decompress(tok1, nt1, dec1);
    printf("norep dec len: %d\n", dl1);
    // EXPECT: norep dec len: 5
    printf("norep match: %d\n", arrays_equal(data1, dec1, 5));
    // EXPECT: norep match: 1

    /* Test 2: repeated pattern [1,2,3,1,2,3] */
    int data2[6];
    data2[0] = 1; data2[1] = 2; data2[2] = 3;
    data2[3] = 1; data2[4] = 2; data2[5] = 3;

    struct lz77_token tok2[20];
    int nt2 = lz77_compress(data2, 6, tok2, 8);
    printf("repeat tokens: %d\n", nt2);
    // EXPECT: repeat tokens: 4

    printf("r0: off=%d len=%d lit=%d\n", tok2[0].offset, tok2[0].length, tok2[0].literal);
    // EXPECT: r0: off=0 len=0 lit=1
    printf("r1: off=%d len=%d lit=%d\n", tok2[1].offset, tok2[1].length, tok2[1].literal);
    // EXPECT: r1: off=0 len=0 lit=2
    printf("r2: off=%d len=%d lit=%d\n", tok2[2].offset, tok2[2].length, tok2[2].literal);
    // EXPECT: r2: off=0 len=0 lit=3
    printf("r3: off=%d len=%d lit=%d\n", tok2[3].offset, tok2[3].length, tok2[3].literal);
    // EXPECT: r3: off=3 len=3 lit=-1

    int dec2[20];
    int dl2 = lz77_decompress(tok2, nt2, dec2);
    printf("repeat dec len: %d\n", dl2);
    // EXPECT: repeat dec len: 6
    printf("repeat match: %d\n", arrays_equal(data2, dec2, 6));
    // EXPECT: repeat match: 1

    /* Test 3: all same [5,5,5,5,5,5,5,5] with self-reference */
    int data3[8];
    int i;
    for (i = 0; i < 8; i++) data3[i] = 5;

    struct lz77_token tok3[20];
    int nt3 = lz77_compress(data3, 8, tok3, 8);
    printf("same tokens: %d\n", nt3);
    // EXPECT: same tokens: 2

    /* t0: literal 5, t1: self-ref match (off=1, len=7, lit=-1) */
    printf("s0: off=%d len=%d lit=%d\n", tok3[0].offset, tok3[0].length, tok3[0].literal);
    // EXPECT: s0: off=0 len=0 lit=5
    printf("s1: off=%d len=%d lit=%d\n", tok3[1].offset, tok3[1].length, tok3[1].literal);
    // EXPECT: s1: off=1 len=7 lit=-1

    int dec3[20];
    int dl3 = lz77_decompress(tok3, nt3, dec3);
    printf("same dec len: %d\n", dl3);
    // EXPECT: same dec len: 8
    int all5 = 1;
    for (i = 0; i < 8; i++) {
        if (dec3[i] != 5) all5 = 0;
    }
    printf("all fives: %d\n", all5);
    // EXPECT: all fives: 1

    /* Test 4: ABCABCABC [1,2,3,1,2,3,1,2,3] */
    int data4[9];
    data4[0] = 1; data4[1] = 2; data4[2] = 3;
    data4[3] = 1; data4[4] = 2; data4[5] = 3;
    data4[6] = 1; data4[7] = 2; data4[8] = 3;

    struct lz77_token tok4[20];
    int nt4 = lz77_compress(data4, 9, tok4, 8);
    printf("abc3 tokens: %d\n", nt4);
    // EXPECT: abc3 tokens: 4

    int dec4[20];
    int dl4 = lz77_decompress(tok4, nt4, dec4);
    printf("abc3 dec len: %d\n", dl4);
    // EXPECT: abc3 dec len: 9
    printf("abc3 match: %d\n", arrays_equal(data4, dec4, 9));
    // EXPECT: abc3 match: 1

    /* Test 5: AABABC [1,1,2,1,2,3] */
    int data5[6];
    data5[0] = 1; data5[1] = 1; data5[2] = 2;
    data5[3] = 1; data5[4] = 2; data5[5] = 3;

    struct lz77_token tok5[20];
    int nt5 = lz77_compress(data5, 6, tok5, 8);
    printf("aababc tokens: %d\n", nt5);
    // EXPECT: aababc tokens: 4

    int dec5[20];
    int dl5 = lz77_decompress(tok5, nt5, dec5);
    printf("aababc match: %d\n", arrays_equal(data5, dec5, 6));
    // EXPECT: aababc match: 1

    /* Test 6: compression ratio */
    printf("norep: %d tok for %d vals\n", nt1, 5);
    // EXPECT: norep: 5 tok for 5 vals
    printf("same: %d tok for %d vals\n", nt3, 8);
    // EXPECT: same: 2 tok for 8 vals
    printf("repeat: %d tok for %d vals\n", nt2, 6);
    // EXPECT: repeat: 4 tok for 6 vals

    printf("done\n");
    // EXPECT: done
    return 0;
}
