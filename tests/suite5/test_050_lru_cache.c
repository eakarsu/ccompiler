int printf(const char *fmt, ...);

int cache_keys[16];
int cache_vals[16];
int cache_prev[16];
int cache_next[16];
int cache_used[16];
int cache_cap;
int cache_count;
int head;
int tail;

void lru_init(int capacity) {
    int i;
    cache_cap = capacity;
    cache_count = 0;
    head = -1;
    tail = -1;
    i = 0;
    while (i < 16) {
        cache_used[i] = 0;
        cache_prev[i] = -1;
        cache_next[i] = -1;
        i = i + 1;
    }
}

void remove_node(int idx) {
    int p;
    int n;
    p = cache_prev[idx];
    n = cache_next[idx];
    if (p != -1) {
        cache_next[p] = n;
    } else {
        head = n;
    }
    if (n != -1) {
        cache_prev[n] = p;
    } else {
        tail = p;
    }
    cache_prev[idx] = -1;
    cache_next[idx] = -1;
}

void move_to_front(int idx) {
    if (head == idx) return;
    remove_node(idx);
    cache_next[idx] = head;
    cache_prev[idx] = -1;
    if (head != -1) {
        cache_prev[head] = idx;
    }
    head = idx;
    if (tail == -1) {
        tail = idx;
    }
}

void add_to_front(int idx) {
    cache_next[idx] = head;
    cache_prev[idx] = -1;
    if (head != -1) {
        cache_prev[head] = idx;
    }
    head = idx;
    if (tail == -1) {
        tail = idx;
    }
}

int find_slot(void) {
    int i;
    i = 0;
    while (i < 16) {
        if (cache_used[i] == 0) return i;
        i = i + 1;
    }
    return -1;
}

int find_key(int key) {
    int i;
    i = 0;
    while (i < 16) {
        if (cache_used[i] && cache_keys[i] == key) {
            return i;
        }
        i = i + 1;
    }
    return -1;
}

int lru_get(int key) {
    int idx;
    idx = find_key(key);
    if (idx == -1) return -1;
    move_to_front(idx);
    return cache_vals[idx];
}

void lru_put(int key, int val) {
    int idx;
    int evict;

    idx = find_key(key);
    if (idx != -1) {
        cache_vals[idx] = val;
        move_to_front(idx);
        return;
    }

    if (cache_count == cache_cap) {
        evict = tail;
        remove_node(evict);
        cache_used[evict] = 0;
        cache_count = cache_count - 1;
    }

    idx = find_slot();
    cache_keys[idx] = key;
    cache_vals[idx] = val;
    cache_used[idx] = 1;
    add_to_front(idx);
    cache_count = cache_count + 1;
}

int lru_head_key(void) {
    if (head == -1) return -1;
    return cache_keys[head];
}

int lru_tail_key(void) {
    if (tail == -1) return -1;
    return cache_keys[tail];
}

int main(void) {
    int val;

    lru_init(3);

    lru_put(1, 10);
    lru_put(2, 20);
    lru_put(3, 30);

    // EXPECT: count: 3
    printf("count: %d\n", cache_count);

    val = lru_get(1);
    // EXPECT: get 1: 10
    printf("get 1: %d\n", val);

    val = lru_get(2);
    // EXPECT: get 2: 20
    printf("get 2: %d\n", val);

    val = lru_get(3);
    // EXPECT: get 3: 30
    printf("get 3: %d\n", val);

    // EXPECT: mru key: 3
    printf("mru key: %d\n", lru_head_key());

    val = lru_get(1);
    // EXPECT: after access 1 mru: 1
    printf("after access 1 mru: %d\n", lru_head_key());

    // EXPECT: lru key: 2
    printf("lru key: %d\n", lru_tail_key());

    lru_put(4, 40);
    // EXPECT: count after evict: 3
    printf("count after evict: %d\n", cache_count);

    val = lru_get(2);
    // EXPECT: evicted 2: -1
    printf("evicted 2: %d\n", val);

    val = lru_get(4);
    // EXPECT: get 4: 40
    printf("get 4: %d\n", val);

    val = lru_get(1);
    // EXPECT: get 1 still: 10
    printf("get 1 still: %d\n", val);

    lru_put(5, 50);
    val = lru_get(3);
    // EXPECT: evicted 3: -1
    printf("evicted 3: %d\n", val);

    val = lru_get(5);
    // EXPECT: get 5: 50
    printf("get 5: %d\n", val);

    lru_put(1, 100);
    val = lru_get(1);
    // EXPECT: updated 1: 100
    printf("updated 1: %d\n", val);

    // EXPECT: mru: 1
    printf("mru: %d\n", lru_head_key());

    // EXPECT: lru: 4
    printf("lru: %d\n", lru_tail_key());

    lru_put(6, 60);
    val = lru_get(4);
    // EXPECT: evicted 4: -1
    printf("evicted 4: %d\n", val);

    val = lru_get(6);
    // EXPECT: get 6: 60
    printf("get 6: %d\n", val);

    return 0;
}
