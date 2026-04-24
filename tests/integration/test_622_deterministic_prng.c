int printf(const char *fmt, ...);
// EXPECT: === Deterministic PRNG Test ===\nGenerating 50 values with seed=1...\nFirst 10 unsorted values:\n  val[0] = 20\n  val[1] = 267\n  val[2] = 478\n  val[3] = 221\n  val[4] = 880\n  val[5] = 447\n  val[6] = 818\n  val[7] = 641\n  val[8] = 340\n  val[9] = 427\nBubble sorting...\nFirst 10 sorted values:\n  sorted[0] = 20\n  sorted[1] = 60\n  sorted[2] = 67\n  sorted[3] = 81\n  sorted[4] = 101\n  sorted[5] = 107\n  sorted[6] = 167\n  sorted[7] = 178\n  sorted[8] = 180\n  sorted[9] = 181\nBubble sort verified: PASS\nBubble sort checksum: 119\nSelection sorting...\nFirst 10 selection sorted values:\n  sel[0] = 20\n  sel[1] = 60\n  sel[2] = 67\n  sel[3] = 81\n  sel[4] = 101\n  sel[5] = 107\n  sel[6] = 167\n  sel[7] = 178\n  sel[8] = 180\n  sel[9] = 181\nSelection sort verified: PASS\nSelection sort checksum: 119\nBoth sorts match: YES\nRange distribution:\n  0-99: 4\n  100-499: 30\n  500-999: 16\nStats: min=20 max=940 sum=20799 avg=415\n=== Test Complete ===

// ============================================================
// Deterministic PRNG: generate, sort, verify, and analyze
// Tests: arrays, loops, function calls, comparisons, bitwise
// ============================================================

// Simple deterministic PRNG: seed = (seed * 13 + 7) % 1000
// Starting seed = 1
// Produces values in range [0, 999]

int prng_next(int seed) {
    return (seed * 13 + 7) % 1000;
}

// Generate n random values into arr, return final seed
int generate_values(int *arr, int n, int seed) {
    int i;
    for (i = 0; i < n; i++) {
        seed = prng_next(seed);
        arr[i] = seed;
    }
    return seed;
}

// Bubble sort: repeatedly swap adjacent out-of-order elements
void bubble_sort(int *arr, int n) {
    int i;
    int j;
    int tmp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

// Selection sort: find minimum, place at front
void selection_sort(int *arr, int n) {
    int i;
    int j;
    int min_idx;
    int tmp;
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            tmp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = tmp;
        }
    }
}

// Verify that array is sorted in non-decreasing order
int verify_sorted(int *arr, int n) {
    int i;
    for (i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

// Compute XOR checksum of all elements
int compute_checksum(int *arr, int n) {
    int checksum;
    int i;
    checksum = 0;
    for (i = 0; i < n; i++) {
        checksum = checksum ^ arr[i];
    }
    return checksum;
}

// Copy array src into dst
void copy_array(int *dst, int *src, int n) {
    int i;
    for (i = 0; i < n; i++) {
        dst[i] = src[i];
    }
}

// Check if two arrays are identical
int arrays_equal(int *a, int *b, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    return 1;
}

// Find minimum value in array
int find_min(int *arr, int n) {
    int min_val;
    int i;
    min_val = arr[0];
    for (i = 1; i < n; i++) {
        if (arr[i] < min_val) {
            min_val = arr[i];
        }
    }
    return min_val;
}

// Find maximum value in array
int find_max(int *arr, int n) {
    int max_val;
    int i;
    max_val = arr[0];
    for (i = 1; i < n; i++) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }
    return max_val;
}

// Compute sum of all elements
int compute_sum(int *arr, int n) {
    int sum;
    int i;
    sum = 0;
    for (i = 0; i < n; i++) {
        sum = sum + arr[i];
    }
    return sum;
}

// Count values in a range [lo, hi] inclusive
int count_in_range(int *arr, int n, int lo, int hi) {
    int count;
    int i;
    count = 0;
    for (i = 0; i < n; i++) {
        if (arr[i] >= lo && arr[i] <= hi) {
            count = count + 1;
        }
    }
    return count;
}

int main() {
    int original[50];
    int bubble[50];
    int selection[50];
    int n;
    int seed;
    int i;
    int sorted_ok;
    int checksum;
    int r0;
    int r1;
    int r2;
    int min_val;
    int max_val;
    int sum;
    int avg;

    n = 50;
    seed = 1;

    printf("=== Deterministic PRNG Test ===\n");
    printf("Generating %d values with seed=%d...\n", n, seed);

    // Generate values
    seed = generate_values(original, n, seed);

    // Print first 10 unsorted
    printf("First 10 unsorted values:\n");
    for (i = 0; i < 10; i++) {
        printf("  val[%d] = %d\n", i, original[i]);
    }

    // ---- Bubble sort ----
    copy_array(bubble, original, n);
    printf("Bubble sorting...\n");
    bubble_sort(bubble, n);

    printf("First 10 sorted values:\n");
    for (i = 0; i < 10; i++) {
        printf("  sorted[%d] = %d\n", i, bubble[i]);
    }

    sorted_ok = verify_sorted(bubble, n);
    if (sorted_ok) {
        printf("Bubble sort verified: PASS\n");
    } else {
        printf("Bubble sort verified: FAIL\n");
    }

    checksum = compute_checksum(bubble, n);
    printf("Bubble sort checksum: %d\n", checksum);

    // ---- Selection sort ----
    copy_array(selection, original, n);
    printf("Selection sorting...\n");
    selection_sort(selection, n);

    printf("First 10 selection sorted values:\n");
    for (i = 0; i < 10; i++) {
        printf("  sel[%d] = %d\n", i, selection[i]);
    }

    sorted_ok = verify_sorted(selection, n);
    if (sorted_ok) {
        printf("Selection sort verified: PASS\n");
    } else {
        printf("Selection sort verified: FAIL\n");
    }

    checksum = compute_checksum(selection, n);
    printf("Selection sort checksum: %d\n", checksum);

    // ---- Compare both sort results ----
    if (arrays_equal(bubble, selection, n)) {
        printf("Both sorts match: YES\n");
    } else {
        printf("Both sorts match: NO\n");
    }

    // ---- Range distribution ----
    r0 = count_in_range(original, n, 0, 99);
    r1 = count_in_range(original, n, 100, 499);
    r2 = count_in_range(original, n, 500, 999);
    printf("Range distribution:\n");
    printf("  0-99: %d\n", r0);
    printf("  100-499: %d\n", r1);
    printf("  500-999: %d\n", r2);

    // ---- Statistics ----
    min_val = find_min(original, n);
    max_val = find_max(original, n);
    sum = compute_sum(original, n);
    avg = sum / n;
    printf("Stats: min=%d max=%d sum=%d avg=%d\n", min_val, max_val, sum, avg);

    printf("=== Test Complete ===\n");

    return 0;
}
