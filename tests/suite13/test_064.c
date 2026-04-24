int printf(const char *fmt, ...);

// Frequency counting with hash map

enum { MAP_SIZE = 32, UNUSED = -1000000 };

struct FreqEntry {
    int key;
    int freq;
};

struct FreqMap {
    struct FreqEntry entries[32];
    int count;
};

void fm_init(struct FreqMap *fm) {
    int i;
    for (i = 0; i < MAP_SIZE; i++) {
        fm->entries[i].key = UNUSED;
        fm->entries[i].freq = 0;
    }
    fm->count = 0;
}

int fm_hash(int key) {
    int h = key;
    if (h < 0) h = -h;
    return h % MAP_SIZE;
}

int fm_find_slot(struct FreqMap *fm, int key) {
    int idx;
    int start;
    idx = fm_hash(key);
    start = idx;
    while (1) {
        if (fm->entries[idx].key == UNUSED || fm->entries[idx].key == key) {
            return idx;
        }
        idx = (idx + 1) % MAP_SIZE;
        if (idx == start) return -1;
    }
    return -1;
}

void fm_increment(struct FreqMap *fm, int key) {
    int slot;
    slot = fm_find_slot(fm, key);
    if (slot < 0) return;
    if (fm->entries[slot].key == UNUSED) {
        fm->entries[slot].key = key;
        fm->entries[slot].freq = 1;
        fm->count = fm->count + 1;
    } else {
        fm->entries[slot].freq = fm->entries[slot].freq + 1;
    }
}

int fm_get_freq(struct FreqMap *fm, int key) {
    int slot;
    slot = fm_find_slot(fm, key);
    if (slot < 0) return 0;
    if (fm->entries[slot].key == UNUSED) return 0;
    return fm->entries[slot].freq;
}

int fm_max_freq_key(struct FreqMap *fm) {
    int i;
    int max_freq;
    int max_key;
    max_freq = 0;
    max_key = UNUSED;
    for (i = 0; i < MAP_SIZE; i++) {
        if (fm->entries[i].key != UNUSED) {
            if (fm->entries[i].freq > max_freq) {
                max_freq = fm->entries[i].freq;
                max_key = fm->entries[i].key;
            }
        }
    }
    return max_key;
}

int fm_count_unique(struct FreqMap *fm) {
    return fm->count;
}

int fm_keys_with_freq(struct FreqMap *fm, int target_freq, int *out, int max_out) {
    int i;
    int n;
    n = 0;
    for (i = 0; i < MAP_SIZE; i++) {
        if (fm->entries[i].key != UNUSED && fm->entries[i].freq == target_freq) {
            if (n < max_out) {
                out[n] = fm->entries[i].key;
                n = n + 1;
            }
        }
    }
    return n;
}

void sort_arr(int *a, int n) {
    int i;
    int j;
    int t;
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (a[j] < a[i]) {
                t = a[i];
                a[i] = a[j];
                a[j] = t;
            }
        }
    }
}

int main() {
    struct FreqMap fm;
    int data[20];
    int result[32];
    int n;
    int i;

    data[0] = 5;  data[1] = 3;  data[2] = 5;  data[3] = 7;
    data[4] = 3;  data[5] = 5;  data[6] = 1;  data[7] = 7;
    data[8] = 5;  data[9] = 3;  data[10] = 1; data[11] = 5;
    data[12] = 7; data[13] = 3; data[14] = 9; data[15] = 2;

    fm_init(&fm);

    for (i = 0; i < 16; i++) {
        fm_increment(&fm, data[i]);
    }

    printf("%d\n", fm_count_unique(&fm));
    // EXPECT: 6

    printf("%d\n", fm_get_freq(&fm, 5));
    // EXPECT: 5

    printf("%d\n", fm_get_freq(&fm, 3));
    // EXPECT: 4

    printf("%d\n", fm_get_freq(&fm, 7));
    // EXPECT: 3

    printf("%d\n", fm_get_freq(&fm, 1));
    // EXPECT: 2

    printf("%d\n", fm_get_freq(&fm, 9));
    // EXPECT: 1

    printf("%d\n", fm_get_freq(&fm, 2));
    // EXPECT: 1

    printf("%d\n", fm_get_freq(&fm, 42));
    // EXPECT: 0

    printf("%d\n", fm_max_freq_key(&fm));
    // EXPECT: 5

    n = fm_keys_with_freq(&fm, 1, result, 32);
    sort_arr(result, n);
    printf("%d\n", n);
    // EXPECT: 2
    for (i = 0; i < n; i++) {
        printf("%d\n", result[i]);
    }
    // EXPECT: 2
    // EXPECT: 9

    n = fm_keys_with_freq(&fm, 2, result, 32);
    printf("%d\n", n);
    // EXPECT: 1
    printf("%d\n", result[0]);
    // EXPECT: 1

    fm_increment(&fm, 42);
    fm_increment(&fm, 42);
    fm_increment(&fm, 42);
    printf("%d\n", fm_get_freq(&fm, 42));
    // EXPECT: 3

    printf("%d\n", fm_count_unique(&fm));
    // EXPECT: 7

    n = fm_keys_with_freq(&fm, 3, result, 32);
    sort_arr(result, n);
    printf("%d\n", n);
    // EXPECT: 2
    for (i = 0; i < n; i++) {
        printf("%d\n", result[i]);
    }
    // EXPECT: 7
    // EXPECT: 42

    return 0;
}
