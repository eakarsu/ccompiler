int printf(const char *fmt, ...);

// Chinese Remainder Theorem

int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int ext_gcd(int a, int b, int *x, int *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    int x1;
    int y1;
    int g = ext_gcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

int mod_inverse(int a, int m) {
    int x;
    int y;
    int g = ext_gcd(a, m, &x, &y);
    if (g != 1) return -1;
    int result = x % m;
    if (result < 0) result = result + m;
    return result;
}

// Solve: x = a1 (mod m1), x = a2 (mod m2)
// Returns smallest non-negative x, or -1 if no solution
int crt_two(int a1, int m1, int a2, int m2) {
    int g = gcd(m1, m2);
    if ((a2 - a1) % g != 0) return -1;

    int m1_div = m1 / g;
    int m2_div = m2 / g;
    int lcm = m1 * m2_div;  // m1 * (m2/g)

    int inv = mod_inverse(m1_div % m2_div, m2_div);
    if (inv < 0) return -1;

    // Use careful arithmetic to avoid overflow
    int diff = ((a2 - a1) / g) % m2_div;
    if (diff < 0) diff = diff + m2_div;
    int k = (diff * inv) % m2_div;
    int result = a1 + k * m1;

    result = result % lcm;
    if (result < 0) result = result + lcm;
    return result;
}

// Solve system of congruences: x = a[i] (mod m[i]) for i = 0..n-1
// All m[i] must be pairwise coprime
int crt_system(int *a, int *m, int n) {
    int current_a = a[0];
    int current_m = m[0];
    int i;
    for (i = 1; i < n; i++) {
        current_a = crt_two(current_a, current_m, a[i], m[i]);
        if (current_a < 0) return -1;
        current_m = current_m * m[i];  // since coprime, lcm = product
    }
    return current_a;
}

// Verify a CRT solution
int verify_crt(int x, int *a, int *m, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (x % m[i] != a[i]) return 0;
    }
    return 1;
}

// Classic puzzle: what number gives remainders r1,r2,r3 when divided by d1,d2,d3?
int solve_puzzle(int r1, int d1, int r2, int d2, int r3, int d3) {
    int a[3];
    int m[3];
    a[0] = r1; m[0] = d1;
    a[1] = r2; m[1] = d2;
    a[2] = r3; m[2] = d3;
    return crt_system(a, m, 3);
}

int main(void) {
    // Simple two-congruence: x=2(mod 3), x=3(mod 5)
    int r1 = crt_two(2, 3, 3, 5);
    printf("x=2(mod3), x=3(mod5): x=%d\n", r1);
    // EXPECT: x=2(mod3), x=3(mod5): x=8

    printf("Verify: %d mod 3 = %d, %d mod 5 = %d\n", r1, r1 % 3, r1, r1 % 5);
    // EXPECT: Verify: 8 mod 3 = 2, 8 mod 5 = 3

    // x=1(mod 2), x=2(mod 3), x=3(mod 5)
    int r2 = solve_puzzle(1, 2, 2, 3, 3, 5);
    printf("x=1(mod2), x=2(mod3), x=3(mod5): x=%d\n", r2);
    // EXPECT: x=1(mod2), x=2(mod3), x=3(mod5): x=23

    printf("Verify: %d%%2=%d, %d%%3=%d, %d%%5=%d\n", r2, r2%2, r2, r2%3, r2, r2%5);
    // EXPECT: Verify: 23%2=1, 23%3=2, 23%5=3

    // Classic Sun Tzu puzzle: x=2(mod3), x=3(mod5), x=2(mod7)
    int r3 = solve_puzzle(2, 3, 3, 5, 2, 7);
    printf("Sun Tzu: x=%d\n", r3);
    // EXPECT: Sun Tzu: x=23

    printf("Check: %d%%3=%d, %d%%5=%d, %d%%7=%d\n", r3, r3%3, r3, r3%5, r3, r3%7);
    // EXPECT: Check: 23%3=2, 23%5=3, 23%7=2

    // 4-congruence system: x=1(mod2), x=2(mod3), x=3(mod5), x=4(mod7)
    int a4[4];
    int m4[4];
    a4[0] = 1; m4[0] = 2;
    a4[1] = 2; m4[1] = 3;
    a4[2] = 3; m4[2] = 5;
    a4[3] = 4; m4[3] = 7;
    int r4 = crt_system(a4, m4, 4);
    printf("4-system: x=%d\n", r4);
    // EXPECT: 4-system: x=53

    int va4[4];
    va4[0] = 53 % 2; va4[1] = 53 % 3; va4[2] = 53 % 5; va4[3] = 53 % 7;
    printf("Verify: %%2=%d, %%3=%d, %%5=%d, %%7=%d\n", va4[0], va4[1], va4[2], va4[3]);
    // EXPECT: Verify: %2=1, %3=2, %5=3, %7=4

    int v4 = verify_crt(r4, a4, m4, 4);
    printf("verified: %d\n", v4);
    // EXPECT: verified: 1

    // Edge: x=0(mod3), x=0(mod5), x=0(mod7) => x=0
    int r5 = solve_puzzle(0, 3, 0, 5, 0, 7);
    printf("all-zero: x=%d\n", r5);
    // EXPECT: all-zero: x=0

    // x=1(mod3), x=1(mod5), x=1(mod7) => x=1
    int r6 = solve_puzzle(1, 3, 1, 5, 1, 7);
    printf("all-one: x=%d\n", r6);
    // EXPECT: all-one: x=1

    // Larger mods: x=3(mod11), x=5(mod13)
    int r7 = crt_two(3, 11, 5, 13);
    printf("x=3(mod11), x=5(mod13): x=%d\n", r7);
    // EXPECT: x=3(mod11), x=5(mod13): x=135

    printf("Check: %d%%11=%d, %d%%13=%d\n", r7, r7%11, r7, r7%13);
    // EXPECT: Check: 135%11=3, 135%13=5

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
