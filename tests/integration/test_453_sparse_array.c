int printf(const char *fmt, ...);
// EXPECT: Count: 5\nsa[0] = 100 (found=1)\nsa[1000] = 200 (found=1)\nsa[5000] = 300 (found=1)\nsa[99999] = 400 (found=1)\nsa[42] = 500 (found=1)\nsa[7] = 0 (found=0)\nUpdated sa[42] = 999\nCount after update: 5\nContains 1000: 1\nContains 7777: 0\nDone

// Hash-based sparse array

struct SparseEntry {
    int key;
    int value;
    int occupied;
};

struct SparseArray {
    struct SparseEntry buckets[32];
    int count;
};

void sa_init(struct SparseArray *sa) {
    sa->count = 0;
    int i = 0;
    while (i < 32) {
        sa->buckets[i].key = 0;
        sa->buckets[i].value = 0;
        sa->buckets[i].occupied = 0;
        i = i + 1;
    }
}

int sa_hash(int key) {
    int h = key;
    if (h < 0) h = -h;
    return h % 32;
}

void sa_set(struct SparseArray *sa, int key, int value) {
    int h = sa_hash(key);
    int i = 0;
    while (i < 32) {
        int idx = (h + i) % 32;
        if (!sa->buckets[idx].occupied) {
            sa->buckets[idx].key = key;
            sa->buckets[idx].value = value;
            sa->buckets[idx].occupied = 1;
            sa->count = sa->count + 1;
            return;
        }
        if (sa->buckets[idx].key == key) {
            sa->buckets[idx].value = value;
            return;
        }
        i = i + 1;
    }
}

int sa_get(struct SparseArray *sa, int key, int *found) {
    int h = sa_hash(key);
    int i = 0;
    while (i < 32) {
        int idx = (h + i) % 32;
        if (!sa->buckets[idx].occupied) {
            *found = 0;
            return 0;
        }
        if (sa->buckets[idx].key == key) {
            *found = 1;
            return sa->buckets[idx].value;
        }
        i = i + 1;
    }
    *found = 0;
    return 0;
}

int sa_contains(struct SparseArray *sa, int key) {
    int found;
    sa_get(sa, key, &found);
    return found;
}

int main(void) {
    struct SparseArray sa;
    sa_init(&sa);

    // Sparse indices
    sa_set(&sa, 0, 100);
    sa_set(&sa, 1000, 200);
    sa_set(&sa, 5000, 300);
    sa_set(&sa, 99999, 400);
    sa_set(&sa, 42, 500);

    printf("Count: %d\n", sa.count);

    int found;
    int v;

    v = sa_get(&sa, 0, &found);
    printf("sa[0] = %d (found=%d)\n", v, found);

    v = sa_get(&sa, 1000, &found);
    printf("sa[1000] = %d (found=%d)\n", v, found);

    v = sa_get(&sa, 5000, &found);
    printf("sa[5000] = %d (found=%d)\n", v, found);

    v = sa_get(&sa, 99999, &found);
    printf("sa[99999] = %d (found=%d)\n", v, found);

    v = sa_get(&sa, 42, &found);
    printf("sa[42] = %d (found=%d)\n", v, found);

    // Not found
    v = sa_get(&sa, 7, &found);
    printf("sa[7] = %d (found=%d)\n", v, found);

    // Update existing
    sa_set(&sa, 42, 999);
    v = sa_get(&sa, 42, &found);
    printf("Updated sa[42] = %d\n", v);
    printf("Count after update: %d\n", sa.count);

    // Contains test
    printf("Contains 1000: %d\n", sa_contains(&sa, 1000));
    printf("Contains 7777: %d\n", sa_contains(&sa, 7777));

    printf("Done\n");
    return 0;
}
