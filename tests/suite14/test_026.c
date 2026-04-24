int printf(const char *fmt, ...);

// Derangement counting
// D(n) = number of permutations where no element is in its original position
// D(n) = (n-1) * (D(n-1) + D(n-2))
// D(n) = n! * sum_{i=0}^{n} (-1)^i / i!

int factorial(int n) {
    int r = 1;
    int i;
    for (i = 2; i <= n; i++) {
        r = r * i;
    }
    return r;
}

int derangement(int n) {
    if (n == 0) return 1;
    if (n == 1) return 0;
    int prev2 = 1; // D(0)
    int prev1 = 0; // D(1)
    int i;
    int cur;
    for (i = 2; i <= n; i++) {
        cur = (i - 1) * (prev1 + prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

// Inclusion-exclusion formula: D(n) = sum_{i=0}^{n} (-1)^i * n! / i!
int derangement_ie(int n) {
    int nfact = factorial(n);
    int sum = 0;
    int sign = 1;
    int i;
    for (i = 0; i <= n; i++) {
        sum = sum + sign * nfact / factorial(i);
        sign = -sign;
    }
    return sum;
}

// Count derangements by brute force (for small n)
void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int is_derangement(int *perm, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (perm[i] == i) return 0;
    }
    return 1;
}

int count_derangements_brute(int n) {
    int arr[7];
    int i;
    for (i = 0; i < n; i++) arr[i] = i;
    int count = 0;
    // Generate all permutations and count derangements
    // Using simple next_permutation
    int total = factorial(n);
    int p;
    // Start with sorted array
    for (i = 0; i < n; i++) arr[i] = i;
    for (p = 0; p < total; p++) {
        if (is_derangement(arr, n)) count++;
        // Next permutation
        int k = n - 2;
        while (k >= 0 && arr[k] >= arr[k + 1]) k--;
        if (k >= 0) {
            int l = n - 1;
            while (arr[l] <= arr[k]) l--;
            swap(&arr[k], &arr[l]);
            // Reverse from k+1 to end
            int left = k + 1;
            int right = n - 1;
            while (left < right) {
                swap(&arr[left], &arr[right]);
                left++;
                right--;
            }
        }
    }
    return count;
}

// Subfactorial ratio: D(n)/n! approaches 1/e
// D(n)/n! * 1000 should approach 368 (= 1000/e)
int derangement_ratio_x1000(int n) {
    return derangement(n) * 1000 / factorial(n);
}

// Number of permutations with exactly k fixed points
// = C(n,k) * D(n-k)
int choose(int n, int k) {
    if (k > n - k) k = n - k;
    int r = 1;
    int i;
    for (i = 0; i < k; i++) {
        r = r * (n - i);
        r = r / (i + 1);
    }
    return r;
}

int perms_with_k_fixed(int n, int k) {
    return choose(n, k) * derangement(n - k);
}

// Verify: sum of perms_with_k_fixed for k=0..n should be n!
int verify_sum_fixed_points(int n) {
    int sum = 0;
    int k;
    for (k = 0; k <= n; k++) {
        sum = sum + perms_with_k_fixed(n, k);
    }
    return sum;
}

// Probleme des menages: married couples seated at round table,
// no husband next to wife. For n couples: M(n) = 2*n! * sum formula
// Simplified: just use derangements in related way
// Actually compute: number of ways n non-attacking rooks on complementary board

// Partial derangement: permutations with exactly 0 fixed points among first k
int partial_derangement_count(int n, int k) {
    // Count permutations of {0..n-1} where perm[i] != i for i < k
    // This is harder, use inclusion-exclusion on the first k positions
    int sum = 0;
    int mask;
    int sign;
    int nk = (1 << k);
    int bits;
    int b;
    for (mask = 0; mask < nk; mask++) {
        bits = 0;
        for (b = 0; b < k; b++) {
            if (mask & (1 << b)) bits++;
        }
        sign = (bits % 2 == 0) ? 1 : -1;
        // Fix the 'bits' positions, rest free: (n - bits)!
        sum = sum + sign * factorial(n - bits);
    }
    return sum;
}

int main(void) {
    // Test 1: Basic derangements
    printf("D(0): %d\n", derangement(0));
    // EXPECT: D(0): 1
    printf("D(1): %d\n", derangement(1));
    // EXPECT: D(1): 0
    printf("D(2): %d\n", derangement(2));
    // EXPECT: D(2): 1
    printf("D(3): %d\n", derangement(3));
    // EXPECT: D(3): 2
    printf("D(4): %d\n", derangement(4));
    // EXPECT: D(4): 9
    printf("D(5): %d\n", derangement(5));
    // EXPECT: D(5): 44
    printf("D(6): %d\n", derangement(6));
    // EXPECT: D(6): 265
    printf("D(7): %d\n", derangement(7));
    // EXPECT: D(7): 1854
    printf("D(8): %d\n", derangement(8));
    // EXPECT: D(8): 14833

    // Test 2: Inclusion-exclusion matches recurrence
    printf("ie(4): %d\n", derangement_ie(4));
    // EXPECT: ie(4): 9
    printf("ie(5): %d\n", derangement_ie(5));
    // EXPECT: ie(5): 44
    printf("ie(6): %d\n", derangement_ie(6));
    // EXPECT: ie(6): 265

    // Test 3: Brute force matches formula
    printf("brute(3): %d\n", count_derangements_brute(3));
    // EXPECT: brute(3): 2
    printf("brute(4): %d\n", count_derangements_brute(4));
    // EXPECT: brute(4): 9
    printf("brute(5): %d\n", count_derangements_brute(5));
    // EXPECT: brute(5): 44

    // Test 4: D(n)/n! ratio * 1000 (approaches 368 = 1000/e)
    printf("ratio(3): %d\n", derangement_ratio_x1000(3));
    // EXPECT: ratio(3): 333
    printf("ratio(5): %d\n", derangement_ratio_x1000(5));
    // EXPECT: ratio(5): 366
    printf("ratio(7): %d\n", derangement_ratio_x1000(7));
    // EXPECT: ratio(7): 367

    // Test 5: Permutations with exactly k fixed points
    printf("fix(4,0): %d\n", perms_with_k_fixed(4, 0));
    // EXPECT: fix(4,0): 9
    printf("fix(4,1): %d\n", perms_with_k_fixed(4, 1));
    // EXPECT: fix(4,1): 8
    printf("fix(4,2): %d\n", perms_with_k_fixed(4, 2));
    // EXPECT: fix(4,2): 6
    printf("fix(4,3): %d\n", perms_with_k_fixed(4, 3));
    // EXPECT: fix(4,3): 0
    printf("fix(4,4): %d\n", perms_with_k_fixed(4, 4));
    // EXPECT: fix(4,4): 1

    // Test 6: Sum of fixed point counts = n!
    printf("sumfix(4): %d\n", verify_sum_fixed_points(4));
    // EXPECT: sumfix(4): 24
    printf("sumfix(5): %d\n", verify_sum_fixed_points(5));
    // EXPECT: sumfix(5): 120
    printf("sumfix(6): %d\n", verify_sum_fixed_points(6));
    // EXPECT: sumfix(6): 720

    // Test 7: Partial derangements (no fixed in first k of n)
    printf("partial(4,2): %d\n", partial_derangement_count(4, 2));
    // EXPECT: partial(4,2): 14
    printf("partial(4,4): %d\n", partial_derangement_count(4, 4));
    // EXPECT: partial(4,4): 9
    printf("partial(5,3): %d\n", partial_derangement_count(5, 3));
    // EXPECT: partial(5,3): 64

    printf("done\n");
    // EXPECT: done
    return 0;
}
