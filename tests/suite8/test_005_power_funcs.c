int printf(const char *fmt, ...);

// Iterative power function
int power_iter(int base, int exp) {
    int result;
    int i;
    result = 1;
    for (i = 0; i < exp; i++) {
        result = result * base;
    }
    return result;
}

// Fast power using repeated squaring
int power_fast(int base, int exp) {
    int result;
    result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = result * base;
        }
        base = base * base;
        exp = exp / 2;
    }
    return result;
}

// Recursive power
int power_rec(int base, int exp) {
    if (exp == 0) return 1;
    if (exp % 2 == 0) {
        int half;
        half = power_rec(base, exp / 2);
        return half * half;
    }
    return base * power_rec(base, exp - 1);
}

// Check if n is a power of 2
int is_power_of_2(int n) {
    if (n <= 0) return 0;
    return (n & (n - 1)) == 0;
}

// Check if n is a power of k
int is_power_of_k(int n, int k) {
    if (n <= 0) return 0;
    if (k <= 1) return n == 1;
    while (n > 1) {
        if (n % k != 0) return 0;
        n = n / k;
    }
    return 1;
}

// Find the highest power of k that divides n
int highest_power_dividing(int n, int k) {
    int count;
    if (n == 0) return 0;
    if (n < 0) n = -n;
    count = 0;
    while (n % k == 0) {
        count = count + 1;
        n = n / k;
    }
    return count;
}

// Sum of powers: 1^k + 2^k + ... + n^k
int sum_of_powers(int n, int k) {
    int total;
    int i;
    total = 0;
    for (i = 1; i <= n; i++) {
        total = total + power_iter(i, k);
    }
    return total;
}

// Tetration: a^^n (iterated exponentiation)
// a^^0 = 1, a^^1 = a, a^^2 = a^a, a^^3 = a^(a^a)
int tetration(int a, int n) {
    int result;
    int i;
    if (n == 0) return 1;
    result = 1;
    for (i = 0; i < n; i++) {
        result = power_iter(a, result);
    }
    return result;
}

// Digital root of power (sum digits repeatedly)
int digit_sum(int n) {
    int s;
    if (n < 0) n = -n;
    s = 0;
    while (n > 0) {
        s = s + n % 10;
        n = n / 10;
    }
    return s;
}

int digital_root(int n) {
    while (n >= 10) {
        n = digit_sum(n);
    }
    return n;
}

void test_power_table(int base, int max_exp) {
    int i;
    int val;
    printf("Power table %d:", base);
    for (i = 0; i < max_exp; i++) {
        val = power_fast(base, i);
        printf(" %d", val);
    }
    printf("\n");
}

int main() {
    // EXPECT: power_iter(2, 10) = 1024
    printf("power_iter(2, 10) = %d\n", power_iter(2, 10));

    // EXPECT: power_iter(3, 5) = 243
    printf("power_iter(3, 5) = %d\n", power_iter(3, 5));

    // EXPECT: power_fast(2, 15) = 32768
    printf("power_fast(2, 15) = %d\n", power_fast(2, 15));

    // EXPECT: power_fast(5, 4) = 625
    printf("power_fast(5, 4) = %d\n", power_fast(5, 4));

    // EXPECT: power_rec(2, 10) = 1024
    printf("power_rec(2, 10) = %d\n", power_rec(2, 10));

    // EXPECT: power_rec(7, 3) = 343
    printf("power_rec(7, 3) = %d\n", power_rec(7, 3));

    // EXPECT: is_pow2(64) = 1
    printf("is_pow2(64) = %d\n", is_power_of_2(64));

    // EXPECT: is_pow2(63) = 0
    printf("is_pow2(63) = %d\n", is_power_of_2(63));

    // EXPECT: is_pow3(81) = 1
    printf("is_pow3(81) = %d\n", is_power_of_k(81, 3));

    // EXPECT: is_pow3(82) = 0
    printf("is_pow3(82) = %d\n", is_power_of_k(82, 3));

    // EXPECT: highest_pow(72, 2) = 3
    printf("highest_pow(72, 2) = %d\n", highest_power_dividing(72, 2));

    // EXPECT: highest_pow(1000, 10) = 3
    printf("highest_pow(1000, 10) = %d\n", highest_power_dividing(1000, 10));

    // EXPECT: sum_powers(5, 2) = 55
    printf("sum_powers(5, 2) = %d\n", sum_of_powers(5, 2));

    // EXPECT: sum_powers(4, 3) = 100
    printf("sum_powers(4, 3) = %d\n", sum_of_powers(4, 3));

    // EXPECT: tetration(2, 3) = 16
    printf("tetration(2, 3) = %d\n", tetration(2, 3));

    // EXPECT: tetration(3, 2) = 27
    printf("tetration(3, 2) = %d\n", tetration(3, 2));

    // EXPECT: digital_root(9875) = 2
    printf("digital_root(9875) = %d\n", digital_root(9875));

    // EXPECT: Power table 2: 1 2 4 8 16 32 64 128 256 512
    test_power_table(2, 10);

    return 0;
}
