int printf(const char *fmt, ...);

// Genetic Algorithm with integer fitness
// Evolving a sequence of integers to match a target

int target[8];
int pop[20][8];    // population of 20 individuals, each 8 genes
int fitness[20];
int new_pop[20][8];

// Simple pseudo-random number generator
int rng_state;

int rand_next() {
    rng_state = rng_state * 1103515245 + 12345;
    int val = rng_state / 65536;
    if (val < 0) val = -val;
    return val;
}

int rand_range(int lo, int hi) {
    int range = hi - lo + 1;
    int val = rand_next() % range;
    if (val < 0) val = -val;
    return lo + val;
}

int compute_fitness(int idx) {
    int fit = 0;
    int i = 0;
    while (i < 8) {
        int diff = pop[idx][i] - target[i];
        if (diff < 0) diff = -diff;
        fit = fit + diff;
        i = i + 1;
    }
    return fit;
}

void evaluate_all() {
    int i = 0;
    while (i < 20) {
        fitness[i] = compute_fitness(i);
        i = i + 1;
    }
}

int find_best() {
    int best = 0;
    int i = 1;
    while (i < 20) {
        if (fitness[i] < fitness[best]) {
            best = i;
        }
        i = i + 1;
    }
    return best;
}

// Tournament selection: pick 2 random, return better one
int tournament_select() {
    int a = rand_range(0, 19);
    int b = rand_range(0, 19);
    if (fitness[a] <= fitness[b]) return a;
    return b;
}

// Single-point crossover
void crossover(int p1, int p2, int child_idx) {
    int cross_pt = rand_range(1, 6);
    int i = 0;
    while (i < 8) {
        if (i < cross_pt) {
            new_pop[child_idx][i] = pop[p1][i];
        } else {
            new_pop[child_idx][i] = pop[p2][i];
        }
        i = i + 1;
    }
}

// Mutation: randomly change one gene
void mutate(int child_idx, int rate) {
    int r = rand_range(0, 99);
    if (r < rate) {
        int gene = rand_range(0, 7);
        new_pop[child_idx][gene] = rand_range(0, 9);
    }
}

void copy_new_to_pop() {
    int i = 0;
    while (i < 20) {
        int j = 0;
        while (j < 8) {
            pop[i][j] = new_pop[i][j];
            j = j + 1;
        }
        i = i + 1;
    }
}

int main() {
    rng_state = 42;

    // Target sequence
    target[0] = 3; target[1] = 1; target[2] = 4; target[3] = 1;
    target[4] = 5; target[5] = 9; target[6] = 2; target[7] = 6;

    printf("Target: %d %d %d %d %d %d %d %d\n",
           target[0], target[1], target[2], target[3],
           target[4], target[5], target[6], target[7]);
    // EXPECT: Target: 3 1 4 1 5 9 2 6

    // Initialize population randomly
    int i = 0;
    while (i < 20) {
        int j = 0;
        while (j < 8) {
            pop[i][j] = rand_range(0, 9);
            j = j + 1;
        }
        i = i + 1;
    }

    evaluate_all();
    int best = find_best();
    printf("Gen 0 best fitness: %d\n", fitness[best]);
    // EXPECT: Gen 0 best fitness: 10

    // Run genetic algorithm for 100 generations
    int gen = 0;
    while (gen < 100) {
        // Elitism: copy best individual
        evaluate_all();
        best = find_best();
        int j = 0;
        while (j < 8) {
            new_pop[0][j] = pop[best][j];
            j = j + 1;
        }

        // Create rest via crossover and mutation
        int c = 1;
        while (c < 20) {
            int p1 = tournament_select();
            int p2 = tournament_select();
            crossover(p1, p2, c);
            mutate(c, 30);
            c = c + 1;
        }

        copy_new_to_pop();
        gen = gen + 1;
    }

    evaluate_all();
    best = find_best();
    printf("Gen 100 best fitness: %d\n", fitness[best]);
    // EXPECT: Gen 100 best fitness: 0

    printf("Best: %d %d %d %d %d %d %d %d\n",
           pop[best][0], pop[best][1], pop[best][2], pop[best][3],
           pop[best][4], pop[best][5], pop[best][6], pop[best][7]);
    // EXPECT: Best: 3 1 4 1 5 9 2 6

    int match = 1;
    i = 0;
    while (i < 8) {
        if (pop[best][i] != target[i]) match = 0;
        i = i + 1;
    }
    if (match) {
        printf("Perfect match found\n");
    } else {
        printf("No perfect match\n");
    }
    // EXPECT: Perfect match found

    printf("GA done\n");
    // EXPECT: GA done

    return 0;
}
