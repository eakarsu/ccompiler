int printf(const char *fmt, ...);

// Convert decimal to another base, store digits in array
// Returns number of digits
int to_base(int n, int base, int *digits) {
    int count;
    int neg;
    neg = 0;
    if (n < 0) {
        neg = 1;
        n = -n;
    }
    if (n == 0) {
        digits[0] = 0;
        return 1;
    }
    count = 0;
    while (n > 0) {
        digits[count] = n % base;
        n = n / base;
        count = count + 1;
    }
    // Reverse the digits
    int i;
    int temp;
    for (i = 0; i < count / 2; i++) {
        temp = digits[i];
        digits[i] = digits[count - 1 - i];
        digits[count - 1 - i] = temp;
    }
    return count;
}

// Convert from base to decimal
int from_base(int *digits, int count, int base) {
    int result;
    int i;
    result = 0;
    for (i = 0; i < count; i++) {
        result = result * base + digits[i];
    }
    return result;
}

// Print number in given base (up to base 16)
void print_in_base(int n, int base) {
    int digits[64];
    int count;
    int i;
    char hex_chars[17];
    hex_chars[0] = '0'; hex_chars[1] = '1'; hex_chars[2] = '2';
    hex_chars[3] = '3'; hex_chars[4] = '4'; hex_chars[5] = '5';
    hex_chars[6] = '6'; hex_chars[7] = '7'; hex_chars[8] = '8';
    hex_chars[9] = '9'; hex_chars[10] = 'A'; hex_chars[11] = 'B';
    hex_chars[12] = 'C'; hex_chars[13] = 'D'; hex_chars[14] = 'E';
    hex_chars[15] = 'F'; hex_chars[16] = 0;

    if (n == 0) {
        printf("0");
        return;
    }
    count = to_base(n, base, digits);
    for (i = 0; i < count; i++) {
        char buf[2];
        buf[0] = hex_chars[digits[i]];
        buf[1] = 0;
        printf("%s", buf);
    }
}

// Count digits in a given base
int count_digits_in_base(int n, int base) {
    int count;
    if (n == 0) return 1;
    if (n < 0) n = -n;
    count = 0;
    while (n > 0) {
        count = count + 1;
        n = n / base;
    }
    return count;
}

// Check if a number is a palindrome in given base
int is_palindrome_in_base(int n, int base) {
    int digits[64];
    int count;
    int i;
    count = to_base(n, base, digits);
    for (i = 0; i < count / 2; i++) {
        if (digits[i] != digits[count - 1 - i]) return 0;
    }
    return 1;
}

// Sum digits in given base
int digit_sum_in_base(int n, int base) {
    int s;
    if (n < 0) n = -n;
    if (n == 0) return 0;
    s = 0;
    while (n > 0) {
        s = s + n % base;
        n = n / base;
    }
    return s;
}

// Convert between two arbitrary bases using decimal as intermediate
int convert_base(int *src_digits, int src_count, int src_base, int dst_base, int *dst_digits) {
    int decimal;
    decimal = from_base(src_digits, src_count, src_base);
    return to_base(decimal, dst_base, dst_digits);
}

// BCD (Binary-Coded Decimal) encoding: each decimal digit -> 4 bits
int to_bcd(int n) {
    int result;
    int shift;
    result = 0;
    shift = 0;
    while (n > 0) {
        result = result | ((n % 10) << shift);
        n = n / 10;
        shift = shift + 4;
    }
    return result;
}

int from_bcd(int bcd) {
    int result;
    int multiplier;
    result = 0;
    multiplier = 1;
    while (bcd > 0) {
        result = result + (bcd & 15) * multiplier;
        bcd = bcd >> 4;
        multiplier = multiplier * 10;
    }
    return result;
}

int main() {
    int digits[64];
    int count;
    int dst[64];
    int dst_count;

    // EXPECT: 255 in base 2: 11111111
    printf("255 in base 2: ");
    print_in_base(255, 2);
    printf("\n");

    // EXPECT: 255 in base 16: FF
    printf("255 in base 16: ");
    print_in_base(255, 16);
    printf("\n");

    // EXPECT: 100 in base 8: 144
    printf("100 in base 8: ");
    print_in_base(100, 8);
    printf("\n");

    // EXPECT: 42 in base 3: 1120
    printf("42 in base 3: ");
    print_in_base(42, 3);
    printf("\n");

    digits[0] = 1; digits[1] = 0; digits[2] = 1; digits[3] = 0;
    // EXPECT: 1010 base 2 = 10
    printf("1010 base 2 = %d\n", from_base(digits, 4, 2));

    digits[0] = 7; digits[1] = 7;
    // EXPECT: 77 base 8 = 63
    printf("77 base 8 = %d\n", from_base(digits, 2, 8));

    // EXPECT: digits_base(255, 2) = 8
    printf("digits_base(255, 2) = %d\n", count_digits_in_base(255, 2));

    // EXPECT: digits_base(999, 10) = 3
    printf("digits_base(999, 10) = %d\n", count_digits_in_base(999, 10));

    // EXPECT: palindrome_base(9, 2) = 1
    printf("palindrome_base(9, 2) = %d\n", is_palindrome_in_base(9, 2));

    // EXPECT: palindrome_base(10, 2) = 0
    printf("palindrome_base(10, 2) = %d\n", is_palindrome_in_base(10, 2));

    // EXPECT: palindrome_base(121, 10) = 1
    printf("palindrome_base(121, 10) = %d\n", is_palindrome_in_base(121, 10));

    // EXPECT: dsum_base(255, 16) = 30
    printf("dsum_base(255, 16) = %d\n", digit_sum_in_base(255, 16));

    // EXPECT: dsum_base(100, 2) = 3
    printf("dsum_base(100, 2) = %d\n", digit_sum_in_base(100, 2));

    // Convert binary 11001 to octal
    digits[0] = 1; digits[1] = 1; digits[2] = 0; digits[3] = 0; digits[4] = 1;
    dst_count = convert_base(digits, 5, 2, 8, dst);
    // EXPECT: 11001 bin -> oct: 31
    printf("11001 bin -> oct: ");
    int i;
    for (i = 0; i < dst_count; i++) {
        printf("%d", dst[i]);
    }
    printf("\n");

    // EXPECT: bcd(42) = 66
    printf("bcd(42) = %d\n", to_bcd(42));

    // EXPECT: from_bcd(66) = 42
    printf("from_bcd(66) = %d\n", from_bcd(66));

    // EXPECT: bcd(99) = 153
    printf("bcd(99) = %d\n", to_bcd(99));

    // EXPECT: from_bcd(153) = 99
    printf("from_bcd(153) = %d\n", from_bcd(153));

    return 0;
}
