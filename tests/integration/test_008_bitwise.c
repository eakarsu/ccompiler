int printf(const char *fmt, ...);
// EXPECT: and_12_10=8
// or_12_10=14
// xor_12_10=6
// not_0=-1
// not_neg1=0
// shl_1_0=1
// shl_1_1=2
// shl_1_4=16
// shl_1_10=1024
// shr_16_1=8
// shr_16_2=4
// shr_16_4=1
// shr_1024_10=1
// popcount_0=0
// popcount_1=1
// popcount_7=3
// popcount_255=8
// popcount_1023=10
// is_pow2_1=1
// is_pow2_2=1
// is_pow2_4=1
// is_pow2_16=1
// is_pow2_3=0
// is_pow2_6=0
// is_pow2_0=0
// lowest_bit_12=4
// lowest_bit_10=2
// lowest_bit_8=8
// clear_lowest_12=8
// clear_lowest_10=8
// set_bit_0_3=8
// set_bit_0_5=32
// clear_bit_15_2=11
// clear_bit_255_7=127
// toggle_bit_0_3=8
// toggle_bit_8_3=0
// check_bit_10_1=1
// check_bit_10_2=0
// check_bit_10_3=1
// swap_xor: a=20 b=10
// abs_neg5=5
// abs_pos3=3
// sign_neg=-1
// sign_pos=1
// sign_zero=0
// is_even_4=1
// is_even_7=0
// is_odd_7=1
// next_pow2_5=8
// next_pow2_9=16
// next_pow2_16=16
// next_pow2_1=1
// reverse_bits_1=128
// reverse_bits_170=85
// parity_7=1
// parity_3=0
// parity_255=0
// parity_128=1
// mask_low4_255=15
// mask_low4_171=11
// mask_high4_255=240
// rotate_left8_1_3=8
// rotate_left8_128_1=1
// rotate_left8_171_4=186
// count_diff_bits_0_0=0
// count_diff_bits_7_0=3
// count_diff_bits_15_240=8
// gray_0=0
// gray_1=1
// gray_2=3
// gray_3=2
// gray_4=6
// gray_5=7
// gray_6=5
// gray_7=4
// merge_bits=202
// done

// =============================================================================
// TEST 008: Bitwise Operations
// =============================================================================

// --- Population count (number of 1 bits) ---
int popcount(int n) {
    int count = 0;
    // Work with positive values using a fixed number of iterations
    int i = 0;
    while (i < 32) {
        if (n & 1) {
            count = count + 1;
        }
        n = (n >> 1) & 0x7FFFFFFF;
        i = i + 1;
    }
    return count;
}

// --- Check if power of 2 ---
int is_power_of_2(int n) {
    if (n <= 0) return 0;
    return (n & (n - 1)) == 0;
}

// --- Lowest set bit ---
int lowest_set_bit(int n) {
    return n & (-n);
}

// --- Clear lowest set bit ---
int clear_lowest_bit(int n) {
    return n & (n - 1);
}

// --- Set bit at position ---
int set_bit(int n, int pos) {
    return n | (1 << pos);
}

// --- Clear bit at position ---
int clear_bit(int n, int pos) {
    return n & ~(1 << pos);
}

// --- Toggle bit at position ---
int toggle_bit(int n, int pos) {
    return n ^ (1 << pos);
}

// --- Check bit at position ---
int check_bit(int n, int pos) {
    return (n >> pos) & 1;
}

// --- XOR swap ---
void xor_swap(int *a, int *b) {
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

// --- Absolute value using bitwise ---
int bit_abs(int n) {
    int mask = n >> 31;
    return (n ^ mask) - mask;
}

// --- Sign function ---
int sign_of(int n) {
    if (n > 0) return 1;
    if (n < 0) return -1;
    return 0;
}

// --- Check even/odd ---
int is_even(int n) {
    return (n & 1) == 0;
}

int is_odd(int n) {
    return (n & 1) == 1;
}

// --- Next power of 2 (>= n) ---
int next_power_of_2(int n) {
    if (n <= 1) return 1;
    int p = 1;
    while (p < n) {
        p = p << 1;
    }
    return p;
}

// --- Reverse 8 bits ---
int reverse_8bits(int n) {
    int result = 0;
    int i = 0;
    while (i < 8) {
        result = result << 1;
        result = result | (n & 1);
        n = n >> 1;
        i = i + 1;
    }
    return result;
}

// --- Parity (1 if odd number of bits, 0 if even) ---
int parity(int n) {
    int p = 0;
    int tmp = n;
    int i = 0;
    while (i < 16) {
        if (tmp & 1) {
            p = p ^ 1;
        }
        tmp = tmp >> 1;
        i = i + 1;
    }
    return p;
}

// --- Mask low N bits ---
int mask_low_bits(int n, int bits) {
    return n & ((1 << bits) - 1);
}

// --- Mask high N bits of byte ---
int mask_high_bits_byte(int n, int bits) {
    int shift = 8 - bits;
    int mask = ((1 << bits) - 1) << shift;
    return n & mask;
}

// --- Rotate left within 8 bits ---
int rotate_left_8(int n, int k) {
    k = k % 8;
    return ((n << k) | (n >> (8 - k))) & 255;
}

// --- Count differing bits between two numbers ---
int count_diff_bits(int a, int b) {
    return popcount(a ^ b);
}

// --- Gray code ---
int to_gray(int n) {
    return n ^ (n >> 1);
}

// --- Merge bits: use mask to select bits from a or b ---
// Result bit i = mask bit i ? a bit i : b bit i
int merge_bits(int a, int b, int mask) {
    return (a & mask) | (b & ~mask);
}

// --- Extract bit field ---
int extract_field(int n, int start, int width) {
    return (n >> start) & ((1 << width) - 1);
}

// --- Insert bit field ---
int insert_field(int n, int val, int start, int width) {
    int mask = ((1 << width) - 1) << start;
    return (n & ~mask) | ((val << start) & mask);
}

// --- Count leading zeros (for 8-bit values) ---
int clz8(int n) {
    if (n == 0) return 8;
    int count = 0;
    int bit = 128;
    while (bit > 0 && !(n & bit)) {
        count = count + 1;
        bit = bit >> 1;
    }
    return count;
}

// --- Count trailing zeros ---
int ctz(int n) {
    if (n == 0) return 32;
    int count = 0;
    while (!(n & 1)) {
        count = count + 1;
        n = n >> 1;
    }
    return count;
}

// --- Bit interleave (low 4 bits of each) ---
int bit_interleave(int a, int b) {
    int result = 0;
    int i = 0;
    while (i < 4) {
        result = result | (((a >> i) & 1) << (2 * i));
        result = result | (((b >> i) & 1) << (2 * i + 1));
        i = i + 1;
    }
    return result;
}

// --- Check if all bits in mask are set ---
int all_bits_set(int n, int mask) {
    return (n & mask) == mask;
}

// --- Check if any bit in mask is set ---
int any_bit_set(int n, int mask) {
    return (n & mask) != 0;
}

// --- Swap nibbles of a byte ---
int swap_nibbles(int n) {
    return ((n & 0x0F) << 4) | ((n >> 4) & 0x0F);
}

// --- Power of 2 ceiling ---
int ceil_pow2(int n) {
    if (n <= 1) return 1;
    int p = 1;
    while (p < n) {
        p = p << 1;
    }
    return p;
}

// --- Floor log2 ---
int floor_log2(int n) {
    int result = 0;
    while (n > 1) {
        n = n >> 1;
        result = result + 1;
    }
    return result;
}

int main() {
    // --- Basic AND, OR, XOR ---
    // 12 = 1100, 10 = 1010
    // AND = 1000 = 8, OR = 1110 = 14, XOR = 0110 = 6
    printf("and_12_10=%d\n", 12 & 10);
    printf("or_12_10=%d\n", 12 | 10);
    printf("xor_12_10=%d\n", 12 ^ 10);

    // --- NOT ---
    printf("not_0=%d\n", ~0);
    printf("not_neg1=%d\n", ~(-1));

    // --- Shifts ---
    printf("shl_1_0=%d\n", 1 << 0);
    printf("shl_1_1=%d\n", 1 << 1);
    printf("shl_1_4=%d\n", 1 << 4);
    printf("shl_1_10=%d\n", 1 << 10);
    printf("shr_16_1=%d\n", 16 >> 1);
    printf("shr_16_2=%d\n", 16 >> 2);
    printf("shr_16_4=%d\n", 16 >> 4);
    printf("shr_1024_10=%d\n", 1024 >> 10);

    // --- Popcount ---
    printf("popcount_0=%d\n", popcount(0));
    printf("popcount_1=%d\n", popcount(1));
    printf("popcount_7=%d\n", popcount(7));
    printf("popcount_255=%d\n", popcount(255));
    printf("popcount_1023=%d\n", popcount(1023));

    // --- Power of 2 checks ---
    printf("is_pow2_1=%d\n", is_power_of_2(1));
    printf("is_pow2_2=%d\n", is_power_of_2(2));
    printf("is_pow2_4=%d\n", is_power_of_2(4));
    printf("is_pow2_16=%d\n", is_power_of_2(16));
    printf("is_pow2_3=%d\n", is_power_of_2(3));
    printf("is_pow2_6=%d\n", is_power_of_2(6));
    printf("is_pow2_0=%d\n", is_power_of_2(0));

    // --- Lowest set bit ---
    // 12=1100 => lowest=0100=4
    printf("lowest_bit_12=%d\n", lowest_set_bit(12));
    // 10=1010 => lowest=0010=2
    printf("lowest_bit_10=%d\n", lowest_set_bit(10));
    // 8=1000 => lowest=1000=8
    printf("lowest_bit_8=%d\n", lowest_set_bit(8));

    // --- Clear lowest bit ---
    // 12=1100 => 1000=8
    printf("clear_lowest_12=%d\n", clear_lowest_bit(12));
    // 10=1010 => 1000=8
    printf("clear_lowest_10=%d\n", clear_lowest_bit(10));

    // --- Set/clear/toggle/check bit ---
    printf("set_bit_0_3=%d\n", set_bit(0, 3));
    printf("set_bit_0_5=%d\n", set_bit(0, 5));
    // 15=1111, clear bit 2 => 1011=11
    printf("clear_bit_15_2=%d\n", clear_bit(15, 2));
    // 255=11111111, clear bit 7 => 01111111=127
    printf("clear_bit_255_7=%d\n", clear_bit(255, 7));
    printf("toggle_bit_0_3=%d\n", toggle_bit(0, 3));
    printf("toggle_bit_8_3=%d\n", toggle_bit(8, 3));
    // 10=1010: bit 1 is 1, bit 2 is 0, bit 3 is 1
    printf("check_bit_10_1=%d\n", check_bit(10, 1));
    printf("check_bit_10_2=%d\n", check_bit(10, 2));
    printf("check_bit_10_3=%d\n", check_bit(10, 3));

    // --- XOR swap ---
    int a = 10; int b = 20;
    xor_swap(&a, &b);
    printf("swap_xor: a=%d b=%d\n", a, b);

    // --- Absolute value ---
    printf("abs_neg5=%d\n", bit_abs(-5));
    printf("abs_pos3=%d\n", bit_abs(3));

    // --- Sign ---
    printf("sign_neg=%d\n", sign_of(-42));
    printf("sign_pos=%d\n", sign_of(42));
    printf("sign_zero=%d\n", sign_of(0));

    // --- Even/odd ---
    printf("is_even_4=%d\n", is_even(4));
    printf("is_even_7=%d\n", is_even(7));
    printf("is_odd_7=%d\n", is_odd(7));

    // --- Next power of 2 ---
    printf("next_pow2_5=%d\n", next_power_of_2(5));
    printf("next_pow2_9=%d\n", next_power_of_2(9));
    printf("next_pow2_16=%d\n", next_power_of_2(16));
    printf("next_pow2_1=%d\n", next_power_of_2(1));

    // --- Reverse 8 bits ---
    // 1 = 00000001 => 10000000 = 128
    printf("reverse_bits_1=%d\n", reverse_8bits(1));
    // 170 = 10101010 => 01010101 = 85
    printf("reverse_bits_170=%d\n", reverse_8bits(170));

    // --- Parity ---
    // 7=111 => 3 bits set => odd parity => 1
    printf("parity_7=%d\n", parity(7));
    // 3=11 => 2 bits => even parity => 0
    printf("parity_3=%d\n", parity(3));
    // 255=11111111 => 8 bits => even parity => 0
    printf("parity_255=%d\n", parity(255));
    // 128=10000000 => 1 bit => odd parity => 1
    printf("parity_128=%d\n", parity(128));

    // --- Mask operations ---
    // mask low 4 bits of 255 = 15
    printf("mask_low4_255=%d\n", mask_low_bits(255, 4));
    // 171=10101011, low 4 bits = 1011 = 11
    printf("mask_low4_171=%d\n", mask_low_bits(171, 4));
    // mask high 4 bits of byte: 255 => 11110000 = 240
    printf("mask_high4_255=%d\n", mask_high_bits_byte(255, 4));

    // --- Rotate left 8-bit ---
    // 1 rotated left 3 = 8
    printf("rotate_left8_1_3=%d\n", rotate_left_8(1, 3));
    // 128=10000000 rotated left 1 = 00000001 = 1
    printf("rotate_left8_128_1=%d\n", rotate_left_8(128, 1));
    // 171=10101011 rotated left 4 = 10111010 = 186
    printf("rotate_left8_171_4=%d\n", rotate_left_8(171, 4));

    // --- Count differing bits ---
    printf("count_diff_bits_0_0=%d\n", count_diff_bits(0, 0));
    // 7=111, 0=000 => 3 different
    printf("count_diff_bits_7_0=%d\n", count_diff_bits(7, 0));
    // 15=00001111, 240=11110000 => 8 different
    printf("count_diff_bits_15_240=%d\n", count_diff_bits(15, 240));

    // --- Gray code ---
    printf("gray_0=%d\n", to_gray(0));
    printf("gray_1=%d\n", to_gray(1));
    printf("gray_2=%d\n", to_gray(2));
    printf("gray_3=%d\n", to_gray(3));
    printf("gray_4=%d\n", to_gray(4));
    printf("gray_5=%d\n", to_gray(5));
    printf("gray_6=%d\n", to_gray(6));
    printf("gray_7=%d\n", to_gray(7));

    // --- Merge bits ---
    // a=0xFF=255, b=0x00=0, mask=0xCA=202
    // Result: bits from a where mask=1, bits from b where mask=0
    // mask=11001010, so result = 11001010 = 202
    printf("merge_bits=%d\n", merge_bits(255, 0, 202));

    printf("done\n");
    return 0;
}
