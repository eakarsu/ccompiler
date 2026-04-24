int printf(const char *fmt, ...);
// EXPECT: === naive power ===\n2^10=1024\n3^5=243\n5^0=1\n=== fast power ===\n2^10=1024\n2^15=32768\n3^7=2187\n7^4=2401\n=== tail power ===\n2^10=1024\n3^5=243\n=== power mod ===\n2^10 mod 1000=24\n3^7 mod 100=87\n5^3 mod 7=6\n=== tetration ===\n2^^1=2\n2^^2=4\n2^^3=16\n3^^2=27\n=== sum of powers ===\nsum(5,2)=55\nsum(5,3)=225\nsum(10,1)=55\n=== perfect power ===\n8=1\n27=1\n10=0\n64=1\n=== digital root ===\ndr(493)=7\ndr(12345)=6\ndr(9)=9
// Test 225: Recursive power function variants

int power_naive(int base, int exp) {
    if (exp == 0) return 1;
    return base * power_naive(base, exp - 1);
}

int power_fast(int base, int exp) {
    if (exp == 0) return 1;
    if (exp == 1) return base;
    int half = power_fast(base, exp / 2);
    if ((exp / 2) * 2 == exp)
        return half * half;
    return half * half * base;
}

int power_tail(int base, int exp, int acc) {
    if (exp == 0) return acc;
    return power_tail(base, exp - 1, acc * base);
}

int power_mod(int base, int exp, int mod) {
    if (exp == 0) return 1;
    if (exp == 1) return base - (base / mod) * mod;
    int half = power_mod(base, exp / 2, mod);
    int result = (half * half) - ((half * half) / mod) * mod;
    if ((exp / 2) * 2 != exp) {
        result = (result * base) - ((result * base) / mod) * mod;
    }
    return result;
}

int tetration(int base, int height) {
    if (height == 0) return 1;
    return power_fast(base, tetration(base, height - 1));
}

int sum_of_powers(int n, int p) {
    if (n <= 0) return 0;
    return power_fast(n, p) + sum_of_powers(n - 1, p);
}

int is_perfect_power(int n) {
    int base = 2;
    while (base * base <= n) {
        int val = base;
        while (val < n) {
            val = val * base;
        }
        if (val == n) return 1;
        base = base + 1;
    }
    return 0;
}

int digital_root(int n) {
    if (n < 10) return n;
    int sum = 0;
    int temp = n;
    while (temp > 0) {
        sum = sum + temp - (temp / 10) * 10;
        temp = temp / 10;
    }
    return digital_root(sum);
}

int main(void) {
    printf("=== naive power ===\n");
    printf("2^10=%d\n", power_naive(2, 10));
    printf("3^5=%d\n", power_naive(3, 5));
    printf("5^0=%d\n", power_naive(5, 0));

    printf("=== fast power ===\n");
    printf("2^10=%d\n", power_fast(2, 10));
    printf("2^15=%d\n", power_fast(2, 15));
    printf("3^7=%d\n", power_fast(3, 7));
    printf("7^4=%d\n", power_fast(7, 4));

    printf("=== tail power ===\n");
    printf("2^10=%d\n", power_tail(2, 10, 1));
    printf("3^5=%d\n", power_tail(3, 5, 1));

    printf("=== power mod ===\n");
    printf("2^10 mod 1000=%d\n", power_mod(2, 10, 1000));
    printf("3^7 mod 100=%d\n", power_mod(3, 7, 100));
    printf("5^3 mod 7=%d\n", power_mod(5, 3, 7));

    printf("=== tetration ===\n");
    printf("2^^1=%d\n", tetration(2, 1));
    printf("2^^2=%d\n", tetration(2, 2));
    printf("2^^3=%d\n", tetration(2, 3));
    printf("3^^2=%d\n", tetration(3, 2));

    printf("=== sum of powers ===\n");
    printf("sum(5,2)=%d\n", sum_of_powers(5, 2));
    printf("sum(5,3)=%d\n", sum_of_powers(5, 3));
    printf("sum(10,1)=%d\n", sum_of_powers(10, 1));

    printf("=== perfect power ===\n");
    printf("8=%d\n", is_perfect_power(8));
    printf("27=%d\n", is_perfect_power(27));
    printf("10=%d\n", is_perfect_power(10));
    printf("64=%d\n", is_perfect_power(64));

    printf("=== digital root ===\n");
    printf("dr(493)=%d\n", digital_root(493));
    printf("dr(12345)=%d\n", digital_root(12345));
    printf("dr(9)=%d\n", digital_root(9));

    return 0;
}
