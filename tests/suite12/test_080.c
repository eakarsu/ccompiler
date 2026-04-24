int printf(const char *fmt, ...);

// Sharded Key-Value Store
// Simulates a distributed KV store with consistent shard routing

struct KVPair {
    int key;
    int value;
    int occupied;
    int version;
};

struct Shard {
    int shard_id;
    int is_active;
    struct KVPair entries[8];
    int count;
};

struct KVStore {
    struct Shard shards[4];
    int num_shards;
};

void kv_init(struct KVStore *store, int n) {
    int i;
    int j;
    store->num_shards = n;
    for (i = 0; i < n; i = i + 1) {
        store->shards[i].shard_id = i;
        store->shards[i].is_active = 1;
        store->shards[i].count = 0;
        for (j = 0; j < 8; j = j + 1) {
            store->shards[i].entries[j].key = 0;
            store->shards[i].entries[j].value = 0;
            store->shards[i].entries[j].occupied = 0;
            store->shards[i].entries[j].version = 0;
        }
    }
}

int route_key(int key, int num_shards) {
    int h;
    h = key;
    if (h < 0) h = -h;
    return h % num_shards;
}

int shard_put(struct Shard *s, int key, int value, int version) {
    int i;
    // Update existing
    for (i = 0; i < 8; i = i + 1) {
        if (s->entries[i].occupied && s->entries[i].key == key) {
            if (version > s->entries[i].version) {
                s->entries[i].value = value;
                s->entries[i].version = version;
                return 1;
            }
            return 0;  // stale write
        }
    }
    // Insert new
    for (i = 0; i < 8; i = i + 1) {
        if (s->entries[i].occupied == 0) {
            s->entries[i].key = key;
            s->entries[i].value = value;
            s->entries[i].occupied = 1;
            s->entries[i].version = version;
            s->count = s->count + 1;
            return 1;
        }
    }
    return -1;  // full
}

int shard_get(struct Shard *s, int key, int *out_ver) {
    int i;
    for (i = 0; i < 8; i = i + 1) {
        if (s->entries[i].occupied && s->entries[i].key == key) {
            *out_ver = s->entries[i].version;
            return s->entries[i].value;
        }
    }
    *out_ver = -1;
    return -1;  // not found
}

int shard_delete(struct Shard *s, int key) {
    int i;
    for (i = 0; i < 8; i = i + 1) {
        if (s->entries[i].occupied && s->entries[i].key == key) {
            s->entries[i].occupied = 0;
            s->count = s->count - 1;
            return 1;
        }
    }
    return 0;
}

int kv_put(struct KVStore *store, int key, int value, int version) {
    int sid;
    sid = route_key(key, store->num_shards);
    if (store->shards[sid].is_active == 0) {
        printf("  PUT key=%d FAILED: shard %d down\n", key, sid);
        return -1;
    }
    return shard_put(&store->shards[sid], key, value, version);
}

int kv_get(struct KVStore *store, int key, int *out_ver) {
    int sid;
    sid = route_key(key, store->num_shards);
    if (store->shards[sid].is_active == 0) {
        printf("  GET key=%d FAILED: shard %d down\n", key, sid);
        *out_ver = -1;
        return -1;
    }
    return shard_get(&store->shards[sid], key, out_ver);
}

int kv_delete(struct KVStore *store, int key) {
    int sid;
    sid = route_key(key, store->num_shards);
    if (store->shards[sid].is_active == 0) return -1;
    return shard_delete(&store->shards[sid], key);
}

void print_shard_counts(struct KVStore *store) {
    int i;
    printf("Shard counts:");
    for (i = 0; i < store->num_shards; i = i + 1) {
        printf(" S%d=%d", i, store->shards[i].count);
    }
    printf("\n");
}

void print_shard_contents(struct Shard *s) {
    int i;
    printf("Shard %d:", s->shard_id);
    for (i = 0; i < 8; i = i + 1) {
        if (s->entries[i].occupied) {
            printf(" [%d=%d]", s->entries[i].key, s->entries[i].value);
        }
    }
    printf("\n");
}

int main() {
    struct KVStore store;
    int val;
    int ver;
    int res;
    int i;

    kv_init(&store, 4);
    printf("KV store with %d shards\n", store.num_shards);
    // EXPECT: KV store with 4 shards

    // Insert some key-value pairs
    kv_put(&store, 0, 100, 1);
    kv_put(&store, 1, 200, 1);
    kv_put(&store, 2, 300, 1);
    kv_put(&store, 3, 400, 1);
    kv_put(&store, 4, 500, 1);
    kv_put(&store, 5, 600, 1);
    kv_put(&store, 6, 700, 1);
    kv_put(&store, 7, 800, 1);

    print_shard_counts(&store);
    // EXPECT: Shard counts: S0=2 S1=2 S2=2 S3=2

    // Read back values
    val = kv_get(&store, 0, &ver);
    printf("GET 0: val=%d ver=%d\n", val, ver);
    // EXPECT: GET 0: val=100 ver=1

    val = kv_get(&store, 5, &ver);
    printf("GET 5: val=%d ver=%d\n", val, ver);
    // EXPECT: GET 5: val=600 ver=1

    val = kv_get(&store, 7, &ver);
    printf("GET 7: val=%d ver=%d\n", val, ver);
    // EXPECT: GET 7: val=800 ver=1

    // Update a key
    res = kv_put(&store, 3, 999, 2);
    printf("Update key 3: %d\n", res);
    // EXPECT: Update key 3: 1

    val = kv_get(&store, 3, &ver);
    printf("GET 3 after update: val=%d ver=%d\n", val, ver);
    // EXPECT: GET 3 after update: val=999 ver=2

    // Stale write should fail
    res = kv_put(&store, 3, 111, 1);
    printf("Stale write to key 3: %d\n", res);
    // EXPECT: Stale write to key 3: 0

    val = kv_get(&store, 3, &ver);
    printf("GET 3 still: val=%d ver=%d\n", val, ver);
    // EXPECT: GET 3 still: val=999 ver=2

    // Delete a key
    res = kv_delete(&store, 0);
    printf("Delete key 0: %d\n", res);
    // EXPECT: Delete key 0: 1

    val = kv_get(&store, 0, &ver);
    printf("GET 0 after delete: val=%d\n", val);
    // EXPECT: GET 0 after delete: val=-1

    print_shard_counts(&store);
    // EXPECT: Shard counts: S0=1 S1=2 S2=2 S3=2

    // Shard failure
    printf("--- Shard 2 goes down ---\n");
    // EXPECT: --- Shard 2 goes down ---
    store.shards[2].is_active = 0;

    val = kv_get(&store, 2, &ver);
    // EXPECT:   GET key=2 FAILED: shard 2 down
    printf("Result: %d\n", val);
    // EXPECT: Result: -1

    kv_put(&store, 10, 1000, 1);
    // EXPECT:   PUT key=10 FAILED: shard 2 down

    // Key 1 is on shard 1, should still work
    val = kv_get(&store, 1, &ver);
    printf("GET 1 (shard 1): val=%d\n", val);
    // EXPECT: GET 1 (shard 1): val=200

    // Restore shard 2
    store.shards[2].is_active = 1;
    val = kv_get(&store, 2, &ver);
    printf("GET 2 after restore: val=%d\n", val);
    // EXPECT: GET 2 after restore: val=300

    print_shard_contents(&store.shards[0]);
    // EXPECT: Shard 0: [4=500]
    print_shard_contents(&store.shards[1]);
    // EXPECT: Shard 1: [1=200] [5=600]

    printf("Sharded KV store simulation done\n");
    // EXPECT: Sharded KV store simulation done

    return 0;
}
