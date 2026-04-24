int printf(const char *fmt, ...);

// Heat diffusion simulation on a 1D grid (integer-based)
// Temperature values scaled by 100
// Using explicit finite difference: T_new[i] = T[i] + alpha*(T[i-1] - 2*T[i] + T[i+1])

struct HeatGrid {
    int temp[20];
    int size;
    int left_bc;
    int right_bc;
    int alpha;
};

void init_grid(struct HeatGrid *g, int size, int initial_temp, int left, int right, int alpha) {
    int i;
    g->size = size;
    g->left_bc = left;
    g->right_bc = right;
    g->alpha = alpha;
    for (i = 0; i < size; i = i + 1) {
        g->temp[i] = initial_temp;
    }
    g->temp[0] = left;
    g->temp[size - 1] = right;
}

void step_heat(struct HeatGrid *g) {
    int new_temp[20];
    int i;

    new_temp[0] = g->left_bc;
    new_temp[g->size - 1] = g->right_bc;

    for (i = 1; i < g->size - 1; i = i + 1) {
        int laplacian = g->temp[i - 1] - 2 * g->temp[i] + g->temp[i + 1];
        new_temp[i] = g->temp[i] + g->alpha * laplacian / 100;
    }

    for (i = 0; i < g->size; i = i + 1) {
        g->temp[i] = new_temp[i];
    }
}

int max_temp(struct HeatGrid *g) {
    int mx = g->temp[0];
    int i;
    for (i = 1; i < g->size; i = i + 1) {
        if (g->temp[i] > mx) mx = g->temp[i];
    }
    return mx;
}

int min_temp(struct HeatGrid *g) {
    int mn = g->temp[0];
    int i;
    for (i = 1; i < g->size; i = i + 1) {
        if (g->temp[i] < mn) mn = g->temp[i];
    }
    return mn;
}

int avg_temp(struct HeatGrid *g) {
    int sum = 0;
    int i;
    for (i = 0; i < g->size; i = i + 1) {
        sum = sum + g->temp[i];
    }
    return sum / g->size;
}

int total_energy(struct HeatGrid *g) {
    int sum = 0;
    int i;
    for (i = 0; i < g->size; i = i + 1) {
        sum = sum + g->temp[i];
    }
    return sum;
}

void print_grid(struct HeatGrid *g) {
    int i;
    for (i = 0; i < g->size; i = i + 1) {
        printf("%d", g->temp[i]);
        if (i < g->size - 1) printf(" ");
    }
    printf("\n");
}

int main() {
    struct HeatGrid grid;
    int step;
    int i;

    printf("=== Heat Diffusion 1D ===\n");
    // EXPECT: === Heat Diffusion 1D ===

    // Test 1: Hot left boundary, cold right boundary (8 cells)
    init_grid(&grid, 8, 0, 1000, 0, 30);
    printf("Initial: ");
    print_grid(&grid);
    // EXPECT: Initial: 1000 0 0 0 0 0 0 0

    for (step = 0; step < 8; step = step + 1) {
        step_heat(&grid);
        printf("Step %d: ", step);
        print_grid(&grid);
    }
    // EXPECT: Step 0: 1000 300 0 0 0 0 0 0
    // EXPECT: Step 1: 1000 420 90 0 0 0 0 0
    // EXPECT: Step 2: 1000 495 162 27 0 0 0 0
    // EXPECT: Step 3: 1000 546 221 59 8 0 0 0
    // EXPECT: Step 4: 1000 584 269 92 20 2 0 0
    // EXPECT: Step 5: 1000 614 310 123 36 6 0 0
    // EXPECT: Step 6: 1000 638 345 153 53 13 1 0
    // EXPECT: Step 7: 1000 658 375 180 71 21 4 0

    // Test 2: Hot spot in the middle (10 cells)
    printf("--- Hot Spot ---\n");
    // EXPECT: --- Hot Spot ---
    init_grid(&grid, 10, 0, 0, 0, 25);
    grid.temp[4] = 1000;
    grid.temp[5] = 1000;

    printf("Initial: ");
    print_grid(&grid);
    // EXPECT: Initial: 0 0 0 0 1000 1000 0 0 0 0

    for (step = 0; step < 10; step = step + 1) {
        step_heat(&grid);
        if (step % 2 == 1) {
            printf("Step %d: ", step);
            print_grid(&grid);
        }
    }
    // EXPECT: Step 1: 0 0 62 312 625 625 312 62 0 0
    // EXPECT: Step 3: 0 34 140 327 492 492 327 140 34 0
    // EXPECT: Step 5: 0 66 173 314 419 419 314 173 66 0
    // EXPECT: Step 7: 0 83 185 296 371 371 296 185 83 0
    // EXPECT: Step 9: 0 90 187 279 337 337 279 187 90 0

    printf("Max=%d Min=%d Avg=%d\n", max_temp(&grid), min_temp(&grid), avg_temp(&grid));
    // EXPECT: Max=337 Min=0 Avg=178

    // Test 3: Steady state convergence (6 cells)
    printf("--- Steady State ---\n");
    // EXPECT: --- Steady State ---
    init_grid(&grid, 6, 500, 1000, 0, 40);

    for (step = 0; step < 200; step = step + 1) {
        step_heat(&grid);
    }

    printf("Converged: ");
    print_grid(&grid);
    // EXPECT: Converged: 1000 799 599 401 201 0

    // Test 4: Two hot sources (12 cells)
    printf("--- Two Sources ---\n");
    // EXPECT: --- Two Sources ---
    init_grid(&grid, 12, 0, 0, 0, 30);
    grid.temp[0] = 500;
    grid.temp[11] = 800;
    grid.left_bc = 500;
    grid.right_bc = 800;

    for (step = 0; step < 100; step = step + 1) {
        step_heat(&grid);
    }

    printf("Two source steady: ");
    print_grid(&grid);
    // EXPECT: Two source steady: 500 500 503 511 525 545 573 607 647 694 745 800

    // Test 5: Cooling object (uniform hot, cold boundaries)
    printf("--- Cooling ---\n");
    // EXPECT: --- Cooling ---
    init_grid(&grid, 8, 800, 0, 0, 25);

    printf("Energy at start: %d\n", total_energy(&grid));
    // EXPECT: Energy at start: 4800

    for (step = 0; step < 50; step = step + 1) {
        step_heat(&grid);
        if (step % 10 == 9) {
            printf("Step %d: energy=%d avg=%d\n",
                   step, total_energy(&grid), avg_temp(&grid));
        }
    }
    // EXPECT: Step 9: energy=2654 avg=331
    // EXPECT: Step 19: energy=1616 avg=202
    // EXPECT: Step 29: energy=986 avg=123
    // EXPECT: Step 39: energy=610 avg=76
    // EXPECT: Step 49: energy=384 avg=48

    // Test 6: Temperature gradient (10 cells, steady state)
    printf("--- Gradient ---\n");
    // EXPECT: --- Gradient ---
    init_grid(&grid, 10, 0, 1000, 0, 30);

    for (step = 0; step < 300; step = step + 1) {
        step_heat(&grid);
    }

    printf("Gradient: ");
    print_grid(&grid);
    // EXPECT: Gradient: 1000 877 757 640 526 415 307 202 100 0

    // Verify monotonically decreasing
    int monotonic = 1;
    for (i = 0; i < grid.size - 1; i = i + 1) {
        if (grid.temp[i] < grid.temp[i + 1]) monotonic = 0;
    }
    printf("Monotonic: %d\n", monotonic);
    // EXPECT: Monotonic: 1

    printf("Heat diffusion done\n");
    // EXPECT: Heat diffusion done

    return 0;
}
