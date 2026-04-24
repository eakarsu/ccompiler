int printf(const char *fmt, ...);

// Integer square root using binary search
int isqrt(int n) {
    int lo;
    int hi;
    int mid;
    int result;
    if (n < 0) return -1;
    if (n == 0) return 0;
    lo = 1;
    hi = n;
    if (hi > 46340) hi = 46340;  // sqrt of INT_MAX roughly
    result = 0;
    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;
        if (mid <= n / mid) {
            result = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return result;
}

// Integer square root using Newton's method
int isqrt_newton(int n) {
    int x;
    int x1;
    if (n <= 0) return 0;
    x = n;
    if (x > 46340) x = 46340;
    x1 = (x + n / x) / 2;
    while (x1 < x) {
        x = x1;
        x1 = (x + n / x) / 2;
    }
    return x;
}

// Check if n is a perfect square
int is_perfect_square(int n) {
    int s;
    if (n < 0) return 0;
    s = isqrt(n);
    return s * s == n;
}

// Integer cube root (simple binary search)
int icbrt(int n) {
    int lo;
    int hi;
    int mid;
    int result;
    if (n < 0) return -1;
    if (n == 0) return 0;
    lo = 1;
    hi = n;
    if (hi > 1290) hi = 1290;  // cbrt of ~2^31
    result = 0;
    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;
        if (mid <= n / mid / mid) {
            result = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return result;
}

// Count perfect squares in range [a, b]
int count_perfect_squares(int a, int b) {
    int count;
    int i;
    int s;
    count = 0;
    for (i = a; i <= b; i++) {
        s = isqrt(i);
        if (s * s == i) {
            count = count + 1;
        }
    }
    return count;
}

// Find nearest perfect square to n
int nearest_perfect_square(int n) {
    int s;
    int lo;
    int hi;
    s = isqrt(n);
    lo = s * s;
    hi = (s + 1) * (s + 1);
    if (n - lo <= hi - n) return lo;
    return hi;
}

// Sum of digits of isqrt of first N integers
int sum_isqrt(int n) {
    int total;
    int i;
    total = 0;
    for (i = 1; i <= n; i++) {
        total = total + isqrt(i);
    }
    return total;
}

// Pythagorean triples with hypotenuse <= limit
void print_pythagorean(int limit) {
    int a;
    int b;
    int c2;
    int c;
    int count;
    count = 0;
    for (a = 1; a < limit; a++) {
        for (b = a; b < limit; b++) {
            c2 = a * a + b * b;
            c = isqrt(c2);
            if (c <= limit && c * c == c2) {
                count = count + 1;
            }
        }
    }
    printf("Pythagorean triples (hyp<=%d): %d\n", limit, count);
}

int main() {
    // EXPECT: isqrt(0) = 0
    printf("isqrt(0) = %d\n", isqrt(0));

    // EXPECT: isqrt(1) = 1
    printf("isqrt(1) = %d\n", isqrt(1));

    // EXPECT: isqrt(4) = 2
    printf("isqrt(4) = %d\n", isqrt(4));

    // EXPECT: isqrt(10) = 3
    printf("isqrt(10) = %d\n", isqrt(10));

    // EXPECT: isqrt(100) = 10
    printf("isqrt(100) = %d\n", isqrt(100));

    // EXPECT: isqrt(10000) = 100
    printf("isqrt(10000) = %d\n", isqrt(10000));

    // EXPECT: newton(144) = 12
    printf("newton(144) = %d\n", isqrt_newton(144));

    // EXPECT: newton(999) = 31
    printf("newton(999) = %d\n", isqrt_newton(999));

    // EXPECT: perfect_sq(49) = 1
    printf("perfect_sq(49) = %d\n", is_perfect_square(49));

    // EXPECT: perfect_sq(50) = 0
    printf("perfect_sq(50) = %d\n", is_perfect_square(50));

    // EXPECT: icbrt(27) = 3
    printf("icbrt(27) = %d\n", icbrt(27));

    // EXPECT: icbrt(100) = 4
    printf("icbrt(100) = %d\n", icbrt(100));

    // EXPECT: perfect_sq in [1,100] = 10
    printf("perfect_sq in [1,100] = %d\n", count_perfect_squares(1, 100));

    // EXPECT: nearest_sq(30) = 25
    printf("nearest_sq(30) = %d\n", nearest_perfect_square(30));

    // EXPECT: nearest_sq(33) = 36
    printf("nearest_sq(33) = %d\n", nearest_perfect_square(33));

    // EXPECT: sum_isqrt(20) = 54
    printf("sum_isqrt(20) = %d\n", sum_isqrt(20));

    // EXPECT: Pythagorean triples (hyp<=50): 20
    print_pythagorean(50);

    return 0;
}
