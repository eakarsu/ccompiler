int printf(const char *fmt, ...);

// Maximum Subarray Sum - Kadane's algorithm and variants

// Standard Kadane: find maximum contiguous subarray sum
int max_subarray(int *arr, int n) {
    int max_ending;
    int max_so_far;
    int i;

    max_ending = arr[0];
    max_so_far = arr[0];

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

// Return the start and end indices of max subarray via pointers
int max_subarray_range(int *arr, int n, int *start, int *end) {
    int max_ending;
    int max_so_far;
    int temp_start;
    int i;

    max_ending = arr[0];
    max_so_far = arr[0];
    temp_start = 0;
    *start = 0;
    *end = 0;

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

// Maximum circular subarray sum
// = max(kadane, total_sum - min_subarray)
// unless all negative, return kadane result
int min_subarray(int *arr, int n) {
    int min_ending;
    int min_so_far;
    int i;

    min_ending = arr[0];
    min_so_far = arr[0];

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

int max_circular_subarray(int *arr, int n) {
    int kadane_max;
    int total;
    int min_sub;
    int circular;
    int i;
    int all_neg;

    kadane_max = max_subarray(arr, n);
    total = 0;
    all_neg = 1;
    for (i = 0; i < n; i++) {
        total = total + arr[i];
        if (arr[i] > 0) all_neg = 0;
    }

    if (all_neg) return kadane_max;

    min_sub = min_subarray(arr, n);
    circular = total - min_sub;

    if (circular > kadane_max) return circular;
    return kadane_max;
}

// Maximum sum of non-adjacent elements
int max_non_adjacent(int *arr, int n) {
    int incl;
    int excl;
    int new_excl;
    int i;

    if (n == 0) return 0;
    if (n == 1) return arr[0];

    incl = arr[0];
    excl = 0;

    for (i = 1; i < n; i++) {
        if (incl > excl) {
            new_excl = incl;
        } else {
            new_excl = excl;
        }
        incl = excl + arr[i];
        excl = new_excl;
    }

    if (incl > excl) return incl;
    return excl;
}

// Maximum product subarray (using only positive/negative ints)
int max_product_subarray(int *arr, int n) {
    int max_prod;
    int min_prod;
    int result;
    int temp_max;
    int temp_min;
    int i;

    max_prod = arr[0];
    min_prod = arr[0];
    result = arr[0];

    for (i = 1; i < n; i++) {
        // Compute candidates
        int a;
        int b;
        int c;
        a = arr[i];
        b = max_prod * arr[i];
        c = min_prod * arr[i];

        // new max
        temp_max = a;
        if (b > temp_max) temp_max = b;
        if (c > temp_max) temp_max = c;

        // new min
        temp_min = a;
        if (b < temp_min) temp_min = b;
        if (c < temp_min) temp_min = c;

        max_prod = temp_max;
        min_prod = temp_min;

        if (max_prod > result) result = max_prod;
    }

    return result;
}

int main() {
    int arr1[9];
    int arr2[5];
    int arr3[4];
    int arr4[6];
    int start;
    int end;
    int result;

    // Standard Kadane: {-2, 1, -3, 4, -1, 2, 1, -5, 4}
    // Max subarray: [4,-1,2,1] = 6
    arr1[0]=-2; arr1[1]=1; arr1[2]=-3; arr1[3]=4; arr1[4]=-1;
    arr1[5]=2; arr1[6]=1; arr1[7]=-5; arr1[8]=4;

    printf("%d\n", max_subarray(arr1, 9));
    // EXPECT: 6

    // With range: should be [3..6]
    result = max_subarray_range(arr1, 9, &start, &end);
    printf("%d %d %d\n", result, start, end);
    // EXPECT: 6 3 6

    // All negative: {-3, -5, -1, -8}
    arr3[0]=-3; arr3[1]=-5; arr3[2]=-1; arr3[3]=-8;
    printf("%d\n", max_subarray(arr3, 4));
    // EXPECT: -1

    // All positive: {2, 3, 4, 1}
    arr3[0]=2; arr3[1]=3; arr3[2]=4; arr3[3]=1;
    printf("%d\n", max_subarray(arr3, 4));
    // EXPECT: 10

    // Single element
    arr3[0] = 42;
    printf("%d\n", max_subarray(arr3, 1));
    // EXPECT: 42

    arr3[0] = -7;
    printf("%d\n", max_subarray(arr3, 1));
    // EXPECT: -7

    // Circular: {5, -3, 5} => normal max=5+(-3)+5=7 or circular=5+5=10
    arr2[0]=5; arr2[1]=-3; arr2[2]=5;
    printf("%d\n", max_circular_subarray(arr2, 3));
    // EXPECT: 10

    // Circular: {-2, 4, -5, 4, -5, 9, 4}
    // Normal kadane: [9,4]=13, total=9, min_sub=-12, circ=9-(-12)=21? no
    // Let me recompute: total = -2+4-5+4-5+9+4 = 9
    // min_subarray: -5+4-5 = -6 ? or just -5+(-5)=-10? let's see
    // min_ending: -2, then min(-2+4,4)=2 so reset to 2? no min is smaller
    // min_ending[0]=-2, min_ending[1]=min(-2+4,4)=2, so min_ending=2
    // min_ending[2]=min(2-5,-5)=-5, min_ending=-5
    // min_ending[3]=min(-5+4,4)=-1, min_ending=-1
    // min_ending[4]=min(-1-5,-5)=-6, min_ending=-6
    // min_ending[5]=min(-6+9,9)=3, min_ending=3
    // min_ending[6]=min(3+4,4)=4, min_ending=4
    // min_so_far = -6
    // circular = 9-(-6) = 15
    // kadane: max subarray = 9+4 = 13 (or 4+(-5)+9+4=12, 4-5+4-5+9+4=11)
    // Actually let me trace kadane more carefully:
    // max_ending[0]=-2, max_so_far=-2
    // max_ending[1]=max(-2+4,4)=4, max_so_far=4
    // max_ending[2]=max(4-5,-5)=-1, max_so_far=4
    // max_ending[3]=max(-1+4,4)=4, max_so_far=4
    // max_ending[4]=max(4-5,-5)=-1, max_so_far=4
    // max_ending[5]=max(-1+9,9)=9, max_so_far=9
    // max_ending[6]=max(9+4,4)=13, max_so_far=13
    // So kadane=13, circular=15, answer=15
    int arr5[7];
    arr5[0]=-2; arr5[1]=4; arr5[2]=-5; arr5[3]=4; arr5[4]=-5; arr5[5]=9; arr5[6]=4;
    printf("%d\n", max_circular_subarray(arr5, 7));
    // EXPECT: 15

    // Non-adjacent: {3, 2, 7, 10} => 3+7=10 or 3+10=13 or 2+10=12 => 13
    arr3[0]=3; arr3[1]=2; arr3[2]=7; arr3[3]=10;
    printf("%d\n", max_non_adjacent(arr3, 4));
    // EXPECT: 13

    // Non-adjacent: {5, 5, 10, 100, 10, 5}
    // 5+10+10=25, 5+100+5=110, 5+10+5=20 => 110
    arr4[0]=5; arr4[1]=5; arr4[2]=10; arr4[3]=100; arr4[4]=10; arr4[5]=5;
    printf("%d\n", max_non_adjacent(arr4, 6));
    // EXPECT: 110

    // Non-adjacent: {1, 2, 3} => 1+3=4
    arr3[0]=1; arr3[1]=2; arr3[2]=3;
    printf("%d\n", max_non_adjacent(arr3, 3));
    // EXPECT: 4

    // Product subarray: {2, 3, -2, 4} => max product = 2*3=6
    arr3[0]=2; arr3[1]=3; arr3[2]=-2; arr3[3]=4;
    printf("%d\n", max_product_subarray(arr3, 4));
    // EXPECT: 6

    // Product: {-2, 0, -1} => max product = 0
    arr3[0]=-2; arr3[1]=0; arr3[2]=-1;
    printf("%d\n", max_product_subarray(arr3, 3));
    // EXPECT: 0

    // Product: {-2, 3, -4} => (-2)*3*(-4)=24
    arr3[0]=-2; arr3[1]=3; arr3[2]=-4;
    printf("%d\n", max_product_subarray(arr3, 3));
    // EXPECT: 24

    printf("done\n");
    // EXPECT: done

    return 0;
}
