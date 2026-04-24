int printf(const char *fmt, ...);
// EXPECT: Max subarray: sum=6, [3..6]\nMax subarray: sum=9, [0..4]\nAll negative: sum=-1\nCircular: 10\nCircular: 15\nCircular all neg: -1
// Test: Maximum subarray (Kadane's algorithm)

typedef struct {
    int sum;
    int start;
    int end;
} SubarrayResult;

SubarrayResult max_subarray(int arr[], int n) {
    SubarrayResult res;
    int max_sum = arr[0];
    int curr_sum = arr[0];
    int start = 0, end = 0, temp_start = 0;
    int i;

    for (i = 1; i < n; i++) {
        if (curr_sum + arr[i] < arr[i]) {
            curr_sum = arr[i];
            temp_start = i;
        } else {
            curr_sum = curr_sum + arr[i];
        }
        if (curr_sum > max_sum) {
            max_sum = curr_sum;
            start = temp_start;
            end = i;
        }
    }

    res.sum = max_sum;
    res.start = start;
    res.end = end;
    return res;
}

int max_circular_subarray(int arr[], int n) {
    int total = 0;
    int max_sum = arr[0];
    int min_sum = arr[0];
    int curr_max = arr[0];
    int curr_min = arr[0];
    int i;
    int all_neg = 1;

    total = arr[0];
    for (i = 1; i < n; i++) {
        total = total + arr[i];
    }
    if (arr[0] > 0) all_neg = 0;

    for (i = 1; i < n; i++) {
        if (arr[i] > 0) all_neg = 0;
        if (curr_max + arr[i] > arr[i])
            curr_max = curr_max + arr[i];
        else
            curr_max = arr[i];
        if (curr_max > max_sum) max_sum = curr_max;

        if (curr_min + arr[i] < arr[i])
            curr_min = curr_min + arr[i];
        else
            curr_min = arr[i];
        if (curr_min < min_sum) min_sum = curr_min;
    }

    if (all_neg) return max_sum;
    int wrap = total - min_sum;
    if (wrap > max_sum) return wrap;
    return max_sum;
}

int main(void) {
    int a[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    int n = 9;
    SubarrayResult r = max_subarray(a, n);
    printf("Max subarray: sum=%d, [%d..%d]\n", r.sum, r.start, r.end);

    int b[] = {1, 2, 3, -2, 5};
    r = max_subarray(b, 5);
    printf("Max subarray: sum=%d, [%d..%d]\n", r.sum, r.start, r.end);

    int c[] = {-1, -2, -3, -4};
    r = max_subarray(c, 4);
    printf("All negative: sum=%d\n", r.sum);

    int d[] = {5, -3, 5};
    printf("Circular: %d\n", max_circular_subarray(d, 3));

    int e[] = {8, -1, 3, 4};
    printf("Circular: %d\n", max_circular_subarray(e, 4));

    int f[] = {-3, -1, -2};
    printf("Circular all neg: %d\n", max_circular_subarray(f, 3));

    return 0;
}
