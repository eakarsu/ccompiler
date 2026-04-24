int printf(const char *fmt, ...);

int power_naive(int base, int exp) {
    int result = 1;
    int i;
    for (i = 0; i < exp; i++) {
        result = result * base;
    }
    return result;
}

int power_fast(int base, int exp) {
    int result = 1;
    int b = base;
    int e = exp;
    while (e > 0) {
        if (e % 2 == 1) {
            result = result * b;
        }
        b = b * b;
        e = e / 2;
    }
    return result;
}

int mod_power(int base, int exp, int mod) {
    if (mod == 1) return 0;
    int result = 1;
    int b = base % mod;
    int e = exp;
    while (e > 0) {
        if (e % 2 == 1) {
            result = (result * b) % mod;
        }
        b = (b * b) % mod;
        e = e / 2;
    }
    return result;
}

int power_of_ten(int n) {
    int result = 1;
    int i;
    for (i = 0; i < n; i++) {
        result = result * 10;
    }
    return result;
}

int is_power_of_two(int n) {
    if (n <= 0) return 0;
    while (n > 1) {
        if (n % 2 != 0) return 0;
        n = n / 2;
    }
    return 1;
}

int log_base_2(int n) {
    if (n <= 0) return -1;
    int count = 0;
    while (n > 1) {
        n = n / 2;
        count = count + 1;
    }
    return count;
}

int isqrt(int n) {
    if (n < 0) return -1;
    if (n == 0) return 0;
    int x = n;
    int y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

int is_perfect_square(int n) {
    if (n < 0) return 0;
    int s = isqrt(n);
    return s * s == n;
}

int tetration(int base, int height, int mod) {
    if (height == 0) return 1 % mod;
    if (height == 1) return base % mod;
    int exp = tetration(base, height - 1, mod);
    return mod_power(base, exp, mod);
}

int sum_of_powers(int n, int p) {
    int sum = 0;
    int i;
    for (i = 1; i <= n; i++) {
        sum = sum + power_fast(i, p);
    }
    return sum;
}

int count_digits_in_power(int base, int exp) {
    int val = power_fast(base, exp);
    if (val == 0) return 1;
    if (val < 0) val = -val;
    int count = 0;
    while (val > 0) {
        count = count + 1;
        val = val / 10;
    }
    return count;
}

int main(void) {
    // EXPECT: power_naive(2,10)=1024
    printf("power_naive(2,10)=%d\n", power_naive(2, 10));

    // EXPECT: power_fast(2,10)=1024
    printf("power_fast(2,10)=%d\n", power_fast(2, 10));

    // EXPECT: power_fast(3,7)=2187
    printf("power_fast(3,7)=%d\n", power_fast(3, 7));

    // EXPECT: power_fast(5,0)=1
    printf("power_fast(5,0)=%d\n", power_fast(5, 0));

    // EXPECT: power_fast(1,100)=1
    printf("power_fast(1,100)=%d\n", power_fast(1, 100));

    // EXPECT: mod_power(2,10,1000)=24
    printf("mod_power(2,10,1000)=%d\n", mod_power(2, 10, 1000));

    // EXPECT: mod_power(3,13,100)=23
    printf("mod_power(3,13,100)=%d\n", mod_power(3, 13, 100));

    // EXPECT: mod_power(7,256,13)=9
    printf("mod_power(7,256,13)=%d\n", mod_power(7, 256, 13));

    // EXPECT: power_of_ten(3)=1000
    printf("power_of_ten(3)=%d\n", power_of_ten(3));

    // EXPECT: power_of_ten(0)=1
    printf("power_of_ten(0)=%d\n", power_of_ten(0));

    // EXPECT: is_power_of_two(16)=1
    printf("is_power_of_two(16)=%d\n", is_power_of_two(16));

    // EXPECT: is_power_of_two(15)=0
    printf("is_power_of_two(15)=%d\n", is_power_of_two(15));

    // EXPECT: is_power_of_two(1)=1
    printf("is_power_of_two(1)=%d\n", is_power_of_two(1));

    // EXPECT: log2(1024)=10
    printf("log2(1024)=%d\n", log_base_2(1024));

    // EXPECT: log2(1)=0
    printf("log2(1)=%d\n", log_base_2(1));

    // EXPECT: isqrt(100)=10
    printf("isqrt(100)=%d\n", isqrt(100));

    // EXPECT: isqrt(99)=9
    printf("isqrt(99)=%d\n", isqrt(99));

    // EXPECT: isqrt(0)=0
    printf("isqrt(0)=%d\n", isqrt(0));

    // EXPECT: is_perfect_square(144)=1
    printf("is_perfect_square(144)=%d\n", is_perfect_square(144));

    // EXPECT: is_perfect_square(145)=0
    printf("is_perfect_square(145)=%d\n", is_perfect_square(145));

    // EXPECT: tetration(2,3,1000)=16
    printf("tetration(2,3,1000)=%d\n", tetration(2, 3, 1000));

    // EXPECT: sum_of_powers(5,2)=55
    printf("sum_of_powers(5,2)=%d\n", sum_of_powers(5, 2));

    // EXPECT: sum_of_powers(4,3)=100
    printf("sum_of_powers(4,3)=%d\n", sum_of_powers(4, 3));

    // EXPECT: count_digits(2,10)=4
    printf("count_digits(2,10)=%d\n", count_digits_in_power(2, 10));

    // EXPECT: count_digits(10,5)=6
    printf("count_digits(10,5)=%d\n", count_digits_in_power(10, 5));

    return 0;
}
