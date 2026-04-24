int printf(const char *fmt, ...);

// Memory Pool with Free List Simulation
// Fixed-size block pool with explicit free list management

int pool_data[32];     // 32 blocks of data
int pool_next[32];     // next pointer for free list (-1 = end)
int pool_in_use[32];   // 1 = allocated, 0 = free
int free_head;         // head of free list
int total_allocs;
int total_frees;
int high_watermark;
int current_used;

void pool_init() {
    int i;
    for (i = 0; i < 32; i++) {
        pool_data[i] = 0;
        pool_in_use[i] = 0;
        if (i < 31) {
            pool_next[i] = i + 1;
        } else {
            pool_next[i] = -1;
        }
    }
    free_head = 0;
    total_allocs = 0;
    total_frees = 0;
    high_watermark = 0;
    current_used = 0;
}

int pool_alloc(int value) {
    if (free_head == -1) return -1;

    int slot;
    slot = free_head;
    free_head = pool_next[slot];
    pool_next[slot] = -1;
    pool_in_use[slot] = 1;
    pool_data[slot] = value;

    total_allocs = total_allocs + 1;
    current_used = current_used + 1;
    if (current_used > high_watermark) {
        high_watermark = current_used;
    }
    return slot;
}

int pool_free(int slot) {
    if (slot < 0 || slot >= 32) return -1;
    if (pool_in_use[slot] == 0) return -1; // double free detection

    pool_in_use[slot] = 0;
    pool_data[slot] = 0;
    pool_next[slot] = free_head;
    free_head = slot;

    total_frees = total_frees + 1;
    current_used = current_used - 1;
    return 0;
}

int pool_get(int slot) {
    if (slot < 0 || slot >= 32) return -1;
    if (pool_in_use[slot] == 0) return -1;
    return pool_data[slot];
}

int pool_set(int slot, int value) {
    if (slot < 0 || slot >= 32) return -1;
    if (pool_in_use[slot] == 0) return -1;
    pool_data[slot] = value;
    return 0;
}

int count_free() {
    int count;
    int cur;
    count = 0;
    cur = free_head;
    while (cur != -1) {
        count = count + 1;
        cur = pool_next[cur];
    }
    return count;
}

int verify_free_list() {
    // Verify that free list length + used count = 32
    int free_count;
    int used_count;
    int i;
    free_count = count_free();
    used_count = 0;
    for (i = 0; i < 32; i++) {
        if (pool_in_use[i] == 1) used_count = used_count + 1;
    }
    if (free_count + used_count == 32) return 1;
    return 0;
}

int detect_double_free(int slot) {
    // Returns 1 if slot is already free (double free), 0 otherwise
    if (slot < 0 || slot >= 32) return 1;
    if (pool_in_use[slot] == 0) return 1;
    return 0;
}

void print_stats() {
    printf("Allocs: %d, Frees: %d, Used: %d, Free: %d\n",
           total_allocs, total_frees, current_used, count_free());
    printf("High watermark: %d\n", high_watermark);
}

int main() {
    int a;
    int b;
    int c;
    int d;
    int e;

    pool_init();
    printf("Memory Pool with Free List\n");
    // EXPECT: Memory Pool with Free List

    printf("Initial free: %d\n", count_free());
    // EXPECT: Initial free: 32

    // Allocate several blocks
    a = pool_alloc(10);
    b = pool_alloc(20);
    c = pool_alloc(30);
    d = pool_alloc(40);
    e = pool_alloc(50);
    printf("Allocated slots: %d %d %d %d %d\n", a, b, c, d, e);
    // EXPECT: Allocated slots: 0 1 2 3 4

    printf("Values: %d %d %d %d %d\n",
           pool_get(a), pool_get(b), pool_get(c), pool_get(d), pool_get(e));
    // EXPECT: Values: 10 20 30 40 50

    print_stats();
    // EXPECT: Allocs: 5, Frees: 0, Used: 5, Free: 27
    // EXPECT: High watermark: 5

    // Free middle blocks
    pool_free(b);
    pool_free(d);
    printf("Freed slots 1 and 3\n");
    // EXPECT: Freed slots 1 and 3

    printf("Free count: %d\n", count_free());
    // EXPECT: Free count: 29

    // Reallocate - should get freed slots back (LIFO order)
    int f;
    int g;
    f = pool_alloc(60);
    g = pool_alloc(70);
    printf("Realloc slots: %d %d\n", f, g);
    // EXPECT: Realloc slots: 3 1

    printf("Values: %d %d\n", pool_get(f), pool_get(g));
    // EXPECT: Values: 60 70

    // Verify free list consistency
    int valid;
    valid = verify_free_list();
    printf("Free list valid: %d\n", valid);
    // EXPECT: Free list valid: 1

    // Test double free detection
    pool_free(a);
    int df;
    df = detect_double_free(a);
    printf("Double free detect on freed slot: %d\n", df);
    // EXPECT: Double free detect on freed slot: 1

    df = detect_double_free(c);
    printf("Double free detect on used slot: %d\n", df);
    // EXPECT: Double free detect on used slot: 0

    // Modify value
    pool_set(c, 999);
    printf("Modified slot 2: %d\n", pool_get(c));
    // EXPECT: Modified slot 2: 999

    // Access freed slot
    int bad;
    bad = pool_get(a);
    printf("Access freed slot: %d\n", bad);
    // EXPECT: Access freed slot: -1

    print_stats();
    // EXPECT: Allocs: 7, Frees: 3, Used: 4, Free: 28
    // EXPECT: High watermark: 5

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
