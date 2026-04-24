int printf(const char *fmt, ...);

// Chained hash table using array-based linked lists
// Each node lives in a pool array; chains are linked by index

enum { BUCKETS = 8, POOL_SIZE = 32, NIL = -1 };

struct Node {
    int key;
    int value;
    int next;
};

struct ChainedHash {
    int buckets[8];
    struct Node pool[32];
    int pool_top;
};

void ch_init(struct ChainedHash *ch) {
    int i;
    for (i = 0; i < BUCKETS; i++) {
        ch->buckets[i] = NIL;
    }
    ch->pool_top = 0;
}

int ch_hash(int key) {
    int h = key;
    if (h < 0) h = -h;
    return h % BUCKETS;
}

int ch_alloc_node(struct ChainedHash *ch, int key, int value) {
    int idx;
    if (ch->pool_top >= POOL_SIZE) return NIL;
    idx = ch->pool_top;
    ch->pool_top = ch->pool_top + 1;
    ch->pool[idx].key = key;
    ch->pool[idx].value = value;
    ch->pool[idx].next = NIL;
    return idx;
}

int ch_insert(struct ChainedHash *ch, int key, int value) {
    int h;
    int cur;
    int node_idx;

    h = ch_hash(key);
    cur = ch->buckets[h];
    while (cur != NIL) {
        if (ch->pool[cur].key == key) {
            ch->pool[cur].value = value;
            return 1;
        }
        cur = ch->pool[cur].next;
    }
    node_idx = ch_alloc_node(ch, key, value);
    if (node_idx == NIL) return 0;
    ch->pool[node_idx].next = ch->buckets[h];
    ch->buckets[h] = node_idx;
    return 1;
}

int ch_get(struct ChainedHash *ch, int key, int *out) {
    int h;
    int cur;
    h = ch_hash(key);
    cur = ch->buckets[h];
    while (cur != NIL) {
        if (ch->pool[cur].key == key) {
            *out = ch->pool[cur].value;
            return 1;
        }
        cur = ch->pool[cur].next;
    }
    return 0;
}

int ch_chain_length(struct ChainedHash *ch, int bucket) {
    int cur;
    int len;
    len = 0;
    cur = ch->buckets[bucket];
    while (cur != NIL) {
        len = len + 1;
        cur = ch->pool[cur].next;
    }
    return len;
}

int ch_total_count(struct ChainedHash *ch) {
    int total;
    int i;
    total = 0;
    for (i = 0; i < BUCKETS; i++) {
        total = total + ch_chain_length(ch, i);
    }
    return total;
}

void ch_collect_keys(struct ChainedHash *ch, int *keys, int *count) {
    int i;
    int cur;
    *count = 0;
    for (i = 0; i < BUCKETS; i++) {
        cur = ch->buckets[i];
        while (cur != NIL) {
            keys[*count] = ch->pool[cur].key;
            *count = *count + 1;
            cur = ch->pool[cur].next;
        }
    }
}

int main() {
    struct ChainedHash ch;
    int val;
    int found;
    int keys[32];
    int key_count;
    int i;
    int sum;

    ch_init(&ch);

    printf("%d\n", ch_total_count(&ch));
    // EXPECT: 0

    ch_insert(&ch, 10, 100);
    ch_insert(&ch, 18, 180);
    ch_insert(&ch, 26, 260);

    printf("%d\n", ch_total_count(&ch));
    // EXPECT: 3

    found = ch_get(&ch, 10, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 100

    found = ch_get(&ch, 18, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 180

    found = ch_get(&ch, 26, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 260

    printf("%d\n", ch_chain_length(&ch, 2));
    // EXPECT: 3

    ch_insert(&ch, 1, 10);
    ch_insert(&ch, 9, 90);
    printf("%d\n", ch_chain_length(&ch, 1));
    // EXPECT: 2

    ch_insert(&ch, 10, 999);
    found = ch_get(&ch, 10, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 999

    printf("%d\n", ch_total_count(&ch));
    // EXPECT: 5

    found = ch_get(&ch, 42, &val);
    printf("%d\n", found);
    // EXPECT: 0

    for (i = 0; i < 8; i++) {
        ch_insert(&ch, 100 + i, i * 10);
    }
    printf("%d\n", ch_total_count(&ch));
    // EXPECT: 13

    found = ch_get(&ch, 105, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 50

    ch_collect_keys(&ch, keys, &key_count);
    printf("%d\n", key_count);
    // EXPECT: 13

    sum = 0;
    for (i = 0; i < key_count; i++) {
        sum = sum + keys[i];
    }
    printf("%d\n", sum);
    // EXPECT: 892

    return 0;
}
