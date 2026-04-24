// stress_084_coroutine_sim.c - Coroutine simulation using static locals and state enums
// EXPECT: fib_0=0
// EXPECT: fib_1=1
// EXPECT: fib_2=1
// EXPECT: fib_3=2
// EXPECT: fib_4=3
// EXPECT: fib_5=5
// EXPECT: fib_6=8
// EXPECT: fib_7=13
// EXPECT: fib_8=21
// EXPECT: fib_9=34
// EXPECT: prime_0=2
// EXPECT: prime_1=3
// EXPECT: prime_2=5
// EXPECT: prime_3=7
// EXPECT: prime_4=11
// EXPECT: prime_5=13
// EXPECT: prime_6=17
// EXPECT: prime_7=19
// EXPECT: prime_8=23
// EXPECT: prime_9=29
// EXPECT: interleave=0 2 1 3 1 5 2 7 3 11
// EXPECT: counter_0=0
// EXPECT: counter_1=3
// EXPECT: counter_2=6
// EXPECT: counter_3=9
// EXPECT: range_sum=45
// EXPECT: powers_0=1
// EXPECT: powers_1=2
// EXPECT: powers_2=4
// EXPECT: powers_3=8
// EXPECT: powers_4=16
// EXPECT: collatz_start=6
// EXPECT: collatz_done=1
// EXPECT: pass_done=1

int printf(const char *fmt, ...);

// --- Fibonacci generator ---
// Uses a struct to hold coroutine state
struct FibGen {
    int state;
    int a;
    int b;
    int current;
};

void fib_init(struct FibGen *g) {
    g->state = 0;
    g->a = 0;
    g->b = 1;
    g->current = 0;
}

int fib_next(struct FibGen *g) {
    if (g->state == 0) {
        g->current = 0;
        g->state = 1;
        return g->current;
    }
    if (g->state == 1) {
        g->current = 1;
        g->state = 2;
        return g->current;
    }
    // state >= 2
    int next = g->a + g->b;
    g->a = g->b;
    g->b = next;
    g->current = next;
    return g->current;
}

// --- Prime generator ---
struct PrimeGen {
    int candidate;
    int count;
};

void prime_init(struct PrimeGen *g) {
    g->candidate = 2;
    g->count = 0;
}

int is_prime(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    int i;
    for (i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

int prime_next(struct PrimeGen *g) {
    while (1) {
        int c = g->candidate;
        g->candidate++;
        if (is_prime(c)) {
            g->count++;
            return c;
        }
    }
}

// --- Counter generator (counts by step) ---
struct CounterGen {
    int current;
    int step;
};

void counter_init(struct CounterGen *g, int start, int step) {
    g->current = start;
    g->step = step;
}

int counter_next(struct CounterGen *g) {
    int val = g->current;
    g->current += g->step;
    return val;
}

// --- Range generator ---
struct RangeGen {
    int current;
    int end;
    int done;
};

void range_init(struct RangeGen *g, int start, int end) {
    g->current = start;
    g->end = end;
    g->done = 0;
}

int range_has_next(struct RangeGen *g) {
    return g->current < g->end;
}

int range_next(struct RangeGen *g) {
    if (g->current >= g->end) {
        g->done = 1;
        return -1;
    }
    int val = g->current;
    g->current++;
    return val;
}

// --- Powers of 2 generator ---
struct PowerGen {
    int current;
};

void power_init(struct PowerGen *g) {
    g->current = 1;
}

int power_next(struct PowerGen *g) {
    int val = g->current;
    g->current = g->current * 2;
    return val;
}

// --- Collatz sequence generator ---
struct CollatzGen {
    int current;
    int done;
};

void collatz_init(struct CollatzGen *g, int start) {
    g->current = start;
    g->done = 0;
}

int collatz_next(struct CollatzGen *g) {
    int val = g->current;
    if (val == 1) {
        g->done = 1;
        return 1;
    }
    if (val % 2 == 0) {
        g->current = val / 2;
    } else {
        g->current = val * 3 + 1;
    }
    return val;
}

int main(void) {
    int i;

    // Fibonacci generator
    struct FibGen fib;
    fib_init(&fib);
    for (i = 0; i < 10; i++) {
        int val = fib_next(&fib);
        printf("fib_%d=%d\n", i, val);
    }

    // Prime generator
    struct PrimeGen primes;
    prime_init(&primes);
    for (i = 0; i < 10; i++) {
        int val = prime_next(&primes);
        printf("prime_%d=%d\n", i, val);
    }

    // Interleave fibonacci and prime generators
    struct FibGen fib2;
    struct PrimeGen primes2;
    fib_init(&fib2);
    prime_init(&primes2);

    printf("interleave=");
    for (i = 0; i < 5; i++) {
        if (i > 0) printf(" ");
        printf("%d %d", fib_next(&fib2), prime_next(&primes2));
    }
    printf("\n");

    // Counter generator (step=3)
    struct CounterGen ctr;
    counter_init(&ctr, 0, 3);
    for (i = 0; i < 4; i++) {
        printf("counter_%d=%d\n", i, counter_next(&ctr));
    }

    // Range generator: sum 0..9
    struct RangeGen rng;
    range_init(&rng, 0, 10);
    int sum = 0;
    while (range_has_next(&rng)) {
        sum += range_next(&rng);
    }
    printf("range_sum=%d\n", sum);

    // Powers of 2
    struct PowerGen pow;
    power_init(&pow);
    for (i = 0; i < 5; i++) {
        printf("powers_%d=%d\n", i, power_next(&pow));
    }

    // Collatz sequence from 6: 6, 3, 10, 5, 16, 8, 4, 2, 1
    struct CollatzGen col;
    collatz_init(&col, 6);
    printf("collatz_start=%d\n", collatz_next(&col));
    while (!col.done) {
        collatz_next(&col);
    }
    printf("collatz_done=%d\n", col.done);

    printf("pass_done=1\n");
    return 0;
}
