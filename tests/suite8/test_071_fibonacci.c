int printf(const char *fmt, ...);

// Fibonacci: iterative and recursive implementations
// Tests memoization via iterative approach and basic recursion

int fib_recursive(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fib_recursive(n - 1) + fib_recursive(n - 2);
}

int fib_iterative(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    int prev2 = 0;
    int prev1 = 1;
    int curr = 0;
    int i;
    for (i = 2; i <= n; i++) {
        curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}

// Bottom-up fibonacci filling an array
void fib_table(int *table, int n) {
    int i;
    table[0] = 0;
    if (n > 0) table[1] = 1;
    for (i = 2; i <= n; i++) {
        table[i] = table[i - 1] + table[i - 2];
    }
}

// Sum of first n fibonacci numbers
int fib_sum(int n) {
    int sum = 0;
    int i;
    for (i = 0; i <= n; i++) {
        sum = sum + fib_iterative(i);
    }
    return sum;
}

// Check if a number is a fibonacci number
int is_fibonacci(int num) {
    int a = 0;
    int b = 1;
    int temp;
    if (num == 0) return 1;
    while (b < num) {
        temp = b;
        b = a + b;
        a = temp;
    }
    if (b == num) return 1;
    return 0;
}

// Count even fibonacci numbers up to nth fib
int count_even_fibs(int n) {
    int count = 0;
    int i;
    for (i = 0; i <= n; i++) {
        int f = fib_iterative(i);
        if (f % 2 == 0) {
            count = count + 1;
        }
    }
    return count;
}

int main() {
    int i;
    int table[21];

    // Test recursive fibonacci for small values
    // EXPECT: fib_rec(0) = 0
    printf("fib_rec(0) = %d\n", fib_recursive(0));
    // EXPECT: fib_rec(1) = 1
    printf("fib_rec(1) = %d\n", fib_recursive(1));
    // EXPECT: fib_rec(5) = 5
    printf("fib_rec(5) = %d\n", fib_recursive(5));
    // EXPECT: fib_rec(10) = 55
    printf("fib_rec(10) = %d\n", fib_recursive(10));

    // Test iterative fibonacci
    // EXPECT: fib_iter(0) = 0
    printf("fib_iter(0) = %d\n", fib_iterative(0));
    // EXPECT: fib_iter(1) = 1
    printf("fib_iter(1) = %d\n", fib_iterative(1));
    // EXPECT: fib_iter(10) = 55
    printf("fib_iter(10) = %d\n", fib_iterative(10));
    // EXPECT: fib_iter(15) = 610
    printf("fib_iter(15) = %d\n", fib_iterative(15));
    // EXPECT: fib_iter(20) = 6765
    printf("fib_iter(20) = %d\n", fib_iterative(20));

    // Verify recursive and iterative match
    int match = 1;
    for (i = 0; i <= 15; i++) {
        if (fib_recursive(i) != fib_iterative(i)) {
            match = 0;
        }
    }
    // EXPECT: rec_iter_match = 1
    printf("rec_iter_match = %d\n", match);

    // Test table-based fibonacci
    fib_table(table, 20);
    // EXPECT: table[0] = 0
    printf("table[0] = %d\n", table[0]);
    // EXPECT: table[5] = 5
    printf("table[5] = %d\n", table[5]);
    // EXPECT: table[10] = 55
    printf("table[10] = %d\n", table[10]);
    // EXPECT: table[20] = 6765
    printf("table[20] = %d\n", table[20]);

    // Test sum of fibonacci numbers
    // EXPECT: fib_sum(5) = 12
    printf("fib_sum(5) = %d\n", fib_sum(5));
    // EXPECT: fib_sum(10) = 143
    printf("fib_sum(10) = %d\n", fib_sum(10));

    // Test is_fibonacci
    // EXPECT: is_fib(8) = 1
    printf("is_fib(8) = %d\n", is_fibonacci(8));
    // EXPECT: is_fib(9) = 0
    printf("is_fib(9) = %d\n", is_fibonacci(9));
    // EXPECT: is_fib(13) = 1
    printf("is_fib(13) = %d\n", is_fibonacci(13));
    // EXPECT: is_fib(14) = 0
    printf("is_fib(14) = %d\n", is_fibonacci(14));

    // Test count even fibs
    // EXPECT: even_fibs(10) = 4
    printf("even_fibs(10) = %d\n", count_even_fibs(10));
    // EXPECT: even_fibs(15) = 6
    printf("even_fibs(15) = %d\n", count_even_fibs(15));

    return 0;
}
