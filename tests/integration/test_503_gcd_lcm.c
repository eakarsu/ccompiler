int printf(const char *fmt, ...);
// EXPECT: GCD and LCM computations:\ngcd(12, 8) = 4, lcm = 24\ngcd(100, 75) = 25, lcm = 300\ngcd(17, 13) = 1, lcm = 221\ngcd(48, 36) = 12, lcm = 144\ngcd(1024, 768) = 256, lcm = 3072\ngcd(91, 65) = 13, lcm = 455\ngcd(252, 105) = 21, lcm = 1260\ngcd(1, 999) = 1, lcm = 999\ngcd(144, 89) = 1, lcm = 12816\ngcd(360, 240) = 120, lcm = 720\nGCD of {120,90,60,150,180,300} = 30\nLCM of {120,90,60,150,180,300} = 1800\nExtended GCD: 35*(1) + 15*(-2) = 5\nVerification: 5
// Test: GCD and LCM computations

int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int lcm(int a, int b) {
    return a / gcd(a, b) * b;
}

int main(void) {
    int pairs_a[10];
    int pairs_b[10];
    int i;

    pairs_a[0] = 12;   pairs_b[0] = 8;
    pairs_a[1] = 100;  pairs_b[1] = 75;
    pairs_a[2] = 17;   pairs_b[2] = 13;
    pairs_a[3] = 48;   pairs_b[3] = 36;
    pairs_a[4] = 1024; pairs_b[4] = 768;
    pairs_a[5] = 91;   pairs_b[5] = 65;
    pairs_a[6] = 252;  pairs_b[6] = 105;
    pairs_a[7] = 1;    pairs_b[7] = 999;
    pairs_a[8] = 144;  pairs_b[8] = 89;
    pairs_a[9] = 360;  pairs_b[9] = 240;

    printf("GCD and LCM computations:\n");
    for (i = 0; i < 10; i++) {
        int g = gcd(pairs_a[i], pairs_b[i]);
        int l = lcm(pairs_a[i], pairs_b[i]);
        printf("gcd(%d, %d) = %d, lcm = %d\n", pairs_a[i], pairs_b[i], g, l);
    }

    // GCD of multiple numbers
    int nums[6];
    nums[0] = 120;
    nums[1] = 90;
    nums[2] = 60;
    nums[3] = 150;
    nums[4] = 180;
    nums[5] = 300;

    int g = nums[0];
    for (i = 1; i < 6; i++) {
        g = gcd(g, nums[i]);
    }
    printf("GCD of {120,90,60,150,180,300} = %d\n", g);

    // LCM of multiple numbers
    int l = nums[0];
    for (i = 1; i < 6; i++) {
        l = lcm(l, nums[i]);
    }
    printf("LCM of {120,90,60,150,180,300} = %d\n", l);

    // Extended GCD: find x,y such that a*x + b*y = gcd(a,b)
    // Using iterative method
    int a = 35, b = 15;
    int old_r = a, r = b;
    int old_s = 1, s = 0;
    int old_t = 0, t = 1;
    while (r != 0) {
        int q = old_r / r;
        int tmp = r;
        r = old_r - q * r;
        old_r = tmp;
        tmp = s;
        s = old_s - q * s;
        old_s = tmp;
        tmp = t;
        t = old_t - q * t;
        old_t = tmp;
    }
    printf("Extended GCD: %d*(%d) + %d*(%d) = %d\n", a, old_s, b, old_t, old_r);

    // Verify: Bezout's identity
    int check = a * old_s + b * old_t;
    printf("Verification: %d\n", check);

    return 0;
}
