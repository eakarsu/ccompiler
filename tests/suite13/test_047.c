int printf(const char *fmt, ...);

// Bitwise rotation (left and right) on n-bit values

// Rotate left by r positions within an n-bit field
int rotate_left(int val, int r, int nbits) {
    int mask = (1 << nbits) - 1;
    val = val & mask;
    r = r % nbits;
    if (r == 0) return val;
    int result = ((val << r) | ((val >> (nbits - r)) & ((1 << r) - 1))) & mask;
    return result;
}

// Rotate right by r positions within an n-bit field
int rotate_right(int val, int r, int nbits) {
    int mask = (1 << nbits) - 1;
    val = val & mask;
    r = r % nbits;
    if (r == 0) return val;
    int lo = val & ((1 << r) - 1);
    int hi = (val >> r) & ((1 << (nbits - r)) - 1);
    int result = (lo << (nbits - r)) | hi;
    return result;
}

// Rotate a byte (8 bits)
int rotate_byte_left(int val, int r) {
    return rotate_left(val, r, 8);
}

int rotate_byte_right(int val, int r) {
    return rotate_right(val, r, 8);
}

// Circular shift register simulation
struct ShiftReg {
    int state;
    int nbits;
    int taps[4];
    int ntaps;
};

void sr_init(struct ShiftReg *sr, int init, int nbits) {
    sr->state = init & ((1 << nbits) - 1);
    sr->nbits = nbits;
    sr->ntaps = 0;
}

void sr_add_tap(struct ShiftReg *sr, int pos) {
    if (sr->ntaps < 4) {
        sr->taps[sr->ntaps] = pos;
        sr->ntaps = sr->ntaps + 1;
    }
}

// Step the LFSR: feedback = XOR of tapped bits, shift left, insert feedback at bit 0
int sr_step(struct ShiftReg *sr) {
    int feedback = 0;
    int i = 0;
    while (i < sr->ntaps) {
        feedback = feedback ^ ((sr->state >> sr->taps[i]) & 1);
        i = i + 1;
    }
    int mask = (1 << sr->nbits) - 1;
    sr->state = ((sr->state << 1) | feedback) & mask;
    return sr->state;
}

// Count the period of a shift register (how many steps until it repeats)
int sr_period(struct ShiftReg *sr) {
    int init = sr->state;
    int count = 0;
    int limit = (1 << sr->nbits);
    int s;
    do {
        s = sr_step(sr);
        count = count + 1;
    } while (s != init && count < limit);
    return count;
}

// Triple rotation: rotate left, then right, then left
int triple_rotate(int val, int r1, int r2, int r3, int nbits) {
    int v = rotate_left(val, r1, nbits);
    v = rotate_right(v, r2, nbits);
    v = rotate_left(v, r3, nbits);
    return v;
}

// Barrel shift: emulate a barrel shifter with select bits
int barrel_shift(int val, int amount, int nbits) {
    int result = val & ((1 << nbits) - 1);
    int shift = amount % nbits;
    int bit = 0;
    // Process shift amount bit by bit
    while (bit < nbits) {
        if (shift & (1 << bit)) {
            result = rotate_left(result, 1 << bit, nbits);
        }
        bit = bit + 1;
    }
    return result;
}

int main() {
    // rotate_left 8-bit
    // 1 = 00000001 rotated left by 1 = 00000010 = 2
    printf("rl(1,1,8)=%d\n", rotate_left(1, 1, 8));
    // EXPECT: rl(1,1,8)=2
    // 128 = 10000000 rotated left by 1 = 00000001 = 1
    printf("rl(128,1,8)=%d\n", rotate_left(128, 1, 8));
    // EXPECT: rl(128,1,8)=1
    // 0xA5 = 10100101 rotated left by 4 = 01011010 = 0x5A = 90
    printf("rl(165,4,8)=%d\n", rotate_left(165, 4, 8));
    // EXPECT: rl(165,4,8)=90
    // 255 rotated by anything = 255
    printf("rl(255,3,8)=%d\n", rotate_left(255, 3, 8));
    // EXPECT: rl(255,3,8)=255
    // 0 rotated = 0
    printf("rl(0,5,8)=%d\n", rotate_left(0, 5, 8));
    // EXPECT: rl(0,5,8)=0

    // rotate_right 8-bit
    // 1 = 00000001 rotated right by 1 = 10000000 = 128
    printf("rr(1,1,8)=%d\n", rotate_right(1, 1, 8));
    // EXPECT: rr(1,1,8)=128
    // 2 = 00000010 rotated right by 1 = 00000001 = 1
    printf("rr(2,1,8)=%d\n", rotate_right(2, 1, 8));
    // EXPECT: rr(2,1,8)=1
    // 0xA5 = 10100101 rotated right by 4 = 01011010 = 0x5A = 90
    printf("rr(165,4,8)=%d\n", rotate_right(165, 4, 8));
    // EXPECT: rr(165,4,8)=90

    // rotate_left and rotate_right are inverses
    int val = 42;
    int r = rotate_left(val, 3, 8);
    int back = rotate_right(r, 3, 8);
    printf("inv=%d\n", back);
    // EXPECT: inv=42

    // 4-bit rotations
    // 5 = 0101 rotate left 1 = 1010 = 10
    printf("r4l(5,1)=%d\n", rotate_left(5, 1, 4));
    // EXPECT: r4l(5,1)=10
    // 5 = 0101 rotate left 2 = 0101 = 5 (palindrome in 4 bits)
    printf("r4l(5,2)=%d\n", rotate_left(5, 2, 4));
    // EXPECT: r4l(5,2)=5
    // 9 = 1001 rotate left 1 = 0011 = 3
    printf("r4l(9,1)=%d\n", rotate_left(9, 1, 4));
    // EXPECT: r4l(9,1)=3
    // 9 = 1001 rotate right 1 = 1100 = 12
    printf("r4r(9,1)=%d\n", rotate_right(9, 1, 4));
    // EXPECT: r4r(9,1)=12

    // Full rotation returns to original
    printf("full=%d\n", rotate_left(42, 8, 8));
    // EXPECT: full=42
    printf("full4=%d\n", rotate_left(13, 4, 4));
    // EXPECT: full4=13

    // Shift register (3-bit LFSR with taps at 0,2)
    struct ShiftReg sr;
    sr_init(&sr, 1, 3);
    sr_add_tap(&sr, 0);
    sr_add_tap(&sr, 2);
    // Initial state: 001
    // step 1: feedback=bit0(1)^bit2(0)=1, state=(001<<1|1)&111 = 011 = 3
    printf("s1=%d\n", sr_step(&sr));
    // EXPECT: s1=3
    // step 2: feedback=bit0(1)^bit2(0)=1, state=(011<<1|1)&111 = 111 = 7
    printf("s2=%d\n", sr_step(&sr));
    // EXPECT: s2=7
    // step 3: feedback=bit0(1)^bit2(1)=0, state=(111<<1|0)&111 = 110 = 6
    printf("s3=%d\n", sr_step(&sr));
    // EXPECT: s3=6
    // step 4: feedback=bit0(0)^bit2(1)=1, state=(110<<1|1)&111 = 101 = 5
    printf("s4=%d\n", sr_step(&sr));
    // EXPECT: s4=5
    // step 5: feedback=bit0(1)^bit2(1)=0, state=(101<<1|0)&111 = 010 = 2
    printf("s5=%d\n", sr_step(&sr));
    // EXPECT: s5=2
    // step 6: feedback=bit0(0)^bit2(0)=0, state=(010<<1|0)&111 = 100 = 4
    printf("s6=%d\n", sr_step(&sr));
    // EXPECT: s6=4
    // step 7: feedback=bit0(0)^bit2(1)=1, state=(100<<1|1)&111 = 001 = 1
    printf("s7=%d\n", sr_step(&sr));
    // EXPECT: s7=1

    // Period should be 7 (maximal length for 3-bit LFSR)
    sr_init(&sr, 1, 3);
    sr.ntaps = 0;
    sr_add_tap(&sr, 0);
    sr_add_tap(&sr, 2);
    printf("period=%d\n", sr_period(&sr));
    // EXPECT: period=7

    // triple_rotate
    int tv = triple_rotate(42, 2, 3, 1, 8);
    int tv_check = rotate_left(42, 2, 8);
    tv_check = rotate_right(tv_check, 3, 8);
    tv_check = rotate_left(tv_check, 1, 8);
    printf("triple=%d\n", tv == tv_check);
    // EXPECT: triple=1

    // barrel_shift equivalence
    printf("bs=%d\n", barrel_shift(1, 3, 8));
    // EXPECT: bs=8
    printf("bs2=%d\n", barrel_shift(165, 4, 8));
    // EXPECT: bs2=90

    printf("done\n");
    // EXPECT: done
    return 0;
}
