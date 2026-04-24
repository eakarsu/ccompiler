int printf(const char *fmt, ...);
// EXPECT: pair sum 16: 1 + 15\npair sum 10: 1 + 9\npair sum 2: not found\nbefore remove 3: 3 2 3 4 3 5\nafter remove 3: 2 4 5\nbefore move zeros: 0 1 0 3 0 5 6\nafter move zeros: 1 3 5 6 0 0 0\nmax water: 16\nmax water (flat): 4
// Test: two-pointer technique

void print_arr(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

// Find pair with given sum in sorted array
int find_pair_sum(int *arr, int n, int target, int *a, int *b) {
    int lo = 0;
    int hi = n - 1;
    while (lo < hi) {
        int s = arr[lo] + arr[hi];
        if (s == target) {
            *a = arr[lo];
            *b = arr[hi];
            return 1;
        }
        if (s < target) lo = lo + 1;
        else hi = hi - 1;
    }
    return 0;
}

// Remove element in-place, return new length
int remove_element(int *arr, int n, int val) {
    int write = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] != val) {
            arr[write] = arr[i];
            write = write + 1;
        }
    }
    return write;
}

// Move zeros to end
void move_zeros(int *arr, int n) {
    int write = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] != 0) {
            arr[write] = arr[i];
            write = write + 1;
        }
    }
    while (write < n) {
        arr[write] = 0;
        write = write + 1;
    }
}

// Container with most water (simplified)
int max_water(int *heights, int n) {
    int lo = 0;
    int hi = n - 1;
    int max_area = 0;
    while (lo < hi) {
        int h = heights[lo];
        if (heights[hi] < h) h = heights[hi];
        int area = h * (hi - lo);
        if (area > max_area) max_area = area;
        if (heights[lo] < heights[hi]) lo = lo + 1;
        else hi = hi - 1;
    }
    return max_area;
}

int main(void) {
    // Find pair with sum
    int sorted[8];
    sorted[0]=1; sorted[1]=3; sorted[2]=5; sorted[3]=7;
    sorted[4]=9; sorted[5]=11; sorted[6]=13; sorted[7]=15;

    int a; int b;
    if (find_pair_sum(sorted, 8, 16, &a, &b)) {
        printf("pair sum 16: %d + %d\n", a, b);
    }
    if (find_pair_sum(sorted, 8, 10, &a, &b)) {
        printf("pair sum 10: %d + %d\n", a, b);
    }
    if (!find_pair_sum(sorted, 8, 2, &a, &b)) {
        printf("pair sum 2: not found\n");
    }

    // Remove element
    int arr1[6];
    arr1[0]=3; arr1[1]=2; arr1[2]=3; arr1[3]=4; arr1[4]=3; arr1[5]=5;
    printf("before remove 3: ");
    print_arr(arr1, 6);
    int newn = remove_element(arr1, 6, 3);
    printf("after remove 3: ");
    print_arr(arr1, newn);

    // Move zeros
    int arr2[7];
    arr2[0]=0; arr2[1]=1; arr2[2]=0; arr2[3]=3; arr2[4]=0; arr2[5]=5; arr2[6]=6;
    printf("before move zeros: ");
    print_arr(arr2, 7);
    move_zeros(arr2, 7);
    printf("after move zeros: ");
    print_arr(arr2, 7);

    // Max water
    int heights[6];
    heights[0]=1; heights[1]=8; heights[2]=6; heights[3]=2; heights[4]=5; heights[5]=4;
    printf("max water: %d\n", max_water(heights, 6));

    int h2[5];
    h2[0]=1; h2[1]=1; h2[2]=1; h2[3]=1; h2[4]=1;
    printf("max water (flat): %d\n", max_water(h2, 5));

    return 0;
}
