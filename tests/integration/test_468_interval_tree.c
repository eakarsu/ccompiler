int printf(const char *fmt, ...);
// EXPECT: Intervals: [5,20] [10,30] [12,15] [15,20] [17,19] [30,40]\nOverlap [14,16]: 1\nOverlap [21,25]: 1\nOverlap [41,50]: 0\nAll overlapping [14,16]:\n  [15,20]\n  [10,30]\n  [5,20]\n  [12,15]\nCount: 4\nAll overlapping [25,35]:\n  [10,30]\n  [30,40]\nCount: 2\nDone

// Interval tree operations using array-based BST

struct Interval {
    int low;
    int high;
};

struct ITNode {
    struct Interval interval;
    int max_high;
    int left;
    int right;
    int valid;
};

struct IntervalTree {
    struct ITNode nodes[32];
    int root;
    int next_id;
};

void it_init(struct IntervalTree *t) {
    t->root = -1;
    t->next_id = 0;
    int i = 0;
    while (i < 32) {
        t->nodes[i].valid = 0;
        t->nodes[i].left = -1;
        t->nodes[i].right = -1;
        i = i + 1;
    }
}

int it_max(int a, int b) {
    return a > b ? a : b;
}

int it_get_max(struct IntervalTree *t, int node) {
    if (node < 0) return -999999;
    return t->nodes[node].max_high;
}

int it_insert(struct IntervalTree *t, int node, int low, int high) {
    if (node < 0) {
        int id = t->next_id;
        t->next_id = t->next_id + 1;
        t->nodes[id].interval.low = low;
        t->nodes[id].interval.high = high;
        t->nodes[id].max_high = high;
        t->nodes[id].left = -1;
        t->nodes[id].right = -1;
        t->nodes[id].valid = 1;
        return id;
    }
    if (low < t->nodes[node].interval.low) {
        t->nodes[node].left = it_insert(t, t->nodes[node].left, low, high);
    } else {
        t->nodes[node].right = it_insert(t, t->nodes[node].right, low, high);
    }
    t->nodes[node].max_high = it_max(t->nodes[node].max_high,
        it_max(it_get_max(t, t->nodes[node].left),
               it_get_max(t, t->nodes[node].right)));
    if (high > t->nodes[node].max_high)
        t->nodes[node].max_high = high;
    return node;
}

int it_overlaps(int l1, int h1, int l2, int h2) {
    return l1 <= h2 && l2 <= h1;
}

int it_search_overlap(struct IntervalTree *t, int node, int low, int high) {
    if (node < 0) return 0;
    if (it_overlaps(t->nodes[node].interval.low, t->nodes[node].interval.high, low, high)) {
        return 1;
    }
    if (t->nodes[node].left >= 0 && it_get_max(t, t->nodes[node].left) >= low) {
        if (it_search_overlap(t, t->nodes[node].left, low, high)) return 1;
    }
    return it_search_overlap(t, t->nodes[node].right, low, high);
}

void it_find_all_overlaps(struct IntervalTree *t, int node, int low, int high, int *count) {
    if (node < 0) return;
    if (it_overlaps(t->nodes[node].interval.low, t->nodes[node].interval.high, low, high)) {
        printf("  [%d,%d]\n", t->nodes[node].interval.low, t->nodes[node].interval.high);
        *count = *count + 1;
    }
    if (t->nodes[node].left >= 0 && it_get_max(t, t->nodes[node].left) >= low) {
        it_find_all_overlaps(t, t->nodes[node].left, low, high, count);
    }
    it_find_all_overlaps(t, t->nodes[node].right, low, high, count);
}

void it_inorder(struct IntervalTree *t, int node) {
    if (node < 0) return;
    it_inorder(t, t->nodes[node].left);
    printf(" [%d,%d]", t->nodes[node].interval.low, t->nodes[node].interval.high);
    it_inorder(t, t->nodes[node].right);
}

int main(void) {
    struct IntervalTree tree;
    it_init(&tree);

    tree.root = it_insert(&tree, tree.root, 15, 20);
    tree.root = it_insert(&tree, tree.root, 10, 30);
    tree.root = it_insert(&tree, tree.root, 17, 19);
    tree.root = it_insert(&tree, tree.root, 5, 20);
    tree.root = it_insert(&tree, tree.root, 12, 15);
    tree.root = it_insert(&tree, tree.root, 30, 40);

    printf("Intervals:");
    it_inorder(&tree, tree.root);
    printf("\n");

    printf("Overlap [14,16]: %d\n", it_search_overlap(&tree, tree.root, 14, 16));
    printf("Overlap [21,25]: %d\n", it_search_overlap(&tree, tree.root, 21, 25));
    printf("Overlap [41,50]: %d\n", it_search_overlap(&tree, tree.root, 41, 50));

    int count = 0;
    printf("All overlapping [14,16]:\n");
    it_find_all_overlaps(&tree, tree.root, 14, 16, &count);
    printf("Count: %d\n", count);

    count = 0;
    printf("All overlapping [25,35]:\n");
    it_find_all_overlaps(&tree, tree.root, 25, 35, &count);
    printf("Count: %d\n", count);

    printf("Done\n");
    return 0;
}
