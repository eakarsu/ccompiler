int printf(const char *fmt, ...);
// EXPECT: array: -2 1 -3 4 -1 2 1 -5 4\nmax subarray sum: 6\nmax range: [3,6] sum=6\nall positive: 15\nall negative: -1\nsingle: 42\nmin subarray: -5\narray2: 5 -4 3 -2 1 6 -3 2\nmax range: [0,5] sum=9
// Test: Kadane's algorithm (maximum subarray sum)

void print_arr(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

// Basic Kadane's
int max_subarray_sum(int *arr, int n) {
    int max_ending = arr[0];
    int max_so_far = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        if (max_ending + arr[i] > arr[i]) {
            max_ending = max_ending + arr[i];
        } else {
            max_ending = arr[i];
        }
        if (max_ending > max_so_far) {
            max_so_far = max_ending;
        }
    }
    return max_so_far;
}

// Kadane's with indices
int max_subarray_range(int *arr, int n, int *start, int *end) {
    int max_ending = arr[0];
    int max_so_far = arr[0];
    int temp_start = 0;
    *start = 0;
    *end = 0;
    int i;
    for (i = 1; i < n; i++) {
        if (max_ending + arr[i] > arr[i]) {
            max_ending = max_ending + arr[i];
        } else {
            max_ending = arr[i];
            temp_start = i;
        }
        if (max_ending > max_so_far) {
            max_so_far = max_ending;
            *start = temp_start;
            *end = i;
        }
    }
    return max_so_far;
}

// Minimum subarray sum (inverted Kadane's)
int min_subarray_sum(int *arr, int n) {
    int min_ending = arr[0];
    int min_so_far = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        if (min_ending + arr[i] < arr[i]) {
            min_ending = min_ending + arr[i];
        } else {
            min_ending = arr[i];
        }
        if (min_ending < min_so_far) {
            min_so_far = min_ending;
        }
    }
    return min_so_far;
}

int main(void) {
    // Classic example
    int a1[9];
    a1[0]=-2; a1[1]=1; a1[2]=-3; a1[3]=4; a1[4]=-1;
    a1[5]=2; a1[6]=1; a1[7]=-5; a1[8]=4;
    printf("array: ");
    print_arr(a1, 9);
    printf("max subarray sum: %d\n", max_subarray_sum(a1, 9));

    int s; int e;
    int sum = max_subarray_range(a1, 9, &s, &e);
    printf("max range: [%d,%d] sum=%d\n", s, e, sum);

    // All positive
    int a2[5];
    a2[0]=1; a2[1]=2; a2[2]=3; a2[3]=4; a2[4]=5;
    printf("all positive: %d\n", max_subarray_sum(a2, 5));

    // All negative
    int a3[4];
    a3[0]=-5; a3[1]=-3; a3[2]=-1; a3[3]=-4;
    printf("all negative: %d\n", max_subarray_sum(a3, 4));

    // Single element
    int a4[1];
    a4[0] = 42;
    printf("single: %d\n", max_subarray_sum(a4, 1));

    // Min subarray
    printf("min subarray: %d\n", min_subarray_sum(a1, 9));

    // Another example
    int a5[8];
    a5[0]=5; a5[1]=-4; a5[2]=3; a5[3]=-2;
    a5[4]=1; a5[5]=6; a5[6]=-3; a5[7]=2;
    printf("array2: ");
    print_arr(a5, 8);
    sum = max_subarray_range(a5, 8, &s, &e);
    printf("max range: [%d,%d] sum=%d\n", s, e, sum);

    return 0;
}
