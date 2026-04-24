int printf(const char *fmt, ...);

int func_b(int n);

int func_a(int n) {
    if (n <= 0) return 0;
    return n + func_b(n - 1);
}

int func_b(int n) {
    if (n <= 0) return 0;
    return n + func_a(n - 1);
}

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

int ping(int n);
int pong(int n);

int ping(int n) {
    if (n <= 0) return 0;
    return 1 + pong(n - 1);
}

int pong(int n) {
    if (n <= 0) return 0;
    return 1 + ping(n - 1);
}

int count_down_a(int n);
int count_down_b(int n);

int count_down_a(int n) {
    if (n <= 0) return 0;
    return count_down_b(n - 1);
}

int count_down_b(int n) {
    if (n <= 0) return 1;
    return count_down_a(n - 1);
}

int grow_a(int n, int acc);
int grow_b(int n, int acc);

int grow_a(int n, int acc) {
    if (n <= 0) return acc;
    return grow_b(n - 1, acc + n);
}

int grow_b(int n, int acc) {
    if (n <= 0) return acc;
    return grow_a(n - 1, acc + n * 2);
}

int step_x(int n);
int step_y(int n);
int step_z(int n);

int step_x(int n) {
    if (n <= 0) return 1;
    return step_y(n - 1) + 1;
}

int step_y(int n) {
    if (n <= 0) return 2;
    return step_z(n - 1) + 2;
}

int step_z(int n) {
    if (n <= 0) return 3;
    return step_x(n - 1) + 3;
}

int hofstadter_f(int n);
int hofstadter_m(int n);

int hofstadter_f(int n) {
    if (n == 0) return 1;
    return n - hofstadter_m(hofstadter_f(n - 1));
}

int hofstadter_m(int n) {
    if (n == 0) return 0;
    return n - hofstadter_f(hofstadter_m(n - 1));
}

int main(void) {
    // EXPECT: mutual_a10: 55
    printf("mutual_a10: %d\n", func_a(10));

    // EXPECT: mutual_b10: 55
    printf("mutual_b10: %d\n", func_b(10));

    // EXPECT: mutual_a5: 15
    printf("mutual_a5: %d\n", func_a(5));

    // EXPECT: even4: 1
    printf("even4: %d\n", is_even(4));

    // EXPECT: even5: 0
    printf("even5: %d\n", is_even(5));

    // EXPECT: odd3: 1
    printf("odd3: %d\n", is_odd(3));

    // EXPECT: odd6: 0
    printf("odd6: %d\n", is_odd(6));

    // EXPECT: even0: 1
    printf("even0: %d\n", is_even(0));

    // EXPECT: ping10: 10
    printf("ping10: %d\n", ping(10));

    // EXPECT: pong10: 10
    printf("pong10: %d\n", pong(10));

    // EXPECT: countdown5: 1
    printf("countdown5: %d\n", count_down_a(5));

    // EXPECT: countdown4: 0
    printf("countdown4: %d\n", count_down_a(4));

    // EXPECT: grow10: 80
    printf("grow10: %d\n", grow_a(10, 0));

    // EXPECT: grow5: 21
    printf("grow5: %d\n", grow_a(5, 0));

    // EXPECT: step3x: 7
    printf("step3x: %d\n", step_x(3));

    // EXPECT: step3y: 8
    printf("step3y: %d\n", step_y(3));

    // EXPECT: step3z: 9
    printf("step3z: %d\n", step_z(3));

    // EXPECT: hof_f5: 3
    printf("hof_f5: %d\n", hofstadter_f(5));

    // EXPECT: hof_m5: 3
    printf("hof_m5: %d\n", hofstadter_m(5));

    return 0;
}
