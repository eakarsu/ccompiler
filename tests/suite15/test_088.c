int printf(const char *fmt, ...);

/* Genetic algorithm: evolve a bit-string to match a target
 * Each individual is an 8-bit integer (0-255).
 * Fitness = number of matching bits with target.
 * Uses tournament selection, single-point crossover, bit-flip mutation.
 */

int lcg_state;

void seed_rng(int s) {
    lcg_state = s;
}

int rand_int(void) {
    lcg_state = lcg_state * 1103515245 + 12345;
    int r;
    r = (lcg_state / 65536) % 32768;
    if (r < 0) r = -r;
    return r;
}

/* Count matching bits between a and b (8 bits) */
int count_matching_bits(int a, int b) {
    int xor_val;
    int count;
    int i;
    xor_val = a ^ b;
    /* Count zero bits in lower 8 = 8 - popcount of xor */
    count = 0;
    i = 0;
    while (i < 8) {
        if ((xor_val & (1 << i)) == 0) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

/* Fitness: number of matching bits with target */
int fitness(int individual, int target) {
    return count_matching_bits(individual, target);
}

/* Tournament selection: pick 2 random, return fitter one */
int tournament(int *pop, int pop_size, int target) {
    int i1;
    int i2;
    int f1;
    int f2;
    i1 = rand_int() % pop_size;
    i2 = rand_int() % pop_size;
    f1 = fitness(pop[i1], target);
    f2 = fitness(pop[i2], target);
    if (f1 >= f2) return pop[i1];
    return pop[i2];
}

/* Single-point crossover at a random bit position */
int crossover(int p1, int p2) {
    int point;
    int mask;
    point = rand_int() % 7 + 1;  /* 1 to 7 */
    mask = (1 << point) - 1;     /* lower bits mask */
    return (p1 & mask) | (p2 & (~mask & 255));
}

/* Bit-flip mutation with given probability (out of 100) */
int mutate(int individual, int rate) {
    int i;
    int result;
    result = individual;
    i = 0;
    while (i < 8) {
        if (rand_int() % 100 < rate) {
            result = result ^ (1 << i);
        }
        i = i + 1;
    }
    return result & 255;
}

/* Find best fitness in population */
int best_fitness(int *pop, int pop_size, int target) {
    int best;
    int i;
    int f;
    best = 0;
    i = 0;
    while (i < pop_size) {
        f = fitness(pop[i], target);
        if (f > best) best = f;
        i = i + 1;
    }
    return best;
}

/* Find best individual */
int best_individual(int *pop, int pop_size, int target) {
    int best;
    int best_f;
    int i;
    int f;
    best = pop[0];
    best_f = fitness(pop[0], target);
    i = 1;
    while (i < pop_size) {
        f = fitness(pop[i], target);
        if (f > best_f) {
            best_f = f;
            best = pop[i];
        }
        i = i + 1;
    }
    return best;
}

/* Average fitness * 100 */
int avg_fitness(int *pop, int pop_size, int target) {
    int sum;
    int i;
    sum = 0;
    i = 0;
    while (i < pop_size) {
        sum = sum + fitness(pop[i], target);
        i = i + 1;
    }
    return sum * 100 / pop_size;
}

int main(void) {
    int pop[16];
    int new_pop[16];
    int target;
    int gen;
    int i;
    int p1;
    int p2;
    int child;
    int bf;

    printf("Genetic Algorithm Tests\n");
    // EXPECT: Genetic Algorithm Tests

    /* Test 1: Evolve towards target 0b10101010 = 170 */
    target = 170;
    seed_rng(42);

    /* Initialize population with LCG */
    i = 0;
    while (i < 16) {
        pop[i] = rand_int() % 256;
        i = i + 1;
    }

    printf("Init best fit: %d\n", best_fitness(pop, 16, target));
    // EXPECT: Init best fit: 7

    /* Run 50 generations */
    gen = 0;
    while (gen < 50) {
        i = 0;
        while (i < 16) {
            p1 = tournament(pop, 16, target);
            p2 = tournament(pop, 16, target);
            child = crossover(p1, p2);
            child = mutate(child, 5);
            new_pop[i] = child;
            i = i + 1;
        }
        /* Copy new_pop to pop */
        i = 0;
        while (i < 16) {
            pop[i] = new_pop[i];
            i = i + 1;
        }
        gen = gen + 1;
    }

    bf = best_fitness(pop, 16, target);
    printf("Gen50 best fit: %d\n", bf);
    // EXPECT: Gen50 best fit: 8

    printf("Gen50 best ind: %d\n", best_individual(pop, 16, target));
    // EXPECT: Gen50 best ind: 170

    /* Test 2: Different target = 0b11001100 = 204 */
    target = 204;
    seed_rng(999);

    i = 0;
    while (i < 16) {
        pop[i] = rand_int() % 256;
        i = i + 1;
    }

    gen = 0;
    while (gen < 80) {
        i = 0;
        while (i < 16) {
            p1 = tournament(pop, 16, target);
            p2 = tournament(pop, 16, target);
            child = crossover(p1, p2);
            child = mutate(child, 3);
            new_pop[i] = child;
            i = i + 1;
        }
        i = 0;
        while (i < 16) {
            pop[i] = new_pop[i];
            i = i + 1;
        }
        gen = gen + 1;
    }

    bf = best_fitness(pop, 16, target);
    printf("T2 best fit: %d\n", bf);
    // EXPECT: T2 best fit: 8

    printf("T2 best ind: %d\n", best_individual(pop, 16, target));
    // EXPECT: T2 best ind: 204

    /* Test 3: Bit counting tests */
    printf("Match 0,0: %d\n", count_matching_bits(0, 0));
    // EXPECT: Match 0,0: 8
    printf("Match 255,0: %d\n", count_matching_bits(255, 0));
    // EXPECT: Match 255,0: 0
    printf("Match 170,85: %d\n", count_matching_bits(170, 85));
    // EXPECT: Match 170,85: 0
    printf("Match 170,170: %d\n", count_matching_bits(170, 170));
    // EXPECT: Match 170,170: 8
    printf("Match 15,240: %d\n", count_matching_bits(15, 240));
    // EXPECT: Match 15,240: 0

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
