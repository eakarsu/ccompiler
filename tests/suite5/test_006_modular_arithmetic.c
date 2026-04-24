int printf(const char *fmt, ...);

int mod_add(int a, int b, int m) {
    return (a % m + b % m) % m;
}

int mod_sub(int a, int b, int m) {
    return ((a % m - b % m) + m) % m;
}

int mod_mul(int a, int b, int m) {
    int result = 0;
    a = a % m;
    b = b % m;
    while (b > 0) {
        if (b % 2 == 1) {
            result = (result + a) % m;
        }
        a = (a * 2) % m;
        b = b / 2;
    }
    return result;
}

int mod_pow(int base, int exp, int m) {
    int result = 1;
    base = base % m;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % m;
        }
        exp = exp / 2;
        base = (base * base) % m;
    }
    return result;
}

int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int mod_inverse(int a, int m) {
    int m0 = m;
    int x0 = 0;
    int x1 = 1;
    if (m == 1) return 0;
    while (a > 1) {
        int q = a / m;
        int t = m;
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) x1 = x1 + m0;
    return x1;
}

int crt_two(int r1, int m1, int r2, int m2) {
    int inv = mod_inverse(m1, m2);
    int diff = r2 - r1;
    while (diff < 0) diff = diff + m2;
    int k = (diff * inv) % m2;
    return r1 + k * m1;
}

int fermat_check(int p) {
    if (p <= 1) return 0;
    if (p == 2) return 1;
    return mod_pow(2, p - 1, p) == 1;
}

int euler_criterion(int a, int p) {
    return mod_pow(a, (p - 1) / 2, p);
}

int sum_mod(int n, int m) {
    int sum = 0;
    int i = 1;
    while (i <= n) {
        sum = (sum + i) % m;
        i = i + 1;
    }
    return sum;
}

int prod_mod(int n, int m) {
    int prod = 1;
    int i = 1;
    while (i <= n) {
        prod = (prod * i) % m;
        i = i + 1;
    }
    return prod;
}

int pow_tower_mod(int base, int n, int m) {
    int result = 1;
    int i = 0;
    while (i < n) {
        result = mod_pow(base, result, m);
        i = i + 1;
    }
    return result;
}

int main(void) {
    // EXPECT: mod_add(7,8,5) = 0
    printf("mod_add(7,8,5) = %d\n", mod_add(7, 8, 5));
    // EXPECT: mod_sub(3,7,5) = 1
    printf("mod_sub(3,7,5) = %d\n", mod_sub(3, 7, 5));
    // EXPECT: mod_mul(3,4,7) = 5
    printf("mod_mul(3,4,7) = %d\n", mod_mul(3, 4, 7));

    // EXPECT: mod_pow(2,10,1000) = 24
    printf("mod_pow(2,10,1000) = %d\n", mod_pow(2, 10, 1000));
    // EXPECT: mod_pow(3,7,13) = 3
    printf("mod_pow(3,7,13) = %d\n", mod_pow(3, 7, 13));
    // EXPECT: mod_pow(5,3,13) = 8
    printf("mod_pow(5,3,13) = %d\n", mod_pow(5, 3, 13));

    // EXPECT: inv(3,7) = 5
    printf("inv(3,7) = %d\n", mod_inverse(3, 7));
    // EXPECT: inv(2,11) = 6
    printf("inv(2,11) = %d\n", mod_inverse(2, 11));

    // EXPECT: crt(2,3,3,5) = 8
    printf("crt(2,3,3,5) = %d\n", crt_two(2, 3, 3, 5));
    // EXPECT: crt(1,3,2,5) = 7
    printf("crt(1,3,2,5) = %d\n", crt_two(1, 3, 2, 5));

    // EXPECT: fermat(7) = 1
    printf("fermat(7) = %d\n", fermat_check(7));
    // EXPECT: fermat(13) = 1
    printf("fermat(13) = %d\n", fermat_check(13));
    // EXPECT: fermat(4) = 0
    printf("fermat(4) = %d\n", fermat_check(4));

    // EXPECT: euler(2,7) = 1
    printf("euler(2,7) = %d\n", euler_criterion(2, 7));
    // EXPECT: euler(3,7) = 6
    printf("euler(3,7) = %d\n", euler_criterion(3, 7));

    // EXPECT: sum_mod(10,7) = 6
    printf("sum_mod(10,7) = %d\n", sum_mod(10, 7));
    // EXPECT: prod_mod(6,7) = 6
    printf("prod_mod(6,7) = %d\n", prod_mod(6, 7));

    // EXPECT: ptower(2,3,100) = 16
    printf("ptower(2,3,100) = %d\n", pow_tower_mod(2, 3, 100));

    return 0;
}
