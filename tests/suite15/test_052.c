int printf(const char *fmt, ...);

// Heat diffusion on a 1D bar (integer-scaled by 1000)
// T[i]_new = T[i] + alpha * (T[i-1] - 2*T[i] + T[i+1])
// alpha = 400 (0.4 scaled by 1000)
// Boundary conditions: T[0] = 1000 (hot), T[N-1] = 0 (cold)

void copy_array(int *dst, int *src, int n) {
    int i;
    for (i = 0; i < n; i++) {
        dst[i] = src[i];
    }
}

void heat_step(int *temp, int *temp_new, int n) {
    int alpha = 400;  // 0.4 * 1000
    int i;
    temp_new[0] = temp[0]; // fixed boundary
    temp_new[n - 1] = temp[n - 1]; // fixed boundary
    for (i = 1; i < n - 1; i++) {
        int laplacian = temp[i - 1] - 2 * temp[i] + temp[i + 1];
        temp_new[i] = temp[i] + alpha * laplacian / 1000;
    }
}

int sum_array(int *arr, int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i++) {
        s = s + arr[i];
    }
    return s;
}

int find_max(int *arr, int n) {
    int m = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] > m) m = arr[i];
    }
    return m;
}

int find_min(int *arr, int n) {
    int m = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] < m) m = arr[i];
    }
    return m;
}

// Check if temperature is monotonically non-increasing from left to right
int is_monotonic(int *arr, int n) {
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] > arr[i - 1]) return 0;
    }
    return 1;
}

// Count cells above a threshold
int count_above(int *arr, int n, int threshold) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] > threshold) count++;
    }
    return count;
}

int main(void) {
    int n = 8;
    int temp[8];
    int temp_new[8];
    int i;
    int step;

    // Initialize: hot left end, cold everywhere else
    temp[0] = 1000;
    for (i = 1; i < n; i++) {
        temp[i] = 0;
    }

    printf("Initial: %d %d %d %d %d %d %d %d\n",
           temp[0], temp[1], temp[2], temp[3],
           temp[4], temp[5], temp[6], temp[7]);
    // EXPECT: Initial: 1000 0 0 0 0 0 0 0

    // Step 1
    heat_step(temp, temp_new, n);
    copy_array(temp, temp_new, n);
    printf("Step 1: %d %d %d %d %d %d %d %d\n",
           temp[0], temp[1], temp[2], temp[3],
           temp[4], temp[5], temp[6], temp[7]);
    // EXPECT: Step 1: 1000 400 0 0 0 0 0 0

    // Step 2
    heat_step(temp, temp_new, n);
    copy_array(temp, temp_new, n);
    printf("Step 2: %d %d %d %d %d %d %d %d\n",
           temp[0], temp[1], temp[2], temp[3],
           temp[4], temp[5], temp[6], temp[7]);
    // EXPECT: Step 2: 1000 480 160 0 0 0 0 0

    // Run more steps
    for (step = 3; step <= 10; step++) {
        heat_step(temp, temp_new, n);
        copy_array(temp, temp_new, n);
    }
    printf("Step 10: %d %d %d %d %d %d %d %d\n",
           temp[0], temp[1], temp[2], temp[3],
           temp[4], temp[5], temp[6], temp[7]);
    // EXPECT: Step 10: 1000 730 490 298 163 77 29 0

    int total = sum_array(temp, n);
    printf("Total heat at step 10: %d\n", total);
    // EXPECT: Total heat at step 10: 2787

    int mono = is_monotonic(temp, n);
    printf("Monotonic at step 10: %d\n", mono);
    // EXPECT: Monotonic at step 10: 1

    int above_500 = count_above(temp, n, 500);
    printf("Cells above 500 at step 10: %d\n", above_500);
    // EXPECT: Cells above 500 at step 10: 2

    // Continue to step 20
    for (step = 11; step <= 20; step++) {
        heat_step(temp, temp_new, n);
        copy_array(temp, temp_new, n);
    }
    printf("Step 20: %d %d %d %d %d %d %d %d\n",
           temp[0], temp[1], temp[2], temp[3],
           temp[4], temp[5], temp[6], temp[7]);
    // EXPECT: Step 20: 1000 802 616 450 307 188 89 0

    int total_20 = sum_array(temp, n);
    printf("Total heat at step 20: %d\n", total_20);
    // EXPECT: Total heat at step 20: 3452

    int mono_20 = is_monotonic(temp, n);
    printf("Monotonic at step 20: %d\n", mono_20);
    // EXPECT: Monotonic at step 20: 1

    // Run to step 50 for near-convergence
    for (step = 21; step <= 50; step++) {
        heat_step(temp, temp_new, n);
        copy_array(temp, temp_new, n);
    }
    printf("Step 50: %d %d %d %d %d %d %d %d\n",
           temp[0], temp[1], temp[2], temp[3],
           temp[4], temp[5], temp[6], temp[7]);
    // EXPECT: Step 50: 1000 850 701 556 413 273 135 0

    // Check approximate linearity at steady state
    int diff_01 = temp[0] - temp[1];
    int diff_12 = temp[1] - temp[2];
    int diff_56 = temp[5] - temp[6];
    printf("Diff 0-1: %d\n", diff_01);
    // EXPECT: Diff 0-1: 150
    printf("Diff 1-2: %d\n", diff_12);
    // EXPECT: Diff 1-2: 149
    printf("Diff 5-6: %d\n", diff_56);
    // EXPECT: Diff 5-6: 138

    // All diffs should be close (within 15 of each other)
    int close = (diff_01 - diff_56 < 15) ? 1 : 0;
    printf("Gradient nearly uniform: %d\n", close);
    // EXPECT: Gradient nearly uniform: 1

    return 0;
}
