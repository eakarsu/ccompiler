int printf(const char *fmt, ...);

// Order Statistics Tree (k-th smallest element)
// Implemented as augmented BST with subtree sizes

struct OSTNode {
    int key;
    int left;    // index or -1
    int right;   // index or -1
    int sz;      // subtree size including self
};

struct OSTree {
    struct OSTNode nodes[20];
    int num_nodes;
    int root;
};

void ost_init(struct OSTree *t) {
    t->num_nodes = 0;
    t->root = -1;
}

int ost_alloc(struct OSTree *t, int key) {
    int idx;
    idx = t->num_nodes;
    t->nodes[idx].key = key;
    t->nodes[idx].left = -1;
    t->nodes[idx].right = -1;
    t->nodes[idx].sz = 1;
    t->num_nodes = t->num_nodes + 1;
    return idx;
}

int ost_size(struct OSTree *t, int node) {
    if (node == -1) return 0;
    return t->nodes[node].sz;
}

void ost_update_size(struct OSTree *t, int node) {
    if (node == -1) return;
    t->nodes[node].sz = 1 + ost_size(t, t->nodes[node].left)
                           + ost_size(t, t->nodes[node].right);
}

int ost_insert_rec(struct OSTree *t, int node, int key) {
    int new_node;
    if (node == -1) {
        return ost_alloc(t, key);
    }
    if (key < t->nodes[node].key) {
        t->nodes[node].left = ost_insert_rec(t, t->nodes[node].left, key);
    } else {
        t->nodes[node].right = ost_insert_rec(t, t->nodes[node].right, key);
    }
    ost_update_size(t, node);
    return node;
}

void ost_insert(struct OSTree *t, int key) {
    t->root = ost_insert_rec(t, t->root, key);
}

// Find k-th smallest (1-indexed)
int ost_kth(struct OSTree *t, int node, int k) {
    int left_size;
    if (node == -1) return -1;
    left_size = ost_size(t, t->nodes[node].left);
    if (k <= left_size) {
        return ost_kth(t, t->nodes[node].left, k);
    } else if (k == left_size + 1) {
        return t->nodes[node].key;
    } else {
        return ost_kth(t, t->nodes[node].right, k - left_size - 1);
    }
}

// Find rank of a key (1-indexed, returns 0 if not found)
int ost_rank(struct OSTree *t, int node, int key) {
    int left_size;
    int right_rank;
    if (node == -1) return 0;
    left_size = ost_size(t, t->nodes[node].left);
    if (key < t->nodes[node].key) {
        return ost_rank(t, t->nodes[node].left, key);
    } else if (key == t->nodes[node].key) {
        return left_size + 1;
    } else {
        right_rank = ost_rank(t, t->nodes[node].right, key);
        if (right_rank == 0) return 0;
        return left_size + 1 + right_rank;
    }
}

// Count elements less than key
int ost_count_less(struct OSTree *t, int node, int key) {
    int left_size;
    if (node == -1) return 0;
    left_size = ost_size(t, t->nodes[node].left);
    if (key <= t->nodes[node].key) {
        return ost_count_less(t, t->nodes[node].left, key);
    } else {
        return left_size + 1 + ost_count_less(t, t->nodes[node].right, key);
    }
}

// Find minimum
int ost_min(struct OSTree *t, int node) {
    if (node == -1) return -1;
    if (t->nodes[node].left == -1) return t->nodes[node].key;
    return ost_min(t, t->nodes[node].left);
}

// Find maximum
int ost_max(struct OSTree *t, int node) {
    if (node == -1) return -1;
    if (t->nodes[node].right == -1) return t->nodes[node].key;
    return ost_max(t, t->nodes[node].right);
}

// In-order traversal to verify sorted order
void ost_inorder(struct OSTree *t, int node, int *arr, int *idx) {
    if (node == -1) return;
    ost_inorder(t, t->nodes[node].left, arr, idx);
    arr[*idx] = t->nodes[node].key;
    *idx = *idx + 1;
    ost_inorder(t, t->nodes[node].right, arr, idx);
}

int main() {
    struct OSTree t;
    int sorted[20];
    int idx;
    int i;

    ost_init(&t);

    // Insert values: 50, 30, 70, 20, 40, 60, 80, 10, 35
    ost_insert(&t, 50);
    ost_insert(&t, 30);
    ost_insert(&t, 70);
    ost_insert(&t, 20);
    ost_insert(&t, 40);
    ost_insert(&t, 60);
    ost_insert(&t, 80);
    ost_insert(&t, 10);
    ost_insert(&t, 35);

    printf("Tree size: %d\n", ost_size(&t, t.root));
    // EXPECT: Tree size: 9

    printf("Min: %d\n", ost_min(&t, t.root));
    // EXPECT: Min: 10

    printf("Max: %d\n", ost_max(&t, t.root));
    // EXPECT: Max: 80

    // k-th smallest: sorted = [10, 20, 30, 35, 40, 50, 60, 70, 80]
    printf("1st smallest: %d\n", ost_kth(&t, t.root, 1));
    // EXPECT: 1st smallest: 10

    printf("2nd smallest: %d\n", ost_kth(&t, t.root, 2));
    // EXPECT: 2nd smallest: 20

    printf("3rd smallest: %d\n", ost_kth(&t, t.root, 3));
    // EXPECT: 3rd smallest: 30

    printf("5th smallest: %d\n", ost_kth(&t, t.root, 5));
    // EXPECT: 5th smallest: 40

    printf("9th smallest: %d\n", ost_kth(&t, t.root, 9));
    // EXPECT: 9th smallest: 80

    // Rank queries
    printf("Rank of 10: %d\n", ost_rank(&t, t.root, 10));
    // EXPECT: Rank of 10: 1

    printf("Rank of 50: %d\n", ost_rank(&t, t.root, 50));
    // EXPECT: Rank of 50: 6

    printf("Rank of 80: %d\n", ost_rank(&t, t.root, 80));
    // EXPECT: Rank of 80: 9

    printf("Rank of 35: %d\n", ost_rank(&t, t.root, 35));
    // EXPECT: Rank of 35: 4

    printf("Rank of 99: %d\n", ost_rank(&t, t.root, 99));
    // EXPECT: Rank of 99: 0

    // Count less than
    printf("Count < 50: %d\n", ost_count_less(&t, t.root, 50));
    // EXPECT: Count < 50: 5

    printf("Count < 10: %d\n", ost_count_less(&t, t.root, 10));
    // EXPECT: Count < 10: 0

    printf("Count < 100: %d\n", ost_count_less(&t, t.root, 100));
    // EXPECT: Count < 100: 9

    printf("Count < 35: %d\n", ost_count_less(&t, t.root, 35));
    // EXPECT: Count < 35: 3

    // Verify in-order traversal
    idx = 0;
    ost_inorder(&t, t.root, sorted, &idx);
    printf("Sorted[0]: %d\n", sorted[0]);
    // EXPECT: Sorted[0]: 10
    printf("Sorted[4]: %d\n", sorted[4]);
    // EXPECT: Sorted[4]: 40
    printf("Sorted[8]: %d\n", sorted[8]);
    // EXPECT: Sorted[8]: 80

    // Insert duplicates and check
    ost_insert(&t, 50);
    printf("Size after dup: %d\n", ost_size(&t, t.root));
    // EXPECT: Size after dup: 10

    printf("Rank of 50 after dup: %d\n", ost_rank(&t, t.root, 50));
    // EXPECT: Rank of 50 after dup: 6

    printf("7th smallest after dup: %d\n", ost_kth(&t, t.root, 7));
    // EXPECT: 7th smallest after dup: 50

    return 0;
}
