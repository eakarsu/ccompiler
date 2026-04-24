int printf(const char *fmt, ...);

// Chaining hash map using array-based linked list simulation
// Each bucket is a head index into a node pool

int node_keys[256];
int node_vals[256];
int node_next[256];  // -1 = end of chain
int node_count;

int buckets[32];     // head index for each bucket, -1 = empty
int num_buckets;

void ch_init(void) {
    int i;
    num_buckets = 32;
    node_count = 0;
    for (i = 0; i < num_buckets; i++) {
        buckets[i] = -1;
    }
    for (i = 0; i < 256; i++) {
        node_keys[i] = 0;
        node_vals[i] = 0;
        node_next[i] = -1;
    }
}

int ch_hash(int key) {
    int h;
    h = key;
    if (h < 0) h = -h;
    return h % num_buckets;
}

int ch_alloc_node(int key, int val) {
    int idx;
    idx = node_count;
    node_count = node_count + 1;
    node_keys[idx] = key;
    node_vals[idx] = val;
    node_next[idx] = -1;
    return idx;
}

void ch_put(int key, int val) {
    int b;
    int cur;
    b = ch_hash(key);
    cur = buckets[b];
    while (cur != -1) {
        if (node_keys[cur] == key) {
            node_vals[cur] = val;
            return;
        }
        cur = node_next[cur];
    }
    // Not found, prepend new node
    {
        int n;
        n = ch_alloc_node(key, val);
        node_next[n] = buckets[b];
        buckets[b] = n;
    }
}

int ch_get(int key, int *found) {
    int b;
    int cur;
    b = ch_hash(key);
    cur = buckets[b];
    while (cur != -1) {
        if (node_keys[cur] == key) {
            *found = 1;
            return node_vals[cur];
        }
        cur = node_next[cur];
    }
    *found = 0;
    return 0;
}

int ch_chain_len(int b) {
    int cur;
    int len;
    len = 0;
    cur = buckets[b];
    while (cur != -1) {
        len = len + 1;
        cur = node_next[cur];
    }
    return len;
}

int ch_total_entries(void) {
    int total;
    int i;
    total = 0;
    for (i = 0; i < num_buckets; i++) {
        total = total + ch_chain_len(i);
    }
    return total;
}

int ch_max_chain(void) {
    int mx;
    int i;
    int cl;
    mx = 0;
    for (i = 0; i < num_buckets; i++) {
        cl = ch_chain_len(i);
        if (cl > mx) mx = cl;
    }
    return mx;
}

int main(void) {
    int found;
    int val;
    int i;

    ch_init();
    // EXPECT: init total=0
    printf("init total=%d\n", ch_total_entries());

    ch_put(1, 10);
    ch_put(2, 20);
    ch_put(3, 30);
    ch_put(4, 40);
    ch_put(5, 50);
    // EXPECT: inserted 5 total=5
    printf("inserted 5 total=%d\n", ch_total_entries());

    val = ch_get(3, &found);
    // EXPECT: get 3: found=1 val=30
    printf("get 3: found=%d val=%d\n", found, val);

    val = ch_get(99, &found);
    // EXPECT: get 99: found=0
    printf("get 99: found=%d\n", found);

    // Update existing key
    ch_put(3, 300);
    val = ch_get(3, &found);
    // EXPECT: updated 3: val=300
    printf("updated 3: val=%d\n", val);

    // Insert colliding keys (all hash to same bucket: 0, 32, 64)
    ch_put(0, 100);
    ch_put(32, 200);
    ch_put(64, 300);
    // EXPECT: bucket 0 chain=3
    printf("bucket 0 chain=%d\n", ch_chain_len(0));

    val = ch_get(0, &found);
    // EXPECT: get 0: val=100
    printf("get 0: val=%d\n", val);
    val = ch_get(32, &found);
    // EXPECT: get 32: val=200
    printf("get 32: val=%d\n", val);
    val = ch_get(64, &found);
    // EXPECT: get 64: val=300
    printf("get 64: val=%d\n", val);

    // Bulk insert
    for (i = 100; i < 130; i++) {
        ch_put(i, i * 3);
    }
    // EXPECT: after bulk total=38
    printf("after bulk total=%d\n", ch_total_entries());

    val = ch_get(115, &found);
    // EXPECT: bulk get 115: found=1 val=345
    printf("bulk get 115: found=%d val=%d\n", found, val);

    // EXPECT: max chain len=4
    printf("max chain len=%d\n", ch_max_chain());

    // EXPECT: nodes allocated=38
    printf("nodes allocated=%d\n", node_count);

    // EXPECT: done
    printf("done\n");

    return 0;
}
