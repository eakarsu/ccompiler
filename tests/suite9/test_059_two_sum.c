int printf(const char *fmt, ...);

// Two-sum problem via hash map
// Given array of integers and a target, find two indices whose values sum to target

int hm_keys[256];
int hm_vals[256];
int hm_used[256];
int hm_cap;

void hm_init(void) {
    int i;
    hm_cap = 256;
    for (i = 0; i < hm_cap; i++) {
        hm_keys[i] = 0;
        hm_vals[i] = 0;
        hm_used[i] = 0;
    }
}

int hm_hash(int key) {
    int h;
    h = key;
    if (h < 0) h = -h;
    return h % hm_cap;
}

void hm_put(int key, int val) {
    int idx;
    int start;
    idx = hm_hash(key);
    start = idx;
    while (1) {
        if (hm_used[idx] == 0) {
            hm_keys[idx] = key;
            hm_vals[idx] = val;
            hm_used[idx] = 1;
            return;
        }
        if (hm_keys[idx] == key) {
            hm_vals[idx] = val;
            return;
        }
        idx = (idx + 1) % hm_cap;
        if (idx == start) return;
    }
}

int hm_get(int key, int *found) {
    int idx;
    int start;
    idx = hm_hash(key);
    start = idx;
    while (1) {
        if (hm_used[idx] == 0) {
            *found = 0;
            return -1;
        }
        if (hm_keys[idx] == key) {
            *found = 1;
            return hm_vals[idx];
        }
        idx = (idx + 1) % hm_cap;
        if (idx == start) {
            *found = 0;
            return -1;
        }
    }
    *found = 0;
    return -1;
}

// Returns 1 if pair found, stores indices in *i1 and *i2
int two_sum(int *nums, int n, int target, int *i1, int *i2) {
    int i;
    int complement;
    int found;
    int j;
    hm_init();
    for (i = 0; i < n; i++) {
        complement = target - nums[i];
        j = hm_get(complement, &found);
        if (found) {
            *i1 = j;
            *i2 = i;
            return 1;
        }
        hm_put(nums[i], i);
    }
    return 0;
}

// Find all pairs that sum to target (brute force verified with hash)
int count_pairs(int *nums, int n, int target) {
    int count;
    int i;
    int j;
    count = 0;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (nums[i] + nums[j] == target) {
                count = count + 1;
            }
        }
    }
    return count;
}

// Three-sum: find if any triplet sums to target using hash map for inner loop
int three_sum(int *nums, int n, int target) {
    int i;
    int j;
    int complement;
    int found;
    int val;
    for (i = 0; i < n - 2; i++) {
        hm_init();
        for (j = i + 1; j < n; j++) {
            complement = target - nums[i] - nums[j];
            val = hm_get(complement, &found);
            if (found && val > i && val < j) {
                return 1;
            }
            hm_put(nums[j], j);
        }
    }
    return 0;
}

int main(void) {
    int nums1[10];
    int nums2[10];
    int nums3[8];
    int i1;
    int i2;
    int res;

    // Test 1: basic two-sum
    nums1[0] = 2; nums1[1] = 7; nums1[2] = 11; nums1[3] = 15;
    res = two_sum(nums1, 4, 9, &i1, &i2);
    // EXPECT: test1: found=1 i1=0 i2=1
    printf("test1: found=%d i1=%d i2=%d\n", res, i1, i2);

    // Test 2: target at end
    nums1[0] = 3; nums1[1] = 2; nums1[2] = 4;
    res = two_sum(nums1, 3, 6, &i1, &i2);
    // EXPECT: test2: found=1 i1=1 i2=2
    printf("test2: found=%d i1=%d i2=%d\n", res, i1, i2);

    // Test 3: no solution
    nums1[0] = 1; nums1[1] = 2; nums1[2] = 3;
    res = two_sum(nums1, 3, 10, &i1, &i2);
    // EXPECT: test3: found=0
    printf("test3: found=%d\n", res);

    // Test 4: larger array
    nums2[0] = 10; nums2[1] = 20; nums2[2] = 30; nums2[3] = 40;
    nums2[4] = 50; nums2[5] = 60; nums2[6] = 70; nums2[7] = 80;
    res = two_sum(nums2, 8, 90, &i1, &i2);
    // EXPECT: test4: found=1 i1=3 i2=4
    printf("test4: found=%d i1=%d i2=%d\n", res, i1, i2);

    // Test 5: negative numbers
    nums1[0] = -3; nums1[1] = 4; nums1[2] = 7; nums1[3] = -1;
    res = two_sum(nums1, 4, 1, &i1, &i2);
    // EXPECT: test5: found=1 i1=0 i2=1
    printf("test5: found=%d i1=%d i2=%d\n", res, i1, i2);

    // Test 6: count pairs
    nums2[0] = 1; nums2[1] = 2; nums2[2] = 3; nums2[3] = 4;
    nums2[4] = 5; nums2[5] = 6;
    // EXPECT: pairs summing to 7: 3
    printf("pairs summing to 7: %d\n", count_pairs(nums2, 6, 7));

    // EXPECT: pairs summing to 5: 2
    printf("pairs summing to 5: %d\n", count_pairs(nums2, 6, 5));

    // EXPECT: pairs summing to 100: 0
    printf("pairs summing to 100: %d\n", count_pairs(nums2, 6, 100));

    // Test 7: three-sum
    nums3[0] = 1; nums3[1] = 2; nums3[2] = 3; nums3[3] = 4;
    nums3[4] = 5;
    // EXPECT: three_sum 6: 1
    printf("three_sum 6: %d\n", three_sum(nums3, 5, 6));
    // EXPECT: three_sum 12: 1
    printf("three_sum 12: %d\n", three_sum(nums3, 5, 12));
    // EXPECT: three_sum 100: 0
    printf("three_sum 100: %d\n", three_sum(nums3, 5, 100));

    // Test 8: two-sum with duplicate values
    nums1[0] = 3; nums1[1] = 3;
    res = two_sum(nums1, 2, 6, &i1, &i2);
    // EXPECT: test8 dup: found=1 i1=0 i2=1
    printf("test8 dup: found=%d i1=%d i2=%d\n", res, i1, i2);

    // EXPECT: done
    printf("done\n");

    return 0;
}
