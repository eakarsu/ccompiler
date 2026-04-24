int printf(const char *fmt, ...);

// Combination generation: n choose k

int factorial(int n) {
    int result = 1;
    int i;
    for (i = 2; i <= n; i++) {
        result = result * i;
    }
    return result;
}

int choose(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    // Use multiplicative formula to avoid overflow
    int result = 1;
    int i;
    if (k > n - k) k = n - k;
    for (i = 0; i < k; i++) {
        result = result * (n - i);
        result = result / (i + 1);
    }
    return result;
}

// Generate all combinations of k elements from {0..n-1}
// Store in combos array, return count
int generate_combinations(int n, int k, int combos[][6], int max_combos) {
    int current[6];
    int count = 0;
    int i;

    // Initialize first combination
    for (i = 0; i < k; i++) {
        current[i] = i;
    }

    while (1) {
        if (count < max_combos) {
            for (i = 0; i < k; i++) {
                combos[count][i] = current[i];
            }
        }
        count++;

        // Find rightmost element that can be incremented
        int pos = k - 1;
        while (pos >= 0 && current[pos] == n - k + pos) {
            pos--;
        }
        if (pos < 0) break;

        current[pos]++;
        for (i = pos + 1; i < k; i++) {
            current[i] = current[i - 1] + 1;
        }
    }
    return count;
}

void print_combination(int *combo, int k) {
    int i;
    printf("{");
    for (i = 0; i < k; i++) {
        if (i > 0) printf(",");
        printf("%d", combo[i]);
    }
    printf("}");
}

// Verify Pascal's identity: C(n,k) = C(n-1,k-1) + C(n-1,k)
int verify_pascal_identity(int n, int k) {
    if (k <= 0 || k >= n) return 1;
    return choose(n, k) == choose(n - 1, k - 1) + choose(n - 1, k);
}

// Sum of C(n,k) for k=0..n should be 2^n
int verify_sum_identity(int n) {
    int sum = 0;
    int k;
    for (k = 0; k <= n; k++) {
        sum = sum + choose(n, k);
    }
    int power = 1;
    int i;
    for (i = 0; i < n; i++) {
        power = power * 2;
    }
    return sum == power;
}

// Alternating sum: sum of (-1)^k * C(n,k) should be 0 for n>0
int verify_alternating_sum(int n) {
    int sum = 0;
    int k;
    int sign = 1;
    for (k = 0; k <= n; k++) {
        sum = sum + sign * choose(n, k);
        sign = -sign;
    }
    return sum;
}

// Vandermonde's identity: C(m+n, r) = sum C(m,k)*C(n,r-k)
int vandermonde_sum(int m, int n, int r) {
    int sum = 0;
    int k;
    for (k = 0; k <= r; k++) {
        sum = sum + choose(m, k) * choose(n, r - k);
    }
    return sum;
}

int main(void) {
    // Test 1: Basic choose values
    printf("C(5,0): %d\n", choose(5, 0));
    // EXPECT: C(5,0): 1
    printf("C(5,1): %d\n", choose(5, 1));
    // EXPECT: C(5,1): 5
    printf("C(5,2): %d\n", choose(5, 2));
    // EXPECT: C(5,2): 10
    printf("C(5,3): %d\n", choose(5, 3));
    // EXPECT: C(5,3): 10
    printf("C(5,5): %d\n", choose(5, 5));
    // EXPECT: C(5,5): 1

    // Test 2: Larger values
    printf("C(10,3): %d\n", choose(10, 3));
    // EXPECT: C(10,3): 120
    printf("C(10,5): %d\n", choose(10, 5));
    // EXPECT: C(10,5): 252
    printf("C(12,4): %d\n", choose(12, 4));
    // EXPECT: C(12,4): 495

    // Test 3: Generate all C(5,2) combinations
    int combos[20][6];
    int count = generate_combinations(5, 2, combos, 20);
    printf("C(5,2) count: %d\n", count);
    // EXPECT: C(5,2) count: 10

    // Print first 3 and last combination
    printf("first: {%d,%d}\n", combos[0][0], combos[0][1]);
    // EXPECT: first: {0,1}
    printf("second: {%d,%d}\n", combos[1][0], combos[1][1]);
    // EXPECT: second: {0,2}
    printf("third: {%d,%d}\n", combos[2][0], combos[2][1]);
    // EXPECT: third: {0,3}
    printf("last: {%d,%d}\n", combos[count - 1][0], combos[count - 1][1]);
    // EXPECT: last: {3,4}

    // Test 4: C(4,3)
    int combos2[10][6];
    int c2 = generate_combinations(4, 3, combos2, 10);
    printf("C(4,3) count: %d\n", c2);
    // EXPECT: C(4,3) count: 4

    // Test 5: Pascal's identity
    printf("pascal(5,2): %d\n", verify_pascal_identity(5, 2));
    // EXPECT: pascal(5,2): 1
    printf("pascal(10,4): %d\n", verify_pascal_identity(10, 4));
    // EXPECT: pascal(10,4): 1
    printf("pascal(8,3): %d\n", verify_pascal_identity(8, 3));
    // EXPECT: pascal(8,3): 1

    // Test 6: Sum identity (sum C(n,k) = 2^n)
    printf("sum(5): %d\n", verify_sum_identity(5));
    // EXPECT: sum(5): 1
    printf("sum(8): %d\n", verify_sum_identity(8));
    // EXPECT: sum(8): 1

    // Test 7: Alternating sum (should be 0 for n>0)
    printf("alt(5): %d\n", verify_alternating_sum(5));
    // EXPECT: alt(5): 0
    printf("alt(8): %d\n", verify_alternating_sum(8));
    // EXPECT: alt(8): 0

    // Test 8: Vandermonde's identity: C(m+n,r) = sum C(m,k)*C(n,r-k)
    int vand = vandermonde_sum(3, 4, 3);
    printf("vand(3,4,3): %d = %d\n", vand, choose(7, 3));
    // EXPECT: vand(3,4,3): 35 = 35

    int vand2 = vandermonde_sum(5, 3, 4);
    printf("vand(5,3,4): %d = %d\n", vand2, choose(8, 4));
    // EXPECT: vand(5,3,4): 70 = 70

    printf("done\n");
    // EXPECT: done
    return 0;
}
