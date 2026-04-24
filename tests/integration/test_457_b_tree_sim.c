int printf(const char *fmt, ...);
// EXPECT: Inorder: 3 5 8 10 12 15 18 20 25 30\nSearch 15: 1\nSearch 25: 1\nSearch 7: 0\nSearch 30: 1\nNodes used: 4\nDone

// B-tree simulation with arrays (order 3)

struct BTreeNode {
    int keys[3];
    int children[4];
    int num_keys;
    int is_leaf;
};

struct BTree {
    struct BTreeNode nodes[32];
    int root;
    int next_node;
};

int bt_new_node(struct BTree *bt, int is_leaf) {
    int id = bt->next_node;
    bt->next_node = bt->next_node + 1;
    bt->nodes[id].num_keys = 0;
    bt->nodes[id].is_leaf = is_leaf;
    int i = 0;
    while (i < 4) {
        bt->nodes[id].children[i] = -1;
        i = i + 1;
    }
    return id;
}

void bt_init(struct BTree *bt) {
    bt->next_node = 0;
    bt->root = bt_new_node(bt, 1);
}

int bt_search(struct BTree *bt, int node, int key) {
    if (node < 0) return 0;
    int i = 0;
    while (i < bt->nodes[node].num_keys) {
        if (key == bt->nodes[node].keys[i]) return 1;
        if (key < bt->nodes[node].keys[i]) {
            return bt_search(bt, bt->nodes[node].children[i], key);
        }
        i = i + 1;
    }
    return bt_search(bt, bt->nodes[node].children[i], key);
}

void bt_insert_nonfull(struct BTree *bt, int node, int key);
void bt_split_child(struct BTree *bt, int parent, int idx);

void bt_split_child(struct BTree *bt, int parent, int idx) {
    int child = bt->nodes[parent].children[idx];
    int new_node = bt_new_node(bt, bt->nodes[child].is_leaf);
    int mid = 1;

    bt->nodes[new_node].keys[0] = bt->nodes[child].keys[2];
    bt->nodes[new_node].num_keys = 1;
    if (!bt->nodes[child].is_leaf) {
        bt->nodes[new_node].children[0] = bt->nodes[child].children[2];
        bt->nodes[new_node].children[1] = bt->nodes[child].children[3];
    }

    int promote = bt->nodes[child].keys[mid];
    bt->nodes[child].num_keys = 1;

    int i = bt->nodes[parent].num_keys;
    while (i > idx) {
        bt->nodes[parent].keys[i] = bt->nodes[parent].keys[i - 1];
        bt->nodes[parent].children[i + 1] = bt->nodes[parent].children[i];
        i = i - 1;
    }
    bt->nodes[parent].keys[idx] = promote;
    bt->nodes[parent].children[idx + 1] = new_node;
    bt->nodes[parent].num_keys = bt->nodes[parent].num_keys + 1;
}

void bt_insert_nonfull(struct BTree *bt, int node, int key) {
    int i = bt->nodes[node].num_keys - 1;
    if (bt->nodes[node].is_leaf) {
        while (i >= 0 && key < bt->nodes[node].keys[i]) {
            bt->nodes[node].keys[i + 1] = bt->nodes[node].keys[i];
            i = i - 1;
        }
        bt->nodes[node].keys[i + 1] = key;
        bt->nodes[node].num_keys = bt->nodes[node].num_keys + 1;
    } else {
        while (i >= 0 && key < bt->nodes[node].keys[i]) {
            i = i - 1;
        }
        i = i + 1;
        if (bt->nodes[bt->nodes[node].children[i]].num_keys == 3) {
            bt_split_child(bt, node, i);
            if (key > bt->nodes[node].keys[i]) {
                i = i + 1;
            }
        }
        bt_insert_nonfull(bt, bt->nodes[node].children[i], key);
    }
}

void bt_insert(struct BTree *bt, int key) {
    if (bt->nodes[bt->root].num_keys == 3) {
        int new_root = bt_new_node(bt, 0);
        bt->nodes[new_root].children[0] = bt->root;
        bt->root = new_root;
        bt_split_child(bt, new_root, 0);
        bt_insert_nonfull(bt, new_root, key);
    } else {
        bt_insert_nonfull(bt, bt->root, key);
    }
}

void bt_inorder(struct BTree *bt, int node) {
    if (node < 0) return;
    int i = 0;
    while (i < bt->nodes[node].num_keys) {
        if (!bt->nodes[node].is_leaf) {
            bt_inorder(bt, bt->nodes[node].children[i]);
        }
        printf(" %d", bt->nodes[node].keys[i]);
        i = i + 1;
    }
    if (!bt->nodes[node].is_leaf) {
        bt_inorder(bt, bt->nodes[node].children[i]);
    }
}

int main(void) {
    struct BTree bt;
    bt_init(&bt);

    int vals[10];
    vals[0] = 10; vals[1] = 20; vals[2] = 5;
    vals[3] = 15; vals[4] = 25; vals[5] = 30;
    vals[6] = 3;  vals[7] = 8;  vals[8] = 12;
    vals[9] = 18;

    int i = 0;
    while (i < 10) {
        bt_insert(&bt, vals[i]);
        i = i + 1;
    }

    printf("Inorder:");
    bt_inorder(&bt, bt.root);
    printf("\n");

    printf("Search 15: %d\n", bt_search(&bt, bt.root, 15));
    printf("Search 25: %d\n", bt_search(&bt, bt.root, 25));
    printf("Search 7: %d\n", bt_search(&bt, bt.root, 7));
    printf("Search 30: %d\n", bt_search(&bt, bt.root, 30));

    printf("Nodes used: %d\n", bt.next_node);
    printf("Done\n");
    return 0;
}
