int printf(const char *fmt, ...);
// EXPECT: Inorder: 1 3 4 5 6 7 8 10\nSize: 8\nRoot key: 5 (pri=90)\nSearch 4: 1\nSearch 9: 0\nSearch 10: 1\n1st element: 1\n4th element: 5\n8th element: 10\nDone

// Treap simulation with array-based nodes and deterministic priorities

struct TreapNode {
    int key;
    int priority;
    int left;
    int right;
    int subtree_size;
    int valid;
};

struct Treap {
    struct TreapNode nodes[32];
    int root;
    int next_id;
};

void treap_init(struct Treap *t) {
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

int treap_new_node(struct Treap *t, int key, int priority) {
    int id = t->next_id;
    t->next_id = t->next_id + 1;
    t->nodes[id].key = key;
    t->nodes[id].priority = priority;
    t->nodes[id].left = -1;
    t->nodes[id].right = -1;
    t->nodes[id].subtree_size = 1;
    t->nodes[id].valid = 1;
    return id;
}

int treap_size(struct Treap *t, int node) {
    if (node < 0) return 0;
    return t->nodes[node].subtree_size;
}

void treap_update_size(struct Treap *t, int node) {
    if (node >= 0) {
        t->nodes[node].subtree_size = 1 +
            treap_size(t, t->nodes[node].left) +
            treap_size(t, t->nodes[node].right);
    }
}

int treap_rotate_right(struct Treap *t, int node) {
    int left = t->nodes[node].left;
    t->nodes[node].left = t->nodes[left].right;
    t->nodes[left].right = node;
    treap_update_size(t, node);
    treap_update_size(t, left);
    return left;
}

int treap_rotate_left(struct Treap *t, int node) {
    int right = t->nodes[node].right;
    t->nodes[node].right = t->nodes[right].left;
    t->nodes[right].left = node;
    treap_update_size(t, node);
    treap_update_size(t, right);
    return right;
}

int treap_insert(struct Treap *t, int node, int key, int priority) {
    if (node < 0) {
        return treap_new_node(t, key, priority);
    }
    if (key < t->nodes[node].key) {
        t->nodes[node].left = treap_insert(t, t->nodes[node].left, key, priority);
        if (t->nodes[t->nodes[node].left].priority > t->nodes[node].priority) {
            node = treap_rotate_right(t, node);
        }
    } else {
        t->nodes[node].right = treap_insert(t, t->nodes[node].right, key, priority);
        if (t->nodes[t->nodes[node].right].priority > t->nodes[node].priority) {
            node = treap_rotate_left(t, node);
        }
    }
    treap_update_size(t, node);
    return node;
}

int treap_search(struct Treap *t, int node, int key) {
    if (node < 0) return 0;
    if (key == t->nodes[node].key) return 1;
    if (key < t->nodes[node].key)
        return treap_search(t, t->nodes[node].left, key);
    return treap_search(t, t->nodes[node].right, key);
}

void treap_inorder(struct Treap *t, int node) {
    if (node < 0) return;
    treap_inorder(t, t->nodes[node].left);
    printf(" %d", t->nodes[node].key);
    treap_inorder(t, t->nodes[node].right);
}

int treap_kth(struct Treap *t, int node, int k) {
    int left_size = treap_size(t, t->nodes[node].left);
    if (k == left_size + 1) return t->nodes[node].key;
    if (k <= left_size)
        return treap_kth(t, t->nodes[node].left, k);
    return treap_kth(t, t->nodes[node].right, k - left_size - 1);
}

int main(void) {
    struct Treap t;
    treap_init(&t);

    // Insert with deterministic priorities
    int keys[8];
    int pris[8];
    keys[0] = 5;  pris[0] = 90;
    keys[1] = 3;  pris[1] = 70;
    keys[2] = 8;  pris[2] = 80;
    keys[3] = 1;  pris[3] = 50;
    keys[4] = 4;  pris[4] = 60;
    keys[5] = 7;  pris[5] = 40;
    keys[6] = 10; pris[6] = 30;
    keys[7] = 6;  pris[7] = 75;

    int i = 0;
    while (i < 8) {
        t.root = treap_insert(&t, t.root, keys[i], pris[i]);
        i = i + 1;
    }

    printf("Inorder:");
    treap_inorder(&t, t.root);
    printf("\n");

    printf("Size: %d\n", treap_size(&t, t.root));
    printf("Root key: %d (pri=%d)\n", t.nodes[t.root].key, t.nodes[t.root].priority);

    printf("Search 4: %d\n", treap_search(&t, t.root, 4));
    printf("Search 9: %d\n", treap_search(&t, t.root, 9));
    printf("Search 10: %d\n", treap_search(&t, t.root, 10));

    printf("1st element: %d\n", treap_kth(&t, t.root, 1));
    printf("4th element: %d\n", treap_kth(&t, t.root, 4));
    printf("8th element: %d\n", treap_kth(&t, t.root, 8));

    printf("Done\n");
    return 0;
}
