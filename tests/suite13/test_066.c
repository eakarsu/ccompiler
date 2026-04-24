int printf(const char *fmt, ...);

// Hash table resizing/rehashing simulation
// Start with small table, track when rehash would trigger, simulate rehash

enum { SMALL_SIZE = 4, BIG_SIZE = 16, EMPTY_KEY = -1 };

struct SmallTable {
    int keys[4];
    int vals[4];
    int count;
};

struct BigTable {
    int keys[16];
    int vals[16];
    int count;
};

int hash_small(int key) {
    int h = key;
    if (h < 0) h = -h;
    return h % SMALL_SIZE;
}

int hash_big(int key) {
    int h = key;
    if (h < 0) h = -h;
    return h % BIG_SIZE;
}

void small_init(struct SmallTable *t) {
    int i;
    for (i = 0; i < SMALL_SIZE; i++) {
        t->keys[i] = EMPTY_KEY;
        t->vals[i] = 0;
    }
    t->count = 0;
}

void big_init(struct BigTable *t) {
    int i;
    for (i = 0; i < BIG_SIZE; i++) {
        t->keys[i] = EMPTY_KEY;
        t->vals[i] = 0;
    }
    t->count = 0;
}

int small_insert(struct SmallTable *t, int key, int val) {
    int idx;
    int start;
    idx = hash_small(key);
    start = idx;
    while (1) {
        if (t->keys[idx] == EMPTY_KEY) {
            t->keys[idx] = key;
            t->vals[idx] = val;
            t->count = t->count + 1;
            return 1;
        }
        if (t->keys[idx] == key) {
            t->vals[idx] = val;
            return 1;
        }
        idx = (idx + 1) % SMALL_SIZE;
        if (idx == start) return 0;
    }
    return 0;
}

int small_get(struct SmallTable *t, int key, int *out) {
    int idx;
    int start;
    idx = hash_small(key);
    start = idx;
    while (1) {
        if (t->keys[idx] == EMPTY_KEY) return 0;
        if (t->keys[idx] == key) {
            *out = t->vals[idx];
            return 1;
        }
        idx = (idx + 1) % SMALL_SIZE;
        if (idx == start) return 0;
    }
    return 0;
}

int big_insert(struct BigTable *t, int key, int val) {
    int idx;
    int start;
    idx = hash_big(key);
    start = idx;
    while (1) {
        if (t->keys[idx] == EMPTY_KEY) {
            t->keys[idx] = key;
            t->vals[idx] = val;
            t->count = t->count + 1;
            return 1;
        }
        if (t->keys[idx] == key) {
            t->vals[idx] = val;
            return 1;
        }
        idx = (idx + 1) % BIG_SIZE;
        if (idx == start) return 0;
    }
    return 0;
}

int big_get(struct BigTable *t, int key, int *out) {
    int idx;
    int start;
    idx = hash_big(key);
    start = idx;
    while (1) {
        if (t->keys[idx] == EMPTY_KEY) return 0;
        if (t->keys[idx] == key) {
            *out = t->vals[idx];
            return 1;
        }
        idx = (idx + 1) % BIG_SIZE;
        if (idx == start) return 0;
    }
    return 0;
}

int needs_rehash(struct SmallTable *t) {
    return t->count * 100 / SMALL_SIZE >= 75;
}

void rehash(struct SmallTable *small, struct BigTable *big) {
    int i;
    big_init(big);
    for (i = 0; i < SMALL_SIZE; i++) {
        if (small->keys[i] != EMPTY_KEY) {
            big_insert(big, small->keys[i], small->vals[i]);
        }
    }
}

int big_probe_length(struct BigTable *t, int key) {
    int idx;
    int probes;
    idx = hash_big(key);
    probes = 0;
    while (1) {
        probes = probes + 1;
        if (t->keys[idx] == EMPTY_KEY) return probes;
        if (t->keys[idx] == key) return probes;
        idx = (idx + 1) % BIG_SIZE;
    }
    return probes;
}

int main() {
    struct SmallTable st;
    struct BigTable bt;
    int val;
    int found;
    int i;
    int total_probes;

    small_init(&st);

    small_insert(&st, 10, 100);
    small_insert(&st, 20, 200);

    printf("%d\n", st.count);
    // EXPECT: 2

    printf("%d\n", needs_rehash(&st));
    // EXPECT: 0

    small_insert(&st, 30, 300);

    printf("%d\n", st.count);
    // EXPECT: 3

    printf("%d\n", needs_rehash(&st));
    // EXPECT: 1

    found = small_get(&st, 10, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 100

    found = small_get(&st, 20, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 200

    found = small_get(&st, 30, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 300

    rehash(&st, &bt);

    printf("%d\n", bt.count);
    // EXPECT: 3

    found = big_get(&bt, 10, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 100

    found = big_get(&bt, 20, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 200

    found = big_get(&bt, 30, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 300

    big_insert(&bt, 5, 50);
    big_insert(&bt, 21, 210);
    big_insert(&bt, 37, 370);
    big_insert(&bt, 53, 530);

    printf("%d\n", bt.count);
    // EXPECT: 7

    found = big_get(&bt, 37, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 370

    found = big_get(&bt, 53, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 530

    printf("%d\n", big_probe_length(&bt, 5));
    // EXPECT: 1

    printf("%d\n", big_probe_length(&bt, 21));
    // EXPECT: 2

    total_probes = 0;
    for (i = 0; i < 7; i++) {
        int k;
        if (i == 0) k = 10;
        else if (i == 1) k = 20;
        else if (i == 2) k = 30;
        else if (i == 3) k = 5;
        else if (i == 4) k = 21;
        else if (i == 5) k = 37;
        else k = 53;
        total_probes = total_probes + big_probe_length(&bt, k);
    }
    printf("%d\n", total_probes);
    // EXPECT: 13

    found = big_get(&bt, 999, &val);
    printf("%d\n", found);
    // EXPECT: 0

    return 0;
}
