int printf(const char *fmt, ...);

// Multiset with counts using sorted array

struct Multiset {
    int keys[32];
    int counts[32];
    int size;
    int total;
};

void ms_init(struct Multiset *ms) {
    ms->size = 0;
    ms->total = 0;
}

int ms_find(struct Multiset *ms, int key) {
    int i;
    for (i = 0; i < ms->size; i++) {
        if (ms->keys[i] == key) return i;
    }
    return -1;
}

void ms_insert(struct Multiset *ms, int key) {
    int idx = ms_find(ms, key);
    int i;
    int pos;
    if (idx >= 0) {
        ms->counts[idx] = ms->counts[idx] + 1;
        ms->total = ms->total + 1;
        return;
    }
    // Insert in sorted order
    pos = ms->size;
    for (i = 0; i < ms->size; i++) {
        if (key < ms->keys[i]) {
            pos = i;
            break;
        }
    }
    for (i = ms->size; i > pos; i--) {
        ms->keys[i] = ms->keys[i - 1];
        ms->counts[i] = ms->counts[i - 1];
    }
    ms->keys[pos] = key;
    ms->counts[pos] = 1;
    ms->size = ms->size + 1;
    ms->total = ms->total + 1;
}

int ms_remove_one(struct Multiset *ms, int key) {
    int idx = ms_find(ms, key);
    int i;
    if (idx < 0) return 0;
    ms->counts[idx] = ms->counts[idx] - 1;
    ms->total = ms->total - 1;
    if (ms->counts[idx] == 0) {
        for (i = idx; i < ms->size - 1; i++) {
            ms->keys[i] = ms->keys[i + 1];
            ms->counts[i] = ms->counts[i + 1];
        }
        ms->size = ms->size - 1;
    }
    return 1;
}

int ms_remove_all(struct Multiset *ms, int key) {
    int idx = ms_find(ms, key);
    int removed;
    int i;
    if (idx < 0) return 0;
    removed = ms->counts[idx];
    ms->total = ms->total - removed;
    for (i = idx; i < ms->size - 1; i++) {
        ms->keys[i] = ms->keys[i + 1];
        ms->counts[i] = ms->counts[i + 1];
    }
    ms->size = ms->size - 1;
    return removed;
}

int ms_count(struct Multiset *ms, int key) {
    int idx = ms_find(ms, key);
    if (idx < 0) return 0;
    return ms->counts[idx];
}

int ms_min(struct Multiset *ms) {
    if (ms->size == 0) return -999;
    return ms->keys[0];
}

int ms_max(struct Multiset *ms) {
    if (ms->size == 0) return -999;
    return ms->keys[ms->size - 1];
}

int ms_unique_count(struct Multiset *ms) {
    return ms->size;
}

int ms_mode(struct Multiset *ms) {
    int max_count = 0;
    int mode_val = -999;
    int i;
    for (i = 0; i < ms->size; i++) {
        if (ms->counts[i] > max_count) {
            max_count = ms->counts[i];
            mode_val = ms->keys[i];
        }
    }
    return mode_val;
}

void ms_print(struct Multiset *ms) {
    int i;
    int j;
    int first = 1;
    for (i = 0; i < ms->size; i++) {
        for (j = 0; j < ms->counts[i]; j++) {
            if (!first) printf(" ");
            printf("%d", ms->keys[i]);
            first = 0;
        }
    }
}

int main() {
    struct Multiset ms;
    int removed;

    ms_init(&ms);

    ms_insert(&ms, 5);
    ms_insert(&ms, 3);
    ms_insert(&ms, 5);
    ms_insert(&ms, 1);
    ms_insert(&ms, 3);
    ms_insert(&ms, 5);
    printf("multiset: ");
    ms_print(&ms);
    printf("\n");                                        // EXPECT: multiset: 1 3 3 5 5 5

    printf("total: %d\n", ms.total);                    // EXPECT: total: 6
    printf("unique: %d\n", ms_unique_count(&ms));       // EXPECT: unique: 3
    printf("count(5): %d\n", ms_count(&ms, 5));         // EXPECT: count(5): 3
    printf("count(3): %d\n", ms_count(&ms, 3));         // EXPECT: count(3): 2
    printf("count(7): %d\n", ms_count(&ms, 7));         // EXPECT: count(7): 0
    printf("min: %d\n", ms_min(&ms));                   // EXPECT: min: 1
    printf("max: %d\n", ms_max(&ms));                   // EXPECT: max: 5
    printf("mode: %d\n", ms_mode(&ms));                 // EXPECT: mode: 5

    ms_remove_one(&ms, 5);
    printf("after remove one 5, count(5): %d\n", ms_count(&ms, 5));
    // EXPECT: after remove one 5, count(5): 2
    printf("total: %d\n", ms.total);                    // EXPECT: total: 5

    removed = ms_remove_all(&ms, 3);
    printf("removed all 3s: %d\n", removed);            // EXPECT: removed all 3s: 2
    printf("after remove: ");
    ms_print(&ms);
    printf("\n");                                        // EXPECT: after remove: 1 5 5

    printf("unique: %d\n", ms_unique_count(&ms));       // EXPECT: unique: 2
    printf("total: %d\n", ms.total);                    // EXPECT: total: 3

    ms_insert(&ms, 2);
    ms_insert(&ms, 2);
    ms_insert(&ms, 2);
    ms_insert(&ms, 2);
    printf("mode: %d\n", ms_mode(&ms));                 // EXPECT: mode: 2
    printf("final: ");
    ms_print(&ms);
    printf("\n");                                        // EXPECT: final: 1 2 2 2 2 5 5

    ms_remove_one(&ms, 1);
    printf("min after remove 1: %d\n", ms_min(&ms));    // EXPECT: min after remove 1: 2

    printf("not found remove: %d\n", ms_remove_one(&ms, 99));
    // EXPECT: not found remove: 0

    return 0;
}
