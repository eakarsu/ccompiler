// EXPECT: === Big Integer Addition Tests ===
// EXPECT: 0 + 0 = 0
// EXPECT: 1 + 0 = 1
// EXPECT: 0 + 1 = 1
// EXPECT: 5 + 3 = 8
// EXPECT: 99 + 1 = 100
// EXPECT: 999 + 1 = 1000
// EXPECT: 123 + 456 = 579
// EXPECT: 999999999 + 1 = 1000000000
// EXPECT: 99999999999999999999 + 1 = 100000000000000000000
// EXPECT: 12345678901234567890 + 98765432109876543210 = 111111111011111111100
// EXPECT: Subtraction tests:
// EXPECT: 8 - 3 = 5
// EXPECT: 100 - 1 = 99
// EXPECT: 1000 - 1 = 999
// EXPECT: 1000000000 - 1 = 999999999
// EXPECT: 100000000000000000000 - 1 = 99999999999999999999
// EXPECT: Comparison tests:
// EXPECT: cmp(123, 456) = -1
// EXPECT: cmp(456, 123) = 1
// EXPECT: cmp(123, 123) = 0
// EXPECT: cmp(1000, 999) = 1
// EXPECT: cmp(99, 100) = -1
// EXPECT: All big integer tests passed!

int printf(const char *fmt, ...);

int str_len(char *s) {
    int len = 0;
    while (s[len] != '\0') len = len + 1;
    return len;
}

// Reverse a string in place
void str_reverse(char *s, int len) {
    int i = 0;
    int j = len - 1;
    while (i < j) {
        char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        i = i + 1;
        j = j - 1;
    }
}

// Compare two big integers represented as strings
// Returns -1, 0, 1
int bigint_cmp(char *a, char *b) {
    int la = str_len(a);
    int lb = str_len(b);
    if (la != lb) {
        return la < lb ? -1 : 1;
    }
    int i;
    for (i = 0; i < la; i++) {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
    }
    return 0;
}

// Add two big integers represented as digit strings
void bigint_add(char *a, char *b, char *result) {
    int la = str_len(a);
    int lb = str_len(b);
    int carry = 0;
    int ri = 0;
    int ia = la - 1;
    int ib = lb - 1;

    while (ia >= 0 || ib >= 0 || carry > 0) {
        int da = 0;
        int db = 0;
        if (ia >= 0) {
            da = a[ia] - '0';
            ia = ia - 1;
        }
        if (ib >= 0) {
            db = b[ib] - '0';
            ib = ib - 1;
        }
        int sum = da + db + carry;
        result[ri] = (char)('0' + sum % 10);
        carry = sum / 10;
        ri = ri + 1;
    }
    result[ri] = '\0';
    str_reverse(result, ri);
}

// Subtract b from a (assumes a >= b)
void bigint_sub(char *a, char *b, char *result) {
    int la = str_len(a);
    int lb = str_len(b);
    int borrow = 0;
    int ri = 0;
    int ia = la - 1;
    int ib = lb - 1;

    while (ia >= 0) {
        int da = a[ia] - '0';
        int db = 0;
        if (ib >= 0) {
            db = b[ib] - '0';
            ib = ib - 1;
        }
        int diff = da - db - borrow;
        if (diff < 0) {
            diff = diff + 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result[ri] = (char)('0' + diff);
        ri = ri + 1;
        ia = ia - 1;
    }

    // Remove leading zeros
    while (ri > 1 && result[ri - 1] == '0') {
        ri = ri - 1;
    }
    result[ri] = '\0';
    str_reverse(result, ri);
}

int main() {
    char result[128];

    printf("=== Big Integer Addition Tests ===\n");

    bigint_add("0", "0", result);
    printf("0 + 0 = %s\n", result);

    bigint_add("1", "0", result);
    printf("1 + 0 = %s\n", result);

    bigint_add("0", "1", result);
    printf("0 + 1 = %s\n", result);

    bigint_add("5", "3", result);
    printf("5 + 3 = %s\n", result);

    bigint_add("99", "1", result);
    printf("99 + 1 = %s\n", result);

    bigint_add("999", "1", result);
    printf("999 + 1 = %s\n", result);

    bigint_add("123", "456", result);
    printf("123 + 456 = %s\n", result);

    bigint_add("999999999", "1", result);
    printf("999999999 + 1 = %s\n", result);

    // Numbers bigger than 64-bit
    bigint_add("99999999999999999999", "1", result);
    printf("99999999999999999999 + 1 = %s\n", result);

    bigint_add("12345678901234567890", "98765432109876543210", result);
    printf("12345678901234567890 + 98765432109876543210 = %s\n", result);

    // Subtraction
    printf("Subtraction tests:\n");

    bigint_sub("8", "3", result);
    printf("8 - 3 = %s\n", result);

    bigint_sub("100", "1", result);
    printf("100 - 1 = %s\n", result);

    bigint_sub("1000", "1", result);
    printf("1000 - 1 = %s\n", result);

    bigint_sub("1000000000", "1", result);
    printf("1000000000 - 1 = %s\n", result);

    bigint_sub("100000000000000000000", "1", result);
    printf("100000000000000000000 - 1 = %s\n", result);

    // Comparison
    printf("Comparison tests:\n");
    printf("cmp(123, 456) = %d\n", bigint_cmp("123", "456"));
    printf("cmp(456, 123) = %d\n", bigint_cmp("456", "123"));
    printf("cmp(123, 123) = %d\n", bigint_cmp("123", "123"));
    printf("cmp(1000, 999) = %d\n", bigint_cmp("1000", "999"));
    printf("cmp(99, 100) = %d\n", bigint_cmp("99", "100"));

    printf("All big integer tests passed!\n");
    return 0;
}
