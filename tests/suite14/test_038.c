int printf(const char *fmt, ...);

/* BCD (Binary Coded Decimal) arithmetic */

/* Convert integer to BCD (packed: 2 digits per byte, up to 8 digits) */
int int_to_bcd(int val) {
    int bcd = 0;
    int shift = 0;
    int v = val;
    if (v < 0) v = -v;
    while (v > 0) {
        int digit = v % 10;
        bcd = bcd | (digit << shift);
        shift = shift + 4;
        v = v / 10;
    }
    return bcd;
}

/* Convert BCD back to integer */
int bcd_to_int(int bcd) {
    int result = 0;
    int multiplier = 1;
    int b = bcd;
    while (b > 0) {
        int digit = b & 0xF;
        result = result + digit * multiplier;
        multiplier = multiplier * 10;
        b = b >> 4;
    }
    return result;
}

/* Count BCD digits */
int bcd_digits(int bcd) {
    int count = 0;
    int b = bcd;
    while (b > 0) {
        count = count + 1;
        b = b >> 4;
    }
    if (count == 0) count = 1;
    return count;
}

/* Get a specific BCD digit (0 = least significant) */
int bcd_get_digit(int bcd, int pos) {
    return (bcd >> (pos * 4)) & 0xF;
}

/* BCD addition (digit-by-digit with carry) */
int bcd_add(int a, int b) {
    int result = 0;
    int carry = 0;
    int shift = 0;
    int va = a;
    int vb = b;
    int i;
    for (i = 0; i < 8; i++) {
        int da = va & 0xF;
        int db = vb & 0xF;
        int sum = da + db + carry;
        if (sum >= 10) {
            sum = sum - 10;
            carry = 1;
        } else {
            carry = 0;
        }
        result = result | (sum << shift);
        shift = shift + 4;
        va = va >> 4;
        vb = vb >> 4;
        if (va == 0 && vb == 0 && carry == 0) break;
    }
    if (carry) {
        result = result | (carry << shift);
    }
    return result;
}

/* BCD subtraction (a - b, assumes a >= b) */
int bcd_sub(int a, int b) {
    int result = 0;
    int borrow = 0;
    int shift = 0;
    int va = a;
    int vb = b;
    int i;
    for (i = 0; i < 8; i++) {
        int da = va & 0xF;
        int db = vb & 0xF;
        int diff = da - db - borrow;
        if (diff < 0) {
            diff = diff + 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result = result | (diff << shift);
        shift = shift + 4;
        va = va >> 4;
        vb = vb >> 4;
        if (va == 0 && vb == 0 && borrow == 0) break;
    }
    return result;
}

/* BCD multiply by single digit */
int bcd_mul_digit(int bcd, int digit) {
    int result = 0;
    int carry = 0;
    int shift = 0;
    int v = bcd;
    int i;
    for (i = 0; i < 8; i++) {
        int d = v & 0xF;
        int prod = d * digit + carry;
        carry = prod / 10;
        int rem = prod % 10;
        result = result | (rem << shift);
        shift = shift + 4;
        v = v >> 4;
        if (v == 0 && carry == 0) break;
    }
    if (carry > 0) {
        result = result | (carry << shift);
    }
    return result;
}

/* Validate BCD (all nibbles 0-9) */
int bcd_valid(int bcd) {
    int b = bcd;
    while (b > 0) {
        if ((b & 0xF) > 9) return 0;
        b = b >> 4;
    }
    return 1;
}

int main(void) {
    /* int to BCD */
    int bcd1 = int_to_bcd(42);
    printf("bcd 42: %d\n", bcd1);
    // EXPECT: bcd 42: 66

    /* 42 -> BCD: 4 in upper nibble, 2 in lower = 0x42 = 66 */

    int bcd2 = int_to_bcd(1234);
    printf("bcd 1234: %d\n", bcd2);
    // EXPECT: bcd 1234: 4660

    /* 1234 -> BCD: 0x1234 = 4096 + 512 + 48 + 4 = 4660 */

    /* BCD to int */
    int val1 = bcd_to_int(66);
    printf("from bcd 66: %d\n", val1);
    // EXPECT: from bcd 66: 42

    int val2 = bcd_to_int(4660);
    printf("from bcd 4660: %d\n", val2);
    // EXPECT: from bcd 4660: 1234

    /* Roundtrip */
    int rt = bcd_to_int(int_to_bcd(9876));
    printf("roundtrip 9876: %d\n", rt);
    // EXPECT: roundtrip 9876: 9876

    /* BCD digits */
    printf("digits of 42: %d\n", bcd_digits(int_to_bcd(42)));
    // EXPECT: digits of 42: 2

    printf("digits of 1234: %d\n", bcd_digits(int_to_bcd(1234)));
    // EXPECT: digits of 1234: 4

    /* Get digit */
    int bcd42 = int_to_bcd(42);
    printf("42 digit 0: %d\n", bcd_get_digit(bcd42, 0));
    // EXPECT: 42 digit 0: 2
    printf("42 digit 1: %d\n", bcd_get_digit(bcd42, 1));
    // EXPECT: 42 digit 1: 4

    /* BCD addition: 42 + 37 = 79 */
    int sum1 = bcd_add(int_to_bcd(42), int_to_bcd(37));
    printf("42+37: %d\n", bcd_to_int(sum1));
    // EXPECT: 42+37: 79

    /* BCD addition with carry: 99 + 1 = 100 */
    int sum2 = bcd_add(int_to_bcd(99), int_to_bcd(1));
    printf("99+1: %d\n", bcd_to_int(sum2));
    // EXPECT: 99+1: 100

    /* BCD addition: 456 + 789 = 1245 */
    int sum3 = bcd_add(int_to_bcd(456), int_to_bcd(789));
    printf("456+789: %d\n", bcd_to_int(sum3));
    // EXPECT: 456+789: 1245

    /* BCD subtraction: 79 - 42 = 37 */
    int diff1 = bcd_sub(int_to_bcd(79), int_to_bcd(42));
    printf("79-42: %d\n", bcd_to_int(diff1));
    // EXPECT: 79-42: 37

    /* BCD subtraction with borrow: 100 - 1 = 99 */
    int diff2 = bcd_sub(int_to_bcd(100), int_to_bcd(1));
    printf("100-1: %d\n", bcd_to_int(diff2));
    // EXPECT: 100-1: 99

    /* BCD multiply by digit: 123 * 4 = 492 */
    int prod1 = bcd_mul_digit(int_to_bcd(123), 4);
    printf("123*4: %d\n", bcd_to_int(prod1));
    // EXPECT: 123*4: 492

    /* BCD multiply: 99 * 9 = 891 */
    int prod2 = bcd_mul_digit(int_to_bcd(99), 9);
    printf("99*9: %d\n", bcd_to_int(prod2));
    // EXPECT: 99*9: 891

    /* Validate BCD */
    printf("valid 0x42: %d\n", bcd_valid(0x42));
    // EXPECT: valid 0x42: 1
    printf("valid 0x4A: %d\n", bcd_valid(0x4A));
    // EXPECT: valid 0x4A: 0

    /* int_to_bcd always produces valid BCD */
    int i;
    int all_valid = 1;
    for (i = 0; i < 1000; i = i + 123) {
        if (!bcd_valid(int_to_bcd(i))) all_valid = 0;
    }
    printf("all valid: %d\n", all_valid);
    // EXPECT: all valid: 1

    /* BCD addition is commutative */
    int sa = bcd_add(int_to_bcd(123), int_to_bcd(456));
    int sb = bcd_add(int_to_bcd(456), int_to_bcd(123));
    printf("commutative: %d\n", bcd_to_int(sa) == bcd_to_int(sb));
    // EXPECT: commutative: 1

    printf("done\n");
    // EXPECT: done
    return 0;
}
