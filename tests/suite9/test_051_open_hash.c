int printf(const char *fmt, ...);

// Open addressing hash map with linear probing
// Keys are non-negative integers, -1 means empty slot

int ht_keys[64];
int ht_vals[64];
int ht_size;
int ht_count;

void ht_init(void) {
    int i;
    ht_size = 64;
    ht_count = 0;
    for (i = 0; i < ht_size; i++) {
        ht_keys[i] = -1;
        ht_vals[i] = 0;
    }
}

int ht_hash(int key) {
    int h;
    h = key;
    if (h < 0) h = -h;
    return h % ht_size;
}

int ht_put(int key, int val) {
    int idx;
    int start;
    idx = ht_hash(key);
    start = idx;
    while (1) {
        if (ht_keys[idx] == -1) {
            ht_keys[idx] = key;
            ht_vals[idx] = val;
            ht_count = ht_count + 1;
            return 1;
        }
        if (ht_keys[idx] == key) {
            ht_vals[idx] = val;
            return 1;
        }
        idx = (idx + 1) % ht_size;
        if (idx == start) return 0;
    }
    return 0;
}

int ht_get(int key, int *found) {
    int idx;
    int start;
    idx = ht_hash(key);
    start = idx;
    while (1) {
        if (ht_keys[idx] == -1) {
            *found = 0;
            return 0;
        }
        if (ht_keys[idx] == key) {
            *found = 1;
            return ht_vals[idx];
        }
        idx = (idx + 1) % ht_size;
        if (idx == start) {
            *found = 0;
            return 0;
        }
    }
    *found = 0;
    return 0;
}

int ht_contains(int key) {
    int found;
    ht_get(key, &found);
    return found;
}

int ht_remove(int key) {
    int idx;
    int start;
    int next;
    int rehash;
    idx = ht_hash(key);
    start = idx;
    while (1) {
        if (ht_keys[idx] == -1) return 0;
        if (ht_keys[idx] == key) {
            ht_keys[idx] = -1;
            ht_vals[idx] = 0;
            ht_count = ht_count - 1;
            next = (idx + 1) % ht_size;
            while (ht_keys[next] != -1) {
                int tk;
                int tv;
                tk = ht_keys[next];
                tv = ht_vals[next];
                ht_keys[next] = -1;
                ht_vals[next] = 0;
                ht_count = ht_count - 1;
                ht_put(tk, tv);
                next = (next + 1) % ht_size;
            }
            return 1;
        }
        idx = (idx + 1) % ht_size;
        if (idx == start) return 0;
    }
    return 0;
}

int main(void) {
    int found;
    int val;

    ht_init();
    // EXPECT: init count=0
    printf("init count=%d\n", ht_count);

    ht_put(10, 100);
    ht_put(20, 200);
    ht_put(30, 300);
    // EXPECT: after inserts count=3
    printf("after inserts count=%d\n", ht_count);

    val = ht_get(10, &found);
    // EXPECT: get 10: found=1 val=100
    printf("get 10: found=%d val=%d\n", found, val);

    val = ht_get(20, &found);
    // EXPECT: get 20: found=1 val=200
    printf("get 20: found=%d val=%d\n", found, val);

    val = ht_get(99, &found);
    // EXPECT: get 99: found=0 val=0
    printf("get 99: found=%d val=%d\n", found, val);

    ht_put(10, 999);
    val = ht_get(10, &found);
    // EXPECT: update 10: val=999
    printf("update 10: val=%d\n", val);

    // EXPECT: contains 30=1
    printf("contains 30=%d\n", ht_contains(30));
    // EXPECT: contains 77=0
    printf("contains 77=%d\n", ht_contains(77));

    // Test collision: keys that hash to same slot
    ht_put(5, 50);
    ht_put(69, 690);  // 69 % 64 = 5, same hash as 5
    val = ht_get(5, &found);
    // EXPECT: collision get 5: found=1 val=50
    printf("collision get 5: found=%d val=%d\n", found, val);
    val = ht_get(69, &found);
    // EXPECT: collision get 69: found=1 val=690
    printf("collision get 69: found=%d val=%d\n", found, val);

    // EXPECT: before remove count=5
    printf("before remove count=%d\n", ht_count);
    ht_remove(20);
    // EXPECT: after remove count=4
    printf("after remove count=%d\n", ht_count);
    // EXPECT: removed 20 contains=0
    printf("removed 20 contains=%d\n", ht_contains(20));
    // EXPECT: still have 30 contains=1
    printf("still have 30 contains=%d\n", ht_contains(30));

    // Bulk insert
    {
        int i;
        for (i = 0; i < 20; i++) {
            ht_put(100 + i, i * 10);
        }
    }
    // EXPECT: bulk count=24
    printf("bulk count=%d\n", ht_count);

    val = ht_get(110, &found);
    // EXPECT: bulk get 110: found=1 val=100
    printf("bulk get 110: found=%d val=%d\n", found, val);

    // EXPECT: done
    printf("done\n");

    return 0;
}
