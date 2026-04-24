int printf(const char *fmt, ...);

// Population dynamics: Lotka-Volterra model (integer-scaled)
// prey' = a*prey - b*prey*pred
// pred' = -c*pred + d*prey*pred
// All values scaled by 1000 to avoid floats

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

// Simulate one step of Lotka-Volterra
// a=300 (0.3), b=20 (0.02), c=400 (0.4), d=10 (0.01)
// scale = 1000
void lv_step(int *prey, int *pred) {
    int p = *prey;
    int q = *pred;
    // dprey = (a*prey - b*prey*pred) / scale
    int dprey = (300 * p - 20 * p * q / 1000) / 1000;
    // dpred = (-c*pred + d*prey*pred) / scale
    int dpred = (-400 * q + 10 * p * q / 1000) / 1000;
    *prey = p + dprey;
    *pred = q + dpred;
}

void run_simulation(int *prey_hist, int *pred_hist, int steps) {
    int prey = 1000;
    int pred = 500;
    int i;
    prey_hist[0] = prey;
    pred_hist[0] = pred;
    for (i = 1; i <= steps; i++) {
        lv_step(&prey, &pred);
        prey_hist[i] = prey;
        pred_hist[i] = pred;
    }
}

int find_max(int *arr, int n) {
    int max = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] > max) max = arr[i];
    }
    return max;
}

int find_min(int *arr, int n) {
    int min = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] < min) min = arr[i];
    }
    return min;
}

int compute_average(int *arr, int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        sum = sum + arr[i];
    }
    return sum / n;
}

// Count how many times population crosses a threshold
int count_crossings(int *arr, int n, int threshold) {
    int crossings = 0;
    int i;
    for (i = 1; i < n; i++) {
        if ((arr[i - 1] < threshold && arr[i] >= threshold) ||
            (arr[i - 1] >= threshold && arr[i] < threshold)) {
            crossings++;
        }
    }
    return crossings;
}

// Check if population is growing at step i
int is_growing(int *arr, int i) {
    if (i <= 0) return 0;
    return arr[i] > arr[i - 1] ? 1 : 0;
}

int main(void) {
    int prey_hist[21];
    int pred_hist[21];
    int steps = 20;

    run_simulation(prey_hist, pred_hist, steps);

    printf("Initial prey: %d\n", prey_hist[0]);
    // EXPECT: Initial prey: 1000
    printf("Initial pred: %d\n", pred_hist[0]);
    // EXPECT: Initial pred: 500

    printf("Step 1 prey: %d\n", prey_hist[1]);
    // EXPECT: Step 1 prey: 1290
    printf("Step 1 pred: %d\n", pred_hist[1]);
    // EXPECT: Step 1 pred: 305

    printf("Step 2 prey: %d\n", prey_hist[2]);
    // EXPECT: Step 2 prey: 1669
    printf("Step 2 pred: %d\n", pred_hist[2]);
    // EXPECT: Step 2 pred: 187

    printf("Step 5 prey: %d\n", prey_hist[5]);
    // EXPECT: Step 5 prey: 3643
    printf("Step 5 pred: %d\n", pred_hist[5]);
    // EXPECT: Step 5 pred: 46

    printf("Step 10 prey: %d\n", prey_hist[10]);
    // EXPECT: Step 10 prey: 13495
    printf("Step 10 pred: %d\n", pred_hist[10]);
    // EXPECT: Step 10 pred: 8

    int max_prey = find_max(prey_hist, steps + 1);
    int min_prey = find_min(prey_hist, steps + 1);
    int max_pred = find_max(pred_hist, steps + 1);
    int min_pred = find_min(pred_hist, steps + 1);

    printf("Max prey: %d\n", max_prey);
    // EXPECT: Max prey: 185838
    printf("Min prey: %d\n", min_prey);
    // EXPECT: Min prey: 1000
    printf("Max pred: %d\n", max_pred);
    // EXPECT: Max pred: 500
    printf("Min pred: %d\n", min_pred);
    // EXPECT: Min pred: 5

    int avg_prey = compute_average(prey_hist, steps + 1);
    printf("Avg prey: %d\n", avg_prey);
    // EXPECT: Avg prey: 38211

    int crossings = count_crossings(prey_hist, steps + 1, 10000);
    printf("Prey crosses 10000: %d\n", crossings);
    // EXPECT: Prey crosses 10000: 1

    int growing_at_10 = is_growing(prey_hist, 10);
    printf("Prey growing at step 10: %d\n", growing_at_10);
    // EXPECT: Prey growing at step 10: 1

    int pred_growing_at_5 = is_growing(pred_hist, 5);
    printf("Pred growing at step 5: %d\n", pred_growing_at_5);
    // EXPECT: Pred growing at step 5: 0

    printf("Final prey: %d\n", prey_hist[steps]);
    // EXPECT: Final prey: 185838
    printf("Final pred: %d\n", pred_hist[steps]);
    // EXPECT: Final pred: 26

    // Prey always grows in this parameter regime, pred always decays initially
    int prey_monotonic = 1;
    int i;
    for (i = 1; i <= steps; i++) {
        if (prey_hist[i] < prey_hist[i - 1]) {
            prey_monotonic = 0;
        }
    }
    printf("Prey monotonically increasing: %d\n", prey_monotonic);
    // EXPECT: Prey monotonically increasing: 1

    return 0;
}
