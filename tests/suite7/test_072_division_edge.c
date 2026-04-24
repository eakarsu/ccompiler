int printf(const char *fmt, ...);

int safe_div(int a, int b) {
    if (b == 0) return 0;
    if (a == (-2147483647 - 1) && b == -1) return 2147483647;
    return a / b;
}

int safe_mod(int a, int b) {
    if (b == 0) return 0;
    if (a == (-2147483647 - 1) && b == -1) return 0;
    return a % b;
}

int truncates_toward_zero(int a, int b) {
    int q = a / b;
    int r = a % b;
    if (a == q * b + r) return 1;
    return 0;
}

int divides_evenly(int a, int b) {
    if (b == 0) return 0;
    return (a % b) == 0;
}

int div_round_away(int a, int b) {
    if (b == 0) return 0;
    int q = a / b;
    int r = a % b;
    if (r != 0) {
        if ((a > 0 && b > 0) || (a < 0 && b < 0)) {
            q = q + 1;
        } else {
            q = q - 1;
        }
    }
    return q;
}

int power_of_two_div(int x, int shift) {
    int result = x;
    int i;
    for (i = 0; i < shift; i++) {
        result = result / 2;
    }
    return result;
}

int repeated_halving(int n) {
    int count = 0;
    while (n > 1) {
        n = n / 2;
        count = count + 1;
    }
    return count;
}

int digit_sum(int n) {
    int sum = 0;
    if (n < 0) n = -n;
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

int reverse_digits(int n) {
    int neg = 0;
    if (n < 0) { neg = 1; n = -n; }
    int rev = 0;
    while (n > 0) {
        rev = rev * 10 + (n % 10);
        n = n / 10;
    }
    if (neg) rev = -rev;
    return rev;
}

int main(void) {
    // EXPECT: safe_div(10,3)=3
    printf("safe_div(10,3)=%d\n", safe_div(10, 3));

    // EXPECT: safe_div(10,0)=0
    printf("safe_div(10,0)=%d\n", safe_div(10, 0));

    // EXPECT: safe_div(-2147483648,-1)=2147483647
    printf("safe_div(-2147483648,-1)=%d\n", safe_div(-2147483647 - 1, -1));

    // EXPECT: safe_mod(10,3)=1
    printf("safe_mod(10,3)=%d\n", safe_mod(10, 3));

    // EXPECT: safe_mod(10,0)=0
    printf("safe_mod(10,0)=%d\n", safe_mod(10, 0));

    // EXPECT: safe_mod(-2147483648,-1)=0
    printf("safe_mod(-2147483648,-1)=%d\n", safe_mod(-2147483647 - 1, -1));

    // EXPECT: trunc(-7,2)=1
    printf("trunc(-7,2)=%d\n", truncates_toward_zero(-7, 2));

    // EXPECT: trunc(7,-2)=1
    printf("trunc(7,-2)=%d\n", truncates_toward_zero(7, -2));

    // EXPECT: -7/2=-3
    printf("-7/2=%d\n", -7 / 2);

    // EXPECT: -7%%2=-1
    printf("-7%%%%2=%d\n", -7 % 2);

    // EXPECT: 7/-2=-3
    printf("7/-2=%d\n", 7 / -2);

    // EXPECT: 7%%-2=1
    printf("7%%%%-2=%d\n", 7 % -2);

    // EXPECT: divides(12,3)=1
    printf("divides(12,3)=%d\n", divides_evenly(12, 3));

    // EXPECT: divides(13,3)=0
    printf("divides(13,3)=%d\n", divides_evenly(13, 3));

    // EXPECT: div_round_away(7,2)=4
    printf("div_round_away(7,2)=%d\n", div_round_away(7, 2));

    // EXPECT: div_round_away(-7,2)=-4
    printf("div_round_away(-7,2)=%d\n", div_round_away(-7, 2));

    // EXPECT: power_of_two_div(1024,3)=128
    printf("power_of_two_div(1024,3)=%d\n", power_of_two_div(1024, 3));

    // EXPECT: power_of_two_div(-16,2)=-4
    printf("power_of_two_div(-16,2)=%d\n", power_of_two_div(-16, 2));

    // EXPECT: repeated_halving(1024)=10
    printf("repeated_halving(1024)=%d\n", repeated_halving(1024));

    // EXPECT: repeated_halving(1)=0
    printf("repeated_halving(1)=%d\n", repeated_halving(1));

    // EXPECT: digit_sum(12345)=15
    printf("digit_sum(12345)=%d\n", digit_sum(12345));

    // EXPECT: digit_sum(-999)=27
    printf("digit_sum(-999)=%d\n", digit_sum(-999));

    // EXPECT: digit_count(12345)=5
    printf("digit_count(12345)=%d\n", digit_count(12345));

    // EXPECT: digit_count(0)=1
    printf("digit_count(0)=%d\n", digit_count(0));

    // EXPECT: reverse_digits(12345)=54321
    printf("reverse_digits(12345)=%d\n", reverse_digits(12345));

    // EXPECT: reverse_digits(-123)=-321
    printf("reverse_digits(-123)=%d\n", reverse_digits(-123));

    return 0;
}
