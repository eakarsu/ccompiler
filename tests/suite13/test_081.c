int printf(const char *fmt, ...);

// Fibonacci with memoization - top-down and bottom-up approaches

int memo[50];

void init_memo() {
    int i;
    for (i = 0; i < 50; i++) {
        memo[i] = -1;
    }
}

int fib_memo(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    if (memo[n] != -1) return memo[n];
    memo[n] = fib_memo(n - 1) + fib_memo(n - 2);
    return memo[n];
}

int fib_bottom_up(int n) {
    int dp[50];
    int i;
    dp[0] = 0;
    dp[1] = 1;
    for (i = 2; i <= n; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    return dp[n];
}

int fib_space_opt(int n) {
    int prev2;
    int prev1;
    int curr;
    int i;
    if (n <= 0) return 0;
    if (n == 1) return 1;
    prev2 = 0;
    prev1 = 1;
    for (i = 2; i <= n; i++) {
        curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

// Tribonacci: T(n) = T(n-1) + T(n-2) + T(n-3), T(0)=0, T(1)=0, T(2)=1
int trib_memo[50];

void init_trib() {
    int i;
    for (i = 0; i < 50; i++) {
        trib_memo[i] = -1;
    }
}

int tribonacci(int n) {
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (trib_memo[n] != -1) return trib_memo[n];
    trib_memo[n] = tribonacci(n - 1) + tribonacci(n - 2) + tribonacci(n - 3);
    return trib_memo[n];
}

// Count calls to measure memoization effectiveness
int call_count;

int fib_counted(int n) {
    call_count = call_count + 1;
    if (n <= 0) return 0;
    if (n == 1) return 1;
    if (memo[n] != -1) return memo[n];
    memo[n] = fib_counted(n - 1) + fib_counted(n - 2);
    return memo[n];
}

int fib_naive_count(int n) {
    call_count = call_count + 1;
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fib_naive_count(n - 1) + fib_naive_count(n - 2);
}

// Check if fib(n) is even
int fib_is_even(int n) {
    return fib_bottom_up(n) % 2 == 0;
}

// Sum of first n fibs
int fib_sum(int n) {
    int s;
    int i;
    s = 0;
    for (i = 0; i <= n; i++) {
        s = s + fib_bottom_up(i);
    }
    return s;
}

int main() {
    int i;
    int result;

    // Basic fibonacci values with memoization
    init_memo();
    printf("%d\n", fib_memo(0));
    // EXPECT: 0
    printf("%d\n", fib_memo(1));
    // EXPECT: 1
    printf("%d\n", fib_memo(5));
    // EXPECT: 5
    printf("%d\n", fib_memo(10));
    // EXPECT: 55
    printf("%d\n", fib_memo(15));
    // EXPECT: 610
    printf("%d\n", fib_memo(20));
    // EXPECT: 6765

    // Bottom-up matches memoized
    printf("%d\n", fib_bottom_up(10));
    // EXPECT: 55
    printf("%d\n", fib_bottom_up(20));
    // EXPECT: 6765
    printf("%d\n", fib_bottom_up(25));
    // EXPECT: 75025

    // Space-optimized matches
    printf("%d\n", fib_space_opt(10));
    // EXPECT: 55
    printf("%d\n", fib_space_opt(20));
    // EXPECT: 6765
    printf("%d\n", fib_space_opt(25));
    // EXPECT: 75025

    // Tribonacci: 0,0,1,1,2,4,7,13,24,44,81
    init_trib();
    printf("%d\n", tribonacci(0));
    // EXPECT: 0
    printf("%d\n", tribonacci(2));
    // EXPECT: 1
    printf("%d\n", tribonacci(4));
    // EXPECT: 2
    printf("%d\n", tribonacci(6));
    // EXPECT: 7
    printf("%d\n", tribonacci(10));
    // EXPECT: 81

    // Memoized fib uses far fewer calls than naive
    init_memo();
    call_count = 0;
    fib_counted(15);
    result = call_count;
    printf("%d\n", result);
    // EXPECT: 29

    // fib_is_even: fib(3)=2 even, fib(4)=3 odd, fib(6)=8 even
    printf("%d\n", fib_is_even(3));
    // EXPECT: 1
    printf("%d\n", fib_is_even(4));
    // EXPECT: 0
    printf("%d\n", fib_is_even(6));
    // EXPECT: 1

    // Sum of first n fibs: sum(0..5) = 0+1+1+2+3+5 = 12
    printf("%d\n", fib_sum(5));
    // EXPECT: 12
    // sum(0..10) = 0+1+1+2+3+5+8+13+21+34+55 = 143
    printf("%d\n", fib_sum(10));
    // EXPECT: 143

    printf("done\n");
    // EXPECT: done

    return 0;
}
