int printf(const char *fmt, ...);

// Robin Hood hashing
// On collision, the element that has traveled farther from its ideal position
// gets priority (steals from the rich, gives to the poor)

enum { RH_SIZE = 16, RH_EMPTY = -1 };

struct RHEntry {
    int key;
    int value;
    int psl;
};

struct RHTable {
    struct RHEntry entries[16];
    int count;
};

int rh_hash(int key) {
    int h = key;
    if (h < 0) h = -h;
    return h % RH_SIZE;
}

void rh_init(struct RHTable *t) {
    int i;
    for (i = 0; i < RH_SIZE; i++) {
        t->entries[i].key = RH_EMPTY;
        t->entries[i].value = 0;
        t->entries[i].psl = 0;
    }
    t->count = 0;
}

int rh_insert(struct RHTable *t, int key, int value) {
    int idx;
    int cur_key;
    int cur_val;
    int cur_psl;
    int tmp_key;
    int tmp_val;
    int tmp_psl;
    int steps;

    if (t->count >= RH_SIZE) return 0;

    idx = rh_hash(key);

    cur_key = key;
    cur_val = value;
    cur_psl = 0;

    for (steps = 0; steps < RH_SIZE; steps++) {
        if (t->entries[idx].key == RH_EMPTY) {
            t->entries[idx].key = cur_key;
            t->entries[idx].value = cur_val;
            t->entries[idx].psl = cur_psl;
            t->count = t->count + 1;
            return 1;
        }
        if (t->entries[idx].key == key && cur_key == key) {
            t->entries[idx].value = value;
            return 1;
        }
        if (cur_psl > t->entries[idx].psl) {
            tmp_key = t->entries[idx].key;
            tmp_val = t->entries[idx].value;
            tmp_psl = t->entries[idx].psl;

            t->entries[idx].key = cur_key;
            t->entries[idx].value = cur_val;
            t->entries[idx].psl = cur_psl;

            cur_key = tmp_key;
            cur_val = tmp_val;
            cur_psl = tmp_psl;
        }
        cur_psl = cur_psl + 1;
        idx = (idx + 1) % RH_SIZE;
    }
    return 0;
}

int rh_get(struct RHTable *t, int key, int *out) {
    int idx;
    int psl;
    idx = rh_hash(key);
    psl = 0;
    while (1) {
        if (t->entries[idx].key == RH_EMPTY) return 0;
        if (psl > t->entries[idx].psl) return 0;
        if (t->entries[idx].key == key) {
            *out = t->entries[idx].value;
            return 1;
        }
        psl = psl + 1;
        idx = (idx + 1) % RH_SIZE;
    }
    return 0;
}

int rh_max_psl(struct RHTable *t) {
    int i;
    int max;
    max = 0;
    for (i = 0; i < RH_SIZE; i++) {
        if (t->entries[i].key != RH_EMPTY) {
            if (t->entries[i].psl > max) {
                max = t->entries[i].psl;
            }
        }
    }
    return max;
}

int rh_avg_psl_x10(struct RHTable *t) {
    int i;
    int total;
    int cnt;
    total = 0;
    cnt = 0;
    for (i = 0; i < RH_SIZE; i++) {
        if (t->entries[i].key != RH_EMPTY) {
            total = total + t->entries[i].psl;
            cnt = cnt + 1;
        }
    }
    if (cnt == 0) return 0;
    return (total * 10) / cnt;
}

int rh_psl_variance_x100(struct RHTable *t) {
    int i;
    int total;
    int cnt;
    int avg_x10;
    int sum_sq_x100;
    int diff_x10;

    total = 0;
    cnt = 0;
    for (i = 0; i < RH_SIZE; i++) {
        if (t->entries[i].key != RH_EMPTY) {
            total = total + t->entries[i].psl;
            cnt = cnt + 1;
        }
    }
    if (cnt == 0) return 0;
    avg_x10 = (total * 10) / cnt;

    sum_sq_x100 = 0;
    for (i = 0; i < RH_SIZE; i++) {
        if (t->entries[i].key != RH_EMPTY) {
            diff_x10 = t->entries[i].psl * 10 - avg_x10;
            sum_sq_x100 = sum_sq_x100 + diff_x10 * diff_x10;
        }
    }
    return sum_sq_x100 / (cnt * 100);
}

int main() {
    struct RHTable t;
    int val;
    int found;
    int i;
    int ok;

    rh_init(&t);

    printf("%d\n", t.count);
    // EXPECT: 0

    rh_insert(&t, 5, 50);
    rh_insert(&t, 21, 210);
    rh_insert(&t, 37, 370);

    printf("%d\n", t.count);
    // EXPECT: 3

    found = rh_get(&t, 5, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 50

    found = rh_get(&t, 21, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 210

    found = rh_get(&t, 37, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 370

    found = rh_get(&t, 99, &val);
    printf("%d\n", found);
    // EXPECT: 0

    rh_insert(&t, 5, 55);
    found = rh_get(&t, 5, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 55

    printf("%d\n", t.count);
    // EXPECT: 3

    printf("%d\n", rh_max_psl(&t));
    // EXPECT: 2

    rh_init(&t);
    for (i = 0; i < 8; i++) {
        rh_insert(&t, i * 16 + 3, i * 100);
    }
    printf("%d\n", t.count);
    // EXPECT: 8

    found = rh_get(&t, 3, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 0

    found = rh_get(&t, 19, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 100

    found = rh_get(&t, 115, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 700

    int max_psl = rh_max_psl(&t);
    printf("%d\n", max_psl <= 8);
    // EXPECT: 1

    int var = rh_psl_variance_x100(&t);
    printf("%d\n", var >= 0);
    // EXPECT: 1

    rh_init(&t);
    rh_insert(&t, 1, 10);
    rh_insert(&t, 2, 20);
    rh_insert(&t, 3, 30);

    printf("%d\n", rh_max_psl(&t));
    // EXPECT: 0

    printf("%d\n", rh_avg_psl_x10(&t));
    // EXPECT: 0

    rh_init(&t);
    ok = 1;
    for (i = 0; i < 16; i++) {
        if (!rh_insert(&t, i * 7, i)) ok = 0;
    }
    printf("%d\n", ok);
    // EXPECT: 1

    printf("%d\n", t.count);
    // EXPECT: 16

    ok = 1;
    for (i = 0; i < 16; i++) {
        found = rh_get(&t, i * 7, &val);
        if (!found || val != i) ok = 0;
    }
    printf("%d\n", ok);
    // EXPECT: 1

    printf("%d\n", rh_max_psl(&t) <= 8);
    // EXPECT: 1

    return 0;
}
