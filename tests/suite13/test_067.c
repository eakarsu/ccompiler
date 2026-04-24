int printf(const char *fmt, ...);

// Cuckoo hashing: two tables with two hash functions
// If insertion causes a cycle, we detect it and report failure

enum { CUCKOO_SIZE = 8, CUCKOO_EMPTY = -1, MAX_KICKS = 20 };

struct CuckooTable {
    int table1[8];
    int table2[8];
    int vals1[8];
    int vals2[8];
    int count;
};

int h1(int key) {
    int h = key;
    if (h < 0) h = -h;
    return h % CUCKOO_SIZE;
}

int h2(int key) {
    int h = key;
    if (h < 0) h = -h;
    return (h / CUCKOO_SIZE + h * 3) % CUCKOO_SIZE;
}

void cuckoo_init(struct CuckooTable *ct) {
    int i;
    for (i = 0; i < CUCKOO_SIZE; i++) {
        ct->table1[i] = CUCKOO_EMPTY;
        ct->table2[i] = CUCKOO_EMPTY;
        ct->vals1[i] = 0;
        ct->vals2[i] = 0;
    }
    ct->count = 0;
}

int cuckoo_lookup(struct CuckooTable *ct, int key, int *out) {
    int idx1;
    int idx2;
    idx1 = h1(key);
    idx2 = h2(key);
    if (ct->table1[idx1] == key) {
        *out = ct->vals1[idx1];
        return 1;
    }
    if (ct->table2[idx2] == key) {
        *out = ct->vals2[idx2];
        return 1;
    }
    return 0;
}

int cuckoo_insert(struct CuckooTable *ct, int key, int val) {
    int idx;
    int kicks;
    int cur_key;
    int cur_val;
    int tmp_key;
    int tmp_val;
    int in_table1;

    idx = h1(key);
    if (ct->table1[idx] == key) {
        ct->vals1[idx] = val;
        return 1;
    }
    idx = h2(key);
    if (ct->table2[idx] == key) {
        ct->vals2[idx] = val;
        return 1;
    }

    cur_key = key;
    cur_val = val;
    in_table1 = 1;

    for (kicks = 0; kicks < MAX_KICKS; kicks++) {
        if (in_table1) {
            idx = h1(cur_key);
            if (ct->table1[idx] == CUCKOO_EMPTY) {
                ct->table1[idx] = cur_key;
                ct->vals1[idx] = cur_val;
                ct->count = ct->count + 1;
                return 1;
            }
            tmp_key = ct->table1[idx];
            tmp_val = ct->vals1[idx];
            ct->table1[idx] = cur_key;
            ct->vals1[idx] = cur_val;
            cur_key = tmp_key;
            cur_val = tmp_val;
            in_table1 = 0;
        } else {
            idx = h2(cur_key);
            if (ct->table2[idx] == CUCKOO_EMPTY) {
                ct->table2[idx] = cur_key;
                ct->vals2[idx] = cur_val;
                ct->count = ct->count + 1;
                return 1;
            }
            tmp_key = ct->table2[idx];
            tmp_val = ct->vals2[idx];
            ct->table2[idx] = cur_key;
            ct->vals2[idx] = cur_val;
            cur_key = tmp_key;
            cur_val = tmp_val;
            in_table1 = 1;
        }
    }
    return 0;
}

int cuckoo_delete(struct CuckooTable *ct, int key) {
    int idx;
    idx = h1(key);
    if (ct->table1[idx] == key) {
        ct->table1[idx] = CUCKOO_EMPTY;
        ct->vals1[idx] = 0;
        ct->count = ct->count - 1;
        return 1;
    }
    idx = h2(key);
    if (ct->table2[idx] == key) {
        ct->table2[idx] = CUCKOO_EMPTY;
        ct->vals2[idx] = 0;
        ct->count = ct->count - 1;
        return 1;
    }
    return 0;
}

int main() {
    struct CuckooTable ct;
    int val;
    int found;
    int ok;
    int i;
    int success_count;

    cuckoo_init(&ct);

    printf("%d\n", ct.count);
    // EXPECT: 0

    ok = cuckoo_insert(&ct, 5, 50);
    printf("%d %d\n", ok, ct.count);
    // EXPECT: 1 1

    ok = cuckoo_insert(&ct, 13, 130);
    printf("%d %d\n", ok, ct.count);
    // EXPECT: 1 2

    ok = cuckoo_insert(&ct, 21, 210);
    printf("%d %d\n", ok, ct.count);
    // EXPECT: 1 3

    found = cuckoo_lookup(&ct, 5, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 50

    found = cuckoo_lookup(&ct, 13, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 130

    found = cuckoo_lookup(&ct, 21, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 210

    found = cuckoo_lookup(&ct, 99, &val);
    printf("%d\n", found);
    // EXPECT: 0

    cuckoo_insert(&ct, 5, 55);
    found = cuckoo_lookup(&ct, 5, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 55

    printf("%d\n", ct.count);
    // EXPECT: 3

    ok = cuckoo_delete(&ct, 13);
    printf("%d %d\n", ok, ct.count);
    // EXPECT: 1 2

    found = cuckoo_lookup(&ct, 13, &val);
    printf("%d\n", found);
    // EXPECT: 0

    found = cuckoo_lookup(&ct, 5, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 55

    cuckoo_init(&ct);
    success_count = 0;
    for (i = 0; i < 10; i++) {
        ok = cuckoo_insert(&ct, i * 7, i * 100);
        if (ok) success_count = success_count + 1;
    }
    printf("%d\n", success_count);
    // EXPECT: 10

    found = cuckoo_lookup(&ct, 0, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 0

    found = cuckoo_lookup(&ct, 35, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 500

    found = cuckoo_lookup(&ct, 63, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 900

    printf("%d\n", ct.count);
    // EXPECT: 10

    return 0;
}
