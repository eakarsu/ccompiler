// stress_093_mutual_recursion.c - Mutual recursion torture test
// Exercises: 4-way mutual recursion cycle, is_even/is_odd, mutual recursive
// computation, deep mutual recursion.
// EXPECT: cycle4(0,20)=25
// EXPECT: cycle4(1,20)=25
// EXPECT: cycle4(2,20)=25
// EXPECT: cycle4(3,20)=25
// EXPECT: even(0)=1
// EXPECT: even(1)=0
// EXPECT: even(17)=0
// EXPECT: even(100)=1
// EXPECT: odd(0)=0
// EXPECT: odd(1)=1
// EXPECT: odd(42)=0
// EXPECT: odd(99)=1
// EXPECT: hofstadter_f(10)=6
// EXPECT: hofstadter_m(10)=6
// EXPECT: ping(50)=50
// EXPECT: collatz_a(7)=1
// EXPECT: collatz_a(27)=1
// EXPECT: all_pass=1

int printf(const char *fmt, ...);

// Forward declarations for 4-way cycle
int func_a(int x, int depth);
int func_b(int x, int depth);
int func_c(int x, int depth);
int func_d(int x, int depth);

// A -> B -> C -> D -> A cycle, base case at depth 20
int func_a(int x, int depth) {
    if (depth <= 0) return x;
    return func_b(x + 1, depth - 1);
}

int func_b(int x, int depth) {
    if (depth <= 0) return x;
    return func_c(x + 2, depth - 1);
}

int func_c(int x, int depth) {
    if (depth <= 0) return x;
    return func_d(x + 3, depth - 1);
}

int func_d(int x, int depth) {
    if (depth <= 0) return x;
    return func_a(x - 1, depth - 1);
}

int cycle4(int start_func, int depth) {
    switch (start_func) {
        case 0: return func_a(0, depth);
        case 1: return func_b(0, depth);
        case 2: return func_c(0, depth);
        case 3: return func_d(0, depth);
        default: return -1;
    }
}

// Classic is_even / is_odd mutual recursion
int is_odd(int n);

int is_even(int n) {
    if (n == 0) return 1;
    if (n < 0) return is_even(-n);
    return is_odd(n - 1);
}

int is_odd(int n) {
    if (n == 0) return 0;
    if (n < 0) return is_odd(-n);
    return is_even(n - 1);
}

// Hofstadter Female and Male sequences
// F(n) = n - M(F(n-1)), F(0) = 1
// M(n) = n - F(M(n-1)), M(0) = 0
int hofstadter_m(int n);

int hofstadter_f(int n) {
    if (n == 0) return 1;
    return n - hofstadter_m(hofstadter_f(n - 1));
}

int hofstadter_m(int n) {
    if (n == 0) return 0;
    return n - hofstadter_f(hofstadter_m(n - 1));
}

// Ping-pong: mutual recursion counting up
int pong(int n, int target);

int ping(int n, int target) {
    if (n >= target) return n;
    return pong(n + 1, target);
}

int pong(int n, int target) {
    if (n >= target) return n;
    return ping(n + 1, target);
}

// Mutual recursive Collatz-like:
// collatz_a handles even, collatz_b handles odd
int collatz_b(int n, int steps);

int collatz_a(int n, int steps) {
    if (n == 1) return 1;
    if (n % 2 == 0) {
        return collatz_a(n / 2, steps + 1);
    } else {
        return collatz_b(n, steps);
    }
}

int collatz_b(int n, int steps) {
    // n is odd
    int next = 3 * n + 1;
    return collatz_a(next, steps + 1);
}

int main(void) {
    int pass = 1;

    // 4-way cycle tests
    printf("cycle4(0,20)=%d\n", cycle4(0, 20));
    if (cycle4(0, 20) != 25) pass = 0;
    printf("cycle4(1,20)=%d\n", cycle4(1, 20));
    if (cycle4(1, 20) != 25) pass = 0;
    printf("cycle4(2,20)=%d\n", cycle4(2, 20));
    if (cycle4(2, 20) != 25) pass = 0;
    printf("cycle4(3,20)=%d\n", cycle4(3, 20));
    if (cycle4(3, 20) != 25) pass = 0;

    // is_even / is_odd
    printf("even(0)=%d\n", is_even(0));
    if (is_even(0) != 1) pass = 0;
    printf("even(1)=%d\n", is_even(1));
    if (is_even(1) != 0) pass = 0;
    printf("even(17)=%d\n", is_even(17));
    if (is_even(17) != 0) pass = 0;
    printf("even(100)=%d\n", is_even(100));
    if (is_even(100) != 1) pass = 0;
    printf("odd(0)=%d\n", is_odd(0));
    if (is_odd(0) != 0) pass = 0;
    printf("odd(1)=%d\n", is_odd(1));
    if (is_odd(1) != 1) pass = 0;
    printf("odd(42)=%d\n", is_odd(42));
    if (is_odd(42) != 0) pass = 0;
    printf("odd(99)=%d\n", is_odd(99));
    if (is_odd(99) != 1) pass = 0;

    // Hofstadter
    printf("hofstadter_f(10)=%d\n", hofstadter_f(10));
    if (hofstadter_f(10) != 6) pass = 0;
    printf("hofstadter_m(10)=%d\n", hofstadter_m(10));
    if (hofstadter_m(10) != 6) pass = 0;

    // Ping-pong
    printf("ping(50)=%d\n", ping(0, 50));
    if (ping(0, 50) != 50) pass = 0;

    // Collatz mutual recursion
    printf("collatz_a(7)=%d\n", collatz_a(7, 0));
    if (collatz_a(7, 0) != 1) pass = 0;
    printf("collatz_a(27)=%d\n", collatz_a(27, 0));
    if (collatz_a(27, 0) != 1) pass = 0;

    printf("all_pass=%d\n", pass);
    return 0;
}
