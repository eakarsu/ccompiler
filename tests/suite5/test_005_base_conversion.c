int printf(const char *fmt, ...);

int count_binary_digits(int n) {
    if (n == 0) return 1;
    int count = 0;
    while (n > 0) {
        count = count + 1;
        n = n / 2;
    }
    return count;
}

int get_bit(int n, int pos) {
    int i = 0;
    while (i < pos) {
        n = n / 2;
        i = i + 1;
    }
    return n % 2;
}

int count_ones(int n) {
    int count = 0;
    while (n > 0) {
        if (n % 2 == 1) count = count + 1;
        n = n / 2;
    }
    return count;
}

int reverse_bits(int n, int bits) {
    int result = 0;
    int i = 0;
    while (i < bits) {
        result = result * 2 + (n % 2);
        n = n / 2;
        i = i + 1;
    }
    return result;
}

int from_binary_array(int arr[], int len) {
    int result = 0;
    int i = 0;
    while (i < len) {
        result = result * 2 + arr[i];
        i = i + 1;
    }
    return result;
}

int get_octal_digit(int n, int pos) {
    int i = 0;
    while (i < pos) {
        n = n / 8;
        i = i + 1;
    }
    return n % 8;
}

int count_octal_digits(int n) {
    if (n == 0) return 1;
    int count = 0;
    while (n > 0) {
        count = count + 1;
        n = n / 8;
    }
    return count;
}

int get_hex_digit(int n, int pos) {
    int i = 0;
    while (i < pos) {
        n = n / 16;
        i = i + 1;
    }
    return n % 16;
}

int from_base(int digits[], int len, int base) {
    int result = 0;
    int i = 0;
    while (i < len) {
        result = result * base + digits[i];
        i = i + 1;
    }
    return result;
}

int to_base_digit_sum(int n, int base) {
    int sum = 0;
    if (n == 0) return 0;
    while (n > 0) {
        sum = sum + (n % base);
        n = n / base;
    }
    return sum;
}

int is_power_of_two(int n) {
    if (n <= 0) return 0;
    return count_ones(n) == 1;
}

int highest_set_bit(int n) {
    int pos = -1;
    int bit = 0;
    while (n > 0) {
        if (n % 2 == 1) pos = bit;
        n = n / 2;
        bit = bit + 1;
    }
    return pos;
}

int main(void) {
    // EXPECT: bin_digits(0) = 1
    printf("bin_digits(0) = %d\n", count_binary_digits(0));
    // EXPECT: bin_digits(7) = 3
    printf("bin_digits(7) = %d\n", count_binary_digits(7));
    // EXPECT: bin_digits(255) = 8
    printf("bin_digits(255) = %d\n", count_binary_digits(255));

    // EXPECT: bit(10,0) = 0
    printf("bit(10,0) = %d\n", get_bit(10, 0));
    // EXPECT: bit(10,1) = 1
    printf("bit(10,1) = %d\n", get_bit(10, 1));
    // EXPECT: bit(10,3) = 1
    printf("bit(10,3) = %d\n", get_bit(10, 3));

    // EXPECT: ones(0) = 0
    printf("ones(0) = %d\n", count_ones(0));
    // EXPECT: ones(7) = 3
    printf("ones(7) = %d\n", count_ones(7));
    // EXPECT: ones(255) = 8
    printf("ones(255) = %d\n", count_ones(255));

    // EXPECT: rev(6,4) = 6
    printf("rev(6,4) = %d\n", reverse_bits(6, 4));
    // EXPECT: rev(13,4) = 11
    printf("rev(13,4) = %d\n", reverse_bits(13, 4));

    int bits[4];
    bits[0] = 1; bits[1] = 0; bits[2] = 1; bits[3] = 1;
    // EXPECT: from_bin = 11
    printf("from_bin = %d\n", from_binary_array(bits, 4));

    // EXPECT: oct(255,0) = 7
    printf("oct(255,0) = %d\n", get_octal_digit(255, 0));
    // EXPECT: oct(255,1) = 7
    printf("oct(255,1) = %d\n", get_octal_digit(255, 1));
    // EXPECT: oct_digs(255) = 3
    printf("oct_digs(255) = %d\n", count_octal_digits(255));

    // EXPECT: hex(255,0) = 15
    printf("hex(255,0) = %d\n", get_hex_digit(255, 0));
    // EXPECT: hex(256,2) = 1
    printf("hex(256,2) = %d\n", get_hex_digit(256, 2));

    int d3[3];
    d3[0] = 1; d3[1] = 2; d3[2] = 0;
    // EXPECT: from_b3 = 15
    printf("from_b3 = %d\n", from_base(d3, 3, 3));

    // EXPECT: dsum_b2(10) = 2
    printf("dsum_b2(10) = %d\n", to_base_digit_sum(10, 2));
    // EXPECT: dsum_b10(123) = 6
    printf("dsum_b10(123) = %d\n", to_base_digit_sum(123, 10));

    // EXPECT: pow2(16) = 1
    printf("pow2(16) = %d\n", is_power_of_two(16));
    // EXPECT: pow2(15) = 0
    printf("pow2(15) = %d\n", is_power_of_two(15));

    // EXPECT: hsb(10) = 3
    printf("hsb(10) = %d\n", highest_set_bit(10));

    return 0;
}
