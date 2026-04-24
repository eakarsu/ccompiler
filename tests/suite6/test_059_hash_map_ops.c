int printf(const char *fmt, ...);

int str_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

int hash_str(char *s, int mod) {
    int hash = 5381;
    int i = 0;
    while (s[i] != 0) {
        hash = hash * 33 + s[i];
        i = i + 1;
    }
    if (hash < 0) hash = -hash;
    return hash % mod;
}

char *map_keys[32];
int map_vals[32];
int map_used[32];
int map_size;

void map_init(void) {
    map_size = 0;
    int i;
    for (i = 0; i < 32; i = i + 1) {
        map_keys[i] = 0;
        map_vals[i] = 0;
        map_used[i] = 0;
    }
}

int map_put(char *key, int value) {
    int h = hash_str(key, 32);
    int i;
    for (i = 0; i < 32; i = i + 1) {
        int idx = (h + i) % 32;
        if (map_used[idx] == 0) {
            map_keys[idx] = key;
            map_vals[idx] = value;
            map_used[idx] = 1;
            map_size = map_size + 1;
            return 1;
        }
        if (str_equal(map_keys[idx], key)) {
            map_vals[idx] = value;
            return 0;
        }
    }
    return -1;
}

int map_get(char *key, int *found) {
    int h = hash_str(key, 32);
    int i;
    for (i = 0; i < 32; i = i + 1) {
        int idx = (h + i) % 32;
        if (map_used[idx] == 0) {
            *found = 0;
            return -1;
        }
        if (str_equal(map_keys[idx], key)) {
            *found = 1;
            return map_vals[idx];
        }
    }
    *found = 0;
    return -1;
}

int map_update(char *key, int delta) {
    int h = hash_str(key, 32);
    int i;
    for (i = 0; i < 32; i = i + 1) {
        int idx = (h + i) % 32;
        if (map_used[idx] == 0) return 0;
        if (str_equal(map_keys[idx], key)) {
            map_vals[idx] = map_vals[idx] + delta;
            return 1;
        }
    }
    return 0;
}

int map_delete(char *key) {
    int h = hash_str(key, 32);
    int i;
    for (i = 0; i < 32; i = i + 1) {
        int idx = (h + i) % 32;
        if (map_used[idx] == 0) return 0;
        if (str_equal(map_keys[idx], key)) {
            map_used[idx] = 0;
            map_size = map_size - 1;
            return 1;
        }
    }
    return 0;
}

int map_sum_values(void) {
    int sum = 0;
    int i;
    for (i = 0; i < 32; i = i + 1) {
        if (map_used[i]) {
            sum = sum + map_vals[i];
        }
    }
    return sum;
}

int map_count_above(int threshold) {
    int count = 0;
    int i;
    for (i = 0; i < 32; i = i + 1) {
        if (map_used[i] && map_vals[i] > threshold) {
            count = count + 1;
        }
    }
    return count;
}

int main(void) {
    int found;
    int val;

    map_init();

    // EXPECT: initial size: 0
    printf("initial size: %d\n", map_size);

    map_put("alice", 85);
    map_put("bob", 92);
    map_put("carol", 78);
    // EXPECT: size after 3 puts: 3
    printf("size after 3 puts: %d\n", map_size);

    // EXPECT: get alice: 85
    val = map_get("alice", &found);
    printf("get alice: %d\n", val);

    // EXPECT: get bob: 92
    val = map_get("bob", &found);
    printf("get bob: %d\n", val);

    // EXPECT: get carol: 78
    val = map_get("carol", &found);
    printf("get carol: %d\n", val);

    // EXPECT: get missing: 0
    map_get("dave", &found);
    printf("get missing: %d\n", found);

    // EXPECT: put dup returns 0: 0
    int ret = map_put("alice", 90);
    printf("put dup returns 0: %d\n", ret);

    // EXPECT: alice updated: 90
    val = map_get("alice", &found);
    printf("alice updated: %d\n", val);

    // EXPECT: update bob +8: 1
    ret = map_update("bob", 8);
    printf("update bob +8: %d\n", ret);

    // EXPECT: bob now: 100
    val = map_get("bob", &found);
    printf("bob now: %d\n", val);

    // EXPECT: update missing: 0
    ret = map_update("dave", 5);
    printf("update missing: %d\n", ret);

    // EXPECT: delete carol: 1
    ret = map_delete("carol");
    printf("delete carol: %d\n", ret);

    // EXPECT: size after delete: 2
    printf("size after delete: %d\n", map_size);

    // EXPECT: carol gone: 0
    map_get("carol", &found);
    printf("carol gone: %d\n", found);

    // EXPECT: delete missing: 0
    ret = map_delete("dave");
    printf("delete missing: %d\n", ret);

    // EXPECT: sum values: 190
    printf("sum values: %d\n", map_sum_values());

    map_put("dave", 88);
    map_put("eve", 95);
    map_put("frank", 70);
    // EXPECT: final size: 5
    printf("final size: %d\n", map_size);

    // EXPECT: count above 85: 4
    printf("count above 85: %d\n", map_count_above(85));

    // EXPECT: done
    printf("done\n");

    return 0;
}
