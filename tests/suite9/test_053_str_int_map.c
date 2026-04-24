int printf(const char *fmt, ...);

// String-to-int map using array-based hash table
// Simple string hashing with open addressing

char map_keys[64][16];   // up to 64 entries, each key up to 15 chars
int map_vals[64];
int map_used[64];        // 0=empty, 1=used
int map_cap;

void sm_init(void) {
    int i;
    int j;
    map_cap = 64;
    for (i = 0; i < map_cap; i++) {
        map_used[i] = 0;
        map_vals[i] = 0;
        for (j = 0; j < 16; j++) {
            map_keys[i][j] = 0;
        }
    }
}

int sm_strlen(char *s) {
    int n;
    n = 0;
    while (s[n] != 0) n = n + 1;
    return n;
}

int sm_strcmp(char *a, char *b) {
    int i;
    i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return a[i] - b[i];
        i = i + 1;
    }
    return a[i] - b[i];
}

void sm_strcpy(char *dst, char *src) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int sm_hash(char *s) {
    int h;
    int i;
    h = 0;
    i = 0;
    while (s[i] != 0) {
        h = h * 31 + s[i];
        if (h < 0) h = -h;
        i = i + 1;
    }
    return h % map_cap;
}

void sm_put(char *key, int val) {
    int idx;
    int start;
    idx = sm_hash(key);
    start = idx;
    while (1) {
        if (map_used[idx] == 0) {
            sm_strcpy(map_keys[idx], key);
            map_vals[idx] = val;
            map_used[idx] = 1;
            return;
        }
        if (sm_strcmp(map_keys[idx], key) == 0) {
            map_vals[idx] = val;
            return;
        }
        idx = (idx + 1) % map_cap;
        if (idx == start) return;
    }
}

int sm_get(char *key, int *found) {
    int idx;
    int start;
    idx = sm_hash(key);
    start = idx;
    while (1) {
        if (map_used[idx] == 0) {
            *found = 0;
            return 0;
        }
        if (sm_strcmp(map_keys[idx], key) == 0) {
            *found = 1;
            return map_vals[idx];
        }
        idx = (idx + 1) % map_cap;
        if (idx == start) {
            *found = 0;
            return 0;
        }
    }
    *found = 0;
    return 0;
}

int sm_count(void) {
    int c;
    int i;
    c = 0;
    for (i = 0; i < map_cap; i++) {
        if (map_used[i]) c = c + 1;
    }
    return c;
}

int main(void) {
    int found;
    int val;

    sm_init();
    // EXPECT: init count=0
    printf("init count=%d\n", sm_count());

    sm_put("apple", 5);
    sm_put("banana", 3);
    sm_put("cherry", 7);
    sm_put("date", 2);
    sm_put("elder", 9);
    // EXPECT: inserted count=5
    printf("inserted count=%d\n", sm_count());

    val = sm_get("apple", &found);
    // EXPECT: apple: found=1 val=5
    printf("apple: found=%d val=%d\n", found, val);

    val = sm_get("cherry", &found);
    // EXPECT: cherry: found=1 val=7
    printf("cherry: found=%d val=%d\n", found, val);

    val = sm_get("grape", &found);
    // EXPECT: grape: found=0
    printf("grape: found=%d\n", found);

    sm_put("apple", 42);
    val = sm_get("apple", &found);
    // EXPECT: apple updated: val=42
    printf("apple updated: val=%d\n", val);

    sm_put("fig", 11);
    sm_put("grape", 13);
    sm_put("honey", 17);
    // EXPECT: more inserts count=8
    printf("more inserts count=%d\n", sm_count());

    val = sm_get("honey", &found);
    // EXPECT: honey: found=1 val=17
    printf("honey: found=%d val=%d\n", found, val);

    val = sm_get("date", &found);
    // EXPECT: date: found=1 val=2
    printf("date: found=%d val=%d\n", found, val);

    val = sm_get("zzz", &found);
    // EXPECT: zzz: found=0
    printf("zzz: found=%d\n", found);

    // Test with single char keys
    sm_put("a", 1);
    sm_put("b", 2);
    sm_put("c", 3);
    val = sm_get("b", &found);
    // EXPECT: b: found=1 val=2
    printf("b: found=%d val=%d\n", found, val);

    // EXPECT: final count=11
    printf("final count=%d\n", sm_count());

    // EXPECT: done
    printf("done\n");

    return 0;
}
