int printf(const char *fmt, ...);
// EXPECT: array: 2 1 5 1 3 2 8 1 3 6\nmax sum (k=3): 13\nmin sum (k=3): 6\nmax sum (k=4): 18\nmax sum (k=5): 20\nwindows >= 10 (k=3): 4\nwindows >= 10 (k=4): 6\nmoving avg (k=3): 2 2 3 2 4 3 4 3\nwindow sums (k=2): 3 6 6 4 5 10 9 4 9 
// Test: sliding window techniques

void print_arr(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

// Maximum sum subarray of size k
int max_sum_window(int *arr, int n, int k) {
    int sum = 0;
    int i;
    for (i = 0; i < k; i++) sum = sum + arr[i];
    int max = sum;
    for (i = k; i < n; i++) {
        sum = sum + arr[i] - arr[i - k];
        if (sum > max) max = sum;
    }
    return max;
}

// Minimum sum subarray of size k
int min_sum_window(int *arr, int n, int k) {
    int sum = 0;
    int i;
    for (i = 0; i < k; i++) sum = sum + arr[i];
    int min = sum;
    for (i = k; i < n; i++) {
        sum = sum + arr[i] - arr[i - k];
        if (sum < min) min = sum;
    }
    return min;
}

// Count windows with sum >= threshold
int count_windows_ge(int *arr, int n, int k, int threshold) {
    int sum = 0;
    int i;
    for (i = 0; i < k; i++) sum = sum + arr[i];
    int count = 0;
    if (sum >= threshold) count = count + 1;
    for (i = k; i < n; i++) {
        sum = sum + arr[i] - arr[i - k];
        if (sum >= threshold) count = count + 1;
    }
    return count;
}

// Moving average (integer)
void moving_avg(int *arr, int n, int k, int *out) {
    int sum = 0;
    int i;
    for (i = 0; i < k; i++) sum = sum + arr[i];
    out[0] = sum / k;
    for (i = k; i < n; i++) {
        sum = sum + arr[i] - arr[i - k];
        out[i - k + 1] = sum / k;
    }
}

int main(void) {
    int arr[10];
    arr[0]=2; arr[1]=1; arr[2]=5; arr[3]=1; arr[4]=3;
    arr[5]=2; arr[6]=8; arr[7]=1; arr[8]=3; arr[9]=6;

    printf("array: ");
    print_arr(arr, 10);

    // Window size 3
    printf("max sum (k=3): %d\n", max_sum_window(arr, 10, 3));
    printf("min sum (k=3): %d\n", min_sum_window(arr, 10, 3));

    // Window size 4
    printf("max sum (k=4): %d\n", max_sum_window(arr, 10, 4));

    // Window size 5
    printf("max sum (k=5): %d\n", max_sum_window(arr, 10, 5));

    // Count windows with sum >= 10
    printf("windows >= 10 (k=3): %d\n", count_windows_ge(arr, 10, 3, 10));
    printf("windows >= 10 (k=4): %d\n", count_windows_ge(arr, 10, 4, 10));

    // Moving average
    int avg[8];
    moving_avg(arr, 10, 3, avg);
    printf("moving avg (k=3): ");
    print_arr(avg, 8);

    // All windows of size 2 sums
    printf("window sums (k=2): ");
    int i;
    for (i = 0; i < 9; i++) {
        printf("%d ", arr[i] + arr[i + 1]);
    }
    printf("\n");

    return 0;
}
