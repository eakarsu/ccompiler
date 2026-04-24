int printf(const char *fmt, ...);

/* Factorial: n! */
int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

/* Fibonacci: fib(0)=0, fib(1)=1, fib(n)=fib(n-1)+fib(n-2) */
int fib(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fib(n - 1) + fib(n - 2);
}

/* Tower of Hanoi: count minimum moves for n disks = 2^n - 1 */
int hanoi_count(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return 2 * hanoi_count(n - 1) + 1;
}

/* Binary search (recursive) */
/* Returns index of target in sorted arr[lo..hi], or -1 if not found */
int bsearch_rec(int *arr, int lo, int hi, int target) {
    if (lo > hi) return -1;
    int mid = lo + (hi - lo) / 2;
    if (arr[mid] == target) return mid;
    if (arr[mid] < target) return bsearch_rec(arr, mid + 1, hi, target);
    return bsearch_rec(arr, lo, mid - 1, target);
}

/* Recursive sum of 1..n */
int sum_to(int n) {
    if (n <= 0) return 0;
    return n + sum_to(n - 1);
}

/* Recursive power: base^exp (exp >= 0) */
int power(int base, int exp) {
    if (exp == 0) return 1;
    if (exp == 1) return base;
    int half = power(base, exp / 2);
    if (exp % 2 == 0) return half * half;
    return base * half * half;
}

/* Ackermann function (small values only) */
/* A(0,n) = n+1 */
/* A(m,0) = A(m-1,1) */
/* A(m,n) = A(m-1, A(m,n-1)) */
int ackermann(int m, int n) {
    if (m == 0) return n + 1;
    if (n == 0) return ackermann(m - 1, 1);
    return ackermann(m - 1, ackermann(m, n - 1));
}

/* Recursive digit sum */
int digit_sum(int n) {
    if (n < 0) n = -n;
    if (n < 10) return n;
    return (n % 10) + digit_sum(n / 10);
}

/* Recursive GCD (Euclidean algorithm) */
int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

/* Count digits recursively */
int count_digits(int n) {
    if (n < 0) n = -n;
    if (n < 10) return 1;
    return 1 + count_digits(n / 10);
}

/* Recursive reverse digits: reverse(123) = 321 */
int reverse_digits(int n) {
    if (n < 10) return n;
    int last = n % 10;
    int rest = reverse_digits(n / 10);
    int digits_in_rest = count_digits(n / 10);
    /* last * 10^digits_in_rest + rest */
    int mul = 1;
    int i;
    for (i = 0; i < digits_in_rest; i++) mul *= 10;
    return last * mul + rest;
}

int main(void) {
    /* Factorial */
    printf("%d\n", factorial(0));   // EXPECT: 1
    printf("%d\n", factorial(1));   // EXPECT: 1
    printf("%d\n", factorial(2));   // EXPECT: 2
    printf("%d\n", factorial(3));   // EXPECT: 6
    printf("%d\n", factorial(4));   // EXPECT: 24
    printf("%d\n", factorial(5));   // EXPECT: 120
    printf("%d\n", factorial(6));   // EXPECT: 720
    printf("%d\n", factorial(10));  // EXPECT: 3628800

    /* Fibonacci */
    printf("%d\n", fib(0));   // EXPECT: 0
    printf("%d\n", fib(1));   // EXPECT: 1
    printf("%d\n", fib(2));   // EXPECT: 1
    printf("%d\n", fib(3));   // EXPECT: 2
    printf("%d\n", fib(4));   // EXPECT: 3
    printf("%d\n", fib(5));   // EXPECT: 5
    printf("%d\n", fib(6));   // EXPECT: 8
    printf("%d\n", fib(7));   // EXPECT: 13
    printf("%d\n", fib(10));  // EXPECT: 55

    /* Tower of Hanoi move counts */
    printf("%d\n", hanoi_count(1));  // EXPECT: 1
    printf("%d\n", hanoi_count(2));  // EXPECT: 3
    printf("%d\n", hanoi_count(3));  // EXPECT: 7
    printf("%d\n", hanoi_count(4));  // EXPECT: 15
    printf("%d\n", hanoi_count(5));  // EXPECT: 31

    /* Binary search */
    int arr[10];
    arr[0] = 2;  arr[1] = 5;  arr[2] = 8;  arr[3] = 12; arr[4] = 16;
    arr[5] = 23; arr[6] = 38; arr[7] = 56; arr[8] = 72; arr[9] = 91;
    printf("%d\n", bsearch_rec(arr, 0, 9, 23));  // EXPECT: 5
    printf("%d\n", bsearch_rec(arr, 0, 9, 2));   // EXPECT: 0
    printf("%d\n", bsearch_rec(arr, 0, 9, 91));  // EXPECT: 9
    printf("%d\n", bsearch_rec(arr, 0, 9, 99));  // EXPECT: -1

    /* Recursive sum */
    printf("%d\n", sum_to(0));   // EXPECT: 0
    printf("%d\n", sum_to(5));   // EXPECT: 15
    printf("%d\n", sum_to(10));  // EXPECT: 55
    printf("%d\n", sum_to(100)); // EXPECT: 5050

    /* Power */
    printf("%d\n", power(2, 0));   // EXPECT: 1
    printf("%d\n", power(2, 1));   // EXPECT: 2
    printf("%d\n", power(2, 8));   // EXPECT: 256
    printf("%d\n", power(3, 4));   // EXPECT: 81
    printf("%d\n", power(5, 3));   // EXPECT: 125

    /* Ackermann(2,3) = 9 */
    /* A(2,3): A(1, A(2,2))
       A(2,2): A(1, A(2,1))
       A(2,1): A(1, A(2,0)) = A(1, A(1,1)) = A(1, A(0,A(1,0)))
               A(1,0) = A(0,1) = 2
               A(1,2) = A(0,A(1,1)) = A(0, A(0,A(1,0))) = A(0, A(0,2)) = A(0,3) = 4
               So A(2,1) = A(1,2) = 4... let me recompute:
       A(2,0) = A(1,1) = A(0,A(1,0)) = A(0,A(0,1)) = A(0,2) = 3
       A(2,1) = A(1,A(2,0)) = A(1,3) = A(0,A(1,2)) = A(0,A(0,A(1,1)))
               A(1,1) = A(0,A(1,0)) = A(0,2) = 3
               A(1,2) = A(0,A(1,1)) = A(0,3) = 4
               A(1,3) = A(0,A(1,2)) = A(0,4) = 5
       A(2,1) = 5
       A(2,2) = A(1,A(2,1)) = A(1,5) = A(0,A(1,4))
               A(1,4) = A(0,A(1,3)) = A(0,5) = 6
               A(1,5) = A(0,A(1,4)) = A(0,6) = 7
       A(2,2) = 7
       A(2,3) = A(1,A(2,2)) = A(1,7)
               A(1,6) = A(0,A(1,5)) = A(0,7) = 8
               A(1,7) = A(0,A(1,6)) = A(0,8) = 9
       A(2,3) = 9
    */
    printf("%d\n", ackermann(0, 0)); // EXPECT: 1
    printf("%d\n", ackermann(0, 5)); // EXPECT: 6
    printf("%d\n", ackermann(1, 0)); // EXPECT: 2
    printf("%d\n", ackermann(1, 3)); // EXPECT: 5
    printf("%d\n", ackermann(2, 0)); // EXPECT: 3
    printf("%d\n", ackermann(2, 3)); // EXPECT: 9

    /* Digit sum */
    printf("%d\n", digit_sum(0));    // EXPECT: 0
    printf("%d\n", digit_sum(9));    // EXPECT: 9
    printf("%d\n", digit_sum(123));  // EXPECT: 6
    printf("%d\n", digit_sum(999));  // EXPECT: 27

    /* GCD */
    printf("%d\n", gcd(12, 8));   // EXPECT: 4
    printf("%d\n", gcd(100, 75)); // EXPECT: 25
    printf("%d\n", gcd(7, 13));   // EXPECT: 1

    return 0;
}
