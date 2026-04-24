int printf(const char *fmt, ...);

int bit_shuffle(int x, int perm[], int nbits) {
    int result = 0;
    int i = 0;
    while (i < nbits) {
        if (x & (1 << i)) {
            result = result | (1 << perm[i]);
        }
        i = i + 1;
    }
    return result;
}

int interleave_bits(int x, int y, int nbits) {
    int result = 0;
    int i = 0;
    while (i < nbits) {
        if (x & (1 << i)) {
            result = result | (1 << (2 * i));
        }
        if (y & (1 << i)) {
            result = result | (1 << (2 * i + 1));
        }
        i = i + 1;
    }
    return result;
}

int deinterleave_even(int z, int nbits) {
    int result = 0;
    int i = 0;
    while (i < nbits) {
        if (z & (1 << (2 * i))) {
            result = result | (1 << i);
        }
        i = i + 1;
    }
    return result;
}

int deinterleave_odd(int z, int nbits) {
    int result = 0;
    int i = 0;
    while (i < nbits) {
        if (z & (1 << (2 * i + 1))) {
            result = result | (1 << i);
        }
        i = i + 1;
    }
    return result;
}

int reverse_bits(int x, int nbits) {
    int result = 0;
    int i = 0;
    while (i < nbits) {
        if (x & (1 << i)) {
            result = result | (1 << (nbits - 1 - i));
        }
        i = i + 1;
    }
    return result;
}

int circular_shift_left(int x, int n, int nbits) {
    int mask = (1 << nbits) - 1;
    x = x & mask;
    n = n % nbits;
    return ((x << n) | ((x >> (nbits - n)) & ((1 << n) - 1))) & mask;
}

int swap_bit_pairs(int x, int pos1, int pos2) {
    int bit1 = (x >> pos1) & 1;
    int bit2 = (x >> pos2) & 1;
    if (bit1 != bit2) {
        x = x ^ (1 << pos1);
        x = x ^ (1 << pos2);
    }
    return x;
}

int outer_perfect_shuffle(int x, int nbits) {
    int half = nbits / 2;
    int lo = x & ((1 << half) - 1);
    int hi = (x >> half) & ((1 << half) - 1);
    return interleave_bits(lo, hi, half);
}

int inner_perfect_shuffle(int x, int nbits) {
    int half = nbits / 2;
    int lo = x & ((1 << half) - 1);
    int hi = (x >> half) & ((1 << half) - 1);
    return interleave_bits(hi, lo, half);
}

int butterfly_permutation(int x, int stage, int nbits) {
    int result = 0;
    int dist = 1 << stage;
    int i = 0;
    while (i < nbits) {
        int partner = i ^ dist;
        if (partner < nbits && partner > i) {
            int bi = (x >> i) & 1;
            int bp = (x >> partner) & 1;
            result = result | (bp << i);
            result = result | (bi << partner);
        }
        i = i + 1;
    }
    int checked = 0;
    i = 0;
    while (i < nbits) {
        int partner = i ^ dist;
        if (partner >= nbits || partner <= i) {
            result = result | (((x >> i) & 1) << i);
        }
        i = i + 1;
    }
    return result;
}

int main(void) {
    int perm[8];
    perm[0] = 7; perm[1] = 6; perm[2] = 5; perm[3] = 4;
    perm[4] = 3; perm[5] = 2; perm[6] = 1; perm[7] = 0;
    // EXPECT: shuffle_rev(0b10110000) = 13
    printf("shuffle_rev(0b10110000) = %d\n", bit_shuffle(0xB0, perm, 8));

    int perm2[4];
    perm2[0] = 2; perm2[1] = 0; perm2[2] = 3; perm2[3] = 1;
    // EXPECT: shuffle(0b1010, perm2) = 3
    printf("shuffle(0b1010, perm2) = %d\n", bit_shuffle(10, perm2, 4));

    // EXPECT: interleave(0b11, 0b00, 2) = 5
    printf("interleave(0b11, 0b00, 2) = %d\n", interleave_bits(3, 0, 2));
    // EXPECT: interleave(0b11, 0b11, 2) = 15
    printf("interleave(0b11, 0b11, 2) = %d\n", interleave_bits(3, 3, 2));
    // EXPECT: interleave(5, 3, 4) = 27
    printf("interleave(5, 3, 4) = %d\n", interleave_bits(5, 3, 4));

    int z = interleave_bits(5, 10, 4);
    // EXPECT: deinterleave_even = 5
    printf("deinterleave_even = %d\n", deinterleave_even(z, 4));
    // EXPECT: deinterleave_odd = 10
    printf("deinterleave_odd = %d\n", deinterleave_odd(z, 4));

    // EXPECT: reverse(0b1100, 4) = 3
    printf("reverse(0b1100, 4) = %d\n", reverse_bits(12, 4));
    // EXPECT: reverse(0b10101, 5) = 21
    printf("reverse(0b10101, 5) = %d\n", reverse_bits(21, 5));
    // EXPECT: reverse(1, 8) = 128
    printf("reverse(1, 8) = %d\n", reverse_bits(1, 8));

    // EXPECT: rot_left(0b1100, 1, 4) = 9
    printf("rot_left(0b1100, 1, 4) = %d\n", circular_shift_left(12, 1, 4));

    // EXPECT: swap_bits(0b1010, 1, 2) = 12
    printf("swap_bits(0b1010, 1, 2) = %d\n", swap_bit_pairs(10, 1, 2));
    // EXPECT: swap_bits(0b1111, 0, 3) = 15
    printf("swap_bits(0b1111, 0, 3) = %d\n", swap_bit_pairs(15, 0, 3));

    // EXPECT: outer_shuffle(0b11001010, 8) = 228
    printf("outer_shuffle(0b11001010, 8) = %d\n", outer_perfect_shuffle(0xCA, 8));

    int round_x = 0b1010;
    int inter = interleave_bits(round_x, 0, 4);
    int back = deinterleave_even(inter, 4);
    // EXPECT: roundtrip = 10
    printf("roundtrip = %d\n", back);

    // EXPECT: butterfly(0b1010, 0, 4) = 15
    printf("butterfly(0b1010, 0, 4) = %d\n", butterfly_permutation(10, 0, 4));

    return 0;
}
