int printf(const char *fmt, ...);

// Integer square root and cube root using Newton's method

// Integer square root: largest x such that x*x <= n
int isqrt(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    int x = n;
    int y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

// Integer cube root: largest x such that x*x*x <= n
// Uses binary search for robustness
int icbrt(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    int lo = 1;
    int hi = 1290;  // cube root of 2^31 is about 1290
    if (hi > n) hi = n;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        int cube = mid * mid * mid;
        if (cube == n) return mid;
        if (cube < n) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

// Check if n is a perfect square
int is_perfect_square(int n) {
    if (n < 0) return 0;
    int s = isqrt(n);
    return s * s == n;
}

// Check if n is a perfect cube
int is_perfect_cube(int n) {
    if (n <= 0) return 0;
    int c = icbrt(n);
    return c * c * c == n;
}

// Integer 4th root
int i4thrt(int n) {
    return isqrt(isqrt(n));
}

// Count perfect squares in [1..n]
int count_perfect_squares(int n) {
    return isqrt(n);
}

// Count perfect cubes in [1..n]
int count_perfect_cubes(int n) {
    return icbrt(n);
}

// Sum of integer square roots from 1 to n
int sum_isqrt(int n) {
    int sum = 0;
    int i;
    for (i = 1; i <= n; i++) {
        sum = sum + isqrt(i);
    }
    return sum;
}

// Find largest perfect square <= n
int largest_square_leq(int n) {
    int s = isqrt(n);
    return s * s;
}

// Find smallest perfect square >= n
int smallest_square_geq(int n) {
    int s = isqrt(n);
    if (s * s == n) return n;
    return (s + 1) * (s + 1);
}

// Digital root of a number
int digital_root(int n) {
    while (n >= 10) {
        int sum = 0;
        while (n > 0) {
            sum = sum + n % 10;
            n = n / 10;
        }
        n = sum;
    }
    return n;
}

// Check if n is a sum of two squares
int is_sum_of_two_squares(int n) {
    int a;
    for (a = 0; a * a <= n; a++) {
        int remainder = n - a * a;
        if (is_perfect_square(remainder)) {
            return 1;
        }
    }
    return 0;
}

// Count how many ways n can be expressed as sum of two squares (a<=b)
int count_sum_two_squares(int n) {
    int count = 0;
    int a;
    for (a = 0; a * a <= n / 2; a++) {
        int remainder = n - a * a;
        if (is_perfect_square(remainder)) {
            count = count + 1;
        }
    }
    return count;
}

int main(void) {
    // Basic square roots
    printf("isqrt(0): %d\n", isqrt(0));
    // EXPECT: isqrt(0): 0

    printf("isqrt(1): %d\n", isqrt(1));
    // EXPECT: isqrt(1): 1

    printf("isqrt(4): %d\n", isqrt(4));
    // EXPECT: isqrt(4): 2

    printf("isqrt(9): %d\n", isqrt(9));
    // EXPECT: isqrt(9): 3

    printf("isqrt(10): %d\n", isqrt(10));
    // EXPECT: isqrt(10): 3

    printf("isqrt(100): %d\n", isqrt(100));
    // EXPECT: isqrt(100): 10

    printf("isqrt(1000): %d\n", isqrt(1000));
    // EXPECT: isqrt(1000): 31

    printf("isqrt(10000): %d\n", isqrt(10000));
    // EXPECT: isqrt(10000): 100

    // Cube roots
    printf("icbrt(1): %d\n", icbrt(1));
    // EXPECT: icbrt(1): 1

    printf("icbrt(8): %d\n", icbrt(8));
    // EXPECT: icbrt(8): 2

    printf("icbrt(27): %d\n", icbrt(27));
    // EXPECT: icbrt(27): 3

    printf("icbrt(64): %d\n", icbrt(64));
    // EXPECT: icbrt(64): 4

    printf("icbrt(125): %d\n", icbrt(125));
    // EXPECT: icbrt(125): 5

    printf("icbrt(100): %d\n", icbrt(100));
    // EXPECT: icbrt(100): 4

    // Perfect square/cube checks
    printf("perf_sq(144): %d\n", is_perfect_square(144));
    // EXPECT: perf_sq(144): 1

    printf("perf_sq(145): %d\n", is_perfect_square(145));
    // EXPECT: perf_sq(145): 0

    printf("perf_cube(216): %d\n", is_perfect_cube(216));
    // EXPECT: perf_cube(216): 1

    printf("perf_cube(200): %d\n", is_perfect_cube(200));
    // EXPECT: perf_cube(200): 0

    // Counting
    printf("perfect squares <= 100: %d\n", count_perfect_squares(100));
    // EXPECT: perfect squares <= 100: 10

    printf("perfect cubes <= 100: %d\n", count_perfect_cubes(100));
    // EXPECT: perfect cubes <= 100: 4

    // Sum of isqrt(1..10) = 1+1+1+2+2+2+2+2+3+3 = 19
    printf("sum isqrt(1..10): %d\n", sum_isqrt(10));
    // EXPECT: sum isqrt(1..10): 19

    // Largest/smallest squares
    printf("largest sq <= 50: %d\n", largest_square_leq(50));
    // EXPECT: largest sq <= 50: 49

    printf("smallest sq >= 50: %d\n", smallest_square_geq(50));
    // EXPECT: smallest sq >= 50: 64

    // Sum of two squares
    // 5 = 1+4, 10 = 1+9, 13 = 4+9
    printf("sum2sq(5): %d\n", is_sum_of_two_squares(5));
    // EXPECT: sum2sq(5): 1

    printf("sum2sq(3): %d\n", is_sum_of_two_squares(3));
    // EXPECT: sum2sq(3): 0

    // 25 = 0+25 = 9+16, so 2 ways
    printf("ways(25): %d\n", count_sum_two_squares(25));
    // EXPECT: ways(25): 2

    // 50 = 1+49 = 25+25, so 2 ways
    printf("ways(50): %d\n", count_sum_two_squares(50));
    // EXPECT: ways(50): 2

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
