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

char *ot_keys[32];
int ot_vals[32];
int ot_used[32];
int ot_cap;
int ot_size;
int ot_probes;

void ot_init(int cap) {
    ot_cap = cap;
    ot_size = 0;
    ot_probes = 0;
    int i;
    for (i = 0; i < cap; i = i + 1) {
        ot_keys[i] = 0;
        ot_vals[i] = 0;
        ot_used[i] = 0;
    }
}

int ot_insert(char *key, int value) {
    int h = hash_str(key, ot_cap);
    int i;
    int probes = 0;
    for (i = 0; i < ot_cap; i = i + 1) {
        int idx = (h + i) % ot_cap;
        probes = probes + 1;
        if (ot_used[idx] == 0) {
            ot_keys[idx] = key;
            ot_vals[idx] = value;
            ot_used[idx] = 1;
            ot_size = ot_size + 1;
            ot_probes = ot_probes + probes;
            return 1;
        }
        if (str_equal(ot_keys[idx], key)) {
            ot_vals[idx] = value;
            ot_probes = ot_probes + probes;
            return 0;
        }
    }
    return -1;
}

int ot_find(char *key, int *found) {
    int h = hash_str(key, ot_cap);
    int i;
    for (i = 0; i < ot_cap; i = i + 1) {
        int idx = (h + i) % ot_cap;
        if (ot_used[idx] == 0) {
            *found = 0;
            return -1;
        }
        if (str_equal(ot_keys[idx], key)) {
            *found = 1;
            return ot_vals[idx];
        }
    }
    *found = 0;
    return -1;
}

int ot_delete(char *key) {
    int h = hash_str(key, ot_cap);
    int i;
    for (i = 0; i < ot_cap; i = i + 1) {
        int idx = (h + i) % ot_cap;
        if (ot_used[idx] == 0) return 0;
        if (str_equal(ot_keys[idx], key)) {
            ot_used[idx] = 0;
            ot_keys[idx] = 0;
            ot_vals[idx] = 0;
            ot_size = ot_size - 1;
            return 1;
        }
    }
    return 0;
}

int main(void) {
    int found;
    int val;

    ot_init(16);

    // EXPECT: size after init: 0
    printf("size after init: %d\n", ot_size);

    ot_insert("alpha", 1);
    // EXPECT: size after 1: 1
    printf("size after 1: %d\n", ot_size);

    ot_insert("beta", 2);
    ot_insert("gamma", 3);
    ot_insert("delta", 4);
    // EXPECT: size after 4: 4
    printf("size after 4: %d\n", ot_size);

    // EXPECT: find alpha: 1
    val = ot_find("alpha", &found);
    printf("find alpha: %d\n", val);

    // EXPECT: alpha found: 1
    printf("alpha found: %d\n", found);

    // EXPECT: find beta: 2
    val = ot_find("beta", &found);
    printf("find beta: %d\n", val);

    // EXPECT: find gamma: 3
    val = ot_find("gamma", &found);
    printf("find gamma: %d\n", val);

    // EXPECT: find delta: 4
    val = ot_find("delta", &found);
    printf("find delta: %d\n", val);

    // EXPECT: missing not found: 0
    ot_find("omega", &found);
    printf("missing not found: %d\n", found);

    // EXPECT: update returns 0: 0
    int ret = ot_insert("alpha", 100);
    printf("update returns 0: %d\n", ret);

    // EXPECT: updated alpha: 100
    val = ot_find("alpha", &found);
    printf("updated alpha: %d\n", val);

    // EXPECT: size after update: 4
    printf("size after update: %d\n", ot_size);

    // EXPECT: delete gamma: 1
    int d = ot_delete("gamma");
    printf("delete gamma: %d\n", d);

    // EXPECT: size after delete: 3
    printf("size after delete: %d\n", ot_size);

    // EXPECT: gamma gone: 0
    ot_find("gamma", &found);
    printf("gamma gone: %d\n", found);

    // EXPECT: delete missing: 0
    d = ot_delete("omega");
    printf("delete missing: %d\n", d);

    // EXPECT: total probes positive: 1
    printf("total probes positive: %d\n", ot_probes > 0);

    ot_insert("epsilon", 5);
    ot_insert("zeta", 6);
    // EXPECT: final size: 5
    printf("final size: %d\n", ot_size);

    // EXPECT: done
    printf("done\n");

    return 0;
}
