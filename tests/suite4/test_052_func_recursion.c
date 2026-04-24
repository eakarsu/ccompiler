int printf(const char *fmt, ...);

// Factorial: n! = n * (n-1)!
int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

// Fibonacci: fib(n) = fib(n-1) + fib(n-2)
int fibonacci(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// Sum of digits: e.g. 1234 -> 1+2+3+4 = 10
int sum_digits(int n) {
    if (n < 0) n = -n;
    if (n < 10) return n;
    return (n % 10) + sum_digits(n / 10);
}

// Power: base^exp (exp >= 0)
int power(int base, int exp) {
    if (exp == 0) return 1;
    return base * power(base, exp - 1);
}

// GCD using Euclidean algorithm
int gcd(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    if (b == 0) return a;
    return gcd(b, a % b);
}

// Count digits in a number
int count_digits(int n) {
    if (n < 0) n = -n;
    if (n < 10) return 1;
    return 1 + count_digits(n / 10);
}

// Recursive sum from 1 to n
int sum_to_n(int n) {
    if (n <= 0) return 0;
    return n + sum_to_n(n - 1);
}

// Recursive binary search in sorted array
int binary_search(int *arr, int lo, int hi, int target) {
    if (lo > hi) return -1;
    int mid = lo + (hi - lo) / 2;
    if (arr[mid] == target) return mid;
    if (arr[mid] < target) return binary_search(arr, mid + 1, hi, target);
    return binary_search(arr, lo, mid - 1, target);
}

// Ackermann function (small values only)
int ackermann(int m, int n) {
    if (m == 0) return n + 1;
    if (n == 0) return ackermann(m - 1, 1);
    return ackermann(m - 1, ackermann(m, n - 1));
}

// Recursive reverse of an array (helper)
void reverse_helper(int *arr, int lo, int hi) {
    if (lo >= hi) return;
    int tmp = arr[lo];
    arr[lo] = arr[hi];
    arr[hi] = tmp;
    reverse_helper(arr, lo + 1, hi - 1);
}

// Recursive max in array
int array_max(int *arr, int n) {
    if (n == 1) return arr[0];
    int sub_max = array_max(arr, n - 1);
    if (arr[n - 1] > sub_max) return arr[n - 1];
    return sub_max;
}

// Tower of Hanoi move counter
int hanoi_moves(int n) {
    if (n <= 0) return 0;
    return 2 * hanoi_moves(n - 1) + 1;
}

int main(void) {
    // Factorial tests
    // EXPECT: 1
    printf("%d\n", factorial(0));
    // EXPECT: 1
    printf("%d\n", factorial(1));
    // EXPECT: 120
    printf("%d\n", factorial(5));
    // EXPECT: 5040
    printf("%d\n", factorial(7));

    // Fibonacci tests
    // EXPECT: 0
    printf("%d\n", fibonacci(0));
    // EXPECT: 1
    printf("%d\n", fibonacci(1));
    // EXPECT: 55
    printf("%d\n", fibonacci(10));
    // EXPECT: 89
    printf("%d\n", fibonacci(11));

    // Sum of digits
    // EXPECT: 10
    printf("%d\n", sum_digits(1234));
    // EXPECT: 15
    printf("%d\n", sum_digits(555));
    // EXPECT: 1
    printf("%d\n", sum_digits(1000));

    // Power
    // EXPECT: 1
    printf("%d\n", power(5, 0));
    // EXPECT: 8
    printf("%d\n", power(2, 3));
    // EXPECT: 243
    printf("%d\n", power(3, 5));

    // GCD
    // EXPECT: 6
    printf("%d\n", gcd(12, 18));
    // EXPECT: 1
    printf("%d\n", gcd(7, 13));
    // EXPECT: 5
    printf("%d\n", gcd(25, 15));

    // Count digits
    // EXPECT: 1
    printf("%d\n", count_digits(0));
    // EXPECT: 4
    printf("%d\n", count_digits(1234));
    // EXPECT: 6
    printf("%d\n", count_digits(999999));

    // Sum 1 to n
    // EXPECT: 55
    printf("%d\n", sum_to_n(10));
    // EXPECT: 5050
    printf("%d\n", sum_to_n(100));

    // Binary search
    int sorted[] = {2, 5, 8, 12, 16, 23, 38, 56, 72, 91};
    // EXPECT: 4
    printf("%d\n", binary_search(sorted, 0, 9, 16));
    // EXPECT: 0
    printf("%d\n", binary_search(sorted, 0, 9, 2));
    // EXPECT: -1
    printf("%d\n", binary_search(sorted, 0, 9, 99));

    // Ackermann (small values)
    // ack(0,0)=1, ack(1,1)=3, ack(2,2)=7, ack(3,1)=13
    // EXPECT: 1
    printf("%d\n", ackermann(0, 0));
    // EXPECT: 3
    printf("%d\n", ackermann(1, 1));
    // EXPECT: 7
    printf("%d\n", ackermann(2, 2));

    // Reverse array
    int arr[] = {1, 2, 3, 4, 5};
    reverse_helper(arr, 0, 4);
    // EXPECT: 5 4 3 2 1
    printf("%d %d %d %d %d\n", arr[0], arr[1], arr[2], arr[3], arr[4]);

    // Array max
    int vals[] = {3, 7, 1, 9, 4, 6};
    // EXPECT: 9
    printf("%d\n", array_max(vals, 6));

    // Hanoi moves: 2^n - 1
    // EXPECT: 1
    printf("%d\n", hanoi_moves(1));
    // EXPECT: 7
    printf("%d\n", hanoi_moves(3));
    // EXPECT: 31
    printf("%d\n", hanoi_moves(5));

    return 0;
}
