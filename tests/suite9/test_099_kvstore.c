int printf(const char *fmt, ...);

struct KVPair {
    char key[32];
    char value[64];
    int active;
};

struct KVStore {
    struct KVPair entries[32];
    int count;
    int capacity;
};

void str_copy(char *dst, const char *src) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int str_eq(const char *a, const char *b) {
    int i;
    i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

int str_len(const char *s) {
    int i;
    i = 0;
    while (s[i] != 0) i = i + 1;
    return i;
}

void init_store(struct KVStore *store) {
    int i;
    store->count = 0;
    store->capacity = 32;
    for (i = 0; i < 32; i = i + 1) {
        store->entries[i].active = 0;
    }
}

int find_key(struct KVStore *store, const char *key) {
    int i;
    for (i = 0; i < store->count; i = i + 1) {
        if (store->entries[i].active && str_eq(store->entries[i].key, key)) {
            return i;
        }
    }
    return -1;
}

void kv_set(struct KVStore *store, const char *key, const char *value) {
    int idx;
    idx = find_key(store, key);
    if (idx >= 0) {
        str_copy(store->entries[idx].value, value);
    } else {
        idx = store->count;
        str_copy(store->entries[idx].key, key);
        str_copy(store->entries[idx].value, value);
        store->entries[idx].active = 1;
        store->count = store->count + 1;
    }
}

const char *kv_get(struct KVStore *store, const char *key) {
    int idx;
    idx = find_key(store, key);
    if (idx >= 0) return store->entries[idx].value;
    return "(null)";
}

int kv_exists(struct KVStore *store, const char *key) {
    return find_key(store, key) >= 0;
}

int kv_delete(struct KVStore *store, const char *key) {
    int idx;
    idx = find_key(store, key);
    if (idx < 0) return 0;
    store->entries[idx].active = 0;
    return 1;
}

int kv_active_count(struct KVStore *store) {
    int i;
    int c;
    c = 0;
    for (i = 0; i < store->count; i = i + 1) {
        if (store->entries[i].active) c = c + 1;
    }
    return c;
}

void kv_dump(struct KVStore *store) {
    int i;
    for (i = 0; i < store->count; i = i + 1) {
        if (store->entries[i].active) {
            printf("  %s = %s\n", store->entries[i].key, store->entries[i].value);
        }
    }
}

void parse_config_line(struct KVStore *store, const char *line) {
    char key[32];
    char val[64];
    int i;
    int ki;
    int vi;
    i = 0;
    ki = 0;
    while (line[i] != 0 && line[i] != '=') {
        if (line[i] != ' ') {
            key[ki] = line[i];
            ki = ki + 1;
        }
        i = i + 1;
    }
    key[ki] = 0;
    if (line[i] == '=') i = i + 1;
    while (line[i] == ' ') i = i + 1;
    vi = 0;
    while (line[i] != 0) {
        val[vi] = line[i];
        vi = vi + 1;
        i = i + 1;
    }
    val[vi] = 0;
    kv_set(store, key, val);
}

int starts_with(const char *s, const char *prefix) {
    int i;
    i = 0;
    while (prefix[i] != 0) {
        if (s[i] != prefix[i]) return 0;
        i = i + 1;
    }
    return 1;
}

int count_with_prefix(struct KVStore *store, const char *prefix) {
    int i;
    int c;
    c = 0;
    for (i = 0; i < store->count; i = i + 1) {
        if (store->entries[i].active && starts_with(store->entries[i].key, prefix)) {
            c = c + 1;
        }
    }
    return c;
}

int main() {
    struct KVStore store;

    // EXPECT: === KV Store Config Test ===
    printf("=== KV Store Config Test ===\n");

    init_store(&store);

    parse_config_line(&store, "db.host = localhost");
    parse_config_line(&store, "db.port = 5432");
    parse_config_line(&store, "db.name = mydb");
    parse_config_line(&store, "app.debug = true");
    parse_config_line(&store, "app.port = 8080");
    parse_config_line(&store, "app.name = MyApp");
    parse_config_line(&store, "log.level = info");
    parse_config_line(&store, "log.file = app.log");

    // EXPECT: Active entries: 8
    printf("Active entries: %d\n", kv_active_count(&store));

    // EXPECT: db.host = localhost
    printf("db.host = %s\n", kv_get(&store, "db.host"));
    // EXPECT: db.port = 5432
    printf("db.port = %s\n", kv_get(&store, "db.port"));
    // EXPECT: app.debug = true
    printf("app.debug = %s\n", kv_get(&store, "app.debug"));
    // EXPECT: app.name = MyApp
    printf("app.name = %s\n", kv_get(&store, "app.name"));

    // EXPECT: db keys: 3
    printf("db keys: %d\n", count_with_prefix(&store, "db"));
    // EXPECT: app keys: 3
    printf("app keys: %d\n", count_with_prefix(&store, "app"));
    // EXPECT: log keys: 2
    printf("log keys: %d\n", count_with_prefix(&store, "log"));

    kv_set(&store, "db.host", "192.168.1.100");
    // EXPECT: Updated db.host = 192.168.1.100
    printf("Updated db.host = %s\n", kv_get(&store, "db.host"));

    // EXPECT: Exists db.name: 1
    printf("Exists db.name: %d\n", kv_exists(&store, "db.name"));
    // EXPECT: Exists db.password: 0
    printf("Exists db.password: %d\n", kv_exists(&store, "db.password"));

    kv_delete(&store, "log.file");
    // EXPECT: After delete log.file: 7
    printf("After delete log.file: %d\n", kv_active_count(&store));

    // EXPECT: Missing key: (null)
    printf("Missing key: %s\n", kv_get(&store, "nonexistent"));

    // EXPECT: All active config:
    printf("All active config:\n");
    // EXPECT:   db.host = 192.168.1.100
    // EXPECT:   db.port = 5432
    // EXPECT:   db.name = mydb
    // EXPECT:   app.debug = true
    // EXPECT:   app.port = 8080
    // EXPECT:   app.name = MyApp
    // EXPECT:   log.level = info
    kv_dump(&store);

    // EXPECT: === Done ===
    printf("=== Done ===\n");
    return 0;
}
