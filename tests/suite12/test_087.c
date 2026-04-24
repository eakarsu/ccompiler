int printf(const char *fmt, ...);

// Memory Leak Detector Simulation
// Tracks allocations and frees, detects leaks at "program exit"

// Allocation tracking table
int alloc_id[32];       // allocation ID
int alloc_size[32];     // size of allocation
int alloc_freed[32];    // 0 = still allocated, 1 = freed
int alloc_line[32];     // simulated "line number" of allocation
int alloc_tag[32];      // user tag for identification
int num_allocs;

// Statistics
int total_bytes_allocated;
int total_bytes_freed;
int peak_bytes;
int current_bytes;
int double_free_count;
int invalid_free_count;

void leak_init() {
    int i;
    for (i = 0; i < 32; i++) {
        alloc_id[i] = -1;
        alloc_size[i] = 0;
        alloc_freed[i] = 1;
        alloc_line[i] = 0;
        alloc_tag[i] = 0;
    }
    num_allocs = 0;
    total_bytes_allocated = 0;
    total_bytes_freed = 0;
    peak_bytes = 0;
    current_bytes = 0;
    double_free_count = 0;
    invalid_free_count = 0;
}

int leak_alloc(int size, int line, int tag) {
    if (num_allocs >= 32) return -1;
    int id;
    id = num_allocs;
    alloc_id[id] = id;
    alloc_size[id] = size;
    alloc_freed[id] = 0;
    alloc_line[id] = line;
    alloc_tag[id] = tag;
    num_allocs = num_allocs + 1;

    total_bytes_allocated = total_bytes_allocated + size;
    current_bytes = current_bytes + size;
    if (current_bytes > peak_bytes) {
        peak_bytes = current_bytes;
    }
    return id;
}

int leak_free(int id) {
    if (id < 0 || id >= 32) {
        invalid_free_count = invalid_free_count + 1;
        return -1;
    }
    if (alloc_id[id] == -1) {
        invalid_free_count = invalid_free_count + 1;
        return -1;
    }
    if (alloc_freed[id] == 1) {
        double_free_count = double_free_count + 1;
        return -2;
    }

    alloc_freed[id] = 1;
    total_bytes_freed = total_bytes_freed + alloc_size[id];
    current_bytes = current_bytes - alloc_size[id];
    return 0;
}

int count_leaks() {
    int i;
    int count;
    count = 0;
    for (i = 0; i < num_allocs; i++) {
        if (alloc_freed[i] == 0) {
            count = count + 1;
        }
    }
    return count;
}

int leaked_bytes() {
    int i;
    int bytes;
    bytes = 0;
    for (i = 0; i < num_allocs; i++) {
        if (alloc_freed[i] == 0) {
            bytes = bytes + alloc_size[i];
        }
    }
    return bytes;
}

void report_leaks() {
    int i;
    int found;
    found = 0;
    for (i = 0; i < num_allocs; i++) {
        if (alloc_freed[i] == 0) {
            printf("LEAK: id=%d, size=%d, line=%d, tag=%d\n",
                   alloc_id[i], alloc_size[i], alloc_line[i], alloc_tag[i]);
            found = found + 1;
        }
    }
    if (found == 0) {
        printf("No leaks detected\n");
    }
}

void print_summary() {
    printf("Total allocs: %d\n", num_allocs);
    printf("Bytes allocated: %d\n", total_bytes_allocated);
    printf("Bytes freed: %d\n", total_bytes_freed);
    printf("Bytes leaked: %d\n", leaked_bytes());
    printf("Peak usage: %d\n", peak_bytes);
    printf("Double frees: %d\n", double_free_count);
    printf("Invalid frees: %d\n", invalid_free_count);
}

int main() {
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int ret;

    leak_init();
    printf("Memory Leak Detector\n");
    // EXPECT: Memory Leak Detector

    // Simulate a program with various alloc/free patterns
    a = leak_alloc(64, 10, 1);   // "buffer"
    b = leak_alloc(128, 15, 2);  // "string"
    c = leak_alloc(32, 20, 3);   // "node"
    d = leak_alloc(256, 25, 4);  // "array"
    e = leak_alloc(16, 30, 5);   // "temp"
    printf("Allocated 5 objects: %d %d %d %d %d\n", a, b, c, d, e);
    // EXPECT: Allocated 5 objects: 0 1 2 3 4

    printf("Current bytes: %d\n", current_bytes);
    // EXPECT: Current bytes: 496

    // Free some properly
    ret = leak_free(a);
    printf("Free a: %d\n", ret);
    // EXPECT: Free a: 0

    ret = leak_free(e);
    printf("Free e: %d\n", ret);
    // EXPECT: Free e: 0

    printf("Current bytes: %d\n", current_bytes);
    // EXPECT: Current bytes: 416

    // Try double free
    ret = leak_free(a);
    printf("Double free a: %d\n", ret);
    // EXPECT: Double free a: -2

    // Try invalid free
    ret = leak_free(-5);
    printf("Invalid free: %d\n", ret);
    // EXPECT: Invalid free: -1

    ret = leak_free(99);
    printf("Invalid free 2: %d\n", ret);
    // EXPECT: Invalid free 2: -1

    // Free one more
    ret = leak_free(c);
    printf("Free c: %d\n", ret);
    // EXPECT: Free c: 0

    // Allocate more
    f = leak_alloc(48, 40, 6);
    printf("New alloc f: id=%d\n", f);
    // EXPECT: New alloc f: id=5

    // Check leak count
    printf("Leak count: %d\n", count_leaks());
    // EXPECT: Leak count: 3

    printf("Leaked bytes: %d\n", leaked_bytes());
    // EXPECT: Leaked bytes: 432

    // Leak report
    printf("--- Leak Report ---\n");
    // EXPECT: --- Leak Report ---
    report_leaks();
    // EXPECT: LEAK: id=1, size=128, line=15, tag=2
    // EXPECT: LEAK: id=3, size=256, line=25, tag=4
    // EXPECT: LEAK: id=5, size=48, line=40, tag=6

    // Summary
    printf("--- Summary ---\n");
    // EXPECT: --- Summary ---
    print_summary();
    // EXPECT: Total allocs: 6
    // EXPECT: Bytes allocated: 544
    // EXPECT: Bytes freed: 112
    // EXPECT: Bytes leaked: 432
    // EXPECT: Peak usage: 496
    // EXPECT: Double frees: 1
    // EXPECT: Invalid frees: 2

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
