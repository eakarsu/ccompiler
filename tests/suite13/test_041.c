int printf(const char *fmt, ...);

// Bit counting: popcount, leading zeros, trailing zeros

int popcount(int x) {
    int count = 0;
    int val = x;
    while (val != 0) {
        if (val & 1) {
            count = count + 1;
        }
        val = (val >> 1) & 0x7FFFFFFF;
    }
    return count;
}

int trailing_zeros(int x) {
    if (x == 0) return 32;
    int count = 0;
    while ((x & 1) == 0) {
        count = count + 1;
        x = (x >> 1) & 0x7FFFFFFF;
    }
    return count;
}

int leading_zeros_31(int x) {
    // Count leading zeros in bits 0..30 (31-bit positive range)
    if (x == 0) return 31;
    int count = 0;
    int bit = 30;
    while (bit >= 0) {
        if (x & (1 << bit)) {
            return count;
        }
        count = count + 1;
        bit = bit - 1;
    }
    return count;
}

int highest_set_bit(int x) {
    if (x == 0) return -1;
    int pos = 0;
    int temp = x;
    while (temp > 1) {
        temp = (temp >> 1) & 0x7FFFFFFF;
        pos = pos + 1;
    }
    return pos;
}

int lowest_set_bit(int x) {
    if (x == 0) return -1;
    int pos = 0;
    while ((x & 1) == 0) {
        x = (x >> 1) & 0x7FFFFFFF;
        pos = pos + 1;
    }
    return pos;
}

// Count total set bits in all integers from 0 to n
int count_bits_0_to_n(int n) {
    int total = 0;
    int i = 0;
    while (i <= n) {
        total = total + popcount(i);
        i = i + 1;
    }
    return total;
}

// Isolate the lowest set bit: returns a value with only that bit set
int isolate_lowest(int x) {
    return x & (-x);
}

// Turn off the lowest set bit
int clear_lowest(int x) {
    return x & (x - 1);
}

int main() {
    // popcount tests
    printf("popcount(0)=%d\n", popcount(0));
    // EXPECT: popcount(0)=0
    printf("popcount(1)=%d\n", popcount(1));
    // EXPECT: popcount(1)=1
    printf("popcount(7)=%d\n", popcount(7));
    // EXPECT: popcount(7)=3
    printf("popcount(15)=%d\n", popcount(15));
    // EXPECT: popcount(15)=4
    printf("popcount(255)=%d\n", popcount(255));
    // EXPECT: popcount(255)=8
    printf("popcount(1023)=%d\n", popcount(1023));
    // EXPECT: popcount(1023)=10
    printf("popcount(170)=%d\n", popcount(170));
    // EXPECT: popcount(170)=4

    // trailing zeros tests
    printf("tz(0)=%d\n", trailing_zeros(0));
    // EXPECT: tz(0)=32
    printf("tz(1)=%d\n", trailing_zeros(1));
    // EXPECT: tz(1)=0
    printf("tz(2)=%d\n", trailing_zeros(2));
    // EXPECT: tz(2)=1
    printf("tz(4)=%d\n", trailing_zeros(4));
    // EXPECT: tz(4)=2
    printf("tz(16)=%d\n", trailing_zeros(16));
    // EXPECT: tz(16)=4
    printf("tz(48)=%d\n", trailing_zeros(48));
    // EXPECT: tz(48)=4
    printf("tz(96)=%d\n", trailing_zeros(96));
    // EXPECT: tz(96)=5

    // leading zeros (31-bit) tests
    printf("lz(0)=%d\n", leading_zeros_31(0));
    // EXPECT: lz(0)=31
    printf("lz(1)=%d\n", leading_zeros_31(1));
    // EXPECT: lz(1)=30
    printf("lz(2)=%d\n", leading_zeros_31(2));
    // EXPECT: lz(2)=29
    printf("lz(255)=%d\n", leading_zeros_31(255));
    // EXPECT: lz(255)=23
    printf("lz(1024)=%d\n", leading_zeros_31(1024));
    // EXPECT: lz(1024)=20

    // highest and lowest set bit
    printf("hsb(0)=%d\n", highest_set_bit(0));
    // EXPECT: hsb(0)=-1
    printf("hsb(1)=%d\n", highest_set_bit(1));
    // EXPECT: hsb(1)=0
    printf("hsb(8)=%d\n", highest_set_bit(8));
    // EXPECT: hsb(8)=3
    printf("hsb(255)=%d\n", highest_set_bit(255));
    // EXPECT: hsb(255)=7
    printf("lsb(0)=%d\n", lowest_set_bit(0));
    // EXPECT: lsb(0)=-1
    printf("lsb(1)=%d\n", lowest_set_bit(1));
    // EXPECT: lsb(1)=0
    printf("lsb(6)=%d\n", lowest_set_bit(6));
    // EXPECT: lsb(6)=1
    printf("lsb(12)=%d\n", lowest_set_bit(12));
    // EXPECT: lsb(12)=2

    // count total bits from 0..n
    // 0=0, 1=1, 2=1, 3=2, 4=1, 5=2, 6=2, 7=3 => sum = 0+1+1+2+1+2+2+3 = 12
    printf("bits_0_7=%d\n", count_bits_0_to_n(7));
    // EXPECT: bits_0_7=12
    // 0..3: 0+1+1+2 = 4
    printf("bits_0_3=%d\n", count_bits_0_to_n(3));
    // EXPECT: bits_0_3=4

    // isolate lowest set bit
    printf("iso(12)=%d\n", isolate_lowest(12));
    // EXPECT: iso(12)=4
    printf("iso(10)=%d\n", isolate_lowest(10));
    // EXPECT: iso(10)=2
    printf("iso(7)=%d\n", isolate_lowest(7));
    // EXPECT: iso(7)=1

    // clear lowest set bit
    printf("clr(12)=%d\n", clear_lowest(12));
    // EXPECT: clr(12)=8
    printf("clr(7)=%d\n", clear_lowest(7));
    // EXPECT: clr(7)=6
    printf("clr(10)=%d\n", clear_lowest(10));
    // EXPECT: clr(10)=8

    // iterate through set bits of 105 = 0b1101001
    // bits at positions: 0, 3, 5, 6
    int val = 105;
    int count = 0;
    while (val != 0) {
        int pos = lowest_set_bit(val);
        printf("bit%d=%d\n", count, pos);
        val = clear_lowest(val);
        count = count + 1;
    }
    // EXPECT: bit0=0
    // EXPECT: bit1=3
    // EXPECT: bit2=5
    // EXPECT: bit3=6

    printf("done\n");
    // EXPECT: done
    return 0;
}
