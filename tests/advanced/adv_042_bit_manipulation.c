// EXPECT: === Advanced Bit Manipulation ===
// EXPECT: --- Set/Clear/Toggle ---
// EXPECT: set bit 3 of 0: 8
// EXPECT: set bit 0 of 8: 9
// EXPECT: clear bit 3 of 15: 7
// EXPECT: clear bit 0 of 7: 6
// EXPECT: toggle bit 2 of 5: 1
// EXPECT: toggle bit 2 of 1: 5
// EXPECT: --- Count Set Bits ---
// EXPECT: popcount(0)=0
// EXPECT: popcount(1)=1
// EXPECT: popcount(7)=3
// EXPECT: popcount(255)=8
// EXPECT: popcount(1023)=10
// EXPECT: popcount(170)=4
// EXPECT: --- Power of 2 Check ---
// EXPECT: is_pow2(0)=0
// EXPECT: is_pow2(1)=1
// EXPECT: is_pow2(2)=1
// EXPECT: is_pow2(3)=0
// EXPECT: is_pow2(4)=1
// EXPECT: is_pow2(16)=1
// EXPECT: is_pow2(15)=0
// EXPECT: is_pow2(64)=1
// EXPECT: --- Next Power of 2 ---
// EXPECT: next_pow2(0)=1
// EXPECT: next_pow2(1)=1
// EXPECT: next_pow2(2)=2
// EXPECT: next_pow2(3)=4
// EXPECT: next_pow2(5)=8
// EXPECT: next_pow2(7)=8
// EXPECT: next_pow2(9)=16
// EXPECT: next_pow2(17)=32
// EXPECT: next_pow2(33)=64
// EXPECT: --- Bit Reversal (8-bit) ---
// EXPECT: reverse8(0)=0
// EXPECT: reverse8(1)=128
// EXPECT: reverse8(128)=1
// EXPECT: reverse8(170)=85
// EXPECT: reverse8(85)=170
// EXPECT: reverse8(255)=255
// EXPECT: --- Parity ---
// EXPECT: parity(0)=0
// EXPECT: parity(1)=1
// EXPECT: parity(3)=0
// EXPECT: parity(7)=1
// EXPECT: parity(15)=0
// EXPECT: parity(255)=0
// EXPECT: parity(170)=0
// EXPECT: parity(171)=1
// EXPECT: --- Lowest Set Bit ---
// EXPECT: lowest_set(1)=1
// EXPECT: lowest_set(2)=2
// EXPECT: lowest_set(12)=4
// EXPECT: lowest_set(40)=8
// EXPECT: lowest_set(128)=128
// EXPECT: --- Bit Extraction ---
// EXPECT: extract bits 4..7 of 0xAB=10
// EXPECT: extract bits 0..3 of 0xAB=11
// EXPECT: extract bits 4..7 of 0xFF=15
// EXPECT: === Done ===

int printf(const char *fmt, ...);

int set_bit(int x, int n) {
    return x | (1 << n);
}

int clear_bit(int x, int n) {
    return x & ~(1 << n);
}

int toggle_bit(int x, int n) {
    return x ^ (1 << n);
}

int popcount(int x) {
    int count = 0;
    while (x) {
        count = count + (x & 1);
        x = x >> 1;
    }
    return count;
}

int is_pow2(int x) {
    if (x <= 0) return 0;
    return (x & (x - 1)) == 0;
}

int next_pow2(int x) {
    if (x <= 1) return 1;
    int p = 1;
    while (p < x) {
        p = p << 1;
    }
    return p;
}

int reverse8(int x) {
    int result = 0;
    int i = 0;
    while (i < 8) {
        result = result << 1;
        result = result | (x & 1);
        x = x >> 1;
        i = i + 1;
    }
    return result;
}

int parity(int x) {
    int p = 0;
    while (x) {
        p = p ^ (x & 1);
        x = x >> 1;
    }
    return p;
}

int lowest_set_bit(int x) {
    return x & (-x);
}

int extract_bits(int x, int lo, int hi) {
    int mask = 0;
    int i = lo;
    while (i <= hi) {
        mask = mask | (1 << i);
        i = i + 1;
    }
    return (x & mask) >> lo;
}

int main(void) {
    printf("=== Advanced Bit Manipulation ===\n");

    printf("--- Set/Clear/Toggle ---\n");
    printf("set bit 3 of 0: %d\n", set_bit(0, 3));
    printf("set bit 0 of 8: %d\n", set_bit(8, 0));
    printf("clear bit 3 of 15: %d\n", clear_bit(15, 3));
    printf("clear bit 0 of 7: %d\n", clear_bit(7, 0));
    printf("toggle bit 2 of 5: %d\n", toggle_bit(5, 2));
    printf("toggle bit 2 of 1: %d\n", toggle_bit(1, 2));

    printf("--- Count Set Bits ---\n");
    printf("popcount(0)=%d\n", popcount(0));
    printf("popcount(1)=%d\n", popcount(1));
    printf("popcount(7)=%d\n", popcount(7));
    printf("popcount(255)=%d\n", popcount(255));
    printf("popcount(1023)=%d\n", popcount(1023));
    printf("popcount(170)=%d\n", popcount(170));

    printf("--- Power of 2 Check ---\n");
    printf("is_pow2(0)=%d\n", is_pow2(0));
    printf("is_pow2(1)=%d\n", is_pow2(1));
    printf("is_pow2(2)=%d\n", is_pow2(2));
    printf("is_pow2(3)=%d\n", is_pow2(3));
    printf("is_pow2(4)=%d\n", is_pow2(4));
    printf("is_pow2(16)=%d\n", is_pow2(16));
    printf("is_pow2(15)=%d\n", is_pow2(15));
    printf("is_pow2(64)=%d\n", is_pow2(64));

    printf("--- Next Power of 2 ---\n");
    printf("next_pow2(0)=%d\n", next_pow2(0));
    printf("next_pow2(1)=%d\n", next_pow2(1));
    printf("next_pow2(2)=%d\n", next_pow2(2));
    printf("next_pow2(3)=%d\n", next_pow2(3));
    printf("next_pow2(5)=%d\n", next_pow2(5));
    printf("next_pow2(7)=%d\n", next_pow2(7));
    printf("next_pow2(9)=%d\n", next_pow2(9));
    printf("next_pow2(17)=%d\n", next_pow2(17));
    printf("next_pow2(33)=%d\n", next_pow2(33));

    printf("--- Bit Reversal (8-bit) ---\n");
    printf("reverse8(0)=%d\n", reverse8(0));
    printf("reverse8(1)=%d\n", reverse8(1));
    printf("reverse8(128)=%d\n", reverse8(128));
    printf("reverse8(170)=%d\n", reverse8(170));
    printf("reverse8(85)=%d\n", reverse8(85));
    printf("reverse8(255)=%d\n", reverse8(255));

    printf("--- Parity ---\n");
    printf("parity(0)=%d\n", parity(0));
    printf("parity(1)=%d\n", parity(1));
    printf("parity(3)=%d\n", parity(3));
    printf("parity(7)=%d\n", parity(7));
    printf("parity(15)=%d\n", parity(15));
    printf("parity(255)=%d\n", parity(255));
    printf("parity(170)=%d\n", parity(170));
    printf("parity(171)=%d\n", parity(171));

    printf("--- Lowest Set Bit ---\n");
    printf("lowest_set(1)=%d\n", lowest_set_bit(1));
    printf("lowest_set(2)=%d\n", lowest_set_bit(2));
    printf("lowest_set(12)=%d\n", lowest_set_bit(12));
    printf("lowest_set(40)=%d\n", lowest_set_bit(40));
    printf("lowest_set(128)=%d\n", lowest_set_bit(128));

    printf("--- Bit Extraction ---\n");
    printf("extract bits 4..7 of 0xAB=%d\n", extract_bits(0xAB, 4, 7));
    printf("extract bits 0..3 of 0xAB=%d\n", extract_bits(0xAB, 0, 3));
    printf("extract bits 4..7 of 0xFF=%d\n", extract_bits(0xFF, 4, 7));

    printf("=== Done ===\n");
    return 0;
}
