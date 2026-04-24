int printf(const char *fmt, ...);

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

int female(int n);

int male(int n) {
    if (n == 0) return 0;
    return n - female(male(n - 1));
}

int female(int n) {
    if (n == 0) return 1;
    return n - male(female(n - 1));
}

int state_b(int n, int acc);

int state_a(int n, int acc) {
    if (n <= 0) return acc;
    return state_b(n - 1, acc + 1);
}

int state_b(int n, int acc) {
    if (n <= 0) return acc;
    return state_a(n - 1, acc + 2);
}

int count_down_b(int n);

int count_down_a(int n) {
    if (n <= 0) return 0;
    return 1 + count_down_b(n - 1);
}

int count_down_b(int n) {
    if (n <= 0) return 0;
    return 1 + count_down_a(n - 2);
}

int ping(int n);
int pong(int n);

int ping(int n) {
    if (n <= 0) return 0;
    return pong(n - 1) + 1;
}

int pong(int n) {
    if (n <= 0) return 0;
    return ping(n - 1) + 2;
}

int dec_b(int n);

int dec_a(int n) {
    if (n <= 1) return n;
    return dec_b(n - 1) + 1;
}

int dec_b(int n) {
    if (n <= 1) return n;
    return dec_a(n - 1) - 1;
}

int collatz_even(int n, int steps);
int collatz_odd(int n, int steps);

int collatz_even(int n, int steps) {
    if (n == 1) return steps;
    int next = n / 2;
    if (next % 2 == 0) return collatz_even(next, steps + 1);
    return collatz_odd(next, steps + 1);
}

int collatz_odd(int n, int steps) {
    if (n == 1) return steps;
    int next = 3 * n + 1;
    if (next % 2 == 0) return collatz_even(next, steps + 1);
    return collatz_odd(next, steps + 1);
}

int collatz_steps(int n) {
    if (n == 1) return 0;
    if (n % 2 == 0) return collatz_even(n, 0);
    return collatz_odd(n, 0);
}

int phase2(int n, int sum);

int phase1(int n, int sum) {
    if (n <= 0) return sum;
    return phase2(n - 1, sum + n);
}

int phase2(int n, int sum) {
    if (n <= 0) return sum;
    return phase1(n - 1, sum + n * 2);
}

int main(void) {
    // EXPECT: even(0)=1
    printf("even(0)=%d\n", is_even(0));
    // EXPECT: even(4)=1
    printf("even(4)=%d\n", is_even(4));
    // EXPECT: even(7)=0
    printf("even(7)=%d\n", is_even(7));
    // EXPECT: odd(5)=1
    printf("odd(5)=%d\n", is_odd(5));
    // EXPECT: odd(10)=0
    printf("odd(10)=%d\n", is_odd(10));
    // EXPECT: even(-6)=1
    printf("even(-6)=%d\n", is_even(-6));

    // EXPECT: male(0)=0
    printf("male(0)=%d\n", male(0));
    // EXPECT: female(0)=1
    printf("female(0)=%d\n", female(0));
    // EXPECT: male(5)=3
    printf("male(5)=%d\n", male(5));
    // EXPECT: female(5)=3
    printf("female(5)=%d\n", female(5));
    // EXPECT: male(10)=6
    printf("male(10)=%d\n", male(10));
    // EXPECT: female(10)=6
    printf("female(10)=%d\n", female(10));

    // EXPECT: stateA(10)=15
    printf("stateA(10)=%d\n", state_a(10, 0));
    // EXPECT: stateB(10)=15
    printf("stateB(10)=%d\n", state_b(10, 0));
    // EXPECT: stateA(1)=1
    printf("stateA(1)=%d\n", state_a(1, 0));

    // EXPECT: countA(10)=7
    printf("countA(10)=%d\n", count_down_a(10));
    // EXPECT: countB(10)=7
    printf("countB(10)=%d\n", count_down_b(10));

    // EXPECT: ping(10)=15
    printf("ping(10)=%d\n", ping(10));
    // EXPECT: pong(10)=15
    printf("pong(10)=%d\n", pong(10));

    // EXPECT: decA(10)=2
    printf("decA(10)=%d\n", dec_a(10));
    // EXPECT: decB(10)=0
    printf("decB(10)=%d\n", dec_b(10));

    // EXPECT: collatz(6)=8
    printf("collatz(6)=%d\n", collatz_steps(6));
    // EXPECT: collatz(27)=111
    printf("collatz(27)=%d\n", collatz_steps(27));
    // EXPECT: collatz(1)=0
    printf("collatz(1)=%d\n", collatz_steps(1));

    // EXPECT: phase1(10)=80
    printf("phase1(10)=%d\n", phase1(10, 0));
    // EXPECT: phase2(10)=85
    printf("phase2(10)=%d\n", phase2(10, 0));

    return 0;
}
