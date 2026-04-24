int printf(const char *fmt, ...);
// EXPECT: N1=12345 L1=5 N2=6789 A1=19134 A2=1000 A3=19998 A4=12345 S1=5556 S2=999 S3=0 M1=56088 M2=998001 M3=10000 M4=99999 M5=0 C1=-1 C2=1 C3=0 C4=1 Z1=1 Z2=0 DS1=15 DS2=27 MD1=37035 MD2=8991 SH1=12300 SH2=5000 F10=3628800 F5=120 F1=1 F12=479001600 P1=1024 P2=65536 P3=243 P4=100000 CH1=100000 CH2=12345 
/* ====================================================================== */
/* Test 046: Big Number Arithmetic                                        */
/* Addition, subtraction, multiplication using digit arrays               */
/* Digits stored in array: digits[0] = least significant digit            */
/* ====================================================================== */

/* Print big number (MSB first) */
void print_bignum(int *digits, int len) {
    int i;
    int leading;

    if (len == 0) {
        printf("0");
        return;
    }

    leading = 1;
    i = len - 1;
    while (i >= 0) {
        if (digits[i] != 0) leading = 0;
        if (leading == 0) {
            printf("%d", digits[i]);
        }
        i = i - 1;
    }
    if (leading) printf("0");
}

/* Get length (index of highest non-zero digit + 1) */
int bignum_len(int *digits, int maxlen) {
    int i;
    i = maxlen - 1;
    while (i > 0 && digits[i] == 0) {
        i = i - 1;
    }
    return i + 1;
}

/* Set bignum from int */
int int_to_bignum(int n, int *digits, int maxlen) {
    int i;
    int temp;

    i = 0;
    while (i < maxlen) {
        digits[i] = 0;
        i = i + 1;
    }

    if (n == 0) {
        digits[0] = 0;
        return 1;
    }

    temp = n;
    i = 0;
    while (temp > 0 && i < maxlen) {
        digits[i] = temp % 10;
        temp = temp / 10;
        i = i + 1;
    }
    return i;
}

/* Big number addition: result = a + b */
/* Returns length of result */
int bignum_add(int *a, int alen, int *b, int blen, int *result, int maxlen) {
    int carry;
    int i;
    int sum;
    int da;
    int db;
    int rlen;

    carry = 0;
    i = 0;
    if (alen > blen) rlen = alen;
    else rlen = blen;

    while (i < rlen || carry > 0) {
        da = 0;
        db = 0;
        if (i < alen) da = a[i];
        if (i < blen) db = b[i];
        sum = da + db + carry;
        result[i] = sum % 10;
        carry = sum / 10;
        i = i + 1;
    }

    /* Zero out remaining */
    rlen = i;
    while (i < maxlen) {
        result[i] = 0;
        i = i + 1;
    }
    return rlen;
}

/* Big number subtraction: result = a - b (assumes a >= b) */
int bignum_sub(int *a, int alen, int *b, int blen, int *result, int maxlen) {
    int borrow;
    int i;
    int diff;
    int da;
    int db;
    int rlen;

    borrow = 0;
    i = 0;

    while (i < alen) {
        da = a[i];
        db = 0;
        if (i < blen) db = b[i];
        diff = da - db - borrow;
        if (diff < 0) {
            diff = diff + 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result[i] = diff;
        i = i + 1;
    }

    rlen = alen;
    while (i < maxlen) {
        result[i] = 0;
        i = i + 1;
    }
    return bignum_len(result, rlen);
}

/* Big number multiplication: result = a * b */
int bignum_mul(int *a, int alen, int *b, int blen, int *result, int maxlen) {
    int i;
    int j;
    int carry;
    int prod;
    int rlen;

    /* Zero out result */
    i = 0;
    while (i < maxlen) {
        result[i] = 0;
        i = i + 1;
    }

    i = 0;
    while (i < alen) {
        carry = 0;
        j = 0;
        while (j < blen) {
            prod = result[i + j] + a[i] * b[j] + carry;
            result[i + j] = prod % 10;
            carry = prod / 10;
            j = j + 1;
        }
        if (carry > 0 && i + j < maxlen) {
            result[i + j] = result[i + j] + carry;
        }
        i = i + 1;
    }

    rlen = alen + blen;
    if (rlen > maxlen) rlen = maxlen;
    return bignum_len(result, rlen);
}

/* Compare two big numbers: -1 if a < b, 0 if equal, 1 if a > b */
int bignum_cmp(int *a, int alen, int *b, int blen) {
    int i;
    int la;
    int lb;

    la = bignum_len(a, alen);
    lb = bignum_len(b, blen);

    if (la > lb) return 1;
    if (la < lb) return -1;

    i = la - 1;
    while (i >= 0) {
        if (a[i] > b[i]) return 1;
        if (a[i] < b[i]) return -1;
        i = i - 1;
    }
    return 0;
}

/* Check if bignum is zero */
int bignum_is_zero(int *digits, int len) {
    int i;
    i = 0;
    while (i < len) {
        if (digits[i] != 0) return 0;
        i = i + 1;
    }
    return 1;
}

/* Sum of digits */
int bignum_digit_sum(int *digits, int len) {
    int sum;
    int i;
    sum = 0;
    i = 0;
    while (i < len) {
        sum = sum + digits[i];
        i = i + 1;
    }
    return sum;
}

/* Multiply big number by single digit */
int bignum_mul_digit(int *a, int alen, int d, int *result, int maxlen) {
    int carry;
    int i;
    int prod;

    carry = 0;
    i = 0;
    while (i < alen) {
        prod = a[i] * d + carry;
        result[i] = prod % 10;
        carry = prod / 10;
        i = i + 1;
    }
    while (carry > 0 && i < maxlen) {
        result[i] = carry % 10;
        carry = carry / 10;
        i = i + 1;
    }
    while (i < maxlen) {
        result[i] = 0;
        i = i + 1;
    }
    return bignum_len(result, maxlen);
}

/* Shift left by n digits (multiply by 10^n) */
int bignum_shift(int *a, int alen, int n, int *result, int maxlen) {
    int i;
    int rlen;

    i = 0;
    while (i < maxlen) {
        result[i] = 0;
        i = i + 1;
    }

    i = 0;
    while (i < alen && i + n < maxlen) {
        result[i + n] = a[i];
        i = i + 1;
    }

    rlen = alen + n;
    if (rlen > maxlen) rlen = maxlen;
    return bignum_len(result, rlen);
}

/* Copy bignum */
void bignum_copy(int *src, int *dst, int len) {
    int i;
    i = 0;
    while (i < len) {
        dst[i] = src[i];
        i = i + 1;
    }
}

/* Factorial using big numbers */
int bignum_factorial(int n, int *result, int maxlen) {
    int temp[50];
    int rlen;
    int i;
    int j;

    /* Initialize result = 1 */
    j = 0;
    while (j < maxlen) {
        result[j] = 0;
        j = j + 1;
    }
    result[0] = 1;
    rlen = 1;

    i = 2;
    while (i <= n) {
        rlen = bignum_mul_digit(result, rlen, i, temp, maxlen);
        bignum_copy(temp, result, maxlen);
        i = i + 1;
    }
    return bignum_len(result, maxlen);
}

/* Power: compute base^exp as bignum */
int bignum_power(int base, int exp, int *result, int maxlen) {
    int base_bn[50];
    int temp[50];
    int blen;
    int rlen;
    int i;
    int j;

    /* Init result = 1 */
    j = 0;
    while (j < maxlen) {
        result[j] = 0;
        j = j + 1;
    }
    result[0] = 1;
    rlen = 1;

    blen = int_to_bignum(base, base_bn, maxlen);

    i = 0;
    while (i < exp) {
        rlen = bignum_mul(result, rlen, base_bn, blen, temp, maxlen);
        bignum_copy(temp, result, maxlen);
        i = i + 1;
    }
    return bignum_len(result, maxlen);
}

int main(void) {
    int a[30];
    int b[30];
    int c[30];
    int result[50];
    int alen;
    int blen;
    int clen;
    int rlen;

    /* Basic int to bignum */
    alen = int_to_bignum(12345, a, 30);
    printf("N1="); print_bignum(a, alen); printf(" ");
    printf("L1=%d ", bignum_len(a, 30));

    blen = int_to_bignum(6789, b, 30);
    printf("N2="); print_bignum(b, blen); printf(" ");

    /* Addition: 12345 + 6789 = 19134 */
    rlen = bignum_add(a, alen, b, blen, result, 30);
    printf("A1="); print_bignum(result, rlen); printf(" ");

    /* More additions */
    alen = int_to_bignum(999, a, 30);
    blen = int_to_bignum(1, b, 30);
    rlen = bignum_add(a, alen, b, blen, result, 30);
    printf("A2="); print_bignum(result, rlen); printf(" ");

    alen = int_to_bignum(9999, a, 30);
    blen = int_to_bignum(9999, b, 30);
    rlen = bignum_add(a, alen, b, blen, result, 30);
    printf("A3="); print_bignum(result, rlen); printf(" ");

    alen = int_to_bignum(0, a, 30);
    blen = int_to_bignum(12345, b, 30);
    rlen = bignum_add(a, alen, b, blen, result, 30);
    printf("A4="); print_bignum(result, rlen); printf(" ");

    /* Subtraction */
    alen = int_to_bignum(12345, a, 30);
    blen = int_to_bignum(6789, b, 30);
    rlen = bignum_sub(a, alen, b, blen, result, 30);
    printf("S1="); print_bignum(result, rlen); printf(" ");

    alen = int_to_bignum(1000, a, 30);
    blen = int_to_bignum(1, b, 30);
    rlen = bignum_sub(a, alen, b, blen, result, 30);
    printf("S2="); print_bignum(result, rlen); printf(" ");

    alen = int_to_bignum(100, a, 30);
    blen = int_to_bignum(100, b, 30);
    rlen = bignum_sub(a, alen, b, blen, result, 30);
    printf("S3="); print_bignum(result, rlen); printf(" ");

    /* Multiplication */
    alen = int_to_bignum(123, a, 30);
    blen = int_to_bignum(456, b, 30);
    rlen = bignum_mul(a, alen, b, blen, result, 30);
    printf("M1="); print_bignum(result, rlen); printf(" ");

    alen = int_to_bignum(999, a, 30);
    blen = int_to_bignum(999, b, 30);
    rlen = bignum_mul(a, alen, b, blen, result, 30);
    printf("M2="); print_bignum(result, rlen); printf(" ");

    alen = int_to_bignum(100, a, 30);
    blen = int_to_bignum(100, b, 30);
    rlen = bignum_mul(a, alen, b, blen, result, 30);
    printf("M3="); print_bignum(result, rlen); printf(" ");

    alen = int_to_bignum(1, a, 30);
    blen = int_to_bignum(99999, b, 30);
    rlen = bignum_mul(a, alen, b, blen, result, 30);
    printf("M4="); print_bignum(result, rlen); printf(" ");

    alen = int_to_bignum(0, a, 30);
    blen = int_to_bignum(12345, b, 30);
    rlen = bignum_mul(a, alen, b, blen, result, 30);
    printf("M5="); print_bignum(result, rlen); printf(" ");

    /* Comparison */
    alen = int_to_bignum(123, a, 30);
    blen = int_to_bignum(456, b, 30);
    printf("C1=%d ", bignum_cmp(a, alen, b, blen));

    alen = int_to_bignum(456, a, 30);
    blen = int_to_bignum(123, b, 30);
    printf("C2=%d ", bignum_cmp(a, alen, b, blen));

    alen = int_to_bignum(123, a, 30);
    blen = int_to_bignum(123, b, 30);
    printf("C3=%d ", bignum_cmp(a, alen, b, blen));

    alen = int_to_bignum(1000, a, 30);
    blen = int_to_bignum(999, b, 30);
    printf("C4=%d ", bignum_cmp(a, alen, b, blen));

    /* Is zero */
    alen = int_to_bignum(0, a, 30);
    printf("Z1=%d ", bignum_is_zero(a, alen));
    alen = int_to_bignum(1, a, 30);
    printf("Z2=%d ", bignum_is_zero(a, alen));

    /* Digit sum */
    alen = int_to_bignum(12345, a, 30);
    printf("DS1=%d ", bignum_digit_sum(a, alen));

    alen = int_to_bignum(999, a, 30);
    printf("DS2=%d ", bignum_digit_sum(a, alen));

    /* Multiply by single digit */
    alen = int_to_bignum(12345, a, 30);
    rlen = bignum_mul_digit(a, alen, 3, result, 30);
    printf("MD1="); print_bignum(result, rlen); printf(" ");

    alen = int_to_bignum(999, a, 30);
    rlen = bignum_mul_digit(a, alen, 9, result, 30);
    printf("MD2="); print_bignum(result, rlen); printf(" ");

    /* Shift (multiply by 10^n) */
    alen = int_to_bignum(123, a, 30);
    rlen = bignum_shift(a, alen, 2, result, 30);
    printf("SH1="); print_bignum(result, rlen); printf(" ");

    alen = int_to_bignum(5, a, 30);
    rlen = bignum_shift(a, alen, 3, result, 30);
    printf("SH2="); print_bignum(result, rlen); printf(" ");

    /* Factorial */
    rlen = bignum_factorial(10, result, 30);
    printf("F10="); print_bignum(result, rlen); printf(" ");

    rlen = bignum_factorial(5, result, 30);
    printf("F5="); print_bignum(result, rlen); printf(" ");

    rlen = bignum_factorial(1, result, 30);
    printf("F1="); print_bignum(result, rlen); printf(" ");

    rlen = bignum_factorial(12, result, 30);
    printf("F12="); print_bignum(result, rlen); printf(" ");

    /* Power */
    rlen = bignum_power(2, 10, result, 30);
    printf("P1="); print_bignum(result, rlen); printf(" ");

    rlen = bignum_power(2, 16, result, 30);
    printf("P2="); print_bignum(result, rlen); printf(" ");

    rlen = bignum_power(3, 5, result, 30);
    printf("P3="); print_bignum(result, rlen); printf(" ");

    rlen = bignum_power(10, 5, result, 30);
    printf("P4="); print_bignum(result, rlen); printf(" ");

    /* Chain: (999 + 1) * 100 */
    alen = int_to_bignum(999, a, 30);
    blen = int_to_bignum(1, b, 30);
    rlen = bignum_add(a, alen, b, blen, c, 30);
    clen = rlen;
    blen = int_to_bignum(100, b, 30);
    rlen = bignum_mul(c, clen, b, blen, result, 30);
    printf("CH1="); print_bignum(result, rlen); printf(" ");

    /* Chain: 12345 * 2 - 12345 = 12345 */
    alen = int_to_bignum(12345, a, 30);
    rlen = bignum_mul_digit(a, alen, 2, c, 30);
    clen = bignum_len(c, 30);
    rlen = bignum_sub(c, clen, a, alen, result, 30);
    printf("CH2="); print_bignum(result, rlen); printf(" ");

    printf("\n");
    return 0;
}
