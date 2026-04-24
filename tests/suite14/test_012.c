int printf(const char *fmt, ...);

// GCD/LCM with extended Euclidean algorithm

int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int lcm(int a, int b) {
    return (a / gcd(a, b)) * b;
}

// Extended GCD: returns gcd, sets *x and *y such that a*x + b*y = gcd(a,b)
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

// Modular inverse: returns x such that a*x = 1 (mod m), or -1 if none
int mod_inverse(int a, int m) {
    int x;
    int y;
    int g = ext_gcd(a, m, &x, &y);
    if (g != 1) {
        return -1;
    }
    // Make sure result is positive
    int result = x % m;
    if (result < 0) {
        result = result + m;
    }
    return result;
}

// GCD of an array of numbers
int array_gcd(int *arr, int n) {
    int result = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        result = gcd(result, arr[i]);
    }
    return result;
}

// LCM of an array of numbers
int array_lcm(int *arr, int n) {
    int result = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        result = lcm(result, arr[i]);
    }
    return result;
}

// Check if two numbers are coprime
int coprime(int a, int b) {
    return gcd(a, b) == 1;
}

// Count coprimes to n in range [1, n]
int count_coprimes(int n) {
    int count = 0;
    int i;
    for (i = 1; i <= n; i++) {
        if (coprime(i, n)) {
            count = count + 1;
        }
    }
    return count;
}

// Bezout identity verification: a*x + b*y should equal gcd(a,b)
int verify_bezout(int a, int b) {
    int x;
    int y;
    int g = ext_gcd(a, b, &x, &y);
    int lhs = a * x + b * y;
    return lhs == g;
}

int main(void) {
    printf("gcd(48, 18): %d\n", gcd(48, 18));
    // EXPECT: gcd(48, 18): 6

    printf("gcd(100, 75): %d\n", gcd(100, 75));
    // EXPECT: gcd(100, 75): 25

    printf("gcd(17, 13): %d\n", gcd(17, 13));
    // EXPECT: gcd(17, 13): 1

    printf("lcm(12, 18): %d\n", lcm(12, 18));
    // EXPECT: lcm(12, 18): 36

    printf("lcm(7, 5): %d\n", lcm(7, 5));
    // EXPECT: lcm(7, 5): 35

    int x;
    int y;
    int g = ext_gcd(35, 15, &x, &y);
    printf("ext_gcd(35,15): g=%d x=%d y=%d\n", g, x, y);
    // EXPECT: ext_gcd(35,15): g=5 x=1 y=-2

    int check = 35 * x + 15 * y;
    printf("Verify: 35*%d + 15*%d = %d\n", x, y, check);
    // EXPECT: Verify: 35*1 + 15*-2 = 5

    int inv1 = mod_inverse(3, 11);
    printf("mod_inverse(3, 11): %d\n", inv1);
    // EXPECT: mod_inverse(3, 11): 4

    int verify1 = (3 * inv1) % 11;
    printf("Verify: 3*4 mod 11 = %d\n", verify1);
    // EXPECT: Verify: 3*4 mod 11 = 1

    int inv2 = mod_inverse(7, 26);
    printf("mod_inverse(7, 26): %d\n", inv2);
    // EXPECT: mod_inverse(7, 26): 15

    int verify2 = (7 * inv2) % 26;
    printf("Verify: 7*15 mod 26 = %d\n", verify2);
    // EXPECT: Verify: 7*15 mod 26 = 1

    int inv3 = mod_inverse(6, 9);
    printf("mod_inverse(6, 9): %d\n", inv3);
    // EXPECT: mod_inverse(6, 9): -1

    int arr1[4];
    arr1[0] = 24;
    arr1[1] = 36;
    arr1[2] = 48;
    arr1[3] = 60;
    printf("array_gcd: %d\n", array_gcd(arr1, 4));
    // EXPECT: array_gcd: 12

    printf("array_lcm: %d\n", array_lcm(arr1, 4));
    // EXPECT: array_lcm: 720

    printf("coprime(8,15): %d\n", coprime(8, 15));
    // EXPECT: coprime(8,15): 1

    printf("coprime(12,15): %d\n", coprime(12, 15));
    // EXPECT: coprime(12,15): 0

    printf("coprimes to 12: %d\n", count_coprimes(12));
    // EXPECT: coprimes to 12: 4

    printf("bezout(35,15): %d\n", verify_bezout(35, 15));
    // EXPECT: bezout(35,15): 1

    printf("bezout(120,23): %d\n", verify_bezout(120, 23));
    // EXPECT: bezout(120,23): 1

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
