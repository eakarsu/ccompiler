int printf(const char *fmt, ...);

/* Buddy System Allocator Simulation
 * Total memory: 64 slots. Minimum block: 4 slots. Maximum block: 64 slots.
 * Levels: 0=64, 1=32, 2=16, 3=8, 4=4
 * We track block status in a flat array: 0=free, 1=allocated, 2=split
 * Tree index: root=1, children of i are 2i and 2i+1
 */

int tree[64]; /* block status: 0=free, 1=alloc, 2=split */
int data[64]; /* the actual "memory" */

int level_size(int level) {
    /* level 0 = 64, level 1 = 32, level 2 = 16, level 3 = 8, level 4 = 4 */
    if (level == 0) return 64;
    if (level == 1) return 32;
    if (level == 2) return 16;
    if (level == 3) return 8;
    return 4;
}

int node_offset(int node, int level) {
    /* Calculate the memory offset for a given tree node at a given level */
    int size = level_size(level);
    int first_at_level = 1;
    int i;
    for (i = 0; i < level; i++) {
        first_at_level = first_at_level * 2;
    }
    return (node - first_at_level) * size;
}

void buddy_init(void) {
    int i;
    for (i = 0; i < 64; i++) {
        tree[i] = 0;
        data[i] = 0;
    }
}

/* Returns the tree node index allocated, or -1 */
int buddy_alloc_helper(int node, int cur_level, int target_level) {
    if (node >= 64) return -1;
    if (tree[node] == 1) return -1; /* already allocated */

    if (cur_level == target_level) {
        if (tree[node] == 0) {
            tree[node] = 1;
            return node;
        }
        return -1;
    }

    /* Need to go deeper - split if free */
    if (tree[node] == 0) {
        tree[node] = 2; /* split */
        /* Children are free by default */
    }

    /* Try left child first */
    int left = 2 * node;
    int right = 2 * node + 1;
    int result = buddy_alloc_helper(left, cur_level + 1, target_level);
    if (result != -1) return result;
    return buddy_alloc_helper(right, cur_level + 1, target_level);
}

/* Returns offset in data array, or -1 */
int buddy_alloc(int size) {
    int target;
    if (size <= 4) target = 4;
    else if (size <= 8) target = 3;
    else if (size <= 16) target = 2;
    else if (size <= 32) target = 1;
    else target = 0;

    int node = buddy_alloc_helper(1, 0, target);
    if (node == -1) return -1;
    return node_offset(node, target);
}

void buddy_free_helper(int node) {
    if (node < 1 || node >= 64) return;
    tree[node] = 0;

    /* Try to merge with buddy */
    if (node > 1) {
        int buddy;
        if (node % 2 == 0) buddy = node + 1;
        else buddy = node - 1;

        if (tree[buddy] == 0) {
            /* Merge - parent becomes free */
            int parent = node / 2;
            tree[parent] = 0;
            /* Recursively try to merge parent */
            buddy_free_helper(parent);
            /* Re-mark parent as free (buddy_free_helper sets it) */
        }
    }
}

void buddy_free_at(int node) {
    if (node < 1 || node >= 64) return;
    buddy_free_helper(node);
}

void data_write(int offset, int val) {
    if (offset >= 0 && offset < 64) data[offset] = val;
}

int data_read(int offset) {
    if (offset >= 0 && offset < 64) return data[offset];
    return -1;
}

int count_allocated(void) {
    int count = 0;
    int i;
    for (i = 1; i < 64; i++) {
        if (tree[i] == 1) count = count + 1;
    }
    return count;
}

void test_basic(void) {
    printf("=== Buddy Basic ===\n"); // EXPECT: === Buddy Basic ===
    buddy_init();

    int a = buddy_alloc(4);
    printf("alloc 4: offset=%d\n", a); // EXPECT: alloc 4: offset=0

    int b = buddy_alloc(4);
    printf("alloc 4: offset=%d\n", b); // EXPECT: alloc 4: offset=4

    int c = buddy_alloc(8);
    printf("alloc 8: offset=%d\n", c); // EXPECT: alloc 8: offset=8

    data_write(a, 100);
    data_write(b, 200);
    data_write(c, 300);
    printf("values: %d %d %d\n", data_read(a), data_read(b), data_read(c)); // EXPECT: values: 100 200 300

    printf("allocated nodes: %d\n", count_allocated()); // EXPECT: allocated nodes: 3
}

void test_large_alloc(void) {
    printf("=== Large Allocation ===\n"); // EXPECT: === Large Allocation ===
    buddy_init();

    int a = buddy_alloc(32);
    printf("alloc 32: offset=%d\n", a); // EXPECT: alloc 32: offset=0

    int b = buddy_alloc(16);
    printf("alloc 16: offset=%d\n", b); // EXPECT: alloc 16: offset=32

    int c = buddy_alloc(16);
    printf("alloc 16: offset=%d\n", c); // EXPECT: alloc 16: offset=48

    int d = buddy_alloc(4);
    printf("alloc 4 (should fail): offset=%d\n", d); // EXPECT: alloc 4 (should fail): offset=-1
}

void test_free_merge(void) {
    printf("=== Free and Merge ===\n"); // EXPECT: === Free and Merge ===
    buddy_init();

    /* Allocate two size-4 blocks that are buddies (nodes 16 and 17) */
    int a = buddy_alloc(4); /* node 16, offset 0 */
    int b = buddy_alloc(4); /* node 17, offset 4 */
    printf("alloc a=%d b=%d\n", a, b); // EXPECT: alloc a=0 b=4

    /* Free both - they should merge */
    buddy_free_at(16);
    buddy_free_at(17);

    /* Now we should be able to allocate size 8 at offset 0 */
    int c = buddy_alloc(8);
    printf("after merge, alloc 8: offset=%d\n", c); // EXPECT: after merge, alloc 8: offset=0
}

void test_multiple_sizes(void) {
    printf("=== Multiple Sizes ===\n"); // EXPECT: === Multiple Sizes ===
    buddy_init();

    int a4 = buddy_alloc(4);
    int b8 = buddy_alloc(8);
    int c16 = buddy_alloc(16);
    int d4 = buddy_alloc(4);

    printf("4:%d 8:%d 16:%d 4:%d\n", a4, b8, c16, d4); // EXPECT: 4:0 8:8 16:16 4:4

    /* Write and verify */
    data_write(a4, 1);
    data_write(a4 + 1, 2);
    data_write(b8, 10);
    data_write(c16, 20);
    data_write(d4, 30);

    int sum = data_read(a4) + data_read(a4 + 1) + data_read(b8) + data_read(c16) + data_read(d4);
    printf("sum=%d\n", sum); // EXPECT: sum=63
    printf("allocated nodes: %d\n", count_allocated()); // EXPECT: allocated nodes: 4
}

int main(void) {
    test_basic();
    test_large_alloc();
    test_free_merge();
    test_multiple_sizes();
    printf("DONE\n"); // EXPECT: DONE
    return 0;
}
