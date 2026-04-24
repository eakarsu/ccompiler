int printf(const char *fmt, ...);

int my_strlen(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

int my_atoi(char *s) {
    int result = 0;
    int sign = 1;
    int i = 0;

    while (s[i] == ' ') {
        i = i + 1;
    }

    if (s[i] == '-') {
        sign = -1;
        i = i + 1;
    } else if (s[i] == '+') {
        i = i + 1;
    }

    while (s[i] >= '0' && s[i] <= '9') {
        result = result * 10 + (s[i] - '0');
        i = i + 1;
    }
    return sign * result;
}

void my_itoa(int val, char *buf) {
    int negative = 0;
    int i = 0;

    if (val == 0) {
        buf[0] = '0';
        buf[1] = 0;
        return;
    }

    if (val < 0) {
        negative = 1;
        val = -val;
    }

    while (val > 0) {
        buf[i] = '0' + (val % 10);
        val = val / 10;
        i = i + 1;
    }

    if (negative) {
        buf[i] = '-';
        i = i + 1;
    }
    buf[i] = 0;

    int left = 0;
    int right = i - 1;
    while (left < right) {
        char tmp = buf[left];
        buf[left] = buf[right];
        buf[right] = tmp;
        left = left + 1;
        right = right - 1;
    }
}

void itoa_base(int val, char *buf, int base) {
    int i = 0;
    int negative = 0;

    if (val == 0) {
        buf[0] = '0';
        buf[1] = 0;
        return;
    }

    if (val < 0 && base == 10) {
        negative = 1;
        val = -val;
    }

    char digits[] = "0123456789abcdef";
    while (val > 0) {
        buf[i] = digits[val % base];
        val = val / base;
        i = i + 1;
    }

    if (negative) {
        buf[i] = '-';
        i = i + 1;
    }
    buf[i] = 0;

    int left = 0;
    int right = i - 1;
    while (left < right) {
        char tmp = buf[left];
        buf[left] = buf[right];
        buf[right] = tmp;
        left = left + 1;
        right = right - 1;
    }
}

int digit_sum(int n) {
    if (n < 0) n = -n;
    int sum = 0;
    while (n > 0) {
        sum = sum + (n % 10);
        n = n / 10;
    }
    return sum;
}

int digit_count(int n) {
    if (n == 0) return 1;
    if (n < 0) n = -n;
    int count = 0;
    while (n > 0) {
        count = count + 1;
        n = n / 10;
    }
    return count;
}

int main(void) {
    // EXPECT: atoi_42=42
    printf("atoi_42=%d\n", my_atoi("42"));

    // EXPECT: atoi_neg=-123
    printf("atoi_neg=%d\n", my_atoi("-123"));

    // EXPECT: atoi_0=0
    printf("atoi_0=%d\n", my_atoi("0"));

    // EXPECT: atoi_sp=99
    printf("atoi_sp=%d\n", my_atoi("  99"));

    // EXPECT: atoi_plus=7
    printf("atoi_plus=%d\n", my_atoi("+7"));

    // EXPECT: atoi_trail=56
    printf("atoi_trail=%d\n", my_atoi("56abc"));

    char buf[32];

    my_itoa(12345, buf);
    // EXPECT: itoa_12345=12345
    printf("itoa_12345=%s\n", buf);

    my_itoa(-678, buf);
    // EXPECT: itoa_neg=-678
    printf("itoa_neg=%s\n", buf);

    my_itoa(0, buf);
    // EXPECT: itoa_0=0
    printf("itoa_0=%s\n", buf);

    my_itoa(1, buf);
    // EXPECT: itoa_1=1
    printf("itoa_1=%s\n", buf);

    itoa_base(255, buf, 16);
    // EXPECT: hex_255=ff
    printf("hex_255=%s\n", buf);

    itoa_base(10, buf, 2);
    // EXPECT: bin_10=1010
    printf("bin_10=%s\n", buf);

    itoa_base(8, buf, 8);
    // EXPECT: oct_8=10
    printf("oct_8=%s\n", buf);

    itoa_base(100, buf, 10);
    // EXPECT: dec_100=100
    printf("dec_100=%s\n", buf);

    int roundtrip = 9876;
    my_itoa(roundtrip, buf);
    int back = my_atoi(buf);
    // EXPECT: roundtrip=9876
    printf("roundtrip=%d\n", back);

    // EXPECT: dsum_123=6
    printf("dsum_123=%d\n", digit_sum(123));

    // EXPECT: dsum_999=27
    printf("dsum_999=%d\n", digit_sum(999));

    // EXPECT: dcount_0=1
    printf("dcount_0=%d\n", digit_count(0));

    // EXPECT: dcount_999=3
    printf("dcount_999=%d\n", digit_count(999));

    // EXPECT: dcount_10000=5
    printf("dcount_10000=%d\n", digit_count(10000));

    return 0;
}
