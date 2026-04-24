int printf(const char *fmt, ...);
// EXPECT: Subsets of {1,2,3}: {} {1} {2} {1,2} {3} {1,3} {2,3} {1,2,3} \nTotal: 8\nSubsets of {1,2}: {} {1} {2} {1,2} \nTotal: 4\nSubsets with sum 5: 3\nSubsets with sum 10: 3\nSubsets with sum 15: 1\nSubsets with sum 0: 1
// Test: Generate subsets (power set)

int subsets[256][8];
int subset_sizes[256];
int num_subsets;

void generate_subsets_bitmask(int arr[], int n) {
    num_subsets = 0;
    int total = 1;
    int i;
    for (i = 0; i < n; i++) total = total * 2;

    int mask;
    for (mask = 0; mask < total; mask++) {
        int size = 0;
        for (i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                subsets[num_subsets][size] = arr[i];
                size++;
            }
        }
        subset_sizes[num_subsets] = size;
        num_subsets++;
    }
}

void print_subsets(void) {
    int i, j;
    for (i = 0; i < num_subsets; i++) {
        printf("{");
        for (j = 0; j < subset_sizes[i]; j++) {
            if (j > 0) printf(",");
            printf("%d", subsets[i][j]);
        }
        printf("} ");
    }
    printf("\n");
}

int count_subsets_with_sum(int arr[], int n, int target) {
    int count = 0;
    int total = 1;
    int i;
    for (i = 0; i < n; i++) total = total * 2;

    int mask;
    for (mask = 0; mask < total; mask++) {
        int sum = 0;
        for (i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                sum = sum + arr[i];
            }
        }
        if (sum == target) count++;
    }
    return count;
}

int main(void) {
    int a[] = {1, 2, 3};
    generate_subsets_bitmask(a, 3);
    printf("Subsets of {1,2,3}: ");
    print_subsets();
    printf("Total: %d\n", num_subsets);

    int b[] = {1, 2};
    generate_subsets_bitmask(b, 2);
    printf("Subsets of {1,2}: ");
    print_subsets();
    printf("Total: %d\n", num_subsets);

    int c[] = {1, 2, 3, 4, 5};
    printf("Subsets with sum 5: %d\n", count_subsets_with_sum(c, 5, 5));
    printf("Subsets with sum 10: %d\n", count_subsets_with_sum(c, 5, 10));
    printf("Subsets with sum 15: %d\n", count_subsets_with_sum(c, 5, 15));
    printf("Subsets with sum 0: %d\n", count_subsets_with_sum(c, 5, 0));

    return 0;
}
