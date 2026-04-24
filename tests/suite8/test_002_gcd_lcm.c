int printf(const char *fmt, ...);

// GCD using Euclidean algorithm
int gcd(int a, int b) {
    while (b != 0) {
        int t;
        t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// GCD using recursive Euclidean algorithm
int gcd_recursive(int a, int b) {
    if (b == 0) return a;
    return gcd_recursive(b, a % b);
}

// LCM using GCD
int lcm(int a, int b) {
    return (a / gcd(a, b)) * b;
}

// Extended GCD: returns gcd, sets *x and *y for ax + by = gcd
int ext_gcd(int a, int b, int *x, int *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    int x1;
    int y1;
    int g;
    g = ext_gcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

// Check if two numbers are coprime
int are_coprime(int a, int b) {
    return gcd(a, b) == 1;
}

// GCD of array of numbers
int gcd_array(int *arr, int n) {
    int result;
    int i;
    result = arr[0];
    for (i = 1; i < n; i++) {
        result = gcd(result, arr[i]);
    }
    return result;
}

// LCM of array of numbers
int lcm_array(int *arr, int n) {
    int result;
    int i;
    result = arr[0];
    for (i = 1; i < n; i++) {
        result = lcm(result, arr[i]);
    }
    return result;
}

// Count numbers from 1..n coprime to n (Euler's totient)
int euler_totient(int n) {
    int count;
    int i;
    count = 0;
    for (i = 1; i <= n; i++) {
        if (gcd(i, n) == 1) {
            count = count + 1;
        }
    }
    return count;
}

// Binary GCD algorithm (Stein's algorithm) - simplified iterative
int binary_gcd(int a, int b) {
    int shift;
    if (a == 0) return b;
    if (b == 0) return a;
    shift = 0;
    while (((a | b) & 1) == 0) {
        a = a / 2;
        b = b / 2;
        shift = shift + 1;
    }
    while ((a & 1) == 0) {
        a = a / 2;
    }
    while (b != 0) {
        while ((b & 1) == 0) {
            b = b / 2;
        }
        if (a > b) {
            int t;
            t = a;
            a = b;
            b = t;
        }
        b = b - a;
    }
    int i;
    int result;
    result = a;
    for (i = 0; i < shift; i++) {
        result = result * 2;
    }
    return result;
}

int main() {
    int arr[5];
    int x;
    int y;
    int g;

    // EXPECT: gcd(48, 18) = 6
    printf("gcd(48, 18) = %d\n", gcd(48, 18));

    // EXPECT: gcd(100, 75) = 25
    printf("gcd(100, 75) = %d\n", gcd(100, 75));

    // EXPECT: gcd_recursive(48, 18) = 6
    printf("gcd_recursive(48, 18) = %d\n", gcd_recursive(48, 18));

    // EXPECT: lcm(12, 18) = 36
    printf("lcm(12, 18) = %d\n", lcm(12, 18));

    // EXPECT: lcm(7, 5) = 35
    printf("lcm(7, 5) = %d\n", lcm(7, 5));

    g = ext_gcd(35, 15, &x, &y);
    // EXPECT: ext_gcd(35, 15) = 5, x=1, y=-2
    printf("ext_gcd(35, 15) = %d, x=%d, y=%d\n", g, x, y);

    // EXPECT: coprime(8, 15) = 1
    printf("coprime(8, 15) = %d\n", are_coprime(8, 15));

    // EXPECT: coprime(12, 15) = 0
    printf("coprime(12, 15) = %d\n", are_coprime(12, 15));

    arr[0] = 12; arr[1] = 18; arr[2] = 24; arr[3] = 36; arr[4] = 60;
    // EXPECT: gcd_array = 6
    printf("gcd_array = %d\n", gcd_array(arr, 5));

    arr[0] = 2; arr[1] = 3; arr[2] = 4; arr[3] = 5; arr[4] = 6;
    // EXPECT: lcm_array = 60
    printf("lcm_array = %d\n", lcm_array(arr, 5));

    // EXPECT: totient(12) = 4
    printf("totient(12) = %d\n", euler_totient(12));

    // EXPECT: totient(7) = 6
    printf("totient(7) = %d\n", euler_totient(7));

    // EXPECT: binary_gcd(48, 18) = 6
    printf("binary_gcd(48, 18) = %d\n", binary_gcd(48, 18));

    // EXPECT: binary_gcd(100, 75) = 25
    printf("binary_gcd(100, 75) = %d\n", binary_gcd(100, 75));

    return 0;
}
