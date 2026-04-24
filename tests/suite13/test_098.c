int printf(const char *fmt, ...);

// Simplified Huffman-style frequency tree builder
// Given character frequencies, builds a binary tree by repeatedly
// combining two lowest-frequency nodes, without using malloc.
// Uses array-based node pool and array-based min-finding.

struct HuffNode {
    int freq;
    int ch;        // character value (-1 for internal nodes)
    int left;      // index, 0 = none
    int right;     // index, 0 = none
    int active;    // 1 if still in the working set
};

struct HuffTree {
    struct HuffNode nodes[64];
    int count;     // next free index (0 = sentinel)
    int num_active; // number of active nodes
};

void huff_init(struct HuffTree *ht) {
    int i;
    for (i = 0; i < 64; i++) {
        ht->nodes[i].freq = 0;
        ht->nodes[i].ch = -1;
        ht->nodes[i].left = 0;
        ht->nodes[i].right = 0;
        ht->nodes[i].active = 0;
    }
    ht->count = 1;
    ht->num_active = 0;
}

int huff_add_leaf(struct HuffTree *ht, int ch, int freq) {
    int idx = ht->count;
    ht->count = ht->count + 1;
    ht->nodes[idx].freq = freq;
    ht->nodes[idx].ch = ch;
    ht->nodes[idx].left = 0;
    ht->nodes[idx].right = 0;
    ht->nodes[idx].active = 1;
    ht->num_active = ht->num_active + 1;
    return idx;
}

// Find the index of the active node with minimum frequency
int huff_find_min(struct HuffTree *ht) {
    int min_idx = 0;
    int min_freq = 999999;
    int i;
    for (i = 1; i < ht->count; i++) {
        if (ht->nodes[i].active && ht->nodes[i].freq < min_freq) {
            min_freq = ht->nodes[i].freq;
            min_idx = i;
        }
    }
    return min_idx;
}

// Build Huffman tree: repeatedly merge two lowest-freq nodes
int huff_build(struct HuffTree *ht) {
    int a;
    int b;
    int new_idx;

    while (ht->num_active > 1) {
        // Find first minimum
        a = huff_find_min(ht);
        ht->nodes[a].active = 0;
        ht->num_active = ht->num_active - 1;

        // Find second minimum
        b = huff_find_min(ht);
        ht->nodes[b].active = 0;
        ht->num_active = ht->num_active - 1;

        // Create internal node
        new_idx = ht->count;
        ht->count = ht->count + 1;
        ht->nodes[new_idx].freq = ht->nodes[a].freq + ht->nodes[b].freq;
        ht->nodes[new_idx].ch = -1;
        ht->nodes[new_idx].left = a;
        ht->nodes[new_idx].right = b;
        ht->nodes[new_idx].active = 1;
        ht->num_active = ht->num_active + 1;
    }

    // Return root (the only active node)
    return huff_find_min(ht);
}

// Compute depth of each leaf and print its code length
// code_buf stores 0/1 for left/right path
void huff_print_codes(struct HuffTree *ht, int node, int depth) {
    if (node == 0) return;
    if (ht->nodes[node].ch != -1) {
        // Leaf node
        printf("  char %d freq %d depth %d\n", ht->nodes[node].ch,
               ht->nodes[node].freq, depth);
        return;
    }
    huff_print_codes(ht, ht->nodes[node].left, depth + 1);
    huff_print_codes(ht, ht->nodes[node].right, depth + 1);
}

// Calculate weighted path length (sum of freq * depth for all leaves)
int weighted_path_length(struct HuffTree *ht, int node, int depth) {
    if (node == 0) return 0;
    if (ht->nodes[node].ch != -1) {
        return ht->nodes[node].freq * depth;
    }
    return weighted_path_length(ht, ht->nodes[node].left, depth + 1) +
           weighted_path_length(ht, ht->nodes[node].right, depth + 1);
}

// Count tree height
int huff_height(struct HuffTree *ht, int node) {
    int lh;
    int rh;
    if (node == 0) return 0;
    lh = huff_height(ht, ht->nodes[node].left);
    rh = huff_height(ht, ht->nodes[node].right);
    if (lh > rh) return lh + 1;
    return rh + 1;
}

// Count leaves
int huff_count_leaves(struct HuffTree *ht, int node) {
    if (node == 0) return 0;
    if (ht->nodes[node].ch != -1) return 1;
    return huff_count_leaves(ht, ht->nodes[node].left) +
           huff_count_leaves(ht, ht->nodes[node].right);
}

// Count internal nodes
int huff_count_internal(struct HuffTree *ht, int node) {
    if (node == 0) return 0;
    if (ht->nodes[node].ch != -1) return 0;
    return 1 + huff_count_internal(ht, ht->nodes[node].left) +
               huff_count_internal(ht, ht->nodes[node].right);
}

int main() {
    struct HuffTree ht;
    int root;

    // Test 1: Simple frequency set
    // Characters a=1, b=2, c=3, d=4, e=5
    // Using integers 1-5 as char codes
    huff_init(&ht);
    huff_add_leaf(&ht, 1, 5);   // a: freq 5
    huff_add_leaf(&ht, 2, 9);   // b: freq 9
    huff_add_leaf(&ht, 3, 12);  // c: freq 12
    huff_add_leaf(&ht, 4, 13);  // d: freq 13
    huff_add_leaf(&ht, 5, 16);  // e: freq 16
    huff_add_leaf(&ht, 6, 45);  // f: freq 45

    root = huff_build(&ht);

    printf("Root freq: %d\n", ht.nodes[root].freq);
    // EXPECT: Root freq: 100

    printf("Height: %d\n", huff_height(&ht, root));
    // EXPECT: Height: 5

    printf("Leaves: %d\n", huff_count_leaves(&ht, root));
    // EXPECT: Leaves: 6

    printf("Internal nodes: %d\n", huff_count_internal(&ht, root));
    // EXPECT: Internal nodes: 5

    int wpl = weighted_path_length(&ht, root, 0);
    printf("Weighted path length: %d\n", wpl);
    // EXPECT: Weighted path length: 224

    printf("Codes:\n");
    // EXPECT: Codes:
    huff_print_codes(&ht, root, 0);
    // EXPECT:   char 6 freq 45 depth 1
    // EXPECT:   char 3 freq 12 depth 3
    // EXPECT:   char 4 freq 13 depth 3
    // EXPECT:   char 1 freq 5 depth 4
    // EXPECT:   char 2 freq 9 depth 4
    // EXPECT:   char 5 freq 16 depth 3

    // Test 2: All equal frequencies
    struct HuffTree ht2;
    huff_init(&ht2);
    huff_add_leaf(&ht2, 1, 10);
    huff_add_leaf(&ht2, 2, 10);
    huff_add_leaf(&ht2, 3, 10);
    huff_add_leaf(&ht2, 4, 10);

    root = huff_build(&ht2);
    printf("Equal freq root: %d\n", ht2.nodes[root].freq);
    // EXPECT: Equal freq root: 40

    printf("Equal freq leaves: %d\n", huff_count_leaves(&ht2, root));
    // EXPECT: Equal freq leaves: 4

    printf("Equal freq height: %d\n", huff_height(&ht2, root));
    // EXPECT: Equal freq height: 3

    int wpl2 = weighted_path_length(&ht2, root, 0);
    printf("Equal freq WPL: %d\n", wpl2);
    // EXPECT: Equal freq WPL: 80

    // Test 3: Two characters only
    struct HuffTree ht3;
    huff_init(&ht3);
    huff_add_leaf(&ht3, 1, 30);
    huff_add_leaf(&ht3, 2, 70);

    root = huff_build(&ht3);
    printf("Two char root: %d\n", ht3.nodes[root].freq);
    // EXPECT: Two char root: 100
    printf("Two char height: %d\n", huff_height(&ht3, root));
    // EXPECT: Two char height: 2
    printf("Two char WPL: %d\n", weighted_path_length(&ht3, root, 0));
    // EXPECT: Two char WPL: 100

    return 0;
}
