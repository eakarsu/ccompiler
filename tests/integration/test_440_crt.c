int printf(const char *fmt, ...);
// EXPECT: x = 2 mod 3, x = 3 mod 5, x = 2 mod 7\nSolution: x = 23\nVerify: 23 mod 3 = 2, 23 mod 5 = 3, 23 mod 7 = 2\n\nx = 1 mod 2, x = 2 mod 3, x = 3 mod 5\nSolution: x = 23\nVerify: 23 mod 2 = 1, 23 mod 3 = 2, 23 mod 5 = 3\n\nExtended GCD tests:\nGCD(35,15) = 5\nGCD(48,18) = 6\n\nTwo-equation CRT:\nx=2 mod 3, x=3 mod 5: x=8\nx=1 mod 4, x=3 mod 6: x=9
// Test: Chinese Remainder Theorem

int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int extended_gcd(int a, int b, int *x, int *y) {
    if (a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }
    int x1, y1;
    int g = extended_gcd(b % a, a, &x1, &y1);
    *x = y1 - (b / a) * x1;
    *y = x1;
    return g;
}

int mod_inverse_ext(int a, int m) {
    int x, y;
    int g = extended_gcd(a, m, &x, &y);
    if (g != 1) return -1;
    return ((x % m) + m) % m;
}

int chinese_remainder(int remainders[], int moduli[], int n) {
    int M = 1;
    int i;
    for (i = 0; i < n; i++) {
        M = M * moduli[i];
    }

    int result = 0;
    for (i = 0; i < n; i++) {
        int Mi = M / moduli[i];
        int yi = mod_inverse_ext(Mi, moduli[i]);
        result = result + remainders[i] * Mi * yi;
    }
    return ((result % M) + M) % M;
}

int solve_two(int r1, int m1, int r2, int m2) {
    int x, y;
    int g = extended_gcd(m1, m2, &x, &y);
    if ((r2 - r1) % g != 0) return -1;
    int lcm = m1 / g * m2;
    int diff = r2 - r1;
    int result = r1 + m1 * ((diff / g * x) % (m2 / g));
    return ((result % lcm) + lcm) % lcm;
}

int main(void) {
    int r1[] = {2, 3, 2};
    int m1[] = {3, 5, 7};
    int x = chinese_remainder(r1, m1, 3);
    printf("x = %d mod 3, x = %d mod 5, x = %d mod 7\n", r1[0], r1[1], r1[2]);
    printf("Solution: x = %d\n", x);
    printf("Verify: %d mod 3 = %d, %d mod 5 = %d, %d mod 7 = %d\n",
           x, x % 3, x, x % 5, x, x % 7);

    int r2[] = {1, 2, 3};
    int m2[] = {2, 3, 5};
    x = chinese_remainder(r2, m2, 3);
    printf("\nx = %d mod 2, x = %d mod 3, x = %d mod 5\n", r2[0], r2[1], r2[2]);
    printf("Solution: x = %d\n", x);
    printf("Verify: %d mod 2 = %d, %d mod 3 = %d, %d mod 5 = %d\n",
           x, x % 2, x, x % 3, x, x % 5);

    printf("\nExtended GCD tests:\n");
    int a, b;
    a = 35; b = 15;
    printf("GCD(%d,%d) = %d\n", a, b, gcd(a, b));
    a = 48; b = 18;
    printf("GCD(%d,%d) = %d\n", a, b, gcd(a, b));

    printf("\nTwo-equation CRT:\n");
    printf("x=2 mod 3, x=3 mod 5: x=%d\n", solve_two(2, 3, 3, 5));
    printf("x=1 mod 4, x=3 mod 6: x=%d\n", solve_two(1, 4, 3, 6));

    return 0;
}
