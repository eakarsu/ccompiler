int printf(const char *fmt, ...);

// Subset enumeration using bitmasks

int popcount(int mask) {
    int count = 0;
    while (mask) {
        count = count + (mask & 1);
        mask = mask >> 1;
    }
    return count;
}

// Print subset represented by bitmask from array
void print_subset(int *arr, int n, int mask) {
    printf("{");
    int first = 1;
    int i;
    for (i = 0; i < n; i++) {
        if (mask & (1 << i)) {
            if (!first) printf(",");
            printf("%d", arr[i]);
            first = 0;
        }
    }
    printf("}");
}

// Count subsets of given size
int count_subsets_of_size(int n, int k) {
    int total = 1 << n;
    int count = 0;
    int mask;
    for (mask = 0; mask < total; mask++) {
        if (popcount(mask) == k) {
            count++;
        }
    }
    return count;
}

// Sum of elements in subset
int subset_sum(int *arr, int n, int mask) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (mask & (1 << i)) {
            sum = sum + arr[i];
        }
    }
    return sum;
}

// Count subsets whose sum equals target
int count_subsets_with_sum(int *arr, int n, int target) {
    int total = 1 << n;
    int count = 0;
    int mask;
    for (mask = 0; mask < total; mask++) {
        if (subset_sum(arr, n, mask) == target) {
            count++;
        }
    }
    return count;
}

// Find maximum subset sum that doesn't exceed limit
int max_subset_sum_bounded(int *arr, int n, int limit) {
    int total = 1 << n;
    int best = 0;
    int mask;
    for (mask = 0; mask < total; mask++) {
        int s = subset_sum(arr, n, mask);
        if (s <= limit && s > best) {
            best = s;
        }
    }
    return best;
}

// Enumerate all pairs of disjoint subsets
int count_disjoint_pairs(int n) {
    int total = 1 << n;
    int count = 0;
    int a;
    int b;
    for (a = 0; a < total; a++) {
        for (b = a + 1; b < total; b++) {
            if ((a & b) == 0) {
                count++;
            }
        }
    }
    return count;
}

// Check if mask1 is a subset of mask2
int is_submask(int mask1, int mask2) {
    return (mask1 & mask2) == mask1;
}

// Count submasks of a given mask
int count_submasks(int mask) {
    int count = 0;
    int sub = mask;
    while (sub > 0) {
        count++;
        sub = (sub - 1) & mask;
    }
    count++; // count the empty submask
    return count;
}

// Gray code: list all 2^n values where consecutive differ by 1 bit
int gray_code(int i) {
    return i ^ (i >> 1);
}

int verify_gray_property(int n) {
    int total = 1 << n;
    int i;
    for (i = 0; i < total - 1; i++) {
        int g1 = gray_code(i);
        int g2 = gray_code(i + 1);
        int diff = g1 ^ g2;
        // diff should be a power of 2 (exactly one bit)
        if (popcount(diff) != 1) return 0;
    }
    return 1;
}

int main(void) {
    // Test 1: Total subsets of set of size n
    printf("subsets(0): %d\n", 1 << 0);
    // EXPECT: subsets(0): 1
    printf("subsets(3): %d\n", 1 << 3);
    // EXPECT: subsets(3): 8
    printf("subsets(5): %d\n", 1 << 5);
    // EXPECT: subsets(5): 32

    // Test 2: Popcount
    printf("pop(0): %d\n", popcount(0));
    // EXPECT: pop(0): 0
    printf("pop(7): %d\n", popcount(7));
    // EXPECT: pop(7): 3
    printf("pop(255): %d\n", popcount(255));
    // EXPECT: pop(255): 8
    printf("pop(170): %d\n", popcount(170));
    // EXPECT: pop(170): 4

    // Test 3: Count subsets of given size (should match C(n,k))
    printf("size(4,0): %d\n", count_subsets_of_size(4, 0));
    // EXPECT: size(4,0): 1
    printf("size(4,1): %d\n", count_subsets_of_size(4, 1));
    // EXPECT: size(4,1): 4
    printf("size(4,2): %d\n", count_subsets_of_size(4, 2));
    // EXPECT: size(4,2): 6
    printf("size(4,3): %d\n", count_subsets_of_size(4, 3));
    // EXPECT: size(4,3): 4
    printf("size(4,4): %d\n", count_subsets_of_size(4, 4));
    // EXPECT: size(4,4): 1

    // Test 4: Subset sum
    int arr[5];
    arr[0] = 1; arr[1] = 2; arr[2] = 4; arr[3] = 8; arr[4] = 16;
    printf("sum all: %d\n", subset_sum(arr, 5, 31));
    // EXPECT: sum all: 31
    printf("sum {0,2}: %d\n", subset_sum(arr, 5, 5));
    // EXPECT: sum {0,2}: 5

    // Test 5: Count subsets with given sum
    int b[4];
    b[0] = 1; b[1] = 2; b[2] = 3; b[3] = 4;
    printf("sum=5: %d\n", count_subsets_with_sum(b, 4, 5));
    // EXPECT: sum=5: 2
    printf("sum=6: %d\n", count_subsets_with_sum(b, 4, 6));
    // EXPECT: sum=6: 2
    printf("sum=10: %d\n", count_subsets_with_sum(b, 4, 10));
    // EXPECT: sum=10: 1

    // Test 6: Max bounded sum (knapsack-like)
    int c[4];
    c[0] = 3; c[1] = 7; c[2] = 5; c[3] = 2;
    printf("max<=10: %d\n", max_subset_sum_bounded(c, 4, 10));
    // EXPECT: max<=10: 10
    printf("max<=8: %d\n", max_subset_sum_bounded(c, 4, 8));
    // EXPECT: max<=8: 8
    printf("max<=6: %d\n", max_subset_sum_bounded(c, 4, 6));
    // EXPECT: max<=6: 5

    // Test 7: Disjoint pairs
    printf("disjoint(2): %d\n", count_disjoint_pairs(2));
    // EXPECT: disjoint(2): 4
    printf("disjoint(3): %d\n", count_disjoint_pairs(3));
    // EXPECT: disjoint(3): 13

    // Test 8: Submask counting (2^popcount(mask))
    printf("submasks(7): %d\n", count_submasks(7));
    // EXPECT: submasks(7): 8
    printf("submasks(5): %d\n", count_submasks(5));
    // EXPECT: submasks(5): 4
    printf("submasks(15): %d\n", count_submasks(15));
    // EXPECT: submasks(15): 16

    // Test 9: Gray code verification
    printf("gray(3): %d\n", verify_gray_property(3));
    // EXPECT: gray(3): 1
    printf("gray(4): %d\n", verify_gray_property(4));
    // EXPECT: gray(4): 1

    // Test 10: Gray code values
    printf("g(0): %d\n", gray_code(0));
    // EXPECT: g(0): 0
    printf("g(1): %d\n", gray_code(1));
    // EXPECT: g(1): 1
    printf("g(2): %d\n", gray_code(2));
    // EXPECT: g(2): 3
    printf("g(3): %d\n", gray_code(3));
    // EXPECT: g(3): 2
    printf("g(4): %d\n", gray_code(4));
    // EXPECT: g(4): 6
    printf("g(5): %d\n", gray_code(5));
    // EXPECT: g(5): 7
    printf("g(6): %d\n", gray_code(6));
    // EXPECT: g(6): 5
    printf("g(7): %d\n", gray_code(7));
    // EXPECT: g(7): 4

    printf("done\n");
    // EXPECT: done
    return 0;
}
