int printf(const char *fmt, ...);

int ht_keys[128];
int ht_vals[128];
int ht_used[128];

void ht_init(void) {
    int i;
    for (i = 0; i < 128; i = i + 1) {
        ht_keys[i] = 0;
        ht_vals[i] = 0;
        ht_used[i] = 0;
    }
}

int hash_int(int x) {
    if (x < 0) x = -x;
    return x % 128;
}

void ht_put(int key, int value) {
    int h = hash_int(key);
    int i;
    for (i = 0; i < 128; i = i + 1) {
        int idx = (h + i) % 128;
        if (ht_used[idx] == 0) {
            ht_keys[idx] = key;
            ht_vals[idx] = value;
            ht_used[idx] = 1;
            return;
        }
        if (ht_keys[idx] == key) {
            ht_vals[idx] = value;
            return;
        }
    }
}

int ht_get(int key, int *found) {
    int h = hash_int(key);
    int i;
    for (i = 0; i < 128; i = i + 1) {
        int idx = (h + i) % 128;
        if (ht_used[idx] == 0) {
            *found = 0;
            return -1;
        }
        if (ht_keys[idx] == key) {
            *found = 1;
            return ht_vals[idx];
        }
    }
    *found = 0;
    return -1;
}

int two_sum(int arr[], int n, int target, int *out_i, int *out_j) {
    ht_init();
    int i;
    for (i = 0; i < n; i = i + 1) {
        int complement = target - arr[i];
        int found;
        int j = ht_get(complement, &found);
        if (found) {
            *out_i = j;
            *out_j = i;
            return 1;
        }
        ht_put(arr[i], i);
    }
    return 0;
}

int count_pairs(int arr[], int n, int target) {
    ht_init();
    int count = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        ht_put(arr[i], i);
    }
    int used[20];
    for (i = 0; i < n; i = i + 1) used[i] = 0;

    for (i = 0; i < n; i = i + 1) {
        if (used[i]) continue;
        int complement = target - arr[i];
        int found;
        int j = ht_get(complement, &found);
        if (found && j != i && used[j] == 0) {
            count = count + 1;
            used[i] = 1;
            used[j] = 1;
        }
    }
    return count;
}

int has_complement(int arr[], int n, int val) {
    ht_init();
    int i;
    for (i = 0; i < n; i = i + 1) {
        ht_put(arr[i], i);
    }
    int found;
    ht_get(val, &found);
    return found;
}

int main(void) {
    int arr1[5];
    arr1[0] = 2; arr1[1] = 7; arr1[2] = 11; arr1[3] = 15; arr1[4] = 1;
    int ri;
    int rj;

    // EXPECT: two sum 9: 1
    int res = two_sum(arr1, 5, 9, &ri, &rj);
    printf("two sum 9: %d\n", res);

    // EXPECT: indices: 0 1
    printf("indices: %d %d\n", ri, rj);

    // EXPECT: sum check: 9
    printf("sum check: %d\n", arr1[ri] + arr1[rj]);

    // EXPECT: two sum 3: 1
    res = two_sum(arr1, 5, 3, &ri, &rj);
    printf("two sum 3: %d\n", res);

    // EXPECT: sum check2: 3
    printf("sum check2: %d\n", arr1[ri] + arr1[rj]);

    // EXPECT: two sum 100: 0
    res = two_sum(arr1, 5, 100, &ri, &rj);
    printf("two sum 100: %d\n", res);

    int arr2[6];
    arr2[0] = 3; arr2[1] = 5; arr2[2] = 2; arr2[3] = 8; arr2[4] = 1; arr2[5] = 7;

    // EXPECT: two sum 10: 1
    res = two_sum(arr2, 6, 10, &ri, &rj);
    printf("two sum 10: %d\n", res);

    // EXPECT: sum check3: 10
    printf("sum check3: %d\n", arr2[ri] + arr2[rj]);

    // EXPECT: two sum 8: 1
    res = two_sum(arr2, 6, 8, &ri, &rj);
    printf("two sum 8: %d\n", res);

    // EXPECT: sum check4: 8
    printf("sum check4: %d\n", arr2[ri] + arr2[rj]);

    int arr3[8];
    arr3[0] = 1; arr3[1] = 2; arr3[2] = 3; arr3[3] = 4;
    arr3[4] = 5; arr3[5] = 6; arr3[6] = 7; arr3[7] = 8;

    // EXPECT: pairs sum 9: 4
    printf("pairs sum 9: %d\n", count_pairs(arr3, 8, 9));

    // EXPECT: pairs sum 5: 2
    printf("pairs sum 5: %d\n", count_pairs(arr3, 8, 5));

    // EXPECT: pairs sum 3: 1
    printf("pairs sum 3: %d\n", count_pairs(arr3, 8, 3));

    // EXPECT: pairs sum 100: 0
    printf("pairs sum 100: %d\n", count_pairs(arr3, 8, 100));

    // EXPECT: has 5: 1
    printf("has 5: %d\n", has_complement(arr3, 8, 5));

    // EXPECT: has 99: 0
    printf("has 99: %d\n", has_complement(arr3, 8, 99));

    int arr4[4];
    arr4[0] = 10; arr4[1] = 20; arr4[2] = 10; arr4[3] = 30;
    // EXPECT: dup two sum 20: 1
    res = two_sum(arr4, 4, 20, &ri, &rj);
    printf("dup two sum 20: %d\n", res);

    // EXPECT: done
    printf("done\n");

    return 0;
}
