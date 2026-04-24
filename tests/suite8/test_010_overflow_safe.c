int printf(const char *fmt, ...);

// Safe addition: returns 1 on success, 0 on overflow
int safe_add(int a, int b, int *result) {
    // Check for overflow: if signs are same and result sign differs
    int sum;
    sum = a + b;
    if (a > 0 && b > 0 && sum <= 0) return 0;
    if (a < 0 && b < 0 && sum >= 0) return 0;
    *result = sum;
    return 1;
}

// Safe subtraction
int safe_sub(int a, int b, int *result) {
    int diff;
    diff = a - b;
    // Overflow if: a positive, b negative, result negative (or vice versa)
    if (b > 0 && a < 0 && diff > 0) return 0;
    if (b < 0 && a > 0 && diff < 0) return 0;
    *result = diff;
    return 1;
}

// Safe multiplication (check using division)
int safe_mul(int a, int b, int *result) {
    int prod;
    if (a == 0 || b == 0) {
        *result = 0;
        return 1;
    }
    prod = a * b;
    if (prod / a != b) return 0;
    *result = prod;
    return 1;
}

// Absolute value (safe version)
int safe_abs(int n) {
    if (n < 0) return -n;
    return n;
}

// Saturating addition: clamp to max/min instead of overflowing
int sat_add(int a, int b) {
    int result;
    if (safe_add(a, b, &result)) return result;
    if (a > 0) return 2147483647;
    return -2147483647 - 1;
}

// Saturating subtraction
int sat_sub(int a, int b) {
    int result;
    if (safe_sub(a, b, &result)) return result;
    if (a > 0) return 2147483647;
    return -2147483647 - 1;
}

// Saturating multiplication
int sat_mul(int a, int b) {
    int result;
    if (safe_mul(a, b, &result)) return result;
    if ((a > 0 && b > 0) || (a < 0 && b < 0)) return 2147483647;
    return -2147483647 - 1;
}

// Average without overflow: (a + b) / 2
int safe_avg(int a, int b) {
    return a / 2 + b / 2 + (a % 2 + b % 2) / 2;
}

// Midpoint without overflow (for binary search)
int safe_midpoint(int lo, int hi) {
    return lo + (hi - lo) / 2;
}

// Power with overflow detection
int safe_pow(int base, int exp, int *result) {
    int r;
    int ok;
    r = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            ok = safe_mul(r, base, &r);
            if (!ok) return 0;
        }
        if (exp / 2 > 0) {
            ok = safe_mul(base, base, &base);
            if (!ok && exp / 2 > 0) return 0;
        }
        exp = exp / 2;
    }
    *result = r;
    return 1;
}

// Factorial with overflow detection
int safe_factorial(int n, int *result) {
    int r;
    int i;
    int ok;
    r = 1;
    for (i = 2; i <= n; i++) {
        ok = safe_mul(r, i, &r);
        if (!ok) return 0;
    }
    *result = r;
    return 1;
}

// Sum of array with overflow detection
int safe_sum_array(int *arr, int n, int *result) {
    int s;
    int i;
    int ok;
    s = 0;
    for (i = 0; i < n; i++) {
        ok = safe_add(s, arr[i], &s);
        if (!ok) return 0;
    }
    *result = s;
    return 1;
}

// Division rounding towards negative infinity
int floor_div(int a, int b) {
    int d;
    d = a / b;
    // C division truncates towards zero; adjust for negative
    if ((a ^ b) < 0 && d * b != a) {
        d = d - 1;
    }
    return d;
}

// Ceiling division
int ceil_div(int a, int b) {
    int d;
    d = a / b;
    if ((a ^ b) >= 0 && d * b != a) {
        d = d + 1;
    }
    return d;
}

// Modulo that always returns non-negative result
int positive_mod(int a, int b) {
    int r;
    r = a % b;
    if (r < 0) r = r + (b > 0 ? b : -b);
    return r;
}

int main() {
    int result;
    int ok;
    int arr[5];

    // EXPECT: safe_add(100, 200) = 1, result=300
    ok = safe_add(100, 200, &result);
    printf("safe_add(100, 200) = %d, result=%d\n", ok, result);

    // EXPECT: safe_add(2000000000, 2000000000) = 0, overflow
    ok = safe_add(2000000000, 2000000000, &result);
    printf("safe_add(2000000000, 2000000000) = %d, overflow\n", ok);

    // EXPECT: safe_sub(100, 300) = 1, result=-200
    ok = safe_sub(100, 300, &result);
    printf("safe_sub(100, 300) = %d, result=%d\n", ok, result);

    // EXPECT: safe_mul(1000, 2000) = 1, result=2000000
    ok = safe_mul(1000, 2000, &result);
    printf("safe_mul(1000, 2000) = %d, result=%d\n", ok, result);

    // EXPECT: safe_mul(100000, 100000) = 0, overflow
    ok = safe_mul(100000, 100000, &result);
    printf("safe_mul(100000, 100000) = %d, overflow\n", ok);

    // EXPECT: sat_add(2000000000, 2000000000) = 2147483647
    printf("sat_add(2000000000, 2000000000) = %d\n", sat_add(2000000000, 2000000000));

    // EXPECT: sat_sub(-2000000000, 2000000000) = -2147483648
    printf("sat_sub(-2000000000, 2000000000) = %d\n", sat_sub(-2000000000, 2000000000));

    // EXPECT: safe_avg(2000000000, 2000000000) = 2000000000
    printf("safe_avg(2000000000, 2000000000) = %d\n", safe_avg(2000000000, 2000000000));

    // EXPECT: safe_avg(1, 2) = 1
    printf("safe_avg(1, 2) = %d\n", safe_avg(1, 2));

    // EXPECT: safe_midpoint(100, 200) = 150
    printf("safe_midpoint(100, 200) = %d\n", safe_midpoint(100, 200));

    // EXPECT: safe_pow(2, 20) = 1, result=1048576
    ok = safe_pow(2, 20, &result);
    printf("safe_pow(2, 20) = %d, result=%d\n", ok, result);

    // EXPECT: safe_pow(2, 40) = 0, overflow
    ok = safe_pow(2, 40, &result);
    printf("safe_pow(2, 40) = %d, overflow\n", ok);

    // EXPECT: safe_fact(10) = 1, result=3628800
    ok = safe_factorial(10, &result);
    printf("safe_fact(10) = %d, result=%d\n", ok, result);

    // EXPECT: safe_fact(20) = 0, overflow
    ok = safe_factorial(20, &result);
    printf("safe_fact(20) = %d, overflow\n", ok);

    arr[0] = 100; arr[1] = 200; arr[2] = 300; arr[3] = 400; arr[4] = 500;
    // EXPECT: safe_sum = 1, result=1500
    ok = safe_sum_array(arr, 5, &result);
    printf("safe_sum = %d, result=%d\n", ok, result);

    // EXPECT: floor_div(7, 2) = 3
    printf("floor_div(7, 2) = %d\n", floor_div(7, 2));

    // EXPECT: floor_div(-7, 2) = -4
    printf("floor_div(-7, 2) = %d\n", floor_div(-7, 2));

    // EXPECT: ceil_div(7, 2) = 4
    printf("ceil_div(7, 2) = %d\n", ceil_div(7, 2));

    // EXPECT: ceil_div(-7, 2) = -3
    printf("ceil_div(-7, 2) = %d\n", ceil_div(-7, 2));

    // EXPECT: positive_mod(-7, 3) = 2
    printf("positive_mod(-7, 3) = %d\n", positive_mod(-7, 3));

    // EXPECT: positive_mod(7, 3) = 1
    printf("positive_mod(7, 3) = %d\n", positive_mod(7, 3));

    return 0;
}
