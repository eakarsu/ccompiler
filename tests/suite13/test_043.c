int printf(const char *fmt, ...);

// Bit reversal of integers

// Reverse the lowest n bits of x
int reverse_bits(int x, int n) {
    int result = 0;
    int i = 0;
    while (i < n) {
        if (x & (1 << i)) {
            result = result | (1 << (n - 1 - i));
        }
        i = i + 1;
    }
    return result;
}

// Reverse all 8 bits of a byte
int reverse_byte(int x) {
    return reverse_bits(x & 0xFF, 8);
}

// Reverse all 16 bits
int reverse_16(int x) {
    return reverse_bits(x & 0xFFFF, 16);
}

// Swap nibbles of a byte (swap upper 4 and lower 4 bits)
int swap_nibbles(int x) {
    int lo = x & 0x0F;
    int hi = (x >> 4) & 0x0F;
    return (lo << 4) | hi;
}

// Swap bytes in a 16-bit value
int swap_bytes_16(int x) {
    int lo = x & 0xFF;
    int hi = (x >> 8) & 0xFF;
    return (lo << 8) | hi;
}

// Swap bytes in a 32-bit value (only lower 4 bytes)
int swap_bytes_32(int x) {
    int b0 = x & 0xFF;
    int b1 = (x >> 8) & 0xFF;
    int b2 = (x >> 16) & 0xFF;
    int b3 = (x >> 24) & 0xFF;
    return (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
}

// Check if a value is a palindrome in binary (using n bits)
int is_bit_palindrome(int x, int n) {
    return reverse_bits(x, n) == x;
}

// Interleave bits of two values: result bit 2i = x bit i, bit 2i+1 = y bit i
int interleave_bits(int x, int y, int n) {
    int result = 0;
    int i = 0;
    while (i < n) {
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

// De-interleave: extract even-position bits
int deinterleave_even(int x, int n) {
    int result = 0;
    int i = 0;
    while (i < n) {
        if (x & (1 << (2 * i))) {
            result = result | (1 << i);
        }
        i = i + 1;
    }
    return result;
}

// De-interleave: extract odd-position bits
int deinterleave_odd(int x, int n) {
    int result = 0;
    int i = 0;
    while (i < n) {
        if (x & (1 << (2 * i + 1))) {
            result = result | (1 << i);
        }
        i = i + 1;
    }
    return result;
}

int main() {
    // reverse_bits tests
    // reverse 3 bits of 5 (101) -> 101 = 5
    printf("rev(5,3)=%d\n", reverse_bits(5, 3));
    // EXPECT: rev(5,3)=5
    // reverse 3 bits of 6 (110) -> 011 = 3
    printf("rev(6,3)=%d\n", reverse_bits(6, 3));
    // EXPECT: rev(6,3)=3
    // reverse 4 bits of 1 (0001) -> 1000 = 8
    printf("rev(1,4)=%d\n", reverse_bits(1, 4));
    // EXPECT: rev(1,4)=8
    // reverse 4 bits of 9 (1001) -> 1001 = 9
    printf("rev(9,4)=%d\n", reverse_bits(9, 4));
    // EXPECT: rev(9,4)=9
    // reverse 4 bits of 11 (1011) -> 1101 = 13
    printf("rev(11,4)=%d\n", reverse_bits(11, 4));
    // EXPECT: rev(11,4)=13
    // reverse 8 bits of 1 (00000001) -> 10000000 = 128
    printf("rev(1,8)=%d\n", reverse_bits(1, 8));
    // EXPECT: rev(1,8)=128
    // reverse 8 bits of 0xA5 (10100101) -> 10100101 = 0xA5 = 165
    printf("rev(165,8)=%d\n", reverse_bits(165, 8));
    // EXPECT: rev(165,8)=165

    // reverse_byte
    printf("rb(0)=%d\n", reverse_byte(0));
    // EXPECT: rb(0)=0
    printf("rb(1)=%d\n", reverse_byte(1));
    // EXPECT: rb(1)=128
    printf("rb(255)=%d\n", reverse_byte(255));
    // EXPECT: rb(255)=255
    // 0x0F = 00001111 -> 11110000 = 0xF0 = 240
    printf("rb(15)=%d\n", reverse_byte(15));
    // EXPECT: rb(15)=240
    // 0x12 = 00010010 -> 01001000 = 72
    printf("rb(18)=%d\n", reverse_byte(18));
    // EXPECT: rb(18)=72

    // swap_nibbles
    // 0x3C = 0011_1100 -> 1100_0011 = 0xC3 = 195
    printf("sn(60)=%d\n", swap_nibbles(60));
    // EXPECT: sn(60)=195
    // 0x12 = 0001_0010 -> 0010_0001 = 0x21 = 33
    printf("sn(18)=%d\n", swap_nibbles(18));
    // EXPECT: sn(18)=33
    printf("sn(0)=%d\n", swap_nibbles(0));
    // EXPECT: sn(0)=0
    printf("sn(255)=%d\n", swap_nibbles(255));
    // EXPECT: sn(255)=255

    // swap_bytes_16
    // 0x0102 = 258 -> 0x0201 = 513
    printf("sb16(258)=%d\n", swap_bytes_16(258));
    // EXPECT: sb16(258)=513
    // 0x00FF = 255 -> 0xFF00 = 65280
    printf("sb16(255)=%d\n", swap_bytes_16(255));
    // EXPECT: sb16(255)=65280
    printf("sb16(0)=%d\n", swap_bytes_16(0));
    // EXPECT: sb16(0)=0

    // is_bit_palindrome
    // 5 in 3 bits = 101, palindrome
    printf("pal(5,3)=%d\n", is_bit_palindrome(5, 3));
    // EXPECT: pal(5,3)=1
    // 9 in 4 bits = 1001, palindrome
    printf("pal(9,4)=%d\n", is_bit_palindrome(9, 4));
    // EXPECT: pal(9,4)=1
    // 6 in 4 bits = 0110, palindrome
    printf("pal(6,4)=%d\n", is_bit_palindrome(6, 4));
    // EXPECT: pal(6,4)=1
    // 3 in 4 bits = 0011, reverse = 1100 = 12, not palindrome
    printf("pal(3,4)=%d\n", is_bit_palindrome(3, 4));
    // EXPECT: pal(3,4)=0
    // 7 in 3 bits = 111, palindrome
    printf("pal(7,3)=%d\n", is_bit_palindrome(7, 3));
    // EXPECT: pal(7,3)=1

    // interleave bits
    // x=5 (101), y=3 (011), n=3
    // bit0: x0=1 -> pos0, y0=1 -> pos1
    // bit1: x1=0 -> pos2, y1=1 -> pos3
    // bit2: x2=1 -> pos4, y2=0 -> pos5
    // result = 1 + 2 + 8 + 16 = 27
    printf("il(5,3,3)=%d\n", interleave_bits(5, 3, 3));
    // EXPECT: il(5,3,3)=27
    // x=3 (11), y=0 (00), n=2
    // bit0: x0=1->pos0, y0=0->pos1; bit1: x1=1->pos2, y1=0->pos3
    // result = 1 + 4 = 5
    printf("il(3,0,2)=%d\n", interleave_bits(3, 0, 2));
    // EXPECT: il(3,0,2)=5
    // x=0, y=3 (11), n=2
    // bit0: x0=0, y0=1->pos1; bit1: x1=0, y1=1->pos3
    // result = 2 + 8 = 10
    printf("il(0,3,2)=%d\n", interleave_bits(0, 3, 2));
    // EXPECT: il(0,3,2)=10

    // deinterleave roundtrip
    int mixed = interleave_bits(13, 7, 4);
    int ex = deinterleave_even(mixed, 4);
    int oy = deinterleave_odd(mixed, 4);
    printf("de_x=%d\n", ex);
    // EXPECT: de_x=13
    printf("de_y=%d\n", oy);
    // EXPECT: de_y=7

    printf("done\n");
    // EXPECT: done
    return 0;
}
