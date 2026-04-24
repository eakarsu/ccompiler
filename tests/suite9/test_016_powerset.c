int printf(const char *fmt, ...);

// Power set generation using recursion

int subset[10];
int subset_size;
int total_subsets;

void generate_powerset(int *arr, int n, int index) {
    int i;
    if (index == n) {
        total_subsets = total_subsets + 1;
        return;
    }
    // Exclude current element
    generate_powerset(arr, n, index + 1);
    // Include current element
    subset[subset_size] = arr[index];
    subset_size = subset_size + 1;
    generate_powerset(arr, n, index + 1);
    subset_size = subset_size - 1;
}

void print_powerset(int *arr, int n, int index) {
    int i;
    if (index == n) {
        printf("{");
        for (i = 0; i < subset_size; i++) {
            if (i > 0) printf(",");
            printf("%d", subset[i]);
        }
        printf("}\n");
        return;
    }
    // Exclude current element
    print_powerset(arr, n, index + 1);
    // Include current element
    subset[subset_size] = arr[index];
    subset_size = subset_size + 1;
    print_powerset(arr, n, index + 1);
    subset_size = subset_size - 1;
}

int sum_of_subsets;

void sum_all_subsets(int *arr, int n, int index, int current_sum) {
    if (index == n) {
        sum_of_subsets = sum_of_subsets + current_sum;
        return;
    }
    // Exclude
    sum_all_subsets(arr, n, index + 1, current_sum);
    // Include
    sum_all_subsets(arr, n, index + 1, current_sum + arr[index]);
}

int target_count;

void count_subsets_with_sum(int *arr, int n, int index, int current_sum, int target) {
    if (index == n) {
        if (current_sum == target) {
            target_count = target_count + 1;
        }
        return;
    }
    count_subsets_with_sum(arr, n, index + 1, current_sum, target);
    count_subsets_with_sum(arr, n, index + 1, current_sum + arr[index], target);
}

int max_subset_sum;

void find_max_subset_sum_le(int *arr, int n, int index, int current_sum, int limit) {
    if (current_sum > limit) return;
    if (current_sum > max_subset_sum) {
        max_subset_sum = current_sum;
    }
    if (index == n) return;
    find_max_subset_sum_le(arr, n, index + 1, current_sum, limit);
    find_max_subset_sum_le(arr, n, index + 1, current_sum + arr[index], limit);
}

int power_of_2(int n) {
    int result;
    int i;
    result = 1;
    for (i = 0; i < n; i++) {
        result = result * 2;
    }
    return result;
}

int main() {
    int arr3[3];
    int arr4[4];
    int arr5[5];
    int p;

    arr3[0] = 1; arr3[1] = 2; arr3[2] = 3;
    arr4[0] = 1; arr4[1] = 2; arr4[2] = 3; arr4[3] = 4;
    arr5[0] = 3; arr5[1] = 5; arr5[2] = 7; arr5[3] = 1; arr5[4] = 2;

    // Test 1: count subsets of {1,2,3}
    total_subsets = 0;
    subset_size = 0;
    generate_powerset(arr3, 3, 0);
    // EXPECT: Subsets of 3 elements: 8
    printf("Subsets of 3 elements: %d\n", total_subsets);

    // Test 2: count subsets of {1,2,3,4}
    total_subsets = 0;
    subset_size = 0;
    generate_powerset(arr4, 4, 0);
    // EXPECT: Subsets of 4 elements: 16
    printf("Subsets of 4 elements: %d\n", total_subsets);

    // Test 3: print all subsets of {1,2,3}
    // EXPECT: Power set of {1,2,3}:
    printf("Power set of {1,2,3}:\n");
    subset_size = 0;
    // EXPECT: {}
    // EXPECT: {3}
    // EXPECT: {2}
    // EXPECT: {2,3}
    // EXPECT: {1}
    // EXPECT: {1,3}
    // EXPECT: {1,2}
    // EXPECT: {1,2,3}
    print_powerset(arr3, 3, 0);

    // Test 4: sum of all subsets of {1,2,3}
    sum_of_subsets = 0;
    sum_all_subsets(arr3, 3, 0, 0);
    // EXPECT: Sum of all subsets: 24
    printf("Sum of all subsets: %d\n", sum_of_subsets);

    // Test 5: count subsets that sum to 5 in {1,2,3,4}
    target_count = 0;
    count_subsets_with_sum(arr4, 4, 0, 0, 5);
    // EXPECT: Subsets summing to 5: 2
    printf("Subsets summing to 5: %d\n", target_count);

    // Test 6: max subset sum <= 10 in {3,5,7,1,2}
    max_subset_sum = 0;
    find_max_subset_sum_le(arr5, 5, 0, 0, 10);
    // EXPECT: Max subset sum <= 10: 10
    printf("Max subset sum <= 10: %d\n", max_subset_sum);

    // Test 7: verify 2^n formula
    p = power_of_2(5);
    // EXPECT: 2^5 = 32
    printf("2^5 = %d\n", p);

    // EXPECT: Power set done
    printf("Power set done\n");

    return 0;
}
