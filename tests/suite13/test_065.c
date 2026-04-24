int printf(const char *fmt, ...);

// Two-sum problem using hash table
// Given an array of integers and a target, find two indices whose values sum to target

enum { HT_SIZE = 64, NO_KEY = -999999 };

struct TwoSumEntry {
    int key;
    int value;
};

struct TwoSumMap {
    struct TwoSumEntry entries[64];
};

void ts_init(struct TwoSumMap *m) {
    int i;
    for (i = 0; i < HT_SIZE; i++) {
        m->entries[i].key = NO_KEY;
        m->entries[i].value = 0;
    }
}

int ts_hash(int key) {
    int h = key;
    if (h < 0) h = -h;
    return h % HT_SIZE;
}

void ts_put(struct TwoSumMap *m, int key, int value) {
    int idx;
    int start;
    idx = ts_hash(key);
    start = idx;
    while (1) {
        if (m->entries[idx].key == NO_KEY || m->entries[idx].key == key) {
            m->entries[idx].key = key;
            m->entries[idx].value = value;
            return;
        }
        idx = (idx + 1) % HT_SIZE;
        if (idx == start) return;
    }
}

int ts_get(struct TwoSumMap *m, int key, int *out) {
    int idx;
    int start;
    idx = ts_hash(key);
    start = idx;
    while (1) {
        if (m->entries[idx].key == NO_KEY) return 0;
        if (m->entries[idx].key == key) {
            *out = m->entries[idx].value;
            return 1;
        }
        idx = (idx + 1) % HT_SIZE;
        if (idx == start) return 0;
    }
    return 0;
}

int two_sum(int *nums, int n, int target, int *idx1, int *idx2) {
    struct TwoSumMap m;
    int i;
    int complement;
    int prev_idx;

    ts_init(&m);
    for (i = 0; i < n; i++) {
        complement = target - nums[i];
        if (ts_get(&m, complement, &prev_idx)) {
            *idx1 = prev_idx;
            *idx2 = i;
            return 1;
        }
        ts_put(&m, nums[i], i);
    }
    return 0;
}

int count_pairs(int *nums, int n, int target) {
    struct TwoSumMap m;
    int i;
    int complement;
    int dummy;
    int pairs;

    ts_init(&m);
    pairs = 0;
    for (i = 0; i < n; i++) {
        complement = target - nums[i];
        if (ts_get(&m, complement, &dummy)) {
            pairs = pairs + 1;
        }
        ts_put(&m, nums[i], i);
    }
    return pairs;
}

int main() {
    int nums[16];
    int a;
    int b;
    int found;

    nums[0] = 2;
    nums[1] = 7;
    nums[2] = 11;
    nums[3] = 15;

    found = two_sum(nums, 4, 9, &a, &b);
    printf("%d %d %d\n", found, a, b);
    // EXPECT: 1 0 1

    found = two_sum(nums, 4, 13, &a, &b);
    printf("%d %d %d\n", found, a, b);
    // EXPECT: 1 0 2

    found = two_sum(nums, 4, 26, &a, &b);
    printf("%d %d %d\n", found, a, b);
    // EXPECT: 1 2 3

    found = two_sum(nums, 4, 100, &a, &b);
    printf("%d\n", found);
    // EXPECT: 0

    nums[0] = 3;
    nums[1] = 3;
    found = two_sum(nums, 2, 6, &a, &b);
    printf("%d %d %d\n", found, a, b);
    // EXPECT: 1 0 1

    nums[0] = 1;
    nums[1] = 5;
    nums[2] = 3;
    nums[3] = 4;
    nums[4] = 2;
    nums[5] = 8;
    nums[6] = 6;
    nums[7] = 7;

    found = two_sum(nums, 8, 10, &a, &b);
    printf("%d %d %d\n", found, a, b);
    // EXPECT: 1 4 5

    found = two_sum(nums, 8, 3, &a, &b);
    printf("%d %d %d\n", found, a, b);
    // EXPECT: 1 0 4

    found = two_sum(nums, 8, 13, &a, &b);
    printf("%d %d %d\n", found, a, b);
    // EXPECT: 1 1 5

    printf("%d\n", count_pairs(nums, 8, 9));
    // EXPECT: 4

    printf("%d\n", count_pairs(nums, 8, 7));
    // EXPECT: 3

    printf("%d\n", count_pairs(nums, 8, 100));
    // EXPECT: 0

    nums[0] = 10;
    nums[1] = 20;
    nums[2] = 30;
    nums[3] = 40;
    nums[4] = 50;
    printf("%d\n", count_pairs(nums, 5, 60));
    // EXPECT: 2

    found = two_sum(nums, 5, 70, &a, &b);
    printf("%d %d %d\n", found, a, b);
    // EXPECT: 1 2 3

    found = two_sum(nums, 5, 90, &a, &b);
    printf("%d %d %d\n", found, a, b);
    // EXPECT: 1 3 4

    found = two_sum(nums, 5, 15, &a, &b);
    printf("%d\n", found);
    // EXPECT: 0

    return 0;
}
