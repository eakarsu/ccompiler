int printf(const char *fmt, ...);
// EXPECT: === Decimal to Binary ===
// EXPECT: 0 -> binary: 0
// EXPECT: 1 -> binary: 1
// EXPECT: 5 -> binary: 101
// EXPECT: 13 -> binary: 1101
// EXPECT: 42 -> binary: 101010
// EXPECT: 255 -> binary: 11111111
// EXPECT: 1024 -> binary: 10000000000
// EXPECT: === Decimal to Octal ===
// EXPECT: 0 -> octal: 0
// EXPECT: 7 -> octal: 7
// EXPECT: 8 -> octal: 10
// EXPECT: 64 -> octal: 100
// EXPECT: 255 -> octal: 377
// EXPECT: 511 -> octal: 777
// EXPECT: 4096 -> octal: 10000
// EXPECT: === Decimal to Hex ===
// EXPECT: 0 -> hex: 0
// EXPECT: 9 -> hex: 9
// EXPECT: 10 -> hex: A
// EXPECT: 15 -> hex: F
// EXPECT: 16 -> hex: 10
// EXPECT: 255 -> hex: FF
// EXPECT: 256 -> hex: 100
// EXPECT: 4095 -> hex: FFF
// EXPECT: 65535 -> hex: FFFF
// EXPECT: === Binary to Decimal ===
// EXPECT: binary 0 = 0
// EXPECT: binary 1 = 1
// EXPECT: binary 1010 = 10
// EXPECT: binary 11111111 = 255
// EXPECT: binary 10000000000 = 1024
// EXPECT: === Octal to Decimal ===
// EXPECT: octal 0 = 0
// EXPECT: octal 7 = 7
// EXPECT: octal 10 = 8
// EXPECT: octal 377 = 255
// EXPECT: octal 777 = 511
// EXPECT: === Hex to Decimal ===
// EXPECT: hex 0 = 0
// EXPECT: hex F = 15
// EXPECT: hex FF = 255
// EXPECT: hex 100 = 256
// EXPECT: hex FFF = 4095
// EXPECT: === Arbitrary Base Conversion ===
// EXPECT: 100 base10 -> base2: 1100100
// EXPECT: 100 base10 -> base3: 10201
// EXPECT: 100 base10 -> base5: 400
// EXPECT: 100 base10 -> base7: 202
// EXPECT: 100 base10 -> base8: 144
// EXPECT: 100 base10 -> base9: 121
// EXPECT: 255 base10 -> base4: 3333
// EXPECT: 255 base10 -> base6: 1103
// EXPECT: === Base to Base Conversion ===
// EXPECT: binary 11010 -> octal: 32
// EXPECT: binary 11111111 -> hex: FF
// EXPECT: octal 377 -> binary: 11111111
// EXPECT: octal 52 -> decimal: 42
// EXPECT: hex FF -> binary: 11111111
// EXPECT: hex 1A -> decimal: 26
// EXPECT: === BCD Representation ===
// EXPECT: 0 -> BCD: 0000
// EXPECT: 5 -> BCD: 0101
// EXPECT: 9 -> BCD: 1001
// EXPECT: 12 -> BCD: 0001 0010
// EXPECT: 42 -> BCD: 0100 0010
// EXPECT: 99 -> BCD: 1001 1001
// EXPECT: 123 -> BCD: 0001 0010 0011
// EXPECT: 255 -> BCD: 0010 0101 0101
// EXPECT: 9876 -> BCD: 1001 1000 0111 0110
// EXPECT: === BCD to Decimal ===
// EXPECT: BCD 0100 0010 = 42
// EXPECT: BCD 1001 1001 = 99
// EXPECT: BCD 0001 0010 0011 = 123
// EXPECT: === Conversion Verification ===
// EXPECT: Round-trip 42: dec->bin->dec = 42 OK
// EXPECT: Round-trip 255: dec->oct->dec = 255 OK
// EXPECT: Round-trip 4095: dec->hex->dec = 4095 OK
// EXPECT: Round-trip 100: dec->base5->dec = 100 OK
// EXPECT: Round-trip 1000: dec->base7->dec = 1000 OK
// EXPECT: === Power of Two Table ===
// EXPECT: 2^0 = 1 (bin: 1)
// EXPECT: 2^1 = 2 (bin: 10)
// EXPECT: 2^2 = 4 (bin: 100)
// EXPECT: 2^3 = 8 (bin: 1000)
// EXPECT: 2^4 = 16 (bin: 10000)
// EXPECT: 2^5 = 32 (bin: 100000)
// EXPECT: 2^6 = 64 (bin: 1000000)
// EXPECT: 2^7 = 128 (bin: 10000000)
// EXPECT: 2^8 = 256 (bin: 100000000)
// EXPECT: 2^9 = 512 (bin: 1000000000)
// EXPECT: 2^10 = 1024 (bin: 10000000000)
// EXPECT: === All base conversion tests passed ===

// Buffer for digit storage
int digits[64];

// Convert decimal to given base, store digits, return count
int dec_to_base(int num, int base, int *buf) {
    int count;
    int i;
    int j;
    int temp;
    count = 0;
    if (num == 0) {
        buf[0] = 0;
        return 1;
    }
    while (num > 0) {
        buf[count] = num - (num / base) * base;
        num = num / base;
        count = count + 1;
    }
    // Reverse the digits
    i = 0;
    j = count - 1;
    while (i < j) {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
        i = i + 1;
        j = j - 1;
    }
    return count;
}

// Print digits array as a number (handles bases up to 16)
void print_digits(int *buf, int count) {
    int i;
    for (i = 0; i < count; i = i + 1) {
        if (buf[i] < 10) {
            printf("%d", buf[i]);
        } else {
            // A=10, B=11, ..., F=15
            printf("%c", 'A' + buf[i] - 10);
        }
    }
}

// Convert array of digits in given base to decimal
int base_to_dec(int *buf, int count, int base) {
    int result;
    int i;
    result = 0;
    for (i = 0; i < count; i = i + 1) {
        result = result * base + buf[i];
    }
    return result;
}

// Convert a decimal number to binary and print
void print_binary(int num) {
    int buf[32];
    int count;
    count = dec_to_base(num, 2, buf);
    print_digits(buf, count);
}

// Convert a decimal number to octal and print
void print_octal(int num) {
    int buf[32];
    int count;
    count = dec_to_base(num, 8, buf);
    print_digits(buf, count);
}

// Convert a decimal number to hex and print
void print_hex(int num) {
    int buf[32];
    int count;
    count = dec_to_base(num, 16, buf);
    print_digits(buf, count);
}

// Convert a decimal number to arbitrary base and print
void print_base(int num, int base) {
    int buf[64];
    int count;
    count = dec_to_base(num, base, buf);
    print_digits(buf, count);
}

// Parse binary string (as array of int 0/1) to decimal
int parse_binary(int *bits, int len) {
    return base_to_dec(bits, len, 2);
}

// Parse octal digits array to decimal
int parse_octal(int *oct, int len) {
    return base_to_dec(oct, len, 8);
}

// Parse hex digits array (0-15) to decimal
int parse_hex(int *hex, int len) {
    return base_to_dec(hex, len, 16);
}

// BCD: encode a decimal number, 4 bits per digit
// bcd_buf stores 4-bit groups, bcd_groups returns number of groups
int bcd_buf[32];

int encode_bcd(int num, int *buf) {
    int dec_digits[10];
    int num_digits;
    int i;
    int j;
    int temp;
    int d;

    if (num == 0) {
        // Single digit 0 -> 0000
        buf[0] = 0;
        buf[1] = 0;
        buf[2] = 0;
        buf[3] = 0;
        return 1;
    }

    // Extract decimal digits
    num_digits = 0;
    temp = num;
    while (temp > 0) {
        dec_digits[num_digits] = temp - (temp / 10) * 10;
        temp = temp / 10;
        num_digits = num_digits + 1;
    }

    // Reverse decimal digits (so MSD first)
    i = 0;
    j = num_digits - 1;
    while (i < j) {
        temp = dec_digits[i];
        dec_digits[i] = dec_digits[j];
        dec_digits[j] = temp;
        i = i + 1;
        j = j - 1;
    }

    // Encode each decimal digit as 4 bits
    for (d = 0; d < num_digits; d = d + 1) {
        int val;
        val = dec_digits[d];
        buf[d * 4 + 0] = (val / 8) - ((val / 8) / 2) * 2;
        // Actually let's just do it properly
        buf[d * 4 + 3] = val - (val / 2) * 2;
        val = val / 2;
        buf[d * 4 + 2] = val - (val / 2) * 2;
        val = val / 2;
        buf[d * 4 + 1] = val - (val / 2) * 2;
        val = val / 2;
        buf[d * 4 + 0] = val - (val / 2) * 2;
    }

    return num_digits;
}

void print_bcd(int *buf, int num_groups) {
    int g;
    int b;
    for (g = 0; g < num_groups; g = g + 1) {
        if (g > 0) printf(" ");
        for (b = 0; b < 4; b = b + 1) {
            printf("%d", buf[g * 4 + b]);
        }
    }
}

// Decode BCD back to decimal
int decode_bcd(int *buf, int num_groups) {
    int result;
    int g;
    int val;
    result = 0;
    for (g = 0; g < num_groups; g = g + 1) {
        val = buf[g * 4 + 0] * 8 + buf[g * 4 + 1] * 4 +
              buf[g * 4 + 2] * 2 + buf[g * 4 + 3];
        result = result * 10 + val;
    }
    return result;
}

// Helper: convert between arbitrary bases
// Converts from base_from digits to base_to digits via decimal intermediate
int convert_base(int *src, int src_len, int base_from, int *dest, int base_to) {
    int decimal;
    int dest_len;
    decimal = base_to_dec(src, src_len, base_from);
    dest_len = dec_to_base(decimal, base_to, dest);
    return dest_len;
}

// Print binary representation of digits array
void print_binary_digits(int *buf, int count) {
    int i;
    for (i = 0; i < count; i = i + 1) {
        printf("%d", buf[i]);
    }
}

// Print octal representation of digits array
void print_octal_digits(int *buf, int count) {
    int i;
    for (i = 0; i < count; i = i + 1) {
        printf("%d", buf[i]);
    }
}

// Print hex representation of digits array
void print_hex_digits(int *buf, int count) {
    int i;
    for (i = 0; i < count; i = i + 1) {
        if (buf[i] < 10) {
            printf("%d", buf[i]);
        } else {
            printf("%c", 'A' + buf[i] - 10);
        }
    }
}

// Round-trip test: decimal -> base -> decimal
int round_trip(int num, int base) {
    int buf[64];
    int count;
    int result;
    count = dec_to_base(num, base, buf);
    result = base_to_dec(buf, count, base);
    return result;
}

// Power function
int power(int base, int exp) {
    int result;
    int i;
    result = 1;
    for (i = 0; i < exp; i = i + 1) {
        result = result * base;
    }
    return result;
}

// Absolute value
int abs_val(int x) {
    if (x < 0) return 0 - x;
    return x;
}

// GCD for base math utilities
int gcd(int a, int b) {
    while (b != 0) {
        int t;
        t = b;
        b = a - (a / b) * b;
        a = t;
    }
    return a;
}

// Count number of digits in a given base
int count_digits(int num, int base) {
    int count;
    if (num == 0) return 1;
    count = 0;
    while (num > 0) {
        count = count + 1;
        num = num / base;
    }
    return count;
}

// Check if number is a palindrome in given base
int is_palindrome_in_base(int num, int base) {
    int buf[64];
    int count;
    int i;
    int j;
    count = dec_to_base(num, base, buf);
    i = 0;
    j = count - 1;
    while (i < j) {
        if (buf[i] != buf[j]) return 0;
        i = i + 1;
        j = j - 1;
    }
    return 1;
}

// Add two numbers represented as digit arrays in a given base
int add_in_base(int *a, int alen, int *b, int blen, int *result, int base) {
    int carry;
    int i;
    int j;
    int k;
    int sum;
    int rlen;
    int temp[64];

    carry = 0;
    i = alen - 1;
    j = blen - 1;
    k = 0;

    while (i >= 0 || j >= 0 || carry > 0) {
        sum = carry;
        if (i >= 0) {
            sum = sum + a[i];
            i = i - 1;
        }
        if (j >= 0) {
            sum = sum + b[j];
            j = j - 1;
        }
        temp[k] = sum - (sum / base) * base;
        carry = sum / base;
        k = k + 1;
    }

    // Reverse into result
    rlen = k;
    for (i = 0; i < rlen; i = i + 1) {
        result[i] = temp[rlen - 1 - i];
    }
    return rlen;
}

// Subtract b from a in given base (assumes a >= b)
int sub_in_base(int *a, int alen, int *b, int blen, int *result, int base) {
    int borrow;
    int i;
    int j;
    int k;
    int diff;
    int temp[64];
    int rlen;

    borrow = 0;
    i = alen - 1;
    j = blen - 1;
    k = 0;

    while (i >= 0) {
        diff = a[i] - borrow;
        if (j >= 0) {
            diff = diff - b[j];
            j = j - 1;
        }
        if (diff < 0) {
            diff = diff + base;
            borrow = 1;
        } else {
            borrow = 0;
        }
        temp[k] = diff;
        k = k + 1;
        i = i - 1;
    }

    // Reverse and remove leading zeros
    rlen = k;
    // Remove leading zeros from reversed
    while (rlen > 1 && temp[rlen - 1] == 0) {
        rlen = rlen - 1;
    }
    for (i = 0; i < rlen; i = i + 1) {
        result[i] = temp[rlen - 1 - i];
    }
    return rlen;
}

// Multiply number in base by single digit
int mul_single_base(int *a, int alen, int digit, int *result, int base) {
    int carry;
    int i;
    int prod;
    int temp[64];
    int rlen;

    carry = 0;
    for (i = alen - 1; i >= 0; i = i - 1) {
        prod = a[i] * digit + carry;
        temp[alen - 1 - i] = prod - (prod / base) * base;
        carry = prod / base;
    }
    rlen = alen;
    if (carry > 0) {
        temp[rlen] = carry;
        rlen = rlen + 1;
    }
    // Reverse
    for (i = 0; i < rlen; i = i + 1) {
        result[i] = temp[rlen - 1 - i];
    }
    return rlen;
}

// Compare two digit arrays (same base)
// Returns: 1 if a > b, -1 if a < b, 0 if equal
int compare_digits(int *a, int alen, int *b, int blen) {
    int i;
    if (alen != blen) {
        if (alen > blen) return 1;
        return -1;
    }
    for (i = 0; i < alen; i = i + 1) {
        if (a[i] > b[i]) return 1;
        if (a[i] < b[i]) return -1;
    }
    return 0;
}

// Check if a digit array represents zero
int is_zero(int *buf, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (buf[i] != 0) return 0;
    }
    return 1;
}

// Copy digit array
void copy_digits(int *dest, int *src, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        dest[i] = src[i];
    }
}

// Shift digit array left by n positions (multiply by base^n)
int shift_left(int *buf, int len, int n, int *result) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        result[i] = buf[i];
    }
    for (i = 0; i < n; i = i + 1) {
        result[len + i] = 0;
    }
    return len + n;
}

int main(void) {
    int i;
    int j;
    int buf[64];
    int buf2[64];
    int count;
    int val;

    // === Decimal to Binary ===
    printf("=== Decimal to Binary ===\n");
    {
        int test_vals[7];
        test_vals[0] = 0;
        test_vals[1] = 1;
        test_vals[2] = 5;
        test_vals[3] = 13;
        test_vals[4] = 42;
        test_vals[5] = 255;
        test_vals[6] = 1024;
        for (i = 0; i < 7; i = i + 1) {
            printf("%d -> binary: ", test_vals[i]);
            print_binary(test_vals[i]);
            printf("\n");
        }
    }

    // === Decimal to Octal ===
    printf("=== Decimal to Octal ===\n");
    {
        int test_vals[7];
        test_vals[0] = 0;
        test_vals[1] = 7;
        test_vals[2] = 8;
        test_vals[3] = 64;
        test_vals[4] = 255;
        test_vals[5] = 511;
        test_vals[6] = 4096;
        for (i = 0; i < 7; i = i + 1) {
            printf("%d -> octal: ", test_vals[i]);
            print_octal(test_vals[i]);
            printf("\n");
        }
    }

    // === Decimal to Hex ===
    printf("=== Decimal to Hex ===\n");
    {
        int test_vals[9];
        test_vals[0] = 0;
        test_vals[1] = 9;
        test_vals[2] = 10;
        test_vals[3] = 15;
        test_vals[4] = 16;
        test_vals[5] = 255;
        test_vals[6] = 256;
        test_vals[7] = 4095;
        test_vals[8] = 65535;
        for (i = 0; i < 9; i = i + 1) {
            printf("%d -> hex: ", test_vals[i]);
            print_hex(test_vals[i]);
            printf("\n");
        }
    }

    // === Binary to Decimal ===
    printf("=== Binary to Decimal ===\n");
    {
        // 0
        int b0[1];
        b0[0] = 0;
        printf("binary 0 = %d\n", parse_binary(b0, 1));
        // 1
        int b1[1];
        b1[0] = 1;
        printf("binary 1 = %d\n", parse_binary(b1, 1));
        // 1010 = 10
        int b2[4];
        b2[0] = 1; b2[1] = 0; b2[2] = 1; b2[3] = 0;
        printf("binary 1010 = %d\n", parse_binary(b2, 4));
        // 11111111 = 255
        int b3[8];
        for (i = 0; i < 8; i = i + 1) b3[i] = 1;
        printf("binary 11111111 = %d\n", parse_binary(b3, 8));
        // 10000000000 = 1024
        int b4[11];
        b4[0] = 1;
        for (i = 1; i < 11; i = i + 1) b4[i] = 0;
        printf("binary 10000000000 = %d\n", parse_binary(b4, 11));
    }

    // === Octal to Decimal ===
    printf("=== Octal to Decimal ===\n");
    {
        int o1[1];
        o1[0] = 0;
        printf("octal 0 = %d\n", parse_octal(o1, 1));
        int o2[1];
        o2[0] = 7;
        printf("octal 7 = %d\n", parse_octal(o2, 1));
        int o3[2];
        o3[0] = 1; o3[1] = 0;
        printf("octal 10 = %d\n", parse_octal(o3, 2));
        int o4[3];
        o4[0] = 3; o4[1] = 7; o4[2] = 7;
        printf("octal 377 = %d\n", parse_octal(o4, 3));
        int o5[3];
        o5[0] = 7; o5[1] = 7; o5[2] = 7;
        printf("octal 777 = %d\n", parse_octal(o5, 3));
    }

    // === Hex to Decimal ===
    printf("=== Hex to Decimal ===\n");
    {
        int h1[1];
        h1[0] = 0;
        printf("hex 0 = %d\n", parse_hex(h1, 1));
        int h2[1];
        h2[0] = 15;
        printf("hex F = %d\n", parse_hex(h2, 1));
        int h3[2];
        h3[0] = 15; h3[1] = 15;
        printf("hex FF = %d\n", parse_hex(h3, 2));
        int h4[3];
        h4[0] = 1; h4[1] = 0; h4[2] = 0;
        printf("hex 100 = %d\n", parse_hex(h4, 3));
        int h5[3];
        h5[0] = 15; h5[1] = 15; h5[2] = 15;
        printf("hex FFF = %d\n", parse_hex(h5, 3));
    }

    // === Arbitrary Base Conversion ===
    printf("=== Arbitrary Base Conversion ===\n");
    {
        int bases[6];
        bases[0] = 2; bases[1] = 3; bases[2] = 5;
        bases[3] = 7; bases[4] = 8; bases[5] = 9;
        for (i = 0; i < 6; i = i + 1) {
            printf("100 base10 -> base%d: ", bases[i]);
            print_base(100, bases[i]);
            printf("\n");
        }
        // 255 in base 4 and 6
        printf("255 base10 -> base4: ");
        print_base(255, 4);
        printf("\n");
        printf("255 base10 -> base6: ");
        print_base(255, 6);
        printf("\n");
    }

    // === Base to Base Conversion ===
    printf("=== Base to Base Conversion ===\n");
    {
        int src[16];
        int dest[64];
        int slen;
        int dlen;

        // binary 11010 -> octal
        src[0] = 1; src[1] = 1; src[2] = 0; src[3] = 1; src[4] = 0;
        dlen = convert_base(src, 5, 2, dest, 8);
        printf("binary 11010 -> octal: ");
        print_octal_digits(dest, dlen);
        printf("\n");

        // binary 11111111 -> hex
        for (i = 0; i < 8; i = i + 1) src[i] = 1;
        dlen = convert_base(src, 8, 2, dest, 16);
        printf("binary 11111111 -> hex: ");
        print_hex_digits(dest, dlen);
        printf("\n");

        // octal 377 -> binary
        src[0] = 3; src[1] = 7; src[2] = 7;
        dlen = convert_base(src, 3, 8, dest, 2);
        printf("octal 377 -> binary: ");
        print_binary_digits(dest, dlen);
        printf("\n");

        // octal 52 -> decimal
        src[0] = 5; src[1] = 2;
        dlen = convert_base(src, 2, 8, dest, 10);
        printf("octal 52 -> decimal: ");
        print_digits(dest, dlen);
        printf("\n");

        // hex FF -> binary
        src[0] = 15; src[1] = 15;
        dlen = convert_base(src, 2, 16, dest, 2);
        printf("hex FF -> binary: ");
        print_binary_digits(dest, dlen);
        printf("\n");

        // hex 1A -> decimal (1=1, A=10)
        src[0] = 1; src[1] = 10;
        dlen = convert_base(src, 2, 16, dest, 10);
        printf("hex 1A -> decimal: ");
        print_digits(dest, dlen);
        printf("\n");
    }

    // === BCD Representation ===
    printf("=== BCD Representation ===\n");
    {
        int bcd[64];
        int groups;
        int test_vals[9];
        test_vals[0] = 0;
        test_vals[1] = 5;
        test_vals[2] = 9;
        test_vals[3] = 12;
        test_vals[4] = 42;
        test_vals[5] = 99;
        test_vals[6] = 123;
        test_vals[7] = 255;
        test_vals[8] = 9876;

        for (i = 0; i < 9; i = i + 1) {
            groups = encode_bcd(test_vals[i], bcd);
            printf("%d -> BCD: ", test_vals[i]);
            print_bcd(bcd, groups);
            printf("\n");
        }
    }

    // === BCD to Decimal ===
    printf("=== BCD to Decimal ===\n");
    {
        int bcd[64];
        int groups;
        int decoded;

        // 42 = 0100 0010
        groups = encode_bcd(42, bcd);
        decoded = decode_bcd(bcd, groups);
        printf("BCD 0100 0010 = %d\n", decoded);

        // 99 = 1001 1001
        groups = encode_bcd(99, bcd);
        decoded = decode_bcd(bcd, groups);
        printf("BCD 1001 1001 = %d\n", decoded);

        // 123 = 0001 0010 0011
        groups = encode_bcd(123, bcd);
        decoded = decode_bcd(bcd, groups);
        printf("BCD 0001 0010 0011 = %d\n", decoded);
    }

    // === Conversion Verification (Round-trip) ===
    printf("=== Conversion Verification ===\n");
    {
        int rt;
        rt = round_trip(42, 2);
        printf("Round-trip 42: dec->bin->dec = %d", rt);
        if (rt == 42) printf(" OK\n"); else printf(" FAIL\n");

        rt = round_trip(255, 8);
        printf("Round-trip 255: dec->oct->dec = %d", rt);
        if (rt == 255) printf(" OK\n"); else printf(" FAIL\n");

        rt = round_trip(4095, 16);
        printf("Round-trip 4095: dec->hex->dec = %d", rt);
        if (rt == 4095) printf(" OK\n"); else printf(" FAIL\n");

        rt = round_trip(100, 5);
        printf("Round-trip 100: dec->base5->dec = %d", rt);
        if (rt == 100) printf(" OK\n"); else printf(" FAIL\n");

        rt = round_trip(1000, 7);
        printf("Round-trip 1000: dec->base7->dec = %d", rt);
        if (rt == 1000) printf(" OK\n"); else printf(" FAIL\n");
    }

    // === Power of Two Table ===
    printf("=== Power of Two Table ===\n");
    {
        int p;
        for (i = 0; i <= 10; i = i + 1) {
            p = power(2, i);
            printf("2^%d = %d (bin: ", i, p);
            print_binary(p);
            printf(")\n");
        }
    }

    printf("=== All base conversion tests passed ===\n");
    return 0;
}
