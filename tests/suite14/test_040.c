int printf(const char *fmt, ...);

/* Checksum algorithms: Adler-32, Fletcher-16, Fletcher-32 */

/* Adler-32: s1 = 1 + sum of bytes, s2 = sum of s1 values, both mod 65521 */
struct adler32_state {
    int s1;
    int s2;
};

void adler32_init(struct adler32_state *st) {
    st->s1 = 1;
    st->s2 = 0;
}

void adler32_update(struct adler32_state *st, int *data, int len) {
    int i;
    for (i = 0; i < len; i++) {
        st->s1 = (st->s1 + data[i]) % 65521;
        st->s2 = (st->s2 + st->s1) % 65521;
    }
}

int adler32_finish(struct adler32_state *st) {
    return (st->s2 << 16) | st->s1;
}

int adler32(int *data, int len) {
    struct adler32_state st;
    adler32_init(&st);
    adler32_update(&st, data, len);
    return adler32_finish(&st);
}

/* Fletcher-16: two running sums of bytes, mod 255 */
struct fletcher16_state {
    int sum1;
    int sum2;
};

void fletcher16_init(struct fletcher16_state *st) {
    st->sum1 = 0;
    st->sum2 = 0;
}

void fletcher16_update(struct fletcher16_state *st, int *data, int len) {
    int i;
    for (i = 0; i < len; i++) {
        st->sum1 = (st->sum1 + data[i]) % 255;
        st->sum2 = (st->sum2 + st->sum1) % 255;
    }
}

int fletcher16_finish(struct fletcher16_state *st) {
    return (st->sum2 << 8) | st->sum1;
}

int fletcher16(int *data, int len) {
    struct fletcher16_state st;
    fletcher16_init(&st);
    fletcher16_update(&st, data, len);
    return fletcher16_finish(&st);
}

/* Simple internet checksum (16-bit one's complement sum) */
int inet_checksum(int *data, int len) {
    int sum = 0;
    int i;
    for (i = 0; i < len; i++) {
        sum = sum + data[i];
        /* Fold carries */
        while (sum > 0xFFFF) {
            sum = (sum & 0xFFFF) + (sum >> 16);
        }
    }
    return (~sum) & 0xFFFF;
}

/* Luhn checksum (used in credit card numbers) */
int luhn_checksum(int *digits, int len) {
    int sum = 0;
    int i;
    int alt = 0;
    for (i = len - 1; i >= 0; i--) {
        int d = digits[i];
        if (alt) {
            d = d * 2;
            if (d > 9) d = d - 9;
        }
        sum = sum + d;
        alt = 1 - alt;
    }
    return sum % 10;
}

int luhn_valid(int *digits, int len) {
    return luhn_checksum(digits, len) == 0;
}

/* XOR-fold: fold a wider checksum into n bits */
int xor_fold(int val, int target_bits) {
    int mask = (1 << target_bits) - 1;
    int result = 0;
    int v = val;
    while (v > 0) {
        result = result ^ (v & mask);
        v = v >> target_bits;
    }
    return result;
}

/* Parity bit computation */
int compute_parity(int *data, int len) {
    int parity = 0;
    int i;
    for (i = 0; i < len; i++) {
        int v = data[i];
        while (v > 0) {
            parity = parity ^ (v & 1);
            v = v >> 1;
        }
    }
    return parity;
}

int main(void) {
    /* Adler-32 of empty data */
    int empty[1];
    struct adler32_state ast;
    adler32_init(&ast);
    int a0 = adler32_finish(&ast);
    printf("adler empty: %d\n", a0);
    // EXPECT: adler empty: 1

    /* (s2=0 << 16) | s1=1 = 0x00010001... wait: (0 << 16) | 1 = 1. No: 65536? */
    /* s2=0, s1=1. result = (0 << 16) | 1 = 1. Let me recalculate. */
    /* Hmm, 0<<16 = 0, | 1 = 1. The expected should be 1. Let me fix. */

    /* Actually let me re-check: adler32_init sets s1=1, s2=0 */
    /* adler32_finish: (s2 << 16) | s1 = (0 << 16) | 1 = 1 */

    /* I made a mistake above. Let me recalculate. */
    printf("adler empty s1: %d\n", ast.s1);
    // EXPECT: adler empty s1: 1
    printf("adler empty s2: %d\n", ast.s2);
    // EXPECT: adler empty s2: 0

    /* Adler-32 of [1] */
    int d1[1];
    d1[0] = 1;
    int a1 = adler32(d1, 1);
    /* s1 = (1 + 1) % 65521 = 2, s2 = (0 + 2) % 65521 = 2 */
    /* result = (2 << 16) | 2 = 131074 */
    printf("adler [1]: %d\n", a1);
    // EXPECT: adler [1]: 131074

    /* Adler-32 of [1, 2, 3] */
    int d2[3];
    d2[0] = 1; d2[1] = 2; d2[2] = 3;
    int a2 = adler32(d2, 3);
    /* i=0: s1=(1+1)%65521=2, s2=(0+2)%65521=2 */
    /* i=1: s1=(2+2)%65521=4, s2=(2+4)%65521=6 */
    /* i=2: s1=(4+3)%65521=7, s2=(6+7)%65521=13 */
    /* result = (13 << 16) | 7 = 851975 */
    printf("adler [1,2,3]: %d\n", a2);
    // EXPECT: adler [1,2,3]: 851975

    /* Fletcher-16 of [1, 2] */
    int f1[2];
    f1[0] = 1; f1[1] = 2;
    int fl1 = fletcher16(f1, 2);
    /* i=0: sum1=(0+1)%255=1, sum2=(0+1)%255=1 */
    /* i=1: sum1=(1+2)%255=3, sum2=(1+3)%255=4 */
    /* result = (4 << 8) | 3 = 1027 */
    printf("fletcher [1,2]: %d\n", fl1);
    // EXPECT: fletcher [1,2]: 1027

    /* Fletcher-16 of [255, 255, 255] */
    int f2[3];
    f2[0] = 255; f2[1] = 255; f2[2] = 255;
    int fl2 = fletcher16(f2, 3);
    /* i=0: sum1=(0+255)%255=0, sum2=(0+0)%255=0 */
    /* i=1: sum1=(0+255)%255=0, sum2=(0+0)%255=0 */
    /* i=2: sum1=(0+255)%255=0, sum2=(0+0)%255=0 */
    /* result = (0 << 8) | 0 = 0 */
    printf("fletcher [255,255,255]: %d\n", fl2);
    // EXPECT: fletcher [255,255,255]: 0

    /* Fletcher-16 of [1, 2, 3, 4] */
    int f3[4];
    f3[0] = 1; f3[1] = 2; f3[2] = 3; f3[3] = 4;
    int fl3 = fletcher16(f3, 4);
    /* i=0: sum1=1, sum2=1 */
    /* i=1: sum1=3, sum2=4 */
    /* i=2: sum1=6, sum2=10 */
    /* i=3: sum1=10, sum2=20 */
    /* result = (20 << 8) | 10 = 5130 */
    printf("fletcher [1,2,3,4]: %d\n", fl3);
    // EXPECT: fletcher [1,2,3,4]: 5130

    /* Internet checksum */
    int id1[3];
    id1[0] = 0x0001; id1[1] = 0x00F2; id1[2] = 0x0003;
    int ic1 = inet_checksum(id1, 3);
    /* sum = 1 + 242 + 3 = 246. ~246 & 0xFFFF = 65289 */
    printf("inet checksum: %d\n", ic1);
    // EXPECT: inet checksum: 65289

    /* Luhn checksum */
    /* Valid: 7-9-9-2-7-3-9-8-7-1 (known valid luhn number) */
    /* Let's use a simpler example: digits 0 -> luhn = 0 (valid) */
    int ld1[1];
    ld1[0] = 0;
    printf("luhn [0]: %d\n", luhn_checksum(ld1, 1));
    // EXPECT: luhn [0]: 0
    printf("luhn valid [0]: %d\n", luhn_valid(ld1, 1));
    // EXPECT: luhn valid [0]: 1

    /* Luhn of [1, 8] */
    /* from right: i=1 (d=1, alt=0): sum+=1, alt=1; i=0 (d=8... wait, we go right to left */
    /* Wait, I start from i=len-1=1: d=8, alt=0, sum=8, alt=1 */
    /* i=0: d=1, alt=1, d=2, sum=10 */
    /* 10 % 10 = 0, valid! */
    int ld2[2];
    ld2[0] = 1; ld2[1] = 8;
    printf("luhn [1,8]: %d\n", luhn_checksum(ld2, 2));
    // EXPECT: luhn [1,8]: 0
    printf("luhn valid [1,8]: %d\n", luhn_valid(ld2, 2));
    // EXPECT: luhn valid [1,8]: 1

    /* Luhn of [1, 2, 3] */
    /* i=2: d=3, alt=0, sum=3, alt=1 */
    /* i=1: d=2, alt=1, d=4, sum=7, alt=0 */
    /* i=0: d=1, alt=0, sum=8, alt=1 */
    /* 8 % 10 = 8 */
    int ld3[3];
    ld3[0] = 1; ld3[1] = 2; ld3[2] = 3;
    printf("luhn [1,2,3]: %d\n", luhn_checksum(ld3, 3));
    // EXPECT: luhn [1,2,3]: 8

    /* XOR fold: fold 0xABCD into 8 bits */
    int xf1 = xor_fold(0xABCD, 8);
    /* 0xABCD: low 8 = 0xCD = 205, high 8 = 0xAB = 171 */
    /* 205 ^ 171 = 0b11001101 ^ 0b10101011 = 0b01100110 = 102 */
    printf("xor fold: %d\n", xf1);
    // EXPECT: xor fold: 102

    /* XOR fold 0xFF into 4 bits */
    int xf2 = xor_fold(0xFF, 4);
    /* 0xFF: low 4 = 15, next 4 = 15 */
    /* 15 ^ 15 = 0 */
    printf("xor fold 4: %d\n", xf2);
    // EXPECT: xor fold 4: 0

    /* Parity */
    int pd1[3];
    pd1[0] = 1; pd1[1] = 2; pd1[2] = 3;
    /* 1=01, 2=10, 3=11 -> total 1s: 1+1+2 = 4 -> even parity = 0 */
    printf("parity [1,2,3]: %d\n", compute_parity(pd1, 3));
    // EXPECT: parity [1,2,3]: 0

    int pd2[2];
    pd2[0] = 7; pd2[1] = 1;
    /* 7=111 (3 ones), 1=1 (1 one). Total = 4, parity = 0 */
    printf("parity [7,1]: %d\n", compute_parity(pd2, 2));
    // EXPECT: parity [7,1]: 0

    int pd3[1];
    pd3[0] = 5;
    /* 5=101, 2 ones, parity = 0 */
    printf("parity [5]: %d\n", compute_parity(pd3, 1));
    // EXPECT: parity [5]: 0

    int pd4[1];
    pd4[0] = 3;
    /* 3=11, 2 ones, parity = 0 */
    printf("parity [3]: %d\n", compute_parity(pd4, 1));
    // EXPECT: parity [3]: 0

    int pd5[2];
    pd5[0] = 1; pd5[1] = 1;
    /* 1 + 1 = 2 ones, parity = 0 */
    printf("parity [1,1]: %d\n", compute_parity(pd5, 2));
    // EXPECT: parity [1,1]: 0

    int pd6[1];
    pd6[0] = 7;
    /* 7=111, 3 ones, parity = 1 */
    printf("parity [7]: %d\n", compute_parity(pd6, 1));
    // EXPECT: parity [7]: 1

    printf("done\n");
    // EXPECT: done
    return 0;
}
