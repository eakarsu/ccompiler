int printf(const char *fmt, ...);
// EXPECT: === Dutch National Flag Tests ===\nDNF1 before: 0s=4 1s=4 2s=4\nDNF1 result: 0 0 0 0 1 1 1 1 2 2 2 2\nDNF1 valid: 1\nDNF2 all_same: 1 1 1 1 1\nDNF2 valid: 1\nDNF3 sorted: 0 0 1 1 2 2\nDNF4 reverse: 0 0 1 1 2 2\nDNF5 3way (pivot=3): 2 1 3 3 3 9 7 5\n=== Kadane's Algorithm Tests ===\nKadane1: 6\nKadane1 indices: start=3 end=6 sum=6\nKadane2 all_neg: -1\nKadane3 all_pos: 10\nKadane4 circular: 22\nKadane5 circ_neg: -1\nKadane6 max_prod: 48\nKadane7 max_prod: 24\n=== Array Rotation Tests ===\nRot1 left2: 3 4 5 6 7 1 2\nRot2 right3: 5 6 7 1 2 3 4\nRot3 is_rotation: 1\nRot3 distance: 2\nRot4 not_rotation: 0\nRot5 full: 10 20 30 40\nRot6 zero: 7 8 9\n=== Next Permutation Tests ===\nNextPerm1: 1 3 2\nNextPerm2: 2 1 3\nNextPerm3: 2 3 1\nNextPerm4: 3 1 2\nNextPerm5: 3 2 1\nNextPerm6 wrapped: 0 result: 1 2 3\nPermsToSorted [2,3,1]: 3\nPrevPerm [2,3,1]: 2 1 3\nNextPerm4el: 1 2 4 3\n=== Leaders Tests ===\nLeaders1 count: 3\nLeaders1: 17 5 2\nLeaders2 count: 5\nLeaders2: 5 4 3 2 1\nLeaders3 count: 1\nLeaders3: 5\nLeaders4 count: 1\n=== Additional Puzzle Tests ===\nMajority1: candidate=2 is_majority=1\nMajority2: is_majority=0\nEquilibrium1: 3\nEquilibrium2: -1\nEquilibrium3: 0\nTrapWater1: 6\nTrapWater2: 7\nStock1 single: 5\nStock1 unlimited: 7\nStock2 single: 0\nStock2 unlimited: 0\nStock3 single: 5\nStock3 unlimited: 5
// ============================================================
// test_090_array_puzzle.c
// Array-based puzzles and algorithms:
//   1. Dutch National Flag problem (3-way partitioning)
//   2. Maximum subarray (Kadane's algorithm)
//   3. Array rotation
//   4. Next permutation
//   5. Leaders in array
//   6. Additional array puzzles
// ============================================================

// ---- Utility functions ----

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void print_array(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(" ");
    }
    printf("\n");
}

void copy_array(int *src, int *dst, int n) {
    int i;
    for (i = 0; i < n; i++) {
        dst[i] = src[i];
    }
}

int arrays_equal(int *a, int *b, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int array_sum(int *arr, int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i++) {
        s = s + arr[i];
    }
    return s;
}

int array_min(int *arr, int n) {
    int m = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] < m) m = arr[i];
    }
    return m;
}

int array_max(int *arr, int n) {
    int m = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] > m) m = arr[i];
    }
    return m;
}

// ============================================================
// 1. Dutch National Flag Problem
// ============================================================
// Given an array of 0s, 1s, and 2s, sort them in-place
// Uses three-way partitioning (Dijkstra's algorithm)

void dutch_national_flag(int *arr, int n) {
    int low = 0;
    int mid = 0;
    int high = n - 1;

    while (mid <= high) {
        if (arr[mid] == 0) {
            swap(&arr[low], &arr[mid]);
            low++;
            mid++;
        } else if (arr[mid] == 1) {
            mid++;
        } else {
            // arr[mid] == 2
            swap(&arr[mid], &arr[high]);
            high--;
        }
    }
}

// Verify DNF result: all 0s, then all 1s, then all 2s
int verify_dnf(int *arr, int n) {
    int i;
    int phase = 0; // 0=expecting 0s, 1=expecting 1s, 2=expecting 2s
    for (i = 0; i < n; i++) {
        if (arr[i] < phase) return 0; // Out of order
        phase = arr[i];
    }
    return 1;
}

// Count occurrences of each value
void count_values(int *arr, int n, int *count0, int *count1, int *count2) {
    *count0 = 0;
    *count1 = 0;
    *count2 = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] == 0) (*count0)++;
        else if (arr[i] == 1) (*count1)++;
        else if (arr[i] == 2) (*count2)++;
    }
}

// Generalized 3-way partition around a pivot
void three_way_partition(int *arr, int n, int pivot) {
    int low = 0;
    int mid = 0;
    int high = n - 1;

    while (mid <= high) {
        if (arr[mid] < pivot) {
            swap(&arr[low], &arr[mid]);
            low++;
            mid++;
        } else if (arr[mid] == pivot) {
            mid++;
        } else {
            swap(&arr[mid], &arr[high]);
            high--;
        }
    }
}

// ============================================================
// 2. Maximum Subarray (Kadane's Algorithm)
// ============================================================

// Find maximum subarray sum
int kadane_max_sum(int *arr, int n) {
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

// Find maximum subarray sum with start and end indices
int kadane_with_indices(int *arr, int n, int *start, int *end) {
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

// Maximum circular subarray sum
int max_circular_subarray(int *arr, int n) {
    // Method: max(kadane_max, total_sum - kadane_min)
    int total = 0;
    int max_ending = arr[0];
    int max_sum = arr[0];
    int min_ending = arr[0];
    int min_sum = arr[0];
    int i;

    total = arr[0];
    for (i = 1; i < n; i++) {
        total = total + arr[i];

        // Kadane max
        if (max_ending + arr[i] > arr[i]) {
            max_ending = max_ending + arr[i];
        } else {
            max_ending = arr[i];
        }
        if (max_ending > max_sum) max_sum = max_ending;

        // Kadane min
        if (min_ending + arr[i] < arr[i]) {
            min_ending = min_ending + arr[i];
        } else {
            min_ending = arr[i];
        }
        if (min_ending < min_sum) min_sum = min_ending;
    }

    // If all elements are negative, return max_sum
    if (total == min_sum) return max_sum;

    int circular = total - min_sum;
    if (circular > max_sum) return circular;
    return max_sum;
}

// Maximum product subarray
int max_product_subarray(int *arr, int n) {
    int max_prod = arr[0];
    int min_prod = arr[0];
    int result = arr[0];
    int i;

    for (i = 1; i < n; i++) {
        if (arr[i] < 0) {
            // Swap max and min when multiplying by negative
            int tmp = max_prod;
            max_prod = min_prod;
            min_prod = tmp;
        }

        // max_prod = max(arr[i], max_prod * arr[i])
        if (max_prod * arr[i] > arr[i]) {
            max_prod = max_prod * arr[i];
        } else {
            max_prod = arr[i];
        }

        // min_prod = min(arr[i], min_prod * arr[i])
        if (min_prod * arr[i] < arr[i]) {
            min_prod = min_prod * arr[i];
        } else {
            min_prod = arr[i];
        }

        if (max_prod > result) result = max_prod;
    }
    return result;
}

// ============================================================
// 3. Array Rotation
// ============================================================

// Reverse a portion of array from index l to r
void reverse_section(int *arr, int l, int r) {
    while (l < r) {
        swap(&arr[l], &arr[r]);
        l++;
        r--;
    }
}

// Left rotate array by d positions using reversal algorithm
void rotate_left(int *arr, int n, int d) {
    if (n == 0) return;
    d = d % n;
    if (d == 0) return;

    // Reverse first d elements
    reverse_section(arr, 0, d - 1);
    // Reverse remaining elements
    reverse_section(arr, d, n - 1);
    // Reverse whole array
    reverse_section(arr, 0, n - 1);
}

// Right rotate array by d positions
void rotate_right(int *arr, int n, int d) {
    if (n == 0) return;
    d = d % n;
    if (d == 0) return;
    rotate_left(arr, n, n - d);
}

// Check if one array is a rotation of another
int is_rotation_of(int *a, int *b, int n) {
    // Find b[0] in a
    int i;
    int j;
    for (i = 0; i < n; i++) {
        if (a[i] == b[0]) {
            // Check if rotation starting at i matches b
            int match = 1;
            for (j = 0; j < n; j++) {
                if (a[(i + j) % n] != b[j]) {
                    match = 0;
                    break;
                }
            }
            if (match) return 1;
        }
    }
    return 0;
}

// Find the rotation count to get from a to b
int rotation_distance(int *a, int *b, int n) {
    int i;
    int j;
    for (i = 0; i < n; i++) {
        int match = 1;
        for (j = 0; j < n; j++) {
            if (a[(j + i) % n] != b[j]) {
                match = 0;
                break;
            }
        }
        if (match) return i;
    }
    return -1;
}

// ============================================================
// 4. Next Permutation
// ============================================================

// Find the next lexicographically greater permutation
// Returns 1 if next permutation exists, 0 if already the last
int next_permutation(int *arr, int n) {
    // Step 1: Find largest i such that arr[i] < arr[i+1]
    int i = n - 2;
    while (i >= 0 && arr[i] >= arr[i + 1]) {
        i--;
    }

    if (i < 0) {
        // Last permutation, reverse to get first
        reverse_section(arr, 0, n - 1);
        return 0;
    }

    // Step 2: Find largest j > i such that arr[j] > arr[i]
    int j = n - 1;
    while (arr[j] <= arr[i]) {
        j--;
    }

    // Step 3: Swap arr[i] and arr[j]
    swap(&arr[i], &arr[j]);

    // Step 4: Reverse the suffix starting at arr[i+1]
    reverse_section(arr, i + 1, n - 1);

    return 1;
}

// Find the previous permutation
int prev_permutation(int *arr, int n) {
    int i = n - 2;
    while (i >= 0 && arr[i] <= arr[i + 1]) {
        i--;
    }

    if (i < 0) {
        reverse_section(arr, 0, n - 1);
        return 0;
    }

    int j = n - 1;
    while (arr[j] >= arr[i]) {
        j--;
    }

    swap(&arr[i], &arr[j]);
    reverse_section(arr, i + 1, n - 1);

    return 1;
}

// Count permutations from current to sorted (ascending)
int count_permutations_to_sorted(int *arr, int n) {
    int work[20];
    copy_array(arr, work, n);
    int count = 0;
    int max_iter = 200; // Safety limit
    // Check if already sorted
    int sorted = 1;
    int i;
    for (i = 0; i < n - 1; i++) {
        if (work[i] > work[i + 1]) { sorted = 0; break; }
    }
    if (sorted) return 0;

    while (count < max_iter) {
        next_permutation(work, n);
        count++;
        // Check if sorted
        sorted = 1;
        for (i = 0; i < n - 1; i++) {
            if (work[i] > work[i + 1]) { sorted = 0; break; }
        }
        if (sorted) return count;
    }
    return -1;
}

// ============================================================
// 5. Leaders in Array
// ============================================================
// An element is a "leader" if no element to its right is greater

// Find all leaders and store in output array
int find_leaders(int *arr, int n, int *leaders) {
    int count = 0;
    // Rightmost element is always a leader
    int max_from_right = arr[n - 1];
    leaders[count] = arr[n - 1];
    count++;

    int i;
    for (i = n - 2; i >= 0; i--) {
        if (arr[i] > max_from_right) {
            leaders[count] = arr[i];
            count++;
            max_from_right = arr[i];
        }
    }

    // Reverse leaders array (they were found right-to-left)
    int l = 0;
    int r = count - 1;
    while (l < r) {
        swap(&leaders[l], &leaders[r]);
        l++;
        r--;
    }

    return count;
}

// ============================================================
// 6. Additional Array Puzzles
// ============================================================

// Majority element (Boyer-Moore voting algorithm)
// Returns the candidate; caller should verify
int majority_candidate(int *arr, int n) {
    int candidate = arr[0];
    int count = 1;
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] == candidate) {
            count++;
        } else {
            count--;
            if (count == 0) {
                candidate = arr[i];
                count = 1;
            }
        }
    }
    return candidate;
}

// Verify if candidate is actually majority
int is_majority(int *arr, int n, int candidate) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] == candidate) count++;
    }
    return count > n / 2;
}

// Equilibrium index (sum of left == sum of right)
int equilibrium_index(int *arr, int n) {
    int total = array_sum(arr, n);
    int left_sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        int right_sum = total - left_sum - arr[i];
        if (left_sum == right_sum) return i;
        left_sum = left_sum + arr[i];
    }
    return -1;
}

// Count all equilibrium indices
int count_equilibrium(int *arr, int n) {
    int total = array_sum(arr, n);
    int left_sum = 0;
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        int right_sum = total - left_sum - arr[i];
        if (left_sum == right_sum) count++;
        left_sum = left_sum + arr[i];
    }
    return count;
}

// Trapping rain water
int trap_water(int *height, int n) {
    if (n < 3) return 0;

    int left_max[50];
    int right_max[50];
    int i;

    // Compute left max for each position
    left_max[0] = height[0];
    for (i = 1; i < n; i++) {
        if (height[i] > left_max[i - 1]) {
            left_max[i] = height[i];
        } else {
            left_max[i] = left_max[i - 1];
        }
    }

    // Compute right max for each position
    right_max[n - 1] = height[n - 1];
    for (i = n - 2; i >= 0; i--) {
        if (height[i] > right_max[i + 1]) {
            right_max[i] = height[i];
        } else {
            right_max[i] = right_max[i + 1];
        }
    }

    // Calculate trapped water
    int water = 0;
    for (i = 0; i < n; i++) {
        int min_h;
        if (left_max[i] < right_max[i]) {
            min_h = left_max[i];
        } else {
            min_h = right_max[i];
        }
        water = water + min_h - height[i];
    }
    return water;
}

// Stock buy and sell (max profit with single transaction)
int max_profit_single(int *prices, int n) {
    if (n < 2) return 0;
    int min_price = prices[0];
    int max_profit = 0;
    int i;
    for (i = 1; i < n; i++) {
        if (prices[i] - min_price > max_profit) {
            max_profit = prices[i] - min_price;
        }
        if (prices[i] < min_price) {
            min_price = prices[i];
        }
    }
    return max_profit;
}

// Stock buy and sell (max profit with unlimited transactions)
int max_profit_unlimited(int *prices, int n) {
    int profit = 0;
    int i;
    for (i = 1; i < n; i++) {
        if (prices[i] > prices[i - 1]) {
            profit = profit + prices[i] - prices[i - 1];
        }
    }
    return profit;
}

// ============================================================
// Test functions
// ============================================================

void test_dutch_flag() {
    printf("=== Dutch National Flag Tests ===\n");

    // Test 1
    int arr1[12];
    arr1[0]=2; arr1[1]=0; arr1[2]=1; arr1[3]=2; arr1[4]=1; arr1[5]=0;
    arr1[6]=0; arr1[7]=2; arr1[8]=1; arr1[9]=0; arr1[10]=1; arr1[11]=2;

    int c0;
    int c1;
    int c2;
    count_values(arr1, 12, &c0, &c1, &c2);
    printf("DNF1 before: 0s=%d 1s=%d 2s=%d\n", c0, c1, c2);

    dutch_national_flag(arr1, 12);
    printf("DNF1 result: ");
    print_array(arr1, 12);
    printf("DNF1 valid: %d\n", verify_dnf(arr1, 12));

    // Test 2: All same
    int arr2[5];
    arr2[0]=1; arr2[1]=1; arr2[2]=1; arr2[3]=1; arr2[4]=1;
    dutch_national_flag(arr2, 5);
    printf("DNF2 all_same: ");
    print_array(arr2, 5);
    printf("DNF2 valid: %d\n", verify_dnf(arr2, 5));

    // Test 3: Already sorted
    int arr3[6];
    arr3[0]=0; arr3[1]=0; arr3[2]=1; arr3[3]=1; arr3[4]=2; arr3[5]=2;
    dutch_national_flag(arr3, 6);
    printf("DNF3 sorted: ");
    print_array(arr3, 6);

    // Test 4: Reverse sorted
    int arr4[6];
    arr4[0]=2; arr4[1]=2; arr4[2]=1; arr4[3]=1; arr4[4]=0; arr4[5]=0;
    dutch_national_flag(arr4, 6);
    printf("DNF4 reverse: ");
    print_array(arr4, 6);

    // Test 5: Three-way partition
    int arr5[8];
    arr5[0]=5; arr5[1]=3; arr5[2]=7; arr5[3]=3; arr5[4]=1; arr5[5]=3; arr5[6]=9; arr5[7]=2;
    three_way_partition(arr5, 8, 3);
    printf("DNF5 3way (pivot=3): ");
    print_array(arr5, 8);
}

void test_kadane() {
    printf("=== Kadane's Algorithm Tests ===\n");

    // Test 1: Standard
    int arr1[8];
    arr1[0]=-2; arr1[1]=1; arr1[2]=-3; arr1[3]=4;
    arr1[4]=-1; arr1[5]=2; arr1[6]=1; arr1[7]=-5;
    printf("Kadane1: %d\n", kadane_max_sum(arr1, 8));

    int start;
    int end;
    int sum = kadane_with_indices(arr1, 8, &start, &end);
    printf("Kadane1 indices: start=%d end=%d sum=%d\n", start, end, sum);

    // Test 2: All negative
    int arr2[5];
    arr2[0]=-3; arr2[1]=-1; arr2[2]=-5; arr2[3]=-2; arr2[4]=-4;
    printf("Kadane2 all_neg: %d\n", kadane_max_sum(arr2, 5));

    // Test 3: All positive
    int arr3[4];
    arr3[0]=1; arr3[1]=2; arr3[2]=3; arr3[3]=4;
    printf("Kadane3 all_pos: %d\n", kadane_max_sum(arr3, 4));

    // Test 4: Circular
    int arr4[5];
    arr4[0]=8; arr4[1]=-4; arr4[2]=3; arr4[3]=-1; arr4[4]=12;
    printf("Kadane4 circular: %d\n", max_circular_subarray(arr4, 5));

    // Test 5: Circular with all negative
    printf("Kadane5 circ_neg: %d\n", max_circular_subarray(arr2, 5));

    // Test 6: Max product subarray
    int arr6[5];
    arr6[0]=2; arr6[1]=3; arr6[2]=-2; arr6[3]=4; arr6[4]=-1;
    printf("Kadane6 max_prod: %d\n", max_product_subarray(arr6, 5));

    int arr7[4];
    arr7[0]=-2; arr7[1]=-3; arr7[2]=4; arr7[3]=-1;
    printf("Kadane7 max_prod: %d\n", max_product_subarray(arr7, 4));
}

void test_rotation() {
    printf("=== Array Rotation Tests ===\n");

    // Test 1: Left rotate by 2
    int arr1[7];
    arr1[0]=1; arr1[1]=2; arr1[2]=3; arr1[3]=4; arr1[4]=5; arr1[5]=6; arr1[6]=7;
    rotate_left(arr1, 7, 2);
    printf("Rot1 left2: ");
    print_array(arr1, 7);

    // Test 2: Right rotate by 3
    int arr2[7];
    arr2[0]=1; arr2[1]=2; arr2[2]=3; arr2[3]=4; arr2[4]=5; arr2[5]=6; arr2[6]=7;
    rotate_right(arr2, 7, 3);
    printf("Rot2 right3: ");
    print_array(arr2, 7);

    // Test 3: Check rotation relationship
    int a[5];
    int b[5];
    a[0]=1; a[1]=2; a[2]=3; a[3]=4; a[4]=5;
    b[0]=3; b[1]=4; b[2]=5; b[3]=1; b[4]=2;
    printf("Rot3 is_rotation: %d\n", is_rotation_of(a, b, 5));
    printf("Rot3 distance: %d\n", rotation_distance(a, b, 5));

    // Test 4: Not a rotation
    int c[5];
    c[0]=1; c[1]=2; c[2]=3; c[3]=5; c[4]=4;
    printf("Rot4 not_rotation: %d\n", is_rotation_of(a, c, 5));

    // Test 5: Full rotation (by n)
    int arr5[4];
    arr5[0]=10; arr5[1]=20; arr5[2]=30; arr5[3]=40;
    rotate_left(arr5, 4, 4);
    printf("Rot5 full: ");
    print_array(arr5, 4);

    // Test 6: Rotate by 0
    int arr6[3];
    arr6[0]=7; arr6[1]=8; arr6[2]=9;
    rotate_left(arr6, 3, 0);
    printf("Rot6 zero: ");
    print_array(arr6, 3);
}

void test_next_permutation() {
    printf("=== Next Permutation Tests ===\n");

    // Test 1: Basic
    int arr1[3];
    arr1[0]=1; arr1[1]=2; arr1[2]=3;
    next_permutation(arr1, 3);
    printf("NextPerm1: ");
    print_array(arr1, 3);

    next_permutation(arr1, 3);
    printf("NextPerm2: ");
    print_array(arr1, 3);

    next_permutation(arr1, 3);
    printf("NextPerm3: ");
    print_array(arr1, 3);

    next_permutation(arr1, 3);
    printf("NextPerm4: ");
    print_array(arr1, 3);

    next_permutation(arr1, 3);
    printf("NextPerm5: ");
    print_array(arr1, 3);

    // This is the last permutation (3,2,1), next should wrap
    int wrapped = next_permutation(arr1, 3);
    printf("NextPerm6 wrapped: %d result: ", wrapped);
    print_array(arr1, 3);

    // Test 2: Count permutations from specific to sorted
    int arr2[3];
    arr2[0]=2; arr2[1]=3; arr2[2]=1;
    int perms = count_permutations_to_sorted(arr2, 3);
    printf("PermsToSorted [2,3,1]: %d\n", perms);

    // Test 3: Previous permutation
    int arr3[3];
    arr3[0]=2; arr3[1]=3; arr3[2]=1;
    prev_permutation(arr3, 3);
    printf("PrevPerm [2,3,1]: ");
    print_array(arr3, 3);

    // Test 4: Four elements
    int arr4[4];
    arr4[0]=1; arr4[1]=2; arr4[2]=3; arr4[3]=4;
    next_permutation(arr4, 4);
    printf("NextPerm4el: ");
    print_array(arr4, 4);
}

void test_leaders() {
    printf("=== Leaders Tests ===\n");

    // Test 1
    int arr1[6];
    arr1[0]=16; arr1[1]=17; arr1[2]=4; arr1[3]=3; arr1[4]=5; arr1[5]=2;
    int leaders[10];
    int count = find_leaders(arr1, 6, leaders);
    printf("Leaders1 count: %d\n", count);
    printf("Leaders1: ");
    print_array(leaders, count);

    // Test 2: Descending
    int arr2[5];
    arr2[0]=5; arr2[1]=4; arr2[2]=3; arr2[3]=2; arr2[4]=1;
    count = find_leaders(arr2, 5, leaders);
    printf("Leaders2 count: %d\n", count);
    printf("Leaders2: ");
    print_array(leaders, count);

    // Test 3: Ascending
    int arr3[5];
    arr3[0]=1; arr3[1]=2; arr3[2]=3; arr3[3]=4; arr3[4]=5;
    count = find_leaders(arr3, 5, leaders);
    printf("Leaders3 count: %d\n", count);
    printf("Leaders3: ");
    print_array(leaders, count);

    // Test 4: All same
    int arr4[4];
    arr4[0]=7; arr4[1]=7; arr4[2]=7; arr4[3]=7;
    count = find_leaders(arr4, 4, leaders);
    printf("Leaders4 count: %d\n", count);
}

void test_additional_puzzles() {
    printf("=== Additional Puzzle Tests ===\n");

    // Majority element
    int arr1[7];
    arr1[0]=2; arr1[1]=2; arr1[2]=1; arr1[3]=1; arr1[4]=2; arr1[5]=2; arr1[6]=2;
    int candidate = majority_candidate(arr1, 7);
    printf("Majority1: candidate=%d is_majority=%d\n", candidate, is_majority(arr1, 7, candidate));

    int arr2[5];
    arr2[0]=1; arr2[1]=2; arr2[2]=3; arr2[3]=4; arr2[4]=5;
    candidate = majority_candidate(arr2, 5);
    printf("Majority2: is_majority=%d\n", is_majority(arr2, 5, candidate));

    // Equilibrium index
    int arr3[7];
    arr3[0]=-7; arr3[1]=1; arr3[2]=5; arr3[3]=2; arr3[4]=-4; arr3[5]=3; arr3[6]=0;
    printf("Equilibrium1: %d\n", equilibrium_index(arr3, 7));

    int arr4[3];
    arr4[0]=1; arr4[1]=2; arr4[2]=3;
    printf("Equilibrium2: %d\n", equilibrium_index(arr4, 3));

    int arr5[1];
    arr5[0]=5;
    printf("Equilibrium3: %d\n", equilibrium_index(arr5, 1));

    // Trapping rain water
    int h1[12];
    h1[0]=0; h1[1]=1; h1[2]=0; h1[3]=2; h1[4]=1; h1[5]=0;
    h1[6]=1; h1[7]=3; h1[8]=2; h1[9]=1; h1[10]=2; h1[11]=1;
    printf("TrapWater1: %d\n", trap_water(h1, 12));

    int h2[6];
    h2[0]=3; h2[1]=0; h2[2]=2; h2[3]=0; h2[4]=4; h2[5]=0;
    printf("TrapWater2: %d\n", trap_water(h2, 6));

    // Stock profit
    int prices1[6];
    prices1[0]=7; prices1[1]=1; prices1[2]=5; prices1[3]=3; prices1[4]=6; prices1[5]=4;
    printf("Stock1 single: %d\n", max_profit_single(prices1, 6));
    printf("Stock1 unlimited: %d\n", max_profit_unlimited(prices1, 6));

    int prices2[5];
    prices2[0]=7; prices2[1]=6; prices2[2]=4; prices2[3]=3; prices2[4]=1;
    printf("Stock2 single: %d\n", max_profit_single(prices2, 5));
    printf("Stock2 unlimited: %d\n", max_profit_unlimited(prices2, 5));

    int prices3[6];
    prices3[0]=1; prices3[1]=2; prices3[2]=3; prices3[3]=4; prices3[4]=5; prices3[5]=6;
    printf("Stock3 single: %d\n", max_profit_single(prices3, 6));
    printf("Stock3 unlimited: %d\n", max_profit_unlimited(prices3, 6));
}


int main() {
    test_dutch_flag();
    test_kadane();
    test_rotation();
    test_next_permutation();
    test_leaders();
    test_additional_puzzles();
    return 0;
}
