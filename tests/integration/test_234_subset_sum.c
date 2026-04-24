int printf(const char *fmt, ...);
// EXPECT: has_sum(11)=1\nhas_sum(14)=1\nhas_sum(22)=0\ncount_sum(10)=3\ncount_sum(11)=3\nsubsets summing to 10:\n  {3,7}\n  {7,1,2}\n  {8,2}\nfound: 3\nsubsets summing to 11:\n  {3,7,1}\n  {3,8}\n  {1,8,2}\nfound: 3\nmin_diff=1\nequal_partition([1,5,11,5])=1\nequal_partition([1,2,3])=1\nknapsack(cap=5)=7\nknapsack(cap=10)=13
// Test 234: Recursive subset sum

int has_subset_sum(int *arr, int n, int target) {
    if (target == 0) return 1;
    if (n == 0) return 0;
    if (arr[n - 1] > target)
        return has_subset_sum(arr, n - 1, target);
    return has_subset_sum(arr, n - 1, target) ||
           has_subset_sum(arr, n - 1, target - arr[n - 1]);
}

int count_subset_sums(int *arr, int n, int target) {
    if (target == 0) return 1;
    if (n == 0) return 0;
    if (arr[n - 1] > target)
        return count_subset_sums(arr, n - 1, target);
    return count_subset_sums(arr, n - 1, target) +
           count_subset_sums(arr, n - 1, target - arr[n - 1]);
}

int subset[20];
int found_count;

void find_subsets(int *arr, int n, int target, int idx, int depth) {
    if (target == 0) {
        printf("  {");
        int i = 0;
        while (i < depth) {
            if (i > 0) printf(",");
            printf("%d", subset[i]);
            i = i + 1;
        }
        printf("}\n");
        found_count = found_count + 1;
        return;
    }
    if (idx == n || target < 0) return;
    subset[depth] = arr[idx];
    find_subsets(arr, n, target - arr[idx], idx + 1, depth + 1);
    find_subsets(arr, n, target, idx + 1, depth);
}

int min_subset_diff(int *arr, int n, int idx, int sum1, int sum2) {
    if (idx == n) {
        int diff = sum1 - sum2;
        if (diff < 0) diff = -diff;
        return diff;
    }
    int d1 = min_subset_diff(arr, n, idx + 1, sum1 + arr[idx], sum2);
    int d2 = min_subset_diff(arr, n, idx + 1, sum1, sum2 + arr[idx]);
    if (d1 < d2) return d1;
    return d2;
}

int can_partition_equal(int *arr, int n) {
    int total = 0;
    int i = 0;
    while (i < n) {
        total = total + arr[i];
        i = i + 1;
    }
    if ((total / 2) * 2 != total) return 0;
    return has_subset_sum(arr, n, total / 2);
}

int knapsack(int *weights, int *values, int n, int capacity) {
    if (n == 0 || capacity == 0) return 0;
    if (weights[n - 1] > capacity)
        return knapsack(weights, values, n - 1, capacity);
    int take = values[n - 1] + knapsack(weights, values, n - 1, capacity - weights[n - 1]);
    int skip = knapsack(weights, values, n - 1, capacity);
    if (take > skip) return take;
    return skip;
}

int main(void) {
    int arr[5];
    arr[0] = 3; arr[1] = 7; arr[2] = 1; arr[3] = 8; arr[4] = 2;

    printf("has_sum(11)=%d\n", has_subset_sum(arr, 5, 11));
    printf("has_sum(14)=%d\n", has_subset_sum(arr, 5, 14));
    printf("has_sum(22)=%d\n", has_subset_sum(arr, 5, 22));

    printf("count_sum(10)=%d\n", count_subset_sums(arr, 5, 10));
    printf("count_sum(11)=%d\n", count_subset_sums(arr, 5, 11));

    printf("subsets summing to 10:\n");
    found_count = 0;
    find_subsets(arr, 5, 10, 0, 0);
    printf("found: %d\n", found_count);

    printf("subsets summing to 11:\n");
    found_count = 0;
    find_subsets(arr, 5, 11, 0, 0);
    printf("found: %d\n", found_count);

    printf("min_diff=%d\n", min_subset_diff(arr, 5, 0, 0, 0));

    int eq1[4];
    eq1[0] = 1; eq1[1] = 5; eq1[2] = 11; eq1[3] = 5;
    printf("equal_partition([1,5,11,5])=%d\n", can_partition_equal(eq1, 4));

    int eq2[3];
    eq2[0] = 1; eq2[1] = 2; eq2[2] = 3;
    printf("equal_partition([1,2,3])=%d\n", can_partition_equal(eq2, 3));

    int w[4]; w[0] = 2; w[1] = 3; w[2] = 4; w[3] = 5;
    int v[4]; v[0] = 3; v[1] = 4; v[2] = 5; v[3] = 6;
    printf("knapsack(cap=5)=%d\n", knapsack(w, v, 4, 5));
    printf("knapsack(cap=10)=%d\n", knapsack(w, v, 4, 10));

    return 0;
}
