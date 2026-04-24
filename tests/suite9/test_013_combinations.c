int printf(const char *fmt, ...);

// Combination generation using recursion

int combo[10];
int combo_count;

void count_combinations(int n, int k, int start, int depth) {
    int i;
    if (depth == k) {
        combo_count = combo_count + 1;
        return;
    }
    for (i = start; i <= n; i++) {
        combo[depth] = i;
        count_combinations(n, k, i + 1, depth + 1);
    }
}

void print_combinations(int n, int k, int start, int depth) {
    int i;
    int j;
    if (depth == k) {
        for (j = 0; j < k; j++) {
            if (j > 0) printf(" ");
            printf("%d", combo[j]);
        }
        printf("\n");
        return;
    }
    for (i = start; i <= n; i++) {
        combo[depth] = i;
        print_combinations(n, k, i + 1, depth + 1);
    }
}

int comb_sum_count;

void count_combo_with_sum(int *vals, int n, int target, int start, int depth, int current_sum) {
    int i;
    if (current_sum == target) {
        comb_sum_count = comb_sum_count + 1;
        return;
    }
    if (current_sum > target) return;
    if (depth >= n) return;
    for (i = start; i < n; i++) {
        combo[depth] = vals[i];
        count_combo_with_sum(vals, n, target, i + 1, depth + 1, current_sum + vals[i]);
    }
}

int choose(int n, int k) {
    if (k == 0) return 1;
    if (k == n) return 1;
    if (k > n) return 0;
    return choose(n - 1, k - 1) + choose(n - 1, k);
}

void print_pascal_row(int row) {
    int k;
    for (k = 0; k <= row; k++) {
        if (k > 0) printf(" ");
        printf("%d", choose(row, k));
    }
    printf("\n");
}

int main() {
    int vals[6];
    int i;

    // Test C(5,2) = 10
    combo_count = 0;
    count_combinations(5, 2, 1, 0);
    // EXPECT: C(5,2) = 10
    printf("C(5,2) = %d\n", combo_count);

    // Test C(4,3) = 4
    combo_count = 0;
    count_combinations(4, 3, 1, 0);
    // EXPECT: C(4,3) = 4
    printf("C(4,3) = %d\n", combo_count);

    // Test C(6,0) = 1
    combo_count = 0;
    count_combinations(6, 0, 1, 0);
    // EXPECT: C(6,0) = 1
    printf("C(6,0) = %d\n", combo_count);

    // Print combinations of C(4,2)
    // EXPECT: C(4,2):
    printf("C(4,2):\n");
    // EXPECT: 1 2
    // EXPECT: 1 3
    // EXPECT: 1 4
    // EXPECT: 2 3
    // EXPECT: 2 4
    // EXPECT: 3 4
    print_combinations(4, 2, 1, 0);

    // Combination sum: find subsets of {1,2,3,4,5} that sum to 7
    vals[0] = 1; vals[1] = 2; vals[2] = 3; vals[3] = 4; vals[4] = 5;
    comb_sum_count = 0;
    count_combo_with_sum(vals, 5, 7, 0, 0, 0);
    // EXPECT: Subsets summing to 7: 3
    printf("Subsets summing to 7: %d\n", comb_sum_count);

    // Pascal's triangle rows 0-4 using choose
    // EXPECT: Pascal triangle:
    printf("Pascal triangle:\n");
    // EXPECT: 1
    // EXPECT: 1 1
    // EXPECT: 1 2 1
    // EXPECT: 1 3 3 1
    // EXPECT: 1 4 6 4 1
    for (i = 0; i < 5; i++) {
        print_pascal_row(i);
    }

    // Verify choose function
    // EXPECT: C(10,3) = 120
    printf("C(10,3) = %d\n", choose(10, 3));

    // EXPECT: Combinations done
    printf("Combinations done\n");

    return 0;
}
