int printf(const char *fmt, ...);

// Simulated Annealing with integer energy function
// Optimize a permutation to minimize total cost

int perm[8];
int best_perm[8];
int cost_matrix[8][8];

// Pseudo-random number generator
int sa_rng;

int sa_rand() {
    sa_rng = sa_rng * 1103515245 + 12345;
    int val = sa_rng / 65536;
    if (val < 0) val = -val;
    return val;
}

int sa_rand_range(int lo, int hi) {
    int range = hi - lo + 1;
    int val = sa_rand() % range;
    if (val < 0) val = -val;
    return lo + val;
}

// Energy: sum of cost_matrix[i][perm[i]] for each position
int compute_energy() {
    int energy = 0;
    int i = 0;
    while (i < 8) {
        energy = energy + cost_matrix[i][perm[i]];
        i = i + 1;
    }
    return energy;
}

void swap_perm(int a, int b) {
    int tmp = perm[a];
    perm[a] = perm[b];
    perm[b] = tmp;
}

void copy_to_best() {
    int i = 0;
    while (i < 8) {
        best_perm[i] = perm[i];
        i = i + 1;
    }
}

void copy_from_best() {
    int i = 0;
    while (i < 8) {
        perm[i] = best_perm[i];
        i = i + 1;
    }
}

// Simulated annealing: temperature is integer, scaled by 100
// accept if delta < 0, or with probability based on temp
int should_accept(int delta, int temp) {
    if (delta <= 0) return 1;
    // Simple approximation: accept if delta * 100 < temp
    if (delta * 100 < temp) return 1;
    // Probabilistic: use random threshold
    int threshold = temp / (delta + 1);
    int r = sa_rand() % 100;
    if (r < threshold) return 1;
    return 0;
}

int main() {
    sa_rng = 12345;

    // Set up cost matrix (lower diagonal-ish costs are better)
    int i = 0;
    while (i < 8) {
        int j = 0;
        while (j < 8) {
            int diff = i - j;
            if (diff < 0) diff = -diff;
            cost_matrix[i][j] = diff * diff + 1;
            j = j + 1;
        }
        i = i + 1;
    }

    // Optimal assignment: perm[i] = i (each assigned to self, cost = 1 each, total = 8)
    printf("Cost matrix diagonal values: ");
    i = 0;
    while (i < 8) {
        printf("%d ", cost_matrix[i][i]);
        i = i + 1;
    }
    printf("\n");
    // EXPECT: Cost matrix diagonal values: 1 1 1 1 1 1 1 1 

    // Initialize with a bad permutation (reverse)
    i = 0;
    while (i < 8) {
        perm[i] = 7 - i;
        i = i + 1;
    }

    int initial_energy = compute_energy();
    printf("Initial perm: %d %d %d %d %d %d %d %d\n",
           perm[0], perm[1], perm[2], perm[3],
           perm[4], perm[5], perm[6], perm[7]);
    // EXPECT: Initial perm: 7 6 5 4 3 2 1 0
    printf("Initial energy: %d\n", initial_energy);
    // EXPECT: Initial energy: 176

    copy_to_best();
    int best_energy = initial_energy;

    // Simulated annealing
    int temp = 10000;  // scaled by 100
    int iterations = 0;
    int accepted = 0;

    while (temp > 1) {
        int step = 0;
        while (step < 50) {
            // Pick two random positions and try swapping
            int a = sa_rand_range(0, 7);
            int b = sa_rand_range(0, 7);
            if (a != b) {
                int old_energy = compute_energy();
                swap_perm(a, b);
                int new_energy = compute_energy();
                int delta = new_energy - old_energy;

                if (should_accept(delta, temp)) {
                    accepted = accepted + 1;
                    if (new_energy < best_energy) {
                        best_energy = new_energy;
                        copy_to_best();
                    }
                } else {
                    // Revert swap
                    swap_perm(a, b);
                }
            }
            step = step + 1;
            iterations = iterations + 1;
        }
        // Cool down
        temp = temp * 95 / 100;
    }

    printf("Iterations: %d\n", iterations);
    // EXPECT: Iterations: 6600

    copy_from_best();
    printf("Best energy: %d\n", best_energy);
    // EXPECT: Best energy: 8

    printf("Best perm: %d %d %d %d %d %d %d %d\n",
           best_perm[0], best_perm[1], best_perm[2], best_perm[3],
           best_perm[4], best_perm[5], best_perm[6], best_perm[7]);
    // EXPECT: Best perm: 0 1 2 3 4 5 6 7

    // Verify it's a valid permutation (each number 0-7 appears once)
    int counts[8];
    i = 0;
    while (i < 8) {
        counts[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (i < 8) {
        counts[best_perm[i]] = counts[best_perm[i]] + 1;
        i = i + 1;
    }
    int valid = 1;
    i = 0;
    while (i < 8) {
        if (counts[i] != 1) valid = 0;
        i = i + 1;
    }
    if (valid) {
        printf("Valid permutation\n");
    } else {
        printf("Invalid permutation\n");
    }
    // EXPECT: Valid permutation

    printf("SA done\n");
    // EXPECT: SA done

    return 0;
}
