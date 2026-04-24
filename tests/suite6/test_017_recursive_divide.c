int printf(const char *fmt, ...);

int rec_sqrt_helper(int n, int low, int high) {
    if (low > high) return high;
    int mid = low + (high - low) / 2;
    int sq = mid * mid;
    if (sq == n) return mid;
    if (sq > n) return rec_sqrt_helper(n, low, mid - 1);
    return rec_sqrt_helper(n, mid + 1, high);
}

int rec_sqrt(int n) {
    if (n <= 1) return n;
    return rec_sqrt_helper(n, 1, n / 2 + 1);
}

int power_mod(int base, int exp, int mod) {
    if (exp == 0) return 1;
    if (exp % 2 == 0) {
        int half = power_mod(base, exp / 2, mod);
        return (half * half) % mod;
    }
    return (base * power_mod(base, exp - 1, mod)) % mod;
}

char bin_buf[33];

void to_binary(int n, int pos) {
    if (n == 0) {
        if (pos == 0) {
            bin_buf[0] = '0';
            bin_buf[1] = 0;
        }
        return;
    }
    to_binary(n / 2, pos + 1);
    int idx = 0;
    while (bin_buf[idx] != 0) idx = idx + 1;
    if (n % 2 == 0) {
        bin_buf[idx] = '0';
    } else {
        bin_buf[idx] = '1';
    }
    bin_buf[idx + 1] = 0;
}

void clear_buf(void) {
    int i = 0;
    while (i < 33) {
        bin_buf[i] = 0;
        i = i + 1;
    }
}

int count_bits(int n) {
    if (n == 0) return 0;
    return (n % 2) + count_bits(n / 2);
}

int reverse_number(int n, int acc) {
    if (n == 0) return acc;
    return reverse_number(n / 10, acc * 10 + n % 10);
}

int is_power_of_two(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    if (n % 2 != 0) return 0;
    return is_power_of_two(n / 2);
}

int log2_rec(int n) {
    if (n <= 1) return 0;
    return 1 + log2_rec(n / 2);
}

int rec_abs(int n) {
    if (n < 0) return -n;
    return n;
}

int rec_max(int a, int b) {
    if (a > b) return a;
    return b;
}

int rec_min(int a, int b) {
    if (a < b) return a;
    return b;
}

int rec_div(int a, int b) {
    if (a < b) return 0;
    return 1 + rec_div(a - b, b);
}

int rec_mod(int a, int b) {
    if (a < b) return a;
    return rec_mod(a - b, b);
}

int nth_digit(int n, int pos) {
    if (pos == 0) return n % 10;
    return nth_digit(n / 10, pos - 1);
}

int from_binary(char *s, int idx) {
    if (s[idx] == 0) return 0;
    int rest = from_binary(s, idx + 1);
    int len = 0;
    int i = idx;
    while (s[i] != 0) { len = len + 1; i = i + 1; }
    int bit = s[idx] - '0';
    int shift = len - 1;
    int multiplier = 1;
    while (shift > 0) { multiplier = multiplier * 2; shift = shift - 1; }
    return bit * multiplier + rest;
}

int main(void) {
    // EXPECT: sqrt(0)=0
    printf("sqrt(0)=%d\n", rec_sqrt(0));
    // EXPECT: sqrt(1)=1
    printf("sqrt(1)=%d\n", rec_sqrt(1));
    // EXPECT: sqrt(16)=4
    printf("sqrt(16)=%d\n", rec_sqrt(16));
    // EXPECT: sqrt(100)=10
    printf("sqrt(100)=%d\n", rec_sqrt(100));
    // EXPECT: sqrt(50)=7
    printf("sqrt(50)=%d\n", rec_sqrt(50));
    // EXPECT: sqrt(10000)=100
    printf("sqrt(10000)=%d\n", rec_sqrt(10000));

    // EXPECT: powmod(2,10,1000)=24
    printf("powmod(2,10,1000)=%d\n", power_mod(2, 10, 1000));
    // EXPECT: powmod(3,7,100)=87
    printf("powmod(3,7,100)=%d\n", power_mod(3, 7, 100));
    // EXPECT: powmod(5,3,13)=8
    printf("powmod(5,3,13)=%d\n", power_mod(5, 3, 13));

    clear_buf();
    to_binary(42, 0);
    // EXPECT: bin(42)=101010
    printf("bin(42)=%s\n", bin_buf);

    clear_buf();
    to_binary(255, 0);
    // EXPECT: bin(255)=11111111
    printf("bin(255)=%s\n", bin_buf);

    clear_buf();
    to_binary(1, 0);
    // EXPECT: bin(1)=1
    printf("bin(1)=%s\n", bin_buf);

    // EXPECT: bits(42)=3
    printf("bits(42)=%d\n", count_bits(42));
    // EXPECT: bits(255)=8
    printf("bits(255)=%d\n", count_bits(255));
    // EXPECT: bits(1024)=1
    printf("bits(1024)=%d\n", count_bits(1024));

    // EXPECT: rev(12345)=54321
    printf("rev(12345)=%d\n", reverse_number(12345, 0));
    // EXPECT: rev(1000)=1
    printf("rev(1000)=%d\n", reverse_number(1000, 0));

    // EXPECT: pow2(64)=1
    printf("pow2(64)=%d\n", is_power_of_two(64));
    // EXPECT: pow2(100)=0
    printf("pow2(100)=%d\n", is_power_of_two(100));
    // EXPECT: pow2(1)=1
    printf("pow2(1)=%d\n", is_power_of_two(1));

    // EXPECT: log2(1)=0
    printf("log2(1)=%d\n", log2_rec(1));
    // EXPECT: log2(1024)=10
    printf("log2(1024)=%d\n", log2_rec(1024));

    // EXPECT: div(100,7)=14
    printf("div(100,7)=%d\n", rec_div(100, 7));
    // EXPECT: mod(100,7)=2
    printf("mod(100,7)=%d\n", rec_mod(100, 7));

    // EXPECT: digit(12345,0)=5
    printf("digit(12345,0)=%d\n", nth_digit(12345, 0));
    // EXPECT: digit(12345,4)=1
    printf("digit(12345,4)=%d\n", nth_digit(12345, 4));

    // EXPECT: frombin=42
    printf("frombin=%d\n", from_binary("101010", 0));

    return 0;
}
