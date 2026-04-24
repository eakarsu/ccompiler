int printf(const char *fmt, ...);
// EXPECT: All permutations of {1,2,3}:\n1 2 3\n1 3 2\n2 1 3\n2 3 1\n3 1 2\n3 2 1\nTotal: 6\n\nAll permutations of {1,2,3,4}:\n1 2 3 4\n1 2 4 3\n1 3 2 4\n1 3 4 2\n1 4 2 3\n1 4 3 2\n2 1 3 4\n2 1 4 3\n2 3 1 4\n2 3 4 1\n2 4 1 3\n2 4 3 1\n3 1 2 4\n3 1 4 2\n3 2 1 4\n3 2 4 1\n3 4 1 2\n3 4 2 1\n4 1 2 3\n4 1 3 2\n4 2 1 3\n4 2 3 1\n4 3 1 2\n4 3 2 1\nTotal: 24\n\nStarting from [2,4,1,3]:\n2 4 1 3\n2 4 3 1\n3 1 2 4\n3 1 4 2\n3 2 1 4\n3 2 4 1\nGenerated 6 permutations\n\nPermutations of {1,1,2,2}:\n1 1 2 2\n1 2 1 2\n1 2 2 1\n2 1 1 2\n2 1 2 1\n2 2 1 1\nTotal: 6
// Test: Next permutation algorithm

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void reverse(int arr[], int lo, int hi) {
    while (lo < hi) {
        swap(&arr[lo], &arr[hi]);
        lo++;
        hi--;
    }
}

// Returns 1 if next permutation exists, 0 if already last
int next_perm(int arr[], int n) {
    // Find largest i such that arr[i] < arr[i+1]
    int i = n - 2;
    while (i >= 0 && arr[i] >= arr[i + 1]) {
        i--;
    }
    if (i < 0) return 0;

    // Find largest j > i such that arr[j] > arr[i]
    int j = n - 1;
    while (arr[j] <= arr[i]) {
        j--;
    }

    swap(&arr[i], &arr[j]);
    reverse(arr, i + 1, n - 1);
    return 1;
}

void print_arr(int arr[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(" ");
    }
}

int main(void) {
    int i;

    // Generate all permutations of {1,2,3}
    int a[3];
    a[0] = 1; a[1] = 2; a[2] = 3;

    printf("All permutations of {1,2,3}:\n");
    int count = 1;
    print_arr(a, 3);
    printf("\n");
    while (next_perm(a, 3)) {
        print_arr(a, 3);
        printf("\n");
        count++;
    }
    printf("Total: %d\n\n", count);

    // Generate all permutations of {1,2,3,4}
    int b[4];
    b[0] = 1; b[1] = 2; b[2] = 3; b[3] = 4;

    printf("All permutations of {1,2,3,4}:\n");
    count = 1;
    print_arr(b, 4);
    printf("\n");
    while (next_perm(b, 4)) {
        print_arr(b, 4);
        printf("\n");
        count++;
    }
    printf("Total: %d\n\n", count);

    // Start from middle permutation
    int c[4];
    c[0] = 2; c[1] = 4; c[2] = 1; c[3] = 3;
    printf("Starting from [2,4,1,3]:\n");
    count = 0;
    print_arr(c, 4);
    printf("\n");
    count++;
    int j;
    for (j = 0; j < 5; j++) {
        if (next_perm(c, 4)) {
            print_arr(c, 4);
            printf("\n");
            count++;
        }
    }
    printf("Generated %d permutations\n\n", count);

    // Count permutations of {1,1,2,2} (multiset)
    int d[4];
    d[0] = 1; d[1] = 1; d[2] = 2; d[3] = 2;
    printf("Permutations of {1,1,2,2}:\n");
    count = 1;
    print_arr(d, 4);
    printf("\n");
    while (next_perm(d, 4)) {
        print_arr(d, 4);
        printf("\n");
        count++;
    }
    printf("Total: %d\n", count);

    return 0;
}
