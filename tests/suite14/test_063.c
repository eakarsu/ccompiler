int printf(const char *fmt, ...);

// Buddy system allocator simulation
// Pool size is 256 (2^8). Minimum block is 16 (2^4).
// Levels: 0=256, 1=128, 2=64, 3=32, 4=16

// Each node in the buddy tree: 0=free, 1=split, 2=allocated
// Tree has 31 nodes (2^5 - 1) for 5 levels
struct BuddyAlloc {
    int tree[31];
    int pool_size;
};

void buddy_init(struct BuddyAlloc *b) {
    int i;
    b->pool_size = 256;
    for (i = 0; i < 31; i++) {
        b->tree[i] = 0;
    }
}

int left_child(int i) { return 2 * i + 1; }
int right_child(int i) { return 2 * i + 2; }
int parent_of(int i) { return (i - 1) / 2; }

int block_size_at_level(int level) {
    int s = 256;
    int i;
    for (i = 0; i < level; i++) s = s / 2;
    return s;
}

int size_to_level(int size) {
    // Find the level where block size >= requested size
    int level = 0;
    int bs = 256;
    while (bs / 2 >= size && level < 4) {
        bs = bs / 2;
        level++;
    }
    return level;
}

// Returns the offset within the pool, or -1 if failed
int buddy_alloc_helper(struct BuddyAlloc *b, int node, int level, int target_level) {
    if (level == target_level) {
        if (b->tree[node] == 0) {
            b->tree[node] = 2; // allocated
            // Compute offset: depends on node position at its level
            int nodes_before = 0;
            int first_at_level = 1;
            int i;
            for (i = 0; i < level; i++) {
                first_at_level = first_at_level * 2;
            }
            first_at_level = first_at_level - 1; // first node index at this level
            int pos = node - first_at_level;
            return pos * block_size_at_level(level);
        }
        return -1;
    }

    if (b->tree[node] == 2) return -1; // node is fully allocated

    if (b->tree[node] == 0) {
        // Split this node
        b->tree[node] = 1;
    }

    // Try left child first
    int result = buddy_alloc_helper(b, left_child(node), level + 1, target_level);
    if (result != -1) return result;

    // Try right child
    return buddy_alloc_helper(b, right_child(node), level + 1, target_level);
}

int buddy_alloc(struct BuddyAlloc *b, int size) {
    int level = size_to_level(size);
    return buddy_alloc_helper(b, 0, 0, level);
}

void buddy_try_merge(struct BuddyAlloc *b, int node) {
    if (node == 0) return;
    int p = parent_of(node);
    int l = left_child(p);
    int r = right_child(p);
    if (b->tree[l] == 0 && b->tree[r] == 0) {
        b->tree[p] = 0; // merge
        buddy_try_merge(b, p);
    }
}

int buddy_free_helper(struct BuddyAlloc *b, int node, int level, int target_level, int offset) {
    if (level == target_level) {
        int first_at_level = 1;
        int i;
        for (i = 0; i < level; i++) first_at_level = first_at_level * 2;
        first_at_level = first_at_level - 1;
        int pos = node - first_at_level;
        int expected_offset = pos * block_size_at_level(level);
        if (expected_offset == offset && b->tree[node] == 2) {
            b->tree[node] = 0;
            buddy_try_merge(b, node);
            return 1;
        }
        return 0;
    }

    if (b->tree[node] != 1) return 0; // not split, can't recurse

    int bs = block_size_at_level(level + 1);
    int first_at_next = 1;
    int i;
    for (i = 0; i < level + 1; i++) first_at_next = first_at_next * 2;
    first_at_next = first_at_next - 1;

    int mid_offset_of_right = (right_child(node) - first_at_next) * bs;

    if (offset < mid_offset_of_right) {
        return buddy_free_helper(b, left_child(node), level + 1, target_level, offset);
    } else {
        return buddy_free_helper(b, right_child(node), level + 1, target_level, offset);
    }
}

void buddy_free(struct BuddyAlloc *b, int offset, int size) {
    int level = size_to_level(size);
    buddy_free_helper(b, 0, 0, level, offset);
}

int count_allocated(struct BuddyAlloc *b) {
    int c = 0;
    int i;
    for (i = 0; i < 31; i++) {
        if (b->tree[i] == 2) c++;
    }
    return c;
}

int main(void) {
    struct BuddyAlloc ba;
    buddy_init(&ba);

    printf("size_to_level(256)=%d\n", size_to_level(256));
    // EXPECT: size_to_level(256)=0
    printf("size_to_level(128)=%d\n", size_to_level(128));
    // EXPECT: size_to_level(128)=1
    printf("size_to_level(64)=%d\n", size_to_level(64));
    // EXPECT: size_to_level(64)=2
    printf("size_to_level(32)=%d\n", size_to_level(32));
    // EXPECT: size_to_level(32)=3
    printf("size_to_level(16)=%d\n", size_to_level(16));
    // EXPECT: size_to_level(16)=4
    printf("size_to_level(20)=%d\n", size_to_level(20));
    // EXPECT: size_to_level(20)=3

    // Allocate 64-byte block
    int a1 = buddy_alloc(&ba, 64);
    printf("alloc 64 -> %d\n", a1);
    // EXPECT: alloc 64 -> 0

    // Allocate another 64-byte block
    int a2 = buddy_alloc(&ba, 64);
    printf("alloc 64 -> %d\n", a2);
    // EXPECT: alloc 64 -> 64

    // Allocate 128-byte block
    int a3 = buddy_alloc(&ba, 128);
    printf("alloc 128 -> %d\n", a3);
    // EXPECT: alloc 128 -> 128

    printf("allocated nodes: %d\n", count_allocated(&ba));
    // EXPECT: allocated nodes: 3

    // Free first 64-byte block
    buddy_free(&ba, 0, 64);
    printf("freed offset 0 size 64\n");
    // EXPECT: freed offset 0 size 64

    // Allocate 32-byte block - should go to offset 0
    int a4 = buddy_alloc(&ba, 32);
    printf("alloc 32 -> %d\n", a4);
    // EXPECT: alloc 32 -> 0

    // Allocate another 32-byte - should go to offset 32
    int a5 = buddy_alloc(&ba, 32);
    printf("alloc 32 -> %d\n", a5);
    // EXPECT: alloc 32 -> 32

    printf("allocated nodes: %d\n", count_allocated(&ba));
    // EXPECT: allocated nodes: 4

    // Free both 32s and the 64 - should merge back
    buddy_free(&ba, 0, 32);
    buddy_free(&ba, 32, 32);
    buddy_free(&ba, 64, 64);
    buddy_free(&ba, 128, 128);

    printf("after all free: allocated=%d tree[0]=%d\n",
           count_allocated(&ba), ba.tree[0]);
    // EXPECT: after all free: allocated=0 tree[0]=0

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
