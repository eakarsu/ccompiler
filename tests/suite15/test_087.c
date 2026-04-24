int printf(const char *fmt, ...);

/* Simulated annealing using LCG-based random
 * Minimize f(x) = (x - 42)^2 + (x/10 - 3)^2 * 5
 * using integer arithmetic.
 */

typedef struct {
    int state;  /* LCG state */
} RNG;

void rng_init(RNG *rng, int seed) {
    rng->state = seed;
}

/* LCG random: returns 0 to 32767 */
int rng_next(RNG *rng) {
    rng->state = rng->state * 1103515245 + 12345;
    /* Take bits 16..30 */
    int r;
    r = (rng->state / 65536) % 32768;
    if (r < 0) r = -r;
    return r;
}

/* Random int in range [-range, range] */
int rng_range(RNG *rng, int range) {
    int r;
    r = rng_next(rng);
    return r % (2 * range + 1) - range;
}

/* Cost function: f(x) = (x-42)^2 + 5*((x/10)-3)^2 */
int cost(int x) {
    int a;
    int b;
    a = x - 42;
    b = x / 10 - 3;
    return a * a + 5 * b * b;
}

/* Simulated annealing
 * temp_init: initial temperature * 100
 * cooling: multiply temp by cooling/100 each step
 * Returns best x found
 */
int simulated_annealing(int x0, int temp_init, int cooling, int steps, RNG *rng) {
    int x;
    int best_x;
    int best_cost;
    int current_cost;
    int new_x;
    int new_cost;
    int delta;
    int temp;
    int i;
    int accept;
    int rand_val;

    x = x0;
    current_cost = cost(x);
    best_x = x;
    best_cost = current_cost;
    temp = temp_init;

    i = 0;
    while (i < steps) {
        /* Generate neighbor */
        new_x = x + rng_range(rng, 5);

        new_cost = cost(new_x);
        delta = new_cost - current_cost;

        accept = 0;
        if (delta < 0) {
            accept = 1;  /* Always accept improvements */
        } else if (temp > 0) {
            /* Accept worse solution with probability based on temp
             * Since we can't do exp(), use: accept if rand < temp/delta
             * Simplified: accept if random(0..100) < temp*10/delta
             */
            rand_val = rng_next(rng) % 100;
            if (delta > 0 && rand_val < temp * 10 / (delta + 1)) {
                accept = 1;
            }
        }

        if (accept) {
            x = new_x;
            current_cost = new_cost;
        }

        if (current_cost < best_cost) {
            best_cost = current_cost;
            best_x = x;
        }

        /* Cool down */
        temp = temp * cooling / 100;

        i = i + 1;
    }

    return best_x;
}

int main(void) {
    RNG rng;
    int result;
    int c;
    int results[5];
    int i;
    int best;
    int best_c;

    printf("Simulated Annealing Tests\n");
    // EXPECT: Simulated Annealing Tests

    /* Test 1: Basic optimization */
    rng_init(&rng, 12345);
    result = simulated_annealing(0, 1000, 99, 500, &rng);
    c = cost(result);
    printf("T1 result: %d\n", result);
    // EXPECT: T1 result: 42
    printf("T1 cost: %d\n", c);
    // EXPECT: T1 cost: 5

    /* Test 2: Starting far away */
    rng_init(&rng, 54321);
    result = simulated_annealing(200, 2000, 99, 1000, &rng);
    c = cost(result);
    printf("T2 result: %d\n", result);
    // EXPECT: T2 result: 42
    printf("T2 cost: %d\n", c);
    // EXPECT: T2 cost: 5

    /* Test 3: Starting at optimum */
    rng_init(&rng, 11111);
    result = simulated_annealing(42, 500, 98, 200, &rng);
    c = cost(result);
    printf("T3 result: %d\n", result);
    // EXPECT: T3 result: 42
    printf("T3 cost: %d\n", c);
    // EXPECT: T3 cost: 5


    /* Test 4: Multiple runs, take best */
    best = 0;
    best_c = 999999;
    i = 0;
    while (i < 5) {
        rng_init(&rng, 1000 + i * 7777);
        results[i] = simulated_annealing(-50, 1500, 99, 800, &rng);
        c = cost(results[i]);
        if (c < best_c) {
            best_c = c;
            best = results[i];
        }
        i = i + 1;
    }

    printf("Multi best: %d\n", best);
    // EXPECT: Multi best: 42
    printf("Multi best cost: %d\n", best_c);
    // EXPECT: Multi best cost: 5

    /* Test 5: Very low temperature (greedy) */
    rng_init(&rng, 99999);
    result = simulated_annealing(30, 10, 90, 300, &rng);
    c = cost(result);
    printf("T5 greedy result: %d\n", result);
    // EXPECT: T5 greedy result: 42
    printf("T5 greedy cost: %d\n", c);
    // EXPECT: T5 greedy cost: 5

    /* Test 6: Cost at known points */
    printf("Cost at 0: %d\n", cost(0));
    // EXPECT: Cost at 0: 1809
    printf("Cost at 42: %d\n", cost(42));
    // EXPECT: Cost at 42: 5
    printf("Cost at 100: %d\n", cost(100));
    // EXPECT: Cost at 100: 3609

    /* Test 7: Negative start */
    rng_init(&rng, 77777);
    result = simulated_annealing(-100, 2000, 99, 1000, &rng);
    printf("T7 neg start: %d\n", result);
    // EXPECT: T7 neg start: 42

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
