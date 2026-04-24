int printf(const char *fmt, ...);

// Permutation generation: next permutation algorithm

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void reverse(int *arr, int left, int right) {
    while (left < right) {
        swap(&arr[left], &arr[right]);
        left++;
        right--;
    }
}

// Returns 1 if next permutation exists, 0 if already last
int next_permutation(int *arr, int n) {
    // Find largest i such that arr[i] < arr[i+1]
    int i = n - 2;
    while (i >= 0 && arr[i] >= arr[i + 1]) {
        i--;
    }
    if (i < 0) {
        return 0; // Last permutation
    }
    // Find largest j such that arr[j] > arr[i]
    int j = n - 1;
    while (arr[j] <= arr[i]) {
        j--;
    }
    swap(&arr[i], &arr[j]);
    reverse(arr, i + 1, n - 1);
    return 1;
}

void print_perm(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

// Count all permutations of array of size n
int count_permutations(int n) {
    int arr[6];
    int i;
    for (i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
    int count = 1;
    while (next_permutation(arr, n)) {
        count++;
    }
    return count;
}

// Check if array is sorted
int is_sorted(int *arr, int n) {
    int i;
    for (i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) return 0;
    }
    return 1;
}

// Find the k-th permutation (1-indexed) of {1..n}
void kth_permutation(int *result, int n, int k) {
    int avail[6];
    int i;
    for (i = 0; i < n; i++) {
        avail[i] = i + 1;
    }

    // Compute factorials
    int fact[7];
    fact[0] = 1;
    for (i = 1; i <= n; i++) {
        fact[i] = fact[i - 1] * i;
    }

    k = k - 1; // Convert to 0-indexed
    int remaining = n;
    int j;
    for (i = 0; i < n; i++) {
        int f = fact[remaining - 1];
        int idx = k / f;
        result[i] = avail[idx];
        // Remove avail[idx]
        for (j = idx; j < remaining - 1; j++) {
            avail[j] = avail[j + 1];
        }
        remaining--;
        k = k % f;
    }
}

// Count inversions in a permutation
int count_inversions(int *arr, int n) {
    int count = 0;
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (arr[i] > arr[j]) count++;
        }
    }
    return count;
}

int main(void) {
    // Test 1: Print first 6 permutations of {1,2,3}
    int a[3];
    a[0] = 1; a[1] = 2; a[2] = 3;
    printf("%d %d %d\n", a[0], a[1], a[2]);
    // EXPECT: 1 2 3
    next_permutation(a, 3);
    printf("%d %d %d\n", a[0], a[1], a[2]);
    // EXPECT: 1 3 2
    next_permutation(a, 3);
    printf("%d %d %d\n", a[0], a[1], a[2]);
    // EXPECT: 2 1 3
    next_permutation(a, 3);
    printf("%d %d %d\n", a[0], a[1], a[2]);
    // EXPECT: 2 3 1
    next_permutation(a, 3);
    printf("%d %d %d\n", a[0], a[1], a[2]);
    // EXPECT: 3 1 2
    next_permutation(a, 3);
    printf("%d %d %d\n", a[0], a[1], a[2]);
    // EXPECT: 3 2 1

    // Test 2: Verify no more permutations after last
    int more = next_permutation(a, 3);
    printf("more: %d\n", more);
    // EXPECT: more: 0

    // Test 3: Count permutations of {1..n}
    printf("perm(3): %d\n", count_permutations(3));
    // EXPECT: perm(3): 6
    printf("perm(4): %d\n", count_permutations(4));
    // EXPECT: perm(4): 24
    printf("perm(5): %d\n", count_permutations(5));
    // EXPECT: perm(5): 120

    // Test 4: k-th permutation
    int res[4];
    kth_permutation(res, 4, 1);
    printf("4p1: %d %d %d %d\n", res[0], res[1], res[2], res[3]);
    // EXPECT: 4p1: 1 2 3 4
    kth_permutation(res, 4, 24);
    printf("4p24: %d %d %d %d\n", res[0], res[1], res[2], res[3]);
    // EXPECT: 4p24: 4 3 2 1
    kth_permutation(res, 4, 10);
    printf("4p10: %d %d %d %d\n", res[0], res[1], res[2], res[3]);
    // EXPECT: 4p10: 2 3 4 1

    // Test 5: Inversions
    int b[4];
    b[0] = 1; b[1] = 2; b[2] = 3; b[3] = 4;
    printf("inv sorted: %d\n", count_inversions(b, 4));
    // EXPECT: inv sorted: 0
    b[0] = 4; b[1] = 3; b[2] = 2; b[3] = 1;
    printf("inv reversed: %d\n", count_inversions(b, 4));
    // EXPECT: inv reversed: 6
    b[0] = 2; b[1] = 3; b[2] = 1; b[3] = 4;
    printf("inv 2314: %d\n", count_inversions(b, 4));
    // EXPECT: inv 2314: 2

    // Test 6: Reverse function
    int c[5];
    c[0] = 1; c[1] = 2; c[2] = 3; c[3] = 4; c[4] = 5;
    reverse(c, 0, 4);
    printf("rev: %d %d %d %d %d\n", c[0], c[1], c[2], c[3], c[4]);
    // EXPECT: rev: 5 4 3 2 1
    reverse(c, 1, 3);
    printf("rev partial: %d %d %d %d %d\n", c[0], c[1], c[2], c[3], c[4]);
    // EXPECT: rev partial: 5 2 3 4 1

    printf("done\n");
    // EXPECT: done
    return 0;
}
