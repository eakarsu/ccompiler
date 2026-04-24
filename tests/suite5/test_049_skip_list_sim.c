int printf(const char *fmt, ...);

int keys[64];
int levels[64];
int sl_count;

int next0[64];
int next1[64];
int next2[64];

void sl_init(void) {
    sl_count = 0;
    int i;
    i = 0;
    while (i < 64) {
        next0[i] = -1;
        next1[i] = -1;
        next2[i] = -1;
        i = i + 1;
    }
}

int pseudo_random_level(int val) {
    int h;
    h = val * 2654435761;
    if (h < 0) h = -h;
    h = h % 100;
    if (h < 25) return 2;
    if (h < 50) return 1;
    return 0;
}

int sl_find_pos(int key) {
    int i;
    i = 0;
    while (i < sl_count) {
        if (keys[i] >= key) return i;
        i = i + 1;
    }
    return sl_count;
}

void sl_insert(int key) {
    int pos;
    int i;
    int lvl;

    pos = sl_find_pos(key);

    i = sl_count;
    while (i > pos) {
        keys[i] = keys[i - 1];
        levels[i] = levels[i - 1];
        i = i - 1;
    }

    lvl = pseudo_random_level(key);
    keys[pos] = key;
    levels[pos] = lvl;
    sl_count = sl_count + 1;

    i = 0;
    while (i < sl_count) {
        if (i < sl_count - 1) {
            next0[i] = i + 1;
        } else {
            next0[i] = -1;
        }
        next1[i] = -1;
        next2[i] = -1;
        i = i + 1;
    }

    i = 0;
    while (i < sl_count) {
        if (levels[i] >= 1) {
            int j;
            j = i + 1;
            while (j < sl_count) {
                if (levels[j] >= 1) {
                    next1[i] = j;
                    break;
                }
                j = j + 1;
            }
        }
        if (levels[i] >= 2) {
            int j;
            j = i + 1;
            while (j < sl_count) {
                if (levels[j] >= 2) {
                    next2[i] = j;
                    break;
                }
                j = j + 1;
            }
        }
        i = i + 1;
    }
}

int sl_search(int key) {
    int i;
    int hops;
    hops = 0;

    i = 0;
    if (sl_count == 0) return -1;

    while (i < sl_count && i != -1) {
        hops = hops + 1;
        if (keys[i] == key) return hops;
        if (keys[i] > key) return -1;

        if (next2[i] != -1 && keys[next2[i]] <= key) {
            i = next2[i];
        } else if (next1[i] != -1 && keys[next1[i]] <= key) {
            i = next1[i];
        } else if (next0[i] != -1) {
            i = next0[i];
        } else {
            return -1;
        }
    }
    return -1;
}

int sl_contains(int key) {
    int i;
    i = 0;
    while (i < sl_count) {
        if (keys[i] == key) return 1;
        i = i + 1;
    }
    return 0;
}

int count_at_level(int lvl) {
    int cnt;
    int i;
    cnt = 0;
    i = 0;
    while (i < sl_count) {
        if (levels[i] >= lvl) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

int main(void) {
    int ret;

    sl_init();

    sl_insert(10);
    sl_insert(30);
    sl_insert(20);
    sl_insert(50);
    sl_insert(40);

    // EXPECT: count: 5
    printf("count: %d\n", sl_count);

    // EXPECT: sorted: 10 20 30 40 50
    printf("sorted: %d %d %d %d %d\n",
        keys[0], keys[1], keys[2], keys[3], keys[4]);

    ret = sl_contains(30);
    // EXPECT: contains 30: 1
    printf("contains 30: %d\n", ret);

    ret = sl_contains(25);
    // EXPECT: contains 25: 0
    printf("contains 25: %d\n", ret);

    ret = sl_contains(10);
    // EXPECT: contains 10: 1
    printf("contains 10: %d\n", ret);

    ret = sl_contains(50);
    // EXPECT: contains 50: 1
    printf("contains 50: %d\n", ret);

    // EXPECT: level 0 count: 5
    printf("level 0 count: %d\n", count_at_level(0));

    ret = count_at_level(1);
    // EXPECT: level 1 count: 2
    printf("level 1 count: %d\n", ret);

    ret = count_at_level(2);
    // EXPECT: level 2 count: 0
    printf("level 2 count: %d\n", ret);

    sl_insert(5);
    sl_insert(60);
    sl_insert(35);

    // EXPECT: count: 8
    printf("count: %d\n", sl_count);

    // EXPECT: first: 5
    printf("first: %d\n", keys[0]);

    // EXPECT: last: 60
    printf("last: %d\n", keys[sl_count - 1]);

    ret = sl_contains(35);
    // EXPECT: contains 35: 1
    printf("contains 35: %d\n", ret);

    ret = sl_search(35);
    // EXPECT: search 35 found: 1
    printf("search 35 found: %d\n", ret > 0 ? 1 : 0);

    ret = sl_search(99);
    // EXPECT: search 99 found: 0
    printf("search 99 found: %d\n", ret > 0 ? 1 : 0);

    return 0;
}
