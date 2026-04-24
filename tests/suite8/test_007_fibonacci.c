int printf(const char *fmt, ...);

// Iterative fibonacci
int fib(int n) {
    int a;
    int b;
    int temp;
    int i;
    if (n <= 0) return 0;
    if (n == 1) return 1;
    a = 0;
    b = 1;
    for (i = 2; i <= n; i++) {
        temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

// Recursive fibonacci (for small values)
int fib_rec(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fib_rec(n - 1) + fib_rec(n - 2);
}

// Check if n is a Fibonacci number
int is_perfect_square(int n) {
    int lo;
    int hi;
    int mid;
    if (n < 0) return 0;
    if (n == 0) return 1;
    lo = 0;
    hi = n;
    if (hi > 46340) hi = 46340;
    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;
        if (mid * mid == n) return 1;
        if (mid * mid < n) lo = mid + 1;
        else hi = mid - 1;
    }
    return 0;
}

int is_fibonacci(int n) {
    return is_perfect_square(5 * n * n + 4) || is_perfect_square(5 * n * n - 4);
}

// Sum of first n Fibonacci numbers
int fib_sum(int n) {
    int total;
    int i;
    total = 0;
    for (i = 0; i <= n; i++) {
        total = total + fib(i);
    }
    return total;
}

// Sum of even Fibonacci numbers up to limit
int fib_even_sum(int limit) {
    int a;
    int b;
    int temp;
    int sum;
    a = 0;
    b = 1;
    sum = 0;
    while (a <= limit) {
        if (a % 2 == 0) {
            sum = sum + a;
        }
        temp = a + b;
        a = b;
        b = temp;
    }
    return sum;
}

// Tribonacci: T(n) = T(n-1) + T(n-2) + T(n-3), T(0)=0, T(1)=0, T(2)=1
int tribonacci(int n) {
    int a;
    int b;
    int c;
    int temp;
    int i;
    if (n <= 1) return 0;
    if (n == 2) return 1;
    a = 0;
    b = 0;
    c = 1;
    for (i = 3; i <= n; i++) {
        temp = a + b + c;
        a = b;
        b = c;
        c = temp;
    }
    return c;
}

// Lucas numbers: L(0)=2, L(1)=1, L(n)=L(n-1)+L(n-2)
int lucas(int n) {
    int a;
    int b;
    int temp;
    int i;
    if (n == 0) return 2;
    if (n == 1) return 1;
    a = 2;
    b = 1;
    for (i = 2; i <= n; i++) {
        temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

// GCD property: gcd(fib(m), fib(n)) = fib(gcd(m,n))
int gcd(int a, int b) {
    while (b != 0) {
        int t;
        t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Find index of fibonacci number closest to n
int fib_closest_idx(int n) {
    int i;
    int prev;
    int curr;
    prev = 0;
    for (i = 0; i < 30; i++) {
        curr = fib(i);
        if (curr >= n) {
            if (i == 0) return 0;
            if (curr - n < n - prev) return i;
            return i - 1;
        }
        prev = curr;
    }
    return 29;
}

void print_fib_sequence(int n) {
    int i;
    printf("Fib seq:");
    for (i = 0; i < n; i++) {
        printf(" %d", fib(i));
    }
    printf("\n");
}

void print_lucas_sequence(int n) {
    int i;
    printf("Lucas seq:");
    for (i = 0; i < n; i++) {
        printf(" %d", lucas(i));
    }
    printf("\n");
}

int main() {
    // EXPECT: fib(0) = 0
    printf("fib(0) = %d\n", fib(0));

    // EXPECT: fib(1) = 1
    printf("fib(1) = %d\n", fib(1));

    // EXPECT: fib(10) = 55
    printf("fib(10) = %d\n", fib(10));

    // EXPECT: fib(20) = 6765
    printf("fib(20) = %d\n", fib(20));

    // EXPECT: fib_rec(10) = 55
    printf("fib_rec(10) = %d\n", fib_rec(10));

    // EXPECT: is_fib(8) = 1
    printf("is_fib(8) = %d\n", is_fibonacci(8));

    // EXPECT: is_fib(9) = 0
    printf("is_fib(9) = %d\n", is_fibonacci(9));

    // EXPECT: fib_sum(10) = 143
    printf("fib_sum(10) = %d\n", fib_sum(10));

    // EXPECT: fib_even_sum(100) = 44
    printf("fib_even_sum(100) = %d\n", fib_even_sum(100));

    // EXPECT: tribonacci(7) = 13
    printf("tribonacci(7) = %d\n", tribonacci(7));

    // EXPECT: tribonacci(10) = 81
    printf("tribonacci(10) = %d\n", tribonacci(10));

    // EXPECT: gcd(fib(6),fib(9)) = fib(gcd(6,9)): 1
    printf("gcd(fib(6),fib(9)) = fib(gcd(6,9)): %d\n",
           gcd(fib(6), fib(9)) == fib(gcd(6, 9)));

    // EXPECT: fib_closest(10) = 6
    printf("fib_closest(10) = %d\n", fib_closest_idx(10));

    // EXPECT: Fib seq: 0 1 1 2 3 5 8 13 21 34 55 89
    print_fib_sequence(12);

    // EXPECT: Lucas seq: 2 1 3 4 7 11 18 29 47 76
    print_lucas_sequence(10);

    return 0;
}
