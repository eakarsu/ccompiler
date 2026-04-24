int printf(const char *fmt, ...);

// Egyptian fraction representation
// Represent a fraction p/q as sum of distinct unit fractions 1/a + 1/b + ...

// GCD for fraction reduction
int gcd(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Greedy algorithm (Fibonacci-Sylvester):
// At each step, take the largest unit fraction <= remaining fraction
// Ceiling division: ceil(q/p) gives denominator of largest unit fraction <= p/q
int ceil_div(int a, int b) {
    return (a + b - 1) / b;
}

// Compute Egyptian fraction decomposition using greedy algorithm
// Store denominators in result array, return count
int egyptian_greedy(int p, int q, int *result, int max_terms) {
    int count = 0;
    while (p > 0 && count < max_terms) {
        int d = ceil_div(q, p);  // smallest d such that 1/d <= p/q
        result[count] = d;
        count = count + 1;
        // p/q - 1/d = (p*d - q) / (q*d)
        p = p * d - q;
        q = q * d;
        // Reduce the fraction
        if (p > 0) {
            int g = gcd(p, q);
            p = p / g;
            q = q / g;
        }
    }
    return count;
}

// Verify Egyptian fraction: sum of 1/result[i] should equal p/q
// Returns 1 if correct (using cross multiplication to avoid floats)
int verify_egyptian(int orig_p, int orig_q, int *denoms, int count) {
    // Compute sum as a fraction
    int sum_p = 0;
    int sum_q = 1;
    int i;
    for (i = 0; i < count; i++) {
        // sum_p/sum_q + 1/denoms[i] = (sum_p * denoms[i] + sum_q) / (sum_q * denoms[i])
        sum_p = sum_p * denoms[i] + sum_q;
        sum_q = sum_q * denoms[i];
        int g = gcd(sum_p, sum_q);
        if (g > 0) {
            sum_p = sum_p / g;
            sum_q = sum_q / g;
        }
    }
    // Reduce original
    int g = gcd(orig_p, orig_q);
    orig_p = orig_p / g;
    orig_q = orig_q / g;
    return (sum_p == orig_p && sum_q == orig_q);
}

// Check all denominators are distinct
int all_distinct(int *arr, int n) {
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (arr[i] == arr[j]) return 0;
        }
    }
    return 1;
}

// Print Egyptian fraction
void print_egyptian(int p, int q, int *denoms, int count) {
    printf("%d/%d = ", p, q);
    int i;
    for (i = 0; i < count; i++) {
        if (i > 0) printf(" + ");
        printf("1/%d", denoms[i]);
    }
    printf("\n");
}

// Count total number of unit fractions needed for all p/q where 1<=p<q<=n
int total_fractions_needed(int n) {
    int total = 0;
    int p;
    int q;
    int denoms[20];
    for (q = 2; q <= n; q++) {
        for (p = 1; p < q; p++) {
            int g = gcd(p, q);
            if (g == 1) {  // only reduced fractions
                int c = egyptian_greedy(p, q, denoms, 20);
                total = total + c;
            }
        }
    }
    return total;
}

int main(void) {
    int denoms[20];
    int count;

    // 2/3 = 1/2 + 1/6
    count = egyptian_greedy(2, 3, denoms, 20);
    print_egyptian(2, 3, denoms, count);
    // EXPECT: 2/3 = 1/2 + 1/6

    printf("terms: %d\n", count);
    // EXPECT: terms: 2

    printf("verify: %d\n", verify_egyptian(2, 3, denoms, count));
    // EXPECT: verify: 1

    printf("distinct: %d\n", all_distinct(denoms, count));
    // EXPECT: distinct: 1

    // 3/7 = 1/3 + 1/11 + 1/231
    count = egyptian_greedy(3, 7, denoms, 20);
    print_egyptian(3, 7, denoms, count);
    // EXPECT: 3/7 = 1/3 + 1/11 + 1/231

    printf("verify: %d\n", verify_egyptian(3, 7, denoms, count));
    // EXPECT: verify: 1

    // 4/5 = 1/2 + 1/4 + 1/20
    count = egyptian_greedy(4, 5, denoms, 20);
    print_egyptian(4, 5, denoms, count);
    // EXPECT: 4/5 = 1/2 + 1/4 + 1/20

    printf("verify: %d\n", verify_egyptian(4, 5, denoms, count));
    // EXPECT: verify: 1

    // 5/6 = 1/2 + 1/3
    count = egyptian_greedy(5, 6, denoms, 20);
    print_egyptian(5, 6, denoms, count);
    // EXPECT: 5/6 = 1/2 + 1/3

    printf("terms: %d\n", count);
    // EXPECT: terms: 2

    // 7/11
    count = egyptian_greedy(7, 11, denoms, 20);
    print_egyptian(7, 11, denoms, count);
    // EXPECT: 7/11 = 1/2 + 1/8 + 1/88

    printf("verify: %d\n", verify_egyptian(7, 11, denoms, count));
    // EXPECT: verify: 1

    // 1/1 should be trivial - but p/q where p=q is just 1, which is 1/1
    count = egyptian_greedy(1, 1, denoms, 20);
    printf("1/1 denom: %d\n", denoms[0]);
    // EXPECT: 1/1 denom: 1

    printf("terms for 1/1: %d\n", count);
    // EXPECT: terms for 1/1: 1

    // 2/5 = 1/3 + 1/15
    count = egyptian_greedy(2, 5, denoms, 20);
    print_egyptian(2, 5, denoms, count);
    // EXPECT: 2/5 = 1/3 + 1/15

    printf("verify: %d\n", verify_egyptian(2, 5, denoms, count));
    // EXPECT: verify: 1

    // Total fractions for all reduced proper fractions with denominator <= 6
    int tf = total_fractions_needed(6);
    printf("total fractions (denom<=6): %d\n", tf);
    // EXPECT: total fractions (denom<=6): 18

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
