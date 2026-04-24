int printf(const char *fmt, ...);

// Buddy Allocator Simulation
// Simulates a buddy system allocator with splitting and coalescing

// Memory pool: 1024 bytes, minimum block 32 bytes
// Tree representation: index 0 = root (1024), children of i = 2i+1, 2i+2
// Levels: 0=1024, 1=512, 2=256, 3=128, 4=64, 5=32

// Block states: 0=free, 1=split, 2=allocated
int tree[63]; // 6 levels: 1+2+4+8+16+32 = 63 nodes

void buddy_init() {
    int i;
    for (i = 0; i < 63; i++) {
        tree[i] = 0;
    }
}

int level_size(int level) {
    if (level == 0) return 1024;
    if (level == 1) return 512;
    if (level == 2) return 256;
    if (level == 3) return 128;
    if (level == 4) return 64;
    if (level == 5) return 32;
    return 0;
}

int size_to_level(int size) {
    if (size <= 32) return 5;
    if (size <= 64) return 4;
    if (size <= 128) return 3;
    if (size <= 256) return 2;
    if (size <= 512) return 1;
    if (size <= 1024) return 0;
    return -1;
}

// Try to allocate at node index, target_level is desired level
int buddy_alloc_node(int idx, int cur_level, int target_level) {
    if (idx >= 63) return -1;
    if (tree[idx] == 2) return -1; // already allocated

    if (cur_level == target_level) {
        if (tree[idx] == 0) {
            tree[idx] = 2;
            return idx;
        }
        return -1;
    }

    // Need to go deeper - split if free
    if (tree[idx] == 0) {
        tree[idx] = 1; // mark as split
    }

    // Try left child first
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    int result;

    result = buddy_alloc_node(left, cur_level + 1, target_level);
    if (result >= 0) return result;

    result = buddy_alloc_node(right, cur_level + 1, target_level);
    if (result >= 0) return result;

    return -1;
}

int buddy_alloc(int size) {
    int level;
    level = size_to_level(size);
    if (level < 0) return -1;
    return buddy_alloc_node(0, 0, level);
}

void buddy_free(int idx) {
    if (idx < 0 || idx >= 63) return;
    tree[idx] = 0;

    // Try to coalesce with buddy
    if (idx == 0) return;

    int buddy;
    int parent;
    if (idx % 2 == 1) {
        buddy = idx + 1;
        parent = (idx - 1) / 2;
    } else {
        buddy = idx - 1;
        parent = (idx - 2) / 2;
    }

    if (buddy < 63 && tree[buddy] == 0) {
        tree[parent] = 0; // coalesce
        buddy_free(parent); // recursive coalesce up
    }
}

int count_free_at_level(int level) {
    int start;
    int count_nodes;
    int i;
    int free_count;

    if (level == 0) { start = 0; count_nodes = 1; }
    else if (level == 1) { start = 1; count_nodes = 2; }
    else if (level == 2) { start = 3; count_nodes = 4; }
    else if (level == 3) { start = 7; count_nodes = 8; }
    else if (level == 4) { start = 15; count_nodes = 16; }
    else { start = 31; count_nodes = 32; }

    free_count = 0;
    for (i = start; i < start + count_nodes; i++) {
        if (tree[i] == 0) free_count = free_count + 1;
    }
    return free_count;
}

int main() {
    int a;
    int b;
    int c;

    buddy_init();
    printf("Buddy Allocator Simulation\n");
    // EXPECT: Buddy Allocator Simulation
    printf("Pool size: %d bytes\n", 1024);
    // EXPECT: Pool size: 1024 bytes

    // Allocate 32 bytes -> level 5
    a = buddy_alloc(32);
    printf("Alloc 32B: node %d\n", a);
    // EXPECT: Alloc 32B: node 31

    // Allocate another 32 bytes
    b = buddy_alloc(32);
    printf("Alloc 32B: node %d\n", b);
    // EXPECT: Alloc 32B: node 32

    // Allocate 128 bytes -> level 3
    c = buddy_alloc(128);
    printf("Alloc 128B: node %d\n", c);
    // EXPECT: Alloc 128B: node 8

    // Check tree state at root
    printf("Root state: %d\n", tree[0]);
    // EXPECT: Root state: 1

    // Free the 32-byte blocks
    buddy_free(a);
    printf("Freed node %d\n", a);
    // EXPECT: Freed node 31
    buddy_free(b);
    printf("Freed node %d\n", b);
    // EXPECT: Freed node 32

    // After freeing both buddies, they should coalesce
    printf("Parent of freed nodes: %d\n", tree[15]);
    // EXPECT: Parent of freed nodes: 0

    // Free the 128 byte block
    buddy_free(c);
    printf("Freed node %d\n", c);
    // EXPECT: Freed node 8

    // After freeing everything, root should be free
    printf("Root after all free: %d\n", tree[0]);
    // EXPECT: Root after all free: 0

    // Multiple allocations
    buddy_init();
    int d;
    int e;
    d = buddy_alloc(64);
    e = buddy_alloc(64);
    printf("Alloc 64B: node %d\n", d);
    // EXPECT: Alloc 64B: node 15
    printf("Alloc 64B: node %d\n", e);
    // EXPECT: Alloc 64B: node 16

    // Level stats
    int free5;
    free5 = count_free_at_level(5);
    printf("Free at level 5: %d\n", free5);
    // EXPECT: Free at level 5: 32

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
