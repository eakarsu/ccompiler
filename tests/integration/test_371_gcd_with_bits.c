int printf(const char *fmt, ...);
// EXPECT: binary_gcd(12, 8) = 4\nbinary_gcd(100, 75) = 25\nbinary_gcd(17, 13) = 1\nbinary_gcd(48, 18) = 6\nbinary_gcd(0, 5) = 5\nbinary_gcd(7, 0) = 7\nbinary_gcd(1, 1) = 1\ngcd(12, 8) = 4\ngcd(100, 75) = 25\ngcd(1024, 768) = 256\ngcd(252, 105) = 21\ngcd(111, 37) = 37\nalgorithms agree: 1\ngcd(16, 64) = 16\ngcd(32, 48) = 16\ngcd(128, 96) = 32\ncoprime checks:\n  gcd(7, 11) = 1\n  gcd(13, 17) = 1\n  gcd(15, 28) = 1
// Test: GCD with bits (binary GCD / Stein's algorithm)

int binary_gcd(int u, int v) {
    if (u == 0) return v;
    if (v == 0) return u;

    // Find common factor of 2
    int shift = 0;
    while (((u | v) & 1) == 0) {
        u = u >> 1;
        v = v >> 1;
        shift++;
    }

    // Remove remaining factors of 2 from u
    while ((u & 1) == 0) {
        u = u >> 1;
    }

    do {
        // Remove factors of 2 from v
        while ((v & 1) == 0) {
            v = v >> 1;
        }
        // Ensure u <= v
        if (u > v) {
            int t = u;
            u = v;
            v = t;
        }
        v = v - u;
    } while (v != 0);

    return u << shift;
}

int euclidean_gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int main(void) {
    // Basic GCD tests
    printf("binary_gcd(12, 8) = %d\n", binary_gcd(12, 8));
    printf("binary_gcd(100, 75) = %d\n", binary_gcd(100, 75));
    printf("binary_gcd(17, 13) = %d\n", binary_gcd(17, 13));
    printf("binary_gcd(48, 18) = %d\n", binary_gcd(48, 18));
    printf("binary_gcd(0, 5) = %d\n", binary_gcd(0, 5));
    printf("binary_gcd(7, 0) = %d\n", binary_gcd(7, 0));
    printf("binary_gcd(1, 1) = %d\n", binary_gcd(1, 1));

    // Compare with Euclidean
    int pairs[10];
    pairs[0] = 12; pairs[1] = 8;
    pairs[2] = 100; pairs[3] = 75;
    pairs[4] = 1024; pairs[5] = 768;
    pairs[6] = 252; pairs[7] = 105;
    pairs[8] = 111; pairs[9] = 37;

    int all_match = 1;
    int i;
    for (i = 0; i < 10; i += 2) {
        int bg = binary_gcd(pairs[i], pairs[i+1]);
        int eg = euclidean_gcd(pairs[i], pairs[i+1]);
        if (bg != eg) all_match = 0;
        printf("gcd(%d, %d) = %d\n", pairs[i], pairs[i+1], bg);
    }
    printf("algorithms agree: %d\n", all_match);

    // GCD of powers of 2
    printf("gcd(16, 64) = %d\n", binary_gcd(16, 64));
    printf("gcd(32, 48) = %d\n", binary_gcd(32, 48));
    printf("gcd(128, 96) = %d\n", binary_gcd(128, 96));

    // Coprime pairs
    printf("coprime checks:\n");
    printf("  gcd(7, 11) = %d\n", binary_gcd(7, 11));
    printf("  gcd(13, 17) = %d\n", binary_gcd(13, 17));
    printf("  gcd(15, 28) = %d\n", binary_gcd(15, 28));

    return 0;
}
