int printf(const char *fmt, ...);

// Deeply recursive mutual recursion (even/odd)

int is_even(int n);
int is_odd(int n);

int is_even(int n) {
    if (n < 0) n = -n;
    if (n == 0) return 1;
    return is_odd(n - 1);
}

int is_odd(int n) {
    if (n < 0) n = -n;
    if (n == 0) return 0;
    return is_even(n - 1);
}

// Mutual recursion: count_down_a and count_down_b alternate
int count_down_a(int n, int acc);
int count_down_b(int n, int acc);

int count_down_a(int n, int acc) {
    if (n <= 0) return acc;
    return count_down_b(n - 1, acc + n);
}

int count_down_b(int n, int acc) {
    if (n <= 0) return acc;
    return count_down_a(n - 1, acc + n * 2);
}

// Three-way mutual recursion
int func_a(int n);
int func_b(int n);
int func_c(int n);

int func_a(int n) {
    if (n <= 0) return 1;
    return func_b(n - 1) + 1;
}

int func_b(int n) {
    if (n <= 0) return 2;
    return func_c(n - 1) + 2;
}

int func_c(int n) {
    if (n <= 0) return 3;
    return func_a(n - 1) + 3;
}

// Fibonacci-like mutual recursion
int fib_f(int n);
int fib_g(int n);

int fib_f(int n) {
    if (n <= 1) return 1;
    return fib_f(n - 1) + fib_g(n - 2);
}

int fib_g(int n) {
    if (n <= 1) return 1;
    return fib_g(n - 1) + fib_f(n - 2);
}

// Collatz-like mutual recursion
int collatz_even(int n, int steps);
int collatz_odd(int n, int steps);

int collatz_even(int n, int steps) {
    if (n == 1) return steps;
    if (n % 2 == 0) return collatz_even(n / 2, steps + 1);
    return collatz_odd(n, steps);
}

int collatz_odd(int n, int steps) {
    if (n == 1) return steps;
    if (n % 2 == 1) return collatz_even(3 * n + 1, steps + 1);
    return collatz_even(n, steps);
}

// Ackermann-like (bounded)
int ack_a(int m, int n);
int ack_b(int m, int n);

int ack_a(int m, int n) {
    if (m == 0) return n + 1;
    if (n == 0) return ack_b(m - 1, 1);
    return ack_b(m - 1, ack_a(m, n - 1));
}

int ack_b(int m, int n) {
    if (m == 0) return n + 2;
    if (n == 0) return ack_a(m - 1, 1);
    return ack_a(m - 1, ack_b(m, n - 1));
}

int main() {
    // Even/odd tests
    printf("%d\n", is_even(0));   // EXPECT: 1
    printf("%d\n", is_even(1));   // EXPECT: 0
    printf("%d\n", is_even(2));   // EXPECT: 1
    printf("%d\n", is_even(99));  // EXPECT: 0
    printf("%d\n", is_even(100)); // EXPECT: 1
    printf("%d\n", is_odd(0));    // EXPECT: 0
    printf("%d\n", is_odd(1));    // EXPECT: 1
    printf("%d\n", is_odd(50));   // EXPECT: 0
    printf("%d\n", is_odd(51));   // EXPECT: 1

    // Count-down alternating accumulation
    // n=6: a(6,0)->b(5,6)->a(4,16)->b(3,20)->a(2,26)->b(1,28)->a(0,30)=30
    printf("%d\n", count_down_a(6, 0)); // EXPECT: 30

    // n=1: a(1,0)->b(0,1)=1
    printf("%d\n", count_down_a(1, 0)); // EXPECT: 1

    // n=4: a(4,0)->b(3,4)->a(2,10)->b(1,12)->a(0,14)=14
    printf("%d\n", count_down_a(4, 0)); // EXPECT: 14

    // Three-way mutual recursion
    // a(3)->b(2)+1->c(1)+2+1->a(0)+3+2+1 = 1+3+2+1 = 7
    printf("%d\n", func_a(3)); // EXPECT: 7

    // a(6)->b(5)+1->c(4)+3->a(3)+6->b(2)+7->c(1)+9->a(0)+12 = 1+12 = 13
    printf("%d\n", func_a(6)); // EXPECT: 13

    // b(0) = 2
    printf("%d\n", func_b(0)); // EXPECT: 2

    // c(0) = 3
    printf("%d\n", func_c(0)); // EXPECT: 3

    // b(3)->c(2)+2->a(1)+3+2->b(0)+1+3+2 = 2+1+3+2 = 8
    printf("%d\n", func_b(3)); // EXPECT: 8

    // Fibonacci-like mutual recursion
    printf("%d\n", fib_f(0)); // EXPECT: 1
    printf("%d\n", fib_f(1)); // EXPECT: 1
    printf("%d\n", fib_f(5)); // EXPECT: 8
    printf("%d\n", fib_g(5)); // EXPECT: 8

    // Collatz steps
    // collatz(1) = 0 steps
    printf("%d\n", collatz_even(1, 0)); // EXPECT: 0

    // collatz(2): 2->1, 1 step
    printf("%d\n", collatz_even(2, 0)); // EXPECT: 1

    // collatz(6): 6->3->10->5->16->8->4->2->1, 8 steps
    printf("%d\n", collatz_even(6, 0)); // EXPECT: 8

    // collatz(7): 7->22->11->34->17->52->26->13->40->20->10->5->16->8->4->2->1, 16 steps
    printf("%d\n", collatz_odd(7, 0)); // EXPECT: 16

    // Ackermann-like
    printf("%d\n", ack_a(0, 0)); // EXPECT: 1
    printf("%d\n", ack_a(0, 5)); // EXPECT: 6
    printf("%d\n", ack_b(0, 0)); // EXPECT: 2
    printf("%d\n", ack_b(0, 5)); // EXPECT: 7

    printf("%s\n", "done"); // EXPECT: done
    return 0;
}
