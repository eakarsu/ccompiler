int printf(const char *fmt, ...);

// Test persistent state across function calls using global variables
// (simulating static local behavior with globals).

int counter_state = 0;
int accumulator_state = 0;
int toggle_state = 0;
int call_count_a = 0;
int call_count_b = 0;
int fib_prev = 0;
int fib_curr = 1;
int min_state = 2147483647;
int max_state = -2147483647;

int counter(void) {
    counter_state = counter_state + 1;
    return counter_state;
}

int accumulate(int val) {
    accumulator_state = accumulator_state + val;
    return accumulator_state;
}

int toggle(void) {
    if (toggle_state == 0) {
        toggle_state = 1;
    } else {
        toggle_state = 0;
    }
    return toggle_state;
}

int count_calls_a(void) {
    call_count_a = call_count_a + 1;
    return call_count_a;
}

int count_calls_b(void) {
    call_count_b = call_count_b + 1;
    return call_count_b;
}

int next_fib(void) {
    int result = fib_curr;
    int tmp = fib_curr;
    fib_curr = fib_prev + fib_curr;
    fib_prev = tmp;
    return result;
}

void track_min(int val) {
    if (val < min_state) {
        min_state = val;
    }
}

void track_max(int val) {
    if (val > max_state) {
        max_state = val;
    }
}

int multiply_state = 1;

int multiply_accumulate(int val) {
    multiply_state = multiply_state * val;
    return multiply_state;
}

int last_val = 0;
int change_count = 0;

int count_changes(int val) {
    if (val != last_val) {
        change_count = change_count + 1;
    }
    last_val = val;
    return change_count;
}

int main(void) {
    // Counter increments each call
    // EXPECT: 1
    printf("%d\n", counter());
    // EXPECT: 2
    printf("%d\n", counter());
    // EXPECT: 3
    printf("%d\n", counter());

    // Accumulator sums values
    // EXPECT: 10
    printf("%d\n", accumulate(10));
    // EXPECT: 30
    printf("%d\n", accumulate(20));
    // EXPECT: 35
    printf("%d\n", accumulate(5));

    // Toggle alternates 0/1
    // EXPECT: 1
    printf("%d\n", toggle());
    // EXPECT: 0
    printf("%d\n", toggle());
    // EXPECT: 1
    printf("%d\n", toggle());

    // Independent counters for different functions
    // EXPECT: 1
    printf("%d\n", count_calls_a());
    // EXPECT: 1
    printf("%d\n", count_calls_b());
    // EXPECT: 2
    printf("%d\n", count_calls_a());
    // EXPECT: 3
    printf("%d\n", count_calls_a());
    // EXPECT: 2
    printf("%d\n", count_calls_b());

    // Fibonacci sequence: 1, 1, 2, 3, 5, 8
    // EXPECT: 1
    printf("%d\n", next_fib());
    // EXPECT: 1
    printf("%d\n", next_fib());
    // EXPECT: 2
    printf("%d\n", next_fib());
    // EXPECT: 3
    printf("%d\n", next_fib());
    // EXPECT: 5
    printf("%d\n", next_fib());
    // EXPECT: 8
    printf("%d\n", next_fib());

    // Min/max tracking
    track_min(50);
    track_min(20);
    track_min(30);
    track_min(10);
    track_min(40);
    // EXPECT: 10
    printf("%d\n", min_state);

    track_max(5);
    track_max(100);
    track_max(50);
    track_max(200);
    track_max(150);
    // EXPECT: 200
    printf("%d\n", max_state);

    // Multiply accumulate: 1*2=2, 2*3=6, 6*4=24
    // EXPECT: 2
    printf("%d\n", multiply_accumulate(2));
    // EXPECT: 6
    printf("%d\n", multiply_accumulate(3));
    // EXPECT: 24
    printf("%d\n", multiply_accumulate(4));

    // Count changes: track how many times value changes
    // val=5, last=0, different -> count=1
    // EXPECT: 1
    printf("%d\n", count_changes(5));
    // val=5, last=5, same -> count=1
    // EXPECT: 1
    printf("%d\n", count_changes(5));
    // val=10, last=5, different -> count=2
    // EXPECT: 2
    printf("%d\n", count_changes(10));
    // val=10, last=10, same -> count=2
    // EXPECT: 2
    printf("%d\n", count_changes(10));
    // val=3, last=10, different -> count=3
    // EXPECT: 3
    printf("%d\n", count_changes(3));

    return 0;
}
